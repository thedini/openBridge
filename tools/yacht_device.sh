#! /bin/bash

PORT=$1

pkill screen
sleep .5
stty -F /dev/$PORT hupcl
sleep .5
echo YDNU MODE SERVICE > /dev/$PORT
sleep .5
screen -a /dev/$PORT
