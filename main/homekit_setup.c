#include "homekit_setup.h"
#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

#include "natureremo.h"
#include "systemled.h"

static const char *TAG = "HOMEKIT_SETUP";

/* HomeKitイベントのハンドラー */
// アクセサリを認識
void accessory_identify_event(homekit_value_t _value) {
    printf("Accessory registered:\n");
    systemled_blink();
}
// リビング天井照明 ステータスを取得
homekit_value_t living_light_get_status_event() {
    return HOMEKIT_BOOL(natureremo_ceiliing_light_status(0));
}
// リビング天井照明 ON OFF
void living_light_set_event(homekit_value_t value) {
    if (value.format != homekit_format_bool) {
        printf("Invalid value format: %d\n", value.format);
        return;
    }
    if (value.bool_value == true) {
        natureremo_ceiliing_light_on(0);
    } else if (value.bool_value == false) {
        natureremo_ceiliing_light_off(0);
    }
}
// キッチン天井照明 ステータスを取得
homekit_value_t kitchen_light_get_status_event() {
    return HOMEKIT_BOOL(natureremo_ceiliing_light_status(1));
}
// キッチン天井照明 ON OFF
void kitchen_light_set_event(homekit_value_t value) {
    if (value.format != homekit_format_bool) {
        printf("Invalid value format: %d\n", value.format);
        return;
    }
    if (value.bool_value == true) {
        natureremo_ceiliing_light_on(1);
    } else if (value.bool_value == false) {
        natureremo_ceiliing_light_off(1);
    }
}

/* HoneKitアクセサリの定義 */
homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(
        .id=1,
        .category=homekit_accessory_category_switch,
        .services=(homekit_service_t*[]) {
            HOMEKIT_SERVICE(
                ACCESSORY_INFORMATION,
                .characteristics=(homekit_characteristic_t*[]) {
                    HOMEKIT_CHARACTERISTIC(NAME, "Homekit http"),
                    HOMEKIT_CHARACTERISTIC(MANUFACTURER, "HaPK"),
                    HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "K0012345"),
                    HOMEKIT_CHARACTERISTIC(MODEL, "v1"),
                    HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
                    HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify_event),
                    NULL
                },
            ),
            HOMEKIT_SERVICE(LIGHTBULB, .characteristics=(homekit_characteristic_t*[]) {
                HOMEKIT_CHARACTERISTIC(NAME, "Living Light"),
                HOMEKIT_CHARACTERISTIC(ON, false,                 
                    .getter=living_light_get_status_event,
                    .setter=living_light_set_event
                ),
                NULL
            }),
            HOMEKIT_SERVICE(LIGHTBULB, .characteristics=(homekit_characteristic_t*[]) {
                HOMEKIT_CHARACTERISTIC(NAME, "Kitchen Light"),
                HOMEKIT_CHARACTERISTIC(ON, false,                 
                    .getter=kitchen_light_get_status_event,
                    .setter=kitchen_light_set_event
                ),
                NULL
            }),
            NULL
        },
    ),
    NULL
};
homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111"
};

void homekit_setup_start(){
    homekit_server_init(&config);
}