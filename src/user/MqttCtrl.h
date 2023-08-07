//
// Created by Ivan Kishchenko on 06/08/2023.
//

#pragma once

#include "service/Registry.h"

struct Action : TMessage<11, SYS_USER_SERVICE> {
    Action() = default;
    explicit Action(uint8_t magicId) : magicId(magicId) {};

    uint8_t magicId{0};
};

class MqttCtrl : public TService<61>, public TMessageSubscriber<MqttCtrl, MqttConnected, MqttMessage> {
public:
    explicit MqttCtrl(Registry &registry);

    void setup();

    void onMessage(const MqttConnected& msg);
    void onMessage(const MqttMessage& msg);
};
