#!/bin/sh

devicename=$(scanimage -L | sed -r '/scanner/!d'| sed -r "s/(^.+\`)|('.+$)//g")
echo ${devicename}
wait
scanimage --device "${devicename}" --format=png --mode Color --resolution 480 > hoge.png
