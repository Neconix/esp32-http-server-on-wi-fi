# ESP32 http server with wi-fi connection
This project uses the http server implementation from **ESP-IDF** framework on the **ESP32** board to display and change the state of pins. 
From a web browser you can monitoring ESP32 state. For page layout used **Bootstrap 5** and for performing requests - a plain **javascript**.
The **spiffs** file system is used to save html, css, js files into ESP32 internal flash.
The device connects to the network via **Wi-Fi** and receives an IP address via DHCP. The received address is logged to the ESP32 console (ESP-IDF monitor). The web server listens to standard port 80 using the HTTP protocol.

## Endpoints
**/** or **/index.html** - index page;
**/include** - included css, js files here.

# Software requirements
esp-idf v5.x

# Hardware Required

* An ESP development board.
* An USB cable for power supply and programming.

# Build and flash

## ESP-IDF from command line

```shell
git clone https://github.com/Neconix/esp32-http-server-on-wi-fi.git
cd esp32-http-server-on-wi-fi
idf.py set-target esp32
idf.py menuconfig
idf.py build
idf.py -p /dev/ttyUSB0 flash # where /dev/ttyUSB0 is a port to connected ESP32 board
idf.py -p /dev/ttyUSB0 monitor # used to see output from ESP_LOG
```
You should set `Wi-Fi SSID`, `Wi-Fi password` in menuconfig step.

With some Linux distributions, you may get the `Failed to open port /dev/ttyUSB0` error message when flashing the ESP32. Run something like this to add current user to `dialout` group:

```shell
sudo usermod -a -G dialout $USER
```
## VSCode

In VSCode with installed [ESP-IDF extension](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md):

- git clone https://github.com/Neconix/esp32-http-server-on-wi-fi.git
- open project folder in VSCode and run from command pallete (`Ctrl + Shift + P`):
- ESP-IDF: Add vscode configuration folder
- ESP-IDF: SDK configuration editor (menuconfig)
- Go to `Project Configuration` section in menuconfig and set `Wi-Fi SSID`, `Wi-Fi password`
- ESP-IDF: Select port to use
- ESP-IDF: Set espressif device target
- ESP-IDF: Build your project
- ESP-IDF: Flash (UART) your project
- ESP-IDF: Monitor your device

Or run ESP-IDF: Build, Flash and start a monitor on your device.

# Docs
esp-idf: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/

