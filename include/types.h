#ifndef H_ESP_ADMIN_TYPES
#define H_ESP_ADMIN_TYPES

#include <Arduino.h>

namespace ESPAdmin
{
    struct UpdateMessage
    {
        String downloadURL;
        String version;
        String projectId;
    };

    enum DebugMessage
    {
        ON,
        OFF
    };

    enum StoreKey
    {
        STORE_CONFIG,
        STORE_HTTP_BASE_URL,
        STORE_HTTP_USERNAME,
        STORE_HTTP_PASSWORD,
        STORE_HTTP_CERT,
        STORE_MQTT_URI_TCP,
        STORE_MQTT_URI_WS,
        STORE_MQTT_CLIENT_ID,
        STORE_MQTT_USERNAME,
        STORE_MQTT_PASSWORD,
        STORE_MQTT_CERT,
        STORE_UPDATE_RELEASE_ID,
        STORE_UPDATE_CERT,
    };
}

#endif