# Elecrow-7-Inch-Display-WZ8048C070
Basic Programming of Elecrow 7 Inch Display ( WZ8048C070 Wizee-ESP33 ) with PlatformIO

The supplier claims compatibility with LVGL . They provide a demo application that is delivered with the display, and a [tutorial](https://www.elecrow.com/wiki/images/5/5b/Tutorial-WZ8048C070.pdf) on how to compile and upload it to the display. They are using the [GFX Library for Arduino](https://github.com/moononournation/Arduino_GFX) by moonournation, the [TAMC_GT911](https://github.com/TAMCTec/gt911-arduino) library for the touch screen and [the LVGL (Light and Versatile Graphics Library](https://lvgl.io/) for the GUI.
LVGL is certainly powerful, but requires quite a bit of design work using their "SquareLine" GUI designer tool

An simpler path, is to just use a graphics library and do the graphics yourself. I have done this, and am just using [Lovyan GFX](https://github.com/lovyan03/LovyanGFX). There is some english [Documentation](https://lovyangfx.readthedocs.io/en/master/), however not going into too much detail.

## Hardware
The Elecrow 7 Inch Display with an ESP32-S3 is an inexpensive platform for any kind of display within home automation. Unfortunately little good information about how to use it is available. This repository is an attempt to help other hobbyists to overcome he obstacles in using it.

## Hardware Details
- Model: 7.0 inch ESP32 display ( WZ8048C070 Wizee-ESP33 )
- Main Processor: ESP32-S3-WROOM-1-N4R2
- Resolution: 800*480
- Color Depth: 16M
- Touch Type: Resistive Touch Screen
- Touch Panel: TN Panel
- Screen: TFT-LCD Screen
- Display driver: EK9716BD3/EK73002ACGB
- External power supply: DC 5V-2A
- Interface: 1*TF Card Slot, 1* GPIO, 1*Speak, 1* UART1, 1*UART0
- Sound interface: One MAX98357A (Pins: LRCLK 18, I2S_BCLK 42, I2S_SDIN 17)

## Resources
[Generic Info by Elecrow](https://www.elecrow.com/esp32-display-7-inch-hmi-display-rgb-tft-lcd-touch-screen-support-lvgl.html)
[Elecrow Wiki for the 7" display](https://www.elecrow.com/wiki/index.php?title=ESP32_Display_7.0%27%27_Intelligent_Touch_Screen_Wi-Fi%26BLE_800*480_HMI_Display)

## Setup for Platformio - LovyanGFX only

The main steps for getting software using LovYanGFX onto the 7" display are:
- copy the board definition file *esp32-s3-devkitc-1-myboard.json* into your project and reference it in *platformio.ini*
- copy the board partition table file *huge_app.csv* into your project and reference it in *platformio.ini*
- copy the driver file *LGFX_ESP32S3_Elecrow70.hpp* into your project, and include it in the source code
- Modify platformio.ini as directed (or just copy the provided example file)
The example should then compile.

### Board definition file
The board definition file "esp32-s3-devkitc-1-myboard.json" needs to be put into the correct directory. This can be the platformio boards directory in *Users\<user>\.platformio\platforms\espressif32\boards\* or in any directory that the *boards_dir* , as defined in platformio.ini, points to. 
if using *boards_dir = .* it can be put into the same directory as *platformio.ini*
```
{
  "build": {
    "arduino":{
      "ldscript": "esp32s3_out.ld",
      "memory_type": "qspi_opi",
      "partitions": "huge_app.csv"
    },
    "core": "esp32",
    "extra_flags": [
      "-DBOARD_HAS_PSRAM",
      "-DARDUINO_USB_MODE=1",
      "-DARDUINO_RUNNING_CORE=1",
      "-DARDUINO_EVENT_RUNNING_CORE=1"
    ],
    "f_cpu": "240000000L",
    "f_flash": "80000000L",
    "flash_mode": "qio",
    "hwids": [
      [
        "0x303A",
        "0x1001"
      ]
    ],
    "mcu": "esp32s3",
    "variant": "esp32s3"
  },
  "connectivity": [
    "wifi"
  ],
  "debug": {
    "default_tool": "esp-builtin",
    "onboard_tools": [
      "esp-builtin"
    ],
    "openocd_target": "esp32s3.cfg"
  },
  "frameworks": [
    "arduino",
    "espidf"
  ],
  "name": "Espressif ESP32-S3-DevKitC-1-N8 -ELECROW",
  "upload": {
    "flash_size": "4MB",
    "maximum_ram_size": 327680,
    "maximum_size": 8388608,
    "require_upload_port": true,
    "speed": 921600
  },
  "url": "https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html",
  "vendor": "Espressif"
}
```
### Board partition table
Use the board partition table *huge_app.csv*, this is defined in platformio.ini via *board_build.partitions*
Simply put it into the same directory as *platformio.ini*
```
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x300000,
spiffs,   data, spiffs,  0x310000,0xE0000,
coredump, data, coredump,0x3F0000,0x10000,
```

### Driver file 
The file *LGFX_ESP32S3_Elecrow70.hpp* contains the proper pin assignements for LovyanGFX. 
This file must be included in the program
```
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

class LGFX : public lgfx::LGFX_Device
{
public:

  lgfx::Bus_RGB     _bus_instance;
  lgfx::Panel_RGB   _panel_instance;
  lgfx::Light_PWM   _light_instance;
  lgfx::Touch_GT911 _touch_instance;

  LGFX(void)
  {
    {
      auto cfg = _panel_instance.config();

      cfg.memory_width  = 800;
      cfg.memory_height = 480;
      cfg.panel_width  = 800;
      cfg.panel_height = 480;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config_detail();

      cfg.use_psram = 1;

      _panel_instance.config_detail(cfg);
    }

    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;
      // LCD panel configuration
      cfg.pin_d0  = GPIO_NUM_15;  // B0 | schematic: I15 - B3
      cfg.pin_d1  = GPIO_NUM_7;  // B1 | schematic: IO7 - B4
      cfg.pin_d2  = GPIO_NUM_6; // B2  | schematic: IO6 - B5
      cfg.pin_d3  = GPIO_NUM_5;  // B3 | schematic: IO5 - B6
      cfg.pin_d4  = GPIO_NUM_4;  // B4 | schematic: IO4 - B7
      cfg.pin_d5  = GPIO_NUM_9;  // G0 | schematic: IO9 - G2
      cfg.pin_d6  = GPIO_NUM_46;  // G1| schematic: I46 - G3
      cfg.pin_d7  = GPIO_NUM_3;  // G2 | schematic: IO3 - G4
      cfg.pin_d8  = GPIO_NUM_8; // G3  | schematic: IO8 - G5
      cfg.pin_d9  = GPIO_NUM_16; // G4 | schematic: I16 - G6
      cfg.pin_d10 = GPIO_NUM_1;  // G5 | schematic: IO1 - G7
      cfg.pin_d11 = GPIO_NUM_14; // R0 | schematic: I14 - R3
      cfg.pin_d12 = GPIO_NUM_21; // R1 | schematic: I21 - R4
      cfg.pin_d13 = GPIO_NUM_47; // R2 | schematic: I47 - R5
      cfg.pin_d14 = GPIO_NUM_48; // R3 | schematic: I48 - R6
      cfg.pin_d15 = GPIO_NUM_45; // R4 | schematic: I45 - R7

      cfg.pin_henable = GPIO_NUM_41; // schematic: IO41 - DE
      cfg.pin_vsync   = GPIO_NUM_40; // schematic: IO40 - VSYNC
      cfg.pin_hsync   = GPIO_NUM_39; // schematic: IO39 - HSYNC
      cfg.pin_pclk    = GPIO_NUM_0; // from elecrow forum: GPIO_NUM_0
      cfg.freq_write  = 12000000;

      cfg.hsync_polarity    = 0;

      cfg.hsync_front_porch = 40; // from elecrow forum: 40
      cfg.hsync_pulse_width = 48; // from elecrow forum: 48
      cfg.hsync_back_porch  = 40; // from elecrow forum: 40

      cfg.vsync_polarity    = 0;  // from elecrow forum: 0
      cfg.vsync_front_porch = 1;  // from elecrow forum: 1
      cfg.vsync_pulse_width = 31; // from elecrow forum: 31
      cfg.vsync_back_porch  = 13; // from elecrow forum: 13

      cfg.pclk_active_neg = 1; // new from elecrow forum: 1. was not defined before
      cfg.de_idle_high = 0;    // new from elecrow forum: 0. was not defined before
      cfg.pclk_idle_high = 0;  // from elecrow forum: 0

      _bus_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = GPIO_NUM_2;
      _light_instance.config(cfg);
    }
    _panel_instance.light(&_light_instance);

    {
      // following lines new from elecrow forum
       auto cfg = _touch_instance.config();
       cfg.x_min      = 0;
       cfg.x_max      = 799;
       cfg.y_min      = 0;
       cfg.y_max      = 479;
       cfg.pin_int    = -1;
       cfg.pin_rst    = -1;
       cfg.bus_shared = false;
       cfg.offset_rotation = 0;
       cfg.i2c_port   = I2C_NUM_1;
       cfg.pin_sda    = GPIO_NUM_19;
       cfg.pin_scl    = GPIO_NUM_20;
       cfg.freq       = 400000;
       cfg.i2c_addr   = 0x14;
       _touch_instance.config(cfg);
       _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};

```

### Libraries
[Lovyan GFX](https://github.com/lovyan03/LovyanGFX) needs to be included. For the audio examples I've also included [ESP32-audioI2S](https://github.com/schreibfaul1/ESP32-audioI2S) library (as an alternative, the [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio) library can be used. 
They are declared in the *lib_deps* section of *platformio.ini*

### platformio.ini
Next step is to adapt "platformio.ini"
```
[platformio]
src_dir = src
boards_dir = .

[env:esp32-s3-devkitc-1-myboard]
platform = espressif32
board = esp32-s3-devkitc-1-myboard
framework = arduino
monitor_speed = 115200
platform_packages = framework-arduinoespressif32@https://github.com/espressif/arduino-esp32#2.0.3
build_unflags = 
	-Os
	-std=gnu++11
build_flags = 
	-O3
	-std=gnu++17
	-DCORE_DEBUG_LEVEL=3
	-DLV_CONF_INCLUDE_SIMPLE
	-DLV_CONF_SUPPRESS_DEFINE_CHECK
	-I./src
lib_deps = 
	lovyan03/LovyanGFX@^1.1.7
	earlephilhower/ESP8266Audio@^1.9.7
board_build.partitions = huge_app.csv
```

## Some details about the program
*main.cpp* :
- Starts the LovyanGFX lib and then runs the complete set of examples given in the LovyanGFX documentation, these are included in *drawExamples()*
- setup() then sets up the audio pinout and volume,
- connects to the Wifi (needed for audio streams from the internet) and to the SD card (for examples from SD)
- forces mono since the Elecrow display only has a mono amplifier
- does any of a selection of audio demos, using connections to radio stations, test files from the web, or from the SD card
  (! if you want to play the examples from the SD card as predefined, the card should contain "test.wav" and "demo01.mp3" .. "demo11.mp3" in it's root directory!). The demo files are repeated endlessly
- Touch read is active during the whole runtime.
