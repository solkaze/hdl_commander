このプログラムは情報科学演習Ⅲ・テーマ3及び、プロセッサ設計などその他設計において手助けを行うために作成されました。

cmakeをインストールしてる方は
```
mkdir build
cd build
```
こちらで「build」ディレクトリを作成して移行し、
```
cmake ..
```
```
make
```
とコマンドを記述することで実行ファイルを作成することができます。
cmake のインストール方法は
Ubuntu, wsl2利用時
```
sudo apt install cmake
```
にてインストールしてください
windows版は
<https://cmake.org/download/>

cmake-3.31.0-rc3.zip
から入手できます。

*cmakeなんて使いたくねえ*
という人は
```
g++ -o -O2 -Wall -Wextra src/main.c -o main
```
を利用してください

