#!/bin/sh

#
#スキャナより画像を読み込むShellScript
#


##===ドライバのインストール方法===###
#https://wiki.archlinuxjp.org/index.php/Sane
#上のページを参考にインストールする。

##以下自機で実行したコマンド
# sudo pacman -S sane
# yaourt -S brscan3
# /usr/local/Brother/sane/setupSaneScan3 -i


#任意のディレクトリに保存
#相対パスの指定の仕方がよくわからないので、/home直下に

scanimage --format=png > ~/test.png
