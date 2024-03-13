

/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"


/*********************
 *   DEFINES/CONST
 *********************/
#define TAG "PARAMETER"

#define SYMBOL_SIGNAL "\uf012"
#define _ParaContWidth 65
#define _ParaContHeight 300
#define txt_y_align 10

#define _xColorBGParas LV_COLOR_MAKE(0x38, 0x38, 0x38) // LV_COLOR_MAKE(0x5f, 0x61, 0x38)

/*********************
 *   DECLARATION
 *********************/
// Declare Fonts here
LV_FONT_DECLARE(signal_20)

// Declare Images Here
LV_IMG_DECLARE(parameter_icon)
LV_IMG_DECLARE(led_icon)
LV_IMG_DECLARE(wifi_icon)
LV_IMG_DECLARE(fan_icon)
LV_IMG_DECLARE(sleep_icon)
LV_IMG_DECLARE(language_icon)
LV_IMG_DECLARE(timer_icon)
LV_IMG_DECLARE(luminosity_icon)
LV_IMG_DECLARE(contrast_icon)
LV_IMG_DECLARE(left_arrow_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

lv_obj_t *crnt_screen;
lv_obj_t *scrParameter;
lv_obj_t *xParentcontainer_par;
lv_obj_t *container_par;
lv_obj_t *_xContainerStatusBar_par;
lv_obj_t *_xTimeLabel_par;
lv_obj_t *_xBatteryLabel_par;
lv_obj_t *_xWifiLabel_par;
lv_obj_t *_xSignalLabel_par;
lv_obj_t *_xParaLabelContainer_par;
lv_obj_t *_xParaHeadingCont_par;
lv_obj_t *_xBackArrowLabel_par1;
lv_obj_t *_xParameterHeading_par;
lv_obj_t *_xParameterIcon_par;
lv_obj_t *_xBaseContainer_par;
lv_obj_t *_xParaBuzzerCont_par;
lv_obj_t *_xBuzzerIconLabel_par;
lv_obj_t *_xBuzzerTextLabel_par;
lv_obj_t *_xBuzzerOnOffTextLabel_par;
lv_obj_t *xBuzzerSwitch;
lv_obj_t *_xParaLedCont_par;
lv_obj_t *_xLedIcon_par;
lv_obj_t *_xLedTextLabel_par;
lv_obj_t *_xLedOnOffTextLabel_par;
lv_obj_t *xLedSwitch;
lv_obj_t *_xParaWiFiCont_par;
lv_obj_t *_xWiFiIcon_par;
lv_obj_t *_xWiFiTextLabel_par;
lv_obj_t *_xWiFiOnOffTextLabel_par;
lv_obj_t *xWiFiSwitch;
lv_obj_t *_xParaFanCont_par;
lv_obj_t *_xFanIcon_par;
lv_obj_t *_xFanTextLabel_par;
lv_obj_t *_xFanOnOffTextLabel_par;
lv_obj_t *xFanSwitch;
lv_obj_t *_xParaSleepCont_par;
lv_obj_t *_xSleepIcon_par;
lv_obj_t *_xSleepTextLabel_par;
lv_obj_t *_xSleepOnOffTextLabel_par;
lv_obj_t *xSleepSwitch;
lv_obj_t *_xParaLangCont_par;
lv_obj_t *_xLangIcon_par;
lv_obj_t *_xLangTextLabel_par;
lv_obj_t *_xSpeakLangTextLabel_par;
lv_obj_t *_xLangDropDown_par;
lv_obj_t *_xParaSlpTmrCont_par;
lv_obj_t *_xSlpTmrIcon_par;
lv_obj_t *_xSlpTmrTextLabel_par;
lv_obj_t *_xAdjTmrTextLabel_par;
lv_obj_t *_xTimerDropDown_par;
lv_obj_t *_xParaLuminCont_par;
lv_obj_t *_xLuminIcon_par;
lv_obj_t *_xLuminTextLabel_par;
lv_obj_t *_xAdjLuminTextLabel_par;
lv_obj_t *_xLuminslider_par;
lv_obj_t *_xParaContrastCont_par;
lv_obj_t *_xContrastIcon_par;
lv_obj_t *_xContrastTextLabel_par;
lv_obj_t *_xAdjContrastTextLabel_par;
lv_obj_t *_xContrastslider_par;
lv_obj_t *_xBackArrowCont;

lv_task_t *param_refresherTask;
/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static void param_refer_func(lv_task_t *refresherTask)
{
    if (lv_obj_get_screen(_xTimeLabel_par) == lv_scr_act())
    {
        lv_label_set_text(_xTimeLabel_par, guiTime);
        lv_label_set_text(_xBatteryLabel_par, get_battery_symbol());
    }
}

static void lang_DD_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.selected_language = lv_dropdown_get_selected(obj);
        nvswrite_device_settings(&devicesettings);
    }
}

static void SlpTmr_DD_event_handler(lv_obj_t *obj, lv_event_t event)
{
    uint8_t selectedoption = 0;
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        selectedoption = lv_dropdown_get_selected(obj);
        if(selectedoption == 0)
            devicesettings.screen_timeout_value = 15;
        else if(selectedoption == 1)
            devicesettings.screen_timeout_value = 30;
        else if(selectedoption == 2)
            devicesettings.screen_timeout_value = 60;  
        nvswrite_device_settings(&devicesettings);          
    }
}

