//
// Created by Ivan Kishchenko on 04/08/2023.
//
#include "Registry.h"

void Registry::addService(Service *service) {
    _services.push_back(service);
}

std::vector<Service *> &Registry::getServices() {
    return _services;
}

MessageBus &Registry::getMessageBus() {
    return _bus;
}

Registry::~Registry() {
    while (_services.size()) {
        delete _services.back();
        _services.pop_back();
    }
}