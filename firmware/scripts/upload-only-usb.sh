#!/bin/bash

# --- CONFIGURATION ---
BOARD="esp32:esp32:esp32c3"
# Ensure this matches the exact name in your boards.txt
# 1.9MB, OTA
PARTITION_SCHEME="min_spiffs" 

# On Mac, the port usually looks like /dev/cu.usbserial-XXXX or /dev/cu.usbmodemXXXX
PORT=$(ls /dev/cu.usb* | head -n 1)

# 1. Kill the serial monitor if it's running
if pgrep -f "arduino-cli monitor" > /dev/null; then
    echo "Closing active serial monitor..."
    pkill -f "arduino-cli monitor"
    sleep 1 # Give macOS a second to release the file handle
fi

echo "--- 🔵 Uploading firmware via USB to port: $PORT ---"

# The --verify flag ensures the flash was successful
# Optional: The --erase-all flag is CRITICAL if your NVS was corrupted
arduino-cli upload \
  -p $PORT \
  -v \
  --fqbn $BOARD \
  --board-options "PartitionScheme=$PARTITION_SCHEME,UploadSpeed=115200,CDCOnBoot=cdc,CPUFreq=160,FlashMode=dio,FlashSize=4M,FlashFreq=40,DebugLevel=none,EraseFlash=none,JTAGAdapter=default" \
  --input-dir ./build \
  --verify

if [ $? -eq 0 ]; then
    echo "✅ USB Upload Success!"
else
    echo "❌ USB Upload Failed. Check your cable or Port permissions."
    exit 1
fi