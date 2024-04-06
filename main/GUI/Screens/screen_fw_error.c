/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "screen_includes.h"

/********************************************************************************************
 *                              DEFINE
 ********************************************************************************************/
#define TAG "SCREEN FW ERROR"

#define SYMBOL_SIGNAL "\uf012"
LV_FONT_DECLARE(signal_20)
/********************************************************************************************
 *                              TYPEDEF
 ********************************************************************************************/

/********************************************************************************************
 *                          GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                          STATIC VARIABLES
 ********************************************************************************************/
lv_obj_t *screen_fwerror;
lv_obj_t *fwerror_parentcont;
lv_obj_t *fwerror_statusbar;
lv_obj_t *fwerror_timelabel;
lv_obj_t *fwerror_batterylabel;
lv_obj_t *fwerror_wifilabel;
lv_obj_t *fwerror_signallabel;
lv_obj_t *fwerror_headingcontainer;
lv_obj_t *fwerror_headinglabel;
lv_obj_t *fwerror_retrybtn;
lv_obj_t *fwerror_retrylabel;
lv_obj_t *fwerror_cancelbtn;
lv_obj_t *fwerror_cancellabel;

lv_obj_t *fwerror_maincontainer;
lv_obj_t *fwerror_label;

lv_obj_t *crnt_screen;
lv_task_t *fw_errorrefresherTask;
/********************************************************************************************
 *                          STATIC PROTOTYPE
 ********************************************************************************************/
static void fw_error_refer_func(lv_task_t *refresherTask);


/********************************************************************************************
 *                          STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *     
 ********************************************************************************************/
static void fw_error_refer_func(lv_task_t *refresherTask)
{
    if (lv_obj_get_screen(fwerror_timelabel) == lv_scr_act())
    {
        lv_label_set_text(fwerror_timelabel, guiTime);
        lv_label_set_text(fwerror_batterylabel, get_battery_symbol());
    }
}

/********************************************************************************************
 *     
 ********************************************************************************************/
static void retry_btn_event_handler(lv_obj_t *obj, lv_event_t event)
{
    device_state_t device_state;
    if (event == LV_EVENT_RELEASED)
    {
        ESP_LOGI(TAG, "retrying for firmware update");
        device_state.fw_update_state = NO_ERROR_FW_UDPATE;
        device_state.device_operating_mode = DO_FIRMWARE_UPDATE;
        bool ret = nvswrite_device_mode_settings( &device_state );
        if ( ret != true )
        {
            ESP_LOGE(TAG, "device mode write error");
        }
        esp_restart();
    }
}

/********************************************************************************************
 *     
 ********************************************************************************************/
static void cancel_btn_event_handler(lv_obj_t *obj, lv_event_t event)
{
    device_state_t device_state;

    if (event == LV_EVENT_RELEASED)
    {
        ESP_LOGI(TAG, "firmware update is cancelled");
        device_state.fw_update_state = NO_ERROR_FW_UDPATE;
        device_state.device_operating_mode = NORMAL_MODE;
        bool ret = nvswrite_device_mode_settings( &device_state );
        if ( ret != true )
        {
            ESP_LOGE(TAG, "device mode write error");
        }
        vTaskDelay(2000/portTICK_PERIOD_MS);
        esp_restart();
    }
}

/********************************************************************************************
 *                          GLOBAL FUNCTIONS
 ********************************************************************************************/
