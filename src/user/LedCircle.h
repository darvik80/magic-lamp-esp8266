//
// Created by Ivan Kishchenko on 23/07/2023.
//

#ifndef MAGIC_LAMP_ESP8266_D1_LEDCIRCLE_H
#define MAGIC_LAMP_ESP8266_D1_LEDCIRCLE_H


#include "service/Registry.h"
#include "Adafruit_NeoPixel.h"
#include "IRRemoteCtrl.h"
#include "MqttCtrl.h"

class MagicGenerator {
    uint32_t _lastUpdate{0};
    uint32_t _delay;
public:
    explicit MagicGenerator(uint32_t delay) : _delay(delay) {}

    virtual void loop(Adafruit_NeoPixel &pixels) {
        uint32_t cur = millis();
        if ((cur - _lastUpdate) < _delay) {
            return;
        }
        generate(pixels);
        _lastUpdate = cur;
    }

    virtual void generate(Adafruit_NeoPixel &pixels) = 0;
};

class RainbowGenerator : public MagicGenerator {
    size_t _offset{0};
    std::array<uint32_t, 4> _shape{};
public:
    RainbowGenerator() : MagicGenerator(1000) {
        _shape[0] = Adafruit_NeoPixel::Color(0xFF, 0x00, 0x00);
        _shape[1] = Adafruit_NeoPixel::Color(0x00, 0xFF, 0x00);
        _shape[2] = Adafruit_NeoPixel::Color(0x00, 0x00, 0xFF);
        _shape[3] = Adafruit_NeoPixel::Color(0xFF, 0xFF, 0xFF);
    }

    void generate(Adafruit_NeoPixel &pixels) override {
        auto frameOffset = _offset;
        for (size_t idx = 0; idx < pixels.numPixels(); idx++) {
            pixels.setPixelColor(idx, _shape[frameOffset]);
            if (++frameOffset >= _shape.size()) {
                frameOffset = 0;
            }
        }

        if (++_offset >= _shape.size()) _offset = 0;
        pixels.show();
    }
};

template<size_t size>
class SinGenerator : public MagicGenerator {
    size_t _offset{0};
    std::array<uint8_t, size> _shape;
public:
    SinGenerator() : MagicGenerator(500) {
        const double pi = std::acos(-1);
        for (size_t idx = 0; idx < _shape.size(); idx++) {
            _shape[idx] = (uint8_t) std::abs((std::sin(pi * 2 * idx / _shape.size()) * 255));
        }
    }

    void generate(Adafruit_NeoPixel &pixels) override {
        auto frameOffset = _offset;
        for (size_t idx = 0; idx < pixels.numPixels(); idx++) {
            auto val = _shape[frameOffset];
            auto b = 255;
            auto g = 0;
            if (val >= 196) g = 255;
            auto r = 0;
            if (val >= 164) r = 255;
            pixels.setPixelColor(idx, r, g, b, val);
            if (++frameOffset >= _shape.size()) {
                frameOffset = 0;
            }
        }

        if (++_offset >= _shape.size()) _offset = 0;
        pixels.show();
    }
};

class SimpleGenerator : public MagicGenerator {
    size_t _offset{0};
    std::array<uint32_t, 10> _shape{};
public:
    SimpleGenerator() : MagicGenerator(3000) {
        _shape[0] = Adafruit_NeoPixel::Color(0xFF, 0x00, 0x00);
        _shape[1] = Adafruit_NeoPixel::Color(0xFF, 0x1F, 0x00);
        _shape[2] = Adafruit_NeoPixel::Color(0x00, 0xFF, 0x00);
        _shape[3] = Adafruit_NeoPixel::Color(0x00, 0xFF, 0x1F);
        _shape[4] = Adafruit_NeoPixel::Color(0x00, 0x00, 0xFF);
        _shape[5] = Adafruit_NeoPixel::Color(0x1F, 0x00, 0x1F);
        _shape[6] = Adafruit_NeoPixel::Color(0x00, 0x1F, 0x1F);
        _shape[7] = Adafruit_NeoPixel::Color(0x1F, 0x1F, 0x00);
        _shape[8] = Adafruit_NeoPixel::Color(0x1F, 0x1F, 0x1F);
        _shape[9] = Adafruit_NeoPixel::Color(0xFF, 0xFF, 0xFF);
    }

