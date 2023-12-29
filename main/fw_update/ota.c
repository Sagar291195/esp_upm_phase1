/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "cJSON.h"

#include "esp_timer.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "mqtt_client.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "storage/flash.h"

#include "wifi.h"
#include "ota.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG            "OTA"

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
/*! Saves OTA config received from ThingsBoard*/
static struct shared_keys
{
    char targetFwServerUrl[256];
    char targetFwVer[128];
} shared_attributes;

/********************************************************************************************
 *                          GLOBAL VARIABLES
 ********************************************************************************************/
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

/********************************************************************************************
 *                          STATIC VARIABLES
 ********************************************************************************************/
/*! Saves bit values used in application */
static EventGroupHandle_t event_group;
static esp_timer_handle_t ota_monitor_timer;
/*! Buffer to save a received MQTT message */
static char mqtt_msg[512];
static esp_mqtt_client_handle_t mqtt_client;

/********************************************************************************************
 *                          STATIC PROTOTYPE
 ********************************************************************************************/
static void parse_ota_config(const cJSON *object);
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
static enum state connection_state(BaseType_t actual_event, const char *current_state_name);
static esp_err_t _http_event_handler(esp_http_client_event_t *evt);
static const char *get_mqtt_url(const char *running_partition_label);
static uint32_t get_mqtt_port(const char *running_partition_label);
static const char *get_mqtt_access_token(const char *running_partition_label);
static void mqtt_app_start(const char *running_partition_label);
static bool fw_versions_are_equal(const char *current_ver, const char *target_ver);
static bool ota_params_are_specified(struct shared_keys ota_config);
static void start_ota(const char *current_ver, struct shared_keys ota_config);

