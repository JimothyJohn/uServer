
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

// Variable for Variables page, change to match your configuration
uint8_t userVariable = 0;

// Delete when possible
String subTopic = "None";

// Library classes
AsyncWebServer server(80);
AsyncEventSource events("/events");
WiFiClient espClient;
PubSubClient pubsubClient(espClient);
DynamicJsonDocument doc(1024);

// Dual-core tasks
TaskHandle_t Task1;

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// MQTT Handling functions
// https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
void callback(char* topic, byte* payload, unsigned int length) {
  // Create spot in memory for message
  char message[length + 1];
  // Write payload to String
  strncpy(message, (char*)payload, length);
  // Nullify last character to eliminate garbage at end
  message[length] = '\0';
  
  // Create correct object
  Serial.print("Received \"");
  Serial.print(message);
  Serial.print("\" from ");
  Serial.println(topic);
  
  DynamicJsonDocument docu(256);
  docu["message"] = message;
  docu["topic"] = topic;
    
  String response;
  response.reserve(1024);
  serializeJson(docu, response);
  const char* responseChar = response.c_str();
  events.send(responseChar, "subscription", millis());
}

void reconnect() {
  // Loop until we're reconnected
  while (!pubsubClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (pubsubClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      pubsubClient.publish("/home/status/esp32", "Hello world!");
      // ... and resubscribe
      pubsubClient.subscribe("/home/status/time");
    } else {
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
void SetupMQTT(const char* hostname ) {
  pubsubClient.setServer(hostname, 1883);
  pubsubClient.setCallback(callback);
  reconnect();
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

  // Template script
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
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }

    if (doc["action"]=="read") {
      if(digitalRead(doc["pin"])) { doc["state"] = "HIGH"; }
      else { doc["state"] = "LOW"; }
    } else if (doc["action"]=="write") {
      digitalWrite(doc["pin"], doc["state"]);
    }

    Serial.print(doc["pin"].as<const char*>());
    Serial.print(" is ");
    Serial.println(doc["state"].as<const char*>());

    String response;
    response.reserve(1024);
    serializeJson(doc, response);
    request->send(200, "application/json", response);
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
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }
    userVariable = doc["postInt"].as<uint8_t>();
    Serial.print("Received a ");
    Serial.println(userVariable);
    String response;
    response.reserve(1024);
    doc["code"] = 0;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
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
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.println(error.f_str());
      return;
    }

    if (doc["action"]=="connect") {
      const char* hostname = doc["host"];
      SetupMQTT(hostname);
      doc["code"] = 0;
    } else if (doc["action"]=="disconnect") {
      Serial.println("Disconnecting from MQTT broker");
      pubsubClient.disconnect();
      if(!pubsubClient.connected()) { doc["code"] = 0; }
      else { doc["code"] = 1; }
    } else if (doc["action"]=="publish") {
      const char* payload = doc["payload"];
      const char* topic = doc["topic"];
      Serial.print("Publishing ");
      Serial.print(payload);
      Serial.print(" to ");
      Serial.println(topic);
      bool published = pubsubClient.publish(topic, payload);
      if(!published) { doc["code"] = 1; }
      else { doc["code"] = 0; }
    } else if (doc["action"]=="subscribe") {
      const char* topic = doc["topic"];
      bool subscribed = pubsubClient.subscribe(topic);
      if(!subscribed) { doc["code"] = 1; }
      else { doc["code"] = 0; }
    }
    
    String response;
    response.reserve(1024);
    serializeJson(doc, response);
    request->send(200, "application/json", response);
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

// Enable OTA updates
void SetupOTA() {
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
  Serial.print("Auxiliary task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) {
    ArduinoOTA.handle();
    if (pubsubClient.connected()) { pubsubClient.loop(); }
    // DELETING THIS DELAY WILL CRASH THE MCU
    delay(25);
  }
}

// Setup sequence
void setup() {
  delay(5000); // pwrLevel-up safety delay
  
  // Start serial server and connect to WiFi
  Serial.begin(115200);
  while (!Serial);

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
                    "Auxiliary",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
}

// Main loop
void loop() { }
