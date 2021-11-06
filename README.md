# uServer

Template for rapid IoT development on ESP32. Build webpages fast and exchange data quickly!

## Sample Page

![Webpage screenshot](docs/Webpage.png)

## Features

See [`FEATURES`](FEATURES.md) for full list of features and examples.

* Instant deployment - <a href="https://github.com/platformio/platformio-core">PlatformIO</a> (Estonia)
* Soft AP - <a href="https://github.com/tzapu/WiFiManager">WiFiManager</a> (Asia)
* Over-the-air updates - <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/ArduinoOTA">Arduino OTA</a> (Slovakia)
* <a href="https://pubsubclient.knolleary.net">MQTT</a> (UK) with <a href="https://arduinojson.org">JSON</a> parsing
* File serving (.html, .css, .js) - <a href="https://github.com/pellepl/spiffs">SPIFFS</a> (Sweden)
* Simple asynchronous web server - <a href="https://github.com/me-no-dev/ESPAsyncWebServer">ESPAsyncWebServer</a> (Bulgaria)
* Dual core multi-tasking - <a href="https://freertos.org">FreeRTOS</a> (USA)
* Responsive web interface - <a href="https://reactjs.org">React</a> and <a href="https://getbootstrap.com">Boostrap</a> via CDN

## Hardware Required

* This project is only tested on an <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a> (China)

## Software Required

* <a href="https://github.com/platformio/platformio-core">PlatformIO</a>

## Installation

See [`QUICKSTART`](QUICKSTART.md) to quickly get up and running.

## Helpful Examples:

* <a href="https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino">Async Webserver</a>
* <a href="https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino">OTA</a>
* <a href="https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino">WiFi Manager</a>
* <a href="https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino">MQTT</a>
* <a href="https://github.com/bblanchon/ArduinoJson/blob/6.x/examples/JsonParserExample/JsonParserExample.ino">JSON Parsing</a>

## To-do List

* Migrate to LittleFS (SPIFFS deprecated)
* Add additional security/authentication
* Minimize energy consumption
* Port to other platforms
* Add React UI
* <a href="https://randomnerdtutorials.com/esp32-microsd-card-arduino">Integrate persistent storage</a>
