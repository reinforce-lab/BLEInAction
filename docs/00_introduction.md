\maketitle
\thispagestyle{empty}

# はじめに{.unnumbered}

ようこそ、たのしい! うれしい! びー・える・いー! に。

このタイトルと表紙にもかかわらず本書を手にしたあなたは、超低消費電力無線通信技術 Bluetooth Low Energy (ブルートゥース ロー エナージー) によほど強い興味を持たれている方でしょう。

高度に発達した技術は魔法のように見えますが、その仕組みを理解すれば誰にでも使える単なる道具です。本書は、この Bluetooth Low Energy という技術をその手にするために、この技術を使うために求められる広い分野の知識と具体的な手順の解説をまとめたものです。

本書は、iPhone およびiOSを搭載するiOSデバイスを軸にして Bluetooth Low Energy を解説していきます。その他のモバイル機器は、しったことではありません。

Blueotooth Low Energy は、いままで無線通信技術が入るとは思われていなかった分野で使える技術です。

いままで無線通信技術を入れようとは思われもしなかったものに無線通信技術があったならば、前提が変わることで今までの常識がひっくりかえるのではないかといった話が広い分野で話題になっています。

2010年に規格化されたBluetooth4.0は、それまでのBluetooth3.0では満たしきれない、それらの利用分野に向けた新技術を統合しました。それがBluetooth Low Energyです。

Bluetooth Low Energy の特徴は、多種多様なものを表現する強力な表現能力と、コンセントがない場所でも電池で数年間無線通信し続けられる超低消費電力です。

小さな電池1つで数年間も無線通信しづつけられる超低消費電力が、運用や製造のコストといった課題を解決します。広く普及したスマートフォンが Bluetooth4.0 に対応してきたことで、スマートフォンのアプリケーションから誰でも容易に運用ができるようになっています。

身の回りにあるあらゆるものがネットワークにつながることで、次の未来が到来すると話題になっています。

これまで新しい技術が登場するたびに、技術的には実現しうる素晴らしい未来図が話題になりました。しかし、そのような大きな絵を現実のものとするには、社会全体が一気にその技術を採用して切り替わる必要があります。そのような大変化が起きることはなく、やがて技術の存在すら忘れ去られてきました。

おそらく次の未来は、1つ1つは小さな数多くの取り組みのなかかから生き残り、規模を拡大するものたちが担うのでしょう。

Bluetooth Low Energy は、小さな取り組みを素早く試みることができる技術です。そして、iPhoneとそのユーザを通じて急速な規模拡大も可能であるゆえに、我々にとって重要で使える技術なのです。

2015年2月1日 上原 昭宏


## 想定する読者 {.unnumbered}

本書の想定読者はエンジニアに限定しません。経営や商品企画など広い分野の読者を想定しています。

第1章および無線通信規格などの解説は、背景知識がなくともいいように書いています。

iOSアプリケーション開発は、Xcode6 と Objective-C もしくは Swift でアプリケーション開発の基礎知識を前提としています。Bluetooth Low Energy の通信規格やハードウェアについての知識は前提としません。

ハードウェアおよびファームウェアの開発は、解説を見ながら電気配線ができる程度を前提としています。例えば、Arduinoを使ったフィジカル・コンピューティングの書籍を見ながらブレッドボードで電気回路が組めるくらいを想定しています。

## 開発環境 {.unnumbered}

本書で解説する Bluetooth のバージョンは 4.2 です。

本書が対象とするiOSおよび開発環境のバージョンは次のとおりです。

- OSのバージョン: iOS7, iOS8
- 開発環境: Xcode6
- 開発言語: Objective-C および Swift
- 対象機種: iPhone4s 以降に発売されたiOSデバイス
	- iPhone4s, iPhone5, iPhone5c, iPhone5s, iPhone6, iPhone6s
	- iPad mini, iPad mini2, iPad mini3
	- iPad (第4世代), iPad Air, iPad Air2
	- iPod touch (第5世代)

