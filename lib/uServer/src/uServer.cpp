/*
  uServer.cpp - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#include "Arduino.h"
// #include "ESP_WiFiManager.h" // AP login and maintenance
#include "PubSubClient.h"      // Enable MQTT
#include "ArduinoJson.h"       // Handle JSON messages
#include "ESPAsyncWebServer.h" // ESP32 async library
#include <FS.h>                // Get FS functions
#include <SPIFFS.h>            // Enable file system
#include "uServer.h"

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
    Serial.println("− failed to open directory");
  }
  if (!root.isDirectory())
  {
    Serial.println(" − not a directory");
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

// Generic file handler
void fileHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  return;
}

void reconnect(PubSubClient client)
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/home/status/esp32", "Hello world!");
      // ... and resubscribe
      client.subscribe("/home/status/time");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
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

// Custom WebServer
uServer::uServer(){};

void uServer::begin()
{
  // Index/home page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });

  // CSS file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // React script
  server.on("/App.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/App.js", "text/js");
  });

  // IO JSON message parser
  server.on(
      "/io", HTTP_POST, requestHandler,
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
        return;
      },
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
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
      });

  // Variable JSON message parser
  server.on(
      "/variables", HTTP_POST, requestHandler,
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
        return;
      },
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
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
      });

  // MQTT Handler
  /*
  server.on("/mqtt", HTTP_POST, requestHandler, fileHandler,
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
              StaticJsonDocument<JSON_SIZE> mqttDoc;
              DeserializationError error = deserializeJson(mqttDoc, data);
              if (error)
              {
                Serial.println(error.f_str());
                return;
              }

              if (mqttDoc["action"] == "connect")
              {
                const char *hostname = mqttDoc["host"];
                pubsubClient.setServer(hostname, 1883);
                pubsubClient.setCallback(callback);
                reconnect(pubsubClient);
                mqttDoc["code"] = 0;
              }
              else if (mqttDoc["action"] == "disconnect")
              {
                Serial.println("Disconnecting from MQTT broker");
                pubsubClient.disconnect();
                if (!pubsubClient.connected())
                {
                  mqttDoc["code"] = 0;
                }
                else
                {
                  mqttDoc["code"] = 1;
                }
              }
              else if (mqttDoc["action"] == "publish")
              {
                const char *payload = mqttDoc["payload"];
                const char *topic = mqttDoc["topic"];
                Serial.print("Publishing ");
                Serial.print(payload);
                Serial.print(" to ");
                Serial.println(topic);
                bool published = pubsubClient.publish(topic, payload);
                if (!published)
                {
                  mqttDoc["code"] = 1;
                }
                else
                {
                  mqttDoc["code"] = 0;
                }
              }
              else if (mqttDoc["action"] == "subscribe")
              {
                const char *topic = mqttDoc["topic"];
                bool subscribed = pubsubClient.subscribe(topic);
                if (!subscribed)
                {
                  mqttDoc["code"] = 1;
                }
                else
                {
                  mqttDoc["code"] = 0;
                }
              }

              serializeJson(mqttDoc, Serial);
              request->send(200, "application/json", jsonResponse(mqttDoc));
              mqttDoc.clear();
            });
  */

  // Variable JSON message parser
  server.on("/files", HTTP_POST, requestHandler, fileHandler,
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
              StaticJsonDocument<JSON_SIZE> fileDoc;
              DeserializationError error = deserializeJson(fileDoc, data);
              if (error)
              {
                Serial.println(error.f_str());
                return;
              }
              Serial.println("/files received");
              serializeJsonPretty(fileDoc, Serial);
              Serial.println();

              StaticJsonDocument<JSON_SIZE> fileList = listDir(SPIFFS, fileDoc["dir"].as<const char *>(), 0);

              Serial.println("/files response");
              serializeJsonPretty(fileList, Serial);
              Serial.println();
              fileDoc.clear();
              request->send(200, "application/json", jsonResponse(fileList));
              fileList.clear();
            });

  // Variable JSON message parser
  server.on("/file", HTTP_POST, requestHandler, fileHandler,
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
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
            });

  events.onConnect([](AsyncEventSourceClient *client) {
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
