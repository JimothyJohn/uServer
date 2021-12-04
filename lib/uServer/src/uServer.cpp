/*
  uServer.cpp - Class wrappers for uServer library
  that allow for easily configureable data collection
  Created by Nick Armenta, November 22, 2021.
  Released into the public domain.
*/

#include "Arduino.h"
// #include "ESP_WiFiManager.h" // AP login and maintenance
// #include "PubSubClient.h" // Enable MQTT
#include "ArduinoJson.h" // Handle JSON messages
// #include "AsyncTCP.h" // Generic async library
// #include "ESPAsyncWebServer.h" // ESP32 async library
#include <FS.h>     // Get FS functions
#include <SPIFFS.h> // Enable file system

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
