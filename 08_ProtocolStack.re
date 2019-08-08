
= スタックを作ってみる
実際の通信例、動きを見る、処理を通じて。
== USBドングルとの通信
usbデバイス、HCI、コマンドのあれこれ。
ソースコードの構成。Playground?
開発とか動作確認手順
== 接続と切断
リンク層
発見、アドバタイジング
接続と切断





== 実際の通信例

手元で動く環境。
HCI。
<!-- hci で色々叩いて、実例を示す -->
HCIで直接接続。
HCIの解説。

実例

アドバタイジング
接続
ATT/GATT、読み出し。ペリフェラル。
ペアリング、ボンディング。状態の永続化。
JustWork、モデル1/2、セキュリティ1/2/3
接続パラメータをいじるのと、通信速度、レイテンシ。

ペリフェラルにしてアドバタイジング。iBeacon互換にしてみる。
振る舞い、アプリ側。
エンディアン。

ANCSの接続をしてみる。

iOSのHCIのログを見てみる。


== 実験メモ
USBのドングルと接続する


=== ドングルの動作確認

* プラネックス BT-Micro4 @<href>{http://www.planex.co.jp/products/bt-micro4/} 価格2000円、実売1100円(Amazon)
 *  CSR8510
* バッファロー BSHSBD08BK @<href>{http://buffalo.jp/products/catalog/supply/bluetooth/bluetooth/adapter/bshsbd08/}  価格2509円 実売1300円
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

デフォルト設定は、

    nvram -p | grep bluetoothHostControllerSwitchBehavior

何も設定されていない。(空文字列。-d で削除できる。) 刺しても、外部BTに写らないみたい。

ターミナルを開いて、NVRAMコマンドを次のように入力する。

    user$ sudo nvram bluetoothHostControllerSwitchBehavior="never"
    Password:********

keyと値は、大文字と小文字を区別する。スペルミスに注意。

vmwareをインストールしていると、切り替わらない。
インストールしていないMBAだと、切り替わる。設定neverにより切り替わらない。
システム情報の表示で、USBでのチップセットと、アップル社のチップセットで名前を確認すれば確実。USBドングルが光るとかだと、それで接続が判別できる。
システム情報は、フロントパネルを開いた時に、その時の情報を一気に読み込んで表示。表示状態で、抜き差ししても表示内容は自動更新されないので、気をつける。
確認したいときに、抜き差しして、表示を開いて確認。
TBD: パネルの画像と、手順を入れること


システムの再起動は、これを実行した直後では、不要。
なぜこのステップが必要なのかは、
see section Understanding the OS X Bluetooth Driver Behavior


== libUSB かなにかでBTドングルに接続する
kernel extension カーネルのモジュール
 kextstat
  83    0 0xffffff7f8162e000 0x9000     0x9000     com.apple.iokit.BroadcomBluetoothHostControllerUSBTransport
 165    0 0xffffff7f83697000 0x3000     0x3000     com.apple.iokit.CSRBluetoothHostControllerUSBTransport

Macのシステム情報で、USBデバイスから直接製造とProductIDを取得。
CSR
