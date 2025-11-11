#!/bin/bash
sudo pacman -S --needed grub os-prober efibootmgr
sed -i 's/#GRUB_DISABLE_OS_PROBER=false/GRUB_DISABLE_OS_PROBER=false/' /etc/default/grub
grep -qxF "GRUB_DISABLE_OS_PROBER=false" /etc/default/grub || echo "GRUB_DISABLE_OS_PROBER=false" >> /etc/default/grub
os-prober
grub-mkconfig -o /boot/grub/grub.cfg