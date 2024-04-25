/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
 *  @file screen_service.c
 *  @brief This Screen is for Service setting
 *  @details
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"

/*********************
 *      DEFINES
 *********************/

#define SYMBOL_SIGNAL "\uf012"

/*********************
 *   DECLARATION
 *********************/
LV_FONT_DECLARE(signal_20)
LV_IMG_DECLARE(service_icon)
LV_IMG_DECLARE(left_arrow_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void _xsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t *sstParentCont;
lv_obj_t *_sstContStausBar;
lv_obj_t *__sstTimeLabel;
lv_obj_t *_sstBatteryLbl;
lv_obj_t *_sstBatteryPercentage;
lv_obj_t *_sstWiFiLbl;
lv_obj_t *_sstSignalLbl;
lv_obj_t *_sstSliderPage;
lv_obj_t *_sstHeadingCont;
lv_obj_t *_sstBackArrowCont;
lv_obj_t *_sstBackArrowLabel;
lv_obj_t *_sstServiceHeadingLbl;
lv_obj_t *_sstServiceLogo;
lv_obj_t *_sstHourServiceCont;
lv_obj_t *_sstHourServiceTxt;
lv_obj_t *_sstAlertTxt;
lv_obj_t *_sstOnOffTxt;
lv_obj_t *_sstAlertSwitch;
lv_obj_t *_sstINTxt;
lv_obj_t *_sstINValue;
lv_obj_t *_sstCustomBTN;
lv_obj_t *_sstCustomBTNLabel;
lv_obj_t *_sstHoursTxt;
lv_obj_t *_sstReminderTxt;
lv_obj_t *_sstBeforeAlertTxt;
lv_obj_t *_sstHourDropDown;
lv_obj_t *_sstActionTxt;
lv_obj_t *_sstMsgTxt;
lv_obj_t *_sstActionSwitch;
lv_obj_t *_sstDateServiceCont;
lv_obj_t *_sstDateServiceTxt;
lv_obj_t *_sstAlertTxt1;
lv_obj_t *_sstOnOffTxt1;
lv_obj_t *_sstAlertSwitch1;
lv_obj_t *_sstINTxt1;
lv_obj_t *_sstINValue1;
lv_obj_t *_sstCustomBTN1;
lv_obj_t *_sstCustomBTNLabel1;
lv_obj_t *_sstReminderTxt1;
lv_obj_t *_sstBeforeAlertTxt1;
lv_obj_t *_sstHourDropDown1;
lv_obj_t *_sstActionTxt1;
lv_obj_t *_sstMsgTxt1;
lv_obj_t *_sstActionSwitch1;
lv_obj_t *_sstVolumeServiceCont;
lv_obj_t *_sstVolumeServiceTxt;
lv_obj_t *_sstAlertTxt12;
lv_obj_t *_sstOnOffTxt12;
lv_obj_t *_sstAlertSwitch12;
lv_obj_t *_sstINTxt12;
lv_obj_t *_sstINValue12;
lv_obj_t *_sstCustomBTN12;
lv_obj_t *_sstCustomBTNLabel12;
lv_obj_t *_sstHoursTxt12;
lv_obj_t *_sstReminderTxt12;
lv_obj_t *_sstBeforeAlertTxt12;
lv_obj_t *_sstHourDropDown12;
lv_obj_t *_sstActionTxt12;
lv_obj_t *_sstMsgTxt12;
lv_obj_t *_sstActionSwitch12;
lv_obj_t *_sstFilterServiceCont;
lv_obj_t *_sstFilterServiceTxt;
lv_obj_t *_sstAlertTxt123;
lv_obj_t *_sstOnOffTxt123;
lv_obj_t *_sstAlertSwitch123;
lv_obj_t *_sstINTxt123;
lv_obj_t *_sstINValue123;
lv_obj_t *_sstCustomBTN123;
lv_obj_t *_sstCustomBTNLabel123;
lv_obj_t *_sstReminderTxt123;
lv_obj_t *_sstBeforeAlertTxt123;
lv_obj_t *_sstHourDropDown123;
lv_obj_t *_sstActionTxt123;
lv_obj_t *_sstMsgTxt123;
lv_obj_t *_sstActionSwitch123;

lv_task_t *service_refresherTask;
/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static void service_refer_func(lv_task_t *refresherTask)
{
    if (lv_obj_get_screen(__sstTimeLabel) == lv_scr_act())
    {
        lv_label_set_text(__sstTimeLabel, guiTime);
        lv_label_set_text(_sstBatteryLbl, get_battery_symbol());
        lv_label_set_text_fmt(_sstBatteryPercentage, "%d%%", get_battery_percentage());
    }
}


void callServiceSetScreen(void)
{
    sstParentCont = lv_obj_create(NULL, NULL);
    lv_scr_load(sstParentCont);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    lv_obj_set_size(sstParentCont, 320, 480);
    lv_obj_set_click(sstParentCont, false);
    lv_obj_align(sstParentCont, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(sstParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_border_opa(sstParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(sstParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _sstContStausBar = lv_cont_create(sstParentCont, NULL);
    lv_obj_set_size(_sstContStausBar, 320, 35);
    lv_obj_align(_sstContStausBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_sstContStausBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_border_opa(_sstContStausBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    __sstTimeLabel = lv_label_create(_sstContStausBar, NULL);
    lv_obj_align(__sstTimeLabel, _sstContStausBar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__sstTimeLabel, guiTime);

    static lv_style_t _sstTimeLabelStyle;
    lv_style_init(&_sstTimeLabelStyle);
    lv_style_set_text_font(&_sstTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_sstTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__sstTimeLabel, LV_LABEL_PART_MAIN, &_sstTimeLabelStyle);

    // Create Label for Battery icon
    _sstBatteryLbl = lv_label_create(_sstContStausBar, NULL);
    lv_obj_align(_sstBatteryLbl, _sstContStausBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(_sstBatteryLbl, get_battery_symbol());

    static lv_style_t _sstBatteryLblStyle;
    lv_style_init(&_sstBatteryLblStyle);
    lv_style_set_text_font(&_sstBatteryLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_sstBatteryLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstBatteryLbl, LV_LABEL_PART_MAIN, &_sstBatteryLblStyle);

    _sstBatteryPercentage = lv_label_create(_sstContStausBar, NULL);
    lv_obj_align(_sstBatteryPercentage, _sstContStausBar, LV_ALIGN_IN_TOP_RIGHT, -60, 7);
    lv_label_set_text_fmt(_sstBatteryPercentage, "%d%%", get_battery_percentage());

    static lv_style_t _xBatteryPercentageStyle;
    lv_style_init(&_xBatteryPercentageStyle);
    lv_style_set_text_font(&_xBatteryPercentageStyle, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_style_set_text_color(&_xBatteryPercentageStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstBatteryPercentage, LV_LABEL_PART_MAIN, &_xBatteryPercentageStyle);

    // Create Label for Wifi icon
    _sstWiFiLbl = lv_label_create(_sstContStausBar, NULL);
    lv_obj_align(_sstWiFiLbl, _sstBatteryLbl, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(_sstWiFiLbl, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(_sstWiFiLbl, true);

    static lv_style_t _xWifiLabelStyle_par;
    lv_style_init(&_xWifiLabelStyle_par);
    lv_style_set_text_font(&_xWifiLabelStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xWifiLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstWiFiLbl, LV_LABEL_PART_MAIN, &_xWifiLabelStyle_par);

    // Create Label for Signal icon
    _sstSignalLbl = lv_label_create(_sstContStausBar, NULL);
    lv_obj_align(_sstSignalLbl, _sstWiFiLbl, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(_sstSignalLbl, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(_sstSignalLbl, true);

    static lv_style_t _xSignalLabelStyle_par;
    lv_style_init(&_xSignalLabelStyle_par);
    lv_style_set_text_font(&_xSignalLabelStyle_par, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xSignalLabelStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstSignalLbl, LV_LABEL_PART_MAIN, &_xSignalLabelStyle_par);

    service_refresherTask = lv_task_create(service_refer_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create a container to put all the parameters
    _sstSliderPage = lv_page_create(sstParentCont, NULL);
    lv_obj_set_size(_sstSliderPage, 320, 430);
    lv_obj_align(_sstSliderPage, _sstContStausBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);
    lv_page_set_scrollbar_mode(_sstSliderPage, LV_SCROLLBAR_MODE_HIDE);
    lv_obj_set_style_local_bg_color(_sstSliderPage, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_width(_sstSliderPage, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    _sstHeadingCont = lv_cont_create(_sstSliderPage, NULL);
    lv_obj_set_size(_sstHeadingCont, 300, 70);
    lv_obj_set_width(_sstHeadingCont, lv_page_get_width_fit(_sstSliderPage));
    lv_obj_align(_sstHeadingCont, _sstSliderPage, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_bg_color(_sstHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_sstHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _sstBackArrowCont = lv_cont_create(_sstHeadingCont, NULL);
    lv_obj_set_size(_sstBackArrowCont, 60, 60);
    lv_obj_align(_sstBackArrowCont, _sstHeadingCont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_obj_set_style_local_bg_color(_sstBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_sstBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_sstBackArrowCont, _xsBackArrow_event_handler);

    _sstBackArrowLabel = lv_img_create(_sstBackArrowCont, NULL);
    lv_img_set_src(_sstBackArrowLabel, &left_arrow_icon);
    lv_obj_align(_sstBackArrowLabel, _sstBackArrowCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(_sstBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(_sstBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_sstBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_sstBackArrowLabel, _xsBackArrow_event_handler);

    // Create Label for Parameter "Heading"
    _sstServiceHeadingLbl = lv_label_create(_sstHeadingCont, NULL);
    lv_obj_align(_sstServiceHeadingLbl, _sstHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -25, -35);
    lv_label_set_text(_sstServiceHeadingLbl, "Service");

    static lv_style_t _xParameterHeadingStyle_par;
    lv_style_init(&_xParameterHeadingStyle_par);
    lv_style_set_text_font(&_xParameterHeadingStyle_par, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&_xParameterHeadingStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstServiceHeadingLbl, LV_LABEL_PART_MAIN, &_xParameterHeadingStyle_par);

    _sstServiceLogo = lv_img_create(_sstHeadingCont, NULL);
    lv_img_set_src(_sstServiceLogo, &service_icon);
    lv_obj_align(_sstServiceLogo, _sstServiceHeadingLbl, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_obj_set_click(_sstServiceLogo, false);
    lv_obj_set_style_local_image_recolor_opa(_sstServiceLogo, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_sstServiceLogo, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Hour Service alert Pallet
    _sstHourServiceCont = lv_cont_create(_sstSliderPage, NULL);
    lv_obj_set_size(_sstHourServiceCont, 300, 220);
    lv_obj_set_click(_sstHourServiceCont, false);
    lv_obj_set_width(_sstHourServiceCont, lv_page_get_width_fit(_sstSliderPage));
    lv_obj_align(_sstHourServiceCont, _sstHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_obj_set_style_local_bg_color(_sstHourServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_sstHourServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _sstHourServiceTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstHourServiceTxt, _sstHourServiceCont, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text(_sstHourServiceTxt, "HOUR SERVICE");

    static lv_style_t _sstBlueTxtStyle;
    lv_style_init(&_sstBlueTxtStyle);
    lv_style_set_text_font(&_sstBlueTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_sstBlueTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_add_style(_sstHourServiceTxt, LV_LABEL_PART_MAIN, &_sstBlueTxtStyle);

    _sstAlertTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstAlertTxt, _sstHourServiceTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_sstAlertTxt, "Alert");

    static lv_style_t _sstWhiteBigTxtStyle;
    lv_style_init(&_sstWhiteBigTxtStyle);
    lv_style_set_text_font(&_sstWhiteBigTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_sstWhiteBigTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstAlertTxt, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstOnOffTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstOnOffTxt, _sstAlertTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstOnOffTxt, "ON / OFF");

    static lv_style_t _sstWhiteSmallTxtStyle;
    lv_style_init(&_sstWhiteSmallTxtStyle);
    lv_style_set_text_font(&_sstWhiteSmallTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_10); // signal_20
    lv_style_set_text_color(&_sstWhiteSmallTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstOnOffTxt, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstAlertSwitch = lv_switch_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstAlertSwitch, _sstOnOffTxt, LV_ALIGN_OUT_RIGHT_TOP, 180, -20);
    lv_obj_set_height(_sstAlertSwitch, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);

    static lv_style_t _sstAlertSwitchStyle;
    lv_style_init(&_sstAlertSwitchStyle);
    lv_style_set_bg_color(&_sstAlertSwitchStyle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_width(&_sstAlertSwitchStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&_sstAlertSwitchStyle, LV_SWITCH_PART_BG, LV_COLOR_WHITE);
    lv_obj_add_style(_sstAlertSwitch, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    _sstINTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstINTxt, _sstOnOffTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstINTxt, "IN :");
    lv_obj_add_style(_sstINTxt, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstINValue = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstINValue, _sstINTxt, LV_ALIGN_OUT_RIGHT_TOP, 30, 0);
    lv_label_set_text(_sstINValue, "3500");
    lv_obj_add_style(_sstINValue, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    // Create custom Button
    _sstCustomBTN = lv_btn_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstCustomBTN, _sstINValue, LV_ALIGN_OUT_RIGHT_TOP, 90, -5);
    lv_obj_set_size(_sstCustomBTN, 100, 30);
    // lv_obj_set_event_cb(_xsValidBtn, BTN_event_handler);

    static lv_style_t _sstCustomBTNStyle;
    lv_style_init(&_sstCustomBTNStyle);
    lv_style_set_radius(&_sstCustomBTNStyle, LV_STATE_DEFAULT, 5);
    lv_style_set_bg_color(&_sstCustomBTNStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // #359Fe2
    lv_style_set_border_width(&_sstCustomBTNStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_sstCustomBTNStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_sstCustomBTN, LV_BTN_PART_MAIN, &_sstCustomBTNStyle);

    // Creat a CUSTOM Button Label Style
    _sstCustomBTNLabel = lv_label_create(_sstCustomBTN, NULL);
    lv_obj_align(_sstCustomBTNLabel, _sstCustomBTN, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_sstCustomBTNLabel, "CUSTOM");

    static lv_style_t _sstCustomBTNLabelStyle;
    lv_style_init(&_sstCustomBTNLabelStyle);
    lv_style_set_text_font(&_sstCustomBTNLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_sstCustomBTNLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_sstCustomBTNLabel, LV_LABEL_PART_MAIN, &_sstCustomBTNLabelStyle);

    _sstHoursTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstHoursTxt, _sstINValue, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_label_set_text(_sstHoursTxt, "HOURS");
    lv_obj_add_style(_sstHoursTxt, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstReminderTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstReminderTxt, _sstINTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 18);
    lv_label_set_text(_sstReminderTxt, "Reminder");
    lv_obj_add_style(_sstReminderTxt, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstBeforeAlertTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstBeforeAlertTxt, _sstReminderTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstBeforeAlertTxt, "BEFORE ALERT:");
    lv_obj_add_style(_sstBeforeAlertTxt, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create a Hours selection drop down list
    _sstHourDropDown = lv_dropdown_create(_sstHourServiceCont, NULL);
    lv_dropdown_set_options(_sstHourDropDown, "100 HOUR\n"
                                              "200 HOUR\n"
                                              "300 HOUR");

    lv_obj_align(_sstHourDropDown, _sstBeforeAlertTxt, LV_ALIGN_OUT_RIGHT_TOP, 100, -20);
    lv_obj_set_size(_sstHourDropDown, 95, 30);

    static lv_style_t _sstHourDropDownStyle;
    lv_style_init(&_sstHourDropDownStyle);
    lv_style_set_bg_color(&_sstHourDropDownStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_sstHourDropDownStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_sstHourDropDownStyle, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_sstHourDropDownStyle, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_sstHourDropDown, LV_DROPDOWN_PART_MAIN, &_sstHourDropDownStyle);
    // lv_obj_set_event_cb(_xLangDropDown_par, lang_DD_event_handler);

    _sstActionTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstActionTxt, _sstBeforeAlertTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_label_set_text(_sstActionTxt, "Action");
    lv_obj_add_style(_sstActionTxt, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstMsgTxt = lv_label_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstMsgTxt, _sstActionTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstMsgTxt, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_sstMsgTxt, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstActionSwitch = lv_switch_create(_sstHourServiceCont, NULL);
    lv_obj_align(_sstActionSwitch, _sstActionTxt, LV_ALIGN_OUT_RIGHT_TOP, 180, 0);
    lv_obj_set_height(_sstActionSwitch, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstActionSwitch, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    // Date Service alert Pallet
    _sstDateServiceCont = lv_cont_create(_sstSliderPage, NULL);
    lv_obj_set_size(_sstDateServiceCont, 300, 220);
    lv_obj_set_click(_sstDateServiceCont, false);
    lv_obj_set_width(_sstDateServiceCont, lv_page_get_width_fit(_sstSliderPage));
    lv_obj_align(_sstDateServiceCont, _sstHourServiceCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_local_bg_color(_sstDateServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_sstDateServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _sstDateServiceTxt = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstDateServiceTxt, _sstDateServiceCont, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text(_sstDateServiceTxt, "DATE SERVICE");
    lv_obj_add_style(_sstDateServiceTxt, LV_LABEL_PART_MAIN, &_sstBlueTxtStyle);

    _sstAlertTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstAlertTxt1, _sstDateServiceTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_sstAlertTxt1, "Alert");
    lv_obj_add_style(_sstAlertTxt1, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstOnOffTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstOnOffTxt1, _sstAlertTxt1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstOnOffTxt1, "ON / OFF");
    lv_obj_add_style(_sstOnOffTxt1, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstAlertSwitch1 = lv_switch_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstAlertSwitch1, _sstOnOffTxt1, LV_ALIGN_OUT_RIGHT_TOP, 180, -20);
    lv_obj_set_height(_sstAlertSwitch1, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstAlertSwitch1, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    _sstINTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstINTxt1, _sstOnOffTxt1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstINTxt1, "IN :");
    lv_obj_add_style(_sstINTxt1, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstINValue1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstINValue1, _sstINTxt1, LV_ALIGN_OUT_RIGHT_TOP, 30, 0);
    lv_label_set_text(_sstINValue1, "24, DEC 2018");
    lv_obj_add_style(_sstINValue1, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    // Create custom Button
    _sstCustomBTN1 = lv_btn_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstCustomBTN1, _sstINValue1, LV_ALIGN_OUT_RIGHT_TOP, 35, -5);
    lv_obj_set_size(_sstCustomBTN1, 100, 30);
    // lv_obj_set_event_cb(_xsValidBtn, BTN_event_handler);
    lv_obj_add_style(_sstCustomBTN1, LV_BTN_PART_MAIN, &_sstCustomBTNStyle);

    // Creat a CUSTOM Button Label Style
    _sstCustomBTNLabel1 = lv_label_create(_sstCustomBTN1, NULL);
    lv_obj_align(_sstCustomBTNLabel1, _sstCustomBTN1, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_sstCustomBTNLabel1, "CUSTOM");
    lv_obj_add_style(_sstCustomBTNLabel1, LV_LABEL_PART_MAIN, &_sstCustomBTNLabelStyle);

    // lv_obj_t * _sstHoursTxt1;
    // _sstHoursTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    // lv_obj_align(_sstHoursTxt1, _sstINValue1, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    // lv_label_set_text(_sstHoursTxt1, "HOURS");
    // lv_obj_add_style(_sstHoursTxt1, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstReminderTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstReminderTxt1, _sstINTxt1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 18);
    lv_label_set_text(_sstReminderTxt1, "Reminder");
    lv_obj_add_style(_sstReminderTxt1, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstBeforeAlertTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstBeforeAlertTxt1, _sstReminderTxt1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstBeforeAlertTxt1, "BEFORE ALERT:");
    lv_obj_add_style(_sstBeforeAlertTxt1, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create a Hours selection drop down list
    _sstHourDropDown1 = lv_dropdown_create(_sstDateServiceCont, NULL);
    lv_dropdown_set_options(_sstHourDropDown1, "100 HOUR\n"
                                               "200 HOUR\n"
                                               "300 HOUR");

    lv_obj_align(_sstHourDropDown1, _sstBeforeAlertTxt1, LV_ALIGN_OUT_RIGHT_TOP, 100, -20);
    lv_obj_set_size(_sstHourDropDown1, 95, 30);
    lv_obj_add_style(_sstHourDropDown1, LV_DROPDOWN_PART_MAIN, &_sstHourDropDownStyle);
    // lv_obj_set_event_cb(_xLangDropDown_par, lang_DD_event_handler);

    _sstActionTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstActionTxt1, _sstBeforeAlertTxt1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_label_set_text(_sstActionTxt1, "Action");
    lv_obj_add_style(_sstActionTxt1, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstMsgTxt1 = lv_label_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstMsgTxt1, _sstActionTxt1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstMsgTxt1, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_sstMsgTxt1, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstActionSwitch1 = lv_switch_create(_sstDateServiceCont, NULL);
    lv_obj_align(_sstActionSwitch1, _sstActionTxt1, LV_ALIGN_OUT_RIGHT_TOP, 180, 0);
    lv_obj_set_height(_sstActionSwitch1, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstActionSwitch1, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    // Volume Service alert Pallet
    _sstVolumeServiceCont = lv_cont_create(_sstSliderPage, NULL);
    lv_obj_set_size(_sstVolumeServiceCont, 300, 220);
    lv_obj_set_click(_sstVolumeServiceCont, false);
    lv_obj_set_width(_sstVolumeServiceCont, lv_page_get_width_fit(_sstSliderPage));
    lv_obj_align(_sstVolumeServiceCont, _sstDateServiceCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_local_bg_color(_sstVolumeServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_sstVolumeServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _sstVolumeServiceTxt = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstVolumeServiceTxt, _sstVolumeServiceCont, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text(_sstVolumeServiceTxt, "VOLUME SERVICE");
    lv_obj_add_style(_sstVolumeServiceTxt, LV_LABEL_PART_MAIN, &_sstBlueTxtStyle);

    _sstAlertTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstAlertTxt12, _sstVolumeServiceTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_sstAlertTxt12, "Alert");
    lv_obj_add_style(_sstAlertTxt12, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstOnOffTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstOnOffTxt12, _sstAlertTxt12, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstOnOffTxt12, "ON / OFF");
    lv_obj_add_style(_sstOnOffTxt12, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstAlertSwitch12 = lv_switch_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstAlertSwitch12, _sstOnOffTxt12, LV_ALIGN_OUT_RIGHT_TOP, 180, -20);
    lv_obj_set_height(_sstAlertSwitch12, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstAlertSwitch12, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    _sstINTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstINTxt12, _sstOnOffTxt12, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstINTxt12, "IN :");
    lv_obj_add_style(_sstINTxt12, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstINValue12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstINValue12, _sstINTxt12, LV_ALIGN_OUT_RIGHT_TOP, 30, 0);
    lv_label_set_text(_sstINValue12, "75000");
    lv_obj_add_style(_sstINValue12, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    // Create custom Button
    _sstCustomBTN12 = lv_btn_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstCustomBTN12, _sstINValue12, LV_ALIGN_OUT_RIGHT_TOP, 80, -5);
    lv_obj_set_size(_sstCustomBTN12, 100, 30);
    // lv_obj_set_event_cb(_xsValidBtn, BTN_event_handler);
    lv_obj_add_style(_sstCustomBTN12, LV_BTN_PART_MAIN, &_sstCustomBTNStyle);

    // Creat a CUSTOM Button Label Style
    _sstCustomBTNLabel12 = lv_label_create(_sstCustomBTN12, NULL);
    lv_obj_align(_sstCustomBTNLabel12, _sstCustomBTN12, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_sstCustomBTNLabel12, "CUSTOM");
    lv_obj_add_style(_sstCustomBTNLabel12, LV_LABEL_PART_MAIN, &_sstCustomBTNLabelStyle);

    _sstHoursTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstHoursTxt12, _sstINValue12, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_label_set_text(_sstHoursTxt12, "Liters");
    lv_obj_add_style(_sstHoursTxt12, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstReminderTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstReminderTxt12, _sstINTxt12, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 18);
    lv_label_set_text(_sstReminderTxt12, "Reminder");
    lv_obj_add_style(_sstReminderTxt12, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstBeforeAlertTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstBeforeAlertTxt12, _sstReminderTxt12, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstBeforeAlertTxt12, "BEFORE ALERT:");
    lv_obj_add_style(_sstBeforeAlertTxt12, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create a Hours selection drop down list
    _sstHourDropDown12 = lv_dropdown_create(_sstVolumeServiceCont, NULL);
    lv_dropdown_set_options(_sstHourDropDown12, "100 HOUR\n"
                                                "200 HOUR\n"
                                                "300 HOUR");

    lv_obj_align(_sstHourDropDown12, _sstBeforeAlertTxt12, LV_ALIGN_OUT_RIGHT_TOP, 100, -20);
    lv_obj_set_size(_sstHourDropDown12, 95, 30);
    lv_obj_add_style(_sstHourDropDown12, LV_DROPDOWN_PART_MAIN, &_sstHourDropDownStyle);
    // lv_obj_set_event_cb(_xLangDropDown_par, lang_DD_event_handler);

    _sstActionTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstActionTxt12, _sstBeforeAlertTxt12, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_label_set_text(_sstActionTxt12, "Action");
    lv_obj_add_style(_sstActionTxt12, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstMsgTxt12 = lv_label_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstMsgTxt12, _sstActionTxt12, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstMsgTxt12, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_sstMsgTxt12, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstActionSwitch12 = lv_switch_create(_sstVolumeServiceCont, NULL);
    lv_obj_align(_sstActionSwitch12, _sstActionTxt12, LV_ALIGN_OUT_RIGHT_TOP, 180, 0);
    lv_obj_set_height(_sstActionSwitch12, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstActionSwitch12, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    // Filter Service alert Pallet
    _sstFilterServiceCont = lv_cont_create(_sstSliderPage, NULL);
    lv_obj_set_size(_sstFilterServiceCont, 300, 220);
    lv_obj_set_click(_sstFilterServiceCont, false);
    lv_obj_set_width(_sstFilterServiceCont, lv_page_get_width_fit(_sstSliderPage));
    lv_obj_align(_sstFilterServiceCont, _sstVolumeServiceCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_local_bg_color(_sstFilterServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_sstFilterServiceCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _sstFilterServiceTxt = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstFilterServiceTxt, _sstFilterServiceCont, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text(_sstFilterServiceTxt, "FILTER SERVICE");
    lv_obj_add_style(_sstFilterServiceTxt, LV_LABEL_PART_MAIN, &_sstBlueTxtStyle);

    _sstAlertTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstAlertTxt123, _sstFilterServiceTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_sstAlertTxt123, "Alert");
    lv_obj_add_style(_sstAlertTxt123, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstOnOffTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstOnOffTxt123, _sstAlertTxt123, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstOnOffTxt123, "ON / OFF");
    lv_obj_add_style(_sstOnOffTxt123, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstAlertSwitch123 = lv_switch_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstAlertSwitch123, _sstOnOffTxt123, LV_ALIGN_OUT_RIGHT_TOP, 180, -20);
    lv_obj_set_height(_sstAlertSwitch123, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstAlertSwitch123, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    _sstINTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstINTxt123, _sstOnOffTxt123, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstINTxt123, "IN :");
    lv_obj_add_style(_sstINTxt123, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstINValue123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstINValue123, _sstINTxt123, LV_ALIGN_OUT_RIGHT_TOP, 30, 0);
    lv_label_set_text(_sstINValue123, "90 %");
    lv_obj_add_style(_sstINValue123, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    // Create custom Button
    _sstCustomBTN123 = lv_btn_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstCustomBTN123, _sstINValue123, LV_ALIGN_OUT_RIGHT_TOP, 90, -5);
    lv_obj_set_size(_sstCustomBTN123, 100, 30);
    // lv_obj_set_event_cb(_xsValidBtn, BTN_event_handler);
    lv_obj_add_style(_sstCustomBTN123, LV_BTN_PART_MAIN, &_sstCustomBTNStyle);

    // Creat a CUSTOM Button Label Style
    _sstCustomBTNLabel123 = lv_label_create(_sstCustomBTN123, NULL);
    lv_obj_align(_sstCustomBTNLabel123, _sstCustomBTN123, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_sstCustomBTNLabel123, "CUSTOM");
    lv_obj_add_style(_sstCustomBTNLabel123, LV_LABEL_PART_MAIN, &_sstCustomBTNLabelStyle);

    _sstReminderTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstReminderTxt123, _sstINTxt123, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 18);
    lv_label_set_text(_sstReminderTxt123, "Reminder");
    lv_obj_add_style(_sstReminderTxt123, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstBeforeAlertTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstBeforeAlertTxt123, _sstReminderTxt123, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_sstBeforeAlertTxt123, "BEFORE ALERT:");
    lv_obj_add_style(_sstBeforeAlertTxt123, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create a Hours selection drop down list

    _sstHourDropDown123 = lv_dropdown_create(_sstFilterServiceCont, NULL);
    lv_dropdown_set_options(_sstHourDropDown123, "100 HOUR\n"
                                                 "200 HOUR\n"
                                                 "300 HOUR");

    lv_obj_align(_sstHourDropDown123, _sstBeforeAlertTxt123, LV_ALIGN_OUT_RIGHT_TOP, 100, -20);
    lv_obj_set_size(_sstHourDropDown123, 95, 30);
    lv_obj_add_style(_sstHourDropDown123, LV_DROPDOWN_PART_MAIN, &_sstHourDropDownStyle);
    // lv_obj_set_event_cb(_xLangDropDown_par, lang_DD_event_handler);

    _sstActionTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstActionTxt123, _sstBeforeAlertTxt123, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_label_set_text(_sstActionTxt123, "Action");
    lv_obj_add_style(_sstActionTxt123, LV_LABEL_PART_MAIN, &_sstWhiteBigTxtStyle);

    _sstMsgTxt123 = lv_label_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstMsgTxt123, _sstActionTxt123, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_sstMsgTxt123, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_sstMsgTxt123, LV_LABEL_PART_MAIN, &_sstWhiteSmallTxtStyle);

    // Create Alert control switch
    _sstActionSwitch123 = lv_switch_create(_sstFilterServiceCont, NULL);
    lv_obj_align(_sstActionSwitch123, _sstActionTxt123, LV_ALIGN_OUT_RIGHT_TOP, 180, 0);
    lv_obj_set_height(_sstActionSwitch123, 25);
    // lv_obj_set_event_cb(_sstAlertSwitch, Buzzer_switch_event_handler);
    lv_obj_add_style(_sstActionSwitch123, LV_SWITCH_PART_BG, &_sstAlertSwitchStyle);

    crnt_screen = sstParentCont; // sstParentCont
    screenid = SCR_SERVICE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void _xsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        // printf("Back to Dashbord from presetscrn\n");
        lv_task_del(service_refresherTask);
        service_refresherTask = NULL;
        pxDashboardScreen();
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
