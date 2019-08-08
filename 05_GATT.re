= ATT, GATT

== Attribute Protocol と Generic Attribute Profile

Attribute Protocol (アトリビュート・プロトコル、ATT)は、ATTはアドレスとラベルつきの値の集合をやりとりするプロトコルです。ATTは、L2CAPまでで確保された接続したデバイス間の通信を使い、 Bluetooth low energy の特徴的な通信の機能や振る舞いを提供しています。

ATTの上位層にGeneric Attribute Profile(ジェネリック・アトリビュート・プロファイル、GATT)があります。このATTとGATTは、Bluetoothに統合される以前の規格では1つの層でした。これが、規格に取り込む際に、通信の手順を表すプロトコル、ATT、と機器の振る舞いを表すプロファイル、GATT、の2つの層に分離されました。ATTは、ATT単体ではなく、 Bluetooth low energy の設計方針およびATTの上位層のとあわせて理解すると、 Bluetooth low energy でのATTの意味がよくわかります。

=== ATTとGATTの特徴

ATTとGATTの特徴は:

- すべてのアプリケーション使うシンプルな1つのプロトコル
- 通信と振る舞いを分離
- Bluetoothの認証の範囲で多種多様な周辺機器の開発を可能にする

です。

クラシックBluetoothは、メーカや機種を超えた相互運用を提供するために、通信プロトコルやデータ形式などの通信部分のみならず機器の振る舞いまでを含めたものを、プロファイルと定義します。例えばヘッドセットやファイル交換などの、アプリケーションごとにプロファイルが定義されます。Bluetooth SIGがプロファイルを定義していないアプリケーションの開発には、シリアル・ポート・プロファイルという、汎用通信のプロファイルを使います。この場合、通信プロトコルなどは、自ら開発しなければなりません。

 Bluetooth low energy では、GATTの上層に、すべてのアプリケーションが作られます。GATTが通信の振る舞いを、アプリケーションが機器の振る舞いを、それぞれ担当するように層が分けられています。そして、 Bluetooth low energy では、Bluetoothの認証範囲はGATTまでで、アプリケーションは含みません。したがって、 Bluetooth low energy は、Bluetoothのロゴ認証のもとで、多種多様なアプリケーション、つまり周辺機器、を一般に販売できます。

 Bluetooth low energy は、機器の振る舞いをプロファイルと呼びます。例えば心拍や血圧計など、メーカや機種を超えた相互運用が一般普及のために必要不可欠な分野があります。 Bluetooth low energy のプロファイルは、それぞれの分野で業界団体あるいは会社群がプロファイルを作成して、それをBluetooth SIGが承認することで成立します。GATTとアプリケーションが分かれているので、GATTというプロトコルはそのままで、分野ごとに多種多様なプロファイルが、必要になるたびに追加されます。

=== アトリビュート・プロトコル

Attribute Protocol(アトリビュート・プロトコル、ATT)は、アトリビュート・サーバ(ATTサーバ)とアトリビュート・クライアント(ATTクライアント)の通信プロトコルです。

ここで、アトリビュートは、アドレスとタイプでタグ付けされた値のことです。ATTサーバは、アトリビュートの集合を公開するものをいいます。ATTクライアントは、サーバが公開するアトリビュートを参照するものをいいます。サーバのアトリビュートは、クライアントから、発見、読み出し、書き込みができます。またサーバからクライアントに、インディケーション(indication)またはノーティフィケーション(notification)ができます。

==== アトリビュート

サーバが公開するアトリビュートの、サーバ内での論理表現は、アトリビュート・ハンドル (attribute handle)、アトリビュート・タイプ(attribute type)そしてアトリビュート・バリュー(attribute value)、アトリビュート・パーミッション(attribut permission)の4つで構成されます。

