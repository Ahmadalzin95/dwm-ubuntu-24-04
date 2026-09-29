#!/usr/bin/env bash

# Variables
REPO_ROOT="$HOME/suckless"
THEME_NAME="Material-Black-Blueberry"
CURSOR_NAME="macOS"
THEME_FILE="$REPO_ROOT/assets/Material-Black-Blueberry-2.9.9-07.tar"
CURSOR_FILE="$REPO_ROOT/assets/macOS.tar.xz"
BOOT_ZIP="$REPO_ROOT/assets/linux-penguin.zip"
ASSET_LOGO="$REPO_ROOT/assets/ubuntu-logo.png"
SYSTEM_LOGO="/usr/share/plymouth/ubuntu-logo.png"
FONT_URL="https://github.com/ryanoasis/nerd-fonts/releases/latest/download/JetBrainsMono.zip"

echo "Starting system setup"

# Install system dependencies
sudo apt update
sudo apt install -y build-essential libx11-dev libxinerama-dev libxft-dev git feh \
    pipewire-audio-client-libraries libspa-0.2-bluetooth brightnessctl pamixer \
    dunst libnotify-bin arc-theme adwaita-icon-theme-full libdbus-1-dev \
    libxrandr-dev libxss-dev libglib2.0-dev libpango1.0-dev libgtk-3-dev \
    libxdg-basedir-dev libgdk-pixbuf-2.0-dev picom maim slop xclip xdotool \
    nsxiv plymouth plymouth-themes libpam-systemd gtk2-engines-murrine \
    gtk2-engines-pixbuf unzip xdg-desktop-portal-gtk x11-xserver-utils imagemagick curl \
    ncal autorandr arandr

# Image viewer defaults
xdg-mime default nsxiv.desktop image/jpeg
xdg-mime default nsxiv.desktop image/png
xdg-mime default nsxiv.desktop image/gif

# Chromium-family browsers: fix wrong content-size rendering under tiling WMs.
mkdir -p "$HOME/.local/share/applications"
for base in brave-browser google-chrome google-chrome-stable chromium chromium-browser microsoft-edge; do
    src="/usr/share/applications/${base}.desktop"
    [ -f "$src" ] || continue
    case "$base" in
        brave-browser)                      proc=brave ;;
        google-chrome|google-chrome-stable) proc=chrome ;;
        chromium|chromium-browser)          proc=chromium ;;
        microsoft-edge)                     proc=msedge ;;
    esac
    sed -E "s#^Exec=([^ ]+)#Exec=chromium-tiled-launch $proc \1#" \
        "$src" > "$HOME/.local/share/applications/${base}.desktop"
done
update-desktop-database "$HOME/.local/share/applications" 2>/dev/null || true

# Prepare directories
mkdir -p "$HOME/.config/dunst" "$HOME/.config/gtk-3.0" "$HOME/.config/gtk-4.0" \
         "$HOME/.themes" "$HOME/.icons" "$HOME/.local/bin" "$HOME/.dwm" \
         "$HOME/.local/share/fonts"

# Install i3lock-color from source
if ! command -v i3lock-color >/dev/null 2>&1; then
    echo "Building i3lock-color"
    sudo apt install -y autoconf gcc make pkg-config libpam0g-dev libcairo2-dev \
        libfontconfig1-dev libxcb-composite0-dev libev-dev libx11-xcb-dev \
        libxcb-xkb-dev libxcb-xinerama0-dev libxcb-randr0-dev libxcb-image0-dev \
        libxcb-util0-dev libxcb-xrm-dev libxkbcommon-dev libxkbcommon-x11-dev libjpeg-dev libgif-dev
    TEMP_DIR=$(mktemp -d)
    git clone https://github.com/Raymo111/i3lock-color.git "$TEMP_DIR"
    cd "$TEMP_DIR" && ./install-i3lock-color.sh
    cd "$REPO_ROOT"
fi

# Betterlockscreen setup
BL_INSTALLER="$REPO_ROOT/assets/betterlockscreen_install.sh"
FIXED_WALL="$REPO_ROOT/assets/lock-wp.jpg"
if [ -f "$BL_INSTALLER" ]; then
    chmod +x "$BL_INSTALLER"
    sudo "$BL_INSTALLER" system latest true
    [ -f "$FIXED_WALL" ] && betterlockscreen -u "$FIXED_WALL" --fx blur
fi

# Extract theme and icons
[ -f "$THEME_FILE" ] && tar -xf "$THEME_FILE" -C "$HOME/.themes/" && tar -xf "$THEME_FILE" -C "$HOME/.icons/"
if [ -f "$CURSOR_FILE" ]; then
    sudo mkdir -p /usr/share/icons
    sudo tar -xf "$CURSOR_FILE" -C /usr/share/icons/
fi

# Compile suckless tools
for tool in dwm dmenu slstatus; do
    if [ -d "$REPO_ROOT/$tool" ]; then
        echo "Installing $tool"
        cd "$REPO_ROOT/$tool" && sudo make clean install
    fi
