#!/bin/bash

echo "🟠 Starting Build (USB)..."

if bash ./scripts/compile.sh; then
    bash ./scripts/upload-only-usb.sh
else
    echo "❌ Compile failed! Aborting upload."
    exit 1
fi