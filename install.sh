#!/bin/bash

# MobPaint Dependency Installer for Arch Linux
# Usage: chmod +x install.sh && ./install.sh

echo "[MobPaint] Installing dependencies..."

sudo pacman -S --needed --noconfirm sdl2 sdl2_gfx

if [ $? -eq 0 ]; then
    echo "[MobPaint] ✅ Dependencies installed successfully."
else
    echo "[MobPaint] ❌ Installation failed. Please install manually."
    exit 1
fi
