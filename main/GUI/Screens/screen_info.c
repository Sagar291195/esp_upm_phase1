/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-04-01
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"
#include "../lv_lib_qrcode/lv_qrcode.h"

/*********************
 *      DEFINES
 *********************/

#define TAG "SCREEN INFO"
#define SYMBOL_SIGNAL "\uf012"

// Declare Fonts here
LV_FONT_DECLARE(signal_20)

// Declare Images Here
LV_IMG_DECLARE(left_arrow_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void infoTimeLabel_refr_func(lv_task_t *inforefresherTask);
static void _infoBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *infoParentCont;
lv_obj_t *crnt_screen;
lv_obj_t *scrInfo;
lv_obj_t *_infoContStatusBar;
lv_obj_t *infoTimeLabel;
lv_obj_t *_infoBatteryLabel;
lv_obj_t *_infoWifiLabel;
lv_obj_t *_infoSignalLabel;
lv_obj_t *_infoHeadingCont;
lv_obj_t *_infoBackArrowLabel;
lv_obj_t *_infoHedingLbl;
lv_obj_t *_infoQRCont;
lv_obj_t *_infoDeviceNumTxtLbl;
lv_obj_t *_infoDeviceNumValLbl;
lv_obj_t *_infoFWVersionTxtLbl;
lv_obj_t *_infoFWVersionValLbl;
lv_obj_t *_infoLabNameTxtLbl;
lv_obj_t *_infoLabNameValLbl;
lv_obj_t *_infoQRcodeImg;
lv_obj_t *_infoDeviceIDTxtLbl;
lv_obj_t *_infoDeviceIDValLbl;
lv_obj_t *info_firmwaredownload;
lv_obj_t *info_firmwaredownloadlabel;
lv_obj_t * mbox1;
lv_task_t *inforefresherTask;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        if(strcmp( "OK", lv_msgbox_get_active_btn_text(obj)) == 0)
        {
            ESP_LOGI(TAG, "Device will be restarting in 5 sec");
            static device_state_t device_state;
            device_state.device_operating_mode = DO_FIRMWARE_UPDATE;
            bool ret = nvswrite_device_mode_settings( &device_state );
            if ( ret == false )
            {
                ESP_LOGE( TAG, "device mode settings write error" );
            }
            vTaskDelay(2000/portTICK_PERIOD_MS);
            esp_restart();
        }
        else if( strcmp( "CANCEL", lv_msgbox_get_active_btn_text(obj)) == 0)
        {
            ESP_LOGI(TAG, "firmware update is canceled by user");
            lv_obj_set_hidden(mbox1, true);
        }
    }
}

