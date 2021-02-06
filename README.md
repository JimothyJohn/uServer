# uServer

Template for rapid IoT development on ESP32. Build webpages fast and exchange data quickly!

* Instant deployment - <a href="https://github.com/platformio/platformio-core">PlatformIO</a>
* Soft AP - <a href="https://github.com/tzapu/WiFiManager/">WiFiManager</a>
* Over-the-air updates - <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/ArduinoOTA">Arduino OTA</a>
* File serving (.html, .css, .js, etc.) - <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS">SPIFFS</a>
* Simple web server - <a href="https://github.com/me-no-dev/ESPAsyncWebServer">ESPAsyncWebServer</a>

## Hardware Required

* This project is only tested on an <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a>.

## Software Required

* <a href="https://github.com/platformio/platformio-core">PlatformIO</a>

## Installation

See [`QUICKSTART`](QUICKSTART.md) to quickly get up and running.

## Helpful Examples:

* <a href="https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino">Async Webserver</a>* <a href="https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino">OTA</a>
* <a href="https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino">WiFi Manager</a>

## To-do List

* Add security/authentication
* Add project examples
    * IoT - MQTT
    * Home automation - Philips Hue
    * Lighting effects - FastLED
* Change Wi-Fi login from default
* Minimize energy consumption
* Port to other platforms
