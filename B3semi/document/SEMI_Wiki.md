# B3 ゼミ Wiki

<details>
<summary> <b>目次をクリックで展開</b> </summary>
<div>

- [講義資料の開発環境について](#anchor1)

- [環境構築について](#anchor2)

- [Markdown の記述について](#anchor3)

- [TeX の記述について](#anchor4)
</dev>
</details>

<a id="anchor1"></a>

## 講義資料の開発環境について

開発環境は以下のとおりです。

| 使用環境 |                                                    |
| -------- | -------------------------------------------------- |
| CPU      | Intel(R) Core(TM) i7-10510U CPU @ 1.80GHz 2.30 GHz |
| メモリ   | 8.00GB                                             |
| OS       | Windows 10 Pro                                     |
| 使用言語 | C++11                                              |

<a id="anchor2"></a>

## 環境構築について

想定する環境構築の流れを大雑把に説明するので、あくまで参考程度にお願いします。

本ゼミの実行環境は WSL(Windows Subsystem for Linux)を想定しています。そのため、手元に Linux 環境が存在しない方は WSL 環境の構築をお願いしています。

環境の構築方法については Microsoft 公式ページ\[[1]\]に記載がありますので、そちらを参考にしてみてください。

[1]: https://learn.microsoft.com/ja-jp/windows/wsl/setup/environment

| 使用環境 |     |
| -------- | --- |

Ubuntu 22.04.3
WSL 2

WSL のインストールが完了したら、次に VSCode のインストール\[[2]\]を行ないます。完了している方は飛ばしてください。

[2]: https://code.visualstudio.com/download

VSCode のインストールが完了したら、コマンドパレットに以下を入力します。

```
C:\Users\Username> Ubuntu
```

Ubuntu が開きますので、以下の 2 つをコードを入れて、build-essential と cmake をインストールします。先頭の`$`は入れないでください。

```
$ sudo apt install build-essential
$ sudo apt install cmake
```

インストールが完了したら、VSCode を開きます。

```
$ code
```

VSCode が開けたら、`ctl + @`でターミナルを表示し、Linux 上の好きなディレクトリに B3 ゼミ用のディレクトリを作成してください。

```
$ mkdir ディレクトリの名前(英語が望ましい、以下では`saji-lab`)
```

作成したらカレントディレクトリを作成したディレクトリにします。

```
$ cd saji-lab
```

ディレクトリ内に講義資料を clone します。

```
$ git clone URLを貼るよ~~~~
```

これでゼミ用の環境構築は終了です。

最後に動作をテストします。
B3Semi ディレクトリにて以下のコマンドを入れてください。

```
$ sh builtrun.sh
```

"Hello, World"がターミナルに出力されれば OK です。

<a id="anchor3"></a>

## Markdown の記述について

Markdown の記述で使いやすそうな qiita の記事です．リンク切れてたらすみません．

[リンク 1](https://qiita.com/toyokky/items/47a5a56c20ad99e1784c)

[リンク 2](https://qiita.com/kamorits/items/6f342da395ad57468ae3)

<a id="anchor4"></a>

## Tex での記述について

ローカルでの環境構築が面倒な場合はオンラインエディタでの作成をおすすめしています．
Overleaf や CloudLaTeX 等でアカウント作成して使用してください．

特に CloudLaTeX はローカルの VisualStudioCode にオンライン上の編集データをダウンロードして使用できます．環境構築等はググってみてください．
