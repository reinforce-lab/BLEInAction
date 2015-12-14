
通信サンプル
開発環境的なところを解説。
発見と接続
ホワイトリスト
認証、ボンディングとペアリング
マスタースレーブ/サーバとクライアント
データベースの構築
サービス変更あたり、GAP
通信速度、MTUの変更とか

libUSB btstack
設定、ドングル、USBさして

# 開発のポイントとプロトタイピング

アプリケーションのサンプル、ハードウェアの試作あれこれ。
プロトタイピング、iOSアプリケーション単体、ハードウェア。
ペリフェラルも作れるから、そっちで試作。
わざわざプロトタイピングするのは、
センサとかIOとか大きさとか電池の稼働時間とか、iPhoneではハードウェアの要件を満たせない。
たくさんばらまく必要がある(費用)

## ハードウェア・プロトタイピング

### ファームウェアの位置づけ

ならば周辺機器側にファームウェア
単体で動かせす、通信？応用例ごとに違うもの。ユーザのアプリケーション。
センサーとかIO拡張系、入力のみ、出力が一般的なもの。
本当はサービスで、IOの先につながるセンサーとか機能を抽象化する、そうするとファームウェア。

オリジナルのファームが必要な場面:

応答時間が問題(100ミリ秒とかの)
サービスとキャラクタリスティクスを定義したい
非接続時に単体で機能させたい
切断時の振る舞いとかを定義したい

要は、ユーザのアプリケーションが度のタイミングで動作するか、とういこと。

通信でつながっている時は、機能をどこに持たせるか。レイテンシ、どうしょうもない。
サービスとキャラクタリスティクス、定義はファームウェア。汎用IOじゃなくて、機能を定義したもの。
例えばI2Cの先に温度センサーがあるなら、温度のサービスを。
汎用に使えるし。

非接続のとき、ファームウェア、単体。
切れているところの振る舞い。プロトタイピングならば、"切れたつもり"で模倣すればOK。でも、けっこう致命的かも。例えば、ラジコンに使っていたら、モータの出力設定そのままになるかもしれない。おとすなら落とすで、いいんだけど。結構、こういう切れた時の振る舞いは、デモンストレーションではもないにならないかもだけど。

###BLEのハードウェア開発

BLEのデバイス開発は、組み込み装置の開発そのものです。ARM Cortex-M0のようなマイコンに、BLEのプロトコルスタックとユーザのアプリケーションを入れて、周辺のスイッチやLED、そしてセンサーを動かすハードウェアを開発していきます。

マイコンを利用する開発の難易度は、何を作りたいか、どう作るかの構想により、大きく違います。構想をたてる時点から、組み込み会社と協業していくことを、おすすめします。マイコン用の開発環境(IDE)があり、C言語で開発していきます。

ここでは、BLEのデバイス開発について、見ていきます。試作では、むき出しの基板に組み上げた回路でもよいでしょうが、実際には、筐体や商品パッケージ、取扱説明書など付随するものも必要です。ここでは、それらは考えません。

###フルカスタム開発

半導体チップを購入して、ゼロから設計と開発をすることを、フルカスタム開発と呼びます。フルカスタム開発の設計の流れは:

1. 半導体チップメーカからBLEの半導体を購入
2. 半導体チップメーカの推奨設計例をもとにして、基板回路などを設計
3. 電波法の認証 (試作では1台単位、製造では製造設備単位で認証が必要)

となります。製造まで考慮すると、製造時試験の手順決定と技術文書の作成など、多くの項目がありますが、ここでは省略しています。

BLEの半導体には、BLEの通信機能だけがあるものと、BLEの通信機能に加えてマイコンまで内蔵した、いわゆるSoC(システム・オン・チップ)の2種類があります。いずれを採用する場合でも、通信の制御にマイコンは必須です。

BLEの通信機能だけがあるものを使う場合は、その半導体とマイコンの間は、ホストコントロールインタフェースというBluetoothの規格に従ったプロトコルでやりとりをします。既存の製品にBLEを追加する場合で、すでに製品の中にあるマイコンで通信制御まで実現する場合には、この形を取ります。

SoCは、BLEのプロトコルスタックとユーザのアプリケーションが、BLEの半導体に内蔵された1つのマイコンで実行されます。回路面積を小さく、かつ消費電力を最小にできる利点があります。BLEデバイスをゼロから企画する場合で、大きさや電池の持ちに注目するときには、こちらを採用します。

CC2540を使ったファームウェア開発で、TI社のファーム焼きソフトを使っている場合、"データフラッシュの消去"をすると、CC2540のユーザ書き込みMACが0に初期化される。ドキュメントには、ユーザ指定のMACが0なら、メーカ書き込みのMACを使うとあるが、実際には、使っていない。ユーザのMACが0もしくは0xffいずれの場合も、その値を使ってしまう。すべてのデバイスのUUIDがおなじになり、個別識別ができない。ロット単位のエラッタなのかどうかは知らない。なので、フラッシュをクリアしてしまったときは、メーカ指定のMACをコピペで書き込んでおく。

###モジュール

BLEの通信部分を入出力端子が外部に出ている小さな基板に収めたモジュールという部品があります。いろいろな会社からモジュールが出ています。

