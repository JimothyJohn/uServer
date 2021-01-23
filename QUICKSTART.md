# Quick Start Guide

Start by cloning the repository into your desired directory and installing PlatformIO:

```sh 
user@host:~$ git clone https://github.com/JimothyJohn/LEDServer.git
user@host:~$ pip install platformio
```

Update [`src/main.cpp`](src/main.cpp) LED parameters

```cpp
// Line 19
// Change to your ESP data pin number
#define LED_PIN     12
// Line 25
// Change to length of LED row (or total number of LED's if in a line)
const uint16_t strandLength = 300;
// OPTIONAL - If you have more than one row of LEDs
const uint8_t strandNumber = 1;
```

<a href="https://macetech.github.io/FastLED-XY-Map-Generator/">Build an XY table</a> and replace existing

```cpp
// Line 40
const uint16_t XYTable[] = {
    Your, table, values, here
};
```

Update [`platform.ini`](platform.ini) configuration

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = ledcontrol.local
```

Move to root of repository and upload

```sh 
user@host:~$ cd <LEDServer>
user@host:<LEDServer>$ pio run -t upload
```

Go to your phone/PC's Wi-Fi settings and use access point "LEDSetup" to connect the ESP32 to your Wi-Fi, then finalize your [`platform.ini`](platform.ini) configuration

```ini
# Leave as comments in case you want to troubleshoot locally
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
monitor_speed = 115200
upload_protocol = espota
upload_port = ledcontrol.local
```

Your device is permanently configured to connect to your Wi-Fi on bootup so future uploads can now be over-the-air!
