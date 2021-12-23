#!/usr/bin/env bash

# Sync file system config to API
echo "Updating API config files..."
rsync -azp ./data/config/ ./api/config/

# Update live demo app
echo "Updating live demo..."
cp ./data/App.js ./docs/App.js
cp ./data/style.css ./docs/style.css

# Sync repo to Jetson Nano in archive/compressed/verbose mode
# Do not include build and environment files
echo "Syncing repo..."
rsync -azp --exclude={'.pio','.vscode'} ./ nick@jetson.home:/home/nick/github/uServer
