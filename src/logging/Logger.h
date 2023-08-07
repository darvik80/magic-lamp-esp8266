//
// Created by Ivan Kishchenko on 03/08/2023.
//

#ifndef WEMOS_D1_MINI_PRO_LOGGER_H
#define WEMOS_D1_MINI_PRO_LOGGER_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <cstdint>

#define CORE_LOG_LEVEL_NONE       0
#define CORE_LOG_LEVEL_ERROR      1
#define CORE_LOG_LEVEL_WARN       2
#define CORE_LOG_LEVEL_INFO       3
#define CORE_LOG_LEVEL_DEBUG      4
#define CORE_LOG_LEVEL_VERBOSE    5

#define CORE_LOG_COLOR_BLACK   "30"
#define CORE_LOG_COLOR_RED     "31" //ERROR
#define CORE_LOG_COLOR_GREEN   "32" //INFO
#define CORE_LOG_COLOR_YELLOW  "33" //WARNING
#define CORE_LOG_COLOR_BLUE    "34"
#define CORE_LOG_COLOR_MAGENTA "35"
#define CORE_LOG_COLOR_CYAN    "36" //DEBUG
#define CORE_LOG_COLOR_GRAY    "37" //VERBOSE
#define CORE_LOG_COLOR_WHITE   "38"

#define CORE_LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define CORE_LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define CORE_LOG_RESET_COLOR   "\033[0m"

#define CORE_LOG_COLOR_E       CORE_LOG_COLOR(CORE_LOG_COLOR_RED)
#define CORE_LOG_COLOR_W       CORE_LOG_COLOR(CORE_LOG_COLOR_YELLOW)
#define CORE_LOG_COLOR_I       CORE_LOG_COLOR(CORE_LOG_COLOR_GREEN)
#define CORE_LOG_COLOR_D       CORE_LOG_COLOR(CORE_LOG_COLOR_CYAN)
#define CORE_LOG_COLOR_V      CORE_LOG_COLOR(CORE_LOG_COLOR_GRAY)
#define CORE_LOG_COLOR_PRINT(letter) log_printf(CORE_LOG_COLOR_ ## letter)
#define CORE_LOG_COLOR_PRINT_END log_printf(CORE_LOG_RESET_COLOR)

#define CORE_SHORT_LOG_FORMAT(letter, tag, format)  "[%06u]" CORE_LOG_COLOR_ ## letter " [" tag "] " format CORE_LOG_RESET_COLOR "\r\n", (uint32_t)millis()

#define APP_LOG_LEVEL 5

#if APP_LOG_LEVEL >= 5
#define esp_log_v(tag, format, ...) Serial.printf(CORE_SHORT_LOG_FORMAT(V, tag, format), ##__VA_ARGS__)
#else
#define esp_log_v(tag, format, ...)
#endif

#if APP_LOG_LEVEL >= 4
#define esp_log_d(tag, format, ...) Serial.printf(CORE_SHORT_LOG_FORMAT(D, tag, format), ##__VA_ARGS__)
#else
#define esp_log_d(tag, format, ...)
#endif

#if APP_LOG_LEVEL >= 3
#define esp_log_i(tag, format, ...) Serial.printf(CORE_SHORT_LOG_FORMAT(I, tag, format), ##__VA_ARGS__)
#else
#define esp_log_i(tag, format, ...)
#endif

#if APP_LOG_LEVEL >= 2
#define esp_log_w(tag, format, ...) Serial.printf(CORE_SHORT_LOG_FORMAT(W, tag, format), ##__VA_ARGS__)
#else
#define esp_log_w(tag, format, ...)
#endif

#if APP_LOG_LEVEL >= 1
#define esp_log_e(tag, format, ...) Serial.printf(CORE_SHORT_LOG_FORMAT(E, tag, format), ##__VA_ARGS__)
#else
#define esp_log_e(tag, format, ...)
#endif

#endif //WEMOS_D1_MINI_PRO_LOGGER_H
