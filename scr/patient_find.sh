#!/bin/bash

#checks for empty string
if [ -z "$1" ]; then
    echo "Error: Please provide a patient name."
    echo "Usage: patient_find <name>"
    exit 1
fi

echo "Searching for: $1..."

FOUND=0

#-iR makes it case sensitive and recursive
for file in Patients/*/info.txt; do
    if grep -iq "Name: $1" "$file"; then
        echo "Patient Record Found."
        cat "$file"
        FOUND=1
    fi
done

if [ $FOUND -eq 0 ]; then
    echo "Patient not found in system."
fi
