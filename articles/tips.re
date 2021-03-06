= 役に立つ豆知識

技術書を作る際に詰まる部分をまとめました。
知らなければたどり着かない知識や、ちょっとした工夫など便利なテクニックを紹介します。

== 推奨するディレクトリ構成

#@# NOTE author:vvakame

TechBoosterが推奨するディレクトリ構成を述べておきます。
要点は次のとおりです。
具体的には@<list>{directory}です。

 * リポジトリのトップレベルにはファイルをあまり散らかさない
 * 複数人で執筆した時にそれぞれのファイルが混ざったり邪魔になったりしないようにする
 * 著者全員で利用するreviewのバージョンを固定する
 * ビルド手順を統一するために何らかのタスクランナーを使う（TechBoosterの場合、Node.js+grunt）

//list[directory][ディレクトリ構成]{
  ├── README.md
  ├── circle.yml            （CIサービスであるCircle CIの設定ファイル）
  ├── setup.sh              （執筆前にgemやnpmのインストールを行うスクリプト）
  ├── Gemfile               （bundler経由でRe:VIEWを利用するための設定ファイル）
  ├── Gemfile.lock          （ライブラリのバージョンをロックする）
  ├── package.json          （gruntを利用するためのnpm用設定ファイル）
  ├── npm-shrinkwrap.json   （ライブラリのバージョンをロックする）
  ├── Gruntfile.js          （gruntの動作設定ファイル）
  ├── articles
  │   ├── catalog.yml       （Re:VIEW用 章立ての設定ファイル）
  │   ├── config.yml        （Re:VIEW用 本を生成する時のメタデータ記述ファイル）
  │   ├── prh.yml           （language-review用校正設定ファイル）
  │   ├── preface.re
  │   ├── foo.re
  │   ├── bar.re
  │   ├── contributors.re
  │   ├── images
  │   │   ├── foo           （foo.re用）
  │   │   │   └── description-of-foo.png
  │   │   └── bar           （bar.re用）
  │   │       └── bar-introduction.png
  │   ├── layouts           （存在しない場合もある）
  │   │   └── 省略
  │   └── sty               （存在しない場合もある）
  │       └── 省略
  └── code                  （サンプルコード置き場）
      ├── foo               （foo.re用）
      │   └── sample.js
      └── bar               （bar.re用）
          └── flush.js
//}

原稿はarticlesディレクトリに、サンプルコードはcodeディレクトリに入れています。
articles/imagesやcodeの中は、原稿の章ごとに.reのファイル名と同名のディレクトリを用意し、その中で活動します。

原稿のファイル名は、わかりやすいのが一番！ということで複数名で執筆する場合筆者の名前にしてしまう場合があります。
vvakame.reというファイル名にしてしまえば、レビューを行ったりビルドエラーなどの問題が発生した時連絡する人がわかりやすい。
という発想です。


== 紙面レイアウトを変更する

印刷所へ入稿する原稿を制作していると、Re:VIEWが標準で用意している構成そのものを変更する必要に迫られる時があります。
Re:VIEWではPDFを出力するためにLaTeXを利用しています。そのため、レイアウトの変更にはLaTeXの知識が必要です。

具体的には.ymlや.cssを配置しているディレクトリの下に@<code>{layouts/layout.tex.erb}を置くことで
Re:VIEWが出力するLaTeXソースファイルの構成を変更できます。

