//
// Created by Ivan Kishchenko on 07.11.2021.
//

#pragma once

#include <utility>

#include <vector>
#include "message/MessageBus.h"

typedef uint_least8_t ServiceId;

enum SystemService {
    Service_WIFI,
    Service_MQTT,
};


class Registry;

class Service {
public:
    [[nodiscard]] virtual ServiceId getServiceId() const = 0;

    virtual Registry& getRegistry() = 0;

    virtual void setup() = 0;

    virtual void loop() = 0;

    virtual ~Service() = default;
};

class Registry {
    std::vector<Service *> _services;
    TMessageBus<> _bus;
public:
    virtual void addService(Service *service);

    virtual std::vector<Service *> &getServices();

    template<typename C, typename... T>
    C &create(T &&... all) {
        auto *service = new C(*this, std::forward<T>(all)...);
        addService(service);
        return *service;
    }

    template<typename C>
    C* findService(uint_least8_t id) {
        for (auto* service : _services) {
            if (service->getServiceId() == id) {
                return static_cast<C*>(service);
            }
        }

        return nullptr;
    }

    virtual MessageBus &getMessageBus();

    ~Registry();
};

template<ServiceId Id = 0>
class TService : public Service {
    Registry &_registry;
public:
    explicit TService(Registry &registry) : _registry(registry) {}

    [[nodiscard]] ServiceId getServiceId() const override {
        return Id;
    }

    Registry& getRegistry() override {
        return _registry;
    }

    void setup() override { }

    void loop() override { }
};

