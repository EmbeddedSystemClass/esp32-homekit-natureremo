#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

/* original */
#include "wifi_setup.h"
#include "systemled.h"
#include "homekit_setup.h"
#include "natureremo.h"

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_initialise();     // WiFiの初期化
    wifi_wait_connected(); // WiFiに接続完了するまで待つ
    homekit_setup_start(); // Homekitサーバーの開始
    natureremo_init();     // NatureRemoシグナル送信処理の準備
    systemled_init();      // ステータス表示用LEDの準備
}