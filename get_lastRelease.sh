#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$0")
echo $SCRIPT_DIR

NEW_VERSION=$(curl --silent "https://api.github.com/repos/$1/releases/latest" | \
    grep '"tag_name":' | \
    sed -E 's/.*"([^"]+)".*/\1/' | \
    sed 's/\.//g' )                                             
sed -i "s/#define LUAU_VERSION_NUM [0-9]\+/#define LUAU_VERSION_NUM $NEW_VERSION/g" $SCRIPT_DIR/luau_ext.h