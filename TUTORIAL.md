# Explanation

## The idea

You have one of the most amazing user interfaces in your pocket and Wi-Fi all around you, why not use them for microontrollers?

My goal was to create a framework that I could monitor and control future IoT projects with my phone.

### Hardware

* This project will only work on an <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a>.
* LED's, I use <a href="https://www.amazon.com/BTF-LIGHTING-Flexible-Individually-Addressable-Non-waterproof/dp/B01CDTEJBG">these LEDs from Amazon</a>(no affiliation).

### Software

I prefer using <a href="https://platformio.org/install/ide?install=vscode">PlatformIO's extension on VS Code</a>, but will showing it the easy way with the command line.

First install PlatformIO via pip (I'm assuming you don't have pip or python to be safe)

```sh
sudo apt-get -y install python3 python3-pip
pip3 install platformio
```

## Setup

Move to the root directory where you want the repo and clone

```sh
cd <TARGET DIRECTORY>
git clone https://github.com/JimothyJohn/LEDServer.git
cd LEDServer/
```

Depending on your OS you'll then need to modify your [`platform.ini`](platform.ini) configuration.

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = ledcontrol.local
```

While at the root of the repo upload the project

```sh
pio run -t upload
```

Go to your Wi-Fi settings and connect to the access point "LEDSetup". Open a browser to connect the ESP32 to your Wi-Fi.

Finalize the [`platform.ini`](platform.ini) configuration

```ini
# Leave as comments in case you need to troubleshoot
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
monitor_speed = 115200
upload_protocol = espota
upload_port = ledcontrol.local
```

All future uploads will now be over-the-air and your device is permanently configured to connect to your Wi-Fi on bootup!
