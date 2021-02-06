# Quick Start Guide

I use the <a href="https://platformio.org/platformio-ide">PlatformIO extension in VS Code</a>, but it's actually quicker to use the command line:

```sh 
# Install PlatformIO
user@host:~$ python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
```

OPTIONAL (Linux/Mac) - I also like to create the alias "platformio" to quickly activate the PlatformIO virtual environment on the command line:

```sh 
# Create alias
user@host:~$ echo "alias platformio=\". ~/.platformio/penv/bin/activate\"" >> ~/.bashrc && . ~/.bashrc
# Try it out!
user@host:~$ platformio
# You should now see
(penv) user@host:~$
```

Configure [`platform.ini`](platform.ini) for USB upload:

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = userver.local
```

Move to root of repository and upload both the file system and program via IDE or command line as shown below (make sure penv is activated first):

```sh
(penv) user@host:~$ cd <uServer>
(penv) user@host:<uServer>$ pio run -t uploadfs
(penv) user@host:<uServer>$ pio run -t upload
```

Go to your phone/PC's Wi-Fi settings and use access point "uServer" to connect the ESP32 to your Wi-Fi, then finalize your [`platform.ini`](platform.ini) configuration:

```ini
# I leave these comments in case I need to troubleshoot locally
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
monitor_speed = 115200
upload_protocol = espota
upload_port = userver.local
```

Your device is permanently configured to connect to your Wi-Fi on bootup so future uploads can now be over-the-air! Connect to your device on the same network at http://userver.local/.