![ #fig_attribute_format アトリビュートのフォーマット](fig/ch02_attribute_format.png)

アトリビュート・ハンドルは、ATTクライアントがATTサーバのアトリビュートにアクセスするときに使われます。アトリビュート・ハンドルは、2オクテットの0x0001から0xffffまでの値です。

アトリビュート・タイプは、アトリビュートのデータの型を示します。これは128ビット(16オクテット)のUUID (Universally Unique IDentifier)
[RFC 4122](@<href>{http://tools.ietf.org/html/rfc4122)} [^2070] です。しかし、128ビットのUUIDそのままを通信でやりとりすると、通信時間が長くなり電力を使います。そこで、Bluetooth SIGが、よく使うものには16ビット(2オクテット)のUUIDを割り当てています [Assigned Numbers](@<href>{https://www.bluetooth.org/ja-jp/specification/assigned-numbers) [^2080]} 。この16ビットのUUIDは、次の128ビットのUUIDのxxxx部分を抜き出した短縮したUUIDです。

[^2070]: [RFC 4122 @<href>{http://tools.ietf.org/html/rfc4122](http://tools.ietf.org/html/rfc4122)}
[^2080]: [Assigned Numbers @<href>{https://www.bluetooth.org/ja-jp/specification/assigned-numbers](https://www.bluetooth.org/ja-jp/specification/assigned-numbers)}

	0000xxxx-0000-1000-8000-00805F9B34FB

16ビットのUUIDを128ビットのUUIDに戻すには、この128ビットのUUIDのxxxx部分を16ビットのUUIDで置き換えます。例えば 0x1801 ならば:

	00001801-0000-1000-8000-00805F9B34FB

となります。

アトリビュート・バリューは0から512バイトまでの任意のバイト・データです。エンディアンは上位層の仕様が決めます。

アトリビュート・パーミッションは、アトリビュート・プロトコルで読み出すことはできません。上位層がどのアトリビュートが読み書きできるかを決めます。アトリビュート・プロトコルで、読み込みのみのアトリビュートに書き込みのリクエストをすれば、パーミッションがないので書き込めないとうエラー・レスポンスが返ってくるので、間接的にパーミッションを知ることができます。

==== リクエストとインディケーション

アトリビュート・プロトコルのアトリビュート・プロトコルPDU (Protocol Data Unit)のやりとりには、リクエスト、インディケーション、コマンド、そしてノーティフィケーションの4つがあります。
  たいていのアトリビュート・プロトコルPDUは、図(a)の、シーケンシャルなリクエスト-レスポンス・プロトコルを使います。

![ #fig_attribute_protocol アトリビュート・プロトコル](fig/ch02_attribute_protocol.png)

クライアントからサーバにリクエストするときには、アトリビュート・リクエスト(ATTリクエスト)とアトリビュート・レスポンス(ATTレスポンス)のペアを、サーバからクライアントに通知するにはアトリビュート・インディケーション(ATTインディケーション)とアトリビュート・コンファメーション(ATTコンファメーション)のペアを使います。これらのやりとりをトランザクションと呼びます。

リクエストおよびインディケーションには、正常に処理できたならば、それぞれレスポンスもしくはコンファメーションが返されます。もしもエラーが生じた場合は、エラーメッセージが返されます。

トランザクションの期間は、ATTリクエストおよびATTレスポンスのペアでは、クライアントではリクエストの送信からサーバからのレスポンスを受信するまで、サーバではリクエストを受信してからレスポンスを送信し終えるまで、です。ATTインディケーションおよびコンファメーションのペアでは、サーバはインディケーションを送信してからコンファメーションを受信するまで、クライアントはインディケーションを受信してからコンファメーションを送信し終えるまで、です。

トランザクションが使えると、サーバとクライアントの間での処理要求や通知を確実に実行できます。ですが、トランザクションの排他処理や、トランザクションの失敗検出とその後処理をどうするのかを決める必要が生じます。

サーバおよびクライアントが実行するトランザクションは常に1つだけです。クライアントは、あるサーバにリクエストを送信したら、そのサーバからのレスポンスを受信するまで、同じサーバにリクエストは送りません。また、サーバがクライアントにインディケーションを送信した場合も、コンファメーションを受診するまで、インディケーションを送信しません。

リクエスト処理が完了するまでクライアントは待つので、フロー制御が得られます。また、サーバのリクエスト処理には、マイクロコントローラの計算時間や外部センサ回路を動かす処理などが必要かもしれません。もしもトランザクションが複数だと、資源の管理やロック機構が必須になります。トランザクションを1つにしているので、サーバにそのような仕組みを持たせずにすみます。

30秒以内にトランザクションが完了しないときは、タイムアウトとみなし、そのトランザクションは失敗とされます。上位層はトランザクション失敗を通知されます。トランザクションが失敗すると、それ以上アトリビュートPDUはやりとりされません。アトリビュートPDUのやりとりを再開するには、接続の切断と再接続をします。

トランザクションの途中で通信が切断した場合は、そのトランザクションは閉じられます。そのトランザクションがサーバの値を変更するものだった場合は、サーバの値がどうなるかは不定です。その時の振る舞いは、上位層の仕様で決めておきます。

サーバのアトリビュート・バリューのサイズは最大で512オクテットです。サイズが1つのアトリビュートPDUに収まらないアトリビュート・バリューの読み書きは、複数回のトランザクションで処理します。このために、アトリビュート・プロトコルには、オフセットを指定できるRead BlogリクエストとPrepare Writeリクエストが用意されています。もしも一連のトランザクションが、途中で失敗したり接続が切断した場合はサーバの値は不定になります。この時の振る舞いは、上位層の仕様で決めておきます。

==== コマンドとノーティフィケーション

アトリビュート・プロトコルには、クライアントからサーバへのアトリビュート・コマンド(ATTコマンド)、そしてサーバからクライアントへのアトリビュート・ノーティフィケーション(ATTノーティフィケーション)があります。
これらはリクエスト/インディケーションと異なり、トランザクションもフロー制御もありません。トランザクションと無関係に、任意のタイミングで任意の回数の送信ができます。

例えば、クライアントがATTリクエストを送信して、まだレスポンスを受信していない時、トランザクションが終了していない時でも、クライアントはサーバにATTコマンドを送信できます。また、サーバも同様にトランザクションの途中でも、クライアントにATTノーティフィケーションを送信できます。

ATTコマンドとATTノーティフィケーションは、任意のタイミングに送信できるので、フロー制御はありません。リンク層の機能で、サーバおよびクライアントは、ATTコマンドおよびATTノーティフィケーションを確実に受信しますが、メモリ容量や計算時間が不足するなどした場合は、それらの処理を無視します。ATTコマンドおよびノーティフィケーションは、処理されるかが信頼できないもの、として扱います。

フロー制御がありませんから、例えば、クライアントから大量のコマンドがサーバに送りつけられるかもしれません。このようなことにならないように、上位層の仕様を決めておきます。

クライアントからのATTコマンドに対して、サーバが何かを返信したい場合、直ちに返信する必要はありません。トランザクションはないので、タイムアウトはありません。サーバは、ATTインディケーションかATTインディケーションノーティフィケーションで、そのうち通知をすればよいのです。

==== アトリビュートPDUフォーマット

アトリビュートPDFは、アトリビュート・オペコード(Attribute opcode)、アトリビュート・パラメータ、オーセンティケーション・シグネチャ(Authentication signature)が続きます。
図のATT\_MTUは、アトリビュートPDUの最大長 Maximum Transfer Unit(MTU)を表し、デフォルト値は23オクテットです。

![ #fig_attribute_pdu_format アトリビュート PDUフォーマット](fig/ch02_attribute_pdu_format.png)

アトリビュート・オペコードは、オーセンティケーション・シグネチャ・フラグ(Authentication signature flag)、コマンド・フラグ(Command flag)そしてメソッド(Method)から構成されます。コマンド・フラグが1ならば、そのPDUはコマンドです。6ビットのメソッドにより、アトリビュート・パラメータのフォーマットとその意味が決まります。

また、オーセンティケーション・シグネチャ・フラグが1ならば、オーセンティケーション・シグネチャがあります。

オーセンティケーション・シグネチャは、通信の暗号化は必要ではないが、サーバの状態を変化させてしまう書き込み処理は、信頼できるクライアントからのみ受け付けたい場合に、用います。リンク層の機能を使い、通信自体を暗号化すれば、スニッフィングも防止できますが、しかしパケットのオーバーヘッドの増加と暗号化処理が必要になります。サーバはシグネチャから、アトリビュートPDUが信頼できるクライアントが送信したものだと、確認ができます。

このような用途なので、オーセンティケーション・シグネチャ・フラグが1になるのは、Writeコマンドのみです。また、リンク層で暗号化をしているときは、必要がないので、オーセンティケーション・シグネチャ・フラグは1にはできません。

==== ロング・アトリビュートとその扱い

長さが(ATT\_MTU–1)オクテットを超えるアトリビュート・バリューをもつアトリビュートを、ロング・アトリビュート(long attribute)と呼びます。サーバのアトリビュート・バリューの最大長は512バイトですが、ATT\_MTUのデフォルト値は23オクテットです。たいていATT\_MTUはデフォルト値がつかわれますから、23オクテット以上のアトリビュート・バリューはたいていロング・アトリビュートです。

アトリビュート・プロトコルで、トランザクションで送信できるアトリビュート・バリューの最大長は(ATT\_MTU–1)オクテットです。アトリビュート・プロトコルのリード/ライト・リクエストは、ロング・アトリビュートであっても、それを自動で分割/再結合したりはしません。その場合は、冒頭(ATT\_MTU–1)オクテットだけをやりとりします。

ロング・アトリビュートの読み書きに、オフセット値を指定できるRead Blogリクエスト、Prepare Write/Execute Writeリクエストがあります。上位層は、これらのリクエストを使って、ロング・アトリビュートを、(ATT\_MTU-1)オクテット単位で複数回のトランザクションに分割して、扱います。

===== Exchange MTUリクエスト/レスポンス

アトリビュート・プロトコルのATT\_MTUを変更するのが、Exchange MTU リクエスト/リスポンスです。このリクエストで、クライアントはサーバに、クライアントが扱えるATT\_MTUを伝えます。サーバは、レスポンスで、サーバが扱えるATT\_MTUを返します。このときのATT\_MTUの値は、ATT\_MTUのデフォルト値23以上の値です。

もしも、サーバにメモリが少ないマイクロコントローラが使われていると、ATT\_MTUは大きく取れないかもしれません。サーバはその場合、ATT\_MTUの最小値でもある、デフォルト値23を返すでしょう。

==== アトリビュートへのアクセス

サーバが公開しているアトリビュートには次の6つのアクセス方法があります。

- ファインド・リクエスト (Find request)
- リード・リクエスト (Read request)
- ライト・リクエスト (Write request)
- ライト・コマンド (Write command)
- インディケーション (Indication)
- ノーティフィケーション (Notification)

==== ファインド・リクエスト

ファインド・リクエストは、クライアントが、サーバのアトリビュートの集合から指定条件に一致するアトリビュート・ハンドルを取得するのに、使います。サーバのアトリビュートをすべて読み出そうとすると、通信に時間がかかります。条件を指定することで、必要なアトリビュートのハンドルのみを素早く取得できます。ファインド・リクエストは、Find InformationとFind By Type Valueの2つがあります:

- Find Information
- Find By Type Value

Find Informationでは、アトリビュート・ハンドルの範囲を指定して、アトリビュート・ハンドルとアトリビュート・タイプを読み出します。クライアントは、得られたアトリビュート・タイプの配列から、サーバのデータベースの構造を知ることができます。

クライアントがサーバに送るFind Information リクエストは、取得したいアトリビュート・ハンドルの開始値から終了値を指定します。サーバは、Find Informationレスポンスで、アトリビュート・ハンドルとアトリビュート・タイプの配列を返します。

例えば、クライアントがサーバのATTハンドルとATTタイプを全て読み出したい時は、ATTアトリビュートの範囲を0x0001から0xFFFFに設定したFind Informationリクエストを送ります。このとき、Find Informationレスポンスが、0x0001から0x0004までのアトリビュート・ハンドルを返してきたとします。まだ0x0005以降のアトリビュート・ハンドルがあるかもしれません。そこでクライアントは、アトリビュート・ハンドルの範囲を0x0005から0xFFFFに設定したFind Informationリクエストを送ります。クライアントは、これを繰り返して、すべてのアトリビュートを読み出します。該当するアトリビュートがなければ、サーバはクライアントに、エラー・コード Attribute not found のエラー・レスポンスを返します。

Find Informationレスポンスは、アトリビュート・タイプのUUIDが、16ビットか128ビットかを示すフラグと、アトリビュート・ハンドルとアトリビュート・タイプの組の配列を返します。1つのアトリビュート・ハンドルとアトリビュート・タイプの組は、アトリビュート・ハンドルは2オクテットなので、16ビットUUIDならば4オクテット、128ビットUUIDならば18オクテットを占めます。

Find Informationレスポンスは、16ビットと128ビットのアトリビュート・タイプを混在させることはできません。ハンドルとタイプの組の配列には、最大 (ATT\_MTU -2) オクテットつかえます。ですから、 1つのFind Informationレスポンスで送れるハンドルとタイプの組は、ATT_MTUがデフォルト値23のとき、16ビットUUIDならば5つ ((23-2)/4 = 5.3)まで、128ビットUUIDならば1つ((23-2)/18=1.2)です。

Find By Type Valueは、使い方はFind Informationと同じで、アトリビュートのタイプおよびバリューが指定できる点が異なります。ただし、指定できるアトリビュート・タイプは16ビットのものだけで、アトリビュート・バリューの長さは(ATT\_MTU -7)オクテットまでの制限があります。Find By Type Valueは、GATTのサービスおよびキャラクタリスティクスの素早い検索に役立ちます。

==== リード・リクエスト

クライアントがサーバのアトリビュートを読み出すリクエストは、6つあります。Readは1つのハンドルを指定する1つのアトリビュート・バリューの単純な読み込みを、Read Mutipleは複数のハンドルを指定しての読み込みを、Read Blobは長さが(ATT_MTU-1)を超えるアトリビュート・バリューの読み出しを行います。Read By TypeおよびRead By Group Typeは、アトリビュート・タイプを指定した読み出しです。

- Read
- Read Multiple
- Read Blob
- Read By Type
- Read By Group Type

クライアントが送信するアトリビュート・ハンドルを指定したReadリクエストに、サーバはReadレスポンスを返します。レスポンスのアトリビュート・バリューの長さは最大(ATT\_MTU-1)、MTUがデフォルト値23のとき22オクテット、です。

Read Multipleリクエストは、複数のアトリビュート・ハンドルを指定して、1回のトランザクションで読み出します。サーバはアトリビュート・バリューを連結したものをレスポンスで返します。このアトリビュート・バリューの長さは最大で(ATT\_MTU-1)オクテットです。もしもアトリビュート・バリューを連結したものが、この長さよりも長い場合は、先頭の(ATT\_MTU-1)オクテットが返されます。またレスポンスには、それぞれのアトリビュート・バリューの境界を示さないので、Read Multipleリクエストはアトリビュート・バリューが固定長のものに限られます。ただし、最後の要素は可変長でもかまいません。

ReadリクエストおよびRead Multipleリクエストは、アトリビュート・バリューの長さが(ATT\_MTU-1)オクテットよりも短いものを読み出せます。バリューが短いものをRead Multipleリクエストでまとめて読みだすことで、Readリクエストを使うよりも、リクエストの回数を小さくできます。

Read Blobリクエストは、(ATT\_MTU-1)オクテットよりも長いアトリビュート・バリューの読み出しに使います。Readリクエストとの違いは、2オクテットのオフセット値を指定できることです。レスポンスは、オフセット位置から(ATT\_MTU-1)オクテットまでのアトリビュート・バリューを返します。

Read By Type リクエストは、アトリビュート・ハンドルはわからないが、アトリビュート・タイプがわかっている場合の、読み出しに使います。動作は、Find By Type Valueリクエストでアトリビュート・ハンドルを取得してから、Readリクエストで読み出すことと同じです。

Read By Group Type リクエストは、Read By Typeリクエストとほぼ同じですが、グループ・タイプアトリビュートの一連のアトリビュートの末尾のアトリビュート・ハンドラを返す点が異なります。

上位層が提供する概念に、アトリビュートのグループ化があります。これは、例えばC言語の構造体のように、複数のアトリビュートを1つのグループにしてデータ構造をつくることです。このために上位層は、グループをあらわすアトリビュート・タイプ、グルーピング・タイプと呼びます、を定義しています。

Read By Group Type リクエストは、グルーピング・タイプの読み出しと、そのグループの範囲を読み出すために使われます。

==== ライト・リクエスト

クライアントがサーバにアトリビュート・バリューを書き込むリクエストは5つあります。Writeリクエストは、1つのハンドルを指定した単純な書き込みを、Prepare WriteとExecute Writeは、(ATT_MTU-1)オクテットよりも長いアトリビュート・バリューの書き込みをおこないます。

- Write
- Prepare Write
- Execute Write
- Write Command
- Signed Write Command

ライト・リクエストは、サーバの不揮発メモリに保存される設定情報など、サーバが確実に処理しなければならない書き込みに使います。ライト・リクエストが書き込めるアトリビュート・バリューの長さは最大(ATT\_MTU–3)オクテット、ATT\_MTUがデフォルト値23のとき20オクテット、です。

(ATT\_MTU–3)オクテットよりも長いアトリビュート・バリューを書き込むときに、Prepareライト・リクエストとExecuteライト・リクエストを使います。

Prepareライト・リクエストで、クライアントはサーバに、オフセットとデータを送ります。この情報はサーバのキューに保持されます。Executeライト・コマンドで、クライアントがサーバに書き込みを支持すれば、キューにあるすべてのデータの書き込みが実行されます。Executeライト・コマンドを使って、クライアントはサーバのキューのクリア(書き込みのキャンセル)も指示できます。

ライト・コマンド(write command)は、リクエストではなくアトリビュート・プロトコルのコマンドです。読み込み処理は、クライアントが出した読み込みリクエストにサーバが値を返すので、必ずリクエストの形をとります。しかし書き込み処理は、サーバからリプライを返す必要はない場合があるので、コマンドが用意されています。

ライト・コマンドは、コントロール・ポイントへの書き込みに使われます。コントロール・ポイントは、サーバの装置等への動作指示を書き込むためのアトリビュートです。例えば、テレビのリモコンがクライアント、テレビ本体がサーバならば、リモコンのボタンを押してテレビに電源ONの指示を出す場合などに、ライト・コマンドが使われます。

Signeライト・コマンドは、シグネチャつきのライト・コマンドです。機器を動作させるような、重要な書き込みを、信頼できるクライアントのみに書き込みを許可したい場合に、使います。

==== インディケーションとノーティフィケーション

サーバからクライアントへの通知は、インディケーションまたはノーティフィケーションのいずれかを使います:

- Handle Value Indication
- Handle Value Notification

Handle Value Indicationはリクエスト/レスポンス、もHandle Value Notificationはコマンドである違いだけです。いずれも、アトリビュート・ハンドルと現在のアトリビュート・バリュー(長さは最大(ATT_MTU–3)オクテット)をクライアントに送ります。


==== ジェネリック・アトリビュート・プロファイル

ジェネリック・アトリビュート・プロファイル(Generic attribute profile, GATT)は、アトリビュート・プロトコルを使ったデータ交換の構造を与えます。その基礎構造は、サービスとキャラクタリスティクスという2つの要素で組み立てられます。

サービスは、機器の内部にある互いに独立した機能を表します。サービスというキャラクタリスティクスの集合です。キャラクタリスティクスは、外部センサー値や機器の内部状態および操作指示を読み書きするアトリビュートです。

GATTは、サービスとキャラクタリスティクスの発見、読み書き、そしてノーティフィケーションとインディケーションを提供します。

GATTは、サーバとクライアントの2つの役割を決めます。サーバは、アトリビュート・プロトコルのコマンドやリクエストを受けてレスポンスを返す役割です。クライアントは、アトリビュート・プロトコルのコマンドやリクエストを発行する役割です。このサーバとクライアントは、GATT層での役割です。リンク層のピコネットのマスターやスレーブなど、ATTTよりも下の層が決める役割とは、関係はありません。

サーバとクライアントの役割は物理的なデバイスに固定されるものではありません。ある機器がサーバになったりクライアントになることも、また同時にサーバかつクライアントになることも、利用場面次第であります。

==== サービスとキャラクタリスティクス

![ #fig_service_and_char サービスとキャラクタリスティクス](fig/ch02_service_and_char.png)

クラシックBluetoothでは、プロトコルは通信手順とデータ表現の仕様を、プロファイルは接続した機器の振る舞いの仕様を表します。Bluetoothのロゴがついていれば、製品やメーカを問わずに相互に接続できるのは、ヘッドフォンや着信通知といったアプリケーションごとに、機器の振る舞いまでを含めて仕様が決められて、規格認証の厳格な運用があるからです。

クラシックBluetoothには、オーディオのストリーミングや大容量のデータ・ファイル交換など、求められるネットワークの特性もデータ形式もまるで異なるアプリケーションがあります。そのために、プロトコルとプロファイルはアプリケーションごとに決められ、論理的通信チャンネルを提供するL2CAP層の上に作られます。

 Bluetooth low energy では、すべてのアプリケーションはGATTの上に作られます。このGATTは、アトリビュート・プロトコルを使って、サービスとキャラクタリスティクスという仕組みを提供します。これが Bluetooth low energy で多種多様なアプリケーションが展開できる鍵になります。

GATTのサービスとは、アトリビュート・プロトコルのサーバが外部公開する、機能の単位です。例えば、ある機器がユーザに警告音を伝える機能、のように、機器の単1の機能を、サービス、という概念で扱います。サービスは、キャラクタリスティクスの集合です。キャラクタリスティクスとは、その機器の内部状態、操作指示、そして外部センサ値を読み書きするアトリビュートです。

GATTは、その機器の機能それぞれをサービスという単位で切り分けます。 Bluetooth low energy のサービスの概念は、ある機器の内部のハードウェアそれぞれの、互いに独立した機能の単位であり、けっして、ユーザやプロファイル(機器の使われ方)から見た個別機器の機能ではありません。例えばエアコンであれば、消費電力の計測、気温の検出、室温制御は、それぞれ個別に3つのサービスとして実装されるでしょう。ユーザ(プロファイル)からみると、これはエアコンという、1つのサービスにしそうです。しかし、 Bluetooth low energy のサービスは、その機器の内部にある互いに独立したハードウェアの機能、を指します。

この Bluetooth low energy のサービスが、機器の機能と、機器の使われ方を分離します。機器の使われ方、つまりプロファイルは、その機器のサービスの中から、必要なサービスを組み合わせて使います。

例えば、機器に音や振動でユーザにアラートを出す機能があるとします。この1つの機能が外部のクライアントにサービスとして公開されます。クライアントは、スマートフォンのアプリケーションなどでしょう。もしもそれが、スマートフォンの置き忘れ防止のアプリケーションならば、スマートフォンを置き忘れそうになったら、アプリケーションが機器にアラートを出させるでしょう。もしも、電話着信通知のアプリケーションであれば、電話着信を検出したら機器にアラートを出させるでしょう。サービスの利用方法は限定されていないので、アプリケーション側を作りサービスを組み合わせて、それまで考えられていなかった振る舞いでも、実現できます。

クラシックBluetoothと異なり、開発者が決めた任意のサービスおよびプロファイルを実装した機器を、Bluetoothの規格認証をうけてBluetoothロゴをつけて販売することができます。それは、 Bluetooth low energy の規格認証の範囲がGATT層までで、サービスやプロファイルはGATT層の上のアプリケーション層で作るからです。 Bluetooth low energy にも、クラシックBluetoothと同様に、メーカを超えた相互運用のために、Bluetooth SIGが承認したプロファイルおよびサービスの仕様があります。それらを実装するときは、それらの仕様に従います。

==== サービスとキャラクタリスティクスの例

懐中電灯を例にして、サービスとキャラクタリスティクスの使われ方を述べます。懐中電灯は、ライトのオンオフ状態などのデータを持っていますから、GATTでの役割はサーバになります。懐中電灯のサービスは、ライトの点灯状態の取得とそのオンオフ設定、そして明るさセンサーからの検出値の取得ができるものとします。

サーバ側のサービスとキャラクタリスティクス、そしてクライアントのプロファイルの設計するかは、Bluetooth SIGが定義しているものがあれば、それを使います。該当するものがない場合は、独自に設計します。該当するプロファイルがない場合でも、そのプロファイルに必要なサービスがBluetooth SIGで定義されていれば、その定義を利用すれば工程が少なくなる場合があります。

サービスに含まれるキャラクタリスティクスは次の3つのタイプに分類されます:

- 外部の値
	- センサー値など、読み出すたびに値が変化するようなもの
- 機器内部の値
	- デバイス内部の動作状態 (制御式の内部変数の値など)
- コントロールポイント
	- 照明スイッチのOn/Offの操作など。
	- このような操作を書き込むものは、readableではなく、writable/notifiableに設計する

このサービスには、ライトのスイッチ、ステータス、そして明るさ、の3つのキャラクタリスティクスがあります。それぞれのキャラクタリスティクスは、コントロール・ポイント、機器内部の値、そしてセンサー値を表します。

スイッチのキャラクタリスティクスは、外部からの操作を書き込むものですから、パーミッションは書き込みのみ読み出し不可に設定されているでしょう。ここにオン、オフに対応する値を書き込めば、サーバである懐中電灯がライトを制御します。オンオフを表す値をどうするかなどは、開発者が仕様を作り定義します。

ステータスのキャラクタリスティクスは、懐中電灯の制御プログラムの内部状態を表します。ライトの制御プログラムの動作は、状態遷移図で表現されます。スイッチの操作により、オンとオフの2状態を遷移します。このステータスは、スイッチのキャラクタリスティクスへの書き込み値で遷移するかもしれませんが、サーバの内部の変数の値ですから、このステータス自体が外部から書き込まれることは決してありません。ですから、パーミッションは読み出しのみに設定されます。

最後の明るさのキャラクタリスティクスは、実際のライトの明るさの検出値を表します。ライトの明るさは、ランプの劣化や乾電池の残量低下などで変化するでしょうから、実際の明るさを検出したくなるでしょう。検出値は読みだすだけで、書き込むものではありません。ですから、パーミッションは読み込みのみに設定されます。また、一定の時間間隔で、サーバからクライアントに通知(ノーティフィケーションまたはインディケーション)が必要かもしれません。GATTのキャラクタリスティクスには、クライアントに通知をする、しないを設定する機能があります。

ここまでの懐中電灯のサービスを定義したあとで、クライアントから乾電池の残量も知りたいと要求が出されたとします。このときに、懐中電灯のサービスにバッテリー残量を表すキャラクタリスティクスを追加する必要はありません。Bluetooth SIGには、バッテリー状態を公開する [Battery Service](@<href>{https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.battery_service.xml) [^2090]} が定義されていますから、このサービスを追加します。懐中電灯のサービスと、このバッテリー状態のサービスを組み合わせて使うことで、追加要求を満たすプロファイルが実現できます。

[^2090]: [Battery Service @<href>{https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.battery_service.xml](https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.battery_service.xml)}

==== サービス

GATTのサービスは、データと関連付けられた機器の振る舞いや機能をあらわすものです。GATTのプロファイルはサービスの集合です。
サービスの内容は、他のサービスへの参照とキャラクタリスティクスの集合です。サービスの定義は、サービスに必須のキャラクタリスティクスと、オプションのキャラクタリスティクスを定義します。

GATTのサービスとキャラクタリスティクスは、オブジェクト指向プログラミングのクラスとプロパティに例えられます。1つの独立した機能がクラスの定義です。アトリビュート・プロトコルのアトリビュートには、ちょうどプログラミングでいうメモリ・アドレスに相当する、ハンドルがあります。GATTは、サーバの複数のアトリビュートで、サービスの定義にしたがったサービスのインスタンスを公開しているようなものです。

サービスは、それぞれが独立した機能です。ですから異なる機器で、1つのサービスの定義を広く使えます。例えば、バッテリー状態を表すバッテリー・サービスが定義されています。このサービスは、電気車ならばその電池残量や容量、あるいは懐中電灯ならばその電池残量を表すでしょう。バッテリー・サービスはバッテリー・サービスとして振る舞うので、異なる機器でもプロファイルが求める機能が含まれていれば、利用ができます。サービスの使われ方、プロファイルは、クライアントの実装が決めます。

サービスは、他のサービスを参照して、そのサービスのキャラクタリスティクスを取り込むことができます。この参照されたサービスを included service と呼びます。他のサービスを取り込んだサービスは、included service自体の振る舞いを変更することは禁止されます。新しい値や振る舞いをキャラクタリスティクスとして追加することだけができます。included serviceの数や階層の深さに制約はありません。

このincluded serviceの仕組みは、サービスを継承して新しいサービスを作ること、後方互換性を保ちつつサービスを拡張することに使えます

例えば、バッテリー・サービスに、充電式電池に対応させるために電池の満充電容量の値を追加したいとします。この場合は、バッテリー・サービスを参照して、満充電容量の値のキャラクタリスティクスを追加した、新しいサービスBを定義できます。サービスBには対応していない更新されていないクライアントは、バッテリー・サービスを発見して、従来通りに動くでしょう。サービスBに対応した新しいクライアントは、拡張された満充電容量の値も読めるでしょう。

サービスには、プライマリ・サービスとセカンダリ・サービスの2タイプがあります。プライマリ・サービスは外部に公開されるサービスです。プライマリ・サービスは他のサービスのincluded serviceにもなれます。セカンダリ・サービスは外部に公開されず、他のサービスのincluded serviceとして使われるのみのサービスです。セカンダリ・サービスは、複数のサービスで定義を共有したいバリューをまとめるのに用いて、他のプライマリ・サービスから参照される使われ方をします。

.サービスのアトリビュート
[options="hader"]
|===
|アトリビュート・タイプ        | アトリビュート・バリュー
|0x2800,プライマリ・サービス  | サービスのUUID(16もしくは128ビット)
|0x2801,セカンダリ・サービス  | サービスのUUID(16もしくは128ビット)
|0x2802,Includedサービス    | Included Service Attribute Handle
|                          |  End Group Handle
|                          |  Service UUID (16ビットの場合のみ)
|===

アトリビュートをグループ化するサービスは、プライマリおよびセカンダリ・サービスの宣言から始まります。
アトリビュート・ハンドルは任意の値が取れます。サービスの宣言は、バリューにそのサービスのUUIDを含みます。

そのサービスが他のサービスを参照している場合は、サービス宣言の次に、インクルード・ディフィニション(include definition)が続きます。インクルード・ディフィニションのバリューは、参照するサービス宣言のアトリビュート・ハンドル、それにそのサービス宣言の末尾を表すアトリビュート・ハンドルが続きます。もしも参照するサービスのUUIDが16ビットならば、そのUUIDが続きます。

==== キャラクタリスティクス

キャラクタリスティクスは、サーバの値や振る舞いの表現に使われます。キャラクタリスティクスの働きは、クライアントからサーバへの値の読み書きと、サーバからクライアントへの通知の2つです。キャラクタリスティクスの値とその読み書きの意図は、サーバで検出されたセンサなどの外部値、サーバの内部動作状態を表す値、そしてサーバへの動作指示、の3つに分類できます。

キャラクタリスティクスは、アトリビュート・ハンドルが連続する、アトリビュートの集合で表現されます。キャラクタリスティクスを構成するアトリビュートは、次の3つです:

- デクラレーション (Declaration)
- バリュー (Value)
- ディスクリプタ (Descriptor(s))

デスクリプションは、キャラクタリスティクスの読み書きなどの属性を制限します。バリューは、そのキャラクタリスティクスの値そのものを表します。ディスクリプタは、バリューの単位や表記を表します。

===== キャラクタリスティクス・デクラレーション

キャラクタリスティクス・デクラレーションは、アトリビュート・タイプ 0x2803 のアトリビュートで表します。アトリビュート・ハンドラは任意の値が取れます。アトリビュート・バリューは、キャラクタリスティクス・プロパティ、そのキャラクタリスティクスのバリューのアトリビュート・ハンドラ、およびキャラクタリスティクスのUUIDで構成されます。

.キャラクタリスティクス・デクラレーション
[options="header"]
|===
|アトリビュート・タイプ      |  アトリビュート・バリュー
|0x2803
| Characteristic Properties
  Characteristic Value Attribute Handle
  Characteristic UUID
|===

このプロパティは、キャラクタリスティクスのパーミションをビット・フィールドで表す1オクテットの値です。クライアントは、このプロパティから、キャラクタリスティクス・バリューに使える
アトリビュート・プロトコルを知ることができます。

プロパティののBroadcastは、アトリビュート・プロパティにはないものです。もしもこのビットが1ならば、キャラクタリスティクス・デスクリプタに対応する値を書き込めば、キャラクタリスティクス・バリューがアドバタイズメント・データでブロードキャストされます。

.キャラクタリスティクス・デクラレーション
|===
|プロパティ名                  | 値       | 概要
|Broadcast                   | 0x01     | アドバタイズメントでのブロードキャスト
|Read                        | 0x02     | 読み出し可能
|Write Without Response      | 0x04     | レスポンスなしの書き込み
|Write                       | 0x08     | レスポンスありでの書き込み
|Notify                      | 0x10     | ノーティフィケーション
|Indicate                    | 0x20     | インディケーション
|Authenticated Signed Writes | 0x40     | シグネチャ付きの書き込み
|Extended Properties         | 0x80     | プロパティの拡張定義
|===

キャラクタリスティクス・バリュー・アトリビュート・ハンドルは、このキャラクタリスティクスのバリューを保持するアトリビュートのアトリビュート・ハンドラを示します。今のGATTの仕様では、キャラクタリスティクス・デクラレーションの次のアトリビュートが、キャラクタリスティクス・バリューになります。暗黙的なアトリビュートの並びに依存しないように、将来の実装が変更になっても対応できるように、このハンドラがあります。

===== キャラクタリスティクス・ディスクリプタ

キャラクタリスティクス・デスクリプタは、キャラクタリスティクス・バリューが文字列なのか数値なのかといった変数の型や単位等の情報の追加と、そのキャラクタリスティクス・バリューが変更された時にクライアントに通知する/しないといった振る舞いの指定に使います。

キャラクタリスティクス・バリューの値の説明や振る舞いは、サービスの仕様書に文章で記載されます。キャラクタリスティクス・デスクリプタは、そのサービスの仕様書にある内容をアトリビュートで表現します。キャラクタリスティクス・デスクリプタを読み出せば、もしもそのサービスの仕様を知らなくても、値の表示はできるはずです。

キャラクタリスティクス・デスクリプタは以下のとおりです:

- Characteristic Extended Properties
- Characteristic User Descriptor
- Client Characteristic Configuration
- Server Characteristic Configuration
- Characteristic Presentation Format
- Characteristic Aggregation Format

多くのデスクリプタがありますが、ほとんどはオプションです。

===== Characteristic Extended Properties

Characteristic Extended Properties は、キャラクタリスティクス・プロパティの追加定義を与えます。キャラクタリスティクス・デクラレーションのプロパティのExtended Propertiesが1の時に、現れます。このプロパティは、Reliable Write と Writable Auxiliaries の2つのフラグがあります。

Reliable Writeのビットが1ならば、キャラクタリスティクス・バリューの書き込みに、2段階の書き込み手順を使うことが許可されます。これは、
まずクライアントからサーバにアトリビュート・プロトコルのPrepare Write リクエスト/レスポンスでデータを送信します。次に、Execute write リクエスト/レスポンスで書き込みます。それぞれのリクエスト/レスポンスごとに、サーバはデータの正当性を確認して、エラーがあればそれをクライアントに通知できます。これにより、より信頼できるデータ書き込みができます。

Write Auxiliariesのビットが1ならば、Characteristic User デスクリプションのバリューへの書き込みが許可されます。

===== Characteristic User Descriptor

Characteristic User Descriptorは、キャラクタリスティクスを説明する人間に読める文字列です。Characteristic Extended Properties の Writable auxiliariesビットを1にして、ユーザに任意の文字列の書き込みを許可できます。

例えば、家の温度センサーなら、その温度がどの部屋のものかを表示したくなるでしょう。そのような場合に、キャラクタリスティクス・ユーザ・デスクリプタに部屋番号を記録しておけば、わかりやすくなります。

===== Client Characteristic Configuration

クライアント・キャラクタリスティクス・コンフィグレーションは、キャラクタリスティクスの振る舞いを設定するために、クライアントがサーバに書き込むものです。キャラクタリスティクスのプロパティの、notifiable もしくは indicatable が1ならば、必ずこのコンフィグレーションがあります。

クライアント・キャラクタリスティクス・コンフィグレーションは2ビットの値、1ビットがnotifications, もう1ビットがindications、を持ちます。クライアントがnotificationsに1を書き込めば、このキャラクタリスティクスのバリューが変更されたときに、サーバはクライアントにノーティフィケーションを送ります。indicationが1ならば、インディケーションを送ります。

クライアント・キャラクタリスティクス・コンフィグレーションは、クライアント固有の設定です。例えば、クライアントAとBの2つがサーバに接続している時、クライアントAがindicationを1にすれば、クライアントAにのみインディケーションが送信されます。またボンディングをしていないクライアントでは、接続時に全てのビットは初期値0に設定されます。

===== Server Characteristic Configuration

サーバ・キャラクタリスティクス・コンフィグレーションは、broadcastの1ビットの値だけを持ちます。このキャラクタリスティクスが含まれるサービスに関連した値が、アドバタイズメント・データでブロードキャストされます。どのようにブロードキャストされるかは、サービスの仕様が決めます。

ブロードキャストされるデータは、このサーバ・キャラクタリスティクス・コンフィグレーションを含むキャラクタリスティクス自体の値ではありません。このキャラクタリスティクスが含まれるサービスに関連するデータです。これは、値の意味や振る舞いを定義するのはサービスであって、キャラクタリスティクスそれ自体ではないからです。例えば、温度が20度、をブロードキャストしても意味がありません。例えば、室内が20度のように、ブロードキャストされたデータを説明するために、文脈が必要になります。

サーバ・キャラクタリスティクス・コンフィグレーションは、サーバ側の設定です。ですから、どのクライアントが設定しても、それはサーバの振る舞いを変更します。

===== Characteristic Presentation Format

キャラクタリスティクス・プレゼンテーション・フォーマットは、キャラクタリスティクスのバリューの変数型や表記フォーマットを表すのに使います。

サービスの仕様を知らない、キャラクタリスティクスを読み出すだけの汎用のクライアントでも、キャラクタリスティクス・プレゼンテーション・フォーマットを読み込むことで、その値がなにを意味するかを理解できなくても、人間が読める形でデータを表示できます。

このキャラクタリスティクスのフィールドは:

- Format
- Exponent
- Unit
- Namescape
- Description

で構成されます。

Formatは、変数の型を表します。次のような型があります:

- Boolean
- unsigned 2-bit / 4-bit
- unsigned/ signed 8 ~ 128 bit 整数
- IEEE-754 浮動小数点型
- 主に医療機器で使われる、2つの、固定サイズの固定小数点
- UTF-8 / UTF-16

Unitは単位を表す16ビットのUUIDです。この割当は、Bluetooth SIGの [assigned numbers](@<href>{https://developer.bluetooth.org/gatt/units/Pages/default.aspx) [^2100]} に列挙されています。

[^2100]: [assigned numbers @<href>{https://developer.bluetooth.org/gatt/units/Pages/default.aspx](https://developer.bluetooth.org/gatt/units/Pages/default.aspx)}

<!--
のこる2つのフィールドは、合わせて1つの値とみなすべき。
namespace とdescription fieldは、値の付加情報。
namespaceは、1バイトのフィールドで、そのdescriptionフィールドをどこの組織が決めているかを表す。descriptionは符号なし16ビットの値。
例えばセンサーが外と中にあり、Descriptionの違いはそれだけなら、このフィールドで、その違いを見てやればいい。

Characteristic Aggregation Format
いくつかのValueの組み合わせで意味がある値がある。例えば地球上での位置は、緯度と経度で表す。そのような値のフォーマットを指定するのがAggregation Format。
例えばこの例なら、Aggregation formatは、正しい順番でその2つのCharacteristics descriptorを参照する。
-->

<!--
ライトが2つある。
Write without response
(ATT_MTU – 3) を書き込む。long attributeには対応していない。
Attribute Protocol Write Command を使う。

Attribute Protocol Write Request はリクエスト/レスポンスを使うけど、やっぱりlong attributeには対応していない。
Write Long Characteristic Valuesを使うと。

UUID、未定義なものはクライアントは無視していい。
-->
