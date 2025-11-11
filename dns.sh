#!/bin/bash
set -e
if [[ $EUID -ne 0 ]]; then
    exit 1
fi

if systemctl is-active --quiet systemd-resolved; then
    resolvectl dns 0 8.8.8.8 8.8.4.4
    resolvectl domain 0 "~."
else
    chattr -i /etc/resolv.conf 2>/dev/null || true
    printf "nameserver 8.8.8.8\nnameserver 8.8.4.4\n" > /etc/resolv.conf
    chattr +i /etc/resolv.conf 2>/dev/null || true
fi