Re:VIEWは@<href>{https://github.com/kmuto/review}の@<code>{review/lib/review/review.tex.erb}を、
LaTeXのソースファイルのテンプレートとして読み込みますが、@<code>{layouts/layout.tex.erb}がある場合、そちらを優先して適用します。

//footnote[review_tex_erb][2013年11月時点。最新の情報についてはGitHubのRe:VIEWリポジトリを参照してください]

カスタマイズに当たっては、@<code>{review/lib/review/review.tex.erb}を@<code>{layouts/layout.tex.erb}にコピーして変更するとよいでしょう。

//image[how_to_convert_re_to_pdf2][layout.tex.erbの取り扱い][scale=0.35]{
//}

== 余白を調節する

PDFで出力するページの余白を指定するには、.styファイルに@<code>{\geometry}を設定します（@<list>{set_margin}）。

//list[set_margin][余白を設定]{
\geometry{top=18mm,bottom=23mm,left=24mm,right=24mm}
//}

指定できる単位は、@<code>{cm}, @<code>{mm}の他にもLaTeXでサポートされている@<code>{in}, @<code>{pt}, @<code>{em/ex}, @<code>{zw/zh}, @<code>{Q}などがあります。


===[column] レイアウトを変更する楽しみ

ヘッダやフッタを変えたい、ノンブルを隠したい（隠しノンブルと呼びます）など紙面に凝ってくるとレイアウトへの要望がでてきます。紙面作りを楽しんでいる証拠ともいえるでしょう。

Re:VIEWではPDF出力を得るためにLaTeXを利用しています。そのためレイアウトに関する部分の多くはLaTeXの知識を必要とします。自由度が高い一方、独特の記法への馴染みの薄さやパッケージ、環境の依存関係などカスタマイズが困難なため、入門を目的とする本書では触れていません。

そんなあなたには、奥村晴彦氏の「LATEX2e 美文書作成入門」@<fn>{book_latex2e}がお勧めです。
深淵を覗けますよ！

===[/column]

//footnote[book_latex2e][「LATEX2e美文書作成入門」 @<href>{http://www.amazon.co.jp/dp/4774160458} - 奥村晴彦著 技術評論社刊]

=={config_task_runner} タスクランナーの設定

#@# NOTE author:vvakame

複数人で執筆する場合、何らかの統一された手順でのビルド手順が必要です。
TechBoosterではNode.js+gruntを利用しています。
この構成になっているのは、プロジェクト構成を主に行っているvvakameが一番使い慣れているから、という理由が大きいです。

新規に書き起こすのであれば、執筆者があまり導入の手間をかけなくてもよいものを選ぶのがよいでしょう。
候補としては、RubyはRe:VIEWのためにすでに入っているはずなのでRuby上で動作するrakeを使うか、色々なOS・環境で導入済みである場合の多いJavaと、gradle@<fn>{why-gradle}の組み合わせがよいかもしれません。

タスクランナーが行うべき作業は少ないです。

 * 古い生成ファイルを消す（消さないとエラーになる場合がある）
 * review-preprocコマンドを実行する
 * 各ターゲット向けのビルド用コマンドを実行する

これだけです。

ひとつひとつ、どういうコマンドと等価な処理をしていけばよいかを解説します。

 * 古い生成ファイルを消す（消さないとエラーになる場合がある）

最低限必要なのは次のコマンドと同様の動作です。

//emlist{
rm -rf articles/C89-FirstStepReVIEW-v2-pdf/ \
       articles/*.pdf  \
       articles/*.epub \
       articles/*.html \
       articles/*.xml  \
       articles/*.txt
//}

#@# prh:disable
それぞれ、pdf、epub、html、idgxml（Adobe InDesign用XML）、text生成時に作成される一時ファイルまたは最終出力ファイルです。
一番最初の行の C89-FirstStepReVIEW-v2-pdf 部分はarticles/config.ymlのbooknameの設定により変化します。
特に、最初の行は必ず行わないとPDFを生成しようとした時にエラーになるのでPDF生成処理前には必ず消すようにします。

 * review-preprocコマンドを実行する

最低限必要なのは次のコマンドの実行です。

//emlist{
# articlesディレクトリ内で実行する想定
$ review-preproc -r --tabwidth=2 *.re
//}

review-preprocコマンドはRe:VIEWの仕組みの中で、もっとも便利な、愛すべきコマンドといえます。
review-preprocコマンドは文書中に埋め込まれたpragmaを処理し、サンプルコードを文書中に展開したり指定のコマンドの実行結果を文書中に展開してくれたりします。
C言語のマクロとだいたい同じものだと思えばよいでしょう。

文書にソースコードを貼りこむ時、インデントは2スペースとします。
このため、4スペース派の人はサンプルコードではタブを使うようにして、エディタ上では1タブ＝4スペースで作業し、文書中に貼りこむ時にタブを2スペースに変換するとよいでしょう。

詳細は@<chapref>{review-introduction}に譲ります。
#@# REVIEW vvakame tabwidthについて言及されてなかった…
#@# REVIEW mstssk tabwidthについて書きました

 * 各ターゲット向けのビルド用コマンドを実行する

最低限必要なのは次のコマンドの実行です。

//emlist{
# 全て、articlesディレクトリ内で実行する想定
# textの生成
$ review-compile --all --target=text
# htmlの生成
$ review-compile --all --target=html --stylesheet=style.css --chapterlink
# latexの生成
$ review-compile --all --target=latex --footnotetext
# idgxmlの生成
$ review-compile --all --target=idgxml
# pdfの生成
$ review-pdfmaker config.yml
# epubの生成
$ review-epubmaker config.yml
//}

欲しい出力結果に応じて、コマンドを使い分けます。
pdf、epubについては利用するコマンドそのものが違うので注意します。
詳細は@<chapref>{review-introduction}を参照してください。

あとはそれぞれのターゲット向けに下準備とビルドを行うタスクを作成するだけです。

#@# prh:disable
これらを詰め込んだ、実際にTechBoosterで使っているgrunt用設定ファイルを公開しています。
@<href>{https://github.com/TechBooster/C89-FirstStepReVIEW-v2/blob/master/Gruntfile.js}
Node.js v4以上が必要ですので注意してください。

//footnote[why-gradle][gradleはgradle wrapperという仕組みがあり、gradle自体を別途導入する必要がないため]

== 関連各所の紹介
TechBoosterがRe:VIEWを使っているなかで関係したお世話になっている各所を紹介します。

 * 達人出版会：Re:VIEWを使った電子書籍を扱う出版社
 * トップスタジオ：Re:VIEWによるリフロー（自動組版）を導入している制作会社
 * 日光企画：お世話になっている印刷所

 ありがとう！そしてありがとう！！

== 品質を高めるためのチェックポイント

限られた時間のなかで可能な限り品質を確保するポイントをリスト形式で紹介します。

 * 企画は書きたいことを選ぶ（モチベーション維持）
 * 一人でかかない（共同執筆がお勧め）
 * 執筆時点は早めの締め切りを設定する（こうしておくと致命傷で済む）
 * レビューを実施する（読者の視点を作り出す）
 * 紙面を=や*で検索する（文法ミスを見つけるため）
 * 記号などの文字化け探し（TeX、フォントはUTF-8対応しているとは限らない）
 * 紙面のはみ出しチェック（TeXコンパイルのtoo lateという文字列を確認する）

実際には何度も読みなおし文章を推敲することが一番ですが、このようなステップを意識することで
ケアレスミスを見つけられます（特にツールに不慣れな場合は有効です）。

具体的には編集（校正）フェーズでは次のように変更しました。

 * 冗長性の排除：検証を行うことを → ～検証することを
 * 冗長性の排除：～することができます → ～できます
 * 受け身の排除：～表示される → ～表示する、表示できます
 * 重複表現の排除：必ず必要です → 必要です
 * 平易な表現：～Aが有効であることが原因で → Aが原因で
 * 事実の表現：～なります → できます
 * 指示語の排除：これそれあれ → 前後関係が明確で無いものは排除。これが示すとおり → 出力結果が示すとおり
 * 過剰な敬語：ご確認いただけると思います → 確認できます
 * 重ね言葉：～したりします → ～だったり、～したりします
 * 表現の訂正：思います → 考えます
 * 書き出し表現のチェック：本章、本節、本項 、本稿、本記事、ここでは等がある。適切なものを選んでいるか
 * リスト、図の参照 → 本文中に＠<list>{id}は～という処理です。と概要を説明してからリストに入ること。そのあと詳細な解説としておくことで驚かない
 * 表現の統一：「こと」でのGrep → 大抵は不要。～することがあります →～します
 * 表現の統一：「どうか」でのGrep → 大抵は不要。～正しいかどうか。→  検証する
 * 表現の統一：箇条書き、リストや図のキャプションが体言止めなのか、動詞で終わっているのか、統一する

また文章単体では、かかりつけの距離を確認し、読みやすく訂正しています。

//list[before][変更前]{
アプリはOSの起動やタイマーによって定期的に起動されるアプリのServiceを作成し、RecommendationパッケージのContentRecommendationを使用してNotificaitonを作成することができます。
//}
//list[after][変更後]{
アプリの作ったServiceは、OS起動時またはタイマーによって定期的に実行できます。Serviceの中では、ContentRecommendationクラスを使ってNotificationを作り、ユーザに通知可能です。
//}