## 章の構成 {.unnumbered}

本書は6章で構成されます。1章は、Bluetooth Low Energy とiOSの全体像を解説しています。チーム全員で共有すべき内容をまとめているので、まずは1章を読んでください。その後に続く章の内容は分野ごとに独立しており、すべてを最初から順番に読む必要はありません。必要な部分だけを読めばいいです。

担当分野ごとの、どの章を嫁日以下の目安は以下のとおりです。カッコ書きは、読む必要はないが参考になる章です。(TBD)

- 経営や企画を担う方 : 第1章
- アプリケーション開発者 : 第1章、第2章のGATTの解説部分、第3章、(第4章)
- ハードウェアのプロトタイピング : 第1章、第2章、(第4章)、第5章、第6章

1章は、メンバー全員が事前共有しておくべき基礎知識や視点、iOSの Bluetooth Low Energy の全体像と基礎知識を解説します。

すべての技術には向き不向きがあります。新しい技術に触れた時にありがちですが、知らないために不適切な設定でアイディアを出してしまうことがあります。なにができるか、それができるのかを判断できる基礎知識は、適切なアイディア提案に必要です。

2章は、 Bluetooth Low Energy の無線通信技術と通信規格を解説します。

CoreBluetoothフレームワーク は Bluetooth Low Energy の無線通信を使いやすく隠蔽して抽象化しているので、通信規格の知識がなくとも使えます。ですが、このフレームワークは通信規格を素直に反映して設計されているので、 通信規格を理解すれば、メソッドの呼び出し1つ1つが具体的にどのような通信をするのかが具体的にイメージできるようになります。これは、設計時やデバック時に役立つ知識です。

またハードウェア担当者には通信規格の理解は必須です。 Bluetooth Low Energy は、サービスおよびキャラクタリスティクスと呼ぶデータ構造を持ちます。ですから、Bluetooth Low Energy を用いると、そのハードウェアをどうモデル化するかという、オブジェクト指向の考え方をしなくてはなりません。

また通信規格の基礎知識は、iOSアプリケーション開発者とハードウェア担当者の接点になる技術情報でもあります。2章の冒頭の  Bluetooth Low Energy の基礎知識は共有すべきです。

3章は、iOSアプリケーションに Bluetooth Low Energy を提供する CoreBluetoothフレームワーク を解説します。まず、クラスの構造や意味を通信規格に照らし合わせながら述べた後、デバイスの発見と接続、そして通信といった具体的な使い方を、サンプル・ソースコードで1段階づつ示します。さらに、フレームワークを通じてどのような無線通信が行われるかを、スループットとレイテンシといった時間軸での振る舞いとあわせて示します。デバッグ時のちょっとしたノウハウも紹介します。

4章は、iOSが Bluetooth Low Energy を使って提供するサービスについて解説します。iOS7から登場したiBeaconとApple Notification Center Service、そしてiOS8から搭載される HealthKit と HomeKitといった、iOSが CoreBluetoothフレームワーク を使って提供するサービス群について述べます。

5章は、市販されている購入可能な Bluetooth Low Energy の電子工作キットや周辺機器と通信するサンプル・プロジェクトを紹介します。

6章は、オリジナルの  Bluetooth Low Energy 周辺機器のプロトタイピングの技術情報、そして一般販売するまでに必要な電波法やBluetoothの認証取得などを解説します。

## サンプルコードのダウンロード  {.unnumbered}

