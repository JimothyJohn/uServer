# uServer

Template for rapid IoT development on ESP32. Build webpages fast and exchange data quickly!

* Instant deployment - <a href="https://github.com/platformio/platformio-core">PlatformIO</a> (Estonia)
* Soft AP - <a href="https://github.com/tzapu/WiFiManager/">WiFiManager</a> (Asia)
* Over-the-air updates - <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/ArduinoOTA">Arduino OTA</a> (Slovakia)
* File serving (.html, .css, .js, etc.) - <a href="https://github.com/pellepl/spiffs">SPIFFS</a> (Sweden)
* Simple web server - <a href="https://github.com/me-no-dev/ESPAsyncWebServer">ESPAsyncWebServer</a> (Bulgaria)

## Hardware Required

* This project is only tested on an <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a> (China)

## Software Required

* <a href="https://github.com/platformio/platformio-core">PlatformIO</a>

## Installation

See [`QUICKSTART`](QUICKSTART.md) to quickly get up and running.

## Sample Page

![Webpage screenshot](https://drive.google.com/uc?id=18oRyFornn44xHVyPGCcWwTIvMA_0hs_T)

## Helpful Examples:

* <a href="https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino">Async Webserver</a>
* <a href="https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino">OTA</a>
* <a href="https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino">WiFi Manager</a>

## To-do List

* Migrate to LittleFS (SPIFFS deprecated)
* Add more responsive CSS
* Add additional security/authentication
* Change Wi-Fi login from default
* Minimize energy consumption
* Port to other platforms
