/*
  uServer.h - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#ifndef uServer_h
#define uServer_h

#include <Arduino.h>
// #include "ESP_WiFiManager.h" // AP login and maintenance
#include <PubSubClient.h>      // Enable MQTT
#include <ArduinoJson.h>       // Handle JSON messages
#include <FS.h>                // Get FS functions
#include <SPIFFS.h>            // Enable file system
#include <ESPAsyncWebServer.h> // ESP32 async library

// Send JSON message in a response string
String jsonResponse(StaticJsonDocument<JSON_SIZE> doc);

// List directories in SPIFFS
StaticJsonDocument<JSON_SIZE> listDir(fs::FS &fs, const char *dirname, uint8_t levels);

// Extract JSON object from .json file
StaticJsonDocument<JSON_SIZE> readFile(fs::FS &fs, const char *filename);

// Generic handling functions
void notFound(AsyncWebServerRequest *request);
void requestHandler(AsyncWebServerRequest *request);
void ignoreFile(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
// Purposeful handling functions
void ioHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void variableHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void dirHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void fileHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void mqttHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void cloudHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

// MQTT Functions
void reconnect();
void callback(char *topic, byte *payload, unsigned int length);
static WiFiClient wifiClient;
static PubSubClient pubsubClient(wifiClient);
static HTTPClient http;
StaticJsonDocument<JSON_SIZE> sendREST(String request);
static AsyncEventSource events = AsyncEventSource("/events"); // access at ws://[esp ip]/w
static AsyncWebSocket ws = AsyncWebSocket("/ws");

// Custom WebServer
class uServer
{
public:
  uServer();
  void begin();
  AsyncWebServer server = AsyncWebServer(80);
};

#endif