    void generate(Adafruit_NeoPixel &pixels) override {
        auto color = _shape[_offset];
        for (size_t idx = 0; idx < pixels.numPixels(); idx++) {
            pixels.setPixelColor(idx, color);
        }

        if (++_offset >= _shape.size()) _offset = 0;
        pixels.show();
    }
};

class TriColorGenerator : public MagicGenerator {
    size_t _offset{0};
    std::vector<uint32_t> _shape{};
public:
    TriColorGenerator() : MagicGenerator(50) {
        _shape.push_back(Adafruit_NeoPixel::Color(0xFF, 0x00, 0x00));
        _shape.push_back(Adafruit_NeoPixel::Color(0x1F, 0x1F, 0x00));
        _shape.push_back(Adafruit_NeoPixel::Color(0x00, 0xFF, 0x00));
        _shape.push_back(Adafruit_NeoPixel::Color(0x00, 0x1F, 0x1F));
        _shape.push_back(Adafruit_NeoPixel::Color(0x00, 0x00, 0xFF));
        _shape.push_back(Adafruit_NeoPixel::Color(0x1F, 0x00, 0x1F));
    }

    void generate(Adafruit_NeoPixel &pixels) override {
        size_t block = (size_t) (std::round((double) pixels.numPixels() / _shape.size())), cur = 0;
        for (auto &color: _shape) {
            for (size_t idx = 0; idx < block; idx++) {
                size_t pos = cur + idx + _offset;
                if (pos >= pixels.numPixels()) pos -= pixels.numPixels();
                pixels.setPixelColor(pos, color);
            }
            cur += block;
        }

        if (++_offset >= pixels.numPixels()) _offset = 0;
        pixels.show();
    }
};

template<ServiceId sid, int16_t pin, int16_t count>
class LedCircle : public TService<sid>, public TMessageSubscriber<LedCircle<sid,pin,count>, IRRemoteMessage, Action> {
    Adafruit_NeoPixel pixels{count, pin};

    std::vector<MagicGenerator *> _generators;
    size_t _curGenerator{0};

public:
    explicit LedCircle(Registry &registry) : TService<sid>(registry) {}

    void setup() override {
        this->getRegistry().getMessageBus().subscribe(this);
        pixels.setBrightness(255);
        _generators.push_back(new RainbowGenerator());
        _generators.push_back(new SinGenerator<count>());
        _generators.push_back(new SimpleGenerator());
        _generators.push_back(new TriColorGenerator());
        pixels.begin();
        pixels.clear();
        pixels.show();
    }

    void loop() override {
        if (pixels.canShow()) {
            _generators[_curGenerator]->loop(pixels);
        }
    }

    void onMessage(const Action& msg) {
        if (msg.magicId < _generators.size()) {
            _curGenerator = msg.magicId;
        }
    }

    void onMessage(const IRRemoteMessage& msg) {
        size_t off = _curGenerator;
        if (msg.button == BTN_UP) {
            if (++off >= _generators.size()) off = 0;
        } else if (msg.button == BTN_DOWN) {
            if (--off >= _generators.size()) off = _generators.size()-1;
        } else if (msg.button >= BTN_0 && msg.button <= BTN_9) {
            off = msg.button - BTN_0;
            if (off >= _generators.size()) {
                off = _curGenerator;
            }
        }
        _curGenerator = off;
        esp_log_w("led", "handle IR: %d, cur: %d", msg.button, _curGenerator);
    }
};

#endif //MAGIC_LAMP_ESP8266_D1_LEDCIRCLE_H