モジュールを採用する利点は、Bluetoothや電波法の認証を自社で取得する必要がないことです。一方で、モジュールの基板(大変小さくて、1cm角ですが)の形と大きさに製品が制約される場合があります。小型あるいは細長いようなBLEデバイスを開発するときには、モジュールの外形確認が必要になります。

モジュールは、Bluetoothの相互接続認証と電波法が求める工事認証を取得しているため、これを採用すればフルカスタム開発のファームウェア開発を除く手順が省けます。

2012年8月までは、モジュールの工事認証の条件に、モジュールが用意に着脱できること、という条件がありました。これを満たすため、モジュールには“コネクタ”がありました。しかし、2012年8月に、この条件が撤廃されたことで、直接ハンダ付けで固定する表面実装タイプの工事認証が通るようになっています。

###BluetoothとMFiのロゴを掲載するには

Bluetooth対応のロゴ、およびMade for iPhoneのロゴを商品に掲載するには、それぞれBluetoothの相互接続認証の取得とMFiプログラムの参加が必要です。

Bluetoothは、認証費用自体は実費で5万円程度ですが、Bluetoothのメンバーに登録するための年会費が1万ドル必要です。自社で設計開発する場合は、自社でBluetoothの相互接続認証を取る必要があります。たいがいのモジュールは、Bluetoothの相互接続認証を取得しています。この場合は、そのモジュールを利用した派生製品であるとBluetooth対応製品のリストに無償で登録ができます。

Bluetoothのロゴを掲載しなくても、正体不明のRF装置として販売はできますが、iPhoneのようなBluetooth SMART READYな装置に接続することを表示するためには、Bluetoothのロゴは必要です。

iPhoneはBluetooth SMART READYなので、BLEデバイスはiPhoneに接続できます。このBLEデバイスの販売およびアプリのストア認証にMFiプログラムは必須ではありません。しかし、MFiのロゴを製品に掲載したいならば、MFiプログラムへの参加が必要です。

###半導体チップについて

Bluetooth4対応デバイスは、従来の3までのBluetoothとLow Enery両方と接続できるデュアルモードと、 Low Energyだけに対応するシングルモードの2つにわけられる。iPhoneとBLEで接続するデバイスは、BLEのみに対応する、シングルモードデバイスになる。
シングルモードデバイスは、無線および制御回路を1つにした集積回路として、テキサス・インスツルメンツ、CSR、およびノルディック社の3社から販売されている。
TI社はCC2540およびCC2541の2つのシングルモード集積回路を販売している。価格は2ドル。8051マイコンを内臓しており、BTLEプロトコルスタックをIAR Embedded Workbench IDEのライブラリとして提供している。GPIOおよびADCなどの豊富なIOもあり、BLE接続センサーがワンチップで実現ができる。 CC2541は、BLEに加えてTI社およびノルディック社の独自2.4GHzデータ通信方式も対応している。この独自の無線通信は、例えばマウスやキーボードで独自の2.4GHzの通信仕様を利用している製品をBLEに移行するときに、従来の独自通信技術に対応させつつ、かつBLE対応が求められる場合に使われる。チップサイズは6mm角。

CC2540/2541の開発は、IAR Embedded Workbench 8051を使う。モバイルライセンス、フルセットで 35万円ほど、機能限定版で 25万ほど。また、保守(更新)は、最初3ヶ月は無料、年間更新料として購入価格の20%がかかる。
CSR社は、ウェブサイトで概略しか情報を公開していない。TI社のCC2540同じようなマイコンを内臓したものを販売している。BLEの開発部門はサムスンの出資をうけている。このため、純粋な半導体メーカとして続くのかは、不安に感じるところがある。
ノルディック社は、nRF8001およびnRF8002を販売している。nRF8001は、TI社のものと違い、BLEの プロトコルスタックまでを提供しており、制御はACIインタフェースをとおして別のマイコンで実現する。 nRF8002はnRF8001に、キーレスエントリーのようなキーホルダーに使われる近接等のプロファイルを実 装したもので、BLEで最もよく使われるキーホルダー的な機能が実現できる。チップの価格は3ドル程度 (Mouserで274円、80円/ドルより)。チップサイズは5mm角。
またCortex-M0+を搭載したSoC、nRF51シリーズを発表している。ノルディック社が提供してきた独自規格の2.4GHz通信とBLEに対応したものが出荷されている。この開発環境はKeil MDK-ARMを使います。このライセンス料金は、30万円程度です。このSoCのファームウェア開発方法は、ARM Cortex-M0+の手順そのままです。

### モジュール

この他に、BlueGiga社はTI社のCC2540を採用したモジュールを販売しています。このモジュールは、IAR WorkbenchのようなC言語開発環境の代わりに、BASICのようなスクリプト開発環境を独自に提供している特徴があります。また、FCCとCEを取得しており、Bluetoothの相互接続認証を取得しています。ですからBluetooth対応の製品リストに無償で登録ができるので、Bluetoothのロゴを表示して販売ができます。

### 許認可の取得

