#!/bin/bash

# MobPaint Multi-Distro Dependency Installer
# Usage: chmod +x install.sh && ./install.sh

DEPENDENCIES=("sdl2" "sdl2_gfx" "sdl2_image" "sdl2_ttf" "cjson")

echo "[MobPaint] Detecting Linux distribution..."

# Lowercase and simplified distro id
DISTRO_ID=$(grep '^ID=' /etc/os-release | cut -d= -f2 | tr -d '"' | tr '[:upper:]' '[:lower:]')

install_with_pacman() {
    echo "[MobPaint] Installing dependencies with pacman..."
    sudo pacman -S --needed --noconfirm "${DEPENDENCIES[@]}"
}

install_with_apt() {
    echo "[MobPaint] Updating apt and installing dependencies..."
    sudo apt update
    sudo apt install -y libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev libcjson-dev
}

install_with_dnf() {
    echo "[MobPaint] Installing dependencies with dnf..."
    sudo dnf install -y SDL2-devel SDL2_gfx-devel SDL2_image-devel cjson-devel
}

install_with_zypper() {
    echo "[MobPaint] Installing dependencies with zypper..."
    sudo zypper install -y libSDL2-devel libSDL2_gfx-devel libSDL2_image-devel libcjson-devel
}

case "$DISTRO_ID" in
    arch|manjaro)
        install_with_pacman
        ;;
    ubuntu|debian|linuxmint|pop)
        install_with_apt
        ;;
    fedora)
        install_with_dnf
        ;;
    opensuse*|suse)
        install_with_zypper
        ;;
    *)
        echo "[MobPaint] ❌ Unsupported distribution: $DISTRO_ID"
        echo "Please install the following dependencies manually:"
        echo "  - SDL2"
        echo "  - SDL2_gfx"
        echo "  - SDL2_image"
        echo "  - cJSON"
        exit 1
        ;;
esac

if [ $? -eq 0 ]; then
    echo "[MobPaint] ✅ Dependencies installed successfully."
else
    echo "[MobPaint] ❌ Installation failed. Please install manually."
    exit 1
fi
