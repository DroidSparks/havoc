#include "hardware-interface.h"

#include <cstdio>
#include <libusb-1.0/libusb.h>

#include "havoc.h"

static libusb_context* usb_context = nullptr;
static libusb_device_handle* havoc_device_handle = nullptr;

bool HW::Initialize() {
    int status = libusb_init(&usb_context);
    if (status < 0) {
        printf("`libusb_init` failed with err %i\n", status);
        return false;
    }
    libusb_set_debug(usb_context, 3); // Very verbose!

    // TODO: Support multiple mice
    havoc_device_handle = libusb_open_device_with_vid_pid(usb_context, 0x2516, 0x001d);
    if (havoc_device_handle == nullptr) {
        printf("`libusb_open_device_with_vid_pid` failed to open device for vid=0x2516, pid=0x001d\n");
        libusb_exit(usb_context);
        usb_context = nullptr;
        return false;
    }

    return true;
}

void HW::Deinitialize() {
    libusb_close(havoc_device_handle);
    havoc_device_handle = nullptr;

    libusb_exit(usb_context);
    usb_context = nullptr;
}

static uint64_t MakeSendSettingsCommand(uint8_t opcode, uint8_t data) {
    return ((uint64_t)0x1 << 24) | ((uint64_t)data << 16) | ((uint64_t)opcode << 8) | 0xDE;
}

bool HW::SendProfileSettings(const ProfileSettings& settings) {
    int status = 0;

    static const int num_cmds = 11;
    uint64_t data[num_cmds] = {
        0x00000000000106C4,
        MakeSendSettingsCommand(0x0B, ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF)),
        MakeSendSettingsCommand(0x0C, ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF)),
        MakeSendSettingsCommand(0x0D, ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF)),
        MakeSendSettingsCommand(0x46, 0xC0 | settings.angle_snapping),
        MakeSendSettingsCommand(0x4C, (uint8_t)settings.led_brightness),
        MakeSendSettingsCommand(0x4D, (uint8_t)settings.led_brightness),
        MakeSendSettingsCommand(0x4E, (uint8_t)settings.led_brightness),
        MakeSendSettingsCommand(0x50, settings.button_response * 0x80),
        0x00000000000000C4,
        0x00000000000006C4,
    };

    for (int i = 0; i < num_cmds; i++) {
        status = libusb_control_transfer(havoc_device_handle,
                                         LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                         9, 0x300, 0, (uint8_t*)(data + i), 8, 1000);
        if (status != 8 || status < 0) {
            printf("`libusb_control_transfer` failed with err %i\n", status);
            return false;
        }
    }
    return true;
}