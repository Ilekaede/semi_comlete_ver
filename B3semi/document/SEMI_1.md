# B3 プログラミングゼミ 第 1 回

今回実施する内容は以下の通りです。

- BMP ファイルの読み込み、書き出しの実装
- カラー画像とグレースケール画像の相互変換

実装の際は以下のようなことを考えてみてください。(ひょっとしたら考察のネタになるかも...)

- BMP ファイルはどのような構造になっているか (\[[1]\]や\[[2]\]のサイトを参考にどうぞ．)
- 画像サイズはどのように計算されているか
- 画素値の最低値、最大値はいくつか、またなぜその値なのか
- 画素の座標(x,y) = (0,0)は画像のどの位置にあるか

[1]: https://algorithm.joho.info/image-processing/bmp-file-data-header/
[2]: https://qiita.com/spc_ehara/items/03d179f4901faeadb184

## 取り扱う画像について

第 1 回目のゼミでは画像処理技術ではなく、データとしての画像そのものについて触れておきましょう。

### ビットマップ画像(.bmp)

本ゼミで使用する画像は bmp(ビットマップ)形式と呼ばれるものです。画像を画素(pixel)と呼ばれる格子状に分割しており、その 1 つ 1 つが画素値と呼ばれる情報を持っています。一般的に画像の拡張子として知られている.jpg や.png とは違い、**データを圧縮していない非圧縮データ**であることが最大の特徴です。その分、ファイルサイズも大きいことには注意しましょう。

### カラー画像

パターン認識の講義とは違い、今後の研究ではカラー画像を使用しての画像処理となります。

カラー画像は画素 1 ピクセルの色を R(赤)、G(緑)、B(青)の 3 原色で表現します。1 画素の RGB 要素を 8 ビットで表す 24 ビット画像が一般的です。

それぞれのチャンネルの画素値を抜き出した 3 つの画像を図 1 に示します。なぜ色がなくグレーであるのかは次の項でわかると思います。

<div>
<div style="text-align: center;">
<img src="semi_img/1_rgb_lenna.png">

図 1 左から R チャンネル、G チャンネル、B チャンネル

</div>

### グレースケール画像

RGB カラー画像に対し、白黒の濃淡を表現した画像がグレースケール画像です。グレースケール画像は 1 画素を 8 ビットで表すため、色情報を持たず、明るさ情報のみを保持しています。画素値が 0 に近いほど画素は暗くなり、255 に近いほど明るくなっていきます。図 1 がグレーだったのは，24 ビットの画像を 3 つに分解し，各チャンネルを 8 ビット画像として表わしているからです．

カラー画像をグレースケール画像に変換したのが図 2 です．

<div>
<div style="text-align: center;">
<img src="semi_img/1_gray_lenna.png" width="300"> <br>
図2 グレースケール画像
</div>

図 1 で示した画像群の平均値を取った画像が図 2 となっています．24 ビット画像を 8 ビット画像に圧縮した，という表現の方がわかりやすいでしょうか．

## 実装課題

1. bmp ファイルの入出力処理を実装し，入出力が適切に行われることを確認してください．

2. カラー画像をグレースケール画像に変換する処理を実装してください．

3. 指定された画像 2 枚をそれぞれグレースケール画像に変換してください．1 枚はグレースケールに変換できますが，もう 1 枚は変換がうまくいきません．なぜそのような違いが出るのか考察，および調査し，レポートに記述してください．

処理を実装するファイルは以下の通りです。

- `myImage.hpp`：ヘッダファイル
  - クラスの定義
  - 関数のプロトタイプ宣言
- `fileio.cpp`：BMP 画像の入出力を行うソースファイル
- `color_convert.cpp`：グレースケール化の処理を行うソースファイル
- `main.cpp`：`main`関数が記述されているソースファイル

## 実装について

基本的には、皆さんに提供したファイルのうち、`.cpp`ファイル、`.hpp`ファイル、`CMakeLists.txt`に追加・変更を行ってプログラムの実装を行なってもらいます。

プログラム中の変数や関数の定義については各ファイルにコメントの形で記述してありますので、必ず確認しながら実装を行ってください。

なお、実装においてわからないことがあれば Slack の B3 ゼミ用チャンネル`b3ゼミ2023`で質問をお願いします。

### 実装時のポイント

- 空欄を適切に埋めれば動作するようになっていますが、想定通りの動作が保証される範囲内でいくらでも改造を施して構いません。

- 作成したファイル名を CMakeLists.txt 内`add_executable()`に入れてください。

- B3Semi ディレクトリにおいてコマンド `$sh build_run.sh` で各ファイルをコンパイルから実行まで一括で行なうことができます。WSL にインストールがされていない方は以下のコマンドを WSL 上で実行してみてください

```
$sudo apt install cmake
```

- C++では、定数表現には const 修飾子を用いて値の保証を行うのが一般的です。const 修飾子を利用することで、定数に対する誤った演算処理にはコンパイルエラーが発生し、バグを未然に防ぐことができます。効率的な実装のために積極的に使用するようにしてください。

- Visual Studio Code の拡張機能である`Hex Editor`\[[3]\]は、bmp ファイルのヘッダ情報を数値で直接確認することができます。考察のために必要ならば使ってみてください。

[3]: https://marketplace.visualstudio.com/items?itemName=ms-vscode.hexeditor