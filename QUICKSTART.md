# Quick Start Guide - Unix (Linux/Mac)

Start by installing PlatformIO with:

```sh 
# Install PlatformIO
user@host:~$ python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
```

OPTIONAL - I use the <a href="https://platformio.org/platformio-ide">PlatformIO extension in VS Code</a>, but I also like to create an alias to quickly activate PlatformIO on the command line.

```sh 
# Create alias
user@host:~$ echo "alias platformio=\"source ~/.platformio/penv/bin/activate\"" >> ~/.bashrc && source ~/.bashrc
# Try it out!
user@host:~$ platformio
# You should now see
(penv) user@host:~$
```

Update [`platform.ini`](platform.ini) configuration

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = userver.local
```

Move to root of repository and upload via VS Code or command line (make sure penv is activated first!)

```sh
(penv) user@host:~$ cd <uServer>
(penv) user@host:<uServer>$ pio run -t upload
```

Go to your phone/PC's Wi-Fi settings and use access point "uServer" to connect the ESP32 to your Wi-Fi, then finalize your [`platform.ini`](platform.ini) configuration

```ini
# Leave as comments in case you want to troubleshoot locally
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
monitor_speed = 115200
upload_protocol = espota
upload_port = userver.local
```

Your device is permanently configured to connect to your Wi-Fi on bootup so future uploads can now be over-the-air!