Bluetooth4 Low Energy機器を販売するためには、販売国での認証取得が必須です。この認証は、電波を 放出する機器が他の機器の動作を阻害しないことを承認することで、無線局免許がなくても電波を発する無 線装置を運用してよいという制度が求めるものです。この認証なく電波を発した場合は、日本では使用者が 電波法違反を問われます。
Bluetoothのロゴを掲載するには、Bluetoothの団体に加入して、機器相互接続試験をクリアしなければな りません。ロゴを掲載せずBluetooth機器だといわないならば、これを取得していなくても、販売は可能で す。プロファイルによりますが、150~250万円程度がかかります。
この認証は、米国ではFCC、日本ではTELEC、欧州ではCE、と呼ばれます。FCCおよびCEは、一般的な 電気機器にもとめられるEMC(電磁両立性、電気機器などが備える、電磁的な不干渉性および耐性)も必要になります。

EMC試験および無線機器の認証取得代理を提供している会社がいくつかあります。試験および書類申請までを一貫してサービスとして提供しています。回路や電波の技術と法律の両面の知識が必要で専門職でなければ対応は難しいので、設計段階から、相談をしておくことが大切です。

電波を出す製品を米国、日本、欧米で販売するための許認可は、それぞれFCC、工事認証、CEになります。設計したものが基準を満たせず再試験になった場合には、その追加費用など、付随する費用発生がありますが、最低限必要な費用なの目安は:

- FCC 165万円くらい
	- EMC(9kHzを超える) 40~50万円
	- 無線に関わるもの(Part15 sub C、電磁障害なきこと) 80万円 
	- 米国への書類提出など 25万円
- 日本 48万円くらい(書類作成、申請代行費用は含まず)
	- 工事認証(工場生産単位での認証) 48万円
- CE 合計220万円くらい
	- EMC 100万円
	- 無線関連(障害なきこと) 80~90万円 
	- 電気機器の安全性確認 30万円
		- 認証に提出するデータは、量産と同じもので計測されたものを提出します。

製造設備および体制に対して、 求められるものは:

- FCC
	- 特に無い
- 日本
	- 製造されたものの特性が同一であること、そのために品質が管理されていることを書類で明らかにする。
	- 具体的にはISOを取得している、出荷時検査の装置や測定手順が決められていてデータが保存されているなどの、体制の整備が審査対象になる。
- CE 
	- 特に無い
	- しかし、各国で抜き取り調査をしている。もしも違反している場合は、製品の全数回収の義務が生じる。また罰金が課せられる。

###プロトタイピングのプラットフォーム

KonashiとかサブレーとかRFduino。
nRF8001を使った、BLE112を使ったArduinoシールド。BLEのなんとかBee。Seeedstudioで購入可能なもの。
タイプとして
USBドングル
モジュール
半導体(HCI)

ファームウェアの問題、書き換え。何かをさせるトリガー的なもの。
PICの環境、Arduino側マイコン。
IOに特化
個別開発環境は、やっぱりきっついので、使い慣れたもの、フィジカルコンピューティングで定評がある環境。

##はまりどころ

通常のコネクションする使い方ならば、ハマるところはない。
強いて言えば:

- CBPeripheralは自分でretainしないといけない
	- scanForPeripheralsWithServicesで取得したCBPeripheralはアプリ側でretainしないといけない
- デバイスから強制定期にBluetoothの接続切断をすると、iOS6では、CoreBluetoothが例外を飛ばしてくる
	- try~catchして処理

このほか:

- iPhoneが接続したことがないBLEデバイスのUUIDは、アドバタイズメント・パケット受信時はnull
	- iPhoneが、任意のアプリで一度でも接続したことがあれば、UUIDが取得できる。iPhoneの電源On/Offをしてもクリアされない。どっかにキャッシュをもっているのだろう。
- iOS6では、iPhoneからBLE接続を切断しても、iPhoneは30秒〜1分程、BLE接続をもっている。このためBLEデバイスからアドバタイズメント・パケットが送出されない
	- アドバタイズメント・パケットを利用する場合は、デバイス側から強制的にBLE接続を切断する。

このように、細かい所で、iOS5とiOS6で振る舞いが違うところ、タイミングのパラメータ値が違うような些細だけど、使い方によっては致命的になる、ところがある。しらないと、はまるので、事前の確認をしっかりすること。

##開発方法のおすすめ

BLEのハードウェア開発は、次章で述べるように、組み込み開発が必要になるために、どうしてもiOS単体で閉じた開発に比べて、時間がかかる。

iOS6では、たぶん、ペリフェラル側もiPhoneでプロトタイピングするのが、よいのではと思っている。BLEデバイスの開発には、ハードウェアそれ自体が特別なセンサーを利用しているか、またBLEの開発の中心は、ハードウェアなのかそれともデータ処理のアルゴリズムなのか、で区別してみる。

まずBLEが特殊なハードウェアを使うものであれば、iPhoneにそのハードウェアの機能がないので、プロトタイピングにはならない。しかしダミーデータを流す程度には使える。

BLE開発が、データ処理に価値がある場合がある。例えばフィットネス関連のBLEデバイスは、ほとんどが、加速度を使う。加速度から消費カロリーや歩数、高度の変化などを算出するのは、データ処理になる。このような、iPhoneにもあるハードウェアを利用し、その開発の工数の多くがデータ処理の場合には、iPhoneでのプロトタイピングは絶大であろう。

iPhoneでプロトタイピングしたソースコードをマイコンに移植すればよい。そのソースコードは、当然ながら、マイコンの性能に合わせて書きなおさなければならないかもしれないが。

