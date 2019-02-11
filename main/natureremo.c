#include "natureremo.h"

static const char* TAG = "NATUREREMO";
static EventGroupHandle_t ready_to_signal_transmission_event_group;
const int READY_TO_SIGNAL_TRANSMISSION_BIT = BIT2;

/* アプライアンスとシグナルIDの定義 */
static natureremo_ceiling_light_t ceiling_lights[] = 
{
    {
        .name = "Living Light\0",
        .on = false,
        .signals = {
            {
                .name = "ON\0",
                .value = "{\"format\":\"us\",\"freq\":32,\"data\":[8895,4493,556,601,577,578,577,579,581,577,581,575,580,580,555,600,577,1655,580,1658,575,581,576,1660,578,578,559,596,578,582,575,1658,575,1658,576,580,577,1656,578,581,576,1660,576,1656,558,596,578,585,577,1653,576,1657,575,583,575,1656,577,579,585,570,577,1658,558,1673,578,581,575,52371,15711,665,17074,665,3137,665,5686,1924,611,648,619,1916,602,1931,622,645,605,663,618,1915,620,1915,623,643,620,1917,621,646,618,650,616,1917,620,1915,619,647,625]}\0",
            },{
                .name = "OFF\0",
                .value = "{\"format\":\"us\",\"freq\":30,\"data\":[8906,4492,573,585,580,577,575,583,575,580,578,583,576,582,554,601,577,1656,578,1660,575,585,574,1661,573,585,575,584,576,580,575,1664,571,1659,576,585,575,583,557,598,575,1665,574,579,559,602,573,585,571,584,577,1656,578,1661,574,1658,579,581,558,1675,578,1660,575,1661,577,1660,571,52411,20824,641,6944,1936,6966,641,620,1918,619,648,624,645,621,647,619,651,624,1912,618,650,620,651,616,653,620,645,621,1919,616,1918,618,1919,624,645,602,1934,619,1920,617,1918,619,1920,617]}\0",
            }
        }
    },{
        .name = "Kitchen Light\0",
        .on = false,
        .signals = {
            {
                .name = "ON\0",
                .value = "{\"format\":\"us\",\"freq\":37,\"data\":[8911,4533,580,583,581,582,578,585,577,582,582,582,578,585,575,584,579,1666,577,1669,576,582,580,1665,580,583,578,581,581,583,577,1666,577,1668,576,1668,577,1664,579,583,576,1665,579,1663,580,584,577,587,574,1665,578,589,573,584,579,1660,582,582,577,586,575,1667,577,1663,580,584,576,52588,10075,5099,620,650,621,652,620,1925,621,652,623,649,620,655,618,652,624,1922,627,1917,26055,1943,604,669,4447,643,4439,1928,618,651,630]}\0",
            },{
                .name = "OFF\0",
                .value = "{\"format\":\"us\",\"freq\":36,\"data\":[8909,4536,577,583,578,586,579,584,576,585,576,586,578,585,574,587,574,1666,580,1669,577,584,575,1666,561,603,576,587,578,581,584,1660,578,1666,575,1666,579,584,586,577,549,1695,576,586,575,588,573,587,575,588,575,584,577,1664,579,1666,587,577,575,1668,585,1660,579,1664,575,1669,576,52613,10081,5108,618,651,621,653,620,1928,620,654,618,652,603,672,620,653,622,1927,625,1920,625,649,618,655,623,652,620,1927,627,648,620,1924,622,652,621,1928,620,653,624,647,626,1924,620,652,606,666,624,656,616,653,624,651,620,1927,621,1926,620,652,622,1929,600,1946,623,1924,620,1927,620]}\0",
            }
        }
    }
};

esp_err_t _signal_transmission_event_handler(esp_http_client_event_t *evt) {
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // Write out data
                printf("%.*s", evt->data_len, (char*)evt->data);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

static bool signal_transmission(char* signal_value) {
    bool result = true;
    esp_http_client_handle_t client = NULL;

    int http_status_code = 0;
    esp_http_client_config_t config = {
        .url = "http://192.168.10.20/messages",
        .event_handler = _signal_transmission_event_handler,
    };
    client = esp_http_client_init(&config);

    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "X-Requested-With", "curl");
    esp_http_client_set_header(client, "accept", "application/json");
    esp_http_client_set_post_field(client, signal_value, strlen(signal_value));
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        http_status_code = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",
                http_status_code,
                esp_http_client_get_content_length(client));
        if(http_status_code != 200){
            result = false;
        }
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
        result = false;
    }
    esp_http_client_cleanup(client);
    return result;
}

static void wait_ready_to_signal_transmission(){
    xEventGroupWaitBits(ready_to_signal_transmission_event_group, READY_TO_SIGNAL_TRANSMISSION_BIT, false, true, portMAX_DELAY);
}

static void signal_transmission_task(void* pvParameters){
    int max_retry_count = 3;
    
    xEventGroupClearBits(ready_to_signal_transmission_event_group, READY_TO_SIGNAL_TRANSMISSION_BIT); // HTTPリクエストの排他処理
    wifi_wait_connected();    // WiFiへの接続が完了するまで待つ
    wait_ready_to_signal_transmission();  // HTTPリクエストが送信可能になるまで待つ
    vTaskDelay(500 / portTICK_PERIOD_MS);

    natureremo_signal_t* natureremo_signal = (natureremo_signal_t*)pvParameters;

    while(1) {
        ESP_LOGI(TAG, "Start signal transmission\n");
        int signal_transmission_result = signal_transmission(natureremo_signal->value);
        if(signal_transmission_result){
            ESP_LOGI(TAG, "Finish signal transmission\n");
            break;
        }
        ESP_LOGI(TAG, "Retry signal transmission...\n");
        if(--max_retry_count == 0){
            break;
        }
        ESP_LOGE(TAG, "Signal transmission error\n");
        ESP_LOGI(TAG, "Retry signal transmission...\n");
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
    
    xEventGroupSetBits(ready_to_signal_transmission_event_group, READY_TO_SIGNAL_TRANSMISSION_BIT); // HTTPリクエストの排他解除
    vTaskDelete(NULL);
}

/* シーリングライトを点灯する */
void natureremo_ceiliing_light_on(int light_no) {
    ceiling_lights[light_no].on = true; // ライトの点灯ステータスをON(true)にする
    natureremo_signal_t* natureremo_signal = &ceiling_lights[light_no].signals[0];
    xTaskCreate(
        &signal_transmission_task,
        "signal_transmission_task",
        8192,
        (void*)natureremo_signal,
        5,
        NULL
    );
}

/* シーリングライトを消灯する */
void natureremo_ceiliing_light_off(int light_no) {
    ceiling_lights[light_no].on = false; // ライトの点灯ステータスをOFF(false)にする
    natureremo_signal_t* natureremo_signal = &ceiling_lights[light_no].signals[1];
    xTaskCreate(
        &signal_transmission_task,
        "signal_transmission_task",
        8192,
        (void*)natureremo_signal,
        5,
        NULL
    );
}

/* シーリングライトのステータスを返す */
bool natureremo_ceiliing_light_status(int light_no) {
    ESP_LOGI(TAG, "Get ceiling light status(No=%d): Status=%d\n", light_no, ceiling_lights[light_no].on);
    return ceiling_lights[light_no].on;
}

void natureremo_init(){
    ready_to_signal_transmission_event_group = xEventGroupCreate();
    xEventGroupSetBits(ready_to_signal_transmission_event_group, READY_TO_SIGNAL_TRANSMISSION_BIT);
}