//
// Created by Ivan Kishchenko on 07.11.2021.
//

#pragma once

#include "Message.h"

class MessageSubscriber {
public:
    virtual void onMessage(const Message &msg) = 0;
};

template<typename T, typename Msg1 = void, typename Msg2 = void, typename Msg3 = void, typename Msg4 = void>
class TMessageSubscriber : public MessageSubscriber {
public:
    void onMessage(const Message &msg) override {
        switch (msg.getMsgId()) {
            case Msg1::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg1 &>(msg));
                break;
            case Msg2::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg2 &>(msg));
                break;
            case Msg3::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg3 &>(msg));
                break;
            case Msg4::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg4 &>(msg));
                break;
            default:
                break;
        }
    }
};

template<typename T, typename Msg1, typename Msg2, typename Msg3>
class TMessageSubscriber<T, Msg1, Msg2, Msg3, void> : public MessageSubscriber {
public:
    void onMessage(const Message &msg) override {
        switch (msg.getMsgId()) {
            case Msg1::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg1 &>(msg));
                break;
            case Msg2::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg2 &>(msg));
                break;
            case Msg3::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg3 &>(msg));
                break;
            default:
                break;
        }
    }
};

template<typename T, typename Msg1, typename Msg2>
class TMessageSubscriber<T, Msg1, Msg2, void, void> : public MessageSubscriber {
public:
    void onMessage(const Message &msg) override {
        switch (msg.getMsgId()) {
            case Msg1::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg1 &>(msg));
                break;
            case Msg2::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg2 &>(msg));
                break;
            default:
                break;
        }
    }
};

template<typename T, typename Msg1>
class TMessageSubscriber<T, Msg1, void, void, void> : public MessageSubscriber {
public:
    void onMessage(const Message &msg) override {
        switch (msg.getMsgId()) {
            case Msg1::ID:
                static_cast<T *>(this)->onMessage(static_cast<const Msg1 &>(msg));
                break;
            default:
                break;
        }
    }
};

