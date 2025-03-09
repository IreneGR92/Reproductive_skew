#!/bin/bash
git pull

screen -d -S "reproductive_skew" -m ./build_and_run.sh
screen -list
