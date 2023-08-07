//
// Created by Ivan Kishchenko on 07.11.2021.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

typedef uint8_t MsgId;
typedef uint8_t SystemId;
typedef uint16_t Id;

enum SysService {
    SYS_CORE_SERVICE,
    SYS_USER_SERVICE,
};

struct Message {
public:
    typedef std::shared_ptr<Message> Ptr;

    [[nodiscard]] virtual MsgId getMsgId() const = 0;

    [[nodiscard]] virtual SystemId getSystemId() const = 0;

    virtual  ~Message() = default;
};

template<MsgId id, SystemId sysId = 0>
struct TMessage : Message {
public:
    enum {
        ID = id
    };

    [[nodiscard]] MsgId getMsgId() const override {
        return id ;
    }

    [[nodiscard]] SystemId getSystemId() const override {
        return sysId;
    }
};
