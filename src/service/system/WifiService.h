//
// Created by Ivan Kishchenko on 04/08/2023.
//

#pragma once

#include "service/Registry.h"
#include <ESP8266WiFi.h>

class WifiService : public TService<Service_WIFI> {
    WiFiEventHandler _fnOnWiFiConnected;
    WiFiEventHandler _fnOnWiFiDisconnected;
    WiFiEventHandler _fnOnWiFiGotIp;
public:
    explicit WifiService(Registry &registry);

    void setup() override;
};
