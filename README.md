# esp-iot-quickstart

A project template for developing IoT projects using the [Espressif Systems ESP32](https://www.espressif.com/en/products/hardware/esp32/overview). Contains the boilerplate required to connect to a wireless network and MQTT broker. Intended to serve as a starting point, and to help avoid repeating myself.


# Usage

To develop a project from this template, begin by checking out the repository, making sure to use the `--recursive` flag to ensure we check out the [espressif/esp-mqtt](https://github.com/espressif/esp-mqtt) component as well:

```bash
$ git clone --recursive https://github.com/jessebraham/esp-iot-quickstart.git
$ cd esp-iot-quickstart/
```

We can use the `make` command to perform a number of helpful tasks. To configure the wireless network and MQTT broker settings, run `make menuconfig` from the project root. From here you will be greeted with a [curses](https://en.wikipedia.org/wiki/Curses_%28programming_library%29) interface, from which you can select the `esp-iot-quickstart config` item.

Within `main/main.c` lies a function named `mqtt_event_handler`; there are a number of comments within it instructing what to add for your application.

Once you're ready to flash the device, run:

```bash
$ make flash
$ make monitor
```