BLEのデバイス自体は、とても簡単な回路ととても単純なデータ処理をする設計が多い。電力消費量とデバイスの大きさを考えれば、そのほとんどの処理はiPhone側に持たせたほうが、合理的で利点があるから。


## iOSアプリケーションの実際

### シミュレータでBluetooth LEのデバッグ

加速度、向き、モニタリング。
セントラルとペリフェラル。

大量のデータを送るには。
すれちがい通信的な使いかた。

### トラブルの原因切り分けと対応

Appcessoryという単語が示す通り、Bluetooth LEデバイスとiOSアプリケーションそしてネットワークの先にあるサービスが連携して初めて、魅力あるBluetooth LEデバイスという製品になります。しかし、意図せぬ振る舞いが、開発中また販売後に起きるかもしれません。ここでは、iOSアプリケーションとBluetooth LEデバイスが意図しない振る舞いをすることを、トラブルと呼びます。

トラブルが発生した場合は、原因の切り分けが必要です。そのためには、トラブルを再現する方法や発生条件を明らかにすることが必要です。これらのタスクは、ユーザから情報を集めるような対人タスクと、与えられた条件から現象を論理的に分析する純粋な技術タスクに分けられます。具体的なトラブルは予測困難ですが、発生しうるトラブルの種類を列挙して、タスクの割り振り役および分野ごとの担当者などを、開発チームで事前に決めておくことが重要です。

トラブルの原因が設計によるものであれば、それは、iOSアプリケーション単体によるもの、Bluetooth LEデバイス単体によるもの、および両者の振る舞いの組み合わせによるもの、の3通りです。また、それが分析できるのはBluetooth LEデバイスの設計者か、iOSアプリケーション開発者のいずれかです。

ほとんどのトラブルは、iOSアプリケーション開発者の初歩的なミスが原因です。例えば、CBPeripheralのインスタンスをリテインし忘れたために接続が解除された、のようなケアレスミスによるものが多いです。開発過程での、ケアレスミスによるトラブルで発生するやりとりを防止するには、Bluetooth LEデバイス開発側が、iOSアプリケーションのCore Bluetoothフレームワークを使う部分まで含めて、開発を担当することが有効です。

iOSアプリケーションとBluetooth LEデバイスの振る舞いの組み合わせがトラブルの原因の場合は、両者の振る舞いをログを取りながら突き合わせるか、あるいはBluetooth LEの通信パケットをロギングして解析します。いずれの場合も、iOSとファームウェアおよび通信の知識が不可欠になるため、領域ごとの担当者が個別に担当するのではなく、同じ場所と同じ時間を共有して対処にあたるように、事前に決めておくことが重要です。

使用しているBluetooth LEデバイスが、市販品とその付属ライブラリを利用しているのか、または独自に設計しているのかで、対処が異なります。市販品の場合は、ライブラリのソースコードが公開されているならば、ソースコードを読むことが原因を見つける早道です。もしもソースコードが公開されていないならば、Bluetooth LEの通信パケットをロギングして解析するほかありません。独自に設計したデバイスを使用しているならば、ファームウェアのソースコードもつきあわせて、開発者間で振る舞いを1つ1つ確認していくのが、早道です。

## 開発環境とターゲット

4S以降、環境iOS5以降
Xcode


### 参考情報源

https://developer.apple.com/library/ios/documentation/NetworkingInternetWeb/Conceptual/CoreBluetooth_concepts/AboutCoreBluetooth/Introduction.html

https://devforums.apple.com/community/ios/core/cbt

https://lists.apple.com/mailman/listinfo/bluetooth-dev



### Bluetotoh LEに対応するiOSデバイスとOSのバージョン

Bluetooth LEに対応するには、ハードウェアとOSがそれぞれ対応しなければなりません。

Bluetooth LEはBluetooth4の規格の一部です。スマートフォンのハードウェアの仕様に、Bluetooth4対応と書かれていれば、それはBluetooth LEにも対応しています。iOSデバイスは、2011年10月に発売されたiPhone4S以降の全てのデバイスがBluetooth4に対応しています。2013年8月時点で、Bluetooth4に対応するiOSデバイスは、iPhone4S、iPhone5, 第5世代iPod touch、第3および第4世代iPad、そしてiPad miniです。

Bluetooth LEに対応するOSのバージョンは、iOS5およびiOS6です。Core Bluetoothフレームワークは、iOS5以降のSDKに含まれています。初めてBluetooth LEをサポートしたiOS5はセントラルの機能を提供しています。次のiOS6では、セントラルに加えてペリフェラルの機能にも対応しました。セントラルおよびペリフェラルについては次の節で述べます。

iOS5およびiOS6は、iPhone3GSおよびiPhone4などの、ハードウェアがBluetooth4に対応していないiOSデバイスにも対応しています。Core Bluetoothフレームワークは、アプリケーションが実行されたデバイスがBluetooth LEに対応しているかいなかを、デリゲートを通してアプリケーションに知らせます。

Bluetooth LEを使うアプリケーションの振る舞いが、機種により異ならないかは、大きな関心事です。もしも振る舞いが異なるならば、どのiOSデバイスでどのように異なるのか、またその理由を理解して、テスト項目に入れなければなりません。筆者の知る範囲では、iOSデバイスの機種間で振る舞いが異なることはありません。

