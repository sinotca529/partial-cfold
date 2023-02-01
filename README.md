# pcfold
i32 同士の加算命令について，定数畳み込みと定数伝播をおこなう LLVM Pass です．

## ビルド方法
1. LLVM15.0.6 の入手．
    1. 自分のマシンに合った [LLVM15.0.6](https://github.com/llvm/llvm-project/releases) をダウンロードする．
    2. ダウンロードしたファイルを展開し，適当な場所に置く．
2. LLVM のライブラリの場所を設定する．
    - 方法1 : 環境変数 LLVM15 を，展開した LLVM へのパスに設定する．
    - 方法2 : CMakeLists.txt 内部の `$ENV{LLVM15}` のパスを，展開した LLVM へのパスに書き換える．
3. ビルド
    1. このリポジトリの CMakeLists.txt があるディレクトリに移動する．
    2. ビルドスクリプトの生成 : `cmake -G Ninja -B build`
    3. ビルド : `cmake --build build`

## テスト
ビルドした後， build ディレクトリ下で `ctest` を実行．
