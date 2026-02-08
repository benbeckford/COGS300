#include "config.h"

#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/at_handler.cpp"
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/BossaUnoR4WiFI.cpp"
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/OTA.cpp"
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/parser.cpp"
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/ping.cpp"
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/server.cpp"
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/SSE.cpp"

// Quick fix for compiiler warnings
#define gpio_set_level(x, y) gpio_set_level((gpio_num_t)x, y)
#define gpio_set_direction(x, y) gpio_set_direction((gpio_num_t)x, y)
#define gpio_get_level(x) gpio_get_level((gpio_num_t)x)
#define pull_down_en pull_down_en = GPIO_PULLDOWN_DISABLE; int x
#define pull_up_en pull_up_en = GPIO_PULLUP_DISABLE; int y
#include "uno-r4-wifi-usb-bridge/UNOR4USBBridge/freedap.c"

#include "USBHID.h"
USBHID HID;

// Quick fix for https://github.com/arduino/uno-r4-wifi-usb-bridge/issues/52
#define USBSerial USBSerial_

#define IDENT(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define PATH(x,y) STR(IDENT(x)IDENT(y))

#define PATH_TO_INO /src/esp32/uno-r4-wifi-usb-bridge/UNOR4USBBridge/UNOR4USBBridge.ino

#define setup() firmware_setup()
#define loop() firmware_loop()
#include PATH(PATH_TO_REPO,PATH_TO_INO)
#undef setup()
#undef loop()
