#!/bin/bash

echo "========================================"
echo "HEALTHCARE OS - SYSTEM STATUS REPORT"
echo "========================================"

# Show current date and time
echo "Time: $(date)"

# Show how long the system has been running
echo "ptime: $(uptime -p)"

# Show disk usage (human readable format)
echo "Disk Space for Medical Records:"
df -h | grep '/$' 

# Show memory usage
echo "Memory Usage:"
free -h | grep "Mem"

echo "========================================"
