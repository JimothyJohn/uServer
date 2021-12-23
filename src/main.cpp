// Load relevant libraries
#include <Arduino.h>           //Base framework
#include <ESP_WiFiManager.h>   // AP login and maintenance
#include <AsyncTCP.h>          // Generic async library
#include <ESPAsyncWebServer.h> // ESP32 async library
// #include <ArduinoOTA.h>        // Enable OTA updates
#include <ESPmDNS.h>      // Connect by hostname
#include <FS.h>           // Get FS functions
#include <SPIFFS.h>       // Enable file system
#include <PubSubClient.h> // Enable MQTT
#include <ArduinoJson.h>  // Handle JSON messages
#include <HTTPClient.h>   // Extract object data
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

#define INFLUXDB_URL "http://127.0.0.1:8086"
#define INFLUXDB_TOKEN ""
#define INFLUXDB_ORG "default"
#define INFLUXDB_BUCKET "default"
// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
#define TZ_INFO "CST7CDT"

uServer myserver;

// Dual-core tasks
TaskHandle_t Task1;

// StaticJsonDocument<JSON_SIZE> CONFIG = readFile(SPIFFS, "/config/env/config.json");

// Enable OTA updates
/*
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
*/
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
    // ArduinoOTA.handle();
    if (pubsubClient.connected())
    {
      Serial.println("LOOPING");
      pubsubClient.loop();
    }

    // sendREST("http://10.0.0.31:8000/");
    // DELETING THIS DELAY WILL CRASH THE MCU
    delay(50);
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
  myserver.begin();
  // Configures over-the-air updates
  // SetupOTA();

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
