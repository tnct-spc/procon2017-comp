#!/bin/sh

devicename=$(cat devicename.txt)

if [ "${devicename}" = ""  ]; then
    # エラー処理
    echo devicename.txtの中身が確認できません
    exit 2
fi

wait
scanimage --device "${devicename}" --format=png --mode Color --resolution 300 > $1

if [ $? -gt 0 ]; then
    # エラー処理
    echo スキャン実行時にトラブルがありました
    exit 2
fi
