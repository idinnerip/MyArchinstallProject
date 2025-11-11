#!/bin/bash
set -euo pipefail

IFACE=$(ip -4 route get 1.1.1.1 | awk '{for(i=1;i<=NF;i++) if($i=="dev") print $(i+1)}')
CIDR=$(ip -4 -o addr show dev "$IFACE" | awk '{print $4}')

declare -A MACMAP
while read -r ip mac; do
    MACMAP["$ip"]=$mac
done < <(sudo arp-scan --interface="$IFACE" --localnet 2>/dev/null | awk '/^[0-9]/{print $1, $2}')

declare -A HOSTS
while read -r ip; do
    HOSTS["$ip"]=1
done < <(sudo nmap -sn "$CIDR" -oG - | awk '/Up$/{print $2}')

for ip in "${!HOSTS[@]}"; do
    mac="${MACMAP[$ip]:-N/A}"
    name=$(getent hosts "$ip" | awk '{print $2}' || true)
    if [[ -z "$name" ]] && command -v avahi-resolve-address >/dev/null 2>&1; then
        name=$(avahi-resolve-address "$ip" 2>/dev/null | awk '{print $2}' || true)
    fi
    [[ -z "$name" ]] && name="N/A"
    printf "%-15s\t%-17s\t%s\n" "$ip" "$mac" "$name"
done | sort -V