done
cd "$REPO_ROOT"

# Install JetBrainsMono Nerd Font
echo "Installing JetBrainsMono Nerd Font..."
TEMP_FONT_DIR=$(mktemp -d)

curl -L "$FONT_URL" -o "$TEMP_FONT_DIR/JetBrainsMono.zip"
unzip -o "$TEMP_FONT_DIR/JetBrainsMono.zip" -d "$HOME/.local/share/fonts"

rm -rf "$TEMP_FONT_DIR"
fc-cache -fv
echo "Font installation complete."

# Link scripts and autostart
chmod +x "$REPO_ROOT/scripts/"*.sh
ln -sf "$REPO_ROOT/scripts/autostart.sh" "$HOME/.dwm/autostart.sh"

# GTK and Gnome configuration
cat <<EOF > "$HOME/.config/gtk-3.0/settings.ini"
[Settings]
gtk-theme-name=$THEME_NAME
gtk-icon-theme-name=$THEME_NAME
gtk-cursor-theme-name=$CURSOR_NAME
gtk-cursor-theme-size=24
gtk-font-name=JetBrainsMono Nerd Font 10
gtk-application-prefer-dark-theme=1
EOF
cp "$HOME/.config/gtk-3.0/settings.ini" "$HOME/.config/gtk-4.0/settings.ini"

gsettings set org.gnome.desktop.interface gtk-theme "$THEME_NAME"
gsettings set org.gnome.desktop.interface icon-theme "$THEME_NAME"
gsettings set org.gnome.desktop.interface cursor-theme "$CURSOR_NAME"
gsettings set org.gnome.desktop.interface cursor-size 24
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark'

# Wallust installation
WAL_TAG="3.4.0"
WAL_FILE="3.3.0"
WAL_URL="https://codeberg.org/explosion-mental/wallust/releases/download/${WAL_TAG}/wallust-${WAL_FILE}-x86_64-unknown-linux-musl.tar.gz"
TEMP_DIR=$(mktemp -d)
curl -L "$WAL_URL" -o "$TEMP_DIR/wallust.tar.gz"
tar -xzf "$TEMP_DIR/wallust.tar.gz" -C "$TEMP_DIR"
sudo mv "$TEMP_DIR/wallust" /usr/local/bin/
sudo chmod +x /usr/local/bin/wallust

# Wallust templates and config
mkdir -p "$HOME/.config/wallust/templates"
ASSETS_DIR="$REPO_ROOT/assets/wallust-setup"
if [ -d "$ASSETS_DIR" ]; then
    cp "$ASSETS_DIR/wallust.toml" "$HOME/.config/wallust/wallust.toml"
    cp "$ASSETS_DIR/xresources.template" "$HOME/.config/wallust/templates/"
    cp "$ASSETS_DIR/dunstrc.template" "$HOME/.config/wallust/templates/"
    cp "$ASSETS_DIR/sequences.template" "$HOME/.config/wallust/templates/"
fi

# Shell and Xresources integration
BASHRC_LINE='[ -f "$HOME/.cache/wallust/sequences" ] && source "$HOME/.cache/wallust/sequences"'
grep -qF "$BASHRC_LINE" "$HOME/.bashrc" || echo -e "\n$BASHRC_LINE" >> "$HOME/.bashrc"

cat <<EOF > "$HOME/.Xresources"
Xcursor.theme: $CURSOR_NAME
Xcursor.size: 24
#include "$HOME/.cache/wallust/colors.Xresources"
EOF
mkdir -p "$HOME/.cache/wallust"
touch "$HOME/.cache/wallust/colors.Xresources"

# Enable tap-to-click for touchpads via libinput
echo "Configuring X11 touchpad settings..."
sudo mkdir -p /etc/X11/xorg.conf.d/
sudo tee /etc/X11/xorg.conf.d/30-touchpad.conf > /dev/null << 'EOF'
Section "InputClass"
    Identifier "libinput touchpad catchall"
    MatchIsTouchpad "on"
    MatchDevicePath "/dev/input/event*"
    Driver "libinput"
    Option "Tapping" "on"
EndSection
EOF

# System logo and boot splash
if [ -f "$ASSET_LOGO" ]; then
    [ -f "$SYSTEM_LOGO" ] && [ ! -f "$SYSTEM_LOGO.back" ] && sudo mv "$SYSTEM_LOGO" "$SYSTEM_LOGO.back"
    sudo cp "$ASSET_LOGO" "$SYSTEM_LOGO"
fi

if [ -f "$BOOT_ZIP" ]; then
    sudo unzip -o "$BOOT_ZIP" -d /usr/share/plymouth/themes/
    sudo update-alternatives --install /usr/share/plymouth/themes/default.plymouth default.plymouth /usr/share/plymouth/themes/linux-penguin/linux-penguin.plymouth 200
    sudo update-alternatives --set default.plymouth /usr/share/plymouth/themes/linux-penguin/linux-penguin.plymouth
    sudo update-initramfs -u
fi

echo "Setup complete"