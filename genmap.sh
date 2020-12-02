#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 map_file robot_count"
    exit 1
fi

map_file=$1
robot_count=$2

build/bot --mode showmap --map $map_file --mapGenerator islandMapGenerator \
          --robotCount $robot_count --log log.txt --cfg config/bot_cfg.json \
          --app .

