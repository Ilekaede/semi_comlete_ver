# B3 プログラミングゼミ 第 4 回

今回実施する内容は以下の通りです。

- 色空間、表色系
- HSL 変換

実装の際は以下のようなことを考えてみてください。(ひょっとしたら考察のネタになるかも...)

- 色相角の定義域はどこからどこまでであるか
- 画像処理研究ではどのように利用されているか

## 色空間と表色系

<span style="color: red">R</span><span style="color: green">G</span><span style="color: blue">B</span>(赤、緑、青)の 3 色は、光の 3 つの基本的な成分を表すカラーモデルです。RGB モデルは、コンピュータグラフィックスやディスプレイ技術に非常に適していますが、それぞれの色が直線的に組み合わさるため、人間の視覚に基づいた色再現には制約があります。人間の視覚は、RGB の三成分だけで完全に説明できないほど複雑です。

人間の視覚システムは、RGB では表現しきれない色差や色の感知特性を持っています。特に、人間の視覚は L 錐体(赤錐体)、M 錐体(緑錐体)、S 錐体(青錐体)の 3 つの錐体細胞からの出力信号を受け取っており、これらの錐体細胞の感受性は一般的な RGB モデルとは異なると考えられています。したがって、特定の色を正確に再現するためには、RGB モデルだけでは不十分なのです。

色を定量的に表す体系には**表色系**と**色空間**があります。いくつか代表例を示します。

- **CIE-RGB 表色系** - Red、Green、Blue を使って加法混色で色を作成。ディスプレイ等の光を利用する装置で利用
- **マンセル表色系** - Hue(色相)、Saturation(彩度)、Lightness(輝度)を使って人間の知覚に似た方法で色を作成
- **L* a* b\*色空間** - 人間の感覚に近い均質な色空間で色を作成、画像から特定の色を抽出するときに利用

今回のゼミでは、表色系の中でもマンセル表色系(以下、HSL モデル)について扱います。

HSL モデルでは色を次の 3 要素で構成し、表現します。

- **色相(Hue)** - 色の違いを示す属性
- **彩度(Saturation)** - 色の鮮やかさを示す属性
- **明度(Lightness)** - 明るさを示す属性

<div style="text-align: center;">
<img src="semi_img/4_hue.png">
　　図1 マンセル表色系
</div>

### 色相(Hue)

色相は、特定の色が「赤」「青」「緑」「黄色」など、色の種類や種類の区別を表す性質です。色相は、色の見た目における基本的な特性であり、色の本質的な性格を示します。

色相は通常、色のスペクトル上の位置で表現され、円環状になります。これは、色相が連続的で循環的であることを示しています。一般的な色相環では、0°(または 360°)が赤色を示し、60° が黄色、120° が緑、180° がシアン(青緑)、240° が青、300° がマゼンタ(赤紫)など、色相の基本的なカテゴリーが配置されています。

色相は式(1)で求めることができます。

$ H = \left\{\begin{array}{ll}
undefined & (if MAX = MIN) \\ \\
60^\circ \times \frac{G-B}{MAX-MIN}+0^\circ & (if \, MAX = R \; and \;G \geqq B) \\ \\
60^\circ \times \frac{G-B}{MAX-MIN}+360^\circ & (if \, MAX = R \: and \: G < B) \\ \\
60^\circ \times \frac{B-R}{MAX-MIN}+120^\circ & (if \, MAX = G) \\ \\
60^\circ \times \frac{R-G}{MAX-MIN}+240^\circ & (if \, MAX = B)
\end{array} \right. \ \ \ - (1)$

### 彩度(Saturation)

彩度(Saturation)は、色の鮮やかさ、鮮度、色の純度、色の濃さを表す指標です。彩度が高い色は、より鮮やかで鮮度の高い色として知られ、彩度が低い色はより灰色や薄く、鮮やかさに欠ける色とされます。

彩度は、特定の色が白または灰色からどれだけ遠いか、または色がその純度を維持しているかを示します。高い彩度の色は、その色が "純粋" であると見なされ、色相が支配的であると感じられます。一方、低い彩度の色は、色相に比べて白または灰色が支配的で、より淡い、より灰色がかった外観を持つことがあります。

彩度は以下のような式(2)で求めることができます。
$
S = \left\{ \begin{array}{ll}
0 & (if L = 0 \; or \; MAX=MIN) \\ \\
\frac{MAX-MIN}{MAX+MIN} = \frac{MAX-MIN}{2L} & (if \, 0 < L \leqq \frac{1}{2}) \\ \\
\frac{MAX-MIN}{2-(MAX+MIN)} = \frac{MAX-MIN}{2-2L} & (if \, L > \frac{1}{2}) 
\end{array} \right. \ \ \ -  (2)$

### 明度(Lightness)

明度(Lightness)は、色の明るさまたは輝度を示す指標です。明度は、色がどれだけ明るいか、またはどれだけ暗いかを評価するために使用されます。色の明度は、色が白色または黒色からどれだけ遠いかを示します。

明度は以下のような式(3)で求めることができます。

$L = \frac{MAX-MIN}{2} \ \ \ - (3)$

これらのパラメータを個別に調整することで、RGB モデルだけでは操作が難しいような処理を簡単に行うことが可能となります。例えば、画像をより明るくしたい場合について、RGB モデルだけの操作では明るさだけでなく濃度も同時にパラメータとして操作されてしまいますが、RGB モデルを HSL モデルに変換、HSL モデルのパラメータ調整、HSL モデルを再び RGB モデルに戻すことで、明るさだけを任意に調節することが可能です。

## 実装課題

1. RGB 画像から HSL 画像に変換する処理を実装してください．

2. HSL 画像から RGB 画像への逆変換を実装してください．また，RGB 画像を HSL 色空間で色相回転させ，逆変換してください．回転度数は任意とします．

3. RGB 画像，HSL 画像，L* a* b\*画像の 3 つについて，各色空間を利用するメリットを考察し，記述してください．

処理結果画像，およびプログラムをレポートに貼り付けて提出してください．

処理を実装するファイルは以下の通りです。

- `myImage.hpp`：ヘッダファイル
  - クラスの定義
  - 関数のプロトタイプ宣言
- `hls.cpp`：RGB と HSL の相互変換を行なうソースファイル
- `main.cpp`：`main`関数が記述されているソースファイル

自分で新しくファイルを作成する、処理を追加する等は問題ありません。想定された処理が実装できる範囲内で、より良い処理を実装してみてください。

### 実装のポイント

- 一般に、RGB 画像はチャンネルの並びが B、G、R の順、HSL 画像はチャンネルの並びが H、L、S の順に並んでいるので注意してください。

- 色相に関しての実装がやや難しいと思います。参考書『ディジタル画像処理[改訂第 2 版]』p.79 ~ p.81 も参考にしてみてください。

- クラスのメンバ関数にはゲッタ(`getPixel()`)とセッタ(`setPixel()`)が実装されています。これらを利用すると index を書かずに画素値を扱うことができます。

- `color_convert.cpp`内にある`split_Channel()`で、チャンネルごとに値を分離することができます。

- デバッグのために OpenCV での答え合わせをすることは問題ありません。ですが、OpenCV で作成した画像を提出するのはゼミの趣旨から逸脱しているので止めましょう。