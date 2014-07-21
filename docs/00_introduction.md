\maketitle
\thispagestyle{empty}

# はじめに{.unnumbered}

小さな電池で年単位の無線通信が可能な超低消費電力無線通信技術 Bluetooth Low Energy(ブルートゥース ロー エナージー)が、スマートフォンと周辺機器とのラスト1メートルをつなぎ、日常とネットワーク世界とを連携させる鍵となる技術として、注目されています。

Bluetooth Low Energy は、従来のBluetoothとは異なる技術です。iOSでは、Bluetooth Low Energy は単なる周辺機器の無線接続技術ではなく、iOSの根幹を支える技術の1つとなっています。iBeaconのように、ハードウェアの設計製造に関わり合いがないアプリケーション開発でも、 Bluetooth Low Energy は重要な技術になります。

本書は、経営や企画者、iOSアプリケーション、電気回路やマイクロプロセッサのファームウェア開発者に向けたものです。Bluetooth Low Energy があるiOSの世界の全体像と、それを作るためのソフトウェアとハードウェアの、領域を網羅した基礎知識に基づいた実践かつ実用的な技術情報を述べます。

Apple社の戦略は、iOSデバイスと連携するハードウェアの試作と製造販売を劇的に容易にしました。新しい無線通信技術が普及するためには、その技術に対応した端末が必要です。Apple社は2011年に発売されたiPhone4sから、すべてのiOSデバイスを Bluetooth Low Energy 対応としました。さらに、iPhone4sと同時に発表されたiOS5から、Bluetooth Low Energy を一般のアプリケーション開発者に開放しました。 それで、2011年から Bluetooth Low Energy をもちいたユニークな製品やベンチャー企業が続々と誕生しています。

2013年に発表されたiOS7から、Bluetooth Low Energy は、単なる周辺機器との無線通信技術から、iOSの根幹を支える技術の1つになってきています。

2013年のiOS7では、新しい近接と位置の検出技術 iBeacon と、ウエアラブル機器にiOSの通知情報を同期する Apple Notification Center Service が発表されました。2014年のiOS8では、身の回りのあらゆる機器をiOSから制御できる HomeKit、そして生体情報の集約と利用を提供する HealthKit が発表されました。これらはすべて Bluetooth Low Energy が鍵となります。

iOS7までの7年間で、アプリケーション市場は成熟しました。iOS8からは、我々の日常世界とネットワークとを互いに結びつけることで、これまでになかった新しい利用場面とより大きな市場にiOSが広がります。いま日常世界とネットワークとの間にある深い溝に橋をかけるキー・テクノロジーが、Bluetooth Low Energy なのです。

Bluetooth Low Energy をもちいた事業の成功には、事業計画と企画、そしてハードウェア、無線通信技術、iOSアプリケーションとネットワーク・サービスなどの開発が、密に連携することが不可欠です。

iOS以外のAndroidやWindowsPhoneなどモバイル・プラットフォームは、iOSのBluetooth Low Energy の戦略を追従してくるでしょう。ですから本書は、今最もBluetooth Low Energyを最も有効に活用しているiOSについて述べます。

本書は、iOSの Bluetooth Low Energy がある世界への案内書として、また、これから誕生する新しいチームの常にそばにある書として、知るべき全てを述べます。

2014年8月12日 上原 昭宏

## 開発環境 {.unnumbered}

本書の開発対象および環境は次のとおりです。iOS8は2014年8月時点ではベータ版なので、実際のソースコードまでは示さず、WWDC2014で発表された内容をまとめます。

- OS: iOS6、iOS7、iOS8(WWDC2014にある内容について)
- 開発環境: Xcode 5.1.1
- 機種: iPhone4s 以降に発売されたiOSデバイス
	- iPhone4s, iPhone5, iPhone5c, iPhone5s
	- iPad mini, iPad (第4世代)
	- iPod touch (第5世代)

## 想定する読者 {.unnumbered}

想定する読者は、経営および経営企画に携わる方々、そして実際の開発を担当するエンジニアの方々を想定しています。

経営および商品企画の方々はiOSデバイスの使い方がわかることを前提とします。自社製品に Bluetooth Low Energy を取り込みたいときに知っておくべき、技術でできること/できないことや、向き不向き、また使いどころを述べます。

iOSアプリケーション開発者は、Xcode5 で Objective-C を使い iOSアプリケーション開発ができることを前提とします。Bluetooth Low Energy の通信規格やハードウェアについての知識は前提としません。

