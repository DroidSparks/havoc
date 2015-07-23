#pragma once

#include <cinttypes>

enum class LedMode : uint8_t {
    OFF = 0,
    ON = 1,
    BREATHING = 2,
    // 3 - Always blue, breathing
    RAPID_FIRE = 4,
};

enum class LedBrightness : uint8_t {
    PERCENT_0 = 0x0,
    PERCENT_20 = 0x1,
    PERCENT_40 = 0x11,
    PERCENT_60 = 0x25,
    PERCENT_80 = 0x6C,
    PERCENT_100 = 0xFF,
};

enum class Color : uint8_t {
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    TEAL = 6,
    WHITE = 7,
};

struct ProfileSettings {
    LedMode led_mode;
    LedBrightness led_brightness;
    Color color;

    /**
     * This variable should be set between 0 and 1. This corresponds to between 250us and 32ms.
     * This is converted to between 0x0 and 0x80 to be sent to the hardware.
     */
    float button_response;
    bool angle_snapping;
};