<!--TBD--> 本書のサンプルソースコードは Github [https://github.com/reinforce-lab/BluetoothLowEnergyInAction](https://github.com/reinforce-lab/BluetoothLowEnergyInAction)  にあります。ライセンスは Apache License Version 2.0 です。バグや問題を見つけた場合は、pull requestを送ってください。

## 本書の問い合わせ先  {.unnumbered}

本書への疑問や質問は、ツイッター @u_akihiro でメンションを送るか、もしくは電子メール・アドレス u-akihiro@reinforce-lab.com にメールを送ってください。

問い合わせいただいた内容の一部または全てを本書への追記または筆者が運営するブログで利用するかもしれません。掲載する文章は質問者が特定できる情報が入らぬように配慮をしますが、情報共有のために利用することをご了承ください。

また本書の内容を超える問い合わせには回答しかねます。筆者は企画や設計についてのコンサルティング・サービスを提供していますので、別途問い合わせてください。


## 謝辞 {.unnumbered}

本書の Bluetooth Low Energy についての知見は、2012年からの[(有)トリガーデバイス](http://www.triggerdevice.com/blebu/index.html) [^001] 佐藤 忠彦さんとの技術調査および技術セミナーや開発案件から得られました。また表紙カバーは(有)トリガーデバイスの池本 貴子さんに描いていただきました。

Bluetooth Low Energy の利用場面や企画での活用は、[株式会社 Matilde(マチルダ）](http://ma-tilde.com) [^002] 白鳥　啓さん、[(株)GOCCO](http://goccojapan.com) [^003] 森 誠之さん、情報科学芸術大学院大学 [IAMAS] [赤松 正行](http://akamatsu.org)さん [^004] とのディスカッションから、多くを得ました。

本書のタイトル「うれしい! たのしい! びー・える・いー!」は [@ksksue](https://twitter.com/ksksue)[^005] さんにいただきました。[@ksksue](https://twitter.com/ksksue) さんは、Internet of Things スタートアップをアクセラレートするためのソフトウェア＆ハードウェアプロジェクト [Physicaloid](http://www.physicaloid.com)[^006] を展開されています。

日本からも Bluetooth Low Energy を用いたユニークな製品を発表する会社が登場してきています。これは、Bluetooth Low Energy の半導体を販売している [Nordic Semiconductor ASA](http://www.nordicsemi.com) の 山崎 光男 さんの、開発を容易にするための様々な働きかけがあってのことです。山崎氏の活動がなければ、本書を世に出すことはありませんでした。

また [達人出版会](http://tatsu-zine.com) の 高橋 征義 さんには、2011年12月に本書の出版を相談してから2年以上も待っていただきました。

このほかにも、岐阜県大垣市で開催されたBluetooth Low EnergyセミナーやiBeaconハッカソン、またBluetooth Low Energyのプロトタイピングでのやりとりなど、数多くの方々からいただいた知見も盛り込んでいます。

[^001]: [(有)トリガーデバイス http://www.triggerdevice.com/blebu/index.html](http://www.triggerdevice.com/blebu/index.html) 
[^002]: [株式会社 Matilde(マチルダ）http://ma-tilde.com](http://ma-tilde.com)
[^003]: [(株)GOCCO http://goccojapan.com](http://goccojapan.com) 
[^004]: [http://akamatsu.org](http://akamatsu.org)
[^005]: [@ksksue https://twitter.com/ksksue](https://twitter.com/ksksue)
[^006]: [Physicaloid http://www.physicaloid.com](http://www.physicaloid.com)
[^007]: [Nordic Semiconductor ASA http://www.nordicsemi.com](http://www.nordicsemi.com)
[^008]: [達人出版会 http://tatsu-zine.com](http://tatsu-zine.com)

## 免責と商標について  {.unnumbered}

- 本書に登場する会社名、製品名、サービス名は、各社の登録商標または商標です。
- 本文中では、<!-- ®、™、© --> &reg;、&trade;、および&copy;マークは明記していません。
- 本書の内容に基づき実施または運用したことで発生したいかなる損害も著者は一切の責任を負いません。
- 本書の内容は2015年2月時点のものです。本書で紹介した製品およびサービスの名称や内容は執筆時点から変更される可能性があります。

<!-- <div style="text-align: center;"> この手に、魔法を </div> -->