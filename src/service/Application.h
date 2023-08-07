//
// Created by Kishchenko, Ivan on 11/11/21.
//

#pragma once

#include "message/MessageBus.h"
#include "Registry.h"

class Application {
    Registry _reg;
private:
    static void systemInfo();
public:
    Registry& getRegistry() {
        return _reg;
    }

    explicit Application() {
        Serial.begin(76800);
    }

    virtual void setup();

    virtual void loop();
};