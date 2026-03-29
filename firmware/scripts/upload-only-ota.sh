#!/bin/bash

# TODO: Use mDNS
SKETCH_NAME="firmware.ino"
IP_ADDRESS="192.168.1.239"
ESP32_VERSION="2.0.17"
OTA_TOOL="$HOME/Library/Arduino15/packages/esp32/hardware/esp32/$ESP32_VERSION/tools/espota.py"
OTA_PORT=3232
# TODO: Change to the OTA password you set via the serial command line (command: otapass)
OTA_PASSWORD="!!TODO!!"

echo "--- 🔵 Performing OTA Upload of firmware to $IP_ADDRESS ---"

# Use the exported binary from the build folder
# TODO: Change hardcoded OTA password
python3 "$OTA_TOOL" \
  -i "$IP_ADDRESS" \
  -f "./build/$SKETCH_NAME.bin" \
  -p $OTA_PORT \
  -a $OTA_PASSWORD \
  -d \
  --progress

if [ $? -eq 0 ]; then
    echo "✅ OTA Upload Success!"
else
    echo "❌ OTA Upload Failed."
    exit 1
fi