/********************************************************************************************
 *                          STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void parse_ota_config(const cJSON *object)
{
    if (object != NULL)
    {
       
        cJSON *server_url_response = cJSON_GetObjectItem(object, TB_SHARED_ATTR_FIELD_TARGET_FW_URL);
        if (cJSON_IsString(server_url_response) && (server_url_response->valuestring != NULL) && strlen(server_url_response->valuestring) < sizeof(shared_attributes.targetFwServerUrl))
        {
            memset(shared_attributes.targetFwServerUrl, '\0', sizeof(shared_attributes.targetFwServerUrl));
            ESP_LOGI(TAG, "parse_ota_config: %s", shared_attributes.targetFwServerUrl);
            ESP_LOGI(TAG, "server_url_response->valuestring: %s", server_url_response->valuestring);
            memcpy(shared_attributes.targetFwServerUrl, server_url_response->valuestring, strlen(server_url_response->valuestring));
            shared_attributes.targetFwServerUrl[sizeof(shared_attributes.targetFwServerUrl) - 1] = 0;
            ESP_LOGI(TAG, "Received firmware URL: %s", shared_attributes.targetFwServerUrl);
        }

        cJSON *target_fw_ver_response = cJSON_GetObjectItem(object, TB_SHARED_ATTR_FIELD_TARGET_FW_VER);
        if (cJSON_IsString(target_fw_ver_response) && (target_fw_ver_response->valuestring != NULL) && strlen(target_fw_ver_response->valuestring) < sizeof(shared_attributes.targetFwVer))
        {
            memset(shared_attributes.targetFwVer, '\0', sizeof(shared_attributes.targetFwVer));
            memcpy(shared_attributes.targetFwVer, target_fw_ver_response->valuestring, strlen(target_fw_ver_response->valuestring));
            shared_attributes.targetFwVer[sizeof(shared_attributes.targetFwVer) - 1] = 0;
            ESP_LOGI(TAG, "Received firmware version: %s", shared_attributes.targetFwVer);
        }
    }
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    assert(event != NULL);

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        xEventGroupClearBits(event_group, MQTT_DISCONNECTED_EVENT);
        xEventGroupSetBits(event_group, MQTT_CONNECTED_EVENT);
        ESP_LOGD(TAG, "MQTT_EVENT_CONNECTED");
        break;
    case MQTT_EVENT_DISCONNECTED:
        xEventGroupClearBits(event_group, MQTT_CONNECTED_EVENT);
        xEventGroupSetBits(event_group, MQTT_DISCONNECTED_EVENT);
        ESP_LOGD(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGD(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGD(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGD(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGD(TAG, "MQTT_EVENT_DATA, msg_id=%d, %s", event->msg_id, event->topic);
        if (event->data_len >= (sizeof(mqtt_msg) - 1))
        {
            ESP_LOGE(TAG, "Received MQTT message size [%d] more than expected [%d]", event->data_len, (sizeof(mqtt_msg) - 1));
            return ESP_FAIL;
        }

        if (strcmp(TB_ATTRIBUTES_RESPONSE_TOPIC, event->topic) == 0)
        {
            memcpy(mqtt_msg, event->data, event->data_len);
            mqtt_msg[event->data_len] = 0;
            ESP_LOGI(TAG, "Shared = %s", mqtt_msg);
            cJSON *attributes = cJSON_Parse(mqtt_msg);
            if (attributes != NULL)
            {
                cJSON *shared = cJSON_GetObjectItem(attributes, "shared");
                parse_ota_config(shared);
            }

            char *attributes_string = cJSON_Print(attributes);
            cJSON_Delete(attributes);
            ESP_LOGD(TAG, "Shared attributes response: %s", attributes_string);
            // Free is intentional, it's client responsibility to free the result of cJSON_Print
            free(attributes_string);
            xEventGroupSetBits(event_group, OTA_CONFIG_FETCHED_EVENT);
        }
        else if (strcmp(TB_ATTRIBUTES_TOPIC, event->topic) == 0)
        {
            memcpy(mqtt_msg, event->data, MIN(event->data_len, sizeof(mqtt_msg)));
            mqtt_msg[event->data_len] = 0;
            ESP_LOGI(TAG, "Attribute = %s", mqtt_msg);
            cJSON *attributes = cJSON_Parse(mqtt_msg);
            parse_ota_config(attributes);
            char *attributes_string = cJSON_Print(attributes);
            cJSON_Delete(attributes);
            ESP_LOGD(TAG, "Shared attributes were updated on ThingsBoard: %s", attributes_string);
            // Free is intentional, it's client responsibility to free the result of cJSON_Print
            free(attributes_string);
            xEventGroupSetBits(event_group, OTA_CONFIG_UPDATED_EVENT);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGD(TAG, "MQTT_EVENT_ERROR");
        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        ESP_LOGD(TAG, "MQTT_EVENT_BEFORE_CONNECT");
        break;
    default:
        break;
    }
    return ESP_OK;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    assert(evt != NULL);

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            // Write out data
            ESP_LOGD(TAG, "%.*s", evt->data_len, (char *)evt->data);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static const char *get_mqtt_url(const char *running_partition_label)
{
    nvs_handle handle;
    APP_ABORT_ON_ERROR(nvs_open(NVS_KEY_MQTT_URL, NVS_READWRITE, &handle));

    static char mqtt_url[MAX_LENGTH_TB_URL + 1];
    size_t mqtt_url_len = sizeof(mqtt_url);

    esp_err_t result_code = nvs_get_str(handle, NVS_KEY_MQTT_URL, mqtt_url, &mqtt_url_len);
    if (result_code == ESP_OK)
    {
        ESP_LOGI(TAG, "MQTT URL from flash memory: %s", mqtt_url);
    }
    else if (result_code == ESP_ERR_NVS_NOT_FOUND)
    {
        APP_ABORT_ON_ERROR(nvs_set_str(handle, NVS_KEY_MQTT_URL, CONFIG_MQTT_BROKER_URL));
        APP_ABORT_ON_ERROR(nvs_commit(handle));
        APP_ABORT_ON_ERROR(nvs_get_str(handle, NVS_KEY_MQTT_URL, mqtt_url, &mqtt_url_len));
    }
    else
    {
        ESP_LOGE(TAG, "Unable to to get MQTT URL from NVS");
        APP_ABORT_ON_ERROR(ESP_FAIL);
    }

    nvs_close(handle);
    return mqtt_url;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static uint32_t get_mqtt_port(const char *running_partition_label)
{
    nvs_handle handle;
    APP_ABORT_ON_ERROR(nvs_open(NVS_KEY_MQTT_PORT, NVS_READWRITE, &handle));

    uint32_t mqtt_port;

    esp_err_t result_code = nvs_get_u32(handle, NVS_KEY_MQTT_PORT, &mqtt_port);
    if (result_code == ESP_OK)
    {
        ESP_LOGI(TAG, "MQTT port from flash memory: %d", mqtt_port);
    }
    else if (result_code == ESP_ERR_NVS_NOT_FOUND)
    {
        APP_ABORT_ON_ERROR(nvs_set_u32(handle, NVS_KEY_MQTT_PORT, CONFIG_MQTT_BROKER_PORT));
        APP_ABORT_ON_ERROR(nvs_commit(handle));
        APP_ABORT_ON_ERROR(nvs_get_u32(handle, NVS_KEY_MQTT_PORT, &mqtt_port));
    }
    else
    {
        ESP_LOGE(TAG, "Unable to to get MQTT port from NVS");
        APP_ABORT_ON_ERROR(ESP_FAIL);
    }

    nvs_close(handle);
    return mqtt_port;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static const char *get_mqtt_access_token(const char *running_partition_label)
{
    nvs_handle handle;
    APP_ABORT_ON_ERROR(nvs_open(NVS_KEY_MQTT_ACCESS_TOKEN, NVS_READWRITE, &handle));

    static char access_token[MAX_LENGTH_TB_ACCESS_TOKEN + 1];
    size_t access_token_len = sizeof(access_token);

    esp_err_t result_code = nvs_get_str(handle, NVS_KEY_MQTT_ACCESS_TOKEN, access_token, &access_token_len);
    if (result_code == ESP_OK)
    {
        ESP_LOGI(TAG, "MQTT access token from flash memory: %s", access_token);
    }
    else if (result_code == ESP_ERR_NVS_NOT_FOUND)
    {
        APP_ABORT_ON_ERROR(nvs_set_str(handle, NVS_KEY_MQTT_ACCESS_TOKEN, CONFIG_MQTT_ACCESS_TOKEN));
        APP_ABORT_ON_ERROR(nvs_commit(handle));
        APP_ABORT_ON_ERROR(nvs_get_str(handle, NVS_KEY_MQTT_ACCESS_TOKEN, access_token, &access_token_len));
    }
    else
    {
        ESP_LOGE(TAG, "Unable to to get MQTT access token from NVS");
        APP_ABORT_ON_ERROR(ESP_FAIL);
    }

    nvs_close(handle);
    return access_token;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void mqtt_app_start(const char *running_partition_label)
{
    assert(running_partition_label != NULL);

    const char *mqtt_url = get_mqtt_url(running_partition_label);
    const uint32_t mqtt_port = get_mqtt_port(running_partition_label);
    const char *mqtt_access_token = get_mqtt_access_token(running_partition_label);

    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = mqtt_url,
        .event_handle = mqtt_event_handler,
        .port = mqtt_port,
        .username = mqtt_access_token};

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    APP_ABORT_ON_ERROR(esp_mqtt_client_start(mqtt_client));

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static bool fw_versions_are_equal(const char *current_ver, const char *target_ver)
{
    assert(current_ver != NULL && target_ver != NULL);

    if (strcmp(current_ver, target_ver) == 0)
    {
        ESP_LOGW(TAG, "Skipping OTA, firmware versions are equal - current: %s, target: %s", FIRMWARE_VERSION, shared_attributes.targetFwVer);
        return true;
    }
    return false;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static bool ota_params_are_specified(struct shared_keys ota_config)
{
    if (strlen(ota_config.targetFwServerUrl) == 0)
    {
        ESP_LOGW(TAG, "Firmware URL is not specified");
        return false;
    }

    if (strlen(ota_config.targetFwVer) == 0)
    {
        ESP_LOGW(TAG, "Target firmware version is not specified");
        return false;
    }

    return true;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void start_ota(const char *current_ver, struct shared_keys ota_config)
{
    assert(current_ver != NULL);

    if (!fw_versions_are_equal(current_ver, ota_config.targetFwVer) && ota_params_are_specified(ota_config))
    {
        ESP_LOGW(TAG, "Starting OTA, firmware versions are different - current: %s, target: %s", current_ver, ota_config.targetFwVer);
        ESP_LOGI(TAG, "Target firmware version: %s", ota_config.targetFwVer);
        ESP_LOGI(TAG, "Firmware URL: %s", ota_config.targetFwServerUrl);
        esp_http_client_config_t config = {
            .url = ota_config.targetFwServerUrl,
            .cert_pem = (char *)server_cert_pem_start,
            .event_handler = _http_event_handler,
        };
        esp_err_t ret = esp_https_ota(&config);
        if (ret == ESP_OK)
        {
            set_fw_update_errorcode(NO_ERROR_FW_UDPATE);
        }
        else
        {
            ESP_LOGE(TAG, "Firmware Upgrades Failed");
            /* SKP : set error code and reboot esp*/
            set_fw_update_errorcode( ERROR_FIRMWARE_DOWNLOAD );
        }
    }
    else
    {
        /* SKP : set error code and reboot esp*/
        set_fw_update_errorcode( ERROR_LATEST_FIRMWARE_ALREADY );
    }
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static enum state connection_state(BaseType_t actual_event, const char *current_state_name)
{
    assert(current_state_name != NULL);

    if (actual_event & WIFI_DISCONNECTED_EVENT)
    {
        ESP_LOGE(TAG, "%s state, Wi-Fi not connected, wait for the connect", current_state_name);
        return STATE_WAIT_WIFI;
    }

    if (actual_event & MQTT_DISCONNECTED_EVENT)
    {
        ESP_LOGW(TAG, "%s state, MQTT not connected, wait for the connect", current_state_name);
        return STATE_WAIT_MQTT;
    }

    return STATE_CONNECTION_IS_OK;
}


