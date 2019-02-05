esp32-homekit-natureremo
====================

Apple Homekit から [Nature Remo](https://nature.global/) を操作する事ができる、Espressif の SoC マイコン [ESP32](https://ja.wikipedia.org/wiki/ESP32) のカスタムファームウェアです。  
```
Apple Homekit -> ESP32 -> HTTP(Nature Remo Local API) -> Nature Remo
```

### 前提
[esp-idfのセットアップ](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html)が完了している事。

### インストール方法  

1. このリポジトリをClone
```
git@github.com:kosukekurimoto/esp32-homekit-natureremo.git
```

2. サブモジュールを同期
```
$ git submodule update --init --recursive
```

3. シリアルポートとWiFiを設定
```
$ make manuconfig
```
```
Serial flasher config - > Default serial port // デバイスへの書き込み用シリアルポートの設定  
```
```
Example Configuration - > WiFi SSID, WiFi Password // 接続するWiFiのSSID, パスワードの設定 ※2.4Ghzのみ  
```

4. ビルド & フラッシュ
```
$ ./flash
```

### 関連リンク  
[Espressif IoT Development Framework](https://github.com/espressif/esp-idf).
