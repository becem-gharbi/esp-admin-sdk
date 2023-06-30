#include <Arduino.h>
#include <WiFi.h>
#include "ESPAdmin.h"
#include <ArduinoJson.h>

const char *ssid = "ASUR_AP";
const char *password = "pass1234";

ESPAdmin::Logger logger("app");

void setup()
{
  Serial.begin(115200);
  Serial.printf("\n");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    logger.info("WiFi wait");
    delay(1000);
  }

  logger.success("WiFi connected");

  ESPAdmin::begin();

  ESPAdmin::Update::checkAndUpdate(
      {.downloadURL = "https://esp-ota-cicd.s3.us-east-005.backblazeb2.com/becem-gharbi/esp-ota-cicd/v0.1.2.bin",
       .releaseId = "id"});
}

void loop()
{
  String content;
  StaticJsonDocument<96> doc;

  doc["title"] = "foo";
  doc["body"] = "bar";
  doc["userId"] = 1;

  serializeJson(doc, content);

  String res = ESPAdmin::HTTP::post("/posts", content, "application/json");

  logger.info(res);

  delay(10000);
}
