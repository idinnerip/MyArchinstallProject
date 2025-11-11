#!/bin/sh
git clone https://aur.archlinux.org/yay.git /tmp/yay && cd /tmp/yay && makepkg -si --noconfirm && cd ~ && rm -rf /tmp/yay
cd && yay -Syu --noconfirm ttf-ms-fonts ungoogled-chromium-bin onlyoffice-bin