= Bluetooth low energyのiOSアプリケーション開発

この章は、iOSアプリケーション開発にBluetooth LEを提供するCore Bluetoothフレームワークを解説します。

Core Bluetoothフレームワークは、iOSアプリケーションにBluetoot LEデバイスの発見、接続と読み書きの操作を提供します。このフレームワークが、2章で述べたBluetooth LEの通信規格や振る舞いなどの技術詳細を隠蔽して抽象化するので、アプリケーション開発者はアプリケーション開発だけに注力できます。

iOS5とiOS6で、Core Bluetothフレームワークが対応する機能が大きく異なります。iOS5は、Bluetooth LEのセントラルという役割を提供します。iOS6は、セントラルに加えてペリフェラルという役割も提供します。iOSのバージョンごとの対応を、iOS5およびiOS6のアイコンを節のタイトルに表示して明示します。

<!-- 全体概要 -->
<!--
iphoneのなかでは、複数のアプリ、BLEのハードウェア、デーモン。
デーモンとのやり取り、CoreBluetoothとは。

なので、他のアプリの動作。


キャッシュ周り。
はまりやすいところ。
-->

== アプリケーション開発のBluetooth LE基礎知識

キーホルダや心拍センサなどの一般に販売されているBluetooth LEデバイスをアプリケーションから使う場合、たいていデバイスに接続して操作するためのライブラリが提供されているので、Core Bluetoothフレームワークを意識することはないかもしれません。また、Core BluetoothフレームワークがBluetooth LEを抽象化していますから、Bluetooth LEの基礎知識がなくてもアプリケーションを開発できます。

しかし、Bluetooth LEを使うオリジナルのハードウェアを企画設計する場合は、Bluetooth LEの基礎知識が不可欠です。iOSとBlutooth LEでなにができるかは企画に必要な知識ですし、試作品のハードウェアが意図した振る舞いをしない時に、その原因を明らかにするには知識が必要です。

2章でBluetooth LEそれ自体の技術詳細を述べました。ここでは、iOSアプリケーション開発者からみたBluetooth LEの基礎技術情報をまとめます。

=== クラシックBluetoothとBluetooth LE

Bluetooth4.0は、Bluetooth3.0までの近接高速通信技術にBluetooth low energyの超低消費電力通信技術を統合したものです。Bluetooth3.0までの技術を、Bluetooth LEと区別するために、ここではクラシックBluetoothと呼びます。

クラシックBluetoothがヘッドフォンやファイル交換などに使う、連続したより高速のデータ通信を目指しているのに対して、Bluetooth low energyは、少量のデータを低頻度でやりとりする用途に、ハードウェアの低コスト化と、コイン型電池で1〜2年間の連続動作ができる超低消費電力に特化しています。

=== クラシックBluetoothのプロファイル

iPhoneは、Human Interface Device Profile (HID)などのクラシックBluetoothのプロファイルをサポートします。BluetoothキーボードをiPhoneに接続しても、アプリケーションは、それがBluetooth接続なのか、あるいは他の接続方式なのかを気にせずに、キーボードとして扱うことができます。クラシックBluetoothでは、周辺機器との接続および制御は全てiOSがおこない、アプリケーションから操作する手段はありません。もしも設計したい周辺機器が、定義されたプロファイルのどれかにあてはまるのであれば、クラシックBluetoothの認証制度に従い設計するだけです。

規格で定義されたプロファイルに当てはまらない機器のために、クラシックBluetoothには、任意の通信を提供するシリアルポートプロファイル(Serial Port Profile, SPP)があります。iOSアプリケーションから、このSPPを使うことはできません。iOSデバイスと接続するクラシックBluetoothデバイスを設計するには、Made For iPhone(MFi)プログラムをApple社と締結して、機器の認証ハードウェアと開発ライブラリの提供をうけなくてはなりません。

シリアルポートプロファイルは、単なる通信路を提供するだけです。例えば、Windowsパソコンに、シリアルポートプロファイルがあるクラシックBluetoothデバイスを接続すると、デバイス・マネージャーは、その接続を仮想シリアルポートとして扱います。Windowsパソコンでは、ユーザが、Bluetoothデバイスの仮想シリアルポートのポート番号をアプリケーションに指定します。

iOSの認証ハードウェアは、これがApple社のライセンスをうけた正規製品であることを示すとともに、
この機器が接続べきiOSアプリケーションもiOSに伝えます。この、本来のシリアルポートプロファイルにはない機能があるため、周辺機器は自動的に対応するiOSアプリケーションと接続できます。

=== Bluetooth LEのプロファイルとプロトコル

Bluetooth LEは、その規格のもとで任意のプロファイルを勝手に定義することができます。例えば、先ほどの体温計のサービスを利用して、iOSアプリケーションの振る舞いの制御を変えれば、室温を計測して熱中症をアラート音で警告する振る舞いをさせることも、できます。プロトコルと、プロファイルという2つの用語があります。プロファイルはBluetooth LEを使うデバイスの振る舞いを示します。プロトコルは、通信の手順やデータ表現を示します。

Bluetooth LEのデバイスの振る舞いは、ジェネリック・アトリビュート・プロファイル(Generic Attribute Profile, GAP)として定義されています。GAPが定義するBluetooth LEデバイスの4つの役割のうち、Core Bluetoothフレームワークでアプリケーション開発者が扱うのはセントラルとペリフェラルの2つです。ペリフェラルは、そのデバイスを発見してもらうためのアドバタイジングを行なう役割です。セントラルは、アドバタイジングをしているデバイスを発見して、そのデバイスに接続する役割です。

