# Rider Kontrol

<img src="assets/logo.png" width="600" />

A Bluetooth Low Energy (BLE) navigation remote for riders, by riders.

There’s nothing more frustrating (or dangerous) than trying to poke at a touchscreen with thick gloves while cruising at 65+ mph.

`Rider Kontrol` is a DIY wireless keyboard designed to mount directly to your motorcycle's handlebars. It allows you to control navigation apps (like DMD2, Gaia GPS, OsmAnd and other) without taking your hands off the grips.

## 🚀 Features

- Glove-Friendly: Large tactile buttons for easy navigation while riding.
- BLE Connectivity: Mimics a standard HID keyboard — no companion app required.
- Versatility: Built in and custom key maps, to support different navigation apps
- Usability: Supports short and long button press (e.g., continous pan or zoom)
- Modality: Has 4 different modes - Navigation (default), Configuration, Diagnostics and OTA (Over The Air firmware updates)
- Deployment: Over-The-Air Updates allow updating the firmware without connecting the USB-C cable
- Low Power: Optimized for the ESP32-C3 to ensure long battery life between charges, powered by a 5V USB (direct from motorcycle cockpit USB port, or converted from the motorcycle's 12V battery).
- Weather Resistant: Designed to be housed in a 3D-printed, sealed enclosure.
- Low cost: DYI project with fully open source firmware

## 🏍️ Usage

<img src="https://raw.githubusercontent.com/bigman73/rider-kontrol/main/assets/rider-kontrol-assembled.jpeg" alt="Assembled Unit" width="300" />

<img src="https://raw.githubusercontent.com/bigman73/rider-kontrol/main/assets/rider-kontrol-mount-701.jpeg" alt="Mounted on a Husqvarna 701 Enduro" width="300" />

TODO: video of Rider Kontrol mounted on a motorcycle

## 🛠️ Hardware

This project is built around the [Lolin C3 mini board](https://www.wemos.cc/en/latest/c3/c3_mini.html), which can be programmed with the Arduino IDE. Its compact footprint and native BLE and Wifi support make it the perfect brain for a sleek handlebar remote.
The board uses the `ESP32-C3FH4` microcontroller.

## ⚠️ Safety & Legal Disclaimer

Ride at your own risk. This project is a DIY experimental tool and is provided "as is" without any warranties.
Attention to the Road: The primary purpose of this controller is to reduce distraction. However, no device is a substitute for your full attention. Never attempt to perform complex configurations or troubleshooting while the motorcycle is in motion.
Mechanical Safety: Ensure the device and its mounting hardware do not interfere with the motorcycle’s controls (throttle, brakes, clutch, or steering sweep). Check the security of your mount before every ride.
Local Laws: Laws regarding the use of electronic devices while operating a motor vehicle vary by region. It is your responsibility to ensure compliance with local traffic regulations.
Liability: The author(s) of this project are not responsible for any equipment damage, personal injury, or legal issues resulting from the use or assembly of this device.
