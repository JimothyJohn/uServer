
// Load relevant libraries
#include <Arduino.h> //Base framework
#include <ESP_WiFiManager.h> // AP login and maintenance
#include <AsyncTCP.h> // Generic async library
#include <ESPAsyncWebServer.h> // ESP32 async library
#include <ArduinoOTA.h> // Enable OTA updates
#include <ESPmDNS.h> // Connect by hostname
#include <SPIFFS.h> // Enable file system
#include <PubSubClient.h> // Enable MQTT
#include <ArduinoJson.h> // Handle JSON messages

// Variables for "Digital I/O" page, change to match your configuration
const uint8_t buttonPin = 2; // the number of the pushbutton pin
const uint8_t ledPin =  13; // the number of the LED pin

// Variables for "MQTT" example, change to match your configuration
String subTopic = "None";

// Library classes
AsyncWebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<200> doc;

// Dual-core tasks
TaskHandle_t Task1;
TaskHandle_t Task2;

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// SPIFFS uses processor to replace HTML text with variables
String processor(const String& var) {
  if(var == "INPUT_NUMBER") { return String(buttonPin); }
  if(var == "INPUT_STATE") { return String(digitalRead(buttonPin)); }
}

// MQTT Handling functions
// https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char message[length + 1];
  strncpy(message, (char*)payload, length);
  message[length] = '\0';
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.println(error.f_str());
    return;
  }
  //const char* current_time = doc["time"];
  Serial.print("Message: ");
  Serial.println(message);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/home/status/esp32", "Hello world!");
      // ... and resubscribe
      client.subscribe("/home/status/time");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Initialize and setup MQTT client then connect
// https://pubsubclient.knolleary.net/
void SetupMQTT(const char* hostname ) {
  client.setServer(hostname, 1883);
  client.setCallback(callback);
  reconnect();
}

// Set up server callback functions
void SetupServer() {
  // Index/home page
  server.on("/|/index.html)$", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  
  // CSS file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Template script
  server.on("/App.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/App.js", "text/js");
  });

  // IO JSON message parser 
  server.on("/io$", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }
    uint8_t outputState = doc["output"].as<uint8_t>();;
    if (outputState) { digitalWrite(ledPin, outputState); }
    else { digitalWrite(ledPin, outputState); }
  });

  // Variable JSON message parser 
  server.on("/variables$", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }
    uint8_t adding = doc["postInt"].as<uint8_t>();;
    Serial.print("postInt: ");
    Serial.println(adding);
  });

  // Variable JSON message parser 
  server.on("/mqtt/connect$", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }
    const char* hostname = doc["host"];
    SetupMQTT(hostname);
  });


  // Variable JSON message parser 
  server.on("/mqtt/pub$", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    delay(1);
  },
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }
    const char* payload = doc["payload"];
    const char* topic = doc["topic"];
    bool published = client.publish(topic, payload);
    if(!published) { Serial.println('Failed to publish!'); }
  });

  server.on("/mqtt/sub$", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("subtopic", true)) {
      subTopic = request->getParam("subtopic", true)->value();
      bool subscribed = client.subscribe(subTopic.c_str());
      if(!subscribed) { Serial.println('Failed to subscribe!'); }
    } else { return; }
    request->send(SPIFFS, "/mqtt.html", String(), false, processor);
  });

  server.onNotFound(notFound);
  server.begin();
}

// Enable OTA updates
void SetupOTA() {
  Serial.print('Configuring OTA...');
  ArduinoOTA
    .onStart([]() {
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
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      ESP.restart();
    });

  ArduinoOTA.begin();
}

// Initialize Wi-Fi manager and connect to Wi-Fi
// https://github.com/khoih-prog/ESP_WiFiManager
void SetupWiFi() {
  Serial.print('Configuring WiFi...');
  Serial.print(F("\nStarting AutoConnect_ESP32_minimal on ")); Serial.println(ARDUINO_BOARD); 
  Serial.println(ESP_WIFIMANAGER_VERSION);
  ESP_WiFiManager wm("uServer");
  wm.autoConnect("uServer");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println(wm.getStatus(WiFi.status()));
  }
}

// WebServer Task 
void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) {
    ArduinoOTA.handle();
    if (client.connected()) { client.loop(); }
    // DELETING THIS DELAY WILL CRASH THE MCU
    delay(10);
  }
}

// Comms Task 
void Task2code( void * pvParameters ) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) {
    Serial.println("Task2 heartbeat");
    delay(10000);
  }
}

// Setup sequence
void setup() {
  delay(5000); // pwrLevel-up safety delay
  
  // Start serial server and connect to WiFi
  Serial.begin(115200);
  while (!Serial);
  Serial.print('Serial alive!');

  // Uses soft AP to connect to Wi-Fi (if saved credentials aren't valid)
  SetupWiFi();
  // Configures Async web server
  SetupServer();
  // Configures over-the-air updates
  SetupOTA();
  // mDNS allows for connection at http://userver.local/
  if(!MDNS.begin("userver")) {
    Serial.println("Error starting mDNS!");
    ESP.restart();
  }
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    ESP.restart();
  }

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "WebServer",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Comms",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */

  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input:
}

// Main loop
void loop() { }
