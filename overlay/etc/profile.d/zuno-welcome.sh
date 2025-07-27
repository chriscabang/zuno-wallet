#!/bin/bash

IP=$(ip -4 addr show wlan0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}' || echo "No IP")

# Get SSID (when connected)
SSID=$(iw wlan0 link | grep SSID | cut -d ':' -f2- | xargs || echo "Disconnected")

# Show uptime
UPTIME=$(uptime -p | sed 's/up //')

# CPU temp (Pi): /sys/class/thermal/... is available on Arch ARM
CPU_TEMP=$(awk '{printf "%.1f°", $1/1000}' /sys/class/thermal/thermal_zone0/temp 2>/dev/null)

# Get memory usage
MEMORY=$(free -m | awk '/Mem:/ { printf "%dMB used / %dMB", $3, $2 }')

# Get disk temparature
# DISK=$(lsblk -dno NAME,TYPE | awk '$2 == "disk" {print $1; exit}')
# DISK_TEMP=$(sudo hddtemp /dev/$DISK 2>/dev/null | awk -F: '{ print $3 }' | xargs || echo "N/A")

# Get current IP
echo
echo -e "\e[1;36mIn the jungle you must wait until the dice read five or eight. - Alan Parrish\e[0m"
echo
printf "    IP Address   : %-22s \n" "$IP"
printf "    SSID         : %-22s \n" "$SSID"
printf "    Uptime       : %-22s \n" "$UPTIME"
printf "    CPU Temp     : %-22s \n" "$CPU_TEMP"
printf "    Memory Usage : %-22s \n" "$MEMORY"
# printf "    Disk Temp    : %-22s |\n" "$DISK_TEMP"
echo
echo
