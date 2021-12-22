# uServer

Template for rapid IoT development on ESP32. Build webpages fast and exchange data quickly!

## <a href="https://jimothyjohn.github.io/uServer/">Live demo</a>

## Features

See [Features](FEATURES.md) for full list of features and examples.

## Hardware Required

* This project is only tested on an <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a> (China)

## Software Required

* <a href="https://github.com/platformio/platformio-core">PlatformIO</a>

## Installation

See [Quickstart](QUICKSTART.md) to quickly get up and running.

## REST API 

uServer creates an API endpoint for simple I/O, variables, MQTT, and can read/write configurations files for persistent storage. See [API](API.md) for examples. Endpoints:

* /io - Read input statuses or change output states
* /variables - Change a user variable
* /mqtt - Connect, publish, and subscribe
* /files - List files in a directory
* /file - Read contents of .json configuration file
* /cloud - Read from cloud API's!

### FastAPI Testing

Repo contains a Docker image with a test endpoint for you:

```bash
docker-compose up -d --build
```

## To-do List

* Migrate to ESPAsync_WifiManager
* Add security/authentication
* Port to other platforms
* <a href="https://randomnerdtutorials.com/esp32-microsd-card-arduino">Integrate persistent storage via microSD</a>