/********************************************************************************************
 *                              
 ********************************************************************************************/
static void ota_monitor_timer_callback(void* arg)
{
    static int  count = 0;
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "One-shot timer called, time since boot: %lld ms", time_since_boot/1000);

    // count++;
    // if(count == 3)
    // {
    //     set_fw_update_errorcode(ERROR_TIMEOUT);
    // }
}

/********************************************************************************************
 *                             GLOBAL FUNCTIONS 
 ********************************************************************************************/

/********************************************************************************************
 *                              
 ********************************************************************************************/
void ota_task(void *pvParameters)
{
    bool ret = false;
    const esp_partition_t *running_partition;
    enum state current_connection_state = STATE_CONNECTION_IS_OK;
    enum state state = STATE_INITIAL;
    BaseType_t ota_events;
    BaseType_t actual_event = 0x00;
    char running_partition_label[sizeof(((esp_partition_t *)0)->label)];
    event_group = xEventGroupCreate();
    ESP_LOGI(TAG, "Running Firmware: %s", FIRMWARE_VERSION);

    while (1)
    {
        if (state != STATE_INITIAL && state != STATE_APP_LOOP)
        {
            if (state != STATE_APP_LOOP)
            {
                xEventGroupClearBits(event_group, OTA_TASK_IN_NORMAL_STATE_EVENT);
            }

            actual_event = xEventGroupWaitBits(event_group,
                                               WIFI_CONNECTED_EVENT | WIFI_DISCONNECTED_EVENT | MQTT_CONNECTED_EVENT | MQTT_DISCONNECTED_EVENT | OTA_CONFIG_FETCHED_EVENT,
                                               false, false, portMAX_DELAY);
        }

        switch (state)
        {
        case STATE_INITIAL:
            running_partition = esp_ota_get_running_partition();
            strncpy(running_partition_label, running_partition->label, sizeof(running_partition_label));
            ESP_LOGI(TAG, "Running partition: %s", running_partition_label);

            ret = initialise_wifi(running_partition_label);
            if ( ret )
            {
                state = STATE_WAIT_WIFI;
                /* SKP : start esp timer here to monitor firmware update process if timer expired 
                set error code and reboot esp32*/
                const esp_timer_create_args_t timer_args = {
                    .callback = &ota_monitor_timer_callback,
                    /* argument specified here will be passed to timer callback function */
                    .arg = NULL,
                    .name = "one-shot"
                };
                
                ESP_ERROR_CHECK(esp_timer_create(&timer_args, &ota_monitor_timer));
                ESP_ERROR_CHECK(esp_timer_start_periodic(ota_monitor_timer, 20000000));
                ESP_LOGI(TAG, "Started timers, time since boot: %lld ms", esp_timer_get_time()/1000);
            }
            else{
                /* SKP : set error code and reboot esp32 */
                set_fw_update_errorcode(ERROR_SSID_PASSWORD_NOT_AVAILABLE);
            }
            break;

        case STATE_WAIT_WIFI:
            if (actual_event & WIFI_DISCONNECTED_EVENT)
            {
                ESP_LOGW(TAG, "WAIT_WIFI state, Wi-Fi not connected, wait for the connect");
                state = STATE_WAIT_WIFI;
                break;
            }

            if (actual_event & WIFI_CONNECTED_EVENT)
            {
                mqtt_app_start(running_partition_label);
                state = STATE_WAIT_MQTT;
                break;
            }
            
            ESP_LOGE(TAG, "WAIT_WIFI state, unexpected event received: %d", actual_event);
            set_fw_update_errorcode(ERROR_WIFI_CONNECT);
            state = STATE_INITIAL;
            break;

        case STATE_WAIT_MQTT:
            current_connection_state = connection_state(actual_event, "WAIT_MQTT");
            if (current_connection_state != STATE_CONNECTION_IS_OK)
            {
                state = current_connection_state;
                break;
            }

            if (actual_event & (WIFI_CONNECTED_EVENT | MQTT_CONNECTED_EVENT))
            {
                ESP_LOGI(TAG, "Connected to MQTT broker %s, on port %d", CONFIG_MQTT_BROKER_URL, CONFIG_MQTT_BROKER_PORT);

                // Send the current firmware version to ThingsBoard
                cJSON *current_fw = cJSON_CreateObject();
                cJSON_AddStringToObject(current_fw, TB_CLIENT_ATTR_FIELD_CURRENT_FW, FIRMWARE_VERSION);
                char *current_fw_attribute = cJSON_PrintUnformatted(current_fw);
                cJSON_Delete(current_fw);
                esp_mqtt_client_publish(mqtt_client, TB_ATTRIBUTES_TOPIC, current_fw_attribute, 0, 1, 0);
                // Free is intentional, it's client responsibility to free the result of cJSON_Print
                free(current_fw_attribute);

                // Send the shared attributes keys to receive their values
                esp_mqtt_client_subscribe(mqtt_client, TB_ATTRIBUTES_SUBSCRIBE_TO_RESPONSE_TOPIC, 1);
                esp_mqtt_client_publish(mqtt_client, TB_ATTRIBUTES_REQUEST_TOPIC, TB_SHARED_ATTR_KEYS_REQUEST, 0, 1, 0);
                ESP_LOGI(TAG, "Waiting for shared attributes response");

                state = STATE_WAIT_OTA_CONFIG_FETCHED;
                break;
            }

            ESP_LOGE(TAG, "WAIT_MQTT state, unexpected event received: %d", actual_event);
            set_fw_update_errorcode(ERROR_MQTT_CONNECT);
            state = STATE_INITIAL;
            break;

        case STATE_WAIT_OTA_CONFIG_FETCHED:
            current_connection_state = connection_state(actual_event, "WAIT_OTA_CONFIG_FETCHED");
            if (current_connection_state != STATE_CONNECTION_IS_OK)
            {
                state = current_connection_state;
                break;
            }

            if (actual_event & (WIFI_CONNECTED_EVENT | MQTT_CONNECTED_EVENT))
            {
                if (actual_event & OTA_CONFIG_FETCHED_EVENT)
                {
                    ESP_LOGI(TAG, "Shared attributes were fetched from ThingsBoard");
                    xEventGroupClearBits(event_group, OTA_CONFIG_FETCHED_EVENT);
                    state = STATE_OTA_CONFIG_FETCHED;
                    break;
                }

                state = STATE_WAIT_OTA_CONFIG_FETCHED;
                break;
            }
            ESP_LOGE(TAG, "WAIT_OTA_CONFIG_FETCHED state, unexpected event received: %d", actual_event);
            set_fw_update_errorcode(ERROR_MQTT_CONFIG_FETCH);
            state = STATE_INITIAL;
            break;

        case STATE_OTA_CONFIG_FETCHED:
            current_connection_state = connection_state(actual_event, "OTA_CONFIG_FETCHED");
            if (current_connection_state != STATE_CONNECTION_IS_OK)
            {
                state = current_connection_state;
                break;
            }

            if (actual_event & (WIFI_CONNECTED_EVENT | MQTT_CONNECTED_EVENT))
            {

                start_ota(FIRMWARE_VERSION, shared_attributes);
                esp_mqtt_client_subscribe(mqtt_client, TB_ATTRIBUTES_TOPIC, 1);
                ESP_LOGI(TAG, "Subscribed to shared attributes updates");
                state = STATE_APP_LOOP;
                break;
            }
            ESP_LOGE(TAG, "OTA_CONFIG_FETCHED state, unexpected event received: %d", actual_event);
            set_fw_update_errorcode(ERROR_MQTT_CONFIG_FETCH);
            state = STATE_INITIAL;
            break;

        case STATE_APP_LOOP:
            current_connection_state = connection_state(actual_event, "APP_LOOP");
            if (current_connection_state != STATE_CONNECTION_IS_OK)
            {
                state = current_connection_state;
                break;
            }

            if (actual_event & (WIFI_CONNECTED_EVENT | MQTT_CONNECTED_EVENT))
            {
                ota_events = xEventGroupWaitBits(event_group, OTA_CONFIG_UPDATED_EVENT, false, true, 0);
                if ((ota_events & OTA_CONFIG_UPDATED_EVENT))
                {
                    start_ota(FIRMWARE_VERSION, shared_attributes);
                }
                xEventGroupClearBits(event_group, OTA_CONFIG_UPDATED_EVENT);
                xEventGroupSetBits(event_group, OTA_TASK_IN_NORMAL_STATE_EVENT);
                state = STATE_APP_LOOP;
                break;
            }
            ESP_LOGE(TAG, "APP_LOOP state, unexpected event received: %d", actual_event);
            set_fw_update_errorcode(ERROR_UNKNOWN);
            state = STATE_INITIAL;
            break;

        default:
            ESP_LOGE(TAG, "Unexpected state");
            set_fw_update_errorcode(ERROR_UNKNOWN);
            state = STATE_INITIAL;
            break;
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
void notify_wifi_connected( void )
{
    xEventGroupClearBits(event_group, WIFI_DISCONNECTED_EVENT);
    xEventGroupSetBits(event_group, WIFI_CONNECTED_EVENT);
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
void notify_wifi_disconnected( void )
{
    xEventGroupClearBits(event_group, WIFI_CONNECTED_EVENT);
    xEventGroupSetBits(event_group, WIFI_DISCONNECTED_EVENT);
}
