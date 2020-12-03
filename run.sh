#!/bin/bash

map=""
if [ $# -gt 0 ]; then
    map="--map $1"
fi

build/bot --log log.txt --cfg config/bot_cfg.json --app . $map

