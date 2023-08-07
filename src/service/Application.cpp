//
// Created by Ivan Kishchenko on 04/08/2023.
//

#include "Application.h"
#include "system/WifiService.h"
#include "system/MqttService.h"

void Application::systemInfo() {
    esp_log_i("sys", "Version: %s", system_get_sdk_version());
    esp_log_i("sys", "CPU Freq: %d", system_get_cpu_freq());
    esp_log_i("sys", "Flash Size: %d", system_get_flash_size_map());
    esp_log_i("sys", "Free Heap Size: %d", system_get_free_heap_size());
}

void Application::setup() {
    esp_log_i("app", "setup...");
    systemInfo();

    getRegistry().create<WifiService>();
    getRegistry().create<MqttService>();

    std::sort(getRegistry().getServices().begin(), getRegistry().getServices().end(), [](auto f, auto s) {
        return f->getServiceId() < s->getServiceId();
    });

    for (auto service: getRegistry().getServices()) {
        if (service) {
            service->setup();
            esp_log_i("app", "setup: %d", service->getServiceId());
        }
    }

    esp_log_i("app", "setup done");
}

void Application::loop() {
    getRegistry().getMessageBus().loop();
    for (auto service: getRegistry().getServices()) {
        if (service) {
            service->loop();
        }
    }
}
