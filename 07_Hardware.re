= カスタムハードウェア
== 構成要素
本体、アプリ、ネットサーバ、
製造、品質管理、認証、
ケース、外装、宣伝、発送、集金、破棄、次
== マイコンとモジュール、カスタム回路
== 選択肢、開発環境、求める機能項目、性能
== カスタムな無線通信
== HomeKitとかライセンスが必要なもの、組み合わせ

== アプリとハードの連携の勘所

=== 電力消費量を最小にする

iOSアプリケーション開発は電池消費量を常に注意します。もしもアプリケーションが、量自体は小さくても、定常的にかつ長時間電力を消費すれば、積分すれば電池消費量は大きくなります。携帯されるiOSデバイスでは、ユーザがすぐそれに気づき、直前にインストールしたアプリケーションが原因だろうと、それを削除するかもしれません。

Bluetooth LEは超低消費電力の無線通信技術です。これは、接続したマスターとスレーブで電波の送受信のタイミングを同期することで、高周波回路を動かす時間を最小にしているからです。Bluetooth LEが使う2.4GHz帯の周波回路の動作電流それ自体は、クラシックBluetoothと違いはありません。その稼働時間を最小限に絞ることで、時間平均では超低消費電力になるのです。

Core Bluetoothフレームワークを使う時は、スキャンの期間を必要最小にすることだけに気をつけます。iOSアプリケーション開発者がほかに注意することは、ありません。通信による電力消費量に大きく影響するリンク層の通信パラメータの設定は、iOSが隠蔽するので、iOSアプリケーション開発者が触れることはできません。通信パラメータの設定は、ペリフェラルのファームウェア開発者の担当領域です。

2.4GHzの高周波回路では、送信する信号が自分でわかる送信回路よりも、むしろ電力値が8桁以上も違う信号を受信する受信回路のほうが、消費電流が大きくなります。スキャンは、おそらく近辺にいるだろうペリフェラルが送信しているかもしれないアドバタイジング・パケットを検出するために、受信回路を長い時間動作させます。このため、消費電力を最小にするために、スキャンの期間が必要最小になるように注意をします。

=== バックグラウンド状態でのデバイスの発見とアドバタイズメント

Bluetooth LEはコイン型電池で年単位の無線通信をし続けられるのが大きな特徴です。この特徴を活かしたアプリケーションとデバイスの協調動作を実現するために、iOSは、iOSアプリケーションがサスペンド状態のときのBluetooth LEデバイスから通知があればそれをユーザに表示する機能と、バックグラウンド動作を提供しています。バックグラウンド動作がデバイスとの連携に重要です。

iOSアプリケーションは、セントラ・ロールおよびペリフェラル・ロールになります。それぞれに、Information property list (Info.plist) fileに指定するバックグラウンド動作のモードが追加されました。バックグラウンド状態のiOSアプリケーションは、Bluetoothのスイッチのオンオフや、リモートのペリフェラルからの通知発生などの、Bluetooth LEに関係するイベント発生の都度、iOSから10秒間のアプリケーション実行時間を与えられて、アプリケーションのイベントに該当するデリゲートが呼び出されます

iOSアプリケーションのフォアグラウンド状態とバックグラウンド状態で、iOSのBluetooth LEの振る舞いが異なることに、注意が必要です。iOSは電力消費量に注意して設計されています。Bluetoooth LEは電力を消費する無線回路を使います。特にiOSアプリケーションが動作し続けるバックグラウンド状態は、少しの電力消費でも時間積分すると大きな電力消費量になります。このため、開発者が不注意で電力を消費してしまわないように、非接続状態でのBluetooth LEの無線利用に対しては、iOS自体に工夫がなされています。

Bluetooth LEで予期せぬ電力消費を生じる場面は、デバイスの発見およびアドバタイズメントをする期間です。いったんBluetooth LEデバイス間の接続が確立すれば、その無線通信による消費電力量はコイン型電池1つで年単位持つ程度のものになります。

iOSアプリケーションがセントラル・ロールならば、バックグラウンド状態では、アドバタイズメントのスキャン周期が12分程度に1回と長くなります。また、アクティブ・スキャンを行いません。このため、リモートのペリフェラルの発見周期が長くなります。またリモートのペリフェラルが、アクティブ・スキャンでのみサービスのUUIDが取れる設計の場合は、サービスを発見できません。<!--TBD  振る舞い確認 -->

Bluetooth LEの無線は、他のiOSアプリケーションと共有しています。そのため、他のアプリケーションの振る舞いが、バックグラウンド状態での動作に影響を与えることがあります。例えば、フォアグラウンドのiOSアプリケーションがスキャンを開始すると、その時に発見されたデバイスはバックグラウンド状態のiOSアプリケーションにも通知されます。

iOSアプリケーションがペリフェラル・ロールならば、バックグラウンド状態では、iOSが送信するアドバタイジング・データが変更されます。バックグラウンド状態では、ローカル・ネームが送信されません。したがって、もしもリモートのセントラルが、ローカル・ネームでデバイスを検索していると、このペリフェラルを発見できません。


=== アクセサリの実例

アプリケーションやネットワークそして事業までを統合したアプセサリは、まだ少なくこれから登場するでしょう。アクセサリ自体は、2012年の夏あたりから次々登場しています。いくつかを紹介します。

