/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-16
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include "screen_includes.h"

#include <sensorManagement.h>
#include <calibration.h>
/*********************
 *      DEFINES
 *********************/
#define TAG  "METRO ADJUST"
#define SYMBOL_SIGNAL "\uf012"

//Declare Fonts here
LV_FONT_DECLARE(signal_20)

//Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)	

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void  __mtaBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void _mtaEnterRefValTA_event_cb(lv_obj_t * ta, lv_event_t event);
static void _mtavalidbuttoncalled_event_cb(lv_obj_t * ta, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/

lv_obj_t *crnt_screen;
lv_obj_t * scrMetroTmpAdj;
lv_obj_t * _mtaEnterRefValTA;
lv_obj_t * _mtaKeyBord;
lv_obj_t * mtaPatrentCont;
lv_obj_t * _mtaContStatusBar;
lv_obj_t * __mtaTimeLabel;
lv_obj_t * __mtaBatteryLabel;
lv_obj_t * __mtaWifiLabel;
lv_obj_t * __mtaSignalLabel;
lv_obj_t * _mtaMetroHeadingCont;
lv_obj_t * __mtaBackArrowLabel;
lv_obj_t * __mtaMetroHeadingLbl;
lv_obj_t * _mtaPointTxtLbl;
lv_obj_t * _mtaPointValLbl;
lv_obj_t * _mtaFactoryValTxtLbl;
lv_obj_t * _mtaFactoryValueLbl;
lv_obj_t * _mtaValidBtn;
lv_obj_t * _mtaValidBtnLbl;
lv_obj_t * _mtaCorrectionTxtLbl;
lv_obj_t * _mtaCorrectionValueLbl;
lv_obj_t * _mtaPercentTxtLbl;

/*Create a custom map for the keyboard*/

    static const char * mta_kb_map[] = {
              "1","2", "3", LV_SYMBOL_BACKSPACE ,  "\n",
              "4", "5", "6", ",",  "\n",
              "7", "8", "9", "0" , "\n" , ""
              
    };

    static const lv_btnmatrix_ctrl_t mta_kb_ctrl[] = {
           80, 80, 80, 80,
           80, 80, 80, 80,
           80, 80, 80, 80,
           
    };

    static const lv_btnmatrix_ctrl_t mta_tgl_kb_ctrl[] = {
           LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
           LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
           LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
           
    };

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void callMetroAdjust(void)
{
    //Create Base container
    scrMetroTmpAdj = lv_obj_create(NULL, NULL);
    lv_scr_load(scrMetroTmpAdj);
    if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    mtaPatrentCont = lv_cont_create(scrMetroTmpAdj, NULL);
    lv_obj_set_size(mtaPatrentCont, 320, 480);
    lv_obj_set_click(mtaPatrentCont, false);
    lv_obj_align(mtaPatrentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( mtaPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(mtaPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(mtaPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status
    _mtaContStatusBar = lv_cont_create(mtaPatrentCont, NULL);
    lv_obj_set_size(_mtaContStatusBar, 320, 35);
    lv_obj_align(_mtaContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mtaContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mtaContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    __mtaTimeLabel = lv_label_create(_mtaContStatusBar, NULL);
    lv_obj_align(__mtaTimeLabel, _mtaContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__mtaTimeLabel, guiTime);

    static lv_style_t _mtaTimeLabelStyle;
    lv_style_init(&_mtaTimeLabelStyle);
    lv_style_set_text_font(&_mtaTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_mtaTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtaTimeLabel, LV_LABEL_PART_MAIN, &_mtaTimeLabelStyle);

    //Create Label for Battery icon
    __mtaBatteryLabel = lv_label_create(_mtaContStatusBar, NULL);
    lv_obj_align(__mtaBatteryLabel, _mtaContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__mtaBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t __mtaBatteryLabelStyle;
    lv_style_init(&__mtaBatteryLabelStyle);
    lv_style_set_text_font(&__mtaBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&__mtaBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtaBatteryLabel, LV_LABEL_PART_MAIN, &__mtaBatteryLabelStyle);

    //Create Label for Wifi icon 
    __mtaWifiLabel = lv_label_create(_mtaContStatusBar, NULL);
    lv_obj_align(__mtaWifiLabel, __mtaBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__mtaWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __mtaWifiLabelStyle;
    lv_style_init(&__mtaWifiLabelStyle);
    lv_style_set_text_font(&__mtaWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__mtaWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtaWifiLabel, LV_LABEL_PART_MAIN, &__mtaWifiLabelStyle);

    //Create Label for Signal icon
    __mtaSignalLabel = lv_label_create(_mtaContStatusBar, NULL);
    lv_obj_align(__mtaSignalLabel, __mtaSignalLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__mtaSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __mtaSignalLabelStyle;
    lv_style_init(&__mtaSignalLabelStyle);
    lv_style_set_text_font(&__mtaSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&__mtaSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtaSignalLabel, LV_LABEL_PART_MAIN, &__mtaSignalLabelStyle);

    //Crate a container to contain Summary Start Header
 
    _mtaMetroHeadingCont = lv_cont_create(mtaPatrentCont, NULL);
    lv_obj_set_size(_mtaMetroHeadingCont, 300, 70);
    lv_obj_align(_mtaMetroHeadingCont, _mtaContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mtaMetroHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_mtaMetroHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    // Create Back arrow img
    __mtaBackArrowLabel = lv_img_create(mtaPatrentCont, NULL);
    lv_img_set_src(__mtaBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__mtaBackArrowLabel, _mtaMetroHeadingCont, LV_ALIGN_IN_LEFT_MID, 5 , 0);
    lv_obj_set_click(__mtaBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__mtaBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__mtaBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__mtaBackArrowLabel, __mtaBackArrow_event_handler);

    //Create Label for External Temperature "Heading"
    __mtaMetroHeadingLbl = lv_label_create(_mtaMetroHeadingCont, NULL);
    if(screenid == SCR_EXTERNAL_TEMPERATURE_SETTINGS){
        lv_obj_align(__mtaMetroHeadingLbl, _mtaMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -50, -35);
        lv_label_set_text(__mtaMetroHeadingLbl, "External \nTemperature");
        screenid = SCR_EXTERNAL_TEMPERATURE_ADJUST;
    }else if(screenid == SCR_EXTERNAL_PRESSURE_SETTINGS){
        lv_obj_align(__mtaMetroHeadingLbl, _mtaMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -30, -35);
        lv_label_set_text(__mtaMetroHeadingLbl, "External \nPressure");
        screenid = SCR_EXTERNAL_PRESSURE_ADJUST;
    }else if(screenid == SCR_EXTERNAL_HUMIDITY_SETTINGS){
        lv_obj_align(__mtaMetroHeadingLbl, _mtaMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -30, -35);
        lv_label_set_text(__mtaMetroHeadingLbl, "External  \nHumidity");  
        screenid = SCR_EXTERNAL_HUMIDITY_ADJUST;  
    }else if(screenid == SCR_INTERNAL_TEMPERATURE_SETTINGS){
        lv_obj_align(__mtaMetroHeadingLbl, _mtaMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -50, -35);
        lv_label_set_text(__mtaMetroHeadingLbl, "Internal  \nTemperature");  
        screenid = SCR_INTERNAL_TEMPERATURE_ADJUST;  
    }else if(screenid == SCR_INTERNAL_PRESSURE_SETTINGS){
        lv_obj_align(__mtaMetroHeadingLbl, _mtaMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -30, -35);
        lv_label_set_text(__mtaMetroHeadingLbl, "Internal  \nPressure");  
        screenid = SCR_INTERNAL_PRESSURE_ADJUST;  
    }else if(screenid == SCR_INTERNAL_HUMIDITY_SETTINGS){
        lv_obj_align(__mtaMetroHeadingLbl, _mtaMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -30, -35);
        lv_label_set_text(__mtaMetroHeadingLbl, "Internal  \nHumidity");  
        screenid = SCR_INTERNAL_HUMIDITY_ADJUST;  
    }
    
    static lv_style_t __mteMetroHeadingLblStyle;
    lv_style_init(&__mteMetroHeadingLblStyle);
    lv_style_set_text_font(&__mteMetroHeadingLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__mteMetroHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtaMetroHeadingLbl, LV_LABEL_PART_MAIN, &__mteMetroHeadingLblStyle);
    lv_label_set_align(__mtaMetroHeadingLbl, LV_LABEL_ALIGN_CENTER);

    //Create Label for "POINT:" Text
    _mtaPointTxtLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaPointTxtLbl, _mtaMetroHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mtaPointTxtLbl, "POINT:");

    //Create Label for "POINT" Value
    _mtaPointValLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaPointValLbl, _mtaMetroHeadingCont, LV_ALIGN_OUT_BOTTOM_MID, 15, 10);
    lv_label_set_text(_mtaPointValLbl, "1");
    lv_obj_set_style_local_text_color(_mtaPointValLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //Create Label for "FACTORY VALUE:" Text
    _mtaFactoryValTxtLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaFactoryValTxtLbl, _mtaPointTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_mtaFactoryValTxtLbl, "FACTORY VALUE:");

    //Create Label for "FACTORY VALUE:" Value
    _mtaFactoryValueLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaFactoryValueLbl, _mtaPointValLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text(_mtaFactoryValueLbl, "24,67");
    lv_obj_set_style_local_text_color(_mtaFactoryValueLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //============================================================================================================
    //============================================================================================================
    //============================================================================================================

    _mtaEnterRefValTA = lv_textarea_create(mtaPatrentCont, NULL);
    lv_obj_set_size( _mtaEnterRefValTA, 210, 50);
    lv_obj_align(  _mtaEnterRefValTA,  _mtaFactoryValTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_click(_mtaEnterRefValTA, false);
    lv_textarea_set_text(_mtaEnterRefValTA, "");  //"Enter REFERENCE Value"
    lv_textarea_set_text_align(_mtaEnterRefValTA, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_event_cb(_mtaEnterRefValTA, _mtaEnterRefValTA_event_cb);
    lv_obj_set_style_local_text_color(_mtaEnterRefValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(_mtaEnterRefValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, &lv_font_montserrat_26);
    lv_obj_set_style_local_bg_color(_mtaEnterRefValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_obj_set_style_local_border_width(_mtaEnterRefValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, 0);
    lv_textarea_set_cursor_blink_time(_mtaEnterRefValTA, 0);

    // Create valid button
    _mtaValidBtn = lv_btn_create(mtaPatrentCont, NULL);
    lv_obj_set_size(_mtaValidBtn, 90, 54);
    lv_obj_align(  _mtaValidBtn,  _mtaEnterRefValTA, LV_ALIGN_OUT_RIGHT_MID, -5, 0);
    lv_obj_set_style_local_radius(_mtaValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);

    _mtaValidBtnLbl = lv_label_create(_mtaValidBtn, NULL);
    lv_obj_align(_mtaValidBtnLbl, _mtaValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_mtaValidBtnLbl, "VALID");
    
    static lv_style_t _mtaValidBtnLblStyle;
    lv_style_init(&_mtaValidBtnLblStyle);
    lv_style_set_text_font(&_mtaValidBtnLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mtaValidBtnLblStyle, LV_STATE_DEFAULT  ,0); 
    lv_style_set_text_color(&_mtaValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mtaValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mtaValidBtnLblStyle, LV_STATE_DEFAULT, 0);

    static lv_style_t _mtaValidBtnLblStylepressed;
    lv_style_init(&_mtaValidBtnLblStylepressed);
    lv_style_set_text_font(&_mtaValidBtnLblStylepressed, LV_STATE_PRESSED  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mtaValidBtnLblStylepressed, LV_STATE_PRESSED, 2);
    lv_style_set_border_color(&_mtaValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_BLUE); 
    lv_style_set_text_color(&_mtaValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mtaValidBtnLblStyle, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mtaValidBtnLblStyle, LV_STATE_PRESSED, 0);

    static lv_style_t _mtaValidBtnLblStylefocused;
    lv_style_init(&_mtaValidBtnLblStylefocused);
    lv_style_set_text_font(&_mtaValidBtnLblStylefocused, LV_STATE_FOCUSED  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mtaValidBtnLblStylefocused, LV_STATE_FOCUSED  ,0); 
    lv_style_set_text_color(&_mtaValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mtaValidBtnLblStyle, LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mtaValidBtnLblStyle, LV_STATE_FOCUSED, 0);

    lv_obj_add_style(_mtaValidBtn, LV_BTN_PART_MAIN, &_mtaValidBtnLblStyle);
    lv_obj_add_style(_mtaValidBtn, LV_BTN_PART_MAIN, &_mtaValidBtnLblStylepressed);
    lv_obj_add_style(_mtaValidBtn, LV_BTN_PART_MAIN, &_mtaValidBtnLblStylefocused);
    lv_obj_set_event_cb(_mtaValidBtn, _mtavalidbuttoncalled_event_cb);

    // Create Label For "CORRECTION" Text
    _mtaCorrectionTxtLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaCorrectionTxtLbl, _mtaEnterRefValTA, LV_ALIGN_OUT_BOTTOM_LEFT, 70, 10);
    lv_label_set_text(_mtaCorrectionTxtLbl, "CORRECTION:");

    static lv_style_t _mtaCorrectionTxtLblStyle;
    lv_style_init(&_mtaCorrectionTxtLblStyle);
    lv_style_set_text_font(&_mtaCorrectionTxtLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); 
    lv_style_set_text_color(&_mtaCorrectionTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_mtaCorrectionTxtLbl, LV_LABEL_PART_MAIN, &_mtaCorrectionTxtLblStyle);

    // Create Label For "CORRECTION" Text
    _mtaCorrectionValueLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaCorrectionValueLbl, _mtaCorrectionTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 10, -5);
    lv_label_set_text(_mtaCorrectionValueLbl, "__,__");

    static lv_style_t _mtaCorrValueLblStyle;
    lv_style_init(&_mtaCorrValueLblStyle);
    lv_style_set_text_font(&_mtaCorrValueLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_mtaCorrValueLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_add_style(_mtaCorrectionValueLbl, LV_LABEL_PART_MAIN, &_mtaCorrValueLblStyle);

    // Create Label For "%" Text
    _mtaPercentTxtLbl = lv_label_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaPercentTxtLbl, _mtaCorrectionValueLbl, LV_ALIGN_OUT_RIGHT_MID, 10, -5);
    lv_label_set_text(_mtaPercentTxtLbl, "%");

    static lv_style_t _mtaPercentTxtLblStyle;
    lv_style_init(&_mtaPercentTxtLblStyle);
    lv_style_set_text_font(&_mtaPercentTxtLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_mtaPercentTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_add_style(_mtaPercentTxtLbl, LV_LABEL_PART_MAIN, &_mtaPercentTxtLblStyle);

    /* Create a keyboard*/
    _mtaKeyBord = lv_keyboard_create(mtaPatrentCont, NULL);
    lv_obj_align(_mtaKeyBord,_mtaCorrectionTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, -70,15 );
    lv_obj_set_size(_mtaKeyBord,  300, 290);
    lv_keyboard_set_mode(_mtaKeyBord, LV_KEYBOARD_MODE_NUM);
    lv_keyboard_set_map(_mtaKeyBord, LV_KEYBOARD_MODE_NUM, mta_kb_map);
    lv_keyboard_set_textarea(_mtaKeyBord, _mtaEnterRefValTA);
    lv_keyboard_set_ctrl_map(_mtaKeyBord, LV_KEYBOARD_MODE_NUM,mta_kb_ctrl);
    lv_keyboard_set_ctrl_map(_mtaKeyBord, LV_KEYBOARD_MODE_NUM,mta_tgl_kb_ctrl);
    lv_obj_set_style_local_radius(_mtaKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
    lv_obj_set_style_local_bg_color(_mtaKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_bg_color(_mtaKeyBord, LV_KEYBOARD_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_text_font(_mtaKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, &lv_font_montserrat_28);
    lv_obj_set_style_local_text_color(_mtaKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_width(_mtaKeyBord, LV_KEYBOARD_PART_BG, LV_STATE_DEFAULT, 0);
    

    crnt_screen = scrMetroTmpAdj; //scrMetroTmpAdj    
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void _mtavalidbuttoncalled_event_cb(lv_obj_t * ta, lv_event_t event)
{
    const char *tempbuffer;
    float referencevalue;
    float calibrationvalue;
    external_sensor_data_t external_sensordata;

    if(event == LV_EVENT_RELEASED) 
    {
        tempbuffer = lv_textarea_get_text(_mtaEnterRefValTA);
        referencevalue = atof(tempbuffer);
         
        if(screenid == SCR_EXTERNAL_TEMPERATURE_ADJUST){
            vGetExternalSensorData(&external_sensordata);
            calibrationvalue = (external_sensordata.fTemperature - referencevalue);
            setcalibrationvalue_ext_temperature(calibrationvalue);
            ESP_LOGI(TAG, "External Temperature calibration : %.02f", calibrationvalue);
            callMetroPressureSettingScreen();
        }else if(screenid == SCR_INTERNAL_TEMPERATURE_ADJUST){
            calibrationvalue = (fGetBme280TemperatureAverages() - referencevalue);
            setcalibrationvalue_int_temperature(calibrationvalue);
            ESP_LOGI(TAG, "Internal Temperature calibration : %.02f", calibrationvalue);
            callMetroPressureSettingScreen();
        }else if(screenid == SCR_EXTERNAL_PRESSURE_ADJUST ){
            vGetExternalSensorData(&external_sensordata);
            calibrationvalue = (external_sensordata.fPressure - referencevalue);
            setcalibrationvalue_ext_pressure(calibrationvalue);
            ESP_LOGI(TAG, "External Pressure calibration : %.02f", calibrationvalue);
            callMetroHumiditySettingScreen();
        }else if( screenid == SCR_INTERNAL_PRESSURE_ADJUST){
            calibrationvalue = (fGetBme280PressureAverages() - referencevalue);
            setcalibrationvalue_int_pressure(calibrationvalue);
            ESP_LOGI(TAG, "Internal Pressure calibration : %.02f", calibrationvalue);
            callMetroHumiditySettingScreen();
        }else if(screenid == SCR_EXTERNAL_HUMIDITY_ADJUST){
            vGetExternalSensorData(&external_sensordata);
            calibrationvalue = (external_sensordata.fHumidity - referencevalue);
            setcalibrationvalue_ext_humidity(calibrationvalue);
            ESP_LOGI(TAG, "External Humidity calibration : %.02f", calibrationvalue);
            callMetroTempSettingScreen();
        }else if(screenid == SCR_INTERNAL_HUMIDITY_ADJUST){
            calibrationvalue = (fGetBme280HumidityAverages() - referencevalue);
            setcalibrationvalue_int_humidity(calibrationvalue);
            ESP_LOGI(TAG, "Internal Humidity calibration : %.02f", calibrationvalue);
            callMetroFlowSettingScreen();
        }

    }
}

static void  __mtaBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) 
    {
        CallMetroMenuScreen();
    }
}

static void _mtaEnterRefValTA_event_cb(lv_obj_t * ta, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) {
        /* Focus on the clicked text area */
        lv_textarea_set_text(_mtaEnterRefValTA, "");
        if(_mtaKeyBord != NULL)
        {
            
            lv_keyboard_set_textarea(_mtaKeyBord, ta);
        }
            
    }

    else if(event == LV_EVENT_INSERT) {
        const char * str = lv_event_get_data();
        if(str[0] == '\n') {
            printf("Ready\n");
        }
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
