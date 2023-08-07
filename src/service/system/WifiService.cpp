//
// Created by Ivan Kishchenko on 04/08/2023.
//

#include "WifiService.h"
#include "service/Registry.h"

WifiService::WifiService(Registry &registry) : TService(registry) {
}

void WifiService::setup() {
    _fnOnWiFiConnected = WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected&) {
        esp_log_i("wifi", "Connected, waiting IP");
        //getRegistry()->getMessageBus().postMessage(WifiConnected{});
    });
    _fnOnWiFiConnected = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& msg) {
        esp_log_i("wifi", "Got IP: %s", msg.ip.toString().c_str());
        esp_log_i("wifi", "MAC: %s", WiFi.macAddress().c_str());
        getRegistry().getMessageBus().postMessage(WifiConnected{});
    });
    _fnOnWiFiDisconnected = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& msg) {
        esp_log_i("wifi", "Disconnected, code: %d", (int)msg.reason);
        getRegistry().getMessageBus().postMessage(WifiDisconnected {});
    });
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin("A12A-08", "66778899");
}
