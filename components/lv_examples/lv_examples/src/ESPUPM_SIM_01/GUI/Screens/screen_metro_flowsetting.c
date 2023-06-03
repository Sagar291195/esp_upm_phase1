/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-24
 */

/**
*  @file screen_metro_flowsetting.c
*  @brief This Screen is see the flow settings
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

//Declare Fonts here
LV_FONT_DECLARE(signal_20)

//Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)	
LV_IMG_DECLARE(fan_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void  __mfsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void  curve_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  unit_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  lowerlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  higherlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  __mfsValidAdjBTN_event_handler(lv_obj_t * obj, lv_event_t event);
/**********************
 *  STATIC VARIABLES
 **********************/


lv_obj_t *crnt_screen;
lv_obj_t * scrFlowSetting;
lv_obj_t * mfsParentCont;
lv_obj_t * _mfsContStatusBar;
lv_obj_t * __mfsTimeLabel;
lv_obj_t * __mfsBatteryLabel;
lv_obj_t * __mfsWifiLabel;
lv_obj_t * __mfsSignalLabel;
lv_obj_t * _mfsFlowHeadingCont;
lv_obj_t * __mfsBackArrowLabel;
lv_obj_t * __mfsFlowHeadingLbl;
lv_obj_t * _mfsFlowLogo;
lv_obj_t * _mfsCurveUnitCont;


int global_CurveDegree;

