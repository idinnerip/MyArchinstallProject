#!/bin/bash

sed -i '/^\s*\[multilib\]/,/Include/d' /etc/pacman.conf

cat <<EOF >> /etc/pacman.conf

[multilib]
Include = /etc/pacman.d/mirrorlist
EOF

pacman -Syy
