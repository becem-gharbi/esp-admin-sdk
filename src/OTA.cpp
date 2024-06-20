#include "OTA.h"

namespace ESPAdmin
{
    Logger OTA::_logger("OTA");
    bool OTA::_aborted = false;
    esp_https_ota_handle_t OTA::_otaHandle;

    /**
     * Starts the OTA (Over-The-Air) update process for the device.
     *
     * @param downloadURL The URL from which to download the binary file.
     *
     * @return None.
     *
     * @throws None.
     */
    void OTA::start(const String &downloadURL)
    {
        esp_http_client_config_t httpConfig = {
            .url = downloadURL.c_str(),
            .cert_pem = Store::options.httpCert,
            .timeout_ms = Store::options.httpTimeoutMs,
        };

        esp_https_ota_config_t otaConfig = {
            .http_config = &httpConfig,
        };

        Store::updateRunning = true;
        Update::onChange(UPDATE_STARTED);

        esp_err_t err = esp_https_ota_begin(&otaConfig, &_otaHandle);

        if (err != ESP_OK)
        {
            Store::updateRunning = false;
            Update::onChange(UPDATE_FAILED);
        }
        else
        {
            xTaskCreatePinnedToCore(_task, "ota_start", Store::options.otaTaskStackSize, nullptr, Store::options.otaTaskPriority, nullptr, 1);
        }
    }

    /**
     * Abort OTA process.
     *
     * @param None.
     *
     * @return None.
     *
     * @throws None.
     */
    void OTA::abort()
    {
        _aborted = true;
    }

    void OTA::_task(void *)
    {
        int imageReadPrev = 0;

        Update::onProgress(0);

        while (Store::updateRunning)
        {
            if (_aborted)
            {
                esp_err_t err = esp_https_ota_abort(_otaHandle);

                if (err == ESP_OK)
                {
                    Store::updateRunning = false;
                    Update::onChange(UPDATE_FAILED);
                    break;
                }
            }

            esp_err_t err = esp_https_ota_perform(_otaHandle);

            int imageReadNow = esp_https_ota_get_image_len_read(_otaHandle);

            if (err == ESP_ERR_HTTPS_OTA_IN_PROGRESS)
            {
                if (imageReadNow - imageReadPrev > 30000) // progress state is updated every 30Kb
                {
                    Update::onProgress(imageReadNow);
                    imageReadPrev = imageReadNow;
                }

                continue;
            }

            Update::onProgress(imageReadNow);

            if (err == ESP_OK)
            {
                Store::updateRunning = false;

                bool completed = esp_https_ota_is_complete_data_received(_otaHandle);

                if (completed)
                {
                    Update::onChange(UPDATE_SUCCEDED);
                }
                else
                {
                    Update::onChange(UPDATE_FAILED);
                }
            }
            else
            {
                Store::updateRunning = false;
                Update::onChange(UPDATE_FAILED);
            }
        }

        if (_aborted == false)
        {
            esp_https_ota_finish(_otaHandle);
        }
        else
        {
            _aborted = false;
        }

        vTaskDelete(nullptr);
    }
}