// Lumin_Slider_event_handler

static void Lumin_Slider_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.luminosity_value = lv_slider_get_value(obj);
        nvswrite_device_settings(&devicesettings);
    }
}

// Contrast_Slider_event_handler

static void Contrast_Slider_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.contrast_value = lv_slider_get_value(obj);
        lcd_set_contrast(devicesettings.contrast_value);
        nvswrite_device_settings(&devicesettings);
    }
}

//_xBackArrow_event_handler
static void _xBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(param_refresherTask);
        param_refresherTask = NULL;
        pxDashboardScreen();
    }
}

static void Buzzer_switch_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.buzzer_enable = lv_switch_get_state(obj);
        if(devicesettings.buzzer_enable == 1)
        {
            buzzer_initialization();
        }
        nvswrite_device_settings(&devicesettings);
    }
}

static void Led_Switch_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.led_enable = lv_switch_get_state(obj);
        if(devicesettings.led_enable == 1)
        {
            ESP_LOGI(TAG, "enabling the es2812 pin");
            ws2812_init(13);
        }
        else
        {
            ESP_LOGI(TAG, "disabling the pin");
            ws2812_disable();
        }
        nvswrite_device_settings(&devicesettings);
    }
}

static void WiFi_Switch_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.wifi_enable = lv_switch_get_state(obj);
        nvswrite_device_settings(&devicesettings);
    }
}

static void Fan_Switch_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.external_fan_enable = lv_switch_get_state(obj);
        nvswrite_device_settings(&devicesettings);
    }
}

static void Sleep_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        devicesettings.screen_sleepmode_enable = lv_switch_get_state(obj);
        nvswrite_device_settings(&devicesettings);
    }
}

