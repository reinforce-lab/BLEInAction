= Appendix

== CBCentralManager Class
これは、Apple社のCBCentralManagerクラスドキュメント [http://developer.apple.com/library/ios/#documentation/CoreBluetooth/Reference/CBCentralManager_Class/](http://developer.apple.com/library/ios/#documentation/CoreBluetooth/Reference/CBCentralManager_Class/)から、CoreBluetooth開発の理解に必要最低限の情報を日本語に意訳したものです。

=== プロパティ
==== @property(assign, nonatomic) id<CBCentralManagerDelegate> delegate;
セントラル・マネージャのイベントを受け取るデリゲートを設定します。

==== @property(readonly) CBCentralManagerState state;
セントラル・マネージャの状態を表します。セントラル・マネージャをインスタンスしたときは初期値CBCentralStateUnknownです。この値が変化した瞬間に、デリゲートのコールバック - (void)centralManagerDidUpdateState:(CBCentralManager *)central が呼ばれます。

CBCentralManagerState列挙型は、以下の値を取ります。

```
typedef NS_ENUM(NSInteger, CBCentralManagerState) {
	CBCentralManagerStateUnknown = 0,
	CBCentralManagerStateResetting,
	CBCentralManagerStateUnsupported,
	CBCentralManagerStateUnauthorized,
	CBCentralManagerStatePoweredOff,
	CBCentralManagerStatePoweredOn
};
```

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

=== インスタンスメソッド
==== - (void)cancelPeripheralConnection:(CBPeripheral *)peripheral;
ペリフェラルへの、ペンディング状態もしくは接続している接続を、キャンセルします。

connectPeripheral:options:
Establish a connection to the peripheral.

==== - (void)connectPeripheral:(CBPeripheral *)peripheral options:(NSDictionary *)options;

ペリフェラルへの接続を確立します。引数optionsで接続のオプションを指定出来ます。
CBConnectPeripheralOptionNotifyOnIiscoonectionKey を参照してください。

このメソッドはタイムアウトをしません。ペンディングされた接続をキャンセルするには、cancelPeripheralConnections: を使います。

==== - (CBCentralManager *)initWithDelegate:(id<CBCentralManagerDelegate>)delegate queue:(dispatch_queue_t)queue;
指定したデリゲートとディスパッチキューで、セントラルマネージャを初期化します。

- queue
	- イベントを処理するキューを指定します。nilならばメインキューがデフォルトでつかわれます。

==== - (void)retrieveConnectedPeripherals;
システムに今在接続しているペリフェラルの一覧を取得するようにセントラルに問い合わせます。
問い合わせ結果はデリゲートを通して通知されます。

==== - (void)retrievePeripherals:(NSArray *)peripheralUUIDs;
UUIDで指定したペリフェラルの一覧をセントラルに問い合わせます。

- peripheralUUIDs
	- 取得するCBPeripheralの CFUUIDRef のリスト。

問い合わせ結果はデリゲートを通して通知されます。

==== - (void)scanForPeripheralsWithServices:(NSArray *)serviceUUIDs options:(NSDictionary *)options;

セントラルにペリフェラルのスキャンを要求します。

- serviceUUIDs
	- アプリケーションが興味があるサービスの CBUUIDs の配列。
- options
	- スキャンを指定するオプションを指定した辞書。 CBCentralManagerScanOptionAllowDuplicatesKey を参照。

ペリフェラルそれぞれを発見する都度、デリゲートのコールバックが呼ばれます。"serviceUUIDs"でCBUUIDの配列が指定されている場合は、セントラルはそのサービス、もしくはそれらのサービスをアドバタイズしているペリフェラルのみを返します(推奨)。
"seriviceUUIDs"がnilの場合は、発見したすべてのペリフェラルを返します(非推奨)。
セントラルが、すでに異なるパラメータでスキャンしている場合は、指定したパラメータがそれを置換します。

==== - (void)stopScan;
セントラルにペリフェラルのスキャンを停止するよう問い合わせます。

=== 定数


==== CBCentralManagerScanOptionAllowDuplicatesKey

このキーにはNSNumber(Boolean)の値が対応します。値がYESのとき、
ペリフェラルのスキャンで、重複したデバイスをフィルタリングをしません。デフォルトでは、同じペリフェラルを複数回発見したときは、1回の発見イベントに集約します。このフィルタリングを無効にすると、バッテリーの稼働時間に不利な影響をもたらすかもしれませんから、必要なときにだけ利用します。

scanForPeripheralsWithServices:options: を参照してください。

==== CBConnectPeripheralOptionNotifyOnConnectionKey
*iOS6から有効*です。

このキーには、NSNumber(Boolean)の値が対応します。値は、アプリケーションがサスペンドしている時に、
あるペリフェラルとの接続が確立したならば、
それをシステムがアラート表示すべきことを示します。

この設定は、アプリケーションが
bluetooth-central バックグラウンド・モード
を指定していないために、アプリケーションが自分でアラート表示できないときに便利です。

あるペリフェラルに対して、2つ以上のアプリケーションがこの通知を要求していたならば、フォアグラウンドで最も最近に要求をしたアプリケーションが、通知を受け取ります。

==== CBConnectPeripheralOptionNotifyOnDisconnectionKey
このキーには、NSNumber(Boolean)の値が対応します。この値は、アプリがサスペンドしている時に、
あるペリフェラルが切断したならば、それをシステムがアラート表示すべきことを示します。

この設定は、アプリケーションが
bluetooth-central バックグラウンド・モード
を指定していないために、アプリケーションが自分でアラート表示できないときに便利です。

あるペリフェラルに対して、2つ以上のアプリケーションがこの通知を要求していたならば、フォアグラウンドで最も最近に要求をしたアプリケーションが、通知を受け取ります。

==== CBConnectPeripheralOptionNotifyOnNotificationKey
このキーには、NSNumber(Boolean)の値が対応します。この値は、アプリがサスペンドしている時に、
あるペリフェラルから受信したすべてののティフィケーション(ここでのノティフィケーションは、iOSのそれではなく、Bluetooth low energyの規格でいうノティフィケーションを示す)に対して、
それをシステムがアラート表示すべきことを示します。

この設定は、アプリケーションが
bluetooth-central バックグラウンド・モード
を指定していないために、アプリケーションが自分でアラート表示できないときに便利です。

あるペリフェラルに対して、2つ以上のアプリケーションがこの通知を要求していたならば、フォアグラウンドで最も最近に要求をしたアプリケーションが、通知を受け取ります。


== CBCentralManagerDelegate Protocol

これは、Apple社のCBCentralManagerクラスドキュメント [http://developer.apple.com/library/mac/#documentation/CoreBluetooth/Reference/CBCentralManagerDelegate_Protocol/translated_content/CBCentralManagerDelegate.html](http://developer.apple.com/library/mac/#documentation/CoreBluetooth/Reference/CBCentralManagerDelegate_Protocol/translated_content/CBCentralManagerDelegate.html)
から、CoreBluetooth開発の理解に必要最低限の情報を日本語に意訳したものです。

CBCentralManagerDelegate プロトコルは、CBCentralのデリゲートのプロトコルです。

CBCentralManagerDelegateは、-[centralManagerDidUpdateState:] のみがrequiredです。その他は optional です。

=== インスタンス・メソッド
==== - (void)centralManagerDidUpdateState:(CBCentralManager *)central;
セントラル・マネージャのステートが更新された時に、呼ばれます。

引数:

- central
	- ステートが変化したセントラルマネージャ。

議論:

詳細は "state" プロパティを参照してください。

==== - (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral;
ペリフェラルとの接続が確立したときに、呼ばれます。

<<<<<<< HEAD
==== - (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error;
ペリフェラルとの既存の接続が切断した時に、呼ばれます。

====
centralManager:didDisconnectPeripheral:error:
Invoked whenever an existing connection with the peripheral is torn down.

==== - (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error;
ペリフェラルとの既存の接続が切断した時に、呼ばれます。

centralManager:didDiscoverPeripheral:advertisementData:RSSI:
Invoked when the central discovers a peripheral while scanning.

==== - (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI;

セントラルがスキャン中にペリフェラルを発見した時に、呼ばれます。

議論:

アドバタイズメント / スキャン・レスポンス・データは、"advertisementData"に収納され、CBAdvertisementData * キーを通して読み出すことができる。 (訳者注:BLEの規格ではアドバタイズメント・パケットには、デバイス名、デバイスが提供するサービスのUUID、メーカ独自の拡張データを収納することができます。最後の拡張データがiOSで読み取れるかは、未確認。)
この引数のperipheralは、このペリフェラルに対して、なにかのコマンドを実行する場合は、必ずリテインしなければならない。(訳者注:リテインしないと、ペリフェラルは開放されて、接続が切断します。)

centralManager:didFailToConnectPeripheral:error:
Invoked whenever the central manager fails to create a connection with the peripheral.

==== - (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error;

セントラル・マネージャがペリフェラルと接続を確立するのに失敗した時に、呼ばれます。

議論:

失敗した原因は"error"に格納されます。

centralManager:didRetrieveConnectedPeripherals:
Invoked when the central manager retrieves the list of peripherals currently connected to the system.

==== - (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray *)peripherals;

セントラル・マネージャが、現在システムと接続しているペリフェラルの一覧を取得した時に、呼び出されます。

議論:

詳細は -[retrieveConnectedPeripherals] メソッドを参照してください。

centralManager:didRetrievePeripherals:
Invoked when the central manager retrieves the list of known peripherals.

==== - (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray *)peripherals;

セントラル・マネージャが、既知のペリフェラルの一覧を取得した時に、呼び出されます。

議論:

詳細は -[retrievePeripherals:] メソッドを参照してください。

See Also:

centralManager:didRetrievePeripheral:

=== 定数(CBAdvertisementData.h)
CBAdvertisementData.h は -[centralManager:didDiscoverPeripheral:advertisementData:RSSI:] のadvertisementDataの辞書の鍵を定義しています。

==== CBAdvertisementDataServiceUUIDsKey;
CBServiceのUUIDを表す、1つもしくはそれ以上のCBUUIDのリスト。

==== const CBAdvertisementDataOverflowServiceUUIDsKey
*iOS6以降*
アドバタイズメントデータの"overflow"領域で見つけられたCBService UUIDを表す、1つもしくはそれ以上のCBUUIDのリスト。
この領域のUUIDは"ベストエフォート"であるため、常に正確とは限らない。

参照:

startAdvertising:

==== CBAdvertisementDataLocalNameKey
ペリフェラルのローカル名を表す、NSString。

==== CBAdvertisementDataTxPowerLevelKey
ペリフェラルの送信電力を表す、NSNumber。

==== CBAdvertisementDataManufacturerDataKey
ペリフェラルの製造者データを表す、NSDataオブジェクト。

==== CBAdvertisementDataServiceDataKey
サービス特有のアドバタイズメントデータを含む辞書。
その辞書の鍵は、CBService UUIDを表す、CBUUIDオブジェクト。値はNSDataオブジェクト。

== CBUUID Class
=== 定数

==== CB_EXTERN NSString * const CBUUIDCharacteristicExtendedPropertiesString;
extended properties descriptorのUUIDの文字列表現です。
このデスクリプタに対応する値は、NSNumber オブジェクトです。

==== CB_EXTERN NSString * const CBUUIDCharacteristicUserDescriptionString;
user description descriptorのUUIDの文字列表現です。
このデスクリプタに対応する値は、NSString オブジェクトです。

==== CB_EXTERN NSString * const CBUUIDClientCharacteristicConfigurationString;
client configuration descriptor
のUUIDの文字列表現です。
このデスクリプタに対応する値は、NSNumber オブジェクトです。

==== CB_EXTERN NSString * const CBUUIDServerCharacteristicConfigurationString;
server configuration descriptor
のUUIDの文字列表現です。
このデスクリプタに対応する値は、NSNumber オブジェクトです。

==== CB_EXTERN NSString * const CBUUIDCharacteristicFormatString;
presentation format descriptor
のUUIDの文字列表現です。
このデスクリプタに対応する値は、NSData オブジェクトです。

==== CB_EXTERN NSString * const CBUUIDCharacteristicAggregateFormatString;
server configuration descriptor
のUUIDの文字列表現です。

==== CB_EXTERN NSString * const CBUUIDGenericAccessProfileString;
GAP
のUUIDの文字列表現です。

==== CB_EXTERN NSString * const CBUUIDGenericAttributeProfileString;
GATT
のUUIDの文字列表現です。

==== CB_EXTERN NSString * const CBUUIDDeviceNameString;
GAP device name
のUUIDの文字列表現です。

==== CB_EXTERN NSString * const CBUUIDAppearanceString;
GAP appearance UUID
のUUIDの文字列表現です。

==== CB_EXTERN NSString * const CBUUIDPeripheralPrivacyFlagString;
GAP privacy flag UUID
の文字列表現です。

==== CB_EXTERN NSString * const CBUUIDReconnectionAddressString;
GAP reconnection address UUID
の文字列表現です。

==== CB_EXTERN NSString * const CBUUIDPeripheralPreferredConnectionParametersString;
GAP preferred connection parameter UUID
の文字列表現です。

==== CB_EXTERN NSString * const CBUUIDServiceChangedString;
GATT service changed UUID
の文字列表現です。


==== CBUUID クラス
16-bitまたは128-bitのBluetooth UUIDを表すクラスです。
16-bit UUIDは、いうまでもなく、Bluetooth Base UUIDで事前に満たされています。
(訳者注:Bluetooth low energyのUUIDは、128-bitが基本です。しかし、Bluetooth SIGが定義したものは16-bitの短縮形UUIDが使えます。この16-bitのUUIDは、Bluetooth Base UUIDという128-bit のUUIDの先頭の末尾16-bitを使うことで、実現しています。)

==== プロパティ


==== @property(nonatomic, readonly) NSData *data;
NSDataとしてのUUID

/*!
 * @method UUIDWithString:
 *
 *  @discussion
 *      Creates a CBUUID with a 16-bit or 128-bit UUID string representation.
 *      The expected format for 128-bit UUIDs is a string punctuated by hyphens, for example 68753A44-4D6F-1226-9C60-0050E4C00067.
 *
 */

==== メソッド

==== + (CBUUID *)UUIDWithString:(NSString *)theString;
16-bitもしくは128-bitのUUID文字列表記からCBUUIDを作ります。
128-bit UUIDはハイフンで区切られた文字列フォーマットを期待します。例: 68753A44-4D6F-1226-9C60-0050E4C00067 。
(訳者注:16-bitのUUIDは、4桁の16進表記文字列で与えます。先頭に0xをつける必要は、ありません。)

==== + (CBUUID *)UUIDWithData:(NSData *)theData;
16-bitもしくは128-bitのデータコンテナからCBUUIDを作ります。

==== + (CBUUID *)UUIDWithCFUUID:(CFUUIDRef)theUUID;
CFUUIDRef からCBUUIDを作ります。


=== CBPeripheral Class
==== プロパティ

==== @property(assign, nonatomic) id<CBPeripheralDelegate> delegate;
ペリフェラルのイベントを受信するデリゲートです。

==== @property(readonly, nonatomic) CFUUIDRef UUID;
ペリフェラルが、少なくとも一度システムから接続されたことがあれば、ペリフェラルにはUUIDが割り当てられます。(訳者注:逆にこれまで一度も接続したことがないペリフェラルでは、nilになります。接続は、他のアプリでの接続、iPhoneの電源オン/オフ、再起動に関係なく、一度でも接続したことがあるペリフェラルであれば、UUIDが割り当てらています。)

ペリフェラルを取得するために、後に
BCentralManager
に与えるために、このUUIDを保存しておけます。

==== @property(retain, readonly) NSString *name;
ペリフェラルの名前です。(訳者注:アドバタイズメント・データのローカル名を示します。通常は型番が与えらています。)

==== @property(retain, readonly) NSNumber *RSSI;
接続している間、接続のRSSIをデシベルで表します。

(訳者注:RSSIは、Received Signal Strength Indicatorの略称で、受信信号強度を表します。このRSSIは、RF送受信の半導体が出力する信号値をそのまま使っていると推測されます。物理的に絶対値が正しい値とは限りませんが、目安としては利用できるでしょう。信号レベルは対数で、デシベル(10log 信号電力)で表します。通常、-40 ~ -90dB程度の範囲です。マイナスになるほど、信号が弱いことを示します。)

==== @property(readonly) BOOL isConnected;
ペリフェラルが現在接続しているかを、示します。

==== @property(retain, readonly) NSArray *services;
ペリフェラルで発見されたサービスの、CBServiceオブジェクトのリストです。

==== メソッド

==== - (void)readRSSI;
接続の現在のRSSIを取得します。

see		peripheralDidUpdateRSSI:error:

==== - (void)discoverServices:(NSArray *)serviceUUIDs;
ペリフェラルで有効なサービスを発見します。

- serviceUUIDs
	- 発見すべきサービスのUUIDを表す、CBUUIDオブジェクトのリストです。もしもnilを与えると、ペリフェラルのすべてのサービスが発見されるでしょうが、とても遅くなり、従って推奨しません。

see peripheral:didDiscoverServices:

==== - (void)discoverIncludedServices:(NSArray *)includedServiceUUIDs forService:(CBService *)service;
指定したサービスのincluded serviceを発見します。
(訳者注: Bluetooth low energyのサービスは、オブジェクト指向でいうクラスの概念に相当します。いまあるサービスはそのまま提供しつつ、そのサービスの機能を拡張する仕組みが、included serviceで、ちょうどクラス継承の概念に相当します。)

- includedServiceUUIDs
	- 発見すべきincluded serviceのUUIDを表すCBUUIDオブジェクトのリストです。もしもnilならば、サービスのすべてのincluded serviceが発見されますが、とても遅くなり、従って推奨しません。
- service
	- プライマリGATTサービス

see						peripheral:didDiscoverIncludedServicesForService:error:

==== - (void)discoverCharacteristics:(NSArray *)characteristicUUIDs forService:(CBService *)service;
サービスの指定したcharacteristicを発見します。

- charactertisticsUUIDs
	- 発見すべきcharacteristicのUUIDを表すCBUUIDオブジェクトのリストです。もしnilならば、サービスのすべてのcharacteristicsが発見されますが、とても遅くなり、従って推奨しません。
- service
	- GATTサービス

see						peripheral:didDiscoverCharacteristicsForService:error:

==== - (void)readValueForCharacteristic:(CBCharacteristic *)characteristic;
characteriticの値を読みます。

- characteristic
	- GATT characteristic

see					peripheral:didUpdateValueForCharacteristic:error:

==== - (void)writeValue:(NSData *)data forCharacteristic:(CBCharacteristic *)characteristic type:(CBCharacteristicWriteType)type;
characteristicの値として、valueを書き込みます。

- data
	- 書き込む値
- characteristic
	- 	書き込み対象のcharacteristic
- type
	- 書き込みタイプ。characteristicへの書き込み完了を報告する/しないが設定できる。

see					peripheral:didWriteValueForCharacteristic:error:

see					CBCharacteristicWriteType

==== - (void)setNotifyValue:(BOOL)enabled forCharacteristic:(CBCharacteristic *)characteristic;
characteristicの値の、notification/indicationの有効/無効を背呈します。

characterisitcが、notification/indicationの両方を許可しているならば、notificationが使われます。
notification/indicationが有効になったとき、characteristicの値更新は、デリゲート
peripheral:didUpdateValueForCharacteristic:error:
に通知されます

更新を送るのは、選択したペリフェラルが行うものなので、
notification/indicationが有効である間、アプリケーションはその変更を処理できるように備えるべきです。

(訳者注:Bluetooth low energyは、接続先の装置の値更新をiPhone側にプッシュする仕組みがあります。それがnotificationとindicationです。この2つの違いは、通知の信頼性です。

- notification
	- 接続先の装置からiPhoneに、値の更新を通知します。
	- notificationは通知だけの、信頼性のない通知です。iPhoneが接続先の装置に、値更新通知を受信したことを返しません。
- indication
	- notificationと同じく、接続先の装置からiPhoneに、値の更新を通知します。
	- indicationは、iPhoneが接続先装置に値更新を受信したことを返信する、信頼性が確保された通知方法です。

例えば、室温やバッテリー残量のように、センサーなどの値変化を通知するが、一定周期で送信するため、通知データを取りこぼしても問題がない場合は、notificationを用いるなどします。

notification/indicationのいずれを実装しているかは、装置のファームウェア設計によります。readできるcharacteristicが、notification/indicationを実装しているとは限りません。iPhone側は、configutaionをみて、そのcharacteristicがnotification/indicationに対応しているかをみることしかできません。)

- enabled
	- notification/indication を有効にすべきか、否かを設定します
- characteristic
	- クライアントcharacteristic configuration descriptorを保持しているcharacteristic


see					peripheral:didUpdateNotificationStateForCharacteristic:error:

seealso                CBConnectPeripheralOptionNotifyOnNotificationKey

==== - (void)discoverDescriptorsForCharacteristic:(CBCharacteristic *)characteristic;
characteristicのdescriptor(s)を発見します。

- characteristic
	- GATT characteristic

see					peripheral:didDiscoverDescriptorsForCharacteristic:error:

==== - (void)readValueForDescriptor:(CBDescriptor *)descriptor;
descriptorの値を読みます。

- descriptor
	- GATT characteristic descriptor

see				peripheral:didUpdateValueForDescriptor:error:

==== - (void)writeValue:(NSData *)data forDescriptor:(CBDescriptor *)descriptor;
descriptorの値としてvalueを書き込みます。Client characteristic configuration descriptorはこのメソッドで書き込みはできません。
それには、 setNotifyValue:forCharacteristic: を使うべきです。

see				peripheral:didWriteValueForCharacteristic:error:

====

==== 列挙型
==== CBCharacteristicWriteType

- CBCharacteristicWriteWithResponse = 0,
- CBCharacteristicWriteWithoutResponse,


=== CBPeripheralDelegate Protocol

これはApple社の[http://developer.apple.com/library/ios/#documentation/CoreBluetooth/Reference/CBPeripheralDelegate_Protocol/translated_content/CBPeripheralDelegate.html](http://developer.apple.com/library/ios/#documentation/CoreBluetooth/Reference/CBPeripheralDelegate_Protocol/translated_content/CBPeripheralDelegate.html)のうち、CoreBluetoothを理解するために必要最小限の部分を日本語訳したものです。

CBPeripheralDelegateプロトコルは、CBPeripheralのdelegateプロパティが実装すべきプロトコルです。
CBPeripheralDelegateプロトコルの、すべてのメソッドはoptionalです。

==== インスタンス・メソッド
==== - (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error;
-[discoverCharacteristics:forService:] リクエストが完了した時に、呼ばれます。

もしも成功したら、"error"はnilで、発見されたcharacteriticsは、それがあったならば、サービスの"characteristics"プロパティにマージされています。もしも成功しなかったら、"error"には、発生した失敗が設定されます。

Invoked upon completion of a request.

==== - (void)peripheral:(CBPeripheral *)peripheral didDiscoverDescriptorsForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error;
-[discoverDescriptorsForCharacteristic:] リクエストが完了した時に、呼ばれます。

もしも成功したら、"error"はnilで、発見されたdescriptorsは、それがあったならば、キャラクタリスティックの"descriptors"プロパティにマージされています。もしも成功しなかったら、"error"には、発生した失敗が設定されます。

Invoked upon completion of a -[discoverIncludedServices:forService:] request.

==== - (void)peripheral:(CBPeripheral *)peripheral didDiscoverIncludedServicesForService:(CBService *)service error:(NSError *)error;
-[discoverIncludedServices:forService:] リクエストが完了した時に、呼ばれます。

もしも成功したら、"error"はnilで、発見されたservicesは、それがあったならば、サービスの"includedServices"プロパティにマージされています。もしも成功しなかったら、"error"には、発生した失敗が設定されます。

==== - (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error;
-[discoverServices:] リクエストが完了した時に、呼ばれます。

もしも成功したら、"error"はnilで、発見されたservicesは、それがあったならば、ペリフェラルの"services"プロパティにマージされています。もしも成功しなかったら、"error"には、発生した失敗が設定されます。

==== - (void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error;
-[setNotifyValue:forCharacteristic:] リクエストが完了した時に、呼ばれます。

もしも成功しなかったら、"error"には、発生した失敗が設定されます。

(訳者注:ここから先の説明があまりに素っ気いないのですが、原文そのままです。あまりにそっけないので、あとで解説を追加します。)

==== - (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error;
-[readValueForCharacteristic:] リクエストが完了した、もしくはnotification/indicationを受信した時に、呼ばれます。

もしも成功しなかったら、"error"には、発生した失敗が設定されます。

==== - (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForDescriptor:(CBDescriptor *)descriptor error:(NSError *)error;
-[readValueForDescriptor:] リクエストが完了した時に、呼ばれます。

もしも成功しなかったら、"error"には、発生した失敗が設定されます。

==== - (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error;
-[writeValue:forCharacteristic:] リクエストが完了した時に、呼ばれます。

もしも成功しなかったら、"error"には、発生した失敗が設定されます。

==== - (void)peripheral:(CBPeripheral *)peripheral didWriteValueForDescriptor:(CBDescriptor *)descriptor error:(NSError *)error;
-[writeValue:forDescriptor:] リクエストが完了した時に、呼ばれます。

もしも成功しなかったら、"error"には、発生した失敗が設定されます。

==== - (void)peripheralDidUpdateRSSI:(CBPeripheral *)peripheral error:(NSError *)error;
-[readRSSI:] リクエストが完了した時に、呼ばれます。

もしも成功したら、"error"はnilで、ペリフェラルの"RSSI"プロパティは更新されています。もしも成功しなかったら、"error"には、発生した失敗が設定されます。

=== CBService Class

これはCBServiceクラスのドキュメントを、CoreBluetoothを理解するために必要最小限の部分について、日本語訳したものです。

CBServiceクラスは、ペリフェラルのサービスまたはサービスのincluded serviceを表します。

==== プロパティ

==== @property(readonly, nonatomic) CBPeripheral *peripheral;
このサービスが属するペリフェラルへのポインタ。

==== @property(readonly, nonatomic) CBUUID *UUID;
サービスのBluetooth UUID

==== @property(readonly, nonatomic) BOOL isPrimary;
サービスのタイプ(primary または secondary)

==== @property(retain, readonly) NSArray *includedServices;
このサービスでこれまでに発見されたincluded serviceのリスト。

==== @property(retain, readonly) NSArray *characteristics;
このサービスでこれまでに発見されたcharacteristicのリスト。

==== CBMutableServiceクラス

CBPeripheralManagerを通してローカルデータベースに追加できる、ローカルサービスもしくはincluded serviceを作るのに使います。
一旦サービスが公開されたならば、キャッシュされて、それ以降は変更できません。
このクラスはCBServiceのすべてのプロパティに書き込み属性を追加します。

*iOS6以降で有効です。*

==== @property(retain, readwrite, nonatomic) CBUUID *UUID;
==== @property(readwrite, nonatomic) BOOL isPrimary;
==== @property(retain, readwrite) NSArray *includedServices;
==== @property(retain, readwrite) NSArray *characteristics;

==== - (id)initWithType:(CBUUID *)UUID primary:(BOOL)isPrimary;
サービスタイプとUUIDで初期化されたサービスを返します。

- UUID
	- サービスのBluetooth UUID
- isPrimary
	- サービスのタイプ(primary または secondary)


=== CBCharacteristics Class
これはCBCharacteristicsクラスのうちCoreBlutoothを理解するために必要最小限の部分を日本語訳したものです。

==== CBCharacteristicsクラス


==== プロパティ
==== @property(readonly, nonatomic) CBService *service;
このcharacteristicが属するサービスへのポインタです。

==== @property(readonly, nonatomic) CBUUID *UUID;
characteristicのBluetooth UUID。

==== @property(readonly, nonatomic) CBCharacteristicProperties properties;
characteristicのプロパテイです。

==== @property(retain, readonly) NSData *value;
characteristicsの値です。

==== @property(retain, readonly) NSArray *descriptors;
このcharacteristicで、これまでに発見されたCBDescriptorsのリストです。

==== @property(readonly) BOOL isBroadcasted;
このcharacteristcが現在ブロードキャストされているか、否かを示します。

==== @property(readonly) BOOL isNotifying;
このcharacteristicが現在ノーティフィケーションされているか否かを示します。

==== CBMutableCharacteristics クラス
*iOS6から有効です。*
CBMutableCharacteristicsクラスは、CBCharacteristicクラスを継承します。

==== プロパティ
==== @property(assign, readwrite, nonatomic) CBAttributePermissions permissions;
characteristic valueの許可設定です。

see		CBAttributePermissions

==== @property(retain, readwrite, nonatomic) CBUUID *UUID;
==== @property(assign, readwrite, nonatomic) CBCharacteristicProperties properties;
==== @property(retain, readwrite) NSData *value;
==== @property(retain, readwrite) NSArray *descriptors;

==== インスタンスメソッド

==== - (id)initWithType:(CBUUID *)UUID properties:(CBCharacteristicProperties)properties value:(NSData *)value permissions:(CBAttributePermissions)permissions;
返り値は、初期化されたcharacteristicです。

- UUID
	- characteristicのBluetooth UUIDです。
- properties
	- 	characteristicのプロパティです。
- value
	- キャッシュされるcharacteristicの値です。もしもnilならば、値はダイナミックであり、オンデマンドで要求されます。
- permissions
	- charactersticの値のパーミションです。

==== 列挙型

==== CBAttributePermissions
ATT attributeの読み出し/書き込み/暗号化許可属性です。論理和で結合できます。
- CBAttributePermissionsReadable
	- 読み出しのみ
- CBAttributePermissionsWriteable
	- 書き込みのみ
- CBAttributePermissionsReadEncryptionRequired
	- 信頼されたデバイスが、読み出し可能
- CBAttributePermissionsWriteEncryptionRequired
	- 信頼されたデバイスが、書き込み可能

==== CBCharacteristicProperties
CBCharacteristicPropertiesは、そのcharacteristicの値がどのように使えるか、またはdescriptor(s)がアクセスできるかを示します。
論理和で結合させることができます。
特に明記がない限り、
CBPeripheralManager で公開されたローカルのcharacteristicsに対してプロパティは有効です。

- CBCharacteristicPropertyBroadcast
	- characteristic configuration descriptorを使って、characteristicの値をブロードキャすることを許します。
	- local characteristcには許されていません。
- CBCharacteristicPropertyRead
	 - characteristicの値を読むことを許します。
- CBCharacteristicPropertyWriteWithoutResponse
 	- characteristicの値への、レスポンスがない書き込みを許可します。
- CBCharacteristicPropertyWrite
	- characteristicの値への、書き込みを許可します。
- CBCharacteristicPropertyNotify
	- characteristicの値の、レスポンスがないノーティフィケーションを許可します。
- CBCharacteristicPropertyIndicate
	- characteristicの値の、インディケーションを許可します。
- CBCharacteristicPropertyAuthenticatedSignedWrites
	- characteristicの値への、承認ありの書き込みを許可します。
- CBCharacteristicPropertyExtendedProperties
	- もしも設定されていれば、追加のcharacteristicプロパティが、characteristic extended properties descriptorに定義されています。
	- local characteristicsには許されていません。
- CBCharacteristicPropertyNotifyEncryptionRequired
	- もしも設定されていれば、信頼されたデバイスのみがcharacteristicの値のノーティフィケーションを有効に出来ます。
- CBCharacteristicPropertyIndicateEncryptionRequired
	- もしも設定されていれば、信頼されたデバイスのみがcharacteristicの値のインディケーションを有効に出来ます。



=== CBDescriptor Class


==== プロパティ


==== @property(readonly, nonatomic) CBCharacteristic *characteristic;
属するcharacteristicのポインタです。

/*!
 *  @property UUID
 *
 *  @discussion
 *      The Bluetooth UUID of the descriptor.
 *
 */
==== @property(readonly, nonatomic) CBUUID *UUID;
DescriptorのBluetooth UUIDです。

==== @property(retain, readonly) id value;
Descriptorのあちあです。様々なデスクリプタに対応するvalue typeの詳細は、CBUUIDクラスで定義されています。

==== CBMutableDescriptorクラス
*iOS6以降で有効です。*

==== - (id)initWithType:(CBUUID *)UUID value:(id)value;
サービスタイプとvalueで初期化されたdescriptorを返します。一旦親であるserviceが公開されたならば、valueは要求されて、ダイナミックに更新することはできません。

- UUID
	- DescriptorのBluetooth UUID
- value
	- Descriptorの値

=== CBPeripheralManager Class

これはCBPeripheralManagerクラスのリファレンスを、CoreBluetoothを理解するために必要最小限の部分を日本語訳したものです。

CBPeripheralManagerクラスは、peripheral roleへの入り口です。
コマンドは、その状態がCBPeripheralManagerStatePoweredOn のときにだけ、発行されるべきです。
2つ以上のCBPeripheralManagerを同時に使うことはサポートされませんし、そうした場合には不定の振る舞いをするでしょう。

CBperipheralManagerクラスは、*iOS6以降で利用できます*。

==== プロパティ

==== @property(assign, nonatomic) id<CBPeripheralManagerDelegate> delegate;
Peripheralイベントを受信するデリゲート。

==== @property(readonly) CBPeripheralManagerState state;
Peripheralの現在の状態。初期値はCBPeripheralManagerStateUnknown。
値更新は、required なデリゲートのメソッド
peripheralManagerDidUpdateState:
に提供されます。

==== @property(readonly) BOOL isAdvertising;
Peripheralが、今データをアドバタイズしているか、いなかを示します。

==== インスタンス・メソッド

==== - (id)initWithDelegate:(id<CBPeripheralManagerDelegate>)delegate queue:(dispatch_queue_t)queue;
イニシャライザです。Peripheral roleのイベントは、指定されたキューで処理されます。
もしもキューがnilならば、メインキューが使われるでしょう。

- delegate
	- Peripheral roleイベントを受け取るデリゲート。
- queue
	- イベントを処理するdispatch queue。

==== - (void)startAdvertising:(NSDictionary *)advertisementData;
アドバタイズメントを開始します。サポートされているアドバタイズメント・データ・タイプは、CBAdvertisementDataLocalNameKey と CBAdvertisementDataServiceUUIDsKey です。

アプリケーションがフォアグランドのときは、
初期のアドバタイズメント・データを、28バイトまで、
サポートされているアドバタイズメント・データ・タイプの任意の組み合わせに使えます。
もしこの領域を使い切ると、scan responseの10バイトを追加の領域として、ローカルネームに対してのみ、使えます。

このサイズは、新しいデータタイプそれぞれに必要な2バイトのヘッダ情報を含まないことに、注意してください。

割り当て領域に収まらなかったサービスUUIDsは、特別な"オーバフロー"領域に追加されます。したがって、それらのサービスUUIDは、
iOSデバイスが、それらを明示的にスキャンしたときにだけ、発見されます。

アプリケーションがバックグラウンドにあるときは、ローカルネームは使われず、すべてのサービスUUIDsは“オーバフロー”エリアに置かれます。

see                        peripheralManagerDidStartAdvertising:error:

seealso                    CBAdvertisementData.h
==== - (void)stopAdvertising;
アドバタイズを停止します。

==== - (void)setDesiredConnectionLatency:(CBPeripheralManagerConnectionLatency)latency forCentral:(CBCentral *)central;
すでにあるセントラルとの接続の、コネクション・レイテンシを希望する値に設定します。
コネクション・レイテンシの変更は保証されず、したがって結果として得られる遅延は、指定したものとは違うかもしれません。
もしも望むレイテンシが設定されないなら、接続が確立した時にセントラルが選んだレイテンシが使われます。
一般に、レイテンシを変更する必要はありません。

see            CBPeripheralManagerConnectionLatency

==== - (void)addService:(CBMutableService *)service;
サービスと、それに関連付けられたcharacteristic(s)をローカルデータベースに公開します。もしもサービスがincluded serviceを含むなら、
まずincluded serviceが最初に公開されねばなりません。

- service
	- GATTサービス

see            peripheralManager:didAddService:error:

==== - (void)removeService:(CBMutableService *)service;
ローカルデータベースから、公開されたサービスを削除します。もしもサービスがincluded serviceを含むならば、まず最初にincluded serviceが削除されねばなりません。

==== - (void)removeAllServices;
ローカルデータベースから、すべての公開されているサービスを削除します。

==== - (void)respondToRequest:(CBATTRequest *)request withResult:(CBATTError)result;
peripheralManager:didReceiveReadRequest: もしくは peripheralManager:didReceiveWriteRequests: のデリゲートのメソッドで受信したリクエストに応答するのに使います。

- request
	- セントラルから受信されたオリジナルのリクエスト
- result
	- request を満たそうとした結果

see            peripheralManager:didReceiveReadRequest:

see            peripheralManager:didReceiveWriteRequests:

==== - (BOOL)updateValue:(NSData *)value forCharacteristic:(CBMutableCharacteristic *)characteristic onSubscribedCentrals:(NSArray *)centrals;
1つもしくはそれ以上のセントラルに、更新されたcharacteristicの値を、notificationもしくはindicationで送信します。

返り値は、アップデートが送信されたならばYES、送信キューが満杯ならばNO。
もしNOが返ってきたら、スペースが有効になった時に1度、デリゲートの peripheralManagerIsReadyToUpdateSubscribers: メソッドが呼ばれる。したがって、もしも望むならば、アップデートを再送信する。

- value
	- notification/indicationで送信される値
- characteristic
	- 値が変化したcharacteristic。
- centrals
	- アップデートを受け取るCBCentralオブジェクトのリスト。characteristic を購読していないセントラルは無視されることに注意する。もしもnilならば、characteristicが購読するセントラルすべてが更新される。
see                    peripheralManager:central:didSubscribeToCharacteristic:

see                    peripheralManager:central:didUnsubscribeFromCharacteristic:

see                    peripheralManagerIsReadyToUpdateSubscribers:

==== 列挙型

==== CBPeripheralManagerState
CBperipheralManagerの現在の状態を表します。
- CBPeripheralManagerStateUnknown
	- 不明な状態。すぐに更新されます。
- CBPeripheralManagerStateResetting
	- システムサービス都の接続が、一時的に失われました。すぐにアップデートされます。
- CBPeripheralManagerStateUnsupported
	- そのプラットフォームはBluetooth low energy Peripheral/Server roleをサポートしません。
- CBPeripheralManagerStateUnauthorized
	- アプリケーションはBluetooth low energy Peripheral/Server roleを使う権限がありません。
- CBPeripheralManagerStatePoweredOff
	- Bluetoothは現在電源がオフです。
- CBPeripheralManagerStatePoweredOn
	- Bluetoothは現在電源がオンで、利用できます。

==== CBPeripheralManagerConnectionLatency
Peripheral-central接続の遅延時間は、メッセージがどれほどの頻度で交換できるか、を制御します。
- CBPeripheralManagerConnectionLatencyLow
	- バッテリーの持ち時間よりも、素早い通信を優先します。
- CBPeripheralManagerConnectionLatencyMedium
	- 通信の頻度とバッテリーの持ち時間とのバランスを取ります。
- CBPeripheralManagerConnectionLatencyHigh
	- 素早い通信よりも、バッテリーの持ち時間を伸ばすことを優先します。


==== CBPeripheralManagerDelegate プロトコル
CBPeripheralManager オブジェクトのデリゲートは、CBPeripheralManagerDelegate プロトコルを実装しなければならない。

実装が必須の1つだけのメソッドは、ペリフェラル・マネージャーの能力を示す、一方のその他のオプションのメソッドは、ローカルデータベースに接続してアクセスするだろう、セントラルについての情報を提供する。

==== peripheralManagerDidUpdateState:
引数は、状態が変化したとき。ペリフェラル・マネージャー。

ペリフェラル・マネージャーの状態が更新された都度、呼び出される。コマンドは
CBPeripheralManagerStatePoweredOn の時のみ発行できる。
CBPeripheralManagerStatePoweredOn状態の時は、
アドバタイズメントはずっと停止したままで、いかなるセントラルも切断完了している状態。
もしも、CBPeripheralManagerStatePoweredOff状態になったら、アドバタイズメントは停止されていて、明示的に再起動せねばならない、そして、ローカルデータベースはクリーンにされていて、すべてのサービスは再度追加されねばならない。

- (void)peripheralManagerDidUpdateState:(CBPeripheralManager *)peripheral;


==== peripheralManagerDidStartAdvertising:error:
引数: peripheral 情報を提供するペリフェラル・マネージャー。
引数: error エラーが発生したら、エラー内容を示す。
このメソッドは、startAdvertising:メソッド呼び出しの結果を返す。
もしもアドバタイズメントが開始できないならば、その原因は引数errorに詳細がある。

- (void)peripheralManagerDidStartAdvertising:(CBPeripheralManager *)peripheral error:(NSError *)error;

==== peripheralManager:didAddService:error:
service      ローカルデータベースに追加するサービス。
error       エラーが発生した時は、その原因。

このメソッドは、addService:メソッドの呼び出し結果を返す。サービスがローカルデータベースに公開されなかったならば、その原因は引数errorで示される。

- (void)peripheralManager:(CBPeripheralManager *)peripheral didAddService:(CBService *)service error:(NSError *)error;

==== peripheralManager:central:didSubscribeToCharacteristic:
peripheral       The peripheral manager providing this update.
central          このコマンドを発行したセントラル
characteristic   ノーティフィケーションまたはインディケーションが有効になったキャラクタリスティクス。

このメソッドは、セントラルがキャラクタリスティクスに、ノーティフィケーションまたはインディケーションを設定した時に呼び出される。
これは、キャラクタリスティクスの値が変化した時に更新を送信するキューとして使われるべきで。

- (void)peripheralManager:(CBPeripheralManager *)peripheral central:(CBCentral *)central didSubscribeToCharacteristic:(CBCharacteristic *)characteristic;

==== peripheralManager:central:didUnsubscribeFromCharacteristic:
peripheral       The peripheral manager providing this update.
central          コマンドを発行したセントラル。
characteristic   インディケーションまたはノーティフィケーションが不可にされたキャラクタリスティクス。

セントラルがキャラクタリスティクスから、ノーティフィケーションまたはインディケーションをを削除した時に呼び出される。

- (void)peripheralManager:(CBPeripheralManager *)peripheral central:(CBCentral *)central didUnsubscribeFromCharacteristic:(CBCharacteristic *)characteristic;

==== peripheralManager:didReceiveReadRequest:
peripheral   The peripheral manager requesting this information.
request      A <code>CBATTRequest</code> object.

このメソッドは、peripheralが、値がダイナミックな、キャラクタリスティクスへのATTリクエストを受信した時に呼び出される。
このメソッドが呼び出されるたび、respondToRequest:withResult:が呼び出される。
@see                CBATTRequest

- (void)peripheralManager:(CBPeripheralManager *)peripheral didReceiveReadRequest:(CBATTRequest *)request;

==== peripheralManager:didReceiveWriteRequests:
peripheral   The peripheral manager requesting this information.
requests     CBATTRequestのインスタンスの配列

このメソッドは、値がダイナミックな、1つもしくはそれ以上のキャラクタリスティクスに、peripheralがATTはリクエストもしくはコマンドを受信した時に呼び出される。
正確に一度だけ
respondToRequest:withResult:
が呼びされるべきである。
requestが複数のリクエストを含んでいれば、それらはアトミックなユニットとして扱われるべきである。
それらのリクエストの1つの実行が失敗を引き起こしたならば、リクエストとエラーの理由は
respondToRequest:withResult:に供給されて、1つのリクエストも実行されるべきではない。
@see                CBATTRequest

- (void)peripheralManager:(CBPeripheralManager *)peripheral didReceiveWriteRequests:(NSArray *)requests;

==== peripheralManagerIsReadyToUpdateSubscribers:
peripheral   The peripheral manager providing this update.

このメソッドは
updateValue:forCharacteristic:onSubscribedCentrals:の呼び出しに失敗した後に呼び出される。
peripheralがキャラクタの値更新を再び送信準備できるときに。

- (void)peripheralManagerIsReadyToUpdateSubscribers:(CBPeripheralManager *)peripheral;

=== CBATTRequest Class
*iOS6以降で有効です。*
CBATTRequestクラスは、セントラルからの読み出し/書き込み要求を表します。

==== プロパティ
==== @property(readonly, retain, nonatomic) CBCentral *central;
リクエストを発生させたセントラルです。

==== @property(readonly, retain, nonatomic) CBCharacteristic *characteristic;
値を読み書きするcharacteristicです。

/*!
 *  @property offset
 *
 *  @discussion The zero-based index of the first byte for the read or write.
 *
 */
==== @property(readonly, nonatomic) NSUInteger offset;
0から始まる、読み書きする最初のバイト位置です。

==== @property(readwrite, copy) NSData *value;
読み書きするデータです。
読み出し要求では、value はnilで、respondToRequest:withResult: に返信する前に設定されるべきです。
書き込み要求では、valueは書き込まれるべき値を含んでいます。
=== CoreBluetoothConstants
これはApple社のドキュメント
[http://developer.apple.com/library/ios/#documentation/CoreBluetooth/Reference/CoreBluetooth_Constants/](http://developer.apple.com/library/ios/#documentation/CoreBluetooth/Reference/CoreBluetooth_Constants/)
からCoreBluetoothフレームワークを理解するために必要最小限の部分のみを日本語訳したものです。

==== CBError.h

Bluetoth low energyのトランザクションで生じうるエラーです。

```
enum {
   CBATTErrorInvalidHandle = 0x01,
   CBATTErrorReadNotPermitted = 0x02,
   CBATTErrorWriteNotPermitted = 0x03,
   CBATTErrorInvalidPdu = 0x04,
   CBATTErrorInsufficientAuthentication = 0x05,
   CBATTErrorRequestNotSupported = 0x06,
   CBATTErrorInvalidOffset = 0x07,
   CBATTErrorInsufficientAuthorization = 0x08,
   CBATTErrorPrepareQueueFull = 0x09,
   CBATTErrorAttributeNotFound = 0x0A,
   CBATTErrorAttributeNotLong = 0x0B,
   CBATTErrorInsufficientEncryptionKeySize = 0x0C,
   CBATTErrorInvalidAttributeValueLength = 0x0D,
   CBATTErrorUnlikelyError = 0x0E,
   CBATTErrorInsufficientEncryption = 0x0F,
   CBATTErrorUnsupportedGroupType = 0x10,
   CBATTErrorInsufficientResources = 0x11,
};
```

# Bluetooth Accessory Design Guidelines for Apple Products (R6) 日本語訳
これは、Apple社のBluetooth Accessory Design Guidelines for Apple Products (R6)
[https://developer.apple.com/hardwaredrivers/BluetoothDesignGuidelines.pdf](https://developer.apple.com/hardwaredrivers/BluetoothDesignGuidelines.pdf)のうち、Bluetooth Low Eneryに関連する部分を日本語訳したものです。17ページ目から20ページ目までを訳しています。

=== Bluetooth low energy

Bluetooth4.0仕様は、バッテリーリソースが限られたアクセサリをターゲットにした新しい無線通信技術、Bluetooth low energyを含みます。
もしBluetooth LEがサポートされていれば、アクセサリーはこの章のガイドラインに従うべきです。

==== Role
Bluetoothアクセサリーは、Bluetooth4.0仕様 Volume3, Part C, Section 2.2.2.3 に定義されているPeripheral role 、
もしくはSection 2.2.2.1に定義されているBroadcaster roleの、いずれかを実装すべきです。

==== Advertising Channels
Bluetoothアクセサリーは、アドバタイズのイベントの都度、すべての3つのアドバタイジング・チャネル(37, 38, および39)でアドバタイズすべきです。
Bluetooth 4.0 仕様, Volume 6, Part B, Section 4.4.2.1 を参照。

==== Advertising PDU
Bluetoothアクセサリーは、次のうちいずれか1つのアドバタイジングPDUを使うべきです:

- ADV_IND
- ADV_NOCONN_IND
- ADV_SCAN_IND

ADV_DIRECT_IND は使うべきではありません。
Bluetooth 4.0 仕様, Volume 6, Part B, Section 2.3.1 を参照。

==== Advertising Data
Bluetoothアクセサリに送信されたアドバタイジング・データは、
Bluetooth 4.0 仕様, Volume 3, Part C, Section 11 に記述されているように、
次の情報の少なくとも1つを含むべきです:

- Flags
- TX Power Level
- Local Name
- Services

例えば、もしも電力消費量を低減する必要がある、あるいはすべてのアドバタイズメント・データがアドバタイジングPDUに収まりきらなかったならば、
アクセサリは、Local Name およびTX Power levelデータをSCAN_RSP PDUに置くかもしれません。
Apple製品は、その状態によっては、常にアクティブスキャンをするとは限らないことに、注意してください。

Primary servicesは常にアドバタイジングPDUでアドバタイズされるべきです。
Secondary servicesはアドバタイズされるべきではありません。
アクセサリの主たる利用方法で重要ではないサービスは、もしもアドバタイジングPDUの領域が限られているならば、無視されるかもしれません。

アドバタイジング・データおよびSCAN_RSP PDUのスキャン・レスポンス・データは、
Bluetooth 4.0 仕様, Volume 3, Part C, Section 18 のフォーマットガイドラインに従うべきです:
長さフィールドの次に、AD TypeおよびAD Dataが続きます。

==== Advertising Interval
Bluetoothアクセサリのアドバタイジング間隔は、
それがアクセサリの発見にかかる時間と接続パフォーマンスに影響するため、
注意深く考慮されるべきです。
電源がバッテリーのアクセサリでは、そのバッテリーリソースもまた考慮すべきでしょう。

Apple製品に発見されるには、Bluetoothアクセサリは、まず最初に、すくなくとも30秒は推奨される20ミリ秒のアドバタイジング間隔を使うべきです。
もしもアクセサリが最初の30秒以内に発見されなければ、アクセサリはバッテリー電力を節約することを選択して、アドバタイジング間隔を長くするかもしれません。
Apple製品が発見する確率を上げるために、Appleはより以下のより長い間隔のいずれかを使うことを推奨します。

- 645 ミリ秒
- 768 ミリ秒
- 961 ミリ秒
- 1065 ミリ秒
- 1294 ミリ秒

注意: より長いアドバタイジング間隔は、通常、より長い発見時間と接続時間をもたらします。

==== Connection Parameters
Bluetoothアクセサリは、LE接続に使われた接続パラメータに責任があります。
アクセサリーは、
適切な時間にL2CAPコネクション・パラメータ・アップデート・リクエストを送信して、
その利用方法にとて適切な接続パラメータを要求すべきです。
詳細は、Bluetooth 4.0 仕様, Volume 3, Part A, Section 4.20 を参照してください。

もしも次の全てのルールに従っていない場合は、その接続パラメータ要求は却下されるかもしれません:

- IntervalMax *(Slve Latency +1) <= 2 秒
- Interval Min >= 20 ミリ秒
- Interval Min + 20ミリ 秒  <= Interval Max
- Slave Latency <= 4
- connSupervisionTimeout <= 6 秒
- Interval Max *(Slave Latency +1) * 3 < connSupervisionTimeout

Apple製品は、Peripheral Preferred Connection Parameters characteristic のパラメータを読んだり、利用したりしません。
Bluetooth 4.0 仕様, Volume 3, Part C, Section 12.5 を参照。

==== Privacy
Bluetoothアクセサリーは、すべての状況で、Resolvable Private Addressを解決できるべきです。
プライバシーへの懸念のため、Apple製品は
Bluetooth 4.0 仕様, Volume 3, Part C, Section 10.8
に定義されたランダムデバイスアドレスを使うでしょう。

==== Permissions
Bluetoothアクセサリーは、serviceとcharacteristicsを発見するために、
ペアリング、認証、または暗号化といった特別な許可を求めるべきではありません。
characteristicの値 または descriptorの値にアクセスするときに限り、それは特別な許可を求めるかもしれません。
Bluetooth 4.0 仕様, Volume 3, Part G, Section 8.1, 5節を参照。

==== Pairing
Bluetoothアクセサリーはペアリングを要求すべきではありません。
セキュリティの理由で、アクセサリーがCentralとbonded relationship を必要とするならば、
適切であるように、
PeripheralはATTリクエストをInsufficient Authenticaion error codeで却下するでしょう。
Bluetooth 4.0 仕様, Volume 3, Part F, Section 4 を参照。

結果として、Apple製品は必要なセキュリティ手順を進めるでしょう。

ペアリングは、Apple製品次第で、ユーザの認証を要求するでしょう。

==== Services
==== Generic Access Profile Service
BluetoothアクセサリーはDevice Name characteristic、
Bluetooth 4.0 仕様, Volume 3, Part C, Section 12.1、
を実装すべきです。Device Name Characteristicは書き込み可能であるべきです。

==== Generic Attribute Profile Service
Bluetoothアクセサリーは、もしもそのアクセサリーが製品寿命の間にサービスを変更する能力がある場合に限り、Service Changed Characteristicを実装すべきです。

Apple製品は、
アクセサリーから前回読み込み(キャッシュされている)情報に頼ることができるかを決めるために、
Service Changed characteristicsを使います。
Bluetooth 4.0 仕様, Volume 3, Part G, Section 7.1 を参照してください。

==== Device Information Service
Bluetoothアクセサリーは、Device Information Serviceを実装すべきです。このサービスのサービスUUIDは、
アドバタイジング・データでアドバタイズされるべきではありません。
次のcharacteristicsがサポートされるべきです:

- Manufacturer Name String
- Model Number String
- Firmware Revision String
- Software Revision String

==== GATT Server
iOS6では、
iOSデバイスがBluetoothアクセサリーとして使えるように、
アプリケーションがGATTサーバにserviceやcharacteristicsを提供するかもしれません。
この章の推奨は、そのような場合のアクセサリーに適用します。

iOSデバイスは、
データベースの内容は任意の時点で変更できるので、
GAP Service Changed characteristicsを実装します。
したがって、Bluetoothアクセサリーは、
このcharacteristicsの
Characteristic Value Indication
をサポートして、indicationを受信したときは、そのデータベースの対応するキャッシュを無効にします。
Bluetooth 4.0 仕様, Volume 3, Part G, Section 7.1 を参照してください。

Bluetoothアクセサリーは、ATT/GATTリクエストとコマンドの利用を最小に、そして必要な物だけを送信すべきです。
例えば、
アクセサリが特定のサービスを探している時に、
GATT Discover All Services は使ってはなりません。
より少ない送信時間は、より少ない電力消費と等価であり、したがって、アクセサリーとApple製品の両方にとって、よりよいパフォーマンスをもたらします。

Bluetoothアクセサリーは、いかなるエラーも扱えるように十分に頑強であるべきです。
もしも、あるサービスをもつアプリケーションがフォアグラウンドになく、かつ、バックグラウンドで実行されるように明記されていないならば、
ペアリングとCharacteristicの値の読み込み/書き込みは、失敗するかもしれません。

もしも ATT Prepare Write Request が使われたら、
全てのキューイングされた属性は同じ
GATT Service に含まれます。


# UUIDとは

UUID(Universally Unique Identifier), またはGUID (Globally Unique IDentifier) とも呼ばれる識別子は、時間と場所にかかわらず唯一であると保証できる128ビット長の識別子です。詳細な仕様は istributed Computing Environment (DCE) http://tools.ietf.org/html/rfc4122 にあります。

UUIDの特徴は、識別子の発行を管理するサーバや認証機関のような仕組みがなくとも、ローカルで生成できることです。また、ネットワーク接続がない計算能力の小さいマイクロプロセッサでも生成できます。生成するコストがとても小さく、短時間に大量に生成すること、例えば1秒に1000万個、も容易にできます。

識別子はネットワークやオペレーティング・システムなどの、様々な場面で使われます。通信のセッションの間だけ有効な一時的な識別子にも使えますし、データベースに保存されたりして永続化された情報の識別および参照にも使えます。

UUIDを使う身近な例は、Bluetooth low energy の GATTレイヤは、サービスおよびキャラクタリスティクスにUUIDを使っています。またiOSシミュレータは、シミュレーションのデバイス識別にUUIDを使っています。iOSアプリケーション開発をしているなら、ディレクトリ ~/Library/Developer/CoreSimulator/Devices を開くと UUIDのフォルダ名がいくつか並んでいます。

==== UUID の表記

UUIDは、128ビットつまり16オクテットの長さがあるので、その値を16進数にすると32文字の英数で表記できます。16進の表記には、0から9の数字、またaからfあるいはAからFまでのアルファベットを使います。アルファベットは大文字でも小文字でもどちらでもよく、また1つのUUIDで混在してもかまいません。

32文字の英数を並べただけでは、人間には読みづらいので、先頭から4オクテット、2オクテット、2オクテット、2オクテット、6オクテット、つまり英数文字で先頭から、8文字、4文字、4文字、4文字、12文字に区分して、その間をハイフン’-‘でつなげて表記します。

例えば、Bluetooth low energyで 16ビットのUUID 0x1801が割り当てられているジェネリック・アトリビュート・サービスのUUIDは、

 00001801-0000-1000-8000-00805F9B34FB

と表記されます。

UUIDのバイトオーダは、ネットワーク・バイトオーダです。ですから、UUIDの一部分をフィールドとして区切り整数として扱うときは、左がUUIDの16進表記で言えば左が最上位バイト右が最下位バイトになる、ビッグ・エンディアンになります。

ジェネリック・アトリビュート・サービスのUUIDを例に、仮にハイフンで区切られた48ビットの末尾部分を整数として扱う場合、その値は 0x00805F9B34FB になります。

<!--
あるいは OS X のターミナルでコマンド uuidgen を実行すると、生成されたUUIDが

 7F2FA9AB-9A57-49AF-A766-391D8B177D51

のように表記されます。
-->

==== UUIDのvariantとバージョン

UUIDのvariant(異体)フィールドは、過去の仕様との後方互換性を保つために定義されています。variantフィールドは、UUIDの先頭から9オクテット目の上位3ビットです。

UUIDの表記を使うと、次のようにNで示した位置にvariantフィールドがあります。このNの上位3ビットがvariantフィールドになります。表記中のxは、UUIDの長さを表現するために使った文字で、任意の値です。

 xxxxxxxx-xxxx-xxxx-Nxxx-xxxxxxxxxxxx

variantフィールドの値とその説明

MSB3| MSB2|MSB1|説明
————+————+-————+——————————————————————————————————————
0	 |*	|*	   |NCS後方互換性のために予約されてる。
1	 |0	|*	   |ここで述べるUUIDに割り当てられている。
1	 |1	|0	   |Microsoft社の後方互換性のために予約されている。
1	 |1	|1	   |将来のために予約されている。

表中の * は、任意の値、値が1でも0でもよいことを示します。したがって、ここで説明するUUIDは、Nの上位2ビットが10、つまりNの値は16新表記で8, 9, A, Bの、いずれかの値を取ります。

ここで述べるUUIDには、5つのバージョンがあります。

バージョン番号   |記述
-———————————---+——————----------------------
1  | 時間を基にするバージョン。
2  | DCEセキュリティバージョン。
3  | MD5ハッシュを使う、名前を基にするバージョン。
4  | ランダムもしくは擬似乱数を基にするバージョン。
5  | SHA-1ハッシュを使う、名前を基にするバージョン。

このバージョン番号は、先頭から7オクテット目の上位4ビットに割り当てられています。variantフィールドと同じように、UUIDの表記を使うと、次のようにMで示した位置の16進表記がバージョン番号になります。

 xxxxxxxx-xxxx-Mxxx-xxxx-xxxxxxxxxxxx

Nil UUIDは、次の表記になる128ビットがすべてゼロのUUIDです。

 00000000-0000-0000-0000-000000000000

またUUIDの値の正しさを検証する方法はありません。UUIDは一致するかしないかを判定するだけに使う識別子です。

UUIDを、セキュリティに関わる用途に使うべきではありません。例えば、通信を開始する都度、乱数を基にUUIDを生成して、それを使い捨てのアクセスを許す識別子に使ったとします。

アクセス権限がない人が、次に発行されるアクセスを許すUUIDの値を推測して、不正にアクセス権限を得ようとしているとします。その乱数の品質が高ければ、次に発行されるUUIDの推測は困難です。ですが、もしもアルゴリズムがわかっていれば次に出る乱数がわかる擬似乱数を使っていれば、次に発行するUUIDは容易に推測されてしまいます。

==== 時間を基にしたUUID

バージョン1もしくは2は、時刻を基にしたUUIDです。このUUIDは、グレゴリオ暦での1582年10月15日午前0時0分を基準点にした100ナノ秒単位の値に、その識別子を発行するノードを識別する値を組み合わせて生成します。タイムスタンプは、100ナノ秒単位の60ビットの値なので、3600年ほどでロールオーバーします。

バージョン1もしくは2のUUIDは、次のように表記できます。

 xxxxxxxx-xxxx-1xxx-Nxxx-KKKKKKKKKKKK (バージョン1)
 xxxxxxxx-xxxx-2xxx-Nxxx-KKKKKKKKKKKK (バージョン2)

xはタイムスタンプの値です。先頭から7オクテット目の値1は、バージョン1を示します。バージョン2では、この値が2になります。variantフィールドを含むNは、16進で 8, 9, A, B のいずれかの値をとります。48ビットのKはノードの識別子を示します。

バージョン2は、ノードの識別子に、POSIXのユーザ識別子 UID (User Identifier)を使います。

バージョン1では、ノードの識別子にネットワーク・カードがもつMACアドレスが使えます。

MACアドレス(media access control address )は、イーサネットや無線LANなどの物理層でパケットをやりとりするときに使われるアドレスです。同じMACアドレスをもつ製品が存在しないように、事業者ごとに固有識別子を割り当てる登録制度で、MACアドレスの割り当ては管理されています。

ネットワークに接続する機器は、たいていMACアドレスを取得できます。バージョン1のUUIDは、その固定値のMACアドレスと時刻を組み合わせるだけで唯一性が確保されたUUIDを生成できます。

バージョン1は、ノード識別子に乱数または擬似乱数を使うこともできます。乱数が既存のネットワークのMACアドレスと衝突することを避けるために、ネットワークカードでは決して使われることがない、MACアドレスの最下位オクテットの最下位ビットの unicast/multicastビット を'1'に設定します。

つまり、47ビットの乱数にビット'1'を追加した48ビットの値を、ノードの識別子とします。

==== 名前および乱数を基にしたUUID

バージョン3およびバージョン5は、名前を元にしたUUIDです。UUIDは次のように表記できます。

 xxxxxxxx-xxxx-3xxx-Nxxx-xxxxxxxxxxxx (バージョン3)
 xxxxxxxx-xxxx-5xxx-Nxxx-xxxxxxxxxxxx (バージョン5)

バージョン番号およびvariantフィールドを除く部分は、名前を基にしたハッシュ値を代入します。この表記では、x部分およびNの最下位ビットにハッシュ値が代入されます。

ハッシュ関数には、バージョン3はMD5、バージョン5はSHA-1を使います。後方互換性などの理由がないならば、SHA-1を使うバージョン5を採用することを勧めます。

ハッシュ値は、RFC4122にある名前空間に割り当てられたUUIDと、その名前空間での識別子を連結して計算します。名前空間は、例えば完全修飾ドメイン名(fully-qualified domain name)やURLなどです。

バージョン3および5は、時間が関係しない、永続化されるデータやネットワーク上のリソースを示す識別子に使えます。

また乱数を基にUUIDを発行できます。次のUUIDの表記の、x部分およびNの最下位ビットに、乱数あるいは擬似乱数を代入して生成します。

 xxxxxxxx-xxxx-4xxx-Nxxx-xxxxxxxxxxxx (バージョン4)

他のバージョンと異なり、121ビットの乱数を基にするので、衝突する可能性はとても低いですがゼロとはいえません。ですから、品質の高い乱数を使うことが推奨されます。

==== カスタム・プロファイルとUUID

カスタム・プロファイルを実装するために、サービスおよびキャラクタリスティクスに割り当てるUUIDの生成方法をまとめます。

16ビットおよび32ビットのUUIDは、Bluetooth SIGが管理しています。Bluetooth SIGのメンバーは、メンバーが定義したカスタムGATT基盤サービスで使うために、申請をして費用を支払うことで、16ビットのUUIDの割り当てを受けられます。

Bluetooth SIGの管理をうけないならば、128ビットのUUIDを使うほかありません。他社製品とUUIDが重複しないかが気になります。仮に乱数を基にしたUUIDを使って、ビット数が十分大きいので、故意でない限りUUIDが重複することはまずありません。

開発では、ぱっとみてわかりやすいUUIDが使いやすいです。Bluetooth SIGが行っているように、バージョン1のUUIDを使うと、わかりやすいUUIDの割り当てができます。

もしもMACアドレス・ブロックに登録しているなら、自社で管理したMACアドレスを使えばいいでしょう。アドレス・ブロックを持たないならば、最下位ビットを1にした乱数を使えばいいでしょう。

#@# UUIDをデータベースのキーに使えるようにとver6というのが提案中。らしい。
#@# https://tools.ietf.org/html/draft-peabody-dispatch-new-uuid-format-00
