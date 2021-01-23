//Base framework
#include <Arduino.h>
#include <WebStrings.h> // HTML Data

// Load async web library
#include <WiFiManager.h> // Use WifiManager for login and maintenance
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h> // Enable OTA updates
#include <ESPmDNS.h> // Connect by hostname

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Set up server callback functions
// https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino
void SetupServer() {
  Serial.print('Configuring webserver...');
  // Index page, defaults to off
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", navHTML());
  });

  server.onNotFound(notFound);
  server.begin();
}

// Enable OTA updates
// https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino
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
    });

  ArduinoOTA.begin();
}

// Initialize Wi-Fi manager and connect to Wi-Fi
// https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino
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
  // Allows for connection at http://ledcontrol.local/
  if(!MDNS.begin("microserver")) {
    Serial.println("Error starting mDNS!");
    ESP.restart();
  }
}

void loop() {
  ArduinoOTA.handle();
}
