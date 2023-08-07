//
// Created by Ivan Kishchenko on 04/08/2023.
//

#pragma once

#include "service/Registry.h"
#include <AsyncMqttClient.h>
#include <Ticker.h>

class MqttService : public TService<Service_MQTT>, public TMessageSubscriber<MqttService, WifiConnected, MqttReconnect> {
    AsyncMqttClient _mqttClient;
    Ticker _reconn;
public:
    explicit MqttService(Registry &registry);

    void setup() override;

    void onMessage(const WifiConnected& msg);
    void onMessage(const MqttReconnect& msg);

    void subscribe(std::string_view topic);
};
