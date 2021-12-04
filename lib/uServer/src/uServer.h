/*
  uServer.h - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#ifndef uServer_h
#define uServer_h

#include "Arduino.h"
// #include "ESP_WiFiManager.h" // AP login and maintenance
#include "ArduinoJson.h" // Handle JSON messages
#include <FS.h>          // Get FS functions
#include <SPIFFS.h>      // Enable file system
// #include "AsyncTCP.h" // Generic async library
// #include "ESPAsyncWebServer.h" // ESP32 async library

// Send JSON message in a response string
String jsonResponse(StaticJsonDocument<JSON_SIZE> doc);
// List directories in SPIFFS
StaticJsonDocument<JSON_SIZE> listDir(fs::FS &fs, const char *dirname, uint8_t levels);
// Read JSON file
StaticJsonDocument<JSON_SIZE> readFile(fs::FS &fs, const char *filename);

#endif
