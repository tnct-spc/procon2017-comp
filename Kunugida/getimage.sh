#!/bin/sh

devicename=$(scanimage -L | sed -r '/scanner/!d'| sed -r "s/(^.+\`)|('.+$)//g")
wait
scanimage --device "${devicename}" --format=png --mode Color --resolution 100 > hoge.png

if [ $? -gt 0 ]; then
    # エラー処理
    exit 2
fi
