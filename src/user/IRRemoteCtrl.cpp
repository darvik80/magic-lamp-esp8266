//
// Created by Ivan Kishchenko on 06/08/2023.
//

#include "IRRemoteCtrl.h"
#include "logging/Logger.h"
#include "service/Registry.h"

IRRemoteCtrl::IRRemoteCtrl(Registry &registry) : TService(registry) {}

void IRRemoteCtrl::setup() {
    ir.enableIRIn();
}

void IRRemoteCtrl::loop() {
    uint32_t cur = millis();
    if ((cur - _lastUpdate) < 500) {
        return;
    }

    decode_results results{};
    if (ir.decode(&results)) {
        // print() & println() can't handle printing long longs. (uint64_t)
        esp_log_w("ir", "ir code: %0llx", results.value);
        ir.resume();
        uint64_t val = results.value;
        if (val == 0xFFFFFFFFFFFFFFFF) {
            val = _lastBtn;
        }
        _lastBtn = val;

        ButtonCode code = BTN_NONE;
        switch (results.value) {
            case 0xFF629D:
                code = BTN_UP;
                break;
            case 0xFFA857:
                code = BTN_DOWN;
                break;
            case 0xFF22DD:
                code = BTN_LEFT;
                break;
            case 0xFFC23D:
                code = BTN_RIGHT;
                break;
            case 0xFF4AB5:
                code = BTN_0;
                break;
            case 0xFF6897:
                code = BTN_1;
                break;
            case 0xFF9867:
                code = BTN_2;
                break;
            case 0xFFB04F:
                code = BTN_3;
                break;
            case 0xFF30CF:
                code = BTN_4;
                break;
            case 0xFF18E7:
                code = BTN_5;
                break;
            case 0xFF7A85:
                code = BTN_6;
                break;
            case 0xFF10EF:
                code = BTN_7;
                break;
            case 0xFF38C7:
                code = BTN_8;
                break;
            case 0xFF5AA5:
                code = BTN_9;
                break;
            case 0xFF02FD:
                code = BTN_OK;
                break;
            case 0xFF42BD:
                code = BTN_STAR;
                break;
            case 0xFF52AD:
                code = BTN_HASH;
                break;
        }

        getRegistry().getMessageBus().postMessage(std::make_shared<IRRemoteMessage>(code));
    }

    _lastUpdate = cur;
}
