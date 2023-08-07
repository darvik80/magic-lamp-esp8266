//
// Created by Ivan Kishchenko on 09.12.2021.
//

#pragma once

#include "message/Message.h"
#include <string_view>

enum SysMessage {
    SYS_MSG_WIFI_CONNECTED,
    SYS_MSG_WIFI_DISCONNECTED,
    SYS_MSG_MQTT_CONNECTED,
    SYS_MSG_MQTT_DISCONNECTED,
    SYS_MSG_MQTT_MESSAGE,
};

typedef TMessage<SYS_MSG_WIFI_CONNECTED, SYS_CORE_SERVICE> WifiConnected;
typedef TMessage<SYS_MSG_WIFI_DISCONNECTED, SYS_CORE_SERVICE> WifiDisconnected;
typedef TMessage<SYS_MSG_MQTT_CONNECTED, SYS_CORE_SERVICE> MqttConnected;
typedef TMessage<SYS_MSG_MQTT_CONNECTED, SYS_CORE_SERVICE> MqttReconnect;
typedef TMessage<SYS_MSG_MQTT_DISCONNECTED, SYS_CORE_SERVICE> MqttDisconnected;

struct MqttMessage : TMessage<SYS_MSG_MQTT_MESSAGE, SYS_CORE_SERVICE> {
    MqttMessage() = default;
    MqttMessage(std::string_view topic, std::string_view payload, int qos) : topic(topic), payload(payload),
                                                                                 qos(qos) {}


    std::string topic;
    std::string payload;
    int qos;
};