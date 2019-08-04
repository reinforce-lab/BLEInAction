Win8
SDK4.4.0をDL。共有フォルダからインストールしようとしたら、インストールの最後に、インストールに失敗したなどのメッセージが出る。デスクトップとかのローカル(相当)に一度コピーしてからだと大丈夫。

J-Linkのドライバインストールの失敗? デバイスを認識しない。
デバイスドライバがらみか。

Seggerのフォルダを指定してドライバを更新かけたら、ドライバインストールできて、アクセスできた。なんじゃそりゃ。

Windows8
MDK-ARMで書き込もうとしたら、フラッシュでEraseでエラー。
SDKのインストーラのログをとったら、 ..msi /log log.txt とったら、原因はわからない。

Windows7なら?

SFXCA: Extracting custom action to temporary directory: C:\Windows\Installer\MSI2454.tmp-\
SFXCA: Failed to get requested CLR info. Error code 0x80131700
SFXCA: Ensure that the proper version of the .NET Framework is installed, or that there is a matching supportedRuntime element in CustomAction.config. If you are binding to .NET 4 or greater add useLegacyV2RuntimeActivationPolicy=true to the <startup> element.

.NET 3.5 SP1 をDLしてインストール。OK。SDKのインストールできた。Keilで動作した。
S110のをnRF Goで書き込んでから、ファームを書き込みで。


Win7
デバイスをさしたままインストール。ドライバインストールエラー。
なので、修復モードで再インストール。
(F8でセーフモードに入る？)

ソフトウェアデバイスが必要なプロジェクトの書き込みは、ソフトウェアデバイスがないと、書き込みに失敗する。かならずnRF Goでソフトウェアデバイスを書き込んでおいてから、MDK-ARM経由の書き込みをする。順番。

Win7 14.34GB
Win8 17.8GB

開発ボードがブリック化
----
nRF Goでも、ボードの書き込み画面がでない。(no device found)
https://devzone.nordicsemi.com/index.php/bricked-nrf51422-board
nrfprog --recover / --recoversでOK

石のリビジョン
---
石が古いとSDK4.2まで、5系は新しい石じゃないと使えない。

BL600
----
シリアル番号を抜く。

<!--
# 開発メモ

SDK version 5.0.0 includes an experimental example of how ANCS can be used from an nRF51822 Peripheral.
Look in Boards/nrf6310/ble/experimental/ble_app_ancs/

/*
 // Setup the GAP Bond Manager
  {
    uint32 passkey = 0; // passkey "000000"
    uint8 pairMode = GAPBOND_PAIRING_MODE_INITIATE;
    uint8 mitm = TRUE;
    uint8 ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8 bonding = TRUE;
    GAPBondMgr_SetParameter( GAPBOND_DEFAULT_PASSCODE, sizeof ( uint32 ), &passkey );
    GAPBondMgr_SetParameter( GAPBOND_PAIRING_MODE, sizeof ( uint8 ), &pairMode );
    GAPBondMgr_SetParameter( GAPBOND_MITM_PROTECTION, sizeof ( uint8 ), &mitm );
    GAPBondMgr_SetParameter( GAPBOND_IO_CAPABILITIES, sizeof ( uint8 ), &ioCap );
    GAPBondMgr_SetParameter( GAPBOND_BONDING_ENABLED, sizeof ( uint8 ), &bonding );
  } */
  -->
  