static void firmware_download_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_obj_set_hidden(mbox1, false);
    }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void CallScreenInfo(void)
{
    ESP_LOGI(TAG, "Loading Screen");
    scrInfo = lv_obj_create(NULL, NULL);
    lv_scr_load(scrInfo);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    infoParentCont = lv_cont_create(scrInfo, NULL);
    lv_obj_set_size(infoParentCont, 320, 480);
    lv_obj_align(infoParentCont, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(infoParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(infoParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(infoParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _infoContStatusBar = lv_cont_create(infoParentCont, NULL);
    lv_obj_set_size(_infoContStatusBar, 320, 35);
    lv_obj_align(_infoContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_infoContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_infoContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    infoTimeLabel = lv_label_create(_infoContStatusBar, NULL);
    lv_obj_align(infoTimeLabel, _infoContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(infoTimeLabel, guiTime);

    static lv_style_t infoTimeLabelStyle;
    lv_style_init(&infoTimeLabelStyle);
    lv_style_set_text_font(&infoTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&infoTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(infoTimeLabel, LV_LABEL_PART_MAIN, &infoTimeLabelStyle);

    inforefresherTask = lv_task_create(infoTimeLabel_refr_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create Label for Battery icon
    _infoBatteryLabel = lv_label_create(_infoContStatusBar, NULL);
    lv_obj_align(_infoBatteryLabel, _infoContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(_infoBatteryLabel, get_battery_symbol());

    static lv_style_t _smmBatteryLabelStyle;
    lv_style_init(&_smmBatteryLabelStyle);
    lv_style_set_text_font(&_smmBatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_smmBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoBatteryLabel, LV_LABEL_PART_MAIN, &_smmBatteryLabelStyle);

    // Create Label for Wifi icon
    _infoWifiLabel = lv_label_create(_infoContStatusBar, NULL);
    lv_obj_align(_infoWifiLabel, _infoBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(_infoWifiLabel, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(_infoWifiLabel, true);

    static lv_style_t infoWifiLabelStyle;
    lv_style_init(&infoWifiLabelStyle);
    lv_style_set_text_font(&infoWifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&infoWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoWifiLabel, LV_LABEL_PART_MAIN, &infoWifiLabelStyle);

    // Create Label for Signal icon
    _infoSignalLabel = lv_label_create(_infoContStatusBar, NULL);
    lv_obj_align(_infoSignalLabel, _infoWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(_infoSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(_infoSignalLabel, true);

    static lv_style_t _infoSignalLabelStyle;
    lv_style_init(&_infoSignalLabelStyle);
    lv_style_set_text_font(&_infoSignalLabelStyle, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_infoSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoSignalLabel, LV_LABEL_PART_MAIN, &_infoSignalLabelStyle);

    //===============================================================================================================

    // Crate a container to contain Info Header
    _infoHeadingCont = lv_cont_create(infoParentCont, NULL);
    lv_obj_set_size(_infoHeadingCont, 300, 70);
    lv_obj_align(_infoHeadingCont, _infoContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_infoHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_infoHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Back arrow img
    _infoBackArrowLabel = lv_img_create(_infoHeadingCont, NULL);
    lv_img_set_src(_infoBackArrowLabel, &left_arrow_icon);
    lv_obj_align(_infoBackArrowLabel, _infoHeadingCont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_obj_set_click(_infoBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(_infoBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_infoBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_infoBackArrowLabel, _infoBackArrow_event_handler);

    // Create Label for Sequences "Heading"
    _infoHedingLbl = lv_label_create(_infoHeadingCont, NULL);
    lv_obj_align(_infoHedingLbl, _infoHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -10, -35);
    lv_label_set_text(_infoHedingLbl, "Info");

    static lv_style_t _infoHeadingLblStyle;
    lv_style_init(&_infoHeadingLblStyle);
    lv_style_set_text_font(&_infoHeadingLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&_infoHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoHedingLbl, LV_LABEL_PART_MAIN, &_infoHeadingLblStyle);

    // Create a container for QR Image
    _infoQRCont = lv_cont_create(infoParentCont, NULL);
    lv_obj_set_size(_infoQRCont, 300, 310);
    lv_obj_align(_infoQRCont, _infoHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_local_bg_color(_infoQRCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_infoQRCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create a label for "DEVICE NUMBER" text
    _infoDeviceNumTxtLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoDeviceNumTxtLbl, _infoQRCont, LV_ALIGN_IN_TOP_LEFT, 10, 5);
    lv_label_set_text(_infoDeviceNumTxtLbl, "Device Number:");

    static lv_style_t _infoDeviceNumTxtLblStyle;
    lv_style_init(&_infoDeviceNumTxtLblStyle);
    lv_style_set_text_font(&_infoDeviceNumTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infoDeviceNumTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_infoDeviceNumTxtLbl, LV_LABEL_PART_MAIN, &_infoDeviceNumTxtLblStyle);

    // Create a label for Device number
    _infoDeviceNumValLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoDeviceNumValLbl, _infoDeviceNumTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 100, 0);
    lv_label_set_text(_infoDeviceNumValLbl, devicesettings.device_serial_number);

    static lv_style_t _infoDeviceNumValLblStyle;
    lv_style_init(&_infoDeviceNumValLblStyle);
    lv_style_set_text_font(&_infoDeviceNumValLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infoDeviceNumValLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoDeviceNumValLbl, LV_LABEL_PART_MAIN, &_infoDeviceNumValLblStyle);

    _infoFWVersionTxtLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoFWVersionTxtLbl, _infoDeviceNumTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    lv_label_set_text(_infoFWVersionTxtLbl, "Firmware Version:");

    static lv_style_t _infofwversionTxtLblStyle;
    lv_style_init(&_infofwversionTxtLblStyle);
    lv_style_set_text_font(&_infofwversionTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infofwversionTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_infoFWVersionTxtLbl, LV_LABEL_PART_MAIN, &_infofwversionTxtLblStyle);

    _infoFWVersionValLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoFWVersionValLbl, _infoFWVersionTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 90, 0);
    lv_label_set_text(_infoFWVersionValLbl, FIRMWARE_VERSION);

    static lv_style_t _infofwversionValLblStyle;
    lv_style_init(&_infofwversionValLblStyle);
    lv_style_set_text_font(&_infofwversionValLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infofwversionValLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoFWVersionValLbl, LV_LABEL_PART_MAIN, &_infofwversionValLblStyle);

    // Create a label for Lab Name
    _infoLabNameTxtLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoLabNameTxtLbl, _infoFWVersionTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    lv_label_set_text(_infoLabNameTxtLbl, "Lab Name:");

    static lv_style_t _infoLabNameTxtLblStyle;
    lv_style_init(&_infoLabNameTxtLblStyle);
    lv_style_set_text_font(&_infoLabNameTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infoLabNameTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_infoLabNameTxtLbl, LV_LABEL_PART_MAIN, &_infoLabNameTxtLblStyle);

    // Create a label for lab number
    _infoLabNameValLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoLabNameValLbl, _infoLabNameTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 140, 0);
    lv_label_set_text(_infoLabNameValLbl, "1356");

    static lv_style_t _infoLabNameValLblStyle;
    lv_style_init(&_infoLabNameValLblStyle);
    lv_style_set_text_font(&_infoLabNameValLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infoLabNameValLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoLabNameValLbl, LV_LABEL_PART_MAIN, &_infoLabNameValLblStyle);

    // Put QR image here
    _infoQRcodeImg = lv_qrcode_create(_infoQRCont, 50, 80, 200, LV_COLOR_BLACK, LV_COLOR_WHITE);
    lv_qrcode_update(_infoQRcodeImg, "hello", strlen("hello"));

    // Create label for "DEVICE ID" Text
    _infoDeviceIDTxtLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoDeviceIDTxtLbl, _infoQRcodeImg, LV_ALIGN_OUT_BOTTOM_LEFT, 50, 8);
    lv_label_set_text(_infoDeviceIDTxtLbl, "DEVICE ID:");

    static lv_style_t _infoDeviceIDTxtLblStyle;
    lv_style_init(&_infoDeviceIDTxtLblStyle);
    lv_style_set_text_font(&_infoDeviceIDTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infoDeviceIDTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_infoDeviceIDTxtLbl, LV_LABEL_PART_MAIN, &_infoDeviceIDTxtLblStyle);

    // Create a label for device ID number
    _infoDeviceIDValLbl = lv_label_create(_infoQRCont, NULL);
    lv_obj_align(_infoDeviceIDValLbl, _infoDeviceIDTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
    lv_label_set_text(_infoDeviceIDValLbl, devicesettings.device_serial_number);

    static lv_style_t _infoDeviceIDValLblStyle;
    lv_style_init(&_infoDeviceIDValLblStyle);
    lv_style_set_text_font(&_infoDeviceIDValLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_infoDeviceIDValLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_infoDeviceIDValLbl, LV_LABEL_PART_MAIN, &_infoDeviceIDValLblStyle);

    info_firmwaredownload = lv_btn_create(infoParentCont, NULL);
    lv_obj_align(info_firmwaredownload, _infoQRCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_obj_set_size(info_firmwaredownload, 300, 44);
    lv_obj_set_event_cb(info_firmwaredownload, firmware_download_event_handler); // Flow Calibration

    static lv_style_t firmwarebtnstyle;
    lv_style_init(&firmwarebtnstyle);
    lv_style_set_radius(&firmwarebtnstyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&firmwarebtnstyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #359Fe2
    lv_style_set_border_width(&firmwarebtnstyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&firmwarebtnstyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(info_firmwaredownload, LV_BTN_PART_MAIN, &firmwarebtnstyle);

    // Creat a Sensor Parameter Button Label
    info_firmwaredownloadlabel = lv_label_create(info_firmwaredownload, NULL);
    lv_obj_align(info_firmwaredownloadlabel, info_firmwaredownload, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(info_firmwaredownloadlabel, "FW UPDATE");

    static lv_style_t fwdownloadlabelstyle;
    lv_style_init(&fwdownloadlabelstyle);
    lv_style_set_text_font(&fwdownloadlabelstyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&fwdownloadlabelstyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(info_firmwaredownloadlabel, LV_LABEL_PART_MAIN, &fwdownloadlabelstyle);

    static const char * btns[] ={"OK", "CANCEL", ""};
    mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox1, "Device will be restared during firmwar update, Click OK to continue or Abort to cancel...");
    lv_msgbox_add_btns(mbox1, btns);
    lv_obj_set_width(mbox1, 300);
    lv_obj_set_event_cb(mbox1, event_handler);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
    lv_obj_set_hidden(mbox1, true);

    crnt_screen = scrInfo;
    screenid = SCR_INFO;
}

static void infoTimeLabel_refr_func(lv_task_t *inforefresherTask)
{
    if (lv_obj_get_screen(infoTimeLabel) == lv_scr_act())
    {
        lv_label_set_text(infoTimeLabel, guiTime);
        lv_label_set_text(_infoBatteryLabel, get_battery_symbol());
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void _infoBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(inforefresherTask);
        inforefresherTask = NULL;
        pxDashboardScreen();
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
