#!/bin/bash

# Configuration
BAUD=115200
echo "~~ Rider Kontrol - ESP32 CLI Serial Monitor 👁️  ~~"
echo "Searching for ESP32-C3... (Press Ctrl+C twice to stop)"

while true; do
    # 1. Check if a USB upload is currently running (Note: OTA is non issue since using Wifi)
    if pgrep -f "arduino-cli upload" > /dev/null; then
        echo "--- Upload in progress... waiting ---"
        sleep 2
        continue
    fi

    # 2. Find the USB port
    PORT=$(ls /dev/cu.usb* 2>/dev/null | head -n 1)

    if [ -z "$PORT" ]; then
        # Port not found, wait and try again
        sleep 2
    else
        # 3. Start the monitor
        arduino-cli monitor -p "$PORT" --config baudrate=$BAUD
        
        # 4. If the monitor exits (reboot/unplug), wait for the port to stabilize
        echo "--- ‼️ Connection Lost. Retrying... ---"
        sleep 1
    fi
done