デバイスごとの差異が生じるうる要素には、アンテナ設計、無線通信の半導体およびその内部で動作するファームウェア、そしてiOS側の通信制御プログラムがあります。

アンテナは機種ごとに異なります。アンテナ自体の利得および指向性が異なるかもしれません。Bluetooth LEで通信をするだけであれば、利得の違いは通信可能距離の大小として見えます。検出される電波強度の絶対値が異なりますから、電波強度を用いる応用例では、その違いが振る舞いを変えるかもしれません。例えば、電波強度による近接検出ならば、近接と判断する距離が機種により異なってくるでしょう。

無線通信の半導体は、Bluetooth規格に従い実装されるので、半導体による機能の違いはないはずです。また[iFixit](http://www.ifixit.com)が報告しているiPhoneの内部構成をみると、無線通信の半導体は、iPhone4SとiPad3はBroadcom社のBCM4330、iPhone5とiPad miniとiPod touch 5th Genは、同じくBroadcom社のBCM4334を採用しています。BCM4330からBCM4334の変更点は、半導体の製造プロセスが65nmから40nm LPに変更され、受信動作時のピーク電流が68mAから36mAに半減したことです。ですから、ただし、受信動作による電池消費量は、機種によって2倍違うでしょうが、ハードウェアの機能の違いはないでしょう。

iOS側の通信制御プログラムは、iOSのメジャー番号が変わると異なることがあります。またマイナーバージョンで、バグの修正や動作をより安定にする変更が入ります。動作保証対象となるiOSのバージョンごとに、動作の確認が必要です。

<!-- iPhoneそれぞれが使っているモデムの半導体の番号を確認する -->
<!-- 機種ごとの振る舞いの違いがあると、いやんなので、半導体型番が同じ=タイミングの振る舞いも同じだろうと、裏付けるための情報 -->

<!-- iPhone4 http://www.ifixit.com/Teardown/iPhone+4+Teardown/3130/3 Broadcom BCM4329FKUBG -->
<!-- iPhone4S http://www.ifixit.com/Teardown/iPhone+4S+Teardown/6610/2 ムラタのモジュール Murata SW SS1830010. 他のサイト http://eetimes.jp/ee/articles/1110/17/news109_5.html ではレイアウトの刻印から  BCM4330 -->

<!-- iPad3 4G http://www.ifixit.com/Teardown/iPad+3+4G+Teardown/8277/2 Broadcom BCM4330 802.11a/b/g/n MAC/baseband/radio with integrated Bluetooth 4.0+HS and FM transceiver-->
<!-- Single-band 2.4 GHz 802.11 b/g/n or dual-band 2.4 GHz and 5Ghz 802.11 a/b/g/n
FM receiver and transmitter
Bluetooth Core Specification Version 4.0 + HS compliant with provisions for supporting future specifications
Full WAPI software and hardware support
An integrated ARM® Cortex™-M3 processor and on-chip memory
The OneDriver™ software architecture for easy migration from existing embedded WLAN and Bluetooth devices as well as future devices
SmartAudio® technology that dramatically improves voice quality in Bluetooth headsets -->

<!-- iPod touch 5th Gen http://www.ifixit.com/Teardown/iPod+Touch+5th+Generation+Teardown/10803/3 Murata 339S0171 Wi-Fi module -->

<!-- iPad mini http://www.ifixit.com/Teardown/iPad+Mini+Wi-Fi+Teardown/11423/1  Murata 339S0171 Wi-Fi module.  BCM4334 http://www.ifixit.com/Teardown/Apple+A6+Teardown/10528/2#s38332 -->

<!-- The Murata Wi-Fi SoC module actually comprises a Broadcom BCM4334 package in addition to an oscillator, capacitors, resistors, etc. You can see all the components in the X-ray (third image).
Murata assembles all of the components together and sends their package to Foxconn, where it eventually ends up on the iPhone's logic board. Chipworks said it best: "Murata makes a house that is full of other people's furniture."
Here are the die images for the Broadcom BCM4334, fabricated in Taiwan at TSMC on a 40 nm CMOS process. Its key features:
Wi-Fi (802.11 a/b/g/n)
Bluetooth 4.0 + HS
FM Receiver -->

<!-- iPhone5 http://www.ifixit.com/Teardown/iPhone+5+Teardown/10525/2#s38299 Murata 339S0171 Wi-Fi module , 上記のiPad  miniと同じもの-->

<!-- http://www.anandtech.com/show/5601/broadcom-announces-new-combo-chips-bcm4334-bcm43241-shows-80211ac-once-more -->
<!--
First is BCM4334 which is the follow-up part to BCM4330 that we've seen in a bunch of devices. BCM4334 changes from a 65nm process to 40nm LP, which itself offers a power profile reduction. The change isn't a simple die shrink either, Broadcom says it has worked on and refined the existing BCM4330 design and reduced power a further 40-50% and dramatically reduced standby power by 3 orders of magnitude. I asked Broadcom to give me a realistic estimate of power consumption - BCM4330 in full Rx mode consumes around 68mA, BCM4334 consumes 36mA at the same voltage, just to give an example of the reduction. Air interfaces don't change between BCM4330 and BCM4334. The second part, BCM43241, is a 2x2MIMO combo chip that's geared at tablets and also is built on a 40nm process. -->


## 周辺機器を使う

### どんな周辺機器があるのか。

Keyfob、
Kickstarterでたくさん。NODE、LUMOBACK、脳波、心拍、感圧ペン。自転車関連。
ライブラリを公開している。Wahooフィットネスとか。
センサー系、IO系。単体で機能をもたせるというのより。

###Keyfob

切断時の振る舞い、
近接の使いかた、

### 心拍センサー

生体情報の基本。
プロファイル、サービス。
機種によって、R-R間隔とか。
使うには、ライブラリ、自分で使う。サンプルコード。

###TI センサータグ

汎用に使える、試作に、小ささと価格。
サンプルコード、認証とか改造するのは、Wikiを参照。
微妙なのよね。

## シミュレータでBLE

iOSのBLE開発で、シミュレータでBLEを使うには、内蔵のBTアダプタは使えない。NVRAMを設定して、本体ではBLEアダプタを掴まないようにして、シミュレータに直接かませる。
使用出来るドングルに制約があるみたい。MLを見ると、BroadcomはNG、CSRはOK。市販のBT4のUSBドングル。
Amazonで購入できるものを試してみる。

- dongle list
- 日本語

http://developer.apple.com/library/ios/#technotes/tn2295/_index.html
Technical Note TN2295
Testing Core Bluetooth Applications in the iOS Simulator

iOS 5.0 provides the Core Bluetooth framework for creating iOS applications, which can detect, connect, and communicate with Bluetooth 4.0 Low Energy (LE) devices. The standard method for testing Core Bluetooth applications is on a device such as the iPhone 4S, which has Bluetooth LE support. In order to facilitate the development of Core Bluetooth iOS applications when one does not have a Bluetooth LE iOS device, the iOS 5 SDK simulator can be used to test these applications with the help of a third-party Bluetooth LE USB adapter. This Technical Note describes the process to enable and verify simulator support on an OS X system.

iOS5.0はiosアプリケーションを作るのにCore Bluetoothフレームワークを提供しています。
BLTEデバイスの発見、接続と通信をする。
Core Bluetoothアプリケーションの標準のテスト方法はiPhone4SのようなBluetooth LEをサポートする実際のデバイスで実行すること。
Bluetooth LE iOS デバイスを持っていない開発者が開発するために、iOS5 SDKシミュレータは、3rdパーティのBluetooth LE USBアダプタの助けでアプrケーションをテストするのに使うことができる。
このテクニカルノートは、OS X システムで、有効にして確認する。

注意書きとして、iOSシミュレータが実機と異なる振る舞いをしたときは、バグレポートを送れと。
注意書きとして、シミュレータだけで動作確認をおわらさせず、必ず実機で確認しろと。

必要なもの
- Mac system with Mac OS X 10.7.3 or greater
- Xcode 4.2.1 with iOS 5 SDK or greater
- Bluetooth LE USB adapter
ビルトインでBTLEサポート(BT4搭載)していても、必ずUSBアダプタが必要なのが注意点。

### 手順

### NVRAM の設定。

ターミナルを開いて、NVRAMコマンドを次のように入力する。

	user$ sudo nvram bluetoothHostControllerSwitchBehavior="never"
	Password:********

keyと値は、大文字と小文字を区別する。スペルミスに注意。

システムの再起動は、これを実行した直後では、不要。
なぜこのステップが必要なのかは、
see section Understanding the OS X Bluetooth Driver Behavior

### Bluetooth LE USBアダプタを接続する

必ず、NVRAMの設定をしたあとに、Bluetooth LE USBアダプタを接続する。

### NVRAM設定を確認する

システム情報アプリケーションを開いて、システムBluetoothドライバがビルドインBluetooth ホストコントローラインタフェース(host controller interface HCI)とマッチしているか、確かめる。
Hardware->Bluetoothセッティングで、Vender IDが"0x5AC"であることを確認。

<!-- 図を入れる-->

もしもシステムBluetoothコントロラーがBluetoth LE USBアダプタにマッチしていたら、iOSシミュレータは外部Bluetooth コントローラをBluetooth LEサービスに使えない。
この場合は、図2のシステム情報Bluetoothパネルのように。
Bluetoothドライバが、Cambridge Silicon Radio(CSR)Bluetooth LE USB adapterにマッチしている。
この場合、アダプタを外して、ステップ1 NVRAM設定、に戻る。

### Bluetooth をiOSシミュレータで有効にする

XcodeでiOS 5 iPhone/iPadシミュレータでiOSをランチする。
iOSシミュレータが起動したら、アプリケーションを閉じて、設定アプリを選択して、一般タブで、BluetoothをOnにする。

シミュレータのiOSアプリケーションを咲き起動。iOS Core Bluetoothアプリケーションは、Blueooth LEデバイスに接続して通信するはず、
Bluetooth LEサポートが有るiOSデバイスで実行された時のように。

注意書き:もしも知っステムにBluetooth LEアダプタが接続されていないなら、Bluetooth設定をOnにすることはできない。
Bluetooth4.0サポートがないときにシミュレータを実行したら、-[CentralManager state] は  CBCentralManagerStatePoweredOff を返す。
Bluetooth LEサポートがないiOSデバイスで実行したときは、、-[CentralManager state] は  CBCentralManagerStateUnspportedを返すだろう。

### 実際

- OS X 10.8.4
- Xcode 
デフォルト設定は、

	nvram -p | grep bluetoothHostControllerSwitchBehavior

何も設定されていない。(空文字列。-d で削除できる。) 刺しても、外部BTに写らないみたい。

設定しないと動かなかった。ハードウェアで見ても、切り替わりがよくわからないのか？
iOSシミュレータをリセットすると、UUIDがNULLになっていた。キャッシュのクリア状態が再現できるんだな。

本体のBT On/Offは関係するか？
わからない。ただ、iOSシミュレータをリセットしたら、Keyfobが繋がらなくなった。アドバタイズはとれているけど。これはなぜ？
実機でアプリを動かすとすぐに接続しているから、この振る舞いはシミュレータ依存であろう。

iOSシミュレータを動かしている時にUSBドングルを抜くと:

	2013-06-26 14:11:54.547 KeyFobSample[12153:1b0b] CoreBluetooth[ERROR] XPC connection interrupted, resetting
	2013-06-26 14:12:03.512 KeyFobSample[12153:c07] CoreBluetooth[WARNING] <CBConcreteCentralManager: 0x8c49c40> is not powered on

ステートがOffに変更される。iOSシミュレータを起動したまま、USBドングルを挿すと。BluetoothがOff状態からOn表示になった。
アプリのほうは、USBの抜き差しで、アドバタイズの受信動作が止まっているみたい。アプリは再起動すれば動く。

### ドングルの動作確認

* プラネックス BT-Micro4 http://www.planex.co.jp/products/bt-micro4/ 価格2000円、実売1100円(Amazon)
 *  CSR8510
* バッファロー BSHSBD08BK http://buffalo.jp/products/catalog/supply/bluetooth/bluetooth/adapter/bshsbd08/  価格2509円 実売1300円
 * CSR8510

### OS X Bluetooth ドライバの振る舞いを理解する

デフォルトのOS X Bluetoothドライバは、外付けのBluetooth HCIがアタッチされたら、
ビルドインのBluetoothインタフェースからドライバが離れて、外部HCIにアタッチする。
もしもHCIがAppleデバイスじゃなければ。
この振る舞いは、
Bluetooth LEアプリケーションを開発する
OS Xアプリケーション開発者にとっては利点。古いMacを持っていて、ビルトインがBluetooth LEをサポートしていないなら。
開発者は、Bluetooth LE USBアダプタを、その新しいHCIにシステムBluetoothドライバがアタッチして、
OS X Core Bluetoothアプリケーションが実行して、そのアダプタを通してBluetoooth LEサービスがアクセスできると。
一方で、ビルトインドライバを通して提供されていた既存のBluetooth接続は失われる(HIDデバイスとか。キーボードやマウスなど)

ところが、iOS Core Bluetooth開発者には、この振る舞いはiOSシミュレータとコンパチブルじゃないの。
iOSデバイスと全く同じようにBluetoothの振る舞いをシミュレータがしようとすれば、Bluetooth LE HCIを直接開かないといけない。
もしも内蔵のドライバが外部Bluetooth LE HCIに自動的にアタッチされたら、シミュレータは外部HCIで接続を開くことはできなくなる。
ドライバのマッチングの振る舞いを制御するのに、ビルトインBluetoothドライバはbluetoothHostControllerSwitchBehavior NVRAMセッティングを認識する。
もしも"never"なら、外部Bluetooth LEアダプタが接続されたとき、システムドライバは外部HCIをサポートするようにスイッチしたりは、しなくなる。
OS XビルトインBluetoothドライバの振る舞い。

* bluetoothHostControllerSwitchBehavior="never"
** あたらしいHCIが接続されたら、ビルトインドライバは、ビルトインHCIにアタッチされたまま。
* bluetoothHostControllerSwitchBehavior="always"
** 新しいHCIが接続されたら、ビルトインドライバはビルドインHCIから切断して外部のHCIに接続される。
* bluetoothHostControllerSwitchBehavior="default"
** 新しいHCIが接続されたら、ビルドインドライバはビルドインHICから切断されて、外部のHCIに接続される。もしも新しいモジュールがAppleモジュールじゃなければ。

### OSX Bluetooth LEアプリケーションをテストする

iOSとOS X、両方でBluetooth LEアプリケーションをテストするなら、
bluetoothHostControllerSwitchBehavior setting を復元しないといけないかも。

2つのケースがあり:
ビルトインBluetooth LEのシステムでiOS / OS X Blueotooth LE アプリケーションをテスト
ビルトインなら、
NVRAMの bluetoothHostControllerSwitchBehavior="never" setting のままにしておく。
iOSシミュレータは外付けHCIを、OS Xアプリケーションは内蔵HCIを使う。

ビルトインがない場合:
OS Xのアプリケーションをテストするなら、bluetoothHostControllerSwitchBehavior setting to the "default"にする。
iOSのアプリをテストするときは、"never"に切り替える。

## スニファの準備と使い方

### 前知識

スニファ？
BTLEでパケットをダンプする。振る舞いの切り分け、通信状態を見る。
アプリから見た振る舞い、アプリ自体、iOSのBluetooth LEの振る舞い、通信自体の振る舞い、接続先のファームウェア、その装置の捜査状況。
切り分け。
通信部分。専用機器、(500ドルから1000ドル程度かな？)。ちょっと見るには、高価。

安価。TI社。

注意:
無線。アドバタイズメントが3チャネルあるから、たまたまモニタしているチャネルで通信が始まれば、追跡できる。
専用のもの。高価。TI開発キットのものを流用。
CC2540MINIDK_Dongle

### 資料

http://processors.wiki.ti.com/index.php/BLE_sniffer_guide

### 手順

- 購入
- ファームウェアをUSBドングルに焼きこむ
- パケットスニファのアプリケーションを起動する

### 環境

Windows xxx

### 買い方。

http://www.ti.com/tool/CC2540DK-MINI
CC2540DK-MINI
99ドル

http://www.ti.com/tool/cc2541dk-mini
CC2541DK-MINI
でもOK。

http://www.ti.com/tool/cc2540emk-usb
CC2540EMK-USB 

違いは、KeyfobがCC2540かCC2541か、という違い。
CC2541は、CC2540のUSBをなく、RFの消費電流を削減したもの。
USB接続が不要な場合は、こっちのほうが適している。

BlueGigaのモジュールはUSBのも、ROMが128kBで、スニファには流用出来ませぬ。
ドングルはデフォルトでスニファのファームウェアが書き込まれている。
The CC2540 dongle is delivered pre-programmed with dedicated packet sniffer firmware. 

<!-- デフォでファームが書き込まれている。これをやるのは、評価用とかで使っちゃったとか、バージョンが上がって、ファーム更新のときのみ -->

### フラッシュプログラマのDL

http://www.ti.com/tool/flash-programmer
v2じゃない方
”Flash Programmer for 8051-based CCxxxx SoCs and RF Evaluation Boards”とある方をDL。

注意:
Windows VistaまたはWindows7では、インストール開始直後に”User Access Control”ダイアログが表示されるかもしれない。
ユーザが管理者権限を持っているならば、"yes”ボタンを押してインストールを継続する。もしも管理者権限がないなら、管理者権限のあるユーザIDとパスワードを入力する。

### スニファをDL

http://www.ti.com/tool/packet-sniffer
からアプリをDLする。
SmartRF Packet Sniffer runs on Windows 98, Windows 2000, Windows XP (32 bit), Windows Vista (32 and 64 bit) and Windows 7 (32 and 64 bit).

### ファームを焼きこむ

プログラマの起動
デスクトップアイコン、スタートメニューのFlash Programmer
C:\Program Files (x86)\Texas Instruments\SmartRF Tools\Flash Programmer\bin\SmartRFProg

最初に、CC DebuggerをUSBに接続。このDebuggerのファームウェアが古いと、警告画面が表示される。
ファームウェアの更新。上のプルダウンメニュー、”Program CCっっxSoC or MSP430”になっているのを、選択して、”Program Evaluation Board”を選択して、右にある”Update EB Firmware”をクリック。
ファームウェア更新完了。

上のプルダウンメニュー、”Program CCっっxSoC or MSP430”に戻す。
USBドングルを接続。電源さえあればいいので、電池とか他のUSBポートでもOK。接続して、CC Debuggerの本体にあるリセットボタンを押して、リセット。

	\program files\Texas Instruments\SmartRF Tools\Packet Sniffer\bin\general\firmware\sniffer_fw_cc2540_usb.hex
	
ファームウェアを選択して、"Erase, grogram and veriry"を選択、”Perform actions"を押せば、書き込み開始。プログレスバーがフルになればOK。

USBにドングルをさして、デスクトップアイコンのSmartRF Packet Snifferを起動。

### パケットの見方

### Tips

パケットスニファには、スニフしたパケットを、UDPパケット	で送信する機能がある。
PDFマニュアルの
SWRU187G、
2.4 Packet broadcast.
14ページ。

外部アプリケーション、テストの自動化に使えるかもね。

暗号化している場合は、Long Term Keyをテキストファイルにして置くことで、解析可能らしい。
暗号化は、パケットのタイミング、送受信パケットの順番でやっているから、1つでもパケットの受信をミスすると、デコードできなくなる。

タイムアウト。普通はパケットをスニフしているからタイムアウトがわかるけれども、もしもパケットを受信ミスしてて、タイムアウトしたら、スニファはタイムアウトとわからず、そのまま受信し続ける。
もしも本当に通信がタイムアウトしているなら、手動で止めればいい。


### リソースリスト

USBドングルの写真
med_cc2540emk-usb_cc2540emk-usb_web.jpg
http://www.ti.com/tool/cc2540emk-usb より引用
49ドル。

## Core Bluetooth フレームワークの実際の振る舞い

実際の振る舞い、通信、外部、アプリ単体で完結しない。
振る舞い。Core Bluetoothからは見えない。しかしアプリを作るときには必要な。タイミング。

### 振る舞いの時間

デバイスを見つける
デバイスを見つけられる
	アドバタイズメントのデータ内容	
識別する
	サービス、ローカルネーム
非接続で状態を知る
	キャラクタリスティクス、サービス、メーカ独自のアドバタイズメント・データ	
接続する
	この一連ので時間はどんだけかかる
普通の読み書きする
長いデータを読み書きする
通知を受ける
暗号化とか権限つき
切断する

### 消費電力?

### 長期安定動作?

## 使いかたの場面によるCore Bluetoothの使いかた

ブロードキャストで周囲に伝搬したい
すれちがい通信をしたい
まとまった量のデータを送信したい
