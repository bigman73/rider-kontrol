#!/bin/bash

# --- CONFIGURATION ---
SKETCH_NAME="firmware.ino"
BOARD="esp32:esp32:esp32c3"
# Ensure this matches the exact name in your boards.txt
PARTITION_SCHEME="min_spiffs" 

echo "--- 🔵 Compiling $SKETCH_NAME ---"

# Compile with the specific partition scheme defined in boards.txt
# optional: --verbose 
arduino-cli compile \
    --log-level info \
    --fqbn $BOARD \
    --board-options "PartitionScheme=$PARTITION_SCHEME,UploadSpeed=115200,CDCOnBoot=cdc,CPUFreq=160,FlashMode=dio,FlashSize=4M,FlashFreq=40,DebugLevel=none,EraseFlash=none,JTAGAdapter=default" \
    --output-dir ./build \
    ./$SKETCH_NAME

if [ $? -eq 0 ]; then
    echo "✅ Firmware Compilation Successful!"

    echo +
    echo ================================================
    # This reads the binary partition file and prints the human-readable table
    python3 ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.17/tools/gen_esp32part.py ./build/$SKETCH_NAME.partitions.bin
else
    echo "❌ Firmware Compilation Failed."
    exit 1
fi

