#!/bin/bash

# Get today's date in YYYY.MM.DD format
TODAY=$(date +%Y.%m.%d)

# Parse the optional flag for a custom date
TARGET_DATE=$TODAY
while getopts "d:" opt; do
    case $opt in
        d) TARGET_DATE=$OPTARG ;;
        *) echo "Usage: $0 [-d YYYY.MM.DD]" >&2
           exit 1 ;;
    esac
done

# Construct the target path
TARGET_PATH="./data/$TARGET_DATE"

# Check if the target directory exists
if [ -d "$TARGET_PATH" ]; then
    echo "Removing all files in $TARGET_PATH..."
    rm -rf "$TARGET_PATH"/*
    echo "All files removed from $TARGET_PATH."
else
    echo "Directory $TARGET_PATH does not exist."
fi
