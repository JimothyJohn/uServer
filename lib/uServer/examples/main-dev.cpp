
// Load relevant libraries
#include <Arduino.h> //Base framework
#include <ArduinoOTA.h> // Enable OTA updates
#include <ESPmDNS.h> // Connect by hostname
#include <SPIFFS.h> // Enable file system
#include "uServer.h" // Include custom library

// Variable for Variables page, change to match your configuration
uint8_t userVariable = 0;

// Dual-core tasks
TaskHandle_t Task1;

// WebServer Task 
void Task1code( void * pvParameters ) {
  Serial.print("Auxiliary task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) {
    ArduinoOTA.handle();
    // if (pubsubClient.connected()) { pubsubClient.loop(); }
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
