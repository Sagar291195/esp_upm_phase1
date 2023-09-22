/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-12-01
 */

/**
 *  @file
 *  @brief
 *  @details
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_archive.h"

/*********************
 *      DEFINES
 *********************/

#define SYMBOL_SIGNAL "\uf012"

// Declare Fonts
LV_FONT_DECLARE(signal_20)

// Declare Images Here
LV_IMG_DECLARE(left_arrow_icon)
LV_IMG_DECLARE(cross_icon)
LV_IMG_DECLARE(archiv_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
char guiTime[25];

lv_obj_t *scrSummaryEndArch;
lv_obj_t *xseParentContainer_seArch;
lv_obj_t *_xseContStatusBar_seArch;
lv_obj_t *__xseTimeLabel_seArch;
lv_obj_t *__xseBatteryLabel_seArch;
lv_obj_t *__xseWifiLabel_seArch;
lv_obj_t *__xseSignalLabel_seArch;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void xCallArchvScreen(void)
{
    scrSummaryEndArch = lv_obj_create(NULL, NULL);
    lv_scr_load(scrSummaryEndArch);

    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    xseParentContainer_seArch = lv_cont_create(scrSummaryEndArch, NULL);
    // lv_scr_load(xseParentContainer_se);
    lv_obj_set_size(xseParentContainer_seArch, 320, 480);
    lv_obj_set_click(xseParentContainer_seArch, false);
    lv_obj_align(xseParentContainer_seArch, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(xseParentContainer_seArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(xseParentContainer_seArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(xseParentContainer_seArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    _xseContStatusBar_seArch = lv_cont_create(xseParentContainer_seArch, NULL);
    lv_obj_set_size(_xseContStatusBar_seArch, 320, 35);
    lv_obj_align(_xseContStatusBar_seArch, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xseContStatusBar_seArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_xseContStatusBar_seArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    __xseTimeLabel_seArch = lv_label_create(_xseContStatusBar_seArch, NULL);
    lv_obj_align(__xseTimeLabel_seArch, _xseContStatusBar_seArch, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__xseTimeLabel_seArch, guiTime); // guiTime
    // printf("Current time is = %s\n", guiTime);

    static lv_style_t _xseTimeLabelStyle_se;
    lv_style_init(&_xseTimeLabelStyle_se);
    lv_style_set_text_font(&_xseTimeLabelStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xseTimeLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseTimeLabel_seArch, LV_LABEL_PART_MAIN, &_xseTimeLabelStyle_se);

    // Create Label for Battery icon
    __xseBatteryLabel_seArch = lv_label_create(_xseContStatusBar_seArch, NULL);
    lv_obj_align(__xseBatteryLabel_seArch, _xseContStatusBar_seArch, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__xseBatteryLabel_seArch, LV_SYMBOL_BATTERY_FULL); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xseBatteryLabelStyle_se;
    lv_style_init(&_xseBatteryLabelStyle_se);
    lv_style_set_text_font(&_xseBatteryLabelStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_xseBatteryLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseBatteryLabel_seArch, LV_LABEL_PART_MAIN, &_xseBatteryLabelStyle_se);

    // Create Label for Wifi icon
    __xseWifiLabel_seArch = lv_label_create(_xseContStatusBar_seArch, NULL);
    lv_obj_align(__xseWifiLabel_seArch, __xseBatteryLabel_seArch, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__xseWifiLabel_seArch, LV_SYMBOL_WIFI);

    static lv_style_t _xseWifiLabelStyle_se;
    lv_style_init(&_xseWifiLabelStyle_se);
    lv_style_set_text_font(&_xseWifiLabelStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xseWifiLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseWifiLabel_seArch, LV_LABEL_PART_MAIN, &_xseWifiLabelStyle_se);

    // Create Label for Signal icon
    __xseSignalLabel_seArch = lv_label_create(_xseContStatusBar_seArch, NULL);
    lv_obj_align(__xseSignalLabel_seArch, __xseWifiLabel_seArch, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__xseSignalLabel_seArch, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t _xseSignalLabelStyle_se;
    lv_style_init(&_xseSignalLabelStyle_se);
    lv_style_set_text_font(&_xseSignalLabelStyle_se, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xseSignalLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseSignalLabel_seArch, LV_LABEL_PART_MAIN, &_xseSignalLabelStyle_se);

    lv_obj_t *_xseBackArrowContainerArch;
    _xseBackArrowContainerArch = lv_cont_create(xseParentContainer_seArch, NULL);
    lv_obj_set_size(_xseBackArrowContainerArch, 60, 60);
    // lv_obj_set_width(_xseBackArrowContainerArch, lv_page_get_width_fit(xseParentContainer_seArch));
    lv_obj_align(_xseBackArrowContainerArch, _xseContStatusBar_seArch, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 0);
    lv_obj_set_style_local_bg_color(_xseBackArrowContainerArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xseBackArrowContainerArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    // lv_obj_set_event_cb(_xseBackArrowContainerArch, __xseBackArrow_event_handler);

    // Create Back arrow img
    lv_obj_t *___xseBackArrowLabel_seArch;
    ___xseBackArrowLabel_seArch = lv_img_create(_xseBackArrowContainerArch, NULL);
    lv_img_set_src(___xseBackArrowLabel_seArch, &left_arrow_icon);
    lv_obj_align(___xseBackArrowLabel_seArch, _xseBackArrowContainerArch, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(___xseBackArrowLabel_seArch, true);
    lv_obj_set_style_local_image_recolor_opa(___xseBackArrowLabel_seArch, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(___xseBackArrowLabel_seArch, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    // lv_obj_set_event_cb(___xseBackArrowLabel_seArch, __xseBackArrow_event_handler);

    lv_obj_t *xTitleTxtArch;
    xTitleTxtArch = lv_label_create(xseParentContainer_seArch, NULL);
    lv_obj_align(xTitleTxtArch, _xseBackArrowContainerArch, LV_ALIGN_OUT_RIGHT_TOP, 50, 10);
    lv_label_set_text(xTitleTxtArch, "Archiv");
    lv_obj_set_style_local_text_font(xTitleTxtArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_obj_set_style_local_text_color(xTitleTxtArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *xArchieveLogo;
    xArchieveLogo = lv_img_create(xseParentContainer_seArch, NULL);
    lv_img_set_src(xArchieveLogo, &archiv_icon);
    lv_obj_align(xArchieveLogo, xTitleTxtArch, LV_ALIGN_OUT_RIGHT_TOP, 40, 0);
    lv_obj_set_click(xArchieveLogo, true);
    lv_obj_set_style_local_image_recolor_opa(xArchieveLogo, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(xArchieveLogo, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    lv_obj_t *_xseContNavBTNSamNArch;
    _xseContNavBTNSamNArch = lv_cont_create(xseParentContainer_seArch, NULL);
    lv_obj_set_size(_xseContNavBTNSamNArch, 320, 80);
    // lv_obj_set_width(_xseBackArrowContainerArch, lv_page_get_width_fit(xseParentContainer_seArch));
    lv_obj_align(_xseContNavBTNSamNArch, xseParentContainer_seArch, LV_ALIGN_IN_TOP_LEFT, 0, 85);
    lv_obj_set_style_local_bg_color(_xseContNavBTNSamNArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xseContNavBTNSamNArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_t *xPrevNavBTNArch;
    xPrevNavBTNArch = lv_label_create(_xseContNavBTNSamNArch, NULL);
    lv_obj_align(xPrevNavBTNArch, _xseContNavBTNSamNArch, LV_ALIGN_IN_TOP_LEFT, 30, 2);
    lv_label_set_text(xPrevNavBTNArch, LV_SYMBOL_PREV);
    lv_obj_set_style_local_text_font(xPrevNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(xPrevNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *xNextNavBTNArch;
    xNextNavBTNArch = lv_label_create(_xseContNavBTNSamNArch, NULL);
    lv_obj_align(xNextNavBTNArch, _xseContNavBTNSamNArch, LV_ALIGN_IN_TOP_RIGHT, -20, 2);
    lv_label_set_text(xNextNavBTNArch, LV_SYMBOL_NEXT);
    lv_obj_set_style_local_text_font(xNextNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(xNextNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *xSampleNoLblArch;
    xSampleNoLblArch = lv_label_create(_xseContNavBTNSamNArch, NULL);
    lv_obj_align(xSampleNoLblArch, _xseContNavBTNSamNArch, LV_ALIGN_IN_TOP_MID, -20, 2);
    lv_label_set_text(xSampleNoLblArch, "Sample \n N°1002");
    lv_obj_set_style_local_text_font(xSampleNoLblArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(xSampleNoLblArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create Sliding page
    lv_obj_t *_xScrollPageArch;
    _xScrollPageArch = lv_page_create(xseParentContainer_seArch, NULL);
    lv_obj_set_size(_xScrollPageArch, 320, 330);
    lv_obj_align(_xScrollPageArch, _xseContNavBTNSamNArch, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -25);
    lv_page_set_scrollbar_mode(_xScrollPageArch, LV_SCROLLBAR_MODE_HIDE);
    lv_obj_set_style_local_bg_color(_xScrollPageArch, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_width(_xScrollPageArch, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    // Create Big Back Container
    lv_obj_t *_xBigBlankContArch;
    _xBigBlankContArch = lv_cont_create(_xScrollPageArch, NULL);
    lv_obj_set_size(_xBigBlankContArch, 310, 350);
    lv_obj_set_click(_xBigBlankContArch, false);
    lv_obj_set_width(_xBigBlankContArch, lv_page_get_width_fit(_xScrollPageArch));
    lv_obj_align(_xBigBlankContArch, _xScrollPageArch, LV_ALIGN_IN_TOP_LEFT, 5, 0);
    lv_obj_set_style_local_bg_color(_xBigBlankContArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xBigBlankContArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Small Black Container
    lv_obj_t *_xSmallBlankContArch;
    _xSmallBlankContArch = lv_cont_create(_xScrollPageArch, NULL);
    lv_obj_set_size(_xSmallBlankContArch, 310, 100);
    lv_obj_set_click(_xSmallBlankContArch, false);
    lv_obj_set_width(_xSmallBlankContArch, lv_page_get_width_fit(_xScrollPageArch));
    lv_obj_align(_xSmallBlankContArch, _xBigBlankContArch, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_style_local_bg_color(_xSmallBlankContArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xSmallBlankContArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    crnt_screen = scrSummaryEndArch;
    screenid = SCR_ARCHIEVE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
