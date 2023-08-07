//
// Created by Ivan Kishchenko on 07.11.2021.
//

#pragma once

#include "logging/Logger.h"
#include "Message.h"
#include "MessageSubscriber.h"
#include "MessageProducer.h"

#include "SystemMessage.h"

#include <set>
#include <vector>

#ifdef ESP8266

#include <ets_sys.h>

#define BUS_TASK_PRIORITY 10
#else
#include <freertos/queue.h>
#include "system/Timer.h"
#endif

class MessageBus : public MessageSubscriber, public MessageProducer {
public:
    inline void subscribe(MessageSubscriber &subscriber) {
        subscribe(&subscriber);
    }

    virtual void subscribe(MessageSubscriber *subscriber) = 0;

    virtual void loop() = 0;
};

template<size_t queueSize = 10, size_t timerSize = 10>
class TMessageBus : public MessageBus {
    struct MessageHolder {
        std::shared_ptr<Message> msg;
    };
#ifdef ESP8266
#else
    std::array<Timer, timerSize> _timers;
    QueueHandle_t _queue;
#endif
private:
    std::vector<MessageSubscriber *> _subscribers;
#ifdef ESP8266
    std::array<ETSEvent, queueSize> _queue;

    static void task(ETSEvent *e) {
        auto *bus = (TMessageBus *) e->sig;
        esp_log_d("bus", "task: %d:%d", e->sig, e->par);
        if ((e->par & 0xFFFF0000) != 0xFFFF0000) {
            auto *holder = (MessageHolder *) e->par;
            bus->onMessage(*holder->msg.get());
            delete holder;
        } else {
            const uint16_t msgId = e->par & 0xFF, sysId = (e->par & 0xFF00) >> 8;
            switch (sysId) {
                case SYS_CORE_SERVICE:
                    bus->onSystemMessage(sysId, msgId);
                    break;
                default:
                    break;
            }
        }
    }

#endif

    void onSystemMessage(uint8_t systemId, uint8_t msgId) {
        if (systemId == SYS_CORE_SERVICE) {
            switch (msgId) {
                case SYS_MSG_WIFI_CONNECTED:
                    onMessage(TMessage<SYS_MSG_WIFI_CONNECTED, SYS_CORE_SERVICE>());
                    break;
                case SYS_MSG_WIFI_DISCONNECTED:
                    onMessage(TMessage<SYS_MSG_WIFI_DISCONNECTED, SYS_CORE_SERVICE>());
                    break;
                case SYS_MSG_MQTT_CONNECTED:
                    onMessage(TMessage<SYS_MSG_MQTT_CONNECTED, SYS_CORE_SERVICE>());
                    break;
                case SYS_MSG_MQTT_DISCONNECTED:
                    onMessage(TMessage<SYS_MSG_MQTT_DISCONNECTED, SYS_CORE_SERVICE>());
                    break;
                default:
                    break;
            }
        }
    }

public:

    TMessageBus() {
#ifdef ESP8266
        ets_task(task, BUS_TASK_PRIORITY, _queue.data(), _queue.size());
#else
        _queue = xQueueCreate(queueSize, sizeof(void *));
#endif
    }

    void subscribe(MessageSubscriber *subscriber) override {
        _subscribers.emplace_back(subscriber);
    }

    void onMessage(const Message &msg) override {
        esp_log_d("bus", "on msg: %d:%d", msg.getMsgId(), _subscribers.size());
        for (const auto sub: _subscribers) {
            sub->onMessage(msg);
        }
    }

    void loop() override {
#ifdef ESP8266
#else
        if (_queue) {
            MessageHolder *holder = nullptr;
            while (pdPASS == xQueueReceive(_queue, &holder, 0)) {
                msg::log::debug("recv msg: {}", holder->msg->getMsgId());
                sendMessage(*holder->msg.get());
                delete holder;
                holder = nullptr;
            }
        }
#endif
    }

    void sendMessage(const Message &msg) override {
        onMessage(msg);
    }

    void postMessage(const Message &msg) override {
        if (SYS_CORE_SERVICE == msg.getSystemId()) {
            ets_post(BUS_TASK_PRIORITY, (ETSSignal)this, 0xFFFF0000 | (((uint32_t) msg.getSystemId()) << 8) | msg.getMsgId());
        } else {
            esp_log_w("bus", "cannot post message - please use Message::Ptr");
        }
    }

    void postMessage(const Message::Ptr &msg) override {
        auto *holder = new MessageHolder{msg};
        ets_post(BUS_TASK_PRIORITY, (ETSSignal)this, (ETSParam) holder);
    }

    bool schedule(uint32_t delay, bool repeat, const std::function<void()> &callback) override {
//        for (auto &timer: _timers) {
//            if (timer.isActive()) {
//                continue;
//            }
//
//            timer.attach(delay, repeat, callback);
//            return true;
//        }

        return false;
    }

    virtual ~TMessageBus() = default;
};

inline static void sendMessage(MessageProducer &pub, const Message &msg) {
    pub.sendMessage(msg);
}

inline static void sendMessage(MessageProducer *pub, const Message &msg) {
    sendMessage(*pub, msg);
}

inline static bool schedule(MessageProducer &pub, uint32_t delay, bool repeat, const std::function<void()> &callback) {
    return pub.schedule(delay, repeat, callback);
}

inline static bool schedule(MessageProducer *pub, uint32_t delay, bool repeat, const std::function<void()> &callback) {
    return schedule(*pub, delay, repeat, callback);
}

inline static void sendMessage(MessageProducer &pub, const std::shared_ptr<Message> &msg) {
    pub.postMessage(msg);
}

inline static void sendMessage(MessageProducer *pub, const std::shared_ptr<Message> &msg) {
    sendMessage(*pub, msg);
}
