#include "systemled.h"

/* プライベート関数のプロトタイプ宣言 */
static void blink_led_task(void *_args);
static void systemled_gpio_write(bool on);

/* LEDの定義 */
static led_config_t systemled_config = {
    .gpio = 2,
    .on = false
};

/* LED用GPIOの初期化 */
void systemled_init() {
    gpio_set_direction(systemled_config.gpio, GPIO_MODE_OUTPUT);
    systemled_gpio_write(systemled_config.on);
}

/* ステータスLEDのアクション */
// ONにする
void systemled_on(){
    systemled_config.on = true;
    systemled_gpio_write(systemled_config.on);
}
// OFFにする
void systemled_off(){
    systemled_config.on = false;
    systemled_gpio_write(systemled_config.on);
}
// 点滅させる
void systemled_blink(){
    xTaskCreate(blink_led_task, "LED identify", 512, NULL, 2, NULL);
}
// 現在のステータスを取得
bool systemled_status(){
    return systemled_config.on;
}

/* LEDの点滅アニメーション */
static void blink_led_task(void *_args) {
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            systemled_gpio_write(true);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            systemled_gpio_write(false);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        vTaskDelay(250 / portTICK_PERIOD_MS);
    }

    systemled_gpio_write(systemled_config.on);

    vTaskDelete(NULL);
}

/* GPIOへの書き込み */
static void systemled_gpio_write(bool on) {
    gpio_set_level(systemled_config.gpio, on ? 1 : 0);
}
