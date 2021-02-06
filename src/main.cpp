#include <Arduino.h> //Base framework
#include <WiFiManager.h> // AP login and maintenance
#include <AsyncTCP.h> // Generic async library
#include <ESPAsyncWebServer.h> // ESP32 async library
#include <ArduinoOTA.h> // Enable OTA updates
#include <ESPmDNS.h> // Connect by hostname
#include <SPIFFS.h> // Enable file system
AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// SPIFFS uses processor to replace HTML text with variables
String processor(const String& var) {
  if(var == "TEST_VAR") {return "first variable";}
  if(var == "TEST_VAR_AGAIN") {return "second variable";}
}

// Set up server callback functions
void SetupServer() {
  Serial.print('Configuring webserver...');
  // Index page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/js");
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
void SetupWiFi() {
  Serial.print('Configuring WiFi...');
  WiFi.mode(WIFI_STA); // make sure your code sets wifi mode

  WiFiManager wm;

  bool res = wm.autoConnect("uServer"); // anonymous ap

  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart();
  } 
  else {
      Serial.println("Connected to WiFi!");
  }
}

// Setup sequence
void setup() {
  delay(1000); // pwrLevel-up safety delay
  
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
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
}

// Main loop
void loop() {
  ArduinoOTA.handle();
}