1つは、プロトタイピングのための開発キットやアクセサリです。これらは、早い時期から登場しています。まずBluetooth LEの半導体やモジュール会社が、組み込み機器開発者向けキットの提供を開始しました。iOSアプリケーション開発者のなかにも、Bluetooth LEデバイスの開発をしてみたい要望があります。次に登場してきたのが、その要望にあわせて、組み込み機器開発者でなくても使いやすい、より安価な開発キットです。

日本の技術基準適合証明を取得しているプロトタイピングに向いたものに:

* SBBLE(サブレー) [http://sbble.micutil.com](http://sbble.micutil.com)
* Konashi [http://konashi.ux-xu.com](http://konashi.ux-xu.com)
* センサータグ [http://www.tij.co.jp/tool/jp/cc2541dk-sensor](http://www.tij.co.jp/tool/jp/cc2541dk-sensor)

があります。

SBLBLE(サブレー)はマイクロチップ・テクノロジー・ジャパン株式会社のPICマイコンを採用した開発キットです。市販のUSB Bluetooth4アダプタを挿して使います。Konashiは、iOSアプリケーション側の開発がObjective-Cに加えてJavaScriptでも開発できます。ウェブ・サービスの開発者が、フィジカル・コンピューティングをiOSでおこなう場合の唯一の選択肢です。センサータグは、テキサス・インスツルメンツ社の半導体の評価キットで、25ドルと低価格です。次のセンサを搭載しています:

* 放射温度センサ、温度センサ
* 湿度センサ
* 圧力センサ
* 加速度計
* ジャイロスコープ
* 磁力計

必要なセンサが上記にあるプロトタイピングであればセンサータグを、LEDの点灯など低速の外部装置制御をiPhoneから行うならKonashiまたはSBBLEを、任意のファームウェアを開発したいときはSBBLEを選択するとよいでしょう。

キーホルダーまたはタグは、よくあるアクセサリです:

* キーホルダー、
[iPhone用探せるキーホルダー](http://buffalo.jp/products/catalog/supply/bluetooth/bluetooth/security/bshsbtpt01/)
* StickNFind [https://www.sticknfind.com](https://www.sticknfind.com)

キーホルダーは、iPhoneと接続して使うものです。iPhoneを置き忘れしそうになった時にキーホルダーから警告音を出したり、キーホルダーのボタンを押すとiPhoneから音が出る機能でiPhoneを探すのに利用します。タグは、代表的なものにStickNFindがありますが、ものに取り付けておいて、ものを探すのに使うものです。その機能は、キーホルダーのボタンを省略したもので、おおよその距離をiPhoneで確認すること、iPhoneから操作してタグから音や光を出すこと、ができます。

たいていのキーホルダーやタグは、仕様が公開されていたり、ソフトウェア開発キットが提供されていて、アプリケーション開発に利用ができます。近接検出または光や音の出力を必要とするプロトタイピングに利用できます。

この他にも:

* 心拍センサ、 [Alpha](http://www.alphaheartrate.com)
* 活動量計、[Fitbit](https://www.fitbit.com)
* 睡眠記録、[オムロン ねむり時間計](http://www.healthcare.omron.co.jp/corp/news/detail/223)
* 姿勢検出、 [Lumoback](http://www.lumoback.com)
* 腕時計、[Pebble](http://getpebble.com)
* 顔検出、[HVC-C1B](http://plus-sensing.omron.co.jp/egg-project/)

などが市販されています。

アクセサリに直接アクセスできるSDKを提供するもの、アクセサリに直接アクセスするAPIはなく、ウェブ側に蓄積されたデータにアクセスするAPIを提供するものなど、APIやSDKの公開は、製品ごとにまちまちです。




=== アクセサリの実例

アプリケーションやネットワークそして事業までを統合したアプセサリは、まだ少なくこれから登場するでしょう。アクセサリ自体は、2012年の夏あたりから次々登場しています。いくつかを紹介します。

1つは、プロトタイピングのための開発キットやアクセサリです。これらは、早い時期から登場しています。まずBluetooth LEの半導体やモジュール会社が、組み込み機器開発者向けキットの提供を開始しました。iOSアプリケーション開発者のなかにも、Bluetooth LEデバイスの開発をしてみたい要望があります。次に登場してきたのが、その要望にあわせて、組み込み機器開発者でなくても使いやすい、より安価な開発キットです。

日本の技術基準適合証明を取得しているプロトタイピングに向いたものに:

* SBBLE(サブレー) [http://sbble.micutil.com](http://sbble.micutil.com)
* Konashi [http://konashi.ux-xu.com](http://konashi.ux-xu.com)
* センサータグ [http://www.tij.co.jp/tool/jp/cc2541dk-sensor](http://www.tij.co.jp/tool/jp/cc2541dk-sensor)

があります。

SBLBLE(サブレー)はマイクロチップ・テクノロジー・ジャパン株式会社のPICマイコンを採用した開発キットです。市販のUSB Bluetooth4アダプタを挿して使います。Konashiは、iOSアプリケーション側の開発がObjective-Cに加えてJavaScriptでも開発できます。ウェブ・サービスの開発者が、フィジカル・コンピューティングをiOSでおこなう場合の唯一の選択肢です。センサータグは、テキサス・インスツルメンツ社の半導体の評価キットで、25ドルと低価格です。次のセンサを搭載しています:

* 放射温度センサ、温度センサ
* 湿度センサ
* 圧力センサ
* 加速度計
* ジャイロスコープ
* 磁力計

必要なセンサが上記にあるプロトタイピングであればセンサータグを、LEDの点灯など低速の外部装置制御をiPhoneから行うならKonashiまたはSBBLEを、任意のファームウェアを開発したいときはSBBLEを選択するとよいでしょう。

キーホルダーまたはタグは、よくあるアクセサリです:

* キーホルダー、
[iPhone用探せるキーホルダー](http://buffalo.jp/products/catalog/supply/bluetooth/bluetooth/security/bshsbtpt01/)
* StickNFind [https://www.sticknfind.com](https://www.sticknfind.com)

キーホルダーは、iPhoneと接続して使うものです。iPhoneを置き忘れしそうになった時にキーホルダーから警告音を出したり、キーホルダーのボタンを押すとiPhoneから音が出る機能でiPhoneを探すのに利用します。タグは、代表的なものにStickNFindがありますが、ものに取り付けておいて、ものを探すのに使うものです。その機能は、キーホルダーのボタンを省略したもので、おおよその距離をiPhoneで確認すること、iPhoneから操作してタグから音や光を出すこと、ができます。

たいていのキーホルダーやタグは、仕様が公開されていたり、ソフトウェア開発キットが提供されていて、アプリケーション開発に利用ができます。近接検出または光や音の出力を必要とするプロトタイピングに利用できます。

この他にも:

* 心拍センサ、 [Alpha](http://www.alphaheartrate.com)
* 活動量計、[Fitbit](https://www.fitbit.com)
* 睡眠記録、[オムロン ねむり時間計](http://www.healthcare.omron.co.jp/corp/news/detail/223)
* 姿勢検出、 [Lumoback](http://www.lumoback.com)
* 腕時計、[Pebble](http://getpebble.com)
* 顔検出、[HVC-C1B](http://plus-sensing.omron.co.jp/egg-project/)

などが市販されています。

アクセサリに直接アクセスできるSDKを提供するもの、アクセサリに直接アクセスするAPIはなく、ウェブ側に蓄積されたデータにアクセスするAPIを提供するものなど、APIやSDKの公開は、製品ごとにまちまちです。-->



=== アプセサリの種類と構成
取りうる構成は、案外決まっている。
ハードウェア要素、使える技術、ソフトウェア的なものとハードウェア的なもの。
技術書、センサ工学、読んでおけばOK。半導体の会社、世の中にある製品。いろいろ。
表示装置: 点、面。LED、液晶、電子ペーパ、有機EL。LED。面に並べた。
入力と出力
最低でもボタン1つとLED。電源ONと動作状態表示。単体で最低限。
センサー、モータ。音。

前提: スマフォとの連携、将来的にはWiFi? スマートウォッチ的なもの、中継。

立場: スマートフォンで、ネットに繋がって、警告。
接続: 繋がりっぱなし、どこで繋がるのか、接続数、必要性。反応

価格、デザイン。
口コミ、売り込み?
制約条件: 取り付け位置、習慣化されるか?、消費電力、充電の手間、運用
第1章で分類した、取り付け設置位置でデータの意味とか採れるとか。
電池制約で、消費できる電力量。実用範囲。実験なら、毎日使うでは、間欠?

要素
- トポロジー
- ペアリング、ボンディング
- 消費電力、レスポンス
- 接続のタイミング

=== ネットワークのトポロジ
半導体やSDKを選ぶ。ペリフェラルのみのもの、ペリフェラルとセントラルになれるもの(4.1以降)、同時接続数。

使い勝手的なところ。
セントラルとペリフェラル。スター型。ただし4.1から同時になれるので、中継、スキャッタネット。

所有権と、ユーザがどこを見るかというので:

一番簡単なのは、自分が持っている1台のデバイスと連携する。通知系デバイスとかは、それでいいだろう。健康系とか。いまのは、そういうのが多いんだろうね。所有者が自分一人っていうもの。

画面が2つあるもの、スマフォではきついものとか。中間に取りまとめ役がつく。:自転車のセンサー。これ統合して表示、でスマフォにも。この場合はセントラル2つに繋がるペリフェラルっていうのいいんだけど、自社製品なら、統合しちゃって、サイコン、スマフォに統合する。データ蓄積とかそういうの。こういうの炎天下とか、厳しい環境、スマフォでもいいんだけど、スマフォではきつい。温度とか。

複数でシェア、同時利用するもの。リモコンであれば、同時接続必要。共有の概念。エアコンのほうがセントラル、スマートフォンがペリフェラル。昔のだとできなかったけど、いまならできるか。あるいは、スマートフォン同士が連携してて、実は1台がつながっているだけ、ほかはネットで同期とか。WiFiにBLE入っていれば、ネット側からという、大きく迂回した構成もとれる。見た目同じなら同じだろう。
独自の仕様、後にアップデート。GATTベースなら共存可能。DFU。

範囲が広いもの:メッシュとか、パケットを中継するっていう。部屋伝いに。でもインターネット側にでるのは、1つのところだろうから、自宅ならね。だからスター型のおおきいやつ、でも物理的に1対1で電波が届かないなら、中継しましょうと。
自社で閉じる?

スマートフォンを使わない構成: IPv6系とか、そういう系統。
農業のセンサーとか、M2M的な。収容数とか、同時接続の数とか、
メッシュの構築、無線+プロトコルで総合でみた消費電力、管理のしやすさ。スマフォが入らないのなら、なんでもいい。たまたま使える。

今後は、ペリフェラルのまとめ役。サイクルコンピュータ的な。
IoT系、スマフォではなくてWiFiも含めた。
ホップ、メッシュ。規格的には。しかしWiFiを見てもわかるように。AppleID統合とか振る舞い。技術的に繋がるが、自社製品であればより簡単に。工夫。エコシステム。技術をユーザに見せない。

=== 接続のタイミング
故障、イベント。突発的。近くを行き過ぎたら反応する。
所有権限。データは見られる。解析できる? 反応すべき?
あることがわかる、一方通行。OSとハード統合されていないときつい。

iBeacon的な。ブロードキャスト、アドバタイジング。

ユーザが画面を見ていること。バックグラウンド。たまたま。

据え置き。WiFi統合。大容量、電池。電源。1秒スキャン、10秒待機。かなり持たせられる。でも単体だと、表示? 検出?専用設計

読み書き。たまに反応。消費電力、スマートフォンが受け側。接地側なら。
アプリ、反応タイミング。バックグラウンド。10分に1回程度。たまたま。
ANCSに反応するものは?

=== ペアリングとボンディング
接続するならば、
必要なのは、勝手につながったら困る。所有権。
勝手に知られるのは困る。データの所有権。操作の所有権。

セントラルをいくつまでボンディングできるかとか、管理、問題。
UIとか表示とか次第で振るまいが違う。要は、確認。りょうh

最初の接続の体験。ユーザが目で見て確認する。
キーがあるのか、パスコード入力か。機種ごとに。
アプリと完全統合するのもいいだろう。独自SMを設計するみたいな。

接続までの時間:
アドバタイジング・インターバル。発見して、接続をかける。初回は、アプリは2段階。なので、インターバルは短く。なにもなければ、長く。
ユーザの操作をした時にアドバタイジングとか、勝手に繋がるものなら、そう設計するとか。無駄な接続要求、電車の中でいたずら的なとか。

初期設定の体験:
コネクション・インターバル。最初の設定時、転送時間。リクエストで。

デバイス間の技術だから、複数の所有するデバイスで接続させたいときは、個別にボンディングさせるくらいしか、やりようがないと思うけど。独自にSMを実装してもいいか。接続は弾けないけど、接続はされないという。

アプリで、いつペアリング画面がでるのか。(実際には、ペアリングとボンディングだけど、iOSでは画面が1つになっているから。)

(ボンディングしてたら、勝手に繋がる? ANCSだと、いつの間にかつながっているっぽいか?)

必要になった時にiOSはダイアログを出す。セキュリティオプションが付いているキャラクタリスティクスにアクセスしたとか、アクセスされたとか。


==  Bluetooth low energyの半導体
モジュールを使うとしても、カスタム設計としても、ファームウェア開発から見たら同じこと。
基本構成、専用プロセッサ+BLE専用、1チップ化。
特殊機能、ANTとか
WiFiとか、デュアル/シングルモードとか。多品種、他の機種。
開発環境や経験。

=== 評価項目
(試作時と量産時の入手性、価格、モジュール、開発者。製造費用と開発期間)
(ODM/OEM?)

=== 半導体の種類
(半導体会社はどんな会社があるのか)
(モジュール会社はどんな会社があるのか)
立ち位置、特徴、付加価値。

=== 構成の全体像

処理と通信が別チップか1チップか:
BLEチップ + マイコン
SoCチップ(BLE+マイコン)
浮動小数点演算、専用回路。マイコン Cortex-M4F Cortex-M0

電池:コイン型リチウム電池、充電式。
半導体が違ってくる。充電管理および電源制御の半導体。

センサー系:

=== 開発環境
IDE, 縛られているもの。MDK-ARM、IAR。Eclipse。フロント。
コンパイラ gcc, arm-cc
RTOS、ライブラリそのもの。
mbed?

=== 他の無線通信技術との統合
マウス、既存。プロプライエタリ。製品間の通信。
WiFi統合。デュアル、シングルBT。

構成: SoCか、外部半導体か。
無線プラスアルファ: WiFi, デュアル、IoT系。ANT+。

- 性能
-- 消費電力、無線と処理
-- 処理能力
- 開発環境
-- 独自ライブラリの学習コスト
-- 今後のメンテナンス
-- ファームウェアの更新機能
- 特徴
-- ANT+, デュアル、WiFiなど

- Nordic
- TI
- Broadcom
- CSR
- ルネサス、東芝、ラピス
- Dialog

電池周り。Li-Po
充電管理、かつDC/DC変換

半導体。
SoC、各社。仕組みや構造。HCIベース、ワンチップ。
プロセッサ、ライブラリ、開発環境。
環境的なもの:
ANT+とか、既存の無線通信とか。マウス的な。
WiFiも同時であれば? そういうモジュールがいいかもしれない。
HomeKitとかならば? そういうのに対応している会社のがいい。契約などで。

モジュール
製造価格、大きさ(ピン)、SiP(アンテナ内蔵)、スクリプト。

マイコンの構成。BLE+マイコン、1チップ、か2構成。
アップデート。それぞれが認証範囲だから。チェック、確認。

=== BT4.0/4.1/4.2
物理層はあるので、その上のスタック次第。けっこう、更新で行ける感じ。先を見越して。
メッシュネットワーク
音声通話

=== 電気的な特性
ピンコンパチブル。
ベア台、WCSP。小ささ。
フォーラムの充実度。

==  アプセサリでのファームウェア開発

=== チーム構成
3の構成。ハードウェア、ファームウェア。1対1。仕事を投げると、動かないなどトラブル。対処。気分的な。常に、なにか仕事があるわけではないが中性中立の、3人目。

=== 回路設計が先か、ファームが先か
回路設計、のちファーム。
機械部品があると、やっかいかもな。テスト用ファーム? BLEとか機能じゃなくて、テスト信号入力で機能確認。マイコンすら別のもの。回路開発の人が使うジグ。

マイルストーンをお互い決めて。回路が先。でも本番までの設計、時間がかかる。
開発用のボード。信号を当たれる。

モジュール、電波法的な。機能が同等品を使ってもOK。
32kHzの推奨のあるなし、LDOかDCDCか。GPIOの本数、アンテナの利得。

判別つかないところがある。大抵はI2CかSPI。
それほど高速バスでもないから、配線だけした開発用ボード。開発時はprintfデバッグ。ファームウェアの書き込みと、デバッグメッセージの出力用ポート。(開発版でも出荷時にファーム書き込みでポートはあるか)

本番の基板となると、ちょっと時間がかかる。
機能設計ならOK。後で電池とかと統合して困ったことがあるけど。


=== はまりどころ
service changed。
ペアリング、ボンディングまわり。
ANCSのCCCDまわり。(iOS9ではどうなんだろう?)

=== トラブルの原因切り分けと対応
振る舞いで。
再現。
パケットレベル
iOSのロギング。iOS自体。ハンドラ、service changedとか。
=== スニファの準備と使い方
Windows/Mac


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

== 開発のポイントとプロトタイピング

アプリケーションのサンプル、ハードウェアの試作あれこれ。
プロトタイピング、iOSアプリケーション単体、ハードウェア。
ペリフェラルも作れるから、そっちで試作。
わざわざプロトタイピングするのは、
センサとかIOとか大きさとか電池の稼働時間とか、iPhoneではハードウェアの要件を満たせない。
たくさんばらまく必要がある(費用)

==  ハードウェア・プロトタイピング

=== ファームウェアの位置づけ

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

=== BLEのハードウェア開発

BLEのデバイス開発は、組み込み装置の開発そのものです。ARM Cortex-M0のようなマイコンに、BLEのプロトコルスタックとユーザのアプリケーションを入れて、周辺のスイッチやLED、そしてセンサーを動かすハードウェアを開発していきます。

マイコンを利用する開発の難易度は、何を作りたいか、どう作るかの構想により、大きく違います。構想をたてる時点から、組み込み会社と協業していくことを、おすすめします。マイコン用の開発環境(IDE)があり、C言語で開発していきます。

ここでは、BLEのデバイス開発について、見ていきます。試作では、むき出しの基板に組み上げた回路でもよいでしょうが、実際には、筐体や商品パッケージ、取扱説明書など付随するものも必要です。ここでは、それらは考えません。

=== フルカスタム開発

半導体チップを購入して、ゼロから設計と開発をすることを、フルカスタム開発と呼びます。フルカスタム開発の設計の流れは:

1. 半導体チップメーカからBLEの半導体を購入
2. 半導体チップメーカの推奨設計例をもとにして、基板回路などを設計
3. 電波法の認証 (試作では1台単位、製造では製造設備単位で認証が必要)

となります。製造まで考慮すると、製造時試験の手順決定と技術文書の作成など、多くの項目がありますが、ここでは省略しています。

BLEの半導体には、BLEの通信機能だけがあるものと、BLEの通信機能に加えてマイコンまで内蔵した、いわゆるSoC(システム・オン・チップ)の2種類があります。いずれを採用する場合でも、通信の制御にマイコンは必須です。

BLEの通信機能だけがあるものを使う場合は、その半導体とマイコンの間は、ホストコントロールインタフェースというBluetoothの規格に従ったプロトコルでやりとりをします。既存の製品にBLEを追加する場合で、すでに製品の中にあるマイコンで通信制御まで実現する場合には、この形を取ります。

SoCは、BLEのプロトコルスタックとユーザのアプリケーションが、BLEの半導体に内蔵された1つのマイコンで実行されます。回路面積を小さく、かつ消費電力を最小にできる利点があります。BLEデバイスをゼロから企画する場合で、大きさや電池の持ちに注目するときには、こちらを採用します。

CC2540を使ったファームウェア開発で、TI社のファーム焼きソフトを使っている場合、"データフラッシュの消去"をすると、CC2540のユーザ書き込みMACが0に初期化される。ドキュメントには、ユーザ指定のMACが0なら、メーカ書き込みのMACを使うとあるが、実際には、使っていない。ユーザのMACが0もしくは0xffいずれの場合も、その値を使ってしまう。すべてのデバイスのUUIDがおなじになり、個別識別ができない。ロット単位のエラッタなのかどうかは知らない。なので、フラッシュをクリアしてしまったときは、メーカ指定のMACをコピペで書き込んでおく。

=== モジュール

BLEの通信部分を入出力端子が外部に出ている小さな基板に収めたモジュールという部品があります。いろいろな会社からモジュールが出ています。

モジュールを採用する利点は、Bluetoothや電波法の認証を自社で取得する必要がないことです。一方で、モジュールの基板(大変小さくて、1cm角ですが)の形と大きさに製品が制約される場合があります。小型あるいは細長いようなBLEデバイスを開発するときには、モジュールの外形確認が必要になります。

モジュールは、Bluetoothの相互接続認証と電波法が求める工事認証を取得しているため、これを採用すればフルカスタム開発のファームウェア開発を除く手順が省けます。

2012年8月までは、モジュールの工事認証の条件に、モジュールが用意に着脱できること、という条件がありました。これを満たすため、モジュールには“コネクタ”がありました。しかし、2012年8月に、この条件が撤廃されたことで、直接ハンダ付けで固定する表面実装タイプの工事認証が通るようになっています。

=== BluetoothとMFiのロゴを掲載するには

Bluetooth対応のロゴ、およびMade for iPhoneのロゴを商品に掲載するには、それぞれBluetoothの相互接続認証の取得とMFiプログラムの参加が必要です。

Bluetoothは、認証費用自体は実費で5万円程度ですが、Bluetoothのメンバーに登録するための年会費が1万ドル必要です。自社で設計開発する場合は、自社でBluetoothの相互接続認証を取る必要があります。たいがいのモジュールは、Bluetoothの相互接続認証を取得しています。この場合は、そのモジュールを利用した派生製品であるとBluetooth対応製品のリストに無償で登録ができます。

Bluetoothのロゴを掲載しなくても、正体不明のRF装置として販売はできますが、iPhoneのようなBluetooth SMART READYな装置に接続することを表示するためには、Bluetoothのロゴは必要です。

iPhoneはBluetooth SMART READYなので、BLEデバイスはiPhoneに接続できます。このBLEデバイスの販売およびアプリのストア認証にMFiプログラムは必須ではありません。しかし、MFiのロゴを製品に掲載したいならば、MFiプログラムへの参加が必要です。

=== 半導体チップについて

Bluetooth4対応デバイスは、従来の3までのBluetoothとLow Enery両方と接続できるデュアルモードと、 Low Energyだけに対応するシングルモードの2つにわけられる。iPhoneとBLEで接続するデバイスは、BLEのみに対応する、シングルモードデバイスになる。
シングルモードデバイスは、無線および制御回路を1つにした集積回路として、テキサス・インスツルメンツ、CSR、およびノルディック社の3社から販売されている。
TI社はCC2540およびCC2541の2つのシングルモード集積回路を販売している。価格は2ドル。8051マイコンを内臓しており、BTLEプロトコルスタックをIAR Embedded Workbench IDEのライブラリとして提供している。GPIOおよびADCなどの豊富なIOもあり、BLE接続センサーがワンチップで実現ができる。 CC2541は、BLEに加えてTI社およびノルディック社の独自2.4GHzデータ通信方式も対応している。この独自の無線通信は、例えばマウスやキーボードで独自の2.4GHzの通信仕様を利用している製品をBLEに移行するときに、従来の独自通信技術に対応させつつ、かつBLE対応が求められる場合に使われる。チップサイズは6mm角。

CC2540/2541の開発は、IAR Embedded Workbench 8051を使う。モバイルライセンス、フルセットで 35万円ほど、機能限定版で 25万ほど。また、保守(更新)は、最初3ヶ月は無料、年間更新料として購入価格の20%がかかる。
CSR社は、ウェブサイトで概略しか情報を公開していない。TI社のCC2540同じようなマイコンを内臓したものを販売している。BLEの開発部門はサムスンの出資をうけている。このため、純粋な半導体メーカとして続くのかは、不安に感じるところがある。
ノルディック社は、nRF8001およびnRF8002を販売している。nRF8001は、TI社のものと違い、BLEの プロトコルスタックまでを提供しており、制御はACIインタフェースをとおして別のマイコンで実現する。 nRF8002はnRF8001に、キーレスエントリーのようなキーホルダーに使われる近接等のプロファイルを実 装したもので、BLEで最もよく使われるキーホルダー的な機能が実現できる。チップの価格は3ドル程度 (Mouserで274円、80円/ドルより)。チップサイズは5mm角。
またCortex-M0+を搭載したSoC、nRF51シリーズを発表している。ノルディック社が提供してきた独自規格の2.4GHz通信とBLEに対応したものが出荷されている。この開発環境はKeil MDK-ARMを使います。このライセンス料金は、30万円程度です。このSoCのファームウェア開発方法は、ARM Cortex-M0+の手順そのままです。

=== モジュール

この他に、BlueGiga社はTI社のCC2540を採用したモジュールを販売しています。このモジュールは、IAR WorkbenchのようなC言語開発環境の代わりに、BASICのようなスクリプト開発環境を独自に提供している特徴があります。また、FCCとCEを取得しており、Bluetoothの相互接続認証を取得しています。ですからBluetooth対応の製品リストに無償で登録ができるので、Bluetoothのロゴを表示して販売ができます。

=== 許認可の取得


=== プロトタイピングのプラットフォーム

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

==  はまりどころ

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

==  開発方法のおすすめ

BLEのハードウェア開発は、次章で述べるように、組み込み開発が必要になるために、どうしてもiOS単体で閉じた開発に比べて、時間がかかる。

iOS6では、たぶん、ペリフェラル側もiPhoneでプロトタイピングするのが、よいのではと思っている。BLEデバイスの開発には、ハードウェアそれ自体が特別なセンサーを利用しているか、またBLEの開発の中心は、ハードウェアなのかそれともデータ処理のアルゴリズムなのか、で区別してみる。

まずBLEが特殊なハードウェアを使うものであれば、iPhoneにそのハードウェアの機能がないので、プロトタイピングにはならない。しかしダミーデータを流す程度には使える。

BLE開発が、データ処理に価値がある場合がある。例えばフィットネス関連のBLEデバイスは、ほとんどが、加速度を使う。加速度から消費カロリーや歩数、高度の変化などを算出するのは、データ処理になる。このような、iPhoneにもあるハードウェアを利用し、その開発の工数の多くがデータ処理の場合には、iPhoneでのプロトタイピングは絶大であろう。

iPhoneでプロトタイピングしたソースコードをマイコンに移植すればよい。そのソースコードは、当然ながら、マイコンの性能に合わせて書きなおさなければならないかもしれないが。

BLEのデバイス自体は、とても簡単な回路ととても単純なデータ処理をする設計が多い。電力消費量とデバイスの大きさを考えれば、そのほとんどの処理はiPhone側に持たせたほうが、合理的で利点があるから。


==  iOSアプリケーションの実際

=== シミュレータでBluetooth LEのデバッグ

加速度、向き、モニタリング。
セントラルとペリフェラル。

大量のデータを送るには。
すれちがい通信的な使いかた。

=== トラブルの原因切り分けと対応

Appcessoryという単語が示す通り、Bluetooth LEデバイスとiOSアプリケーションそしてネットワークの先にあるサービスが連携して初めて、魅力あるBluetooth LEデバイスという製品になります。しかし、意図せぬ振る舞いが、開発中また販売後に起きるかもしれません。ここでは、iOSアプリケーションとBluetooth LEデバイスが意図しない振る舞いをすることを、トラブルと呼びます。

トラブルが発生した場合は、原因の切り分けが必要です。そのためには、トラブルを再現する方法や発生条件を明らかにすることが必要です。これらのタスクは、ユーザから情報を集めるような対人タスクと、与えられた条件から現象を論理的に分析する純粋な技術タスクに分けられます。具体的なトラブルは予測困難ですが、発生しうるトラブルの種類を列挙して、タスクの割り振り役および分野ごとの担当者などを、開発チームで事前に決めておくことが重要です。

トラブルの原因が設計によるものであれば、それは、iOSアプリケーション単体によるもの、Bluetooth LEデバイス単体によるもの、および両者の振る舞いの組み合わせによるもの、の3通りです。また、それが分析できるのはBluetooth LEデバイスの設計者か、iOSアプリケーション開発者のいずれかです。

ほとんどのトラブルは、iOSアプリケーション開発者の初歩的なミスが原因です。例えば、CBPeripheralのインスタンスをリテインし忘れたために接続が解除された、のようなケアレスミスによるものが多いです。開発過程での、ケアレスミスによるトラブルで発生するやりとりを防止するには、Bluetooth LEデバイス開発側が、iOSアプリケーションのCore Bluetoothフレームワークを使う部分まで含めて、開発を担当することが有効です。

iOSアプリケーションとBluetooth LEデバイスの振る舞いの組み合わせがトラブルの原因の場合は、両者の振る舞いをログを取りながら突き合わせるか、あるいはBluetooth LEの通信パケットをロギングして解析します。いずれの場合も、iOSとファームウェアおよび通信の知識が不可欠になるため、領域ごとの担当者が個別に担当するのではなく、同じ場所と同じ時間を共有して対処にあたるように、事前に決めておくことが重要です。

使用しているBluetooth LEデバイスが、市販品とその付属ライブラリを利用しているのか、または独自に設計しているのかで、対処が異なります。市販品の場合は、ライブラリのソースコードが公開されているならば、ソースコードを読むことが原因を見つける早道です。もしもソースコードが公開されていないならば、Bluetooth LEの通信パケットをロギングして解析するほかありません。独自に設計したデバイスを使用しているならば、ファームウェアのソースコードもつきあわせて、開発者間で振る舞いを1つ1つ確認していくのが、早道です。

==  開発環境とターゲット

4S以降、環境iOS5以降
Xcode


=== 参考情報源

https://developer.apple.com/library/ios/documentation/NetworkingInternetWeb/Conceptual/CoreBluetooth_concepts/AboutCoreBluetooth/Introduction.html

https://devforums.apple.com/community/ios/core/cbt

https://lists.apple.com/mailman/listinfo/bluetooth-dev



=== Bluetotoh LEに対応するiOSデバイスとOSのバージョン

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


==  周辺機器を使う

=== どんな周辺機器があるのか。

Keyfob、
Kickstarterでたくさん。NODE、LUMOBACK、脳波、心拍、感圧ペン。自転車関連。
ライブラリを公開している。Wahooフィットネスとか。
センサー系、IO系。単体で機能をもたせるというのより。

=== Keyfob

切断時の振る舞い、
近接の使いかた、

=== 心拍センサー

生体情報の基本。
プロファイル、サービス。
機種によって、R-R間隔とか。
使うには、ライブラリ、自分で使う。サンプルコード。

=== TI センサータグ

汎用に使える、試作に、小ささと価格。
サンプルコード、認証とか改造するのは、Wikiを参照。
微妙なのよね。

==  シミュレータでBLE

iOSのBLE開発で、シミュレータでBLEを使うには、内蔵のBTアダプタは使えない。NVRAMを設定して、本体ではBLEアダプタを掴まないようにして、シミュレータに直接かませる。
使用出来るドングルに制約があるみたい。MLを見ると、BroadcomはNG、CSRはOK。市販のBT4のUSBドングル。
Amazonで購入できるものを試してみる。

- dongle list
- 日本語

==  その他

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

=== 手順

=== NVRAM の設定。

ターミナルを開いて、NVRAMコマンドを次のように入力する。

	user$ sudo nvram bluetoothHostControllerSwitchBehavior="never"
	Password:********

keyと値は、大文字と小文字を区別する。スペルミスに注意。

システムの再起動は、これを実行した直後では、不要。
なぜこのステップが必要なのかは、
see section Understanding the OS X Bluetooth Driver Behavior

=== Bluetooth LE USBアダプタを接続する

必ず、NVRAMの設定をしたあとに、Bluetooth LE USBアダプタを接続する。

=== NVRAM設定を確認する

システム情報アプリケーションを開いて、システムBluetoothドライバがビルドインBluetooth ホストコントローラインタフェース(host controller interface HCI)とマッチしているか、確かめる。
Hardware->Bluetoothセッティングで、Vender IDが"0x5AC"であることを確認。

<!-- 図を入れる-->

もしもシステムBluetoothコントロラーがBluetoth LE USBアダプタにマッチしていたら、iOSシミュレータは外部Bluetooth コントローラをBluetooth LEサービスに使えない。
この場合は、図2のシステム情報Bluetoothパネルのように。
Bluetoothドライバが、Cambridge Silicon Radio(CSR)Bluetooth LE USB adapterにマッチしている。
この場合、アダプタを外して、ステップ1 NVRAM設定、に戻る。

=== Bluetooth をiOSシミュレータで有効にする

XcodeでiOS 5 iPhone/iPadシミュレータでiOSをランチする。
iOSシミュレータが起動したら、アプリケーションを閉じて、設定アプリを選択して、一般タブで、BluetoothをOnにする。

シミュレータのiOSアプリケーションを咲き起動。iOS Core Bluetoothアプリケーションは、Blueooth LEデバイスに接続して通信するはず、
Bluetooth LEサポートが有るiOSデバイスで実行された時のように。

注意書き:もしも知っステムにBluetooth LEアダプタが接続されていないなら、Bluetooth設定をOnにすることはできない。
Bluetooth4.0サポートがないときにシミュレータを実行したら、-[CentralManager state] は  CBCentralManagerStatePoweredOff を返す。
Bluetooth LEサポートがないiOSデバイスで実行したときは、、-[CentralManager state] は  CBCentralManagerStateUnspportedを返すだろう。

=== 実際

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

=== ドングルの動作確認

* プラネックス BT-Micro4 http://www.planex.co.jp/products/bt-micro4/ 価格2000円、実売1100円(Amazon)
 *  CSR8510
* バッファロー BSHSBD08BK http://buffalo.jp/products/catalog/supply/bluetooth/bluetooth/adapter/bshsbd08/  価格2509円 実売1300円
 * CSR8510

=== OS X Bluetooth ドライバの振る舞いを理解する

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

=== OSX Bluetooth LEアプリケーションをテストする

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

==  スニファの準備と使い方

=== 前知識

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

=== 資料

http://processors.wiki.ti.com/index.php/BLE_sniffer_guide

=== 手順

- 購入
- ファームウェアをUSBドングルに焼きこむ
- パケットスニファのアプリケーションを起動する

=== 環境

Windows xxx

=== 買い方。

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

==== フラッシュプログラマのDL

http://www.ti.com/tool/flash-programmer
v2じゃない方
”Flash Programmer for 8051-based CCxxxx SoCs and RF Evaluation Boards”とある方をDL。

注意:
Windows VistaまたはWindows7では、インストール開始直後に”User Access Control”ダイアログが表示されるかもしれない。
ユーザが管理者権限を持っているならば、"yes”ボタンを押してインストールを継続する。もしも管理者権限がないなら、管理者権限のあるユーザIDとパスワードを入力する。

=== スニファをDL

http://www.ti.com/tool/packet-sniffer
からアプリをDLする。
SmartRF Packet Sniffer runs on Windows 98, Windows 2000, Windows XP (32 bit), Windows Vista (32 and 64 bit) and Windows 7 (32 and 64 bit).

=== ファームを焼きこむ

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

=== パケットの見方

=== Tips

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


=== リソースリスト

USBドングルの写真
med_cc2540emk-usb_cc2540emk-usb_web.jpg
http://www.ti.com/tool/cc2540emk-usb より引用
49ドル。

==  Core Bluetooth フレームワークの実際の振る舞い

実際の振る舞い、通信、外部、アプリ単体で完結しない。
振る舞い。Core Bluetoothからは見えない。しかしアプリを作るときには必要な。タイミング。

=== 振る舞いの時間

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

=== 消費電力?

=== 長期安定動作?

==  使いかたの場面によるCore Bluetoothの使いかた

ブロードキャストで周囲に伝搬したい
すれちがい通信をしたい
まとまった量のデータを送信したい
