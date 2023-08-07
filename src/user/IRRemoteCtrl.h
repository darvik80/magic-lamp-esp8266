//
// Created by Ivan Kishchenko on 06/08/2023.
//

#pragma once

#include "service/Registry.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

enum ButtonCode {
    BTN_NONE,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_UP,
    BTN_DOWN,
    BTN_0,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,
    BTN_8,
    BTN_9,
    BTN_OK,
    BTN_STAR,
    BTN_HASH,
};

struct IRRemoteMessage : TMessage<10, SYS_USER_SERVICE> {
    IRRemoteMessage() = default;
    explicit IRRemoteMessage(int button) : button(button) {}

    int button;
};

class IRRemoteCtrl : public TService<60> {
    IRrecv ir{14};
    uint64_t _lastBtn{0};
    uint32_t _lastUpdate{0};
public:
    explicit IRRemoteCtrl(Registry &registry);

    void setup() override;

    void loop() override;
};
