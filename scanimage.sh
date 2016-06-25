#!/bin/sh

#
#スキャナより画像を読み込むShellScript
#

#sh scanimage.sh
#上のコマンドで実行

##===ドライバのインストール方法===###
#https://wiki.archlinuxjp.org/index.php/Sane
#上のページを参考にインストールする。

##以下自機で実行したコマンド
#ただしMFC-6890CN(プリンタの型番)用
# sudo pacman -S sane
# yaourt -S brscan3
# /usr/local/Brother/sane/setupSaneScan3 -i


#このスクリプトが置かれている場所/CompImage下(./CompImage/)にファイルを保存する
#$script_dir_pathが./に相当する
#SECONDSは時間計測用．BashScriptが起動している間は毎秒1ずつ増える変数

script_dir_path=$(dirname $(readlink -f $0))

SECONDS=0
scanimage --format=png --resolution 300 > $script_dir_path/CompImage/dpi300test.png
echo time=$SECONDS
SECONDS=0
scanimage --format=png --resolution 600 > $script_dir_path/CompImage/dpi600test.png
echo time=$SECONDS