void ppxParameterScreen(void)
{
    // Parent Container
    scrParameter = lv_obj_create(NULL, NULL);
    lv_scr_load(scrParameter);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    xParentcontainer_par = lv_cont_create(scrParameter, NULL);
    lv_obj_set_size(xParentcontainer_par, 320, 480);
    lv_obj_align(xParentcontainer_par, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(xParentcontainer_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(xParentcontainer_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(xParentcontainer_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    container_par = lv_cont_create(xParentcontainer_par, NULL);
    lv_obj_set_size(container_par, 320, 480);
    lv_obj_align(container_par, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(container_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(container_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(container_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _xContainerStatusBar_par = lv_cont_create(container_par, NULL);
    lv_obj_set_size(_xContainerStatusBar_par, 320, 35);
    lv_obj_align(_xContainerStatusBar_par, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xContainerStatusBar_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(_xContainerStatusBar_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    _xTimeLabel_par = lv_label_create(_xContainerStatusBar_par, NULL);
    lv_obj_align(_xTimeLabel_par, _xContainerStatusBar_par, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(_xTimeLabel_par, guiTime);

    static lv_style_t _xTimeLabelStyle_par;
    lv_style_init(&_xTimeLabelStyle_par);
    lv_style_set_text_font(&_xTimeLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xTimeLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xTimeLabel_par, LV_LABEL_PART_MAIN, &_xTimeLabelStyle_par);

    // Create Label for Battery icon
    _xBatteryLabel_par = lv_label_create(_xContainerStatusBar_par, NULL);
    lv_obj_align(_xBatteryLabel_par, _xContainerStatusBar_par, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(_xBatteryLabel_par, get_battery_symbol());

    static lv_style_t _xBatteryLabelStyle_par;
    lv_style_init(&_xBatteryLabelStyle_par);
    lv_style_set_text_font(&_xBatteryLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_xBatteryLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xBatteryLabel_par, LV_LABEL_PART_MAIN, &_xBatteryLabelStyle_par);

    // Create Label for Wifi icon
    _xWifiLabel_par = lv_label_create(_xContainerStatusBar_par, NULL);
    lv_obj_align(_xWifiLabel_par, _xBatteryLabel_par, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(_xWifiLabel_par, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(_xWifiLabel_par, true);

    static lv_style_t _xWifiLabelStyle_par;
    lv_style_init(&_xWifiLabelStyle_par);
    lv_style_set_text_font(&_xWifiLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xWifiLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xWifiLabel_par, LV_LABEL_PART_MAIN, &_xWifiLabelStyle_par);

    // Create Label for Signal icon
    _xSignalLabel_par = lv_label_create(_xContainerStatusBar_par, NULL);
    lv_obj_align(_xSignalLabel_par, _xWifiLabel_par, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(_xSignalLabel_par, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(_xSignalLabel_par, true);

    static lv_style_t _xSignalLabelStyle_par;
    lv_style_init(&_xSignalLabelStyle_par);
    lv_style_set_text_font(&_xSignalLabelStyle_par, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xSignalLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xSignalLabel_par, LV_LABEL_PART_MAIN, &_xSignalLabelStyle_par);

    param_refresherTask = lv_task_create(param_refer_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create a container to put all the parameter
    _xParaLabelContainer_par = lv_page_create(container_par, NULL);
    lv_obj_set_size(_xParaLabelContainer_par, 320, 430);
    lv_obj_align(_xParaLabelContainer_par, _xContainerStatusBar_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);
    lv_page_set_scrollbar_mode(_xParaLabelContainer_par, LV_SCROLLBAR_MODE_HIDE);
    lv_obj_set_style_local_bg_color(_xParaLabelContainer_par, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_width(_xParaLabelContainer_par, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    // Create a container to contain Parameter Header
    _xParaHeadingCont_par = lv_cont_create(_xParaLabelContainer_par, NULL);
    lv_obj_set_size(_xParaHeadingCont_par, 300, 70);
    lv_obj_set_width(_xParaHeadingCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaHeadingCont_par, _xParaLabelContainer_par, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_bg_color(_xParaHeadingCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xParaHeadingCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Back Arrow Container
    _xBackArrowCont = lv_cont_create(_xParaHeadingCont_par, NULL);
    lv_obj_set_size(_xBackArrowCont, 60, 60);
    // lv_obj_set_width(_xBackArrowCont, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xBackArrowCont, _xParaHeadingCont_par, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_obj_set_style_local_bg_color(_xBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_xBackArrowCont, _xBackArrow_event_handler);

    // Create back arrow image
    _xBackArrowLabel_par1 = lv_img_create(_xBackArrowCont, NULL);
    lv_img_set_src(_xBackArrowLabel_par1, &left_arrow_icon);
    lv_obj_align(_xBackArrowLabel_par1, _xBackArrowCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(_xBackArrowLabel_par1, true);
    lv_obj_set_style_local_image_recolor_opa(_xBackArrowLabel_par1, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xBackArrowLabel_par1, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xBackArrowLabel_par1, _xBackArrow_event_handler);

    // Create Label for Parameter "Heading"
    _xParameterHeading_par = lv_label_create(_xParaHeadingCont_par, NULL);
    lv_obj_align(_xParameterHeading_par, _xParaHeadingCont_par, LV_ALIGN_IN_BOTTOM_MID, -50, -35);
    lv_label_set_text(_xParameterHeading_par, "Parameter");

    static lv_style_t _xParameterHeadingStyle_par;
    lv_style_init(&_xParameterHeadingStyle_par);
    lv_style_set_text_font(&_xParameterHeadingStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&_xParameterHeadingStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xParameterHeading_par, LV_LABEL_PART_MAIN, &_xParameterHeadingStyle_par);

    // Create Parameter logo
    _xParameterIcon_par = lv_img_create(_xParaHeadingCont_par, NULL);
    lv_img_set_src(_xParameterIcon_par, &parameter_icon);
    lv_obj_align(_xParameterIcon_par, _xParaHeadingCont_par, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_obj_set_style_local_image_recolor_opa(_xParameterIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xParameterIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    _xBaseContainer_par = lv_cont_create(_xParaLabelContainer_par, NULL);
    lv_obj_set_size(_xBaseContainer_par, 300, 670);
    lv_obj_set_click(_xBaseContainer_par, false);
    lv_obj_set_width(_xBaseContainer_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xBaseContainer_par, _xParaHeadingCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_set_style_local_bg_color(_xBaseContainer_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
    lv_obj_set_style_local_border_width(_xBaseContainer_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create a container to contain buzzer logo, text & toggle switch
    _xParaBuzzerCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaBuzzerCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaBuzzerCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaBuzzerCont_par, _xParaHeadingCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_set_style_local_bg_color(_xParaBuzzerCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
    lv_obj_set_style_local_border_width(_xParaBuzzerCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Ctreate label for buzzer icon
    _xBuzzerIconLabel_par = lv_label_create(_xParaBuzzerCont_par, NULL);
    lv_obj_align(_xBuzzerIconLabel_par, _xParaBuzzerCont_par, LV_ALIGN_IN_LEFT_MID, 5, -10);
    lv_label_set_text(_xBuzzerIconLabel_par, LV_SYMBOL_VOLUME_MAX); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xBuzzerIconLabelStyle_par;
    lv_style_init(&_xBuzzerIconLabelStyle_par);
    lv_style_set_text_font(&_xBuzzerIconLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_30);
    lv_style_set_text_color(&_xBuzzerIconLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xBuzzerIconLabel_par, LV_LABEL_PART_MAIN, &_xBuzzerIconLabelStyle_par);

    // Create lavel for BUZZER Text
    _xBuzzerTextLabel_par = lv_label_create(_xParaBuzzerCont_par, NULL);
    lv_obj_align(_xBuzzerTextLabel_par, _xParaBuzzerCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align); // #define txt_y_align 10
    lv_label_set_text(_xBuzzerTextLabel_par, "BUZZER");                                               // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xBuzzerTextLabelStyle_par;
    lv_style_init(&_xBuzzerTextLabelStyle_par);
    lv_style_set_text_font(&_xBuzzerTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xBuzzerTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xBuzzerTextLabel_par, LV_LABEL_PART_MAIN, &_xBuzzerTextLabelStyle_par);

    // Create lavel for BUZZER ON/OFF Text
    _xBuzzerOnOffTextLabel_par = lv_label_create(_xParaBuzzerCont_par, NULL);
    lv_obj_align(_xBuzzerOnOffTextLabel_par, _xBuzzerTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xBuzzerOnOffTextLabel_par, "ON/OFF"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xBuzzerOnOffTextLabelStyle_par;
    lv_style_init(&_xBuzzerOnOffTextLabelStyle_par);
    lv_style_set_text_font(&_xBuzzerOnOffTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xBuzzerOnOffTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xBuzzerOnOffTextLabel_par, LV_LABEL_PART_MAIN, &_xBuzzerOnOffTextLabelStyle_par);

    // Create buzzer control switch
    xBuzzerSwitch = lv_switch_create(_xParaBuzzerCont_par, NULL);
    lv_obj_align(xBuzzerSwitch, _xParaBuzzerCont_par, LV_ALIGN_IN_RIGHT_MID, -30, 0);
    lv_obj_set_height(xBuzzerSwitch, 25);
    lv_obj_set_event_cb(xBuzzerSwitch, Buzzer_switch_event_handler);

    static lv_style_t _xSwitchStle;
    lv_style_init(&_xSwitchStle);
    lv_style_set_bg_color(&_xSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_width(&_xSwitchStle, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&_xSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_WHITE);
    lv_obj_add_style(xBuzzerSwitch, LV_SWITCH_PART_BG, &_xSwitchStle);

    static lv_style_t _xSwitchStle1;
    lv_style_init(&_xSwitchStle1);
    // lv_style_set_bg_color(&_xSwitchStle1, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_color(&_xSwitchStle1, LV_SWITCH_PART_INDIC, LV_COLOR_GREEN);
    lv_obj_add_style(xBuzzerSwitch, LV_SWITCH_PART_INDIC, &_xSwitchStle1);
    if( devicesettings.buzzer_enable ==  1)
        lv_switch_on(xBuzzerSwitch, LV_ANIM_OFF);
    else    
        lv_switch_off(xBuzzerSwitch, LV_ANIM_OFF);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line, p, 2);
    lv_obj_align(hor_line, _xParaBuzzerCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    static lv_style_t style_hor_line;
    lv_style_init(&style_hor_line);
    lv_style_set_line_color(&style_hor_line, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
    lv_style_set_line_width(&style_hor_line, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line, LV_LINE_PART_MAIN, &style_hor_line);

    // Create a container to contain Led logo, LED text, ON/OFF & switch
    _xParaLedCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaLedCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaLedCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaLedCont_par, hor_line, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaLedCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas
    lv_obj_set_style_local_border_width(_xParaLedCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create LED logo
    _xLedIcon_par = lv_img_create(_xParaLedCont_par, NULL);
    lv_img_set_src(_xLedIcon_par, &led_icon);
    lv_obj_align(_xLedIcon_par, _xParaLedCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xLedIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xLedIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for LED Text
    _xLedTextLabel_par = lv_label_create(_xParaLedCont_par, NULL);
    lv_obj_align(_xLedTextLabel_par, _xParaLedCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xLedTextLabel_par, "LED"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xLedTextLabelStyle_par;
    lv_style_init(&_xLedTextLabelStyle_par);
    lv_style_set_text_font(&_xLedTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xLedTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xLedTextLabel_par, LV_LABEL_PART_MAIN, &_xLedTextLabelStyle_par);

    // Create lavel for LED ON/OFF Text
    _xLedOnOffTextLabel_par = lv_label_create(_xParaLedCont_par, NULL);
    lv_obj_align(_xLedOnOffTextLabel_par, _xLedTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xLedOnOffTextLabel_par, "ON/OFF"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xParaLedContStyle_par;
    lv_style_init(&_xParaLedContStyle_par);
    lv_style_set_text_font(&_xParaLedContStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xParaLedContStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xParaLedCont_par, LV_LABEL_PART_MAIN, &_xParaLedContStyle_par);

    // Create LED control switch
    xLedSwitch = lv_switch_create(_xParaLedCont_par, NULL);
    lv_obj_align(xLedSwitch, _xParaLedCont_par, LV_ALIGN_IN_RIGHT_MID, -30, 0);
    lv_obj_set_height(xLedSwitch, 25);
    lv_obj_add_style(xLedSwitch, LV_LABEL_PART_MAIN, &_xSwitchStle);
    lv_obj_add_style(xLedSwitch, LV_SWITCH_PART_INDIC, &_xSwitchStle1);
    lv_obj_set_event_cb(xLedSwitch, Led_Switch_event_handler);

    if( devicesettings.led_enable ==  1)
        lv_switch_on(xLedSwitch, LV_ANIM_OFF);
    else    
        lv_switch_off(xLedSwitch, LV_ANIM_OFF);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line1 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p1[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line1, p1, 2);
    lv_obj_align(hor_line1, _xParaLedCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    static lv_style_t style_hor_line1;
    lv_style_init(&style_hor_line1);
    lv_style_set_line_color(&style_hor_line1, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
    lv_style_set_line_width(&style_hor_line1, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line1, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line1, LV_LINE_PART_MAIN, &style_hor_line1);

    // Create a container to contain WiFi logo, Wifi text, ON/OFF & switch
    _xParaWiFiCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaWiFiCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaWiFiCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaWiFiCont_par, hor_line1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaWiFiCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaWiFiCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Wifi logo
    _xWiFiIcon_par = lv_img_create(_xParaWiFiCont_par, NULL);
    lv_img_set_src(_xWiFiIcon_par, &wifi_icon);
    lv_obj_align(_xWiFiIcon_par, _xParaWiFiCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xWiFiIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xWiFiIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for WIFI Text
    _xWiFiTextLabel_par = lv_label_create(_xParaWiFiCont_par, NULL);
    lv_obj_align(_xWiFiTextLabel_par, _xParaWiFiCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xWiFiTextLabel_par, "WiFi"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xWiFiTextLabelStyle_par;
    lv_style_init(&_xWiFiTextLabelStyle_par);
    lv_style_set_text_font(&_xWiFiTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xWiFiTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xWiFiTextLabel_par, LV_LABEL_PART_MAIN, &_xWiFiTextLabelStyle_par);

    // Create lavel for LED ON/OFF Text
    _xWiFiOnOffTextLabel_par = lv_label_create(_xParaWiFiCont_par, NULL);
    lv_obj_align(_xWiFiOnOffTextLabel_par, _xWiFiTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xWiFiOnOffTextLabel_par, "ON/OFF"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xParaWiFiContStyle_par;
    lv_style_init(&_xParaWiFiContStyle_par);
    lv_style_set_text_font(&_xParaWiFiContStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xParaWiFiContStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xParaWiFiCont_par, LV_LABEL_PART_MAIN, &_xParaWiFiContStyle_par);

    // Create WiFi control switch
    xWiFiSwitch = lv_switch_create(_xParaWiFiCont_par, NULL);
    lv_obj_align(xWiFiSwitch, _xParaWiFiCont_par, LV_ALIGN_IN_RIGHT_MID, -30, 0);
    lv_obj_set_height(xWiFiSwitch, 25);
    lv_obj_add_style(xWiFiSwitch, LV_LABEL_PART_MAIN, &_xSwitchStle);
    lv_obj_add_style(xWiFiSwitch, LV_SWITCH_PART_INDIC, &_xSwitchStle1);
    lv_obj_set_event_cb(xWiFiSwitch, WiFi_Switch_event_handler);

    if( devicesettings.wifi_enable ==  1)
        lv_switch_on(xWiFiSwitch, LV_ANIM_OFF);
    else    
        lv_switch_off(xWiFiSwitch, LV_ANIM_OFF);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line2 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p2[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line2, p2, 2);
    lv_obj_align(hor_line2, _xParaWiFiCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    static lv_style_t style_hor_line2;
    lv_style_init(&style_hor_line2);
    lv_style_set_line_color(&style_hor_line2, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
    lv_style_set_line_width(&style_hor_line2, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line2, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line2, LV_LINE_PART_MAIN, &style_hor_line1);

    // Create a container to contain FAN logo, External fan text, ON/OFF & switch
    _xParaFanCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaFanCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaFanCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaFanCont_par, hor_line2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaFanCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaFanCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Wifi logo
    _xFanIcon_par = lv_img_create(_xParaFanCont_par, NULL);
    lv_img_set_src(_xFanIcon_par, &fan_icon);
    lv_obj_align(_xFanIcon_par, _xParaFanCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xFanIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xFanIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for External Fan Text
    _xFanTextLabel_par = lv_label_create(_xParaFanCont_par, NULL);
    lv_obj_align(_xFanTextLabel_par, _xParaFanCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xFanTextLabel_par, "EXTERNAL FAN"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xFanTextLabelStyle_par;
    lv_style_init(&_xFanTextLabelStyle_par);
    lv_style_set_text_font(&_xFanTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xFanTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xFanTextLabel_par, LV_LABEL_PART_MAIN, &_xFanTextLabelStyle_par);

    // Create lavel for FAN ON/OFF Text
    _xFanOnOffTextLabel_par = lv_label_create(_xParaFanCont_par, NULL);
    lv_obj_align(_xFanOnOffTextLabel_par, _xFanTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xFanOnOffTextLabel_par, "ON/OFF"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xFanOnOffTextLabelStyle_par;
    lv_style_init(&_xFanOnOffTextLabelStyle_par);
    lv_style_set_text_font(&_xFanOnOffTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xFanOnOffTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xFanOnOffTextLabel_par, LV_LABEL_PART_MAIN, &_xFanOnOffTextLabelStyle_par);

    // Create Fan control switch
    xFanSwitch = lv_switch_create(_xParaFanCont_par, NULL);
    lv_obj_align(xFanSwitch, _xParaFanCont_par, LV_ALIGN_IN_RIGHT_MID, -30, 0);
    lv_obj_set_height(xFanSwitch, 25);
    lv_obj_add_style(xFanSwitch, LV_LABEL_PART_MAIN, &_xSwitchStle);
    lv_obj_add_style(xFanSwitch, LV_SWITCH_PART_INDIC, &_xSwitchStle1);
    lv_obj_set_event_cb(xFanSwitch, Fan_Switch_event_handler);

    if( devicesettings.external_fan_enable ==  1)
        lv_switch_on(xFanSwitch, LV_ANIM_OFF);
    else    
        lv_switch_off(xFanSwitch, LV_ANIM_OFF);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line3 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p3[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line3, p3, 2);
    lv_obj_align(hor_line3, _xParaFanCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    static lv_style_t style_hor_line3;
    lv_style_init(&style_hor_line3);
    lv_style_set_line_color(&style_hor_line3, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
    lv_style_set_line_width(&style_hor_line3, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line3, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line3, LV_LINE_PART_MAIN, &style_hor_line3);

    // Create a container to contain SLEEP logo, SLEEP text, ON/OFF & switch
    _xParaSleepCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaSleepCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaSleepCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaSleepCont_par, hor_line3, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaSleepCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaSleepCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Sleep logo
    _xSleepIcon_par = lv_img_create(_xParaSleepCont_par, NULL);
    lv_img_set_src(_xSleepIcon_par, &sleep_icon);
    lv_obj_align(_xSleepIcon_par, _xParaSleepCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xSleepIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xSleepIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for External Fan Text
    _xSleepTextLabel_par = lv_label_create(_xParaSleepCont_par, NULL);
    lv_obj_align(_xSleepTextLabel_par, _xParaSleepCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xSleepTextLabel_par, "SLEEP"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xSleepTextLabelStyle_par;
    lv_style_init(&_xSleepTextLabelStyle_par);
    lv_style_set_text_font(&_xSleepTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xSleepTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xSleepTextLabel_par, LV_LABEL_PART_MAIN, &_xSleepTextLabelStyle_par);

    // Create lavel for FAN ON/OFF Text
    _xSleepOnOffTextLabel_par = lv_label_create(_xParaSleepCont_par, NULL);
    lv_obj_align(_xSleepOnOffTextLabel_par, _xSleepTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xSleepOnOffTextLabel_par, "ON/OFF"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xSleepOnOffTextLabelStyle_par;
    lv_style_init(&_xSleepOnOffTextLabelStyle_par);
    lv_style_set_text_font(&_xSleepOnOffTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xSleepOnOffTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xSleepOnOffTextLabel_par, LV_LABEL_PART_MAIN, &_xSleepOnOffTextLabelStyle_par);

    // Create Sleep control switch
    xSleepSwitch = lv_switch_create(_xParaSleepCont_par, NULL);
    lv_obj_align(xSleepSwitch, _xParaSleepCont_par, LV_ALIGN_IN_RIGHT_MID, -30, 0);
    lv_obj_set_height(xSleepSwitch, 25);
    lv_obj_add_style(xSleepSwitch, LV_LABEL_PART_MAIN, &_xSwitchStle);
    lv_obj_add_style(xSleepSwitch, LV_SWITCH_PART_INDIC, &_xSwitchStle1);
    lv_obj_set_event_cb(xSleepSwitch, Sleep_event_handler);

    if( devicesettings.screen_sleepmode_enable ==  1)
        lv_switch_on(xSleepSwitch, LV_ANIM_OFF);
    else    
        lv_switch_off(xSleepSwitch, LV_ANIM_OFF);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line4 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p4[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line4, p4, 2);
    lv_obj_align(hor_line4, _xParaSleepCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    static lv_style_t style_hor_line4;
    lv_style_init(&style_hor_line4);
    lv_style_set_line_color(&style_hor_line4, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
    lv_style_set_line_width(&style_hor_line4, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line4, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line4, LV_LINE_PART_MAIN, &style_hor_line4);

    // Create a container to contain Language Logo, Language text, Drop down language & switch
    _xParaLangCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaLangCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaLangCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaLangCont_par, hor_line4, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaLangCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaLangCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Sleep logo
    _xLangIcon_par = lv_img_create(_xParaLangCont_par, NULL);
    lv_img_set_src(_xLangIcon_par, &language_icon);
    lv_obj_align(_xLangIcon_par, _xParaLangCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xLangIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xLangIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for Language Text
    _xLangTextLabel_par = lv_label_create(_xParaLangCont_par, NULL);
    lv_obj_align(_xLangTextLabel_par, _xParaLangCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xLangTextLabel_par, "LANGUAGE"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xLangTextLabelStyle_par;
    lv_style_init(&_xLangTextLabelStyle_par);
    lv_style_set_text_font(&_xLangTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xLangTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xLangTextLabel_par, LV_LABEL_PART_MAIN, &_xLangTextLabelStyle_par);

    // Create lavel for Speak Language
    _xSpeakLangTextLabel_par = lv_label_create(_xParaLangCont_par, NULL);
    lv_obj_align(_xSpeakLangTextLabel_par, _xLangTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xSpeakLangTextLabel_par, "SPEAK LANGUAGE"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xSpeakLangTextLabelStyle_par;
    lv_style_init(&_xSpeakLangTextLabelStyle_par);
    lv_style_set_text_font(&_xSpeakLangTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xSpeakLangTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xSpeakLangTextLabel_par, LV_LABEL_PART_MAIN, &_xSpeakLangTextLabelStyle_par);

    // Create a Language selection drop down list
    _xLangDropDown_par = lv_dropdown_create(_xParaLangCont_par, NULL);
    lv_dropdown_set_options(_xLangDropDown_par, "English\n"
                                                "French\n"
                                                "Italian");

    lv_obj_align(_xLangDropDown_par, _xParaLangCont_par, LV_ALIGN_IN_RIGHT_MID, 30, 0);
    lv_obj_set_size(_xLangDropDown_par, 80, 30);

    static lv_style_t _xLangDropDownStyle_par;
    lv_style_init(&_xLangDropDownStyle_par);
    lv_style_set_bg_color(&_xLangDropDownStyle_par, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_xLangDropDownStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xLangDropDownStyle_par, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_xLangDropDownStyle_par, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_xLangDropDown_par, LV_DROPDOWN_PART_MAIN, &_xLangDropDownStyle_par);
    lv_obj_set_event_cb(_xLangDropDown_par, lang_DD_event_handler);
    lv_dropdown_set_selected(_xLangDropDown_par, devicesettings.selected_language);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line5 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p5[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line5, p5, 2);
    lv_obj_align(hor_line5, _xParaLangCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_add_style(hor_line5, LV_LINE_PART_MAIN, &style_hor_line4);

    // Create a container to contain Sleep Timer Logo, Sleep timer text, Drop down Min value
    _xParaSlpTmrCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaSlpTmrCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaSlpTmrCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaSlpTmrCont_par, hor_line5, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaSlpTmrCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaSlpTmrCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Timer logo
    _xSlpTmrIcon_par = lv_img_create(_xParaSlpTmrCont_par, NULL);
    lv_img_set_src(_xSlpTmrIcon_par, &timer_icon);
    lv_obj_align(_xSlpTmrIcon_par, _xParaSlpTmrCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xSlpTmrIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xSlpTmrIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for Sleep Timer Text
    _xSlpTmrTextLabel_par = lv_label_create(_xParaSlpTmrCont_par, NULL);
    lv_obj_align(_xSlpTmrTextLabel_par, _xParaSlpTmrCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xSlpTmrTextLabel_par, "SLEEP TIMER"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xSlpTmrTextLabelStyle_par;
    lv_style_init(&_xSlpTmrTextLabelStyle_par);
    lv_style_set_text_font(&_xSlpTmrTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xSlpTmrTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xSlpTmrTextLabel_par, LV_LABEL_PART_MAIN, &_xSlpTmrTextLabelStyle_par);

    // Create lavel for Adjust Timer Text
    _xAdjTmrTextLabel_par = lv_label_create(_xParaSlpTmrCont_par, NULL);
    lv_obj_align(_xAdjTmrTextLabel_par, _xSlpTmrTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xAdjTmrTextLabel_par, "ADJUST TIMER"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xAdjTmrTextLabelStyle_par;
    lv_style_init(&_xAdjTmrTextLabelStyle_par);
    lv_style_set_text_font(&_xAdjTmrTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xAdjTmrTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xAdjTmrTextLabel_par, LV_LABEL_PART_MAIN, &_xAdjTmrTextLabelStyle_par);

    // Create a Language selection drop down list
    _xTimerDropDown_par = lv_dropdown_create(_xParaSlpTmrCont_par, NULL);
    lv_dropdown_set_options(_xTimerDropDown_par, "15 min\n"
                                                 "30 min\n"
                                                 "60 min");
    lv_obj_align(_xTimerDropDown_par, _xParaSlpTmrCont_par, LV_ALIGN_IN_RIGHT_MID, 30, 0);
    lv_obj_set_size(_xTimerDropDown_par, 80, 30);

    static lv_style_t _xTimerDropDownStyle_par;
    lv_style_init(&_xTimerDropDownStyle_par);
    lv_style_set_bg_color(&_xTimerDropDownStyle_par, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_xTimerDropDownStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xTimerDropDownStyle_par, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_xTimerDropDownStyle_par, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_xTimerDropDown_par, LV_DROPDOWN_PART_MAIN, &_xTimerDropDownStyle_par);
    lv_obj_set_event_cb(_xTimerDropDown_par, SlpTmr_DD_event_handler);
    
    if(devicesettings.screen_timeout_value == 15)
        lv_dropdown_set_selected(_xTimerDropDown_par, 0);
    else if(devicesettings.screen_timeout_value == 30)
        lv_dropdown_set_selected(_xTimerDropDown_par, 1);
    else if(devicesettings.screen_timeout_value == 60)
        lv_dropdown_set_selected(_xTimerDropDown_par, 2);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line6 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p6[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line6, p6, 2);
    lv_obj_align(hor_line6, _xParaSlpTmrCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_add_style(hor_line6, LV_LINE_PART_MAIN, &style_hor_line4);

    // Create a container to contain LUMINOUS Logo, Lumin text, Adjust slider
    _xParaLuminCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaLuminCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaLuminCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaLuminCont_par, hor_line6, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaLuminCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaLuminCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create LUMINOSITY logo
    _xLuminIcon_par = lv_img_create(_xParaLuminCont_par, NULL);
    lv_img_set_src(_xLuminIcon_par, &luminosity_icon);
    lv_obj_align(_xLuminIcon_par, _xParaLuminCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xLuminIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xLuminIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for LUMINOSITY Text
    _xLuminTextLabel_par = lv_label_create(_xParaLuminCont_par, NULL);
    lv_obj_align(_xLuminTextLabel_par, _xParaLuminCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xLuminTextLabel_par, "LUMINOSITY"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xLuminTextLabelStyle_par;
    lv_style_init(&_xLuminTextLabelStyle_par);
    lv_style_set_text_font(&_xLuminTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xLuminTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xLuminTextLabel_par, LV_LABEL_PART_MAIN, &_xLuminTextLabelStyle_par);

    // Create lavel for Adjust LUMINOSITY Text
    _xAdjLuminTextLabel_par = lv_label_create(_xParaLuminCont_par, NULL);
    lv_obj_align(_xAdjLuminTextLabel_par, _xLuminTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xAdjLuminTextLabel_par, "ADJUST SCREEN \nLUMINOSITY"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xAdjLuminTextLabelStyle_par;
    lv_style_init(&_xAdjLuminTextLabelStyle_par);
    lv_style_set_text_font(&_xAdjLuminTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xAdjLuminTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xAdjLuminTextLabel_par, LV_LABEL_PART_MAIN, &_xAdjLuminTextLabelStyle_par);

    // Create a slider to adjust lumin
    _xLuminslider_par = lv_slider_create(_xParaLuminCont_par, NULL);
    lv_obj_align(_xLuminslider_par, _xParaLuminCont_par, LV_ALIGN_IN_RIGHT_MID, 170, 0);
    lv_obj_set_width(_xLuminslider_par, 80);
    lv_obj_set_event_cb(_xLuminslider_par, Lumin_Slider_event_handler);

    static lv_style_t _xSliderStle;
    lv_style_init(&_xSliderStle);
    lv_style_set_bg_color(&_xSliderStle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_add_style(_xLuminslider_par, LV_LABEL_PART_MAIN, &_xSliderStle);
    lv_slider_set_value(_xLuminslider_par, devicesettings.luminosity_value, LV_ANIM_OFF);

    // Seprator line
    // Create Horizontal Line
    lv_obj_t *hor_line7 = lv_line_create(_xParaLabelContainer_par, NULL);
    static lv_point_t p7[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line7, p7, 2);
    lv_obj_align(hor_line7, _xParaLuminCont_par, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_add_style(hor_line7, LV_LINE_PART_MAIN, &style_hor_line4);

    // Create a container to contain CONTRAST Logo, Contrast text, Adjust slider
    _xParaContrastCont_par = lv_cont_create(_xBaseContainer_par, NULL);
    lv_obj_set_size(_xParaContrastCont_par, 300, _ParaContWidth);
    lv_obj_set_width(_xParaContrastCont_par, lv_page_get_width_fit(_xParaLabelContainer_par));
    lv_obj_align(_xParaContrastCont_par, hor_line7, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_bg_color(_xParaContrastCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f , _xColorBGParas, LV_COLOR_WHITE
    lv_obj_set_style_local_border_width(_xParaContrastCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create CONTRAST logo
    _xContrastIcon_par = lv_img_create(_xParaContrastCont_par, NULL);
    lv_img_set_src(_xContrastIcon_par, &contrast_icon);
    lv_obj_align(_xContrastIcon_par, _xParaContrastCont_par, LV_ALIGN_IN_LEFT_MID, 7, 0);
    lv_obj_set_style_local_image_recolor_opa(_xContrastIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xContrastIcon_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create lavel for CONTRAST Text
    _xContrastTextLabel_par = lv_label_create(_xParaContrastCont_par, NULL);
    lv_obj_align(_xContrastTextLabel_par, _xParaContrastCont_par, LV_ALIGN_IN_TOP_MID, -70, txt_y_align);
    lv_label_set_text(_xContrastTextLabel_par, "CONTRAST"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xContrastTextLabelStyle_par;
    lv_style_init(&_xContrastTextLabelStyle_par);
    lv_style_set_text_font(&_xContrastTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xContrastTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xContrastTextLabel_par, LV_LABEL_PART_MAIN, &_xContrastTextLabelStyle_par);

    // Create lavel for CONTRAST Text
    _xAdjContrastTextLabel_par = lv_label_create(_xParaContrastCont_par, NULL);
    lv_obj_align(_xAdjContrastTextLabel_par, _xContrastTextLabel_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xAdjContrastTextLabel_par, "ADJUST SCREEN \nCONTRAST"); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xAdjContrastTextLabelStyle_par;
    lv_style_init(&_xAdjContrastTextLabelStyle_par);
    lv_style_set_text_font(&_xAdjContrastTextLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_xAdjContrastTextLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xAdjContrastTextLabel_par, LV_LABEL_PART_MAIN, &_xAdjContrastTextLabelStyle_par);

    // Create a slider to adjust lumin
    _xContrastslider_par = lv_slider_create(_xParaContrastCont_par, NULL);
    lv_obj_align(_xContrastslider_par, _xParaContrastCont_par, LV_ALIGN_IN_RIGHT_MID, 170, 0);
    lv_obj_set_width(_xContrastslider_par, 80);
    lv_obj_set_event_cb(_xContrastslider_par, Contrast_Slider_event_handler);
    lv_obj_add_style(_xContrastslider_par, LV_LABEL_PART_MAIN, &_xSliderStle);
    lv_slider_set_value(_xContrastslider_par, devicesettings.contrast_value, LV_ANIM_OFF);

    crnt_screen = scrParameter; // scrParameter
    screenid = SCR_PARAMETER;
}
