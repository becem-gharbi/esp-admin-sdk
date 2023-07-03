#include "Update.h"

namespace ESPAdmin
{
    UpdateMessage Update::_message;
    Logger Update::_logger("Update");

    void Update::checkAndUpdate(UpdateMessage message)
    {
        _message = message;

        String currentReleaseId = Store::get(STORE_UPDATE_RELEASE_ID);
        String currentVersion = Store::get(STORE_UPDATE_VERSION);

        if (currentReleaseId == _message.releaseId && currentVersion == _message.version)
        {
            _logger.info("already updated");
        }
        else
        {
            _logger.info("Update to release " + _message.releaseId + " version " + _message.version);
            OTA::start(message.downloadURL);
        }
    }

    void Update::onChange(UpdateStatus status)
    {
        switch (status)
        {
        case UPDATE_STARTED:
            _onStart();
            break;
        case UPDATE_SUCCEDED:
            _onSuccess();
            break;
        case UPDATE_FAILED:
            _onFail();
            break;
        }
    }

    void Update::_onSuccess()
    {
        _logger.success("succeded");
        Report::sendUpdate(_message, "succeded");

        Store::set(STORE_UPDATE_RELEASE_ID, _message.releaseId);
        Store::set(STORE_UPDATE_VERSION, _message.version);

        esp_restart();
    }

    void Update::_onFail()
    {
        _logger.error("failed");

        Report::sendUpdate(_message, "failed");
    }

    void Update::_onStart()
    {
        _logger.info("started");

        Report::sendUpdate(_message, "started");
    }
}
