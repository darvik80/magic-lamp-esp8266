#include <Arduino.h>
#include "service/Application.h"
#include "user/LedCircle.h"
#include "user/IRRemoteCtrl.h"
#include "user/MqttCtrl.h"

class MagicApp : public Application {
public:
    void setup() override {
        getRegistry().create<LedCircle<50, 00, 24>>();
        getRegistry().create<IRRemoteCtrl>();
        getRegistry().create<MqttCtrl>();
        Application::setup();
    }
};

MagicApp app;

void setup() {
    app.setup();
}

void loop() {
    app.loop();
}