// Load relevant libraries
#include <Arduino.h>           //Base framework
#include <ESP_WiFiManager.h>   // AP login and maintenance
#include <AsyncTCP.h>          // Generic async library
#include <ESPAsyncWebServer.h> // ESP32 async library
#include <ArduinoOTA.h>        // Enable OTA updates
#include <ESPmDNS.h>           // Connect by hostname
#include <FS.h>                // Get FS functions
#include <SPIFFS.h>            // Enable file system
#include <PubSubClient.h>      // Enable MQTT
#include <ArduinoJson.h>       // Handle JSON messages
#include <HTTPClient.h>        // Extract object data
#include "uServer.h"
// #include <InfluxDbClient.h> //
// #include <InfluxDbCloud.h>

#if defined(ESP32)
#define DEVICE "ESP32"
#elif defined(ESP8266)
#define DEVICE "ESP8266"
#endif

#if defined(DEV_BUILD)
#define INCLUDE_LIBS true
#else
#define INCLUDE_LIBS false
#endif

#define INFLUXDB_URL "http://10.0.0.28:8086"
#define INFLUXDB_TOKEN "s7ETHxOg_FOl1QuIdPeJEbqmV3hB-xth0yueACkkgZ4s_9nwkYwXteGK118csqTKhlljaufIt3jHUiOOBEropA=="
#define INFLUXDB_ORG "default"
#define INFLUXDB_BUCKET "uServer"
// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
#define TZ_INFO "CST7CDT"

// InfluxDB client instance with preconfigured InfluxCloud certificate
// InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
// Point sensor("mqtt");

// Variable for Variables page, change to match your configuration
uint8_t userVariable = 0;

// Library classes
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events");
WiFiClient espClient;
PubSubClient pubsubClient(espClient);

// Dual-core tasks
TaskHandle_t Task1;

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

/* Influx function
void WriteToInflux(char* topic, char* message) {
  sensor.clearFields();

  // Store measured value into point
  sensor.addField("topic", topic);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}
*/

// MQTT Handling functions
// https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
void callback(char *topic, byte *payload, unsigned int length)
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
  events.send(responseChar, "subscription", millis());
  // WriteToInflux(topic, message);
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
      // ... and resubscribe
      pubsubClient.subscribe("/home/status/time");
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

// Initialize and setup MQTT client then connect
// https://pubsubclient.knolleary.net/
void SetupMQTT(const char *hostname)
{
  pubsubClient.setServer(hostname, 1883);
  pubsubClient.setCallback(callback);
  reconnect();
}

// Generic request handler
void requestHandler(AsyncWebServerRequest *request)
{
  delay(1);
  // Serial.println("Request handled!");
}

// Generic file handler
void fileHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  Serial.println("File handled!");
}

// Set up server callback functions
void SetupServer()
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
  server.on("/io", HTTP_POST, requestHandler,
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
        delay(1);
      },
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<JSON_SIZE> ioDoc;
        DeserializationError error = deserializeJson(ioDoc, data);
        if (error)
        {
          Serial.println(error.f_str());
          return;
        }

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

        Serial.print(ioDoc["pin"].as<const char *>());
        Serial.print(" is ");
        Serial.println(ioDoc["state"].as<const char *>());

        request->send(200, "application/json", jsonResponse(ioDoc));
        ioDoc.clear();
      });

  // Variable JSON message parser
  server.on("/variables", HTTP_POST, requestHandler,
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
        delay(1);
      },
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<JSON_SIZE> variableDoc;
        DeserializationError error = deserializeJson(variableDoc, data);
        if (error)
        {
          Serial.println(error.f_str());
          return;
        }
        userVariable = variableDoc["postInt"].as<uint8_t>();
        Serial.print("Received a ");
        Serial.println(userVariable);
        variableDoc["code"] = 0;
        request->send(200, "application/json", jsonResponse(variableDoc));
        variableDoc.clear();
      });

  // Variable JSON message parser
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
                SetupMQTT(hostname);
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

              StaticJsonDocument<JSON_SIZE> fileList = listDir(SPIFFS, fileDoc["dir"].as<const char *>(), 0);
              Serial.print("Server response: ");
              serializeJson(fileList, Serial);
              Serial.println();
              fileDoc.clear();
              request->send(200, "application/json", jsonResponse(fileList));
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

              StaticJsonDocument<JSON_SIZE> configFile = readFile(SPIFFS, fileDoc["filename"].as<const char *>());
              serializeJson(configFile, Serial);
              Serial.println();
              fileDoc.clear();
              request->send(200, "application/json", jsonResponse(configFile));
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
  Serial.print("WebServer running on core ");
  Serial.println(xPortGetCoreID());
}

// Enable OTA updates
void SetupOTA()
{
  Serial.println("Configuring OTA...");
  ArduinoOTA.onStart([]() {
              String type;
              if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
              else // U_SPIFFS
                type = "filesystem";

              // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
              Serial.println("Start updating " + type);
            })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
        ESP.restart();
      });

  ArduinoOTA.begin();
}

// Initialize Wi-Fi manager and connect to Wi-Fi
// https://github.com/khoih-prog/ESP_WiFiManager
void SetupWiFi()
{
  Serial.print(F("\nStarting AutoConnect_ESP32_minimal on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP_WIFIMANAGER_VERSION);
  ESP_WiFiManager wm("uServer");
  wm.autoConnect("uServer");
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println(wm.getStatus(WiFi.status()));
  }
}

// Auxiiliary Task
void Task1code(void *pvParameters)
{
  Serial.print("Auxiliary task running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    ArduinoOTA.handle();
    if (pubsubClient.connected())
    {
      pubsubClient.loop();
    }
    // DELETING THIS DELAY WILL CRASH THE MCU
    delay(25);
  }
}

// Setup sequence
void setup()
{

  // Start serial server and connect to WiFi
  delay(200);
  Serial.begin(115200);
  while (!Serial)
    ;

  delay(2000); // pwrLevel-up safety delay

  // Uses soft AP to connect to Wi-Fi (if saved credentials aren't valid)
  SetupWiFi();
  // Configures Async web server
  SetupServer();
  // Configures over-the-air updates
  SetupOTA();
  // mDNS allows for connection at http://userver.local/
  if (!MDNS.begin("userver"))
  {
    Serial.println("Error starting mDNS!");
    ESP.restart();
  }
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
    ESP.restart();
  }

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      Task1code,   /* Task function. */
      "Auxiliary", /* name of task. */
      10000,       /* Stack size of task */
      NULL,        /* parameter of the task */
      1,           /* priority of the task */
      &Task1,      /* Task handle to keep track of created task */
      0);          /* pin task to core 0 */

  /* Influx setup        
  // Add tags
  sensor.addTag("device", DEVICE);

  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }               
  */
}

// Main loop
void loop() {}
