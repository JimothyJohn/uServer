# uServer Features & Examples

* Instant deployment - <a href="https://github.com/platformio/platformio-core">PlatformIO</a> (Estonia)
* Soft AP - <a href="https://github.com/tzapu/WiFiManager">WiFiManager</a> (Asia)
    * <a href="https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino">WiFi example</a>
* Over-the-air updates - <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/ArduinoOTA">Arduino OTA</a> (Slovakia)
    * <a href="https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino">OTA example</a>
* <a href="https://pubsubclient.knolleary.net">MQTT</a> (UK) with <a href="https://arduinojson.org">JSON</a> parsing
    * <a href="https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino">MQTT example</a>
    * <a href="https://github.com/bblanchon/ArduinoJson/blob/6.x/examples/JsonParserExample/JsonParserExample.ino">JSON example</a>
* File serving (.html, .css, .js) - <a href="https://github.com/pellepl/spiffs">SPIFFS</a> (Sweden)
    * <a href="https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/ESP_AsyncFSBrowser/ESP_AsyncFSBrowser.ino">SPIFFS file server example</a>
* Simple asynchronous web server - <a href="https://github.com/me-no-dev/ESPAsyncWebServer">ESPAsyncWebServer</a> (Bulgaria)
    * <a href="https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino">Webserver example</a>
* Dual core multi-tasking - <a href="https://freertos.org">FreeRTOS</a> (USA)
    * <a href="https://microcontrollerslab.com/esp32-dual-core-freertos-arduino-ide/">Task handler example</a>
* Single page app powered by <a href="https://reactjs.org">React UI</a>, <a href="https://axios-http.com">Axios API</a>, and <a href="https://getbootstrap.com">Boostrap</a> layout via CDN
    * <a href="https://reactjs.org/tutorial/tutorial.html">React tutorial</a>
    * <a href="https://getbootstrap.com/docs/5.0/getting-started/introduction/">Bootstrap docs</a>
    * <a href="https://axios-http.com/docs/api_intro">Axios docs</a>

## To-do

* Fix Events handler
* Add pubsubClient as external client from library
* Make web utilities persistent in separate task