通信機能は、機能毎に階層表示したプロトコル・スタック ( [#fig_ble_protocol_stack] ) で表されます。

リンク層は隣接するデバイス都の間に信頼できる1本の通信路を確立します。Bluetoothには、他のデバイスのパケットを他のデバイスに中継するトランスポート層はなく、隣接したデバイスとの直接通信のみをサポートします。このリンク層で通信タイミングなどを制御する中心となる役割をマスターと呼び、そのマスターに接続するデバイスの役割をスレーブと呼びます。1つのマスターに複数のスレーブが接続した小さなネットワークを、ピコネットと呼びます。

L2CAP(Logical Link Control and Adaptation Protocol)は、上層にある複数のプロトコルが、お互いの存在を気にせず、それぞれが通信できる論理的な通信路を提供します。TCP/IPネットワーキング・アプリケーション開発で使う、HTTPやFTPなどのプロトコルには、デフォルトのポート番号を指定して通信します。L2CAPの論理的な通信路は、このポート番号の指定に相当します。

アトリビュート・プロトコル(Attribute Protocol, ATT)とジェネリック・アトリビュート・プロファイル(Generic Attribute Profile, GATT)は、デバイスの持つデータの表現と、読み書きの手順を提供します。このレイヤで、データを持っているデバイスをサーバ、サーバの値を読み書きするものをクライアントと呼びます。例えば、Bluetooth LEに対応した心拍センサにiPhoneが接続した場合を考えます。心拍センサーが、心拍というデータを持っているのでサーバになります。iPhoneは、心拍の値を読みにいくクライアントになります。

GATTの、サーバ/クライアントの役割は、リンク層のマスター/スレーブの役割と関係なく割り当てられます。通常は、セントラルはマスターでクライアント、ペリフェラルにはスレーブでサーバ、の役割が割り当てられます。

=== サービスとキャラクタリスティクス

GATTは、サーバが持つデータを、キャラクタリスティクスという単位で読み書きする仕組みを提供します。また、サーバには多くのキャラクタリスティクスがありますが、そのキャラクタリスティクスをグループ化する、サービスという仕組みも提供します。

サービスは、その機器のハードウェアとしての機能を表します。例えば、体温計があるとします。体温計のハードウェアとしての機能は、温度の計測と、計測が完了した時のアラート音の出力の2つだとします。この場合、温度計測をするサービスが1つ、そして警告音を出すサービスが1つ、あるでしょう。それぞれのサービスには、温度を読み出すキャラクタリスティクス、警告音のOn/Offの指示を書き込むキャラクタリスティクスがあるでしょう。体温計という振る舞いにではなく、ハードウェアとしての機能に、サービスが割り当てられます。

Bluetooth LEのアプリケーションは、GATTの上に作られます。Bluetooth LEの規格認証の範囲はGATTまでです。サーバがどのようなサービスやキャラクタリスティクスを提供するかは、機器のアプリケーションとして実装されます。ですから、機器の開発者は、Bluetooth LEの規格認証のもとで、任意のサービスおよびキャラクタリスティクスを定義して、それを実装できます。

これはiOSアプリケーションでも同じです。iOSアプリケーションは、Core Bluetoothフレームワークを通して、GATTのサービスとキャラクタリスティクスにアクセスできます。iOSアプリケーションは、接続した機器の複数のサービスを組み合わせて、機器の振る舞いを制御します。Bluetooth LEを使うiOSアプリケーションは、パソコンで言うデバイス・ドライバに相当する部分を、アプリケーション内部に持つことになります。

クラシックBluetoothで任意のアプリケーションを作る場合は、勝手なプロファイルを定義できないため、SPPの汎用通信を使うほかありません。しかし、Bluetooth LEは、その規格のもとで任意のプロファイルを勝手に定義することができます。例えば、先ほどの体温計のサービスを利用して、iOSアプリケーションの振る舞いの制御を変えれば、室温を計測して熱中症をアラート音で警告する振る舞いをさせることも、できます。

Bluetooth SIGは、よく使われるサービスやプロファイルを定義しています。例えば、バッテリー残量やアラートの出力のようなサービスが定義されています。必要なプロファイルに使えるサービスがすでに定義されていれば、それを利用することができます。わざわざ定義や開発をする必要はありません。

また、プロファイルは、開発者が任意に定義できますが、これでは特定のメーカや機種を超えた汎用性が得られません。Bluetooth SIGは、それぞれの業界団体から提出された汎用化したプロファイルを公式のプロファイルとして審査承認して、Bluetooth LEの公式のプロファイルの定義とすることで、これを解決します。


== Core Bluetoothフレームワーク

Core Bluetoothフレームワークは、iOSアプリケーションにBluetooth LEデバイスの発見と接続そして通信を提供するフレームワークです。Bluetooth LEの
通信パラメータ設定などの詳細を隠蔽してくれるので、振る舞いのみに注力して開発ができます。

図はアプリケーションのダンプ・レポートから推測したiOSのBluetooth LEの実装です。Bluetooth LEのホストは、BTServerというデーモンが実装しています。Core BLuetoothフレームワークは、iOSアプリケーションとこのBSServerとの間のプロセス間通信(Inter Process Communication, IPC)を提供します。

=== UUIDとCBUUIDクラス

UUIDは、サーバのように統合するものがなくとも生成できる一意に特定できる128ビットの識別子のことで、IETFの[RFC412](@<href>{http://tools.ietf.org/html/rfc4122)}に詳細があります。Bluetooth LEの通信はATT層のアトリビュートというデータの単位で通信をします。アトリビュートが表す様々なデータ型の識別子に、このUUID(Universally Unique Identifier)を使います。また個別のサービスやキャラクタリスティクスの識別にもUUIDを使います。

このUUIDを表すのがCBUUIDクラスです。CBUUIDクラスのインスタンスは UUIDWithString:クラスメソッドで文字列から生成できます。

[source, swift]
----
    CBUUID *anyServiceUUID = [CBUUID UUIDWithString: @"7E20767A-30BB-4EB2-A43E-AC318D9A89A0"];
----

Bluetooth LEはUUIDの16ビットの短縮表現を決めています。これは128ビットのUUID"0000XXXX-0000-1000-8000-00805F9B34FB"のうち、X部分の16ビットだけを抜き出した表現です。短縮形の16ビットのUUIDをX部分に当てはめて、128ビットの本来のUUIDを復元できます。
例えば、0x180Dが割り振られた心拍を表すサービスの本来のUUIDは、0000180D-0000-1000-8000-00805F9B34FBになります。

UUIDWithString:クラスメソッドに16新表記で16ビットのUUIDを文字列で指定すると、この短縮形のUUIDが生成されます。

[source, swift]
----
    CBUUID *heartRateServiceUUID = [CBUUID UUIDWithString: @"180D"];
----

16ビットの短縮形UUIDは、無線通信のデータ量の削減が目的です。この16ビットの値は、Bluetooth SIGが割り当てと定義を決めており、ユーザが勝手に利用することはできません。GATTのサーバおよびクライアントは、この16ビットの値を内部で128ビットのUUIDに復元して、UUIDの比較などの処理は、128ビットのUUIDに対して実行されます。

===== UUIDの生成

任意のサービスまたはキャラクタリスティクスを定義するときには、開発者が128ビットのUUIDを生成して割り当てを決めます。UUIDを生成するネットサービスがありますが、OS Xではuuidgenというコマンドライン・ツールがあります。次のコマンドでターミナルからUUIDを生成できます。

    $ uuidgen
    71DA3FD1-7E10-41C1-B16F-4430B506CDE7

UUIDはハイフンで区切られたASCII文字列で出力されます。この文字列は、CBUUIDクラスの UUIDWithString:クラスメソッドにそのまま使えます。uuidgenコマンドに引数"-hdr"を指定すると、以下のような、そのままヘッダファイルに貼り付けられるソースコードを出力します。

    $ uuidgen -hdr
    // 5F066077-1DCF-4F13-913A-728584900517
    #warning Change the macro name MYUUID below to something useful!
    #define MYUUID CFUUIDGetConstantUUIDWithBytes(kCFAllocatorSystemDefault, 0x5F, 0x06, 0x60, 0x77, 0x1D, 0xCF, 0x4F, 0x13, 0x91, 0x3A, 0x72, 0x85, 0x84, 0x90, 0x05, 0x17)

=== セントラル・マネージャとペリフェラル・マネージャ

iOSアプリケーションはiOS5およびiOS6ではセントラル・ロールを、iOS6ではペリフェラル・ロールになれます。それぞれ役割を提供するのが、CBCentralManagerクラス、CBPeripheralManagerクラスです。

==== セントラル・マネージャの役割

iOSアプリケーションがセントラル・ロールのとき、その通信制御は CBCentralManagerクラス で行います。Bluetooth LEを使うiOSアプリケーションの処理手順は:

- ペリフェラルの発見と接続
- サービスおよびキャラクタリスティクスの検索
- キャラクタリスティクスへのデータの読み書き
- キャラクタリスティクスからの変更通知設定

です。CBCentralManagerクラスの役割は:

- アドバタイジング・データの取得
- ペリフェラルの発見と接続
- ペリフェラルの切断

です。ローカル・デバイスにセントラル・ロールを実装するサンプル・コードで、このこの3つの役割の実装を見ていきます。

===== セントラル・マネージャのインスタンス生成

セントラル・マネージャに対応するものが、CBCentralManagerクラスです。Bluetooth LEの通信を始める前に、CBCentralMangerクラスのオブジェクトにメモリ領域を割り当て、initWithDelegate:queue:メソッドで初期化します。

[source, swift]
----
    myCentralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
----

initWithDelegate:queue:メソッドの引数は2つあります。最初の引数はセントラル・ロールのイベントを受け取るデリゲートを指定します。ここではselfを指定しています。この引数に指定するインスタンスはCBCentralManagerDelegateを実装します。

2つめのquequeは、通信処理を行なうキューを指定します。nilを指定した場合はメインキューが使われます。もしも、時間がかかるBluetooth LE関連の処理を行なう場合は、その処理がメインキューを止めてしまい、ユーザ・インタフェースを邪魔してしまうため、適当なキューを指定します。

CBCentralManagerクラスのインスタンスはiOSアプリケーションに1つだけ持つようにします。2つ以上を持っても動作するかもしれませんが、おかしな挙動をしたり、またiOSのバージョン更新で動作しなくなるかもしれません。また、アプリケーションがBluetooth LEで通信する間は、CBCentralManagerのインスタンスを保持します。デバイスとの接続が完了したからと、CBCentralManagerを解放しないようにします。

===== Bluetoothの状態取得

CBCentralManagerのstateプロパティは、Bluetoothの状態を表すCBCentralManagerState列挙型で、その値と意味は次のとおりです:

- CBCentralManagerStateUnknown = 0,
    - 初期値です。すぐに更新されます。
- CBCentralManagerStateResetting,
    - システムサービスとの接続が一時的に失なわれました。すぐに更新されます。
- CBCentralManagerStateUnsupported,
    - Bluetooth low energyのCentral/Clientをサポートしていません。
- CBCentralManagerStateUnauthorized,
    - このアプリケーションは、Bluetooth low energyのCentral/Client使う認可がありません。
- CBCentralManagerStatePoweredOff,
    - Bluetoothがオフになっています。
- CBCentralManagerStatePoweredOn
    - Bluetoothがオンで、かつ、いま利用できます。

CBCentralManagerをインスタンスした直後のstateプロパティは、CBCentralManagerStateUnknown(=0)の初期値です。このstateプロパティが変更されると、CBCentralManagerDelegateプロトコルのcentralManagerDidUpdateStateメソッドが呼び出されます。このメソッドにはCBCentralManagerのインスタンスが渡されます。

CBCentralManagerDelegateプロトコルには、たくさんのメソッドがありますが、必ず実装しなければいけない(required)ものはcentralManagerDidUpdateStateメソッドだけです。その他のメソッドは、オプショナルです。もしもCBCentralManagerDelegateプロトコルのメソッド名をタイプミスしていても、コンパイラは警告を出してくれません。タイプミスは入力補完やドキュメントからのコピーを利用して防止します。

CBCentralManagerのインスタンスを作ると、直ちにstateの値が変更されます。Bluetooth4に対応していないiOSデバイスであれば、stateプロパティはCBCentralManagerStateUnsupportedになります。Bluetooth LEを使うことはできないので、iOSアプリケーションで対処をします。Bluetooth4対応のiOSデバイスであれば、stateプロパティはCBCentralManagerStatePoweredOnまたはCBCentralManagerStatePoweredOffに変更されます。

Bluetoothの電源On/Offは、iOSアプリケーションから操作することはできません。ユーザがiOSの設定アプリケーションから、Bluetoothの電源On/Offができます。ペリフェラルを検索するために、iOSアプリケーションがスキャンを開始したときに、Bluetoothの電源がOffであれば<!--TBD  スキャン時だっけ?-->、Bluetoothの電源をOnにする画面が自動で表示されます<!--TBD 電源ONのダイアログ  -->。設定ボタンを押すと、自分のiOSアプリケーションからiOSの設定アプリケーションに遷移して、Bluetoothの電源設定画面が表示されます。

<!--
TBD: これを引き起こす再現実験とその時の処理の確認はできないだろうか?
BTServerが異常終了してBluetooth LEの通信制御が停止するなどで、Bluetooth LEの接続が失われた場合は、CBCentralManagerStateResettingに遷移するようです。接続が失われたとして、切断処理をおこない、そして再接続処理を行います。

TBD: これは、おそらく書き込みの時のシグネチャとか、そのあたりで引っかかりそう。SMを使わない時。
再現実験をして、その場合の状態を書き込もう。
CBCentralManagerStateUnauthorized,
-->

<!--
TBD
状態遷移したときの処理のコードをここに書く。エラーのときの遷移です。
-->

===== ペリフェラルのスキャン

セントラルが行なう最初のタスクは、ペリフェラル・デバイスの発見です。ペリフェラル・デバイスは、自分の存在を周囲のセントラル・デバイスに伝えるために、アドバタイジング・パケットを送信しています。CBCentralManagerのscanForPeripheralsWithServices:options:メソッドを呼び出して、セントラルのアドバタイジング・パケットの受信を開始します。

[source, swift]
----
    [myCentralManager scanForPeripheralsWithServices:nil options:nil];
----

scanForPeripheralsWithServices:options:メソッドの引数は2つあります。最初の引数は、<!--TBD  一致は論理積なのか和なのか  -->ペリフェラルをフィルタリングするためのものです。発見したいサービスのUUIDを、NSArrayで配列にして指定します。サービスはペリフェラルの機能に対応しています。サービスでフィルタリングすることで、セントラルが必要とする機能がある機器のみを、発見できます。この引数にnilを指定すると、フィルタリングせず、発見した全てのペリフェラルが報告されます。Appleは、Core Bluetoothプログラミング・ガイドで、より低消費電力の動作にするために、サービスを指定することを推奨しています。nilを指定することは、推奨していません。

2つめの引数optionsは、スキャンの動作を指定するオプションをNSDictionaryクラスのインスタンスで渡します。NSDictionaryには、オプションの種類をキー値に、そのオプション値を値に設定します。指定できるオプションは、CBCentralManagerScanOptionAllowDuplicatesKey のみで、値にBooleanをとります。この引数がnilのときは、CBCentralManagerScanOptionAllowDuplicatesKey の値をNOに設定したのと同じになります。

CBCentralManagerScanOptionAllowDuplicatesKey の値をYESにすると、ローカルのセントラルデバイスがペリフェラルからのアドバタイジング・パケットを受信するたび、ペリフェラルの発見が通知されます。NOの場合は、初めてアドバタイジング・パケットを受信した時にペリフェラルの発見が通知され、それ以降、同じペリフェラルからアドバタイジング・パケットを受信しても、発見を通知しません。

このオプションは:

- 非接続で電波強度を知りたいとき
- ブロードキャスタのデータを取るとき

によく使われます。アドバタイジング・パケットの受信通知では受信信号強度(Received Signal Strength Indicator,RSSI)も通知されます。RSSIから、ペリフェラルとの距離を大雑把に推定できます。ペリフェラルが近くに来た時に初めて接続させたいときなどに、用います。

周囲のBluetooth LEデバイスに非接続で情報を広報するものをブロードキャスターといいます。ブロードキャスタは、位置ビーコンなど、周囲の不特定多数のデバイスに20バイト<!--TBD  だっけ? -->程度までの少量の同じ情報を伝えます。ブロードキャスタのデータが不変であれば、アドバタイジング・パケットの受信通知は1度だけで済みます。ブロードキャスターまでの位置を大雑把に把握し続けたいとき、またはブロードキャスターのデータが変化するときには、このオプションを使います。

スキャンを一旦開始すると、スキャンし続けます。タイムアウトなど自動的に停止することはありません。スキャンは、iOSアプリケーションが、CBCentralManagerのstopScanメソッドを呼び出して、明示的に停止しなければなりません。

iOSアプリケーションがすでにスキャンを開始しているときに、scanForPeripheralsWithServices:options:メソッドをパラメータを指定して呼び出すと、指定したパラメータで更新されて、動作が切り替わります。

====== アドバタイジング・パケットの受信

ローカルのセントラルがアドバタイジング・パケットを受信すると、CBCentralManagerDelegateプロトコルのcentralManager:didDiscoverPeripheral:advertisementData:RSSI: メソッドに通知されます。

[source, swift]
----
    -(void)centralManager:(CBCentralManager *)central
    didDiscoverPeripheral:(CBPeripheral *)peripheral
        advertisementData:(NSDictionary *)advertisementData
                     RSSI:(NSNumber *)RSSI {
      NSLog(@"Discovered %@", peripheral.name);
      ...
----

centralManager:didDiscoverPeripheral:advertisementData:RSSI: メソッドには4つの引数があります。最初の引数はCBCentralManagerクラスのインスタンスを、次の引数はペリフェラルを表すCBperipheralクラスのインスタンスです。3つ目の引数 advertisementData は、アドバタイジング・パケットのデータをおさめたNSDictionaryクラスのインスタンスです。最後のRSSIは受信信号強度を表します。

受信信号強度の値は、次の式で与えられます:

$$RSSI = 10 * log I(mW) (dBm)$$

この式のIは、受信信号電力をミリワット単位で表します。RSSIの値は、送信電力、デバイスとの距離等できまる伝搬損失、そして受信感度で決まります。受信信号電力は桁違いに変化するため、扱いやすくするためにRSSIは対数で表現されます。RSSIは、目安として、-30 dBm から-100 dBm までのマイナスの値を取ります。

scanForPeripheralsWithServices:options:メソッドのオプションに、CBCentralManagerScanOptionAllowDuplicatesKeyにYESの値設定していれば、同じペリフェラルのアドバタイジング・パケットを受診するたび、このcentralManager:didDiscoverPeripheral:advertisementData:RSSI: メソッドが呼び出されます。これは変化するアドバタイジング・データやRSSIのモニタリングに使います。

===== アドバタイジング・データ

アドバタイジング・パケットのペイロードは、AD structureの配列です。AD structureは、1オクテットのAD typeと、それにつづくAD Dataというバイト・データです <!--TBD  参照 第2章の ===== アドバタイジング・データのフォーマット  -->。

scanForPeripheralsWithServices:options:メソッドの3つ目の引数advertisementDataは、アドバタイジング・パケットから読みだしたデータを収めたNSDictionaryクラスのインスタンスです。キー値にAD typeに対応した文字列が、バリューにはキー値に対応するクラスのインスタンスが入ります。アドバタイジング・パケットにどのようなタイプの情報を入れるかは、ペリフェラル次第です。もしもアドバタイジング・パケットにキー値に対応する値がない場合は、そのキーバリューペアはありません。
<!--TBD  パッシブとアクティブでキャッシングされるの?  -->

次の6つのキー値が CoreBluetooth/CBAdvertisementData.h に定義されています。

- CBAdvertisementDataLocalNameKey
- CBAdvertisementDataTxPowerLevelKey
- CBAdvertisementDataServiceUUIDsKey
- CBAdvertisementDataOverflowServiceUUIDsKey (iOS6以降)
- CBAdvertisementDataManufacturerDataKey
- CBAdvertisementDataServiceDataKey

CBAdverisementDataLocalNameKey のバリューは、ペリフェラルのローカル・ネームを表す、NSStringクラスのオブジェクトです。ローカル・ネームは、特定のペリフェラルを探すのによく使います。機種番号であったり、あるいはシリアル番号を含む何かの識別文字列が割り当てられます。

AD typeには、完全なローカル名を表すタイプと、ローカル名が長すぎてアドバタイジング・パケットに収まりきらない場合に使う短縮形の2つのタイプがあります。AD typeがいずれの値でも<!--TBD  確認 -->CBAdverisementDataLocalNameKey の1つのキーで、ローカル名が返されるので、ここでそれらを区別する方法はありません。

CBAdvertisementDataTxPowerLevelKey のバリューは、ペリフェラルの送信電力を表すNSNumberクラスのインスタンスです。送信電力の単位はdBmです。ペリフェラルとの距離の推定に、送信電力値を使います。

セントラルのスキャンには、パッシブ・スキャンとアクティブ・スキャンがあります。ペリフェラルが送信するアドバタイジング・パケットを受信するだけなのが、パッシブ・スキャンです。セントラルからペリフェラルに、更にアドバタイジング・パケットをリクエストするのが、アクティブ・スキャンです。

CBAdvertisementDataServiceUUIDsKey のバリューは、ペリフェラルのサービスを表すCBUUIDクラスのインスタンスの配列(NSArrayクラスのインスタンス)です。CBAdvertisementDataOverflowServiceUUIDsKey はiOS6移行にあるキー値です。アクティブ・スキャンで得られた、ペリフェラルのサービスを表すCBUUIDクラスのインスタンスの配列(NSArrayクラスのインスタンス)です。

周囲の不特定多数のレシーバに一方向にデータを伝える役割をブロードキャスタと呼びます。位置ビーコンなどに使われます。アドバタイジング・パケットは、ペリフェラルの存在を周囲に知らせる以外に、ブロードキャストに使われます。

CBAdvertisementDataManufacturerDataKey のバリューは、ペリフェラルの設計製造会社が設定できる任意のバイト・データを表すNSDataクラスのインスタンスです。Bluetooth SIGが企業に割り当てた2バイトの識別子に任意のデータが続きます。CBAdvertisementDataServiceDataKeyのバリューは、NSDataクラスのインスタンスで、そのバイト・データは先頭2バイトがサービスを表す16ビットのUUIDが、その後にサービスに関連する任意のバイト・データが続きます。

ブロードキャスティングに、いずれのキー値が使われるかは、ペリフェラルの実装次第です。もしも、その仕様が製造業者が独自に定義したものであれば、CBAdvertisementDataManufacturerDataKey が使われるでしょう。仕様が特定の製造会社に縛られない一般的なもので、Bluetooth SIGが承認したサービスの定義があるならば、CBAdvertisementDataServiceDataKey が使われるでしょう。

===== ペリフェラルへの接続

ペリフェラルが発見できれば、次にそれが接続したいペリフェラルかを判定して、接続処理を行います。

[source, swift]
----
    -(void)centralManager:(CBCentralManager *)central
    didDiscoverPeripheral:(CBPeripheral *)peripheral
        advertisementData:(NSDictionary *)advertisementData
                     RSSI:(NSNumber *)RSSI {
      ...
    NSString *localName = [advertisementData objectForKey:CBAdvertisementDataLocalNameKey];
    if(localName != nil && [localName isEqualToString:@"devide name"] ) {
      _peripheral = peripheral;
      [myCentralManager connectPeripheral:peripheral options:nil];
    }
----

ペリフェラルが接続先か否かは、引数advertisementDataの内容で判別します。ローカル・ネームが対応機種名かで判定する実装が、よく使われます。接続するならば、CBCentralManagerクラスのconnectPeripheral:options: メソッドを呼び出します。

CBPeripheralクラスのインスタンスは、そのペリフェラルの利用が終了するまで、必ずiOSアプリケーションで保持します。上のコードでは、引数_peripheral_をインスタンス変数 _\_peripheral_ で保持しています。CBperipheralクラスのインスタンスを解放すると、CBCentralManagerは、接続開始処理および接続状態を終了させて通信を切断します。CBperipheralManagerは、CBPeripheralクラスのインスタンスを保持しません。

Bluetooth LEの接続処理は、アドバタイジング・パケットを受信した後に、直ちに接続要求を出さねばなりません。したがって、connectPeripheral:options:メソッドを呼び出しても、接続が完了するのは、次のアドバタイジング・パケットを受信したタイミング以降になります。<!--TBD  接続完了の時間  -->

ペリフェラルとの接続が確立すると、CBCentralManagerDelegateプロトコルのcentralManager:didConnectPeripheral:メソッドが呼び出されます。この引数peripheralは、connectPeripheral:options:メソッドに指定したCBPeripheralクラスのインスタンスと同じものです<!--TBD  確認  -->。

[source, swift]
----
- (void)centralManager:(CBCentralManager *)central
  didConnectPeripheral:(CBPeripheral *)peripheral {
    NSLog(@"Peripheral connected");
    ...
----

接続処理に失敗した場合は、CBCentralManagerDelegateプロトコルのcentralManager:didFailToConnectPeripheral:error:メソッドが呼び出されます。接続失敗の原因は、引数_error_で渡されます。

[source, swift]
----
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"Failed to connect to a Peripheral");
    ...
----

===== サービスの検索

ペリフェラルには様々な機能があります。その機能それぞれを表すのが、Bluetooth LEのサービスです。ペリフェラルと接続できたならば、次は必要なサービスを検索します。

[source, swift]
----
- (void)centralManager:(CBCentralManager *)central
  didConnectPeripheral:(CBPeripheral *)peripheral {
    ...
    peripheral.delegate = self;
    [peripheral discoverServices:nil];
----

ペリフェラルの処理結果は、デリゲートのメソッド呼び出しで非同期に返されます。このコードはCBPeripheralDelegateプロトコルを実装した_self_を、引数_peripheral_のdelegateプロパティに設定します。

CBPeripheralクラスのdiscoverServices:メソッドを呼び出して、サービスの検索を開始します。ペリフェラルの全てのサービスを検索するならば、引数に_nil_を指定します。検索するサービスを指定するならば、NSArrayクラスのインスタンスで、指定したいサービスのUUIDを設定したCBUUIDクラスのインスタンスを配列にして、引数に渡します。

ペリフェラルを発見した時のアドバタイジング・パケットのサービスUUIDよりも、このdiscoverServices:メソッドで発見できるサービスのほうが、多いかもしれません。これは、128ビットのサービスUUIDはアドバタイジング・パケットに1つしか入らないため、アドバタイジング・パケットですべてのサービスを取得できるとは、限らないからです。

Core Bluetoothフレームワークは、discoverServices:メソッドに、サービスを指定することを推奨しています。サービスの検索には通信時間と処理時間がかかります。検索対象のサービスを事前に指定することで、それらをより短くできます。

サービスを発見すると、そのCBPeripheralのインスタンスのdelegateプロパティに設定したインスタンスの、CBPeripheralDelegateプロトコルのperipheral:didDiscoverServices: メソッドが呼ばれます。

[source, swift]
----
- (void)peripheral:(CBPeripheral *)peripheral
didDiscoverServices:(NSError *)error {
    for (CBService *service in peripheral.services) {
        NSLog(@"Discovered service %@", service);
        if ([service.UUID.data isEqualToData:_targetServiceUUID.data]) {
            [peripheral discoverCharacteristics:nil forService:service];
        }
        ...
----

引数peripheralのservicesプロパティは、NSArrayクラスのインスタンスで、その内容はCBServiceクラスのインスタンスの配列です。もしもサービス検索でエラーが発生していれば、引数_error_にその内容が入ります。

===== キャラクタリスティクスの検索

使いたいサービスの検索が完了すれば、次はサービスそれぞれの、アクセスしたいキャラクタリスティクスを取得していきます。サービスごとに検索したいキャラクタリスティクスは異なるので、まず、CBPeripheralクラスのインスタンスのservicesプロパティのインスタンスが、どのサービスに対応しているのかを判別しなければなりません。これは、CBServiceクラスのインスタンスのUUIDプロパティの一致で判定できます。

[source, swift]
----
- (void)peripheral:(CBPeripheral *)peripheral
didDiscoverServices:(NSError *)error {
    for (CBService *service in peripheral.services) {
        NSLog(@"Discovered service %@", service);
        if ([service.UUID.data isEqualToData:_targetServiceUUID.data]) {
            [peripheral discoverCharacteristics:nil forService:service];
        }
        ...
----

CBPeripheralクラスの discoverCharacteristics:forService: メソッドを呼び出してキャラクタリスティクスを検索します。このメソッドには2つの引数があります。最初の引数は、検索したいキャラクタリスティクスのUUIDの配列を指定します。forService:には検索対象のサービスを指定します。

discoverCharacteristics:forService: メソッドの最初の引数にnilを指定すると、そのサービスの全てのキャラクタリスティクスを検索します。iOSアプリケーションが、そのペリフェラルをどう使うかは、設計時点で決まるので、ここでnilを渡すことは、あまりありません。キャラクタリスティクスの検索には、通信時間がかかり電池を消費するため、Apple社のプログラミングガイドは、キャラクタリスティクスのUUIDを指定することを推奨しています。

キャラクタリスティクスが検索できると、CBPeripheralDelegateプロトコルのperipheral:didDiscoverCharacteristicsForService:error: メソッドが呼ばれます。

[source, swift]
----
- (void)peripheral:(CBPeripheral *)peripheral
didDiscoverCharacteristicsForService:(CBService *)service
             error:(NSError *)error {
    for (CBCharacteristic *characteristic in service.characteristics) {
        NSLog(@"Discovered characteristic %@", characteristic);
        ...
} ...
----

キャラクタリスティクスは、CBCharacteristicクラスのインスタンスで与えられます。引数serviceのcharacteristicsプロパティは、このCBCharacteristicのインスタンスをNSArrayで配列にしたものです。キャラクタリスティクスの取得時にエラーが発生した場合は、引数errorでエラー情報が渡されます。正常動作時は、引数errorの値はnilです。

===== キャラクタリスティクスの読み出し

ペリフェラルの値の読み書きは、キャラクタリスティクスを通して行います。例えば、ペリフェラルが心拍計なら、心拍計速のサービスのなかに、心拍数を表すキャラクタリスティクスがあります。

キャラクタリスティクスの値読み出し方法には、直接読み出しと、サブスクリプションの2つがあります。直接読み出しは、セントラルからペリフェラルに読み出し要求を送信して、それを受けたペリフェラルがセントラルにキャラクタリスティクスのデータを返す方法です。サブスクリプションは、ペリフェラルがキャラクタリスティクスの値を変更したときに、ペリフェラルがセントラルに、更新された値を送信する方法です。

Bluetooth LEの通信プロトコルには、キャラクタリスティクスの読み出し方法には、ノーティフィケーションとインディケーションの2つがあります。Core Bluetoothフレームワークのキャラクタリスティクスの読み出しでは、この2つを区別する方法はありません。用語を区別して混同しないように、Core Blutoothフレームワークのペリフェラルからセントラルの変更通知を、ここではサブスクリプションと呼びます。

キャラクタリスティクスの値が固定値ならば、接続時に1度読み出せばいいので、直接読み出しをします。値が変化するキャラクタリスティクスを常にモニタしたいならば、サブスクリプションを使います。セントラルが一定周期で読み出しをするポーリングに比べて、値の変更時に通知してくれるサブスクリプションは、無駄に無線通信することがありません。

キャラクタリスティクスの読み出しには、CBPeripheralクラスのreadValueForCharacteristic:メソッドを使います。引数には、読み出したいキャラクタリスティクスに対応するCBCharacteristicのインスタンスを与えます。

[source, swift]
----
    NSLog(@"Reading value for characteristic %@", interestingCharacteristic);
    [peripheral readValueForCharacteristic:interestingCharacteristic];
----

キャラクタリスティクスの読み出し結果は、CBPeripheralDelegateプロトコルのperipheral:didUpdateValueForCharacteristic:error:メソッドで返されます。

[source, swift]
----
- (void)peripheral:(CBPeripheral *)peripheral
didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic
             error:(NSError *)error {
    NSData *data = characteristic.value;
    // parse the data as needed
    ...
----

読み出しに成功すれば引数errorはnilです。もしも失敗した場合は、引数errorにはエラー内容を表すNSErrorのインスタンスが与えられます。読み出しに成功していれば、読み出し値は、引数characteristicのvalueプロパティから取り出せます。

読み出しでは、全てのキャラクタリスティクスに読み出し権限があるとは限らないことに注意します。キャラクタリスティクスには、それぞれにパーミションが設定されています。パーミションの、読み出しフラグと、サブスクリプション権限のフラグは、それぞれ独立して設定できます。このため、読み出しはできるがサブスクリプションはできない設定や、逆に、読み出しができないがサブスクリプションはできる設定が、ありえます。読み出し権限がないキャラクタリスティクスに直接読み出しをした場合、あるいはサブスクリプション権限のフラグがない場合にサブスクリプションした場合は、エラーになります。

キャラクタリスティクスのパーミション設定は、列挙型CBCharacteristicProperties のpropertiesプロパティから読み出せます。列挙型CBCharacteristicPropertiesには、読み出し権限以外にも多くの権限が定義されています。それぞれの権限はビット・フラグで、論理和で設定できます。

読み出し権限があることを確認するならば、CBCharacteristicPropertyRead とCBCharacteristicのpropertyプロパティの論理和が0でないことを、確認します。通常は、ペリフェラルの仕様書に、キャラクタリスティクスのパーミション設定が明記されています。ですから、iOSアプリケーションでパーミション設定を調べることは、あまりありません。

キャラクタリスティクスの値変更通知を受け取るには、CBPeripheralクラスのsetNotifyValue:forCharacteristic:メソッドを使います。サブスクリプションの権限がないキャラクタリスティクスに、このメソッドを呼び出しても、なにも変化しません。

[source, swift]
----
￼￼￼￼[peripheral setNotifyValue:YES forCharacteristic:interestingCharacteristic];
----

setNotifyValue:forCharacteristic:メソッドには引数が2つあります。最初の引数は、通知を受け取るか受け取らないかを指定するBooleanの値です。YESまたはNOを与えます。2つ目の引数は、通知を受けたいキャラクタリスティクスのCBCharacteristicのインスタンスです。

通知設定のデフォルト値はNOです。また、セントラルとの通信が切断した時に、ペリフェラルは通知設定をデフォルト値NOに初期化します。ですから、サブスクリプションをするならば、セントラルはペリフェラルに接続した都度、設定します。

サブスクリプションをYESに設定すると、キャラクタリスティクスの値が更新されると、その更新値が
CBPeripheralDelegateプロトコルの peripheral:didUpdateNotificationStateForCharacteristic:error: メソッドで通知されます。

[source, swift]
----
- (void)peripheral:(CBPeripheral *)peripheral
didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic
             error:(NSError *)error {
    if (error) {
        NSLog(@"Error changing notification state: %@",
} ...
----

引数の意味や使いかたは、値読み出しの peripheral:didUpdateValueForCharacteristic:error:メソッドのそれと、同じです。

===== キャラクタリスティクスへの書き込み

ペリフェラルへの値の書き込みは、読み込みと同じくキャラクタリスティクスを通じて行います。値の書き込みは、ペリフェラルに保存されているデータを変更する意味のほか、例えばエアコンの赤外線リモコンに開始/停止の動作指定や温度設定のボタンがあるように、動作や目標値を指示する意味もあります。

[source, swift]
----
    NSLog(@"Writing value for characteristic %@", interestingCharacteristic); [peripheral writeValue:dataToWrite forCharacteristic:interestingCharacteristic
         type:CBCharacteristicWriteWithResponse];
----

キャラクタリスティクスに書き込む CBPeripheral クラスの writeValue:forCharacteristic:type: メソッドには、3つの引数があります。最初の引数には書き込みたいバイト・データを収めたNSDataのインスタンスを、2つ目の引数には書き込み対象のキャラクタリスティクスの CBCharacteristic のインスタンスを、最後の3つ目の引数には書き込みタイプを表す定数を与えます。

サンプルコードの最初の引数_dataToWrite_のバイト・サイズは20バイト以下にします。*キャラクタリスティクスに書き込めるバイト・データのサイズは20バイト以下です。* 20バイトよりも大きいデータを与えた場合は、その部分のバイトデータはペリフェラルに送信されません。

キャラクタリスティクスに書き込めるデータ・サイズの上限は、Bluetooth LEのアトリビュート・プロトコル(Attribute Protocol, ATT)によるものです。ATTのプロトコル・データ・ユニット(Protocol Data Unit, PDU)の最大長 Maximum Transfer Unit(MTU)、ATT\_MTU と表します、はデフォルト値が23バイトです。ヘッダを除くと、1つのPDUで送れるデータ・サイズは20バイトになります。これが、キャラクタリスティクスに1回で書き込めるデータ・サイズを決めています。

ATTには、セントラルとペリフェラルがそれぞれのATT\_MTUのサイズを交換しあって、デフォルト値23バイトよりも大きなATT\_MTUを扱えるようにする仕組みがあります。しかしiOS5およびiOS6は、このATT\_MTUのサイズ交換に対応していません。

3つ目の引数typeは列挙型 CBCharacteristicWriteType の値です。この列挙型は、2つの値 CBCharacteristicWriteWithResponse (= 0)および CBCharacteristicWriteWithoutResponse を定義しています。このサンプルコードは、引数_type_に CBCharacteristicWriteWithResponse を指定しています。

この書き込みタイプは、Bluetooth LEの通信プロトコルの2つの書き込み方法に対応しています。CBCharacteristicWriteWithResponseを指定すると、セントラルはライト・リクエストでペリフェラルに書き込みを行います。ペリフェラルは、書き込み処理の成功失敗に関係なく、処理結果をセントラルに返します。この結果はデリゲートを通じてiOSアプリケーションに返されます。これは、ペリフェラルの不揮発メモリの書き込みなどの、書き込みの完了を必ず確認しなければならない場合に使います。

CBCharacteristicWriteWithoutResponse を指定すると、セントラルは、ライト・コマンドを使って書き込みます。ペリフェラルは、書き込み処理が成功しても失敗しても、セントラルに結果を返すことはありません。これは、例えばエアコンのリモコンの動作開始指示ボタンのような、動作を指示する場合などに使われます。

書き込みをしたのに、ペリフェラルの動作に反映されないなどの、書き込み処理の失敗は、大抵ペリフェラルの都合によるものです。セントラルのライト・コマンドが通信路で失われて、ペリフェラルが受信できないことは、ありえません。接続している限り、リンク層がセントラルとペリフェラル間に信頼できる通信を提供しています。コマンドは無視してもよいため、処理時間がないなどの理由があると、ペリフェラルはコマンドの処理をスキップするかもしれません。

キャラクタリスティクスが、どの書き込みのタイプに対応しているかは、キャラクタリスティクスのパーミション設定で判別ができます。CBCharacteristicクラスのpropertyプロパティと、
CBCharacteristicPropertyWrite との論理積が0でなれば(ビットが立っていれば)、そのキャラクタリスティクスはレスポンスありの書き込み対応しています。CBCharacteristicPropertyWriteWithoutResponseとの論理積が0でないならば、レスポンスなしの書き込みに対応しています。この2つのパーミションは、それぞれ独立に設定できるので、キャラクタリスティクスが書き込み可能であれば、パーミション設定には3通りあります。

writeValue:forCharacteristic:type: メソッドの引数_type_に CBCharacteristicWriteWithResponse を指定したとき、CBPeripheralDelegateプロトコルのperipheral:didWriteValueForCharacteristic:error: メソッドに、書き込み結果が返されます。

[source, swift]
----
- (void)peripheral:(CBPeripheral *)peripheral
didWriteValueForCharacteristic:(CBCharacteristic *)characteristic
             error:(NSError *)error {
    if (error) {
        NSLog(@"Error writing characteristic value: %@",
} ...
----

引数は3つあります。読み込み処理の場合と同じように、エラーが発生すれば引数_error_にその内容が渡されます。正常に処理できたならば、引数_error_はnilです。

==== ペリフェラル・マネージャーの役割

Bluetooth LEにはセントラルとペリフェラルという2つの役割があります。iOS6から、iOSアプリケーションがペリフェラル・ロールになれるCBPeripheralManagerクラスが提供されています。ペリフェラル・ロールは、Bluetooth LEの、リンク層のスレーブ、ジェネリック・アトリビュート・プロトコル層ではサーバの役割を担います。

ペリフェラル・ロールのiOSアプリケーションは:

- アドバタイジング・パケットの送信
- セントラルとの接続および切断
- 読み出しおよび書き出し要求の処理
- 通知処理

を行います。CBPeripheralManagerクラスの役割は:

- アドバタイズ
- サービスおよびキャラクタリスティクスのデータベースの作成と公開
- セントラルからのリクエストおよびコマンドの処理

です。ローカル・デバイスにペリフェラル・ロールを実装するサンプル・コードで、このこの3つの役割の実装を見ていきます。

===== ペリフェラルのマネージャーを開始する

ペリフェラルの役割はCBPeripheralManagerクラスが提供します。CBPeripheralManagerクラスのinitWithDelegate:queue:メソッドでインスタンスを生成します。

[source, swift]
----
    myPeripheralManager = [[CBPeripheralManager alloc] initWithDelegate:self queue:nil];
----

initWithDelegate:queue:メソッド
の引数は2つあります。最初の引数はペリフェラル・ロールのイベントを受け取るCBPeripheralManagerDelegateプロトコルを実装したデリゲートを指定します。ここではselfを与えています。2つめのquequeは、通信処理を行なうキューを指定します。nilを指定した場合はメインキューが使われます。

CBPeripheralManagerクラスのインスタンスは、iOSアプリケーションに1つだけ持つようにします。CBPeripheralManagerを2つ以上インスタンスした時の振る舞いがどうなるかは、わかりません。

CBPeripheralManagerDelegateプロトコルは、必ず実装しなければならないperipheralManagerDidUpdateState:メソッドと、その他のオプションのメソッドがあります。peripheralManagerDidUpdateState:メソッドは、Bluetoothの電源やペリフェラル・マネージャーの状態を伝えます。その他のメソッドは、ローカルのデータベースに接続およびアクセスするセントラルについての情報を提供します。

===== ペリフェラル・マネージャーの状態

CBPeripheralManagerのstateプロパティは、ペリフェラル・マネージャーの状態を伝える列挙型CBPeripheralManagerStateの値です。CBPeripheralManagerのインスタンスを作成したときは、stateプロパティの値はCBPeripheralManagerStateUnknownです。このstateプロパティが変化すると、その都度CBPeripheralManagerDelegateプロトコルのperipheralManagerDidUpdateState:メソッドが呼ばれます。

列挙型CBPeripheralManagerStateの値とその意味は次のとおりです:

- CBPeripheralManagerStateUnknown
    - インスタンスした直後の、初期値を示します。すぐにペリフェラル・マネージャーの状態を表す値に変更されます。
- CBPeripheralManagerStateResetting
    - システムサービスとの接続が一時的に失われたことを示します。この値はすぐに更新されます。
- CBPeripheralManagerStateUnsupported
    - この機種が、Bluetooth LEのスレーブ/サーバをサポートしていないことを示します。
- CBPeripheralManagerStateUnauthorized
    - このアプリケーションに、Bluetooth low energy のスレーブ/サーバを使う権限がありません。
-CBPeripheralManagerStatePoweredOff
    - Bluetoothがオフになっています。
ｰ CBPeripheralManagerStatePoweredOn
    - BluetoothがONで、利用できます。

Bluetooth LEに対応した機種であれば、ペリフェラル・マネージャーの状態は、CBPeripheralManagerStatePoweredOn状態とCBPeripheralManagerStatePoweredOff状態のいずれかです。

ペリフェラル・マネージャーは、アドバタイジングを開始する前に、ローカルのデータベースを構築してサービスおよびキャラクタリスティクスの設定を行います。この設定は、CBPeripheralManagerStatePoweredOn状態のときのみ、実行できます。

もしもペリフェラル・マネージャーがアドバタイジングをしている間に、stateプロパティがCBPeripheralManagerStatePoweredOff状態になると、アドバタイジングは停止されます。
電源がOnになっても、アドバタイジングは自動的に再開されません。ペリフェラル・マネージャーが明示的にアドバタイジングを再開します。また、Off状態になった時に、ローカル・データベースは全てクリアされます。このため、ペリフェラル・マネージャーは、サービスも再度追加しなければいけません。

===== サービスとキャラクタリスティクスの構築

Bluetooth LEのペリフェラルのデータベースは、ハンドル、タイプ、バリューの3つのフィールドで構成されるアトリビュートの集合です。アトリビュート・プロトコルが、タイプやハンドルを指定してアトリビュートにアクセスするプロトコルを提供しています。そして、ジェネリック・アトリビュート・プロファイルは、このアトリビュートを使って、サービスとキャラクタリスティクスという概念を導入します。キャラクタリスティクスは、値を保持するものです。サービスはキャラクタリスティクスの集合を表します。

iOSアプリケーションが、内部のデータを外部に公開するには、まずペリフェラル・マネージャーのデータベースを構築します。セントラル・ロールでは、サービスおよびキャラクタリスティクスは、それぞれCBServiceクラスおよびCBCharacteristicクラスに対応していました。ペリフェラル・マネージャーは、サービスおよびキャラクタリスティクスの編集に、それぞれCBMutableServiceクラスおよびCBMutableCharacteristicクラスを使います。CBMutableServiceクラスはCBServiceクラスを、またCBMutableCharacteristicクラスはCBCharacteristicクラスを継承します。

まずサービスとキャラクタリスティクスのUUID、そしてキャラクタリスティクスのパーミション設定と役割を定義します。UUIDを表すCBUUIDクラスおよびUUIDの生成方法は(TBD 参照)で述べました。Bluetooth SIGが公開しているサービスを実装するならば16ビットのUUIDを使います。任意のサービスを定義するならば128ビットのUUIDを生成します。

まずCBMutableCharacteristcクラスのインスタンスを生成します。CBMutableCharacteristicクラスのinitWithType:properties:value:permissions:メソッドを使います。

[source, swift]
----
    myCharacteristic =
        [[CBMutableCharacteristic alloc] initWithType:myCharacteristicUUID
         properties:CBCharacteristicPropertyRead
         value:myValue permissions:CBAttributePermissionsReadable];
----

initWithType:properties:value:permissions:メソッドには引数が4つあります。最初の引数initWithTypeはキャラクタリスティクスのUUIDを表すCBUUIDのインスタンスを与えます。引数valueは、キャラクタリスティクスの値を表すNSDataのインスタンスを与えます。引数propertiesには、キャラクタリスティクスの属性をCBCharacteristicProperties列挙型の値で与えます。引数permissionsには、キャラクタリスティクスのパーミションをCBAttributePermissions列挙型の値で与えます。この2つの列挙型は、いずれも列挙型の複数の値を論理和でまとめて指定できます。

引数valueがnilの場合は、そのキャラクタリスティクスの値は、変化するものとされます。リモートのセントラルがキャラクタリスティクスの値を読み出すと、それがデリゲートに通知されます。デリゲートに、値の返信処理の責任があります。

引数valueにNSDataのインスタンスを与えると、Core Bluetoothフレームワークは、そのキャラクタリスティクスの値は固定値として扱います。キャラクタリスティクスのプロパティは自動的に読み込み可能になります<!--TBD  自動的か確認する-->。引数valueの値はキャッシュされて、リモートがこのキャラクタリスティクスを読み出すと、そのキャッシュされた値が自動的にセントラルに返されます。このとき、ペリフェラル・マネージャーはデリゲートのメソッドを呼び出しません。<!--TBD 動作確認  -->

サービスは、キャラクタリスティクスの集合です。CBMutableServiceクラスのinitWithType:primary:メソッドでインスタンスを生成します。

[source, swift]
----
    myService = [[CBMutableService alloc] initWithType:myServiceUUID primary:YES];
    myService.characteristics = @[myCharacteristic];
----

initWithType:primary:メソッドは2つの引数をとります。最初の引数は、そのサービスのUUIDを表すCBUUIDのインスタンスです。2つ目の引数はサービスがプライマリ・サービスかを示すBoolean型の値です。このサンプル・コードはプライマリ・サービスを指定しています。引数primaryをNOにすると、セカンダリ・サービスの指定になります。

このプライマリ・サービスとセカンダリ・サービスは、ペリフェラル・マネージャー内部のサービス定義で使われるものです。リモートのセントラルから見えるのは、プライマリ・サービスだけで、セカンダリ・サービスは見えません。セカンダリ・サービスは、補助的なキャラクタリスティクスをペリフェラル・マネージャー内部でまとめるのに用います。

サービスへのキャラクタリスティクスの追加は、CBMutableServiceのインスタンスのcharacteristicsプロパティに、CBMutableCharacteristcの配列を設定しておこないます。

===== キャラクタリスティクスのコンフィグレーション

リモートのセントラルから見える、ローカルのキャラクタリスティクスの振る舞いは、パーミションの設定で決まります。

- セントラルにキャラクタリスティクスをサブスクライブさせるのを許可する
- ペアリングしていないセントラルからのアクセスを制限、センシティブなもの

もしもローカルのキャラクタリスティクスの値が、しばしば変更されるものならば、リモートのセントラルにサブスクライブさせることを推奨します。そのために、キャラクタリスティクスにサブスクライブを許可するパーミション設定が必要です。これは、CBCharacteristicのpropertyプロパティに、CBCharacteristicPropertyNotifyを設定します:

[source, swift]
----
myCharacteristic = [[CBMutableCharacteristic alloc]
    initWithType:myCharacteristicUUID
    properties:CBCharacteristicPropertyRead | CBCharacteristicPropertyNotify
    value:nil permissions:CBAttributePermissionsReadable];
----

このサンプルコードはCBCharacteristicのインスタンス生成時に指定しています。<!--TBD  あとでプロパティ設定でもOK?  -->

==== センシティブなデータの扱い

<!-- これ、章を1つ割り当てるべき -->
BLEデバイスに個人情報を記録する場合があります。例えばソーシャルメディアのプロファイルを提供するBluetooth LEペリフェラルであれば、メンバーのアカウントや名前、住所といった個人情報を保持するでしょう。そのようなデバイスは、誰もが読み書きできる状態では、困ります。信頼したデバイスのみに読み書きを許可する必要があります。

キャラクタリスティクスのプロパティには、読み書きおよびノーティフィケーションの暗号化を指定できます。<!--TBD  下のコード、プロパティとパーミションが2つ分かれているけど、個々の区別、解説すべし  -->

[source, swift]
----
emailCharacteristic = [[CBMutableCharacteristic alloc]
    initWithType:emailCharacteristicUUID
    properties:CBCharacteristicPropertyRead
    | CBCharacteristicPropertyNotifyEncryptionRequired
    value:nil permissions:CBAttributePermissionsReadEncryptionRequired];
----

このサンプル・コードでは、キャラクタリスティクスは信頼するデバイスからのみ、読み出しおよびサブスクライブを受け付けます。リモートのセントラルがこのキャラクタリスティクスにアクセスすると、ローカルのペリフェラル・マネージャーはリモートのセントラルに、必要な権限がないというエラーを返します。そこで、リモートのセントラル・マネージャーは、次に、暗号化に必要な鍵を交換するため、ペリフェラルにペリフェラルを求めます。ローカルのiOSデバイスは、リモートのデバイスがペアリングを要求していることを、ユーザにダイアログ表示します。ユーザが、それを承認することで、ペアリングとボンディングが完了し、リモートのセントラルは、キャラクタリスティクスを読み出せるようになります。

セントラル・ロールとペリフェラル・ロールが両方共iOSデバイスであれば、セントラルがセキュアなキャラクタリスティクスにアクセスした時点で、それぞれのデバイスに、他のデバイスとのペアリングが必要であるダイアログを表示します。セントラル・ロールのデバイスには、ペリフェラルに入力すべきテキスト・コードが表示されます。ペリフェラル・ロールのデバイスに、そのテキスト・コードを入力してペアリングおよびボンディングが完了すれば、ペリフェラルはそのセントラルを信頼できるデバイスとみなします。

===== ペリフェラル・マネージャーへのサービス追加

キャラクタリスティクスを設定したCBMutableCharacteristcクラスのインスタンスができれば、次はペリフェラル・マネージャーのデータベースに、それらのサービスを追加します。これには、CBPeripheralManagerクラスのaddService:メソッドを使います。

[source, swift]
----
    [myPeripheralManager addService:myService];
----

ペリフェラル・マネージャーにaddService:メソッドでサービスとキャラクタリスティクスを追加すると、その内容はペリフェラル・マネージャーにキャッシュされて、変更することはできません。複数のサービスが複数ある場合はaddService:メソッドを繰り返し呼び出します。

ペリフェラル・マネージャーのデリゲートがperipheralManager:didAddService:error:メソッドを実装していれば、ペリフェラル・マネージャーはサービスの追加処理が完了するつど、このメソッドを呼び出します。もしもサービスの追加に失敗した場合は、このメソッドから原因を取得できます。

[source, swift]
----
- (void)peripheralManager:(CBPeripheralManager *)peripheral
            didAddService:(CBService *)service
                    error:(NSError *)error {
    if (error) {
        NSLog(@"Error publishing service: %@", [error localizedDescription]);
} ...
----

===== アドバタイズメントの開始と停止

周囲にいるかもしれないセントラルにペリフェラルの存在を伝えるために、CBPeripheralManagerクラスのstartAdvertising:メソッドで、アドバタイズメントを開始します。

[source, swift]
----
[myPeripheralManager startAdvertising:@{ CBAdvertisementDataServiceUUIDsKey :
    @[myFirstService.UUID, mySecondService.UUID] }];
----

startAdvertising:メソッドの引数はオプションを指定したNSDictionaryのインスタンスです。このサンプル・コードでは、CBAdvertisementDataServiceUUIDsKey をキー値に、サービスのUUIDに対応するCBUUIDの配列をバリューにしています。

Bluetooth LEのアドバタイズメント・パケットのデータ・タイプは、フラグ、ローカル名、製造者特有のデータ、送信電力およびサービスUUIDの配列の5種類で、CBAdvertisementData.h に、それぞれに対応するキー値が定義されています。

startAdvertising:メソッドの引数の辞書に指定できる値は、 CBAdvertisementDataLocalNameKey と CBAdvertisementDataServiceUUIDsKey の2つです。CBAdvertisementDataLocalNameKey は、デバイスのローカル名を表し、NSStringのインスタンスをバリューに設定します。CBAdvertisementDataServiceUUIDsKey はサービスUUIDの配列を表し、CBUUIDの配列をバリューに設定します。


startAdvertising:メソッドを呼び出すと、ペリフェラル・マネージャーはデリゲートのperipheralManagerDidStartAdvertising:error:メソッドを呼び出します。もしもエラーがありアドバタイズメントが開始出きない場合は、引数errorで詳細情報が渡されます。

[source, swift]
----
- (void)peripheralManagerDidStartAdvertising:(CBPeripheralManager *)peripheral
                                       error:(NSError *)error {
    if (error) {
        NSLog(@"Error advertising: %@", [error localizedDescription]);
} ...
----

Core Bluetoothフレームワークは、下層の通信の詳細を隠すため、iOSアプリケーションから、アドバタイズメント・パケットの周期を設定することはできません。また、アドバタイズメントは、パケット・サイズに制約があるため、データ・サイズにも制約があることに注意が必要です。<!--TBD  制約条件?  -->。さらに、iOSアプリケーションがフォアグラウンドかバックグラウンドかにより、アドバタイズメント・パケットの内容が異なります。

iOSアプリケーションがフォアグラウンド状態ならば、アドバタイズメント・パケットの最大データサイズ28バイトを、任意のデータ・タイプの組み合わせで使えます。もしも28バイトを超える場合は、ローカル・ネームに対してのみ、スキャン・レスポンスの10バイトの領域を使うことが可能です。ここでのデータ・サイズは、データ・タイプごとに2バイトのヘッダ情報が付加された後のサイズであることに、注意が必要です。

アドバタイズメント・パケットの28オクテットの領域に収まりきらなかったサービスUUIDsは、スキャン・レスポンスの領域に収められます。したがって、それらの収まりきらかなったサービスUUIDsは、iOSがアクティブ・スキャンをしたときに初めて発見されます。

セントラル・マネージャーのスキャン動作で述べたように、セントラルがペリフェラルを発見した時に、そのペリフェラルが接続対象かの判断は、ローカル・ネームもしくはサービスUUIDsで判断します。また、128ビットのUUIDは、16バイトあるので、アドバタイズメント・パケットに1つしか格納できません。ペリフェラルが意図したように発見されない場合は、startAdvertising:メソッドに指定したデータが、実際にはどのようにアドバタイジング・パケットに格納されているかを、把握することが必要です。

iOSアプリケーションがバックグラウンド状態ならば、アドバタイズメント・パケットにローカル・ネームは含まれません。また、全てのサービスUUIDsは、全てスキャン・レスポンスからのみ取得できます。

<!--TBD  停止ってどうするねん、接続検出がないけど  -->

===== 読み出しリクエストへの応答

<!--TBD  接続検出イベントがないけど、どうするんだろう  -->
セントラルがペリフェラルのキャラクタリスティクスに読み書きリクエストを送ると、それが固定値でないならば、ペリフェラル・マネージャーはデリゲートのperipheralManager:didReceiveReadRequest:メソッドを呼び出します。キャラクタリスティクスのレスポンス処理は、iOSアプリケーションがすべき処理です。

値が固定値ではないキャラクタリスティクスは、CBMutableCharacteristicクラスのinitWithType:properties:value:permissions:メソッドで、valueにnilを設定したものです。

[source, swift]
----
- (void)peripheralManager:(CBPeripheralManager *)peripheral
    didReceiveReadRequest:(CBATTRequest *)request {
    if ([request.characteristic.UUID isEqual:myCharacteristic.UUID]) {
        ...
----

peripheralManager:didReceiveReadRequest:メソッドは2つの引数をとります。最初の引数peripheralはペリフェラル・マネージャーを示します。2つ目の引数requestはセントラルからのリクエストを表すCBATTRequestクラスのインスタンスです。

CBATTRequestクラスは、プロパティのみでメソッドを持たないバリュー・オブジェクトです。プロパティは次の4つです。value以外、読み込みのみです。

- CBCentral \*central
    - リクエストを発生させたセントラルです。
- CBCharacteristic \*characteristic;
    - 対象となるキャラクタリスティクスです。
- NSUInteger offset;
    - データのオフセットを表します。
- NSData \*value;
    - 値です

CBCentralクラスは、1つだけのプロパティ CFUUIDRef UUID のプロパティを持ちます。CFUUIDRefは128ビットのUUIDを扱う構造体のポインタです。CBUUIDクラスの + (CBUUID \*)UUIDWithCFUUID:(CFUUIDRef)theUUID; クラスメソッドでCBUUIDのインスタンスに変換できます。このプロパティはセントラルの識別に使います。

リード・リクエストを受け取ったデリゲートは、セントラルが要求したデータを、CBATTRequestクラスのvalueプロパティに設定して返します。このために、まず対象となるキャラクタリスティクスが何かを調べます。これは、引数requestのcharacteristicプロパティのUUIDで判別できます。次のステップは、リード・リクエストのオフセット位置が、キャラクタリスティクスの値の範囲を超えていないことを確認します。

[source, swift]
----
    if (request.offset > myCharacteristic.value.length) {
        [myPeripheralManager respondToRequest:request
            withResult:CBATTErrorInvalidOffset];
        return;
    }
----

このオフセット値は、キャラクタリスティクスのデータが長くて、アトリビュート・プロトコルの1つのPDUに収まらない時に、複数のトランザクションに分割して読みだすときに使います。オフセット値が正しいと確認したのち、CBATTRequestのvalueプロパティに値を設定して、ペリフェラル・マネージャーのrespondToRequest:withResult:メソッドを呼び出します。Bluetooth LEの、リクエスト-レスポンスは1対1に対応しています。ですから、peripheralManager:didReceiveReadRequest:メソッドが呼び出されたら、対応するrespondToRequest:withResult:メソッドを、かならず1度だけ呼び出すようにします。

[source, swift]
----
    [myPeripheralManager respondToRequest:request withResult:CBATTErrorSuccess];
    ...
----

respondToRequest:withResult:メソッドは引数を2つ取ります。最初の引数requestは、デリゲートの呼び出しで渡されたCBATTRequestのインスタンスのvalueプロパティを設定したものです。2つ目の引数は、CBATTError列挙型の変数です。

もしも、ペリフェラルが公開しているキャラクタリスティクスにないキャラクタリスティクスを、リモートのセントラルが要求してきたなど、正常に処理ができない場合は、respondToRequest:withResult:にエラーコードを指定します。
<!--
valueはnil？
17個の値が定義されていますが、そのほとんどはアトリビュート・プロトコルで処理されるエラーです。アトリビュート・プロトコルの上層であるiOSアプリケーションが返すべき値は、正しく処理ができたことを示す CBATTErrorSuccess ( = 0)か、オフセット値が不正であることを示す CBATTErrorInvalidOffset の2つのうちいずれかです。
    CBATTErrorInvalidHandle                         = 0x01,
    CBATTErrorReadNotPermitted                      = 0x02,
    CBATTErrorWriteNotPermitted                     = 0x03,
    CBATTErrorInvalidPdu                            = 0x04,
    CBATTErrorInsufficientAuthentication            = 0x05,
    CBATTErrorRequestNotSupported                   = 0x06,
    CBATTErrorInvalidOffset                         = 0x07,
    CBATTErrorInsufficientAuthorization             = 0x08,
    CBATTErrorPrepareQueueFull                      = 0x09,
    CBATTErrorAttributeNotFound                     = 0x0A,
    CBATTErrorAttributeNotLong                      = 0x0B,
    CBATTErrorInsufficientEncryptionKeySize         = 0x0C,
    CBATTErrorInvalidAttributeValueLength           = 0x0D,
    CBATTErrorUnlikelyError                         = 0x0E,
    CBATTErrorInsufficientEncryption                = 0x0F,
    CBATTErrorUnsupportedGroupType                  = 0x10,
    CBATTErrorInsufficientResources                 = 0x11
-->

===== 書き込みリクエストへの応答

リモートのセントラルからローカルのペリフェラルへの書き込みの処理は、読み込みと同じです。セントラルが送信する、1つかそれ以上のキャラクタリスティクスへの書き込みリクエスを、ペリフェラル・マネージャーが受信すると、デリゲートのperipheralManager:didReceiveWriteRequests:メソッドを呼び出します。

peripheralManager:didReceiveWriteRequests:メソッドの引数は2つです。最初の引数は、ペリフェラル・マネージャーのインスタンスです。2つ目の引数はCBATTRequestのインスタンスの配列です。この引数のCBATTRequestのインスタンス1つ1つが、個別の書き込みリクエストです。

もしも2つ上の書き込みリクエストがきたときは、デリゲートは、それらのリクエストを1つのまとまりとして、アトミックに処理をします。一連の書き込みリクエストのうち1つが失敗したならば、一連の書き込み処理は実行されるべきではありません。

[source, swift]
----
    myCharacteristic.value = request.value;
----

書き込みリクエストの内容が正しいならば、対応するキャラクタリスティクスに値を書き込みます。このサンプル・コードにはありませんが、読み込み時と同じく、書き込みを実行する前に、オフセット値がキャラクタリスティクスの値の範囲を正しく示しているかを確認します。

読み込み時と同じく、リクエストとレスポンスは1対1に対応します。デリゲートのperipheralManager:didReceiveWriteRequests:メソッドが呼ばれたら、そのレスポンスとなるrespondToRequest:withResult:メソッドを、1回だけ呼び出します。

respondToRequest:withResult:メソッドの最初の引数は、respondToRequest:withResult:メソッドで渡されたCBATTRequestのインスタンスです。2つ目の引数はCBATTError列挙型の変数です。<!-- ここ複数の返し方、ちょっとわからない -->

[source, swift]
----
 [myPeripheralManager respondToRequest:[requests objectAtIndex:0]
    withResult:CBATTErrorSuccess];
----

===== セントラルへの通知

リモートのセントラルは、ローカルのペリフェラル・マネージャーの1つあるいはそれ以上のキャラクタリスティクスをサブスクライブできます。サブスクライブされたキャラクタリスティクスの値が変更されると、ペリフェラル・マネージャーは、その更新値をリモートのセントラルに通知します。

接続しているセントラルがキャラクタリスティクスをサブスクライブを要求した時、ペリフェラル・マネージャーはデリゲートの peripheralManager:central:didSubscribeToCharacteristic: メソッドを呼び出します。

[source, swift]
----
    - (void)peripheralManager:(CBPeripheralManager *)peripheral
                      central:(CBCentral *)central
    didSubscribeToCharacteristic:(CBCharacteristic *)characteristic {
        NSLog(@"Central subscribed to characteristic %@", characteristic);
        ...
----

peripheralManager:central:didSubscribeToCharacteristic:メソッドは3つの引数、ペリフェラル・マネージャー、セントラル、そしてサブスクライブされるキャラクタリスティクスのインスタンスを取ります。iOSアプリケーションが、このキャラクタリスティクスがサブスクライブされていることを、覚えておきます。


iOSアプリケーションは、サブスクライブされているキャラクタリスティクスの値を更新するときに、ペリフェラル・マネージャーの updateValue:forCharacteristic:onSubscribedCentrals: メソッドを呼び出します。

[source, swift]
----
    NSData *updatedValue = // fetch the characteristic's new value
    BOOL didSendValue = [myPeripheralManager updateValue:updatedValue
        forCharacteristic:characteristic onSubscribedCentrals:nil];
----

updateValue:forCharacteristic:onSubscribedCentrals:メソッドは3つの引数をとります。最初の引数は、更新された値を示すNSDataのインスタンス、2つ目の引数はキャラクタリスティクス、最後の引数は通知を送るCBCentralオブジェクトの配列です。

updateValue:forCharacteristic:onSubscribedCentrals:メソッドの最後のCBCentralの配列に、nilを指定すると、そのキャラクタリスティクスをサブスクライブする全てのセントラルに通知が送信されます。引数にCBCentralの配列を与えれば、その配列のセントラルのうち、そのキャラクタリスティクスをサブスクライブしているものに、通知が送信されます。配列の中に、そのキャラクタリスティクスをサブスクライブしていないセントラルがあっても、そのセントラルに通知が送信されることはありません。通知を送るセントラルの範囲を限定したい場合に使います。

updateValue:forCharacteristic:onSubscribedCentrals:メソッドの戻り値はBoolean型です。アップデートが送信できるならばYESが、送信キューが満杯で通知ができないならばNOが返されます。もしも戻り値がNOであれば、送信キューに空きができたときに、ペリフェラル・マネージャーはデリゲートのeripheralManagerIsReadyToUpdateSubscribers:メソッドを呼び出します。もしも望むならば、このメソッドが呼び出されたときに、再度、通知処理を行います。

この通知で送られるキャラクタリスティクスのデータ・サイズは、セントラル・マネージャーの場合と同じく、アトリビュート・プロトコルのATT\_MTUで制約されます。もしもキャラクタリスティクスの値が、サイズ制約を超える場合は、それぞれのセントラルが、readValueForCharacteristic:メソッドを使ってオフセット値を指定して、そのキャラクタリスティクスのすべての値を読み出すべきです。

== バックグラウンド処理

iOSアプリケーションはバックグラウンド実行に対応できます。しかし、iOSアプリケーションでは、電力やメモリなどのリソースの消費量が問題になります。そのため、iOSアプリケーションがフォアグラウンドかバックグラウンドにあるかで、無線を使うCore Bluetoothフレームワークの振る舞いが異なります。iOSアプリケーションのマルチタスキングの詳細は、[iOSアプリケーション開発ガイド](https://developer.apple.com/jp/devcenter/ios/library/japanese.html)に掲載された[iOSアプリケーション プログラミングガイド](https://developer.apple.com/jp/devcenter/ios/library/documentation/iPhoneAppProgrammingGuide.pdf)の、アプリケーションの状態とマルチタスキング、の章を参照してください。

通常のiOSアプリケーションは、セントラルおよびペリフェラルいずれも、バックグラウンドでは動作が不可にされます。iOSアプリケーションのバックグラウンド・モードを設定してあれば、Bluetooth LE関連のイベントが発生すれば、対応するデリゲートのメソッドが呼び出されます。イベントの処理は、iOSアプリケーションが全ての処理を行う場合と、ユーザに通知表示を出すか否かだけをiOSに伝えて、あとの実際の通知処理はiOSに任せる場合の、2通りがあります。

=== フォアグラウンド状態のみiOSアプリケーション

バックグラウンド・モードをなにも指定しない通常のiOSアプリケーションは、バックグラウンド状態に入ると、短い実行時間ののちにサスペンド状態に移行します。サスペンド状態ではBluetooth関連のイベントは通知されず、システムがキューに貯めています。iOSアプリケーションがフォアグラウンド状態に戻った時に、キューにあるイベントが通知されます。ですから、もしもリモートとの接続が切断しても、それが通知されるのは、iOSアプリケーションがフォアグラウンドに戻った時です。<!--TBD  ここの振る舞い?  -->

バックグラウンド・モードを指定しないセントラル・ロールのiOSアプリケーションは、フォアグラウンドでスキャンをしていても、バックグラウンドに入ると、スキャンは停止します。ペリフェラルの発見および切断のイベントも通知されません。

バックグラウンド・モードを指定しないペリフェラル・ロールのiOSアプリケーションは、バックグラウンド状態ではアドバタイズメントが停止されます。すでに接続が確立しているリモートのセントラルとの接続は、バックグラウンド状態になっても切れません<!--TBD  接続は切れるの?-->。しかし、リモートのセントラルがダイナミックなキャラクタリスティクスの値にアクセスすると、エラーが返されます。<!--TBD どんなエラ?  -->

=== ユーザへのアラート通知

iOSアプリケーションが、バックグラウンド・モードを設定していなくて、バックグラウンド状態でBluetoothのイベントをアプリが処理できない場合でも、設定でiOSにBluetoothのイベントをユーザに通知させることはできます。

ユーザに通知できるのは、ローカルのiOSアプリケーションがセントラル・ロールのとき、リモートのペリフェラルとの接続が確立したこと、切断したこと、およびペリフェラルからのノーティフィケーションを受信したこと、の3つです。これらをiOSに通知させるかは、ペリフェラルに接続するときに呼び出す、セントラル・マネージャーのconnectPeripheral:options:メソッドのオプションで指定します。

connectPeripheral:options:メソッドのオプションには、NSDictionaryのインスタンスを指定します。辞書に使えるキーは以下の3つです。バリューはBoolean型変数をおさめたNSNumberのインスタンスです。キーを指定しなければ、NOを指定したのと同じで、ユーザ通知をしません。3つのうち2つのキーは、iOS6以降で利用できます。iOS5では、指定しても無視されます。

- CBConnectPeripheralOptionNotifyOnConnectionKey (iOS6以降)
- CBConnectPeripheralOptionNotifyOnDisconnectionKey
- CBConnectPeripheralOptionNotifyOnNotificationKey (iOS6以降)

CBConnectPeripheralOptionNotifyOnConnectionKey はiOS6以降で有効です。バリューがYESのとき、iOSアプリケーションがサスペンドしている時にペリフェラルとの接続が確立すれば、iOSがユーザにアラート表示を行います。<!-- TBD 確認 あるペリフェラルに対して、2つ以上のアプリケーションがこの通知を要求していたならば、フォアグラウンドで最も最近に要求をしたアプリケーションが、通知を受け取ります。-->

CBConnectPeripheralOptionNotifyOnDisconnectionKey のバリューがYESのとき、iOSアプリケーションがサスペンドしている時にペリフェラルとの接続が切断すれば、iOSがユーザにアラート表示を行います。<!-- あるペリフェラルに対して、2つ以上のアプリケーションがこの通知を要求していたならば、フォアグラウンドで最も最近に要求をしたアプリケーションが、通知を受け取ります。-->

CBConnectPeripheralOptionNotifyOnNotificationKey はiOS6以降で有効です。iOSアプリケーションがサスペンドしている時に、ペリフェラルからインディケーションがきたら、iOSはユーザにその内容を通知します。<!-- インディケーションはどうなんだろう、表示はどう出るんだろう --> <!-- あるペリフェラルに対して、2つ以上のアプリケーションがこの通知を要求していたならば、フォアグラウンドで最も最近に要求をしたアプリケーションが、通知を受け取ります。 -->

=== バックグラウンド・モード

バックグラウンドでも、ペリフェラルからの通知や送られてくるセンサ・データの保存など、何かしらの処理が必要ならば、iOSアプリケーションのバックグラウンド・モードを指定します。バックグラウンド・モードは、iOSアプリケーションが任意の時間に任意の処理を行うためのものではなく、外部からのイベントを処理するためのものです。バックグラウンド・モードが指定されたiOSアプリケーションは、Bluetooth LEのイベントが発生するたびに、10秒間の実行時間を与えられて、サスペンド状態から実行状態になります。<!--TBD  タイマー? -->

バックグラウンド・モードは、iOSアプリケーションのInformation property list (Info.plist) ファイルにモードを宣言します。UIBackgroundModes keyに、セントラル・ロールならばbluetooth-centralを、ペリフェラル・ロールならばbluetooth-peripheralを設定します。もしもiOSアプリケーションが、セントラルとペリフェラルの両方の役割を持つならば、2つのバックグラウンド・モードを指定します。

Xcodeで
Info.plistファイルを開くと、プロパティ・リスト・エディタは生のキー名ではなく、人間がわかりやすい表記で表示します。生のキー名を表示したい時は、エディタ・ウィンドウのどこでもいいので、コントロール・キーを押しながらクリックをします。コンテキスト・メニューが表示されるので、その中の"Show Raw Keys/Values item"をチェックします。

==== セントラル・ロールでのバックグラウンド実行

iOSアプリケーションのInfo.plistファイルに、UIBackgroundModesキーにbluetooth-centralを設定すれば、セントラル・ロールのiOSアプリケーションはバックグラウンドで実行されます。バックグラウンド状態でペリフェラルのスキャンができます。またiOSは、ペリフェラルの発見と接続完了、読み書きと切断のイベントで、バックグラウンドでも、iOSアプリケーションをスタンドバイ状態から起こして、CBCentralManagerDelegateプロトコルの該当するメソッドを呼び出します。

ただし、iOSアプリケーションがフォアグラウンド状態とバックグラウンド状態のときでは、電力消費量を抑えるために、Bluetooth LEの振る舞いが異なります。Bluetooth LEの通信接続自体はフォアグラウンド状態の時と同じです。

バックグラウンド状態では、重複するペリフェラルのアドバタイジングを都度知らせるスキャン・オプションCBCentralManagerScanOptionAllowDuplicatesKeyが無視され、ペリフェラルの発見は1度だけ通知されます。また、定常的に電力を消費するスキャンは、ペリフェラルをスキャンする全てのiOSアプリケーションがバックグラウンド状態であれば、12分に1回程度と、低頻度になります。このため、バックグラウンド状態ではペリフェラルの発見に、時間がかかります。フォアグラウンド状態のiOSアプリケーションがスキャンをしていれば、バックグラウンド状態のiOSアプリケーションにも、フォアグラウンド状態と同じスキャン周期が与えられます。<!--TBD  重複するパケットは無視? -->

==== ペリフェラル・ロールでのバックグラウンド実行

iOSアプリケーションのInfo.plistファイルに、UIBackgroundModesキーにbluetooth-peripheralを設定すれば、ペリフェラル・ロールのiOSアプリケーションはバックグラウンドで実行されます。バックグラウンド状態でアドバタイズメントができます。またiOSは、セントラルからの読み書きやサブスクライブのアクセスがあると、iOSアプリケーションをスタンドバイ状態から起こして、CBPeripheralManagerDelegateプロトコルの該当するメソッドを呼び出します。

バックグラウンド状態でのアドバタイズメントは、フォアグラウンド状態のときとは、振る舞いが異なります。iOSはアドバタイズメント・パケットの長さを可能な限り短くすることで、電波の送信時間を短くして、電池消費量を最小にしようとします。

アドバタイズメント・データに指定したCBAdvertisementDataLocalNameKeyキーが無視されて、ペリフェラルのローカル・ネームはアドバタイジングされません。CBAdvertisementDataServiceUUIDsKeyキーで指定したサービスUUIDsは、アドバタイズメント・パケットで送信されません。リモートのセントラルが、スキャン・リクエストすることで、サービスUUIDsが取得できます。また、アドバタイズメントしているiOSアプリケーションが全てバックグラウンド状態のときは、アドバタイジングの周期自体が下がります。<!--TBD  どの程度? -->フォアグラウンド状態のiOSアプリケーションがアドバタイジングしていれば、バックグラウンド状態のiOSアプリケーションにも、フォアグラウンド状態と同じアドバタイズメントの周期が与えられます。



==== バックグラウンド・モードのベストプラクティス

iOSアプリケーションは、電池消費量を特に気にします。個別の実装ごとに、こうなればこうなると考えていくのは大変です。バックグラウンド・モードの使いかたの指針をいくつか示します。

- Bluetoothの利用をユーザが選択できるインタフェース設計
- 目的に合わせたバックグラウンド・モードの利用
- バックグラウンドでの初利を最小の時間に抑える

iOSアプリケーションは、Bluetooth LEを利用する場面では、セッションベースのインタフェースを提供します。例えばスキャンがいつ開始して、いつ終了するかが、画面の表示遷移とひもづけられていれば、ユーザから見てなにをしているのかが明確になります。また、バックグラウンド・モードでデータ同期する/しないなど、Bluetooth LEを利用する機能を、ユーザ設定画面で提供することもよいでしょう。

Bluetooth LEのバックグラウンド・モードは、それを使う必要があるiOSアプリケーションが使うべきものです。Bluetooth LEを必要としないのに、バックグラウンドでのiOSアプリケーションの動作を得るためだけに、このバックグラウンド・モードを使うべきではありません。

また、Bluetooth LEのイベントごとに、iOSアプリケーションには10秒の実行時間が与えられます。この10秒をすべて使うのではなく、必要なタスク処理が完了すれば、iOSに処理を戻して、iOSアプリケーションの実行時間を最小にします。

== Core Bluetoothフレームワークのベスト・プラクティス

Core Bluetoothフレームワークは、iOSアプリケーションに、Bluetooth LEの通信の詳細を隠蔽して、抽象化された振る舞いを提供します。これによりiOSアプリケーション開発者は、Bluetooth LEの通信規格を理解しなくても、求める振る舞いを実装できます。

しかし、Bluetooth LEを利用する限り、必ずリモートのペリフェラルまたはセントラルが振る舞いに関わります。そして、ローカルとリモートのデバイスの間を電波がつなぐのです。高度に抽象化されても、なおiOSデバイス単体で完結する通常のiOSアプリケーションにはない配慮も必要です。ここでは、Core Bluetoothフレームワークを使うノウハウを述べます。

=== スキャンは必要最小限に

Bluetooth LEで電力を消費するのは、スキャンです。いつくるかタイミングが分からない、8桁も違う受信信号電力を検出するため、高周波回路が最も電力を消費する状態です。次に電力を消費するのが、アドバタイズメントです。無線通信は、超低消費電力無線通信の名称が示すとおり、高周波回路の消費電力は少量です。ただし、イベントを処理するために、iOSアプリケーションが動作すれば、その分の電力消費が生じます。したがって、スキャンを最小に、アドバタイズメントは利用時間をなるべく少なくするように、注意します。

リモートのペリフェラルの発見のために、セントラル・マネージャーのscanForPeripheralsWithServices:options:メソッドを呼び出せば、スキャンが開始されます。このスキャンはタイムアウトしません。ペリフェラルを発見したならば、セントラル・マネージャーのstopScanメソッドを呼び出して、iOSにスキャンの停止を指示します。

stopScanメソッドを呼び出しても、直ちにそのiOSデバイスのスキャンが停止するとは限りません。Bluetooth LEは複数のiOSアプリケーションで利用されます。そのiOSアプリケーションがstopScanメソッドを呼び出しても、他のiOSアプリケーションがスキャンを実行していれば、そのiOSデバイスのスキャンは実行され続けるでしょう。<!--TBD  発見をかけてなくても、発見される?  -->

iOSアプリケーションがバックグラウンド状態になったとき、バックグラウンド・モードが指定されていない場合は、スキャンは停止します。<!--TBD 物理的にスキャンが停止するの? また、フォアグラウンドになったら再開するの?  --> バックグラウンド・モードにbluetooth-centralを指定していれば、スキャンは継続して実行されますが、ペリフェラルを見つける周期はフォアグラウンド状態のときよりも長くなります。

=== 重複するアドバタイズメント・パケットの検出は最小に

セントラルに接続していないペリフェラルは、周囲のセントラルに存在を伝えるために、アドバタイズメント・パケットを送信し続けます。

セントラル・マネージャーのscanForPeripheralsWithServices:options:メソッドは、アドバタイズメント・パケットを受信してペリフェラルを発見した時に、デリゲートのcentralManager:didDiscoverPeripheral:advertisementData:RSSI:メソッドを呼び出します。また、同じペリフェラルでも、アドバタイズメント・パケットのデータが、以前に受信したデータと異なる場合にも、このデリゲートを呼び出します。

アドバタイズメント・パケットを受信した都度、デリゲートを呼び出させたいときは、scanForPeripheralsWithServices:options:メソッドのオプションで、CBCentralManagerScanOptionAllowDuplicatesKeyのバリューをYESに設定します。これは、受信信号強度をモニタし続けて、ペリフェラルが十分近くに来た時に初めて接続させたいときに、使ったりします。しかし、連続してスキャンをし続けるため、バッテリー消費量が大きくなります。必要な場合にのみ使用します。

=== サービスの検索を最小に

たいてい、ペリフェラルには数多くのサービスがあり、iOSアプリケーションが利用するのは、その一部分です。ペリフェラルにどのようなサービスおよびキャラクタリスティクスがあるかを検索すると、その検索データのやり取りのために、無線通信の時間と処理時間がかかります。

iOSアプリケーションの設計段階で、ユースケースに求められるペリフェラルのサービスとキャラクタリスティクスは決まります。ですから、検索は利用するものだけに限定することを勧めます。サービスを検索するCBPeripheralクラスのdiscoverServices:メソッドには、検索したいサービスのUUIDsを引数に指定できます。同様に、CBperipheralクラスのscanForPeripheralsWithServices:options:メソッドも、検索したいキャラクタリスティクスのUUIDsを引数に指定できます。

[source, swift]
----
    [peripheral discoverServices:@[firstServiceUUID, secondServiceUUID]];
----

=== サブスクライブと読み出しの使いかた

リモートのペリフェラルのキャラクタリスティクスの値が変化する場合、その値の読み出し方法には、ポーリングとサブスクライブの2つの方法があります。ポーリングは、ローカルのセントラルから一定期間でreadValueForCharacteristic:メソッドを呼び出して、明示的にキャラクタリスティクスの値を読み出す方法です。サブスクライブは、setNotifyValue:forCharacteristic:を1度呼び出しておけば、リモートのキャラクタリスティクスの値が変更される都度、リモートのペリフェラルからセントラルに通知される方法です。

キャラクタリスティクスの値の変化をモニタするには、サブスクライブを使います。ただし、そのキャラクタリスティクスのpropertyプロパティをみて、インディケーションまたはノーティフィケーションのすくなくとも1つをサポートしていなければ、サブスクライブはできません。

ただし、あまりに頻繁に値が変化するキャラクタリスティクスは、ポーリングを使ったほうが消費電力を抑えられるかもしれません。

=== ペリフェラルの切断

もうペリフェラルを使わないならば、接続を切断すれば無線をより使わなくなり、電力消費量を抑えられるでしょう。

キャラクタリスティクスがノーティフィケーションを停止した場合は、もう値の更新を通知してくることはありません。これは、キャラクタリスティクスのisNotifyingプロパティがNOになっていることで確認できます。サブスクライブしている全てのキャラクタリスティクスのノーティフィケーションが停止したならば、もうペリフェラルと接続しつづける必要がないので、切断します。

またペリフェラルから必要なすべてのデータを読みだした場合も、接続し続ける必要はないので、切断します。
<!--
この説、なにが言いたいんだろう。

どっちの場合でも、持ってるだろうサブスクライブををキャンセルしろ、
ペリフェラルから切断する。

キャラクタリスティクスの値のサブスクライブは、
setNotifyValue:forCharacteristic: methodの最初のパラメータをNO
ペリフェラルへの接続をキャンセルできる、cancelPeripheralConnection: method of the CBCentralManager class,

[myCentralManager cancelPeripheralConnection:peripheral];
-->

ペリフェラルの切断には、CBPeripheralクラスのcancelPeripheralConnection:メソッドを呼び出します。このメソッドは、処理をブロックすることなく直ちに返ってきます。しかし、ペリフェラルとの実際の切断処理は、iOSがキャッシュして、後に実行されます。このため、ペリフェラルとの切断がいつ完了するかは、iOSアプリケーションからはわかりません。<!-- TBD 切断していなくても、iOSアプリケーションとの切断は通知されるの? centralManager:didDisconnectPeripheral:error: method of its delegate object.
その他のアプリは、ペリフェラルに接続を持っているだおるから、
ローカルの接続切断が、下層の物理の切断になるかは、ただちには、わからない。
アプリからの視点では、ペリフェラルは切断されたと見える。
-->

=== ペリフェラルへの再接続

ペリフェラルに接続する方法は、今までに述べたペリフェラルを発見して接続する以外に、過去に一度接続したことがあるペリフェラル、また現在iOSデバイスに接続しているペリフェラルに接続する方法があります。

ペリフェラルに接続した時、iOSはそのペリフェラルをそれぞれ識別するUUIDを生成します。このUUIDを保存しておくと、あとでペリフェラルに再接続したいときに、セントラル・マネージャーのretrievePeripherals:メソッドを呼び出す時のペリフェラルの指定に使えます。

[source, swift]
----
    [myCentralManager retrievePeripherals:savedPeripheralUUIDs];
----

iOSデバイスが過去に接続したことがあるペリフェラルであれば、セントラル・マネージャーのretrievePeripherals:メソッドに、問い合わせたいペリフェラルのUUIDの配列を与えて呼び出します。UUIDはCFUUIDRefのインスタンスで表します。CBUUIDはBluetooth LEのサービスやキャラクタリスティクスの識別に使います。CBUUIDクラス自体がUUIDを表すのですが、これはBluetoothのために作られたクラスなので、ペリフェラルの識別のUUIDには、Bluetoothに関係がないCFUUIDRefが使われます。

セントラル・マネージャーは、指定されたUUIDの配列の中から、過去に接続したことのあるペリフェラルのUUIDを見つけます。CBCentralManagerDelegateプロトコルのcentralManager:didRetrievePeripherals:メソッドで、発見したペリフェラルに対応するCBPeripheralのインスタンスの配列を返します。もしも該当するペリフェラルが1つもなかった場合は、空の配列を返します。

取得したペリフェラルの配列のなかから、画面に表示してユーザが選択する、あるいはiOSアプリケーション内部で判定して、接続したいペリフェラルを選択します。ペリフェラルへの接続には、スキャンの後に接続した時と同じく、セントラル・マネージャーのconnectPeripheral:options:メソッドを呼び出します。このとき、そのペリフェラルがiOSデバイスとすでに接続していれば、セントラル・マネージャーはデリゲートのcentralManager:didConnectPeripheral:メソッドを呼び出します。<!--接続していなかったら?-->

centralManager:didConnectPeripheral:メソッドを呼び出しても、ペリフェラルが周囲にいなければ、接続はできません。また、Bluetooth LEデバイスには、プライバシーを保護するために、デバイスの特定を防ぐ、ランダム・アドレスという仕組みがあります。これは、リンク層のアドレスを一定周期で変更します。ランダム・アドレスを使うペリフェラルは、たとえ周囲にあったとしても、前回接続した時とアドレスが異なるので、再接続ができません。この場合は、再度スキャンをしてペリフェラルの発見から、接続をやり直します。

=== 接続しているペリフェラルのリストを取得する

ペリフェラルに再接続する残る1つの方法は、セントラル・マネージャーのretrieveConnectedPeripheralsメソッドを呼び出して、現在iOSデバイスに接続しているペリフェラルを取得することです。

retrieveConnectedPeripheralsメソッドを呼び出すと、
セントラル・マネージャーは、いまiOSデバイスに接続している全てのペリフェラルを、デリゲートのcentralManager:didRetrieveConnectedPeripherals:メソッドで通知します。

centralManager:didRetrieveConnectedPeripherals:メソッドの2つ目の引数はCBPeripheralのインスタンスの配列です。もしもiOSデバイスに接続しているペリフェラルがない時は、空の配列が渡されます。<!--アプリごとに同時アクセスってできるよな-->

iOSデバイスとペリフェラルとのBluetooth LEの接続が確立しているときでも、iOSアプリケーションとペリフェラルとの接続は、ペリフェラルを発見した時と同じ手順です。セントラル・マネージャーのconnectPeripheral:options:メソッドを呼び出して接続を行います。iOSアプリケーションとの接続が確立すれば、セントラル・マネージャーはデリゲートのcentralManager:didConnectPeripheral:メソッドを呼び出します。
<!-- 結果はデリゲートで帰ってくる。接続処理について p.41の図 上のリストにあるその順番で、再接続のワークフロー。-->
<!-- 接続オプションの再指定、指定しなければデフォのまま? それとも、前の設定が残る? -->

=== アドバタイジング・データの考慮

iOSアプリケーションがペリフェラル・ロールのとき、そのアドバタイジング・データは、ペリフェラル・マネージャーのstartAdvertising:メソッドに渡す辞書で指定できます。しかし、アドバタイズメント・パケットのペイロードにはサイズ制約があります。またバックグラウンド状態では、iOSがパケット・サイズを最小にしようとします。このため、指定したアドバタイズメント・データと、実際に送信されるアドバタイジング・データの対応を知っておくことが大切になります。

アドバタイズメント・パケットは、ペリフェラルに関する情報をブロードキャストするのに、使います。そのアドバタイジング・データは、ペリフェラルのローカル名およびサービスのUUIDsを入れられます。指定できるキーは:

- CBAdvertisementDataLocalNameKey
- CBAdvertisementDataServiceUUIDsKey.

これ以外のキーを指定すると、エラーとなります。<!--TBD  本当か? 確認  -->

アドバタイジング・データは、iOSアプリケーションがフォアグラウンド状態のとき、28バイトまでの制限があります。これはBluetooth LEの、パケットの最大サイズによる制約です。この28バイトは、キーごとに必要な2バイトのヘッダと、そのバイト・データの合計値が、それ以下になることを意味します。<!--TBD  キー値2つ指定したら、個別ヘッダになるのか?  -->

もしも指定したデータが28バイトを超える場合は、アクティブ・スキャンでペリフェラルが返すパケットの空き領域10バイトが使われます。ただし、この領域を使えるのは、ローカルネームだけです。<!--TBD  ややこしいから、実際の組み合わせで具体的な解説を入れること  --> そのため、アクティブ・スキャンをしない限りローカルネームを見つけられません。

iOSアプリケーションがバックグラウンド状態にあると、全てのサービスUUIDsは、オーバフロー領域に置かれます。またローカルネームはアドバタイジングされません。<!--TBD  これって、アクティブ・スキャンでも取れないってことか?  -->

これらの制約から、アドバタイジングするサービスUUIDは、プライマリ・サービスを特定するものだけにしておくことを勧めます。

- ユーザにアドバタイズメントするか決めさせる





== Apple Notification Center Service イントロダクション

== iOS6からiOS7へ
Appleは、Bluetooth LEにいち早く対応し、またその利用領域を毎年拡大しています。

2011年に世界で初めてBluetooth low energyに対応したiPhone4sと同時に公開されたiOS5は、CoreBluetoothフレームワークで一般アプリ開発者に、Bluetooth LE周辺機器との接続を開放しました。翌年のiOS6は、セントラル・ロールに加えて、ペリフェラル・ロールが追加されました。

iOS5とiOS6は、個別のアプリケーションとBluetooth LE機器との接続だけでしたが、iOS7からは、iOS自体がBluetooth LE機器およびBluetooth LEを利用したサービスに対応しはじめました。

iOS7から提供されたiOSが担当するBluetooth LEの機能は:

- Apple Notification Center Service (ANCS)
- iBeacon
- Gameコントローラ (MFi)
- HID over BLE(キーボードなどの入力装置)

です。今回はこの内ANCSについて解説します。

== Apple Notification Center Serviceの目的
The Apple Notification Center Service (ANCS)の目的は、Bluetooth LEでiOSデバイスに接続するBluetoothアクセサリーに、iOSが出力する様々な通知(メールやFacebookなどの新着、電話着信など)にアクセスする勘弁な方法を提供することです。

ANCSの設計原則は、Simplicity, Efficienty そして Scalabilityの3つです。LEDが光るだけの簡単なアクセサリから、液晶表示器がある強力な周辺機器まで、様々な周辺デバイスにサービスを提供します。

ANCSは、Bluetooth LEの規格にあるGATTを使います。特定の、閉じた技術は使っていません。GATTクライアントとして振る舞う周辺機器は、iOSデバイスが提供するANCSを通して、通知サービスの発見と接続そしてアクセスができます。

=== 用語など
明記しな限り、GATTのパケットの数値はリトル・エンディアンで表現します。文字列の文字符号化方式は、UnicodeおよびUTF-8です。

- The Apple Notification Center Service (ANCS)
- Notification Provider (NP): ANCSサービスを公開するもの(iOSデバイス)
- Notification Consumer (NC): のクライアント (アクセサリ)
- iOS notification : iOS ノーティフィケーション・センターがiOSデバイスに表示するノーティフィケーション
- GATT notification : GATT キャラクタリスティクスで送られる、ノーティフィケーション、非同期のメッセージとして

== ANCSでできること、使いかた

== 公開されるサービスとプロファイル
== サンプルアプリ


サービスUUIDは、7905F431-B5CE-4E99-A40F-4B1E122D00D0。

ANCSのインスタンスは、NPに1つだけ。ANCSが常にあるとは限らない。GATTサービスの、Service Changed characteristicを探してサブスクライブすべし、NCは。いつ何時、公開/非公開になるかわからないから、それらをモニタするために。

== キャラクタリスティクス
ANCSが公開するキャラクタリスティクス:


- Notification Source: UUID 9FBF120D-6301-42D9-8C58-25E699A21DBD (notifiable)
- Control Point: UUID 69D1D8F3-45E1-49A8-9821-9BBDFDAAD9D9 (writeable with response)
- Data Source: UUID 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB (notifiable)

Notification Source characteristicのサポートは必須。
the Control Point characteristic and Data Source characteristic はオプション。

この3つ以外のキャラクタリスティクスがANCSにあるかもしれない。いわば、NCは認識できないキャラクタリスティクスは無視していい。

=== Notification Source (notifiable)
The Notification Source characteristic
NCに情報を伝えるキャラクタリスティクス。

- NPの新しいiOSのノーティフィケーション到着
- NPのiOSノーティフィケーションの変更
- NPのiOSノーティフィケーションの削除



the Notification Source characteristicをNCがサブスクライブすれば、直ちに
GATT notifications が。
結果として、NCはアクセプトと、処理が出来る状態にないと。それらのメッセージを、サブスクライブする前に。

The format of GATT notifications delivered through the Notification Source characteristic is shown in Figure 2-1.
https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Art/formatOfGATTNotifications_2x.png

the Notification Source characteristicを通じて配布される A GATT notification
は次の情報を含んでいる。

EventID

与えられたiOSノーティフィケーションが、追加、変更または削除のいずれなのかを、アクセサリに通知するフィールド。
このフィールドの値の列挙は、Event ID Valuesに定義される。


EventFlags
iOSノーティフィケーションの特殊性、
NCにそれを伝えるビットマスクの集合。
例えば、iOSノーティフィケーションが、"重要"だと思われたならば、そのNCはユーザが適切に警告を受け取れるように、もっと積極的なUI表示をしたいだろう。このビットフィールドの列挙値はEventFlagsに定義している。

CategoryID
iOSノーティフィケーションが属するカテゴリを表す数値。NPはそれぞれのiOS通知ごとに性格なカテゴリを提供しようとするだろう。
このフィールドの列挙値“CategoryID Values"に定義されている。

CategoryCount
与えられたカテゴリのなかでの、有効なiOSノーティフィケーションの現在の数。
例えば、2つの未読メールがユーザのメールボックスにあれば、そして新しいメールがユーザのiOSデバイスにプッシュされたならば、
このCategoryCountは3になる。

NotificationUID
32ビットの、iOS通知ごとにユニークなID(Unique identifier, UID)。この値はControl Point characteristicに送るコマンドのハンドラとして使う。iOS通知の情報をもっと引き出すときの。
iOSノーティフィケーションの寿命は、NPにより生成された一連のNotifcation Source GATTノーティフィケーションから、明示的に推測できる。

https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Art/lifetimeOfAnIOSNotification_2x.png

== Control Point and Data Source
NCはiOSノーティフィケーションについてのさらなる情報を求めるだろう、その内容を含めて。それらの属性の取得は、Control Point and Data Source characteristics を通して実行できる。

https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Art/formatOfAGetNotificationAttributesCommand_2x.png

A Get Notification Attributes コマンドは次の情報を含む:

CommandID
0 (CommandIDGetNotificationAttributes) に設定される。

NotificationUID
クライアントが情報を取得したい、iOSノーティフィケーションのUIDを示す32ビットの数値。

AttributeIDs
取得したい属性の配列。アトリビュートによっては、NCが取得したいアトリビュートの最大バイト長を指定する16ビット長のパラメータが続くものもある。

NCはControl Point characteristicに特定のコマンドを書き込むことで、iOSノーティフィケーションのさらなる情報の取得のリクエストを発行できる。
Control Point characteristicへの書き込みが成功すれば、NPは即時に
Data Source characteristicのGATTノーティフィケーションのストリームを通じて、リクエストに応答する。


https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Art/formatOfAResponseToAGetNotificationAttributesCommand_2x.png

Get Notification Attributes commandへの応答は次の情報を含む:

CommandID
Set to 0 (CommandIDGetNotificationAttributes).

NotificationUID
続くアトリビュートが対応する、iOSノーティフィケーションを示す32ビットのUUID。

AttributeList
AttributeIDs/16-bit Length/Attributeの組み合わせの配列。アトリビュートは常に、その長さがタプルのなかでバイトで示される文字列。
しかし、NULL終端していない。もしも要求された属性が、そのiOSノーティフィケーションで、空あるいはないならば、その長さは0。
もしも要求が、事前に交換したGATT Maximum Transmission Unit(MTU)を超えるならば、NPはそれを複数のフラグメントに分割する。
NCはそれぞれのフラグメントにわけられた応答を再構築しなけrばならない。応答は、それぞれの要求した属性の完全なタプルが受信された時に、完了する。

=== Get App Attributes
The Get App Attributes コマンドは、NCがNPにインストールされた特定のアプリケーションの属性を取得するのを許す。
そのフォーマットは次:

https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Art/formatOfAGetAppAttributesCommand_2x.png

A Get App Attributes コマンドは次の情報を含む:

CommandID
1に設定されるべき (CommandIDGetAppAttributes)

AppIdentifier
クライアントが情報を欲しいアプリケーションを指定する文字列。文字列はNULL終端でなければならない。

AttributeIDs
NCが取得したいアトリビュートの配列。
Get App Attributes コマンドのフォーマットは:

https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Art/formatOfAResponseToAGetAppAttributesCommand_2x.png

Get App Attributes コマンドへの応答は次の情報を含む:

CommandID
Set to 1 (CommandIDGetAppAttributes).

AppIdentifier
属性が所属するアプリケーションを特定する文字列。NULL終端。

AttributeList
AttributeIDs/16-bit Length/Attribute タプルの配列。
属性はタプルで提供されるバイト長の文字列。NULL終端されていない。
アプリに、要求された属性が空もしくは該当がないなら、長さは0。


Get Notification Attributes コマンドへの応答とともに、
Get App Attributes commandへの応答がもしも
事前に交換したGATT Maximum Transmission Unit (MTU)よりも長い場合は、
NPは複数のフラグメントに分解。NCにはそれを再構成しないといけない。

=== セッション
ANCSセッションは、NCがNPのNotification Sourceキャラクタリスティクスをサブスクライブしだして、
NCが同じキャラクタリスティクスをアンサブスクライブするか、NPと切断するかで終了する。
ANCSは完全な同期サービスとして設計されてはいないため、セッションをまたいだ状態の維持はしない。
その結果として、全ての識別子(NotificationUIDおよびAppIdentifier)そしてNCとNPの間で交換される全てのデータは、特定のセッションでのみ有効である。

特定のセッションが終了した時、NCはセッション内で集めて保管した全ての識別子とデータを削除すべきである。
新しいセッションが開始したならば、NPはNCに、システムにある全てのiOSノーティフィケーションを知らせるべく最善を尽くす。
NCは、セッションのリマインダを使うためのモデルを構築する情報として使う。

== アトリビュートの取得とキャッシュ
NCには、ユーザの行動に反応するためのもの、そして必要な物だけで、属性を取得することを強く推奨する。
例えば、NCがiOSノーティフィケーションを簡単なリストで表示することを選択したなら、ユーザが選択したときに特定のiOSノーティフィケーションの詳細を表示するなら、iOSノーティフィケーションのアトリビュートの取得は、レイジーにトリガされるだろう。

セッションの間、NCはapp識別子それぞれのApp Attributeのキャッシュを構築すること、遭遇した、を推奨。
このキャッシュの構築は、変化しないAPp AttributeをNCが複数回要求することを避けて、時間とバッテリーの節約になる。

== Error Codes
Control Point characteristicに書き込むとき、NCは次のANCS特有のエラーコードを受け取るかもしれない。

Unknown command (0xA0): commandIDがNPによって認識されない。

Invalid command (0xA1): コマンドのフォーマットが不正。

Invalid parameter (0xA2): いずれかのパラメータ(例えば NotificationUID)がNPの既存のオブジェクトを参照していない。もしもNPがこのエラーを返したならば、対応するコマンドへのGATT notificationをData Source characteristicに生成しない。

== Example
TBD


== 参考文献
@<href>{https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Specification/Specification.html#//apple_ref/doc/uid/TP40013460-CH1-SW7}

そいえば、ANCS対応の周辺装置を実装するときは、Service changed、の実装は必須。iOSってアプリの起動時とか、iOSアプリがサービスを追加、削除するたび、ローカルデータベースが変化するから。
posted at 09:37:47

メールとかの通知をBLEでやるANCS、あのサービスはiOSが提供するから、iOSアプリ側にはフィルタされて見えない。だから、LightBlueとかのiOSアプリでは、見えないのよ。自分でHCI叩くか、iOS意外の装置開発環境でやるしかない

ANCSはCBPeripheralをアプリでアドバして、とりあえずアクセサリが接続すればサービスが取れる。iOS7はiOSがフィルタするからアプリで模擬できないが、iOS6なら、アプリから見える。


== 実際にやってみると、詰まった
iOS6でサービス指定でスキャンしたけど、繋がらない。デバイス名指定でOK。サービスまでスキャンしないのかしら。
