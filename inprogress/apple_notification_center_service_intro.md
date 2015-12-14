# Apple Notification Center Service イントロダクション #

## iOS6からiOS7へ
Appleは、Bluetooth LEにいち早く対応し、またその利用領域を毎年拡大しています。

2011年に世界で初めてBluetooth Low Energyに対応したiPhone4sと同時に公開されたiOS5は、CoreBluetoothフレームワークで一般アプリ開発者に、Bluetooth LE周辺機器との接続を開放しました。翌年のiOS6は、セントラル・ロールに加えて、ペリフェラル・ロールが追加されました。

iOS5とiOS6は、個別のアプリケーションとBluetooth LE機器との接続だけでしたが、iOS7からは、iOS自体がBluetooth LE機器およびBluetooth LEを利用したサービスに対応しはじめました。

iOS7から提供されたiOSが担当するBluetooth LEの機能は:

- Apple Notification Center Service (ANCS)
- iBeacon
- Gameコントローラ (MFi)
- HID over BLE(キーボードなどの入力装置)

です。今回はこの内ANCSについて解説します。

## Apple Notification Center Serviceの目的
The Apple Notification Center Service (ANCS)の目的は、Bluetooth LEでiOSデバイスに接続するBluetoothアクセサリーに、iOSが出力する様々な通知(メールやFacebookなどの新着、電話着信など)にアクセスする勘弁な方法を提供することです。

ANCSの設計原則は、Simplicity, Efficienty そして Scalabilityの3つです。LEDが光るだけの簡単なアクセサリから、液晶表示器がある強力な周辺機器まで、様々な周辺デバイスにサービスを提供します。

ANCSは、Bluetooth LEの規格にあるGATTを使います。特定の、閉じた技術は使っていません。GATTクライアントとして振る舞う周辺機器は、iOSデバイスが提供するANCSを通して、通知サービスの発見と接続そしてアクセスができます。

### 用語など
明記しな限り、GATTのパケットの数値はリトル・エンディアンで表現します。文字列の文字符号化方式は、UnicodeおよびUTF-8です。

- The Apple Notification Center Service (ANCS)
- Notification Provider (NP): ANCSサービスを公開するもの(iOSデバイス)
- Notification Consumer (NC): のクライアント (アクセサリ)
- iOS notification : iOS ノーティフィケーション・センターがiOSデバイスに表示するノーティフィケーション
- GATT notification : GATT キャラクタリスティクスで送られる、ノーティフィケーション、非同期のメッセージとして

## ANCSでできること、使いかた

## 公開されるサービスとプロファイル
## サンプルアプリ


サービスUUIDは、7905F431-B5CE-4E99-A40F-4B1E122D00D0。

ANCSのインスタンスは、NPに1つだけ。ANCSが常にあるとは限らない。GATTサービスの、Service Changed characteristicを探してサブスクライブすべし、NCは。いつ何時、公開/非公開になるかわからないから、それらをモニタするために。

## キャラクタリスティクス
ANCSが公開するキャラクタリスティクス:


- Notification Source: UUID 9FBF120D-6301-42D9-8C58-25E699A21DBD (notifiable)
- Control Point: UUID 69D1D8F3-45E1-49A8-9821-9BBDFDAAD9D9 (writeable with response)
- Data Source: UUID 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB (notifiable)

Notification Source characteristicのサポートは必須。
the Control Point characteristic and Data Source characteristic はオプション。

この3つ以外のキャラクタリスティクスがANCSにあるかもしれない。いわば、NCは認識できないキャラクタリスティクスは無視していい。

### Notification Source (notifiable)
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

## Control Point and Data Source
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

### Get App Attributes
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

### セッション
ANCSセッションは、NCがNPのNotification Sourceキャラクタリスティクスをサブスクライブしだして、
NCが同じキャラクタリスティクスをアンサブスクライブするか、NPと切断するかで終了する。
ANCSは完全な同期サービスとして設計されてはいないため、セッションをまたいだ状態の維持はしない。
その結果として、全ての識別子(NotificationUIDおよびAppIdentifier)そしてNCとNPの間で交換される全てのデータは、特定のセッションでのみ有効である。

特定のセッションが終了した時、NCはセッション内で集めて保管した全ての識別子とデータを削除すべきである。
新しいセッションが開始したならば、NPはNCに、システムにある全てのiOSノーティフィケーションを知らせるべく最善を尽くす。
NCは、セッションのリマインダを使うためのモデルを構築する情報として使う。

## アトリビュートの取得とキャッシュ
NCには、ユーザの行動に反応するためのもの、そして必要な物だけで、属性を取得することを強く推奨する。
例えば、NCがiOSノーティフィケーションを簡単なリストで表示することを選択したなら、ユーザが選択したときに特定のiOSノーティフィケーションの詳細を表示するなら、iOSノーティフィケーションのアトリビュートの取得は、レイジーにトリガされるだろう。

セッションの間、NCはapp識別子それぞれのApp Attributeのキャッシュを構築すること、遭遇した、を推奨。
このキャッシュの構築は、変化しないAPp AttributeをNCが複数回要求することを避けて、時間とバッテリーの節約になる。

## Error Codes
Control Point characteristicに書き込むとき、NCは次のANCS特有のエラーコードを受け取るかもしれない。

Unknown command (0xA0): commandIDがNPによって認識されない。

Invalid command (0xA1): コマンドのフォーマットが不正。

Invalid parameter (0xA2): いずれかのパラメータ(例えば NotificationUID)がNPの既存のオブジェクトを参照していない。もしもNPがこのエラーを返したならば、対応するコマンドへのGATT notificationをData Source characteristicに生成しない。

## Example
TBD


# 参考文献
https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Specification/Specification.html#//apple_ref/doc/uid/TP40013460-CH1-SW7

そいえば、ANCS対応の周辺装置を実装するときは、Service changed、の実装は必須。iOSってアプリの起動時とか、iOSアプリがサービスを追加、削除するたび、ローカルデータベースが変化するから。
posted at 09:37:47
   
メールとかの通知をBLEでやるANCS、あのサービスはiOSが提供するから、iOSアプリ側にはフィルタされて見えない。だから、LightBlueとかのiOSアプリでは、見えないのよ。自分でHCI叩くか、iOS意外の装置開発環境でやるしかない

ANCSはCBPeripheralをアプリでアドバして、とりあえずアクセサリが接続すればサービスが取れる。iOS7はiOSがフィルタするからアプリで模擬できないが、iOS6なら、アプリから見える。


## 実際にやってみると、詰まった
iOS6でサービス指定でスキャンしたけど、繋がらない。デバイス名指定でOK。サービスまでスキャンしないのかしら。


