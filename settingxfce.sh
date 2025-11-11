#!/bin/sh

# Copy backgrounds
if [ -d "sample-background" ]; then
    sudo mkdir -p /usr/share/backgrounds/
    sudo cp -r sample-background /usr/share/backgrounds/
fi

# Appearance
xfconf-query -c xsettings -p /Net/ThemeName -s "Orchis-Dark-Compact"
xfconf-query -c xsettings -p /Net/IconThemeName -s "Papirus-Dark"
xfconf-query -c xsettings -p /Gtk/FontName -s "Noto Sans Medium 10"
xfconf-query -c xfwm4 -p /general/title_font -s "Noto Sans Medium 10"
xfconf-query -c xsettings -p /Gtk/MonospaceFontName -s "JetBrains Mono Medium 10"

# Disable XFWM compositor (Picom users only)
xfconf-query -c xfwm4 -p /general/use_compositing -s false

# Keyboard shortcut: Super only → whisker menu
xfconf-query -c xfce4-keyboard-shortcuts \
    -p "/commands/custom/<Super>Super_L" \
    -n -t string -s "xfce4-popup-whiskermenu"

# Mouse: disable adaptive acceleration
xfconf-query -c pointers -p /Default/AccelerationProfile -s 0

# Power settings
xfconf-query -c xfce4-power-manager -p /xfce4-power-manager/power-button-action -s 4
xfconf-query -c xfce4-power-manager -p /xfce4-power-manager/sleep-button-action -s 1
xfconf-query -c xfce4-power-manager -p /xfce4-power-manager/critical-power-action -s 1

# Autostart entries
mkdir -p ~/.config/autostart

cat > ~/.config/autostart/ibus.desktop <<EOF
[Desktop Entry]
Type=Application
Name=IBus
Exec=ibus start
EOF

cat > ~/.config/autostart/mousefix.desktop <<EOF
[Desktop Entry]
Type=Application
Name=Mouse Button Fix
Exec=xinput set-button-map "SIGMACHIP Usb Mouse" 1 0 3 4 5 6 7
EOF