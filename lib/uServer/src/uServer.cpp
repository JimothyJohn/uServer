/*
  uServer.cpp - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#include <Arduino.h>
// #include "ESP_WiFiManager.h" // AP login and maintenance
#include <PubSubClient.h>      // Enable MQTT
#include <ArduinoJson.h>       // Handle JSON messages
#include <ESPAsyncWebServer.h> // ESP32 async library
#include <FS.h>                // Get FS functions
#include <SPIFFS.h>            // Enable file system
#include <HTTPClient.h>        // Enable API calls
#include "uServer.h"           // Import uServer headers

// Variable for Variables page, change to match your configuration
uint8_t userVariable = 0;

String jsonResponse(StaticJsonDocument<JSON_SIZE> doc)
{
  String response;
  response.reserve(JSON_SIZE);
  serializeJson(doc, response);
  return response;
}

// List directories in SPIFFS
StaticJsonDocument<JSON_SIZE> listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  // allocate the memory for the document
  StaticJsonDocument<JSON_SIZE> arrayDoc;
  StaticJsonDocument<JSON_SIZE> fileDoc;
  JsonArray fileList = arrayDoc.to<JsonArray>();
  JsonObject fileObject = fileDoc.to<JsonObject>();

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
    }
    else
    {
      String filename = file.name();
      fileObject["name"] = filename;
      fileObject["size"] = file.size();
    }
    fileList.add(fileObject);
    file.close();
    file = root.openNextFile();
  }
  fileObject.clear();
  fileDoc.clear();
  return fileList;
}

// Extract JSON object from .json file
StaticJsonDocument<JSON_SIZE> readFile(fs::FS &fs, const char *filename)
{
  StaticJsonDocument<JSON_SIZE> doc;
  File file = fs.open(filename);
  if (file)
  {
    deserializeJson(doc, file);
    file.close();
    doc["code"] = 0;
  }
  else
  {
    doc["code"] = 1;
  }
  return doc;
}

void reconnect()
{
  // Loop until we're reconnected
  while (!pubsubClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (pubsubClient.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      pubsubClient.publish("/home/status/esp32", "Hello world!");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(pubsubClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// MQTT Handling functions
// https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
void callback(char *topic, uint8_t *payload, unsigned int length)
{
  // Create spot in memory for message
  char message[length + 1];
  // Write payload to String
  strncpy(message, (char *)payload, length);
  // Nullify last character to eliminate garbage at end
  message[length] = '\0';

  // Create correct object
  Serial.print("Received \"");
  Serial.print(message);
  Serial.print("\" from ");
  Serial.println(topic);

  StaticJsonDocument<JSON_SIZE> subDoc;
  subDoc["payload"] = message;
  subDoc["topic"] = topic;

  const char *responseChar = jsonResponse(subDoc).c_str();
  // events.send(responseChar, "subscription", millis());
  // WriteToInflux(topic, message);
}

StaticJsonDocument<JSON_SIZE> sendREST(String request)
{
  StaticJsonDocument<JSON_SIZE> doc;
  http.begin(request);
  // http.setAuthorization(API_KEY);
  int code = http.GET();
  if (code < 0)
  {
    doc["code"] = 1;
  }
  else
  {
    String response = http.getString();
    deserializeJson(doc, response);
    doc["code"] = 0;
  }
  return doc;
}

// 404 Page
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

// Ignore request
void requestHandler(AsyncWebServerRequest *request)
{
  return;
}

// Ignore file
void ignoreFile(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  return;
}

void ioHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<JSON_SIZE> ioDoc;
  DeserializationError error = deserializeJson(ioDoc, data);
  if (error)
  {
    Serial.println(error.f_str());
    return;
  }
  Serial.println("/io received");
  serializeJsonPretty(ioDoc, Serial);
  Serial.println();

  if (ioDoc["action"] == "read")
  {
    if (digitalRead(ioDoc["pin"]))
    {
      ioDoc["state"] = "HIGH";
    }
    else
    {
      ioDoc["state"] = "LOW";
    }
  }
  else if (ioDoc["action"] == "write")
  {
    digitalWrite(ioDoc["pin"], ioDoc["state"]);
  }

  ioDoc["code"] = 0;

  Serial.println("/io response: ");
  serializeJsonPretty(ioDoc, Serial);
  Serial.println();

  request->send(200, "application/json", jsonResponse(ioDoc));
  ioDoc.clear();
  return;
}

void variableHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<JSON_SIZE> variableDoc;
  DeserializationError error = deserializeJson(variableDoc, data);
  if (error)
  {
    Serial.println(error.f_str());
    return;
  }

  Serial.println("/variables received");
  serializeJsonPretty(variableDoc, Serial);
  Serial.println();

  userVariable = variableDoc["postInt"].as<uint8_t>();
  variableDoc["code"] = 0;
  Serial.println("/variables response");
  serializeJsonPretty(variableDoc, Serial);
  Serial.println();
  request->send(200, "application/json", jsonResponse(variableDoc));
  variableDoc.clear();
}

void dirHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<JSON_SIZE> dirDoc;
  DeserializationError error = deserializeJson(dirDoc, data);
  if (error)
  {
    Serial.println(error.f_str());
    return;
  }
  Serial.println("/dir received");
  serializeJsonPretty(dirDoc, Serial);
  Serial.println();

  StaticJsonDocument<JSON_SIZE> fileList = listDir(SPIFFS, dirDoc["dir"].as<const char *>(), 0);

  Serial.println("/dir response");
  serializeJsonPretty(fileList, Serial);
  Serial.println();
  dirDoc.clear();
  request->send(200, "application/json", jsonResponse(fileList));
  fileList.clear();
  return;
}

void fileHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<JSON_SIZE> fileDoc;
  DeserializationError error = deserializeJson(fileDoc, data);
  if (error)
  {
    Serial.println(error.f_str());
    return;
  }
  Serial.println("/file received");
  serializeJsonPretty(fileDoc, Serial);
  Serial.println();

  StaticJsonDocument<JSON_SIZE> configFile = readFile(SPIFFS, fileDoc["filename"].as<const char *>());
  configFile["code"] = 0;

  Serial.println("/file response");
  serializeJsonPretty(configFile, Serial);
  Serial.println();
  fileDoc.clear();
  request->send(200, "application/json", jsonResponse(configFile));
  configFile.clear();
  return;
}

void mqttHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<JSON_SIZE> mqttDoc;
  DeserializationError error = deserializeJson(mqttDoc, data);
  if (error)
  {
    Serial.println(error.f_str());
    return;
  }
  Serial.println("/mqtt received");
  serializeJsonPretty(mqttDoc, Serial);
  Serial.println();

  mqttDoc["code"] = 1;

  if (mqttDoc["action"] == "connect" && !pubsubClient.connected())
  {
    const char *hostname = mqttDoc["host"];
    pubsubClient.setServer(hostname, 1883);
    pubsubClient.setCallback(callback);
    reconnect();

    if (pubsubClient.connected())
    {
      mqttDoc["code"] = 0;
    }
  }
  else if (mqttDoc["action"] == "disconnect" && pubsubClient.connected())
  {
    pubsubClient.disconnect();
    if (!pubsubClient.connected())
    {
      mqttDoc["code"] = 0;
    }
  }
  else if (mqttDoc["action"] == "publish")
  {
    const char *payload = mqttDoc["payload"];
    const char *topic = mqttDoc["topic"];
    if (pubsubClient.publish(topic, payload))
    {
      mqttDoc["code"] = 0;
    }
  }
  else if (mqttDoc["action"] == "subscribe")
  {
    const char *topic = mqttDoc["topic"];
    if (pubsubClient.subscribe(topic))
    {
      mqttDoc["code"] = 0;
    }
  }
  else if (mqttDoc["action"] == "configure")
  {
    mqttDoc = readFile(SPIFFS, "/config/mqtt.json");
  }

  Serial.println("/mqtt response");
  serializeJsonPretty(mqttDoc, Serial);
  Serial.println();
  request->send(200, "application/json", jsonResponse(mqttDoc));
  mqttDoc.clear();
  return;
}

void cloudHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<JSON_SIZE> requestDoc;
  DeserializationError error = deserializeJson(requestDoc, data);
  if (error)
  {
    Serial.println(error.f_str());
    return;
  }
  Serial.println("/cloud received");
  serializeJsonPretty(requestDoc, Serial);
  Serial.println();

  String hostname = requestDoc["hostname"];
  String endpoint = requestDoc["endpoint"];
  String query = requestDoc["query"];
  requestDoc.clear();
  String httpRequest = hostname + endpoint + query;
  StaticJsonDocument<JSON_SIZE> responseDoc = sendREST(httpRequest);

  Serial.println("/cloud response");
  serializeJsonPretty(responseDoc, Serial);
  Serial.println();

  request->send(200, "application/json", jsonResponse(responseDoc));
  responseDoc.clear();
}

// Custom WebServer
uServer::uServer(){};

void uServer::begin()
{
  // Index/home page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });

  // CSS file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // React script
  server.on("/App.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/App.js", "text/js"); });

  /*
  // React utilities
  server.on("/uServer.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/uServer.js", "text/js");
  });
  */

  // IO JSON message parser
  server.on("/io", HTTP_POST, requestHandler, ignoreFile, ioHandler);

  // Variable JSON message parser
  server.on("/variables", HTTP_POST, requestHandler, ignoreFile, variableHandler);

  server.on("/mqtt", HTTP_POST, requestHandler, ignoreFile, mqttHandler);

  // Variable JSON message parser
  server.on("/dir", HTTP_POST, requestHandler, ignoreFile, dirHandler);

  // Variable JSON message parser
  server.on("/file", HTTP_POST, requestHandler, ignoreFile, fileHandler);

  // Variable JSON message parser
  server.on("/cloud", HTTP_POST, requestHandler, ignoreFile, cloudHandler);

  events.onConnect(
      [](AsyncEventSourceClient *client)
      {
        if (client->lastId())
        {
          Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
        }
        //send event with message "hello!", id current millis
        // and set reconnect delay to 1 second
        client->send("hello!", NULL, millis(), 1000);
      });
  //HTTP Basic authentication
  // events.setAuthentication("user", "pass");
  server.addHandler(&events);

  server.onNotFound(notFound);
  server.begin();
}
