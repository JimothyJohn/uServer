# Quick Start Guide

I use the <a href="https://platformio.org/platformio-ide">PlatformIO extension in VS Code</a>, but it's actually quicker to use the command line:

```bash 
# Install PlatformIO
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
```

OPTIONAL (Linux) - I also like to create the alias "platformio" to quickly activate the PlatformIO virtual environment on the command line:

```bash 
# Create alias
echo "alias platformio=\". ~/.platformio/penv/bin/activate\"" >> ~/.bashrc && . ~/.bashrc
# Try it out!
platformio
echo "You should now see: (penv) user@host:~$"
```

Configure [platform.ini](platform.ini) for USB upload:

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NOT ON LINUX
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NOT ON LINUX
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = userver.local
```

Move to root of repository and upload both the file system and program via IDE or command line as shown below (make sure penv is activated first):

```bash
git clone https://github.com/JimothyJohn/uServer/
cd uServer
echo "Make sure (penv) is activated!"
pio run -t upload && pio run -t uploadfs
```

Go to your phone/PC's Wi-Fi settings and use access point "uServer" to connect the ESP32 to your Wi-Fi.

Your device is permanently configured to connect to your Wi-Fi on bootup. Connect to your device on the same network at http://userver.local/.
