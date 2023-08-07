//
// Created by Ivan Kishchenko on 04/08/2023.
//

#include "MqttService.h"

#define MQTT_HOST IPAddress(192, 168, 0, 3)
#define MQTT_PORT 1883

MqttService::MqttService(Registry &registry) : TService(registry) {
}

void MqttService::setup() {
    getRegistry().getMessageBus().subscribe(this);
    _mqttClient.onConnect([this](bool sessionPresent) {
        esp_log_d("mqtt", "Connected");
        getRegistry().getMessageBus().postMessage(MqttConnected{});
    });
    _mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
        esp_log_d("mqtt", "Disconnected, reason: %d", (int)reason);
        getRegistry().getMessageBus().postMessage(MqttDisconnected{});
        _reconn.once_ms(3000, [this]() {
            esp_log_d("mqtt", "Reconnect");
            getRegistry().getMessageBus().postMessage(MqttReconnect{});
        });
    });
    _mqttClient.onMessage(
            [this](auto topic, auto payload, auto props, size_t len, size_t, size_t) {
                esp_log_d("mqtt", "Msg: %s", topic);
                getRegistry().getMessageBus().postMessage(
                        std::make_shared<MqttMessage>(topic, std::string_view(payload, len), props.qos)
                );
            });
    _mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    _mqttClient.setCredentials("mqtt", "mqtt");
    _mqttClient.setCleanSession(true);
    _mqttClient.setKeepAlive(60);
    _mqttClient.setClientId("magic-lamp");
}

void MqttService::onMessage(const WifiConnected &msg) {
    _mqttClient.connect();
}

void MqttService::onMessage(const MqttReconnect& msg) {
    _mqttClient.connect();
}

void MqttService::subscribe(std::string_view topic) {
    _mqttClient.subscribe(topic.data(), 0);
}