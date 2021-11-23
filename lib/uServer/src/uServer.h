/*
  uServer.h - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#ifndef uServer_h
#define uServer_h

#include "Arduino.h"
#include "ESP_WiFiManager.h" // AP login and maintenance
#include "ArduinoJson.h" // Handle JSON messages
#include "AsyncTCP.h" // Generic async library
#include "ESPAsyncWebServer.h" // ESP32 async library

String jsonResponse(StaticJsonDocument<128> doc);

void notFound(AsyncWebServerRequest *request);

void SetupServer();

#endif
