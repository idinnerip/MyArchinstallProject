#!/bin/bash

orphans=$(pacman -Qdtq)
[ -n "$orphans" ] && sudo pacman -Rns --noconfirm $orphans
yay -Scc --noconfirm
sudo journalctl --vacuum-time=1d
sudo rm -rf /tmp/*
