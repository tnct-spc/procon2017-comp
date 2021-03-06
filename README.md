Cult of the Party Parrot
===
## 東京高専競技部門
第28回 全国高等専門学校プログラミングコンテストの競技部門のソースコード．

## 結果
#### 競技部門 特別賞受賞
### 各試合の結果
|　 | 順位 | 結果 |
|:---|:---|:---|
|第1回戦第1試合 | 第12位 | 敗退 |
|敗者復活戦第1試合 | 第2位 | 勝ち抜き |
|準決勝第1試合 | 第3位 | 勝ち抜き |
|決勝 | 第5位 | - |

## 開発環境
ArchLinux

ArchLinux Archiveを利用して、2017.05.01のスナップショットを用いて、各位の環境を統一

/etc/pacman.d/mirrorlist
```
Server=https://archive.archlinux.org/repos/2017/05/01/$repo/os/$arch
```

- Gcc 6.3.1
- Boost C++ Libraries 1.63.0
- Qt5.8.0
- OpenCV 3.2.0
- Zbar 0.1.0

## 実行方法

Kunugida ... 本体(サーバー)
Slave ... 複数台運用向け問題解答専用クライアント

## ディレクトリ構造

| パス              | 説明 |
|:------------------|:-----|
| ./AbilityCheck | 徒然なるままに適当なコードを書いてしまったサブプロジェクト |
| ./DisplayAnswer | 解答表示プログラム |
| ./Hazama | 昨年度の本体 |
| ./ImageRecognition | 画像処理プログラム |
| ./FindBug | バ↑グ↓ってなんですか？ |
| ./Polygon | ピースクラスなど |
| ./ProbMaker | 問題自動生成機 |
| ./ManPowerProb | 人力問題生成器 |
| ./Slave | 複数台運用向け問題解答専用クライアント |
| ./Solver | 昨年度のソルバ |
| ./NeoSolver | 今年度のソルバ |
| ./Test | テスト |
| ./Utilities | 最低限の汎用機能入れ |
| ./QRcode | QRコード読み取り用 |
| ./sample | サンプル問題等の収納場所 |
| ./picture | サンプル問題等の収納場場所 |