void screen_firmware_error(void)
{
    screen_fwerror = lv_obj_create(NULL, NULL);
    lv_scr_load(screen_fwerror);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    fwerror_parentcont = lv_cont_create(screen_fwerror, NULL);
    lv_obj_set_size(fwerror_parentcont, 320, 480);
    lv_obj_align(fwerror_parentcont, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(fwerror_parentcont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(fwerror_parentcont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(fwerror_parentcont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    fwerror_statusbar = lv_cont_create(fwerror_parentcont, NULL);
    lv_obj_set_size(fwerror_statusbar, 320, 35);
    lv_obj_align(fwerror_statusbar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(fwerror_statusbar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(fwerror_statusbar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    fwerror_timelabel = lv_label_create(fwerror_statusbar, NULL);
    lv_obj_align(fwerror_timelabel, fwerror_statusbar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(fwerror_timelabel, guiTime);

    static lv_style_t TimeLabelStyle;
    lv_style_init(&TimeLabelStyle);
    lv_style_set_text_font(&TimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&TimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_timelabel, LV_LABEL_PART_MAIN, &TimeLabelStyle);

    fw_errorrefresherTask = lv_task_create(fw_error_refer_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create Label for Battery icon
    fwerror_batterylabel = lv_label_create(fwerror_statusbar, NULL);
    lv_obj_align(fwerror_batterylabel, fwerror_statusbar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(fwerror_batterylabel, get_battery_symbol()); 

    static lv_style_t BatteryLabelStyle;
    lv_style_init(&BatteryLabelStyle);
    lv_style_set_text_font(&BatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&BatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_batterylabel, LV_LABEL_PART_MAIN, &BatteryLabelStyle);
    lv_obj_set_hidden(fwerror_batterylabel, true);

    // Create Label for Wifi icon
    fwerror_wifilabel = lv_label_create(fwerror_statusbar, NULL);
    lv_obj_align(fwerror_wifilabel, fwerror_batterylabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(fwerror_wifilabel, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(fwerror_wifilabel, true);

    static lv_style_t WifiLabelStyle;
    lv_style_init(&WifiLabelStyle);
    lv_style_set_text_font(&WifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&WifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_wifilabel, LV_LABEL_PART_MAIN, &WifiLabelStyle);

    // Create Label for Signal icon
    fwerror_signallabel = lv_label_create(fwerror_statusbar, NULL);
    lv_obj_align(fwerror_signallabel, fwerror_wifilabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(fwerror_signallabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(fwerror_signallabel, true);

    static lv_style_t SignalLabelStyle;
    lv_style_init(&SignalLabelStyle);
    lv_style_set_text_font(&SignalLabelStyle, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&SignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_signallabel, LV_LABEL_PART_MAIN, &SignalLabelStyle);

    // Crate a container to contain Info Header
    fwerror_headingcontainer = lv_cont_create(fwerror_parentcont, NULL);
    lv_obj_set_size(fwerror_headingcontainer, 300, 40);
    lv_obj_align(fwerror_headingcontainer, fwerror_statusbar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(fwerror_headingcontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(fwerror_headingcontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for Sequences "Heading"
    fwerror_headinglabel = lv_label_create(fwerror_headingcontainer, NULL);
    lv_obj_align(fwerror_headinglabel, fwerror_headingcontainer, LV_ALIGN_IN_TOP_LEFT, 50, 5);
    lv_label_set_text(fwerror_headinglabel, "Firmware Update");

    static lv_style_t HeadingLblStyle;
    lv_style_init(&HeadingLblStyle);
    lv_style_set_text_font(&HeadingLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&HeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_headinglabel, LV_LABEL_PART_MAIN, &HeadingLblStyle);

    fwerror_maincontainer = lv_cont_create(fwerror_parentcont, NULL);
    lv_obj_set_size(fwerror_maincontainer, 300, 300);
    lv_obj_align(fwerror_maincontainer, fwerror_headingcontainer, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_local_bg_color(fwerror_maincontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(fwerror_maincontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    fwerror_label = lv_label_create(fwerror_maincontainer, NULL);
    lv_obj_set_width(fwerror_label, 300);
    if( get_firmware_update_error() == ERROR_SSID_PASSWORD_NOT_AVAILABLE)
        lv_label_set_text(fwerror_label, "SSID and Password not available");
    else if( get_firmware_update_error() == ERROR_FIRMWARE_DOWNLOAD )
        lv_label_set_text(fwerror_label, "Firmware Download Error");
    else if( get_firmware_update_error() == ERROR_WIFI_CONNECT )
        lv_label_set_text(fwerror_label, "WiFi Connect Error");
    else if( get_firmware_update_error() == ERROR_MQTT_CONNECT )
        lv_label_set_text(fwerror_label, "MQTT Connect Error");
    else if( get_firmware_update_error() == ERROR_MQTT_CONFIG_FETCH )
        lv_label_set_text(fwerror_label, "MQTT Config fetch error");
    else if( get_firmware_update_error() == ERROR_TIMEOUT )
        lv_label_set_text(fwerror_label, "Timeout error");
    else if( get_firmware_update_error() == ERROR_LATEST_FIRMWARE_ALREADY )
        lv_label_set_text(fwerror_label, "Device has latest firmware");
    else
        lv_label_set_text(fwerror_label, "Unknown error");
    lv_label_set_align(fwerror_label, LV_LABEL_ALIGN_CENTER);
    // lv_label_set_long_mode(fwerror_label, LV_LABEL_LONG_BREAK); 
    lv_obj_align(fwerror_label, fwerror_maincontainer, LV_ALIGN_CENTER, 0, -60);
    lv_obj_set_style_local_text_font(fwerror_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_obj_set_style_local_text_color(fwerror_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

    fwerror_retrybtn = lv_btn_create(fwerror_maincontainer, NULL);
    lv_obj_align(fwerror_retrybtn, fwerror_maincontainer, LV_ALIGN_CENTER, 0, 30);
    lv_obj_set_size(fwerror_retrybtn, 150, 44);
    lv_obj_set_event_cb(fwerror_retrybtn, retry_btn_event_handler); 

    static lv_style_t retrybtnstyle;
    lv_style_init(&retrybtnstyle);
    lv_style_set_radius(&retrybtnstyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&retrybtnstyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #359Fe2
    lv_style_set_border_width(&retrybtnstyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&retrybtnstyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(fwerror_retrybtn, LV_BTN_PART_MAIN, &retrybtnstyle);

    fwerror_retrylabel = lv_label_create(fwerror_retrybtn, NULL);
    lv_obj_align(fwerror_retrylabel, fwerror_retrybtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(fwerror_retrylabel, "RETRY");

    static lv_style_t retrylabelstyle;
    lv_style_init(&retrylabelstyle);
    lv_style_set_text_font(&retrylabelstyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&retrylabelstyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_retrylabel, LV_LABEL_PART_MAIN, &retrylabelstyle);


    fwerror_cancelbtn = lv_btn_create(fwerror_maincontainer, NULL);
    lv_obj_align(fwerror_cancelbtn, fwerror_maincontainer, LV_ALIGN_CENTER, 0, 100);
    lv_obj_set_size(fwerror_cancelbtn, 150, 44);
    lv_obj_set_event_cb(fwerror_cancelbtn, cancel_btn_event_handler); 

    static lv_style_t cancelbtnstyle;
    lv_style_init(&cancelbtnstyle);
    lv_style_set_radius(&cancelbtnstyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&cancelbtnstyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #359Fe2
    lv_style_set_border_width(&cancelbtnstyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&cancelbtnstyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(fwerror_cancelbtn, LV_BTN_PART_MAIN, &cancelbtnstyle);

    fwerror_cancellabel = lv_label_create(fwerror_cancelbtn, NULL);
    lv_obj_align(fwerror_cancellabel, fwerror_cancelbtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(fwerror_cancellabel, "CANCEL");

    static lv_style_t cancellabelstyle;
    lv_style_init(&cancellabelstyle);
    lv_style_set_text_font(&cancellabelstyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&cancellabelstyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(fwerror_cancellabel, LV_LABEL_PART_MAIN, &cancellabelstyle);

    crnt_screen = screen_fwerror;
    screenid = SCR_FW_ERROR;
}