Bluetooth Low Energy の通信につかう CoreBluetoothフレームワークを、なぜそうなっているのかという基礎の部分から通信プロトコルに基づき解説します。そして、ハードウェアと連携するアプリケーション開発のノウハウを述べます。

ハードウェアおよびマイコンで実行されるファームウェアの開発者は、ハードウェア設計が本職ではない方を前提とします。例えば、Arduinoを使ったフィジカル・コンピューティングの書籍を見ながらブレッドボードで電気回路が組める程度を想定します。

Bluetooth Low Energy をもちいる開発は、アイディアが技術的に実現可能かを確認しながら、イメージをつかみ具体化していく繰り返し作業からはじまります。短期間になんども試作を繰り返すためにも、自分たちで実現可能性をちゃんと理解するためにも、自分たちでモックアップと簡単な動作をする回路を組み合わせた試作をすることが大切です。

## 章の構成 {.unnumbered}

本書は6章で構成されます。1章は、Bluetooth Low Energy とiOSの全体像を解説しています。チーム全員で共有すべき内容をまとめているので、まずは1章を読んでください。その後に続く章の内容は分野ごとに独立しており、すべてを最初から順番に読む必要はありません。必要な部分だけを読めばいいです。

担当分野ごとの、どの章を嫁日以下の目安は以下のとおりです。カッコ書きは、読む必要はないが参考になる章です。(TBD)

- 企画 : 第1章
- アプリケーション開発者 : 第1章、第2章の最初、第3章、(第4章)
- ハードウェアのプロトタイピング : 第1章、第2章、(第4章)、第5章、第6章

1章は、経営企画の会議などで事前に共有すべき視点や基礎知識、iOSの Bluetooth Low Energy の全体像と基礎知識を、技術情報に基づき解説します。

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

<TBD> 本書のサンプルソースコードは Github [https://github.com/reinforce-lab/wafuBeacon](https://github.com/reinforce-lab/wafuBeacon)  にあります。ライセンスは Apache License Version 2.0 です。バグや問題を見つけた場合は、pull requestを送ってください。


## 本書の問い合わせ先  {.unnumbered}

本書への疑問や質問は、ツイッター @u_akihiro でメンションを送るか、もしくは電子メール・アドレス u-akihiro@reinforce-lab.com にメールを送ってください。

問い合わせいただいた内容の一部または全てを本書への追記または筆者が運営するブログで利用するかもしれません。掲載する文章は質問者が特定できる情報が入らぬように配慮をしますが、情報共有のために利用することをご了承ください。

また本書の内容を超える問い合わせには回答しかねます。筆者は企画や設計についてのコンサルティング・サービスを提供していますので、別途問い合わせてください。


## 謝辞 {.unnumbered}

本書の Bluetooth Low Energy についての知見は、2012年からの[(有)トリガーデバイス](http://www.triggerdevice.com/blebu/index.html) [^001] 佐藤 忠彦さんとの技術調査および技術セミナーや開発案件から得られました。また表紙カバーは(有)トリガーデバイスの池本 貴子さんに描いていただきました。

Bluetooth Low Energy の利用場面や企画での活用は、[株式会社 Matilde(マチルダ）](http://ma-tilde.com) [^002] 白鳥　啓さん、[(株)GOCCO](http://goccojapan.com) [^003] 森 誠之さん、情報科学芸術大学院大学 [IAMAS] [赤松 正行](http://akamatsu.org)さん [^004] とのディスカッションから、多くを得ました。

本書のタイトル「うれしい! たのしい! びー・える・いー!」は [@ksksue](https://twitter.com/ksksue)[^005] さんにいただきました。[@ksksue](https://twitter.com/ksksue) さんは、Internet of Things スタートアップをアクセラレートするためのソフトウェア＆ハードウェアプロジェクト [Physicaloid](http://www.physicaloid.com)[^006] を展開されています。

日本からも Bluetooth Low Energy を用いたユニークな製品を発表する会社が登場してきています。これは、Bluetooth Low Energy の半導体を販売している [Nordic Semiconductor ASA](http://www.nordicsemi.com) の 山崎 光男 さんの、開発を容易にするための様々な働きかけがあってのことです。氏の活動がなければ、本書を世に出すことはありませんでした。

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
- 本書の内容は2014年8月時点のものです。本書で紹介した製品およびサービスの名称や内容は執筆時点から変更される可能性があります。

===
<div style="text-align: center;"> このてに、まほうを </div>
===
