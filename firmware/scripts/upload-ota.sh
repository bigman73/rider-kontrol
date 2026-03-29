#!/bin/bash

echo "🟠 Starting Build (OTA)..."

if bash ./scripts/compile.sh; then
    bash ./scripts/upload-only-ota.sh
else
    echo "❌ Compile failed! Aborting upload."
    exit 1
fi