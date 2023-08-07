//
// Created by Ivan Kishchenko on 06/08/2023.
//

#include "MqttCtrl.h"
#include "service/system/MqttService.h"
#include <ArduinoJson.h>

#define MAGIC_TOPIC "/smart-home/magic-lamp-98cdac3282f4/action"

MqttCtrl::MqttCtrl(Registry &registry) : TService(registry) {

}

void MqttCtrl::setup() {
    getRegistry().getMessageBus().subscribe(this);
}

void MqttCtrl::onMessage(const MqttConnected &msg) {
    esp_log_d("ctrl", "Sub to: %s", MAGIC_TOPIC);
    auto mqtt = getRegistry().findService<MqttService>(Service_MQTT);
    if (mqtt) {
        mqtt->subscribe(MAGIC_TOPIC);
    } else {
        esp_log_d("ctrl", "can't find mqtt service!!!");
    }
}

void MqttCtrl::onMessage(const MqttMessage &msg) {
    if (msg.topic == MAGIC_TOPIC) {
        esp_log_d("ctrl", "Handle %s", MAGIC_TOPIC);
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, msg.payload);
        if (!error) {
            getRegistry().getMessageBus().postMessage(
                    std::make_shared<Action>((uint8_t) doc["magic-id"])
            );
        }
    }
}

