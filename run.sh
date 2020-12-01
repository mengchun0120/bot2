#!/bin/bash

map=""
if [ "$#" != "0" ]; then
    map="--map res/map/$1"
fi

build/bot --log log.txt --cfg config/bot_cfg.json --app . $map

