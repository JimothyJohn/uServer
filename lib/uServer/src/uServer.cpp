/*
  uServer.cpp - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ESP_WiFiManager.h" // AP login and maintenance
#include "ArduinoOTA.h" // Enable OTA updates
#include "PubSubClient.h" // Enable MQTT
#include "ArduinoJson.h" // Handle JSON messages
#include "AsyncTCP.h" // Generic async library
#include "ESPAsyncWebServer.h" // ESP32 async library
#include "SPIFFS.h" // Enable file system

AsyncWebServer server(80);
AsyncEventSource events("/events");
WiFiClient espClient;
PubSubClient pubsubClient(espClient);

String jsonResponse(StaticJsonDocument<128> doc) {
  String response;
  response.reserve(1024);
  serializeJson(doc, response);
  return response;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Set up server callback functions
void SetupServer() {
  // Index/home page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  
  // CSS file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // React script
  server.on("/App.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/App.js", "text/js");
  });

  // IO JSON message parser 
  server.on("/io", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    StaticJsonDocument<128> ioDoc;
    DeserializationError error = deserializeJson(ioDoc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }

    if (ioDoc["action"]=="read") {
      if(digitalRead(ioDoc["pin"])) { ioDoc["state"] = "HIGH"; }
      else { ioDoc["state"] = "LOW"; }
    } else if (ioDoc["action"]=="write") {
      digitalWrite(ioDoc["pin"], ioDoc["state"]);
    }

    Serial.print(ioDoc["pin"].as<const char*>());
    Serial.print(" is ");
    Serial.println(ioDoc["state"].as<const char*>());

    request->send(200, "application/json", jsonResponse(ioDoc));
  });

  // Variable JSON message parser 
  server.on("/variables", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    StaticJsonDocument<128> variableDoc;
    DeserializationError error = deserializeJson(variableDoc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }
    Serial.print("Received a ");
    Serial.println(variableDoc["postInt"].as<uint8_t>());
    variableDoc["code"] = 0;
    request->send(200, "application/json", jsonResponse(variableDoc));
  });

  // Variable JSON message parser 
  server.on("/mqtt", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    StaticJsonDocument<128> mqttDoc;
    DeserializationError error = deserializeJson(mqttDoc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }

    if (mqttDoc["action"]=="connect") {
      const char* hostname = mqttDoc["host"];
      SetupMQTT(hostname);
      mqttDoc["code"] = 0;
    }
    else if (mqttDoc["action"]=="disconnect") {
      Serial.println("Disconnecting from MQTT broker");
      pubsubClient.disconnect();
      if(!pubsubClient.connected()) { mqttDoc["code"] = 0; }
      else { mqttDoc["code"] = 1; }
    }
    else if (mqttDoc["action"]=="publish") {
      const char* payload = mqttDoc["payload"];
      const char* topic = mqttDoc["topic"];
      Serial.print("Publishing ");
      Serial.print(payload);
      Serial.print(" to ");
      Serial.println(topic);
      bool published = pubsubClient.publish(topic, payload);
      if(!published) { mqttDoc["code"] = 1; }
      else { mqttDoc["code"] = 0; }
    }
    else if (mqttDoc["action"]=="subscribe") {
      const char* topic = mqttDoc["topic"];
      bool subscribed = pubsubClient.subscribe(topic);
      if(!subscribed) { mqttDoc["code"] = 1; }
      else { mqttDoc["code"] = 0; }
    }
    
    request->send(200, "application/json", jsonResponse(mqttDoc));
  });

  // setup ......
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
    }
    //send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
  });
  //HTTP Basic authentication
  // events.setAuthentication("user", "pass");
  server.addHandler(&events);

  server.onNotFound(notFound);
  server.begin();
  Serial.print("WebServer running on core ");
  Serial.println(xPortGetCoreID());
}