int metroflowUnit;    // for LPH flowUnit == 0, For LPM == 1

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void callMetroFlowSettingScreen(void)
{
    //Create Base container
    
    
    scrFlowSetting = lv_cont_create(NULL, NULL);
    lv_scr_load(scrFlowSetting);
    //lv_scr_load(scrFlowSetting);
    mfsParentCont = lv_cont_create(scrFlowSetting, NULL);
    lv_obj_set_size(mfsParentCont, 320, 480);
    lv_obj_set_click(mfsParentCont, false);
    lv_obj_align(mfsParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( mfsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(mfsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(mfsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    
    _mfsContStatusBar = lv_cont_create(mfsParentCont, NULL);
    lv_obj_set_size(_mfsContStatusBar, 320, 35);
    lv_obj_align(_mfsContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mfsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mfsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    
    __mfsTimeLabel = lv_label_create(_mfsContStatusBar, NULL);
    lv_obj_align(__mfsTimeLabel, _mfsContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__mfsTimeLabel, guiTime);

    static lv_style_t _mfsTimeLabelStyle;
    lv_style_init(&_mfsTimeLabelStyle);
    lv_style_set_text_font(&_mfsTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_mfsTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mfsTimeLabel, LV_LABEL_PART_MAIN, &_mfsTimeLabelStyle);

    //Create Label for Battery icon
    
    __mfsBatteryLabel = lv_label_create(_mfsContStatusBar, NULL);
    lv_obj_align(__mfsBatteryLabel, _mfsContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__mfsBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t __mfsBatteryLabelStyle;
    lv_style_init(&__mfsBatteryLabelStyle);
    lv_style_set_text_font(&__mfsBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&__mfsBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mfsBatteryLabel, LV_LABEL_PART_MAIN, &__mfsBatteryLabelStyle);

    //Create Label for Wifi icon
    __mfsWifiLabel = lv_label_create(_mfsContStatusBar, NULL);
    lv_obj_align(__mfsWifiLabel, __mfsBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__mfsWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __mfsWifiLabelStyle;
    lv_style_init(&__mfsWifiLabelStyle);
    lv_style_set_text_font(&__mfsWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__mfsWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mfsWifiLabel, LV_LABEL_PART_MAIN, &__mfsWifiLabelStyle);

    //Create Label for Signal icon
    
    __mfsSignalLabel = lv_label_create(_mfsContStatusBar, NULL);
    lv_obj_align(__mfsSignalLabel, __mfsWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__mfsSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __mfsSignalLabelStyle;
    lv_style_init(&__mfsSignalLabelStyle);
    lv_style_set_text_font(&__mfsSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&__mfsSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mfsSignalLabel, LV_LABEL_PART_MAIN, &__mfsSignalLabelStyle);

    //Crate a container to contain FLOW Header

    
    _mfsFlowHeadingCont = lv_cont_create(mfsParentCont, NULL);
    lv_obj_set_size(_mfsFlowHeadingCont, 300, 70);
    lv_obj_align(_mfsFlowHeadingCont, _mfsContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mfsFlowHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_mfsFlowHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    // Create Back arrow img
    
    __mfsBackArrowLabel = lv_img_create(mfsParentCont, NULL);
    lv_img_set_src(__mfsBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__mfsBackArrowLabel, _mfsFlowHeadingCont, LV_ALIGN_IN_LEFT_MID, 5 , 0);
    lv_obj_set_click(__mfsBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__mfsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__mfsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__mfsBackArrowLabel, __mfsBackArrow_event_handler);

    //Create Label for FLOW "Heading"
    
    __mfsFlowHeadingLbl = lv_label_create(_mfsFlowHeadingCont, NULL);
    lv_obj_align(__mfsFlowHeadingLbl, _mfsFlowHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -10, -35);
    lv_label_set_text(__mfsFlowHeadingLbl, "Flow");

    static lv_style_t __fasFlowHeadingLblStyle;
    lv_style_init(&__fasFlowHeadingLblStyle);
    lv_style_set_text_font(&__fasFlowHeadingLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__fasFlowHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mfsFlowHeadingLbl, LV_LABEL_PART_MAIN, &__fasFlowHeadingLblStyle);

    //Create FAN Logo
    
    _mfsFlowLogo = lv_img_create(mfsParentCont, NULL);
    lv_img_set_src(_mfsFlowLogo, &fan_icon);
    lv_obj_align(_mfsFlowLogo, mfsParentCont, LV_ALIGN_IN_TOP_RIGHT, -25 , 55);
    lv_img_set_auto_size(_mfsFlowLogo, true);

    //===============================================================
    //===============================================================

    _mfsCurveUnitCont = lv_cont_create(mfsParentCont, NULL);
    lv_obj_set_size(_mfsCurveUnitCont, 300, 110);
    lv_obj_align(_mfsCurveUnitCont, _mfsFlowHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_mfsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mfsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    static lv_style_t _mfsBlueTxtStyle;
    lv_style_init(&_mfsBlueTxtStyle);
    lv_style_set_text_font(&_mfsBlueTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_mfsBlueTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35,0x9F,0xE2));

    lv_obj_t * _mfsCurveTxt;
    _mfsCurveTxt = lv_label_create(_mfsCurveUnitCont, NULL);
    lv_obj_align(_mfsCurveTxt, _mfsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, -25);
    lv_label_set_text(_mfsCurveTxt, "CURVE");
    lv_obj_add_style(_mfsCurveTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    lv_obj_t * _mfsUnitTxt;
    _mfsUnitTxt = lv_label_create(_mfsCurveUnitCont, NULL);
    lv_obj_align(_mfsUnitTxt, _mfsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, 15);
    lv_label_set_text(_mfsUnitTxt, "UNIT");
    lv_obj_add_style(_mfsUnitTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    //```````````````````````````````````````````````````````````
    //
    //```````````````````````````````````````````````````````````

    //Create a Curve selection drop down list
    lv_obj_t * _mfsCurveDropDown = lv_dropdown_create(_mfsCurveUnitCont, NULL);
    // lv_dropdown_set_options(_mfsCurveDropDown, "Linear1\n"
    //              "Linear2\n"
    //              "Linear3\n"
    //              "Linear4\n"
    //              "Linear5\n"
    //              "Linear6\n"
    //              "Linear7\n"
    //              "Linear8"
    //              );

    lv_dropdown_set_options(_mfsCurveDropDown, 
                 "Linear4\n"
                 "Linear5\n"
                 "Linear6\n"
                 "Linear7\n"
                 "Linear8"
                 );
                

    lv_obj_align(_mfsCurveDropDown, _mfsCurveTxt, LV_ALIGN_OUT_RIGHT_TOP, 80, -2);
    lv_obj_set_size(_mfsCurveDropDown, 120, 30);
    
    static lv_style_t _mfsCurveDropDownStyle;
    lv_style_init(&_mfsCurveDropDownStyle);
    lv_style_set_bg_color(&_mfsCurveDropDownStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mfsCurveDropDownStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mfsCurveDropDownStyle, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mfsCurveDropDownStyle, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_mfsCurveDropDown, LV_DROPDOWN_PART_MAIN, &_mfsCurveDropDownStyle);
    lv_obj_set_event_cb(_mfsCurveDropDown, curve_dropdown_event_handler);

    //===-------------------------->----------------->---------------------->------------->

    //Create a Unit selection drop down list
    lv_obj_t * _mfsUnitDropDown = lv_dropdown_create(_mfsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mfsUnitDropDown, "LPM\n"
                                              "LPH");
                

    lv_obj_align(_mfsUnitDropDown, _mfsUnitTxt, LV_ALIGN_OUT_RIGHT_TOP, 97, -2);
    lv_obj_set_size(_mfsUnitDropDown, 120, 30);
    
    static lv_style_t _mfsUnitDropDownStyle;
    lv_style_init(&_mfsUnitDropDownStyle);
    lv_style_set_bg_color(&_mfsUnitDropDownStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mfsUnitDropDownStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mfsUnitDropDownStyle, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mfsUnitDropDownStyle, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_mfsUnitDropDown, LV_DROPDOWN_PART_MAIN, &_mfsUnitDropDownStyle);
    lv_obj_set_event_cb(_mfsUnitDropDown, unit_dropdown_event_handler);

    //=================================================================
    //=================================================================

    lv_obj_t * _mfsAlarmStPtCont;
    _mfsAlarmStPtCont = lv_cont_create(mfsParentCont, NULL);
    lv_obj_set_size(_mfsAlarmStPtCont, 300, 180);
    lv_obj_align(_mfsAlarmStPtCont, _mfsCurveUnitCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_mfsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mfsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    static lv_style_t _mfsBigTxtStyle;
    lv_style_init(&_mfsBigTxtStyle);
    lv_style_set_text_font(&_mfsBigTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_mfsBigTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    static lv_style_t _mfsSmallTxtStyle;
    lv_style_init(&_mfsSmallTxtStyle);
    lv_style_set_text_font(&_mfsSmallTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); //signal_20
    lv_style_set_text_color(&_mfsSmallTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    lv_obj_t * _mfsAlarmSetPointTxt;
    _mfsAlarmSetPointTxt = lv_label_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsAlarmSetPointTxt, _mfsAlarmStPtCont, LV_ALIGN_IN_TOP_LEFT, 10, 8);
    lv_label_set_text(_mfsAlarmSetPointTxt, "ALERT FROM SET POINT");
    lv_obj_add_style(_mfsAlarmSetPointTxt, LV_LABEL_PART_MAIN, &_mfsBigTxtStyle);

    lv_obj_t * _mfsOnOffTxt;
    _mfsOnOffTxt = lv_label_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsOnOffTxt, _mfsAlarmSetPointTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mfsOnOffTxt, "ON / OFFF");
    lv_obj_add_style(_mfsOnOffTxt, LV_LABEL_PART_MAIN, &_mfsSmallTxtStyle);

    //=========================
    //=========================

    //Create On / OFF control switch 

    lv_obj_t *_mfsOnOffSwitch = lv_switch_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsOnOffSwitch, _mfsOnOffTxt, LV_ALIGN_OUT_RIGHT_TOP, 170, -8);
    lv_obj_set_height(_mfsOnOffSwitch, 20);
    //lv_obj_set_event_cb(_mfsOnOffSwitch, Buzzer_switch_event_handler);
   

    
    static lv_style_t _mfsOnOffSwitchStle;
    lv_style_init(&_mfsOnOffSwitchStle);
    lv_style_set_bg_color(&_mfsOnOffSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_width(&_mfsOnOffSwitchStle, LV_STATE_DEFAULT, 2 );
    lv_style_set_border_color(&_mfsOnOffSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mfsOnOffSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_color(&_mfsOnOffSwitchStle,  LV_SWITCH_PART_INDIC, LV_COLOR_GREEN);
    lv_obj_add_style(_mfsOnOffSwitch, LV_SWITCH_PART_BG, &_mfsOnOffSwitchStle);


    //==============================
    //==============================


    lv_obj_t * _mfsLowerLimitTxt;
    _mfsLowerLimitTxt = lv_label_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsLowerLimitTxt, _mfsOnOffTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mfsLowerLimitTxt, "LOWER LIMIT");
    lv_obj_add_style(_mfsLowerLimitTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    //============================
    //============================

    //Create a Curve selection drop down list
    lv_obj_t * _mfsLowerLimDropDown = lv_dropdown_create(_mfsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mfsLowerLimDropDown, "-10%\n"
                "-20%");
                

    lv_obj_align(_mfsLowerLimDropDown, _mfsLowerLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 70, -4);
    lv_obj_set_size(_mfsLowerLimDropDown, 120, 30);
    
    static lv_style_t _mfsLowerLimDropDownStyle;
    lv_style_init(&_mfsLowerLimDropDownStyle);
    lv_style_set_bg_color(&_mfsLowerLimDropDownStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mfsLowerLimDropDownStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mfsLowerLimDropDownStyle, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mfsLowerLimDropDownStyle, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_mfsLowerLimDropDown, LV_DROPDOWN_PART_MAIN, &_mfsLowerLimDropDownStyle);
    lv_obj_set_event_cb(_mfsLowerLimDropDown, lowerlim_dropdown_event_handler);

    //============================
    //============================

    lv_obj_t * _mfsHigherLimitTxt;
    _mfsHigherLimitTxt = lv_label_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsHigherLimitTxt, _mfsLowerLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(_mfsHigherLimitTxt, "HIGHETR LIMIT");
    lv_obj_add_style(_mfsHigherLimitTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    //============================
    //============================

    //Create a Curve selection drop down list
    lv_obj_t * _mfsHigherLimDropDown = lv_dropdown_create(_mfsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mfsHigherLimDropDown, "10%\n"
                "20%");
                

    lv_obj_align(_mfsHigherLimDropDown, _mfsHigherLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mfsHigherLimDropDown, 120, 30);
    
    static lv_style_t _mfsHigherLimDropDownStyle;
    lv_style_init(&_mfsHigherLimDropDownStyle);
    lv_style_set_bg_color(&_mfsHigherLimDropDownStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mfsHigherLimDropDownStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mfsHigherLimDropDownStyle, LV_DROPDOWN_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mfsHigherLimDropDownStyle, LV_DROPDOWN_PART_MAIN, 0);
    lv_obj_add_style(_mfsHigherLimDropDown, LV_DROPDOWN_PART_MAIN, &_mfsHigherLimDropDownStyle);
    lv_obj_set_event_cb(_mfsHigherLimDropDown, higherlim_dropdown_event_handler);

    //============================
    //============================

    lv_obj_t * _mfsActionTxt;
    _mfsActionTxt = lv_label_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsActionTxt, _mfsHigherLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_mfsActionTxt, "ACTION");
    lv_obj_add_style(_mfsActionTxt, LV_LABEL_PART_MAIN, &_mfsBigTxtStyle);

    lv_obj_t * _mfsDisableDeviceTxt;
    _mfsDisableDeviceTxt = lv_label_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsDisableDeviceTxt, _mfsActionTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    lv_label_set_text(_mfsDisableDeviceTxt, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_mfsDisableDeviceTxt, LV_LABEL_PART_MAIN, &_mfsSmallTxtStyle);

    //=========================
    //=========================

    //Create On / OFF control switch 

    lv_obj_t *_mfsActionSwitch = lv_switch_create(_mfsAlarmStPtCont, NULL);
    lv_obj_align(_mfsActionSwitch, _mfsActionTxt, LV_ALIGN_OUT_RIGHT_TOP, 178, 5);
    lv_obj_set_height(_mfsActionSwitch, 20);
    //lv_obj_set_event_cb(_mfsOnOffSwitch, Buzzer_switch_event_handler);
   

    
    static lv_style_t _mfsActionSwitchStle;
    lv_style_init(&_mfsActionSwitchStle);
    lv_style_set_bg_color(&_mfsActionSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_width(&_mfsActionSwitchStle, LV_STATE_DEFAULT, 2 );
    lv_style_set_border_color(&_mfsActionSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mfsActionSwitchStle, LV_SWITCH_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_color(&_mfsActionSwitchStle,  LV_SWITCH_PART_INDIC, LV_COLOR_GREEN);
    lv_obj_add_style(_mfsActionSwitch, LV_SWITCH_PART_BG, &_mfsActionSwitchStle);


    //==============================
    //==============================
    


    //===================================================================
    //===================================================================

    //Creat a stop Button
    lv_obj_t * _mfsValidBtn;
    _mfsValidBtn = lv_btn_create(mfsParentCont, NULL);
    lv_obj_align(_mfsValidBtn, mfsParentCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );
    lv_obj_set_size(_mfsValidBtn, 300, 44);
    lv_obj_set_style_local_radius(_mfsValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(_mfsValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_set_event_cb(_mfsValidBtn,  __mfsValidAdjBTN_event_handler);
   

    //Creat a stop Button Label
    lv_obj_t * _mfsValidTxt;
    _mfsValidTxt = lv_label_create(_mfsValidBtn, NULL);
    lv_obj_align(_mfsValidTxt, _mfsValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_mfsValidTxt, "VALID & ADJUST");
    lv_obj_set_style_local_text_font(_mfsValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_text_color(_mfsValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    crnt_screen = scrFlowSetting;

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void  __mfsValidAdjBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Back to Metrology Menu Screen\n");
        xCallFlowCalibrationScreen();
    }
}

static void  __mfsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Back to Metrology Menu Screen\n");
        CallMetroMenuScreen();
    }
}

static void  curve_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
        if(  strcmp (buf, "Linear1") == 0)
        {
            global_CurveDegree = 1;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear2") == 0)
        {
            global_CurveDegree = 2;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear3") == 0)
        {
            global_CurveDegree = 3;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear4") == 0)
        {
            global_CurveDegree = 4;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear5") == 0)
        {
            global_CurveDegree = 5;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear6") == 0)
        {
            global_CurveDegree = 6;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear7") == 0)
        {
            global_CurveDegree = 7;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear8") == 0)
        {
            global_CurveDegree = 8;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear9") == 0)
        {
            global_CurveDegree = 9;
            printf("Curve Points: %d\n", global_CurveDegree);
            fflush(NULL);
        }
        else{}


            
    }
}

static void  unit_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
        if( ! strcmp( buf, "LPH"))
        {
            metroflowUnit = 0;
            printf("Flow Unit selected is LPH = %d\n", metroflowUnit);
        }
        if( ! strcmp( buf, "LPM"))
        {
            metroflowUnit = 1;
            printf("Flow Unit selected is LPM = %d\n", metroflowUnit);
        }
    }
}

static void  lowerlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
    }
}

static void  higherlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
    }
}


/**********************
 *    ERROR ASSERT
 **********************/
