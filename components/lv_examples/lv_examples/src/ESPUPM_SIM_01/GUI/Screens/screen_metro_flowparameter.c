/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-25
 */

/**
*  @file screen_metro_flowparameter.c
*  @brief This Screen is for flow calibration
*  @details 
*/


/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"

/*********************
 *      DEFINES
 *********************/

//Declare Images Here
LV_IMG_DECLARE(left_arrow_icon)
LV_IMG_DECLARE(fan_icon)
LV_IMG_DECLARE(ok_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void _fpsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void _fpsEraseBTN_event_handler(lv_obj_t * obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t * scrFlowPara;
lv_obj_t * fpsParentCont;
lv_obj_t * _fpsContStatusBar;
lv_obj_t * __fpsTimeLabel;
lv_obj_t * __fpsBatteryLabel;
lv_obj_t * __fpsWifiLabel;
lv_obj_t * _fpsParaLblCont;
lv_obj_t * _fpsHeadingCont_par;
lv_obj_t * _fpsBackArrowLabel_par;
lv_obj_t * _fpsParaHeading_par;
lv_obj_t * _fasParaBlackCont;
lv_obj_t * __fasDateTxtLbl;
lv_obj_t * __fasDateValLbl;
lv_obj_t * __fasHrMinValLbl;
lv_obj_t * __fasPointNumberTxt;
lv_obj_t * __fasPointNumberVal;
lv_obj_t * __fasGlobalAccuTxt;
lv_obj_t * __fasGlobalAccuVal;
lv_obj_t * __fasHourCounterTxt;
lv_obj_t * __fasHourCounterVal;
lv_obj_t * __fasVoilumeCounterTxt;
lv_obj_t * __fasValueCounterVal;
lv_obj_t * __fasCurveTxt;
lv_obj_t * __fasCurveTxtVal;
lv_obj_t * __fasUnitTxt;
lv_obj_t * __fasUnitVal;
lv_obj_t * __fasLowerLimitTxt;
lv_obj_t * __fasLowerLimitVal;
lv_obj_t * __fasHigherLimitTxt;
lv_obj_t * __fasHigherLimitVal;
lv_obj_t * __fasLimitAlertTxt;
lv_obj_t * __fasLimitAlertVal;
lv_obj_t * __fasAfterLtAlertTxt;
lv_obj_t * __fasAfterLtAlertVal;
lv_obj_t * _fasPointHeadlbl_pt1;
lv_obj_t * _fasFactoryValTxt_pt1;
lv_obj_t * _fasFactoryValValue_pt1;
lv_obj_t * _fasCompValTxt_pt1;
lv_obj_t * _fasCompValValValue_pt1;
lv_obj_t * _fasAccuracyTxt_pt1;
lv_obj_t * _fasAccuracyValue_pt1;
lv_obj_t * _fasOkLogo_pt1;
lv_obj_t * _fasPointCont1;
lv_obj_t * _fasPointCont2;
lv_obj_t * _fasPointHeadlbl_pt2;
lv_obj_t * _fasFactoryValTxt_pt2;
lv_obj_t * _fasFactoryValValue_pt2;
lv_obj_t * _fasCompValTxt_pt2;
lv_obj_t * _fasCompValValValue_pt2;
lv_obj_t * _fasAccuracyTxt_pt2;
lv_obj_t * _fasAccuracyValue_pt2;
lv_obj_t * _fasOkLogo_pt2;
lv_obj_t * _fasPointCont3;
lv_obj_t * _fasPointHeadlbl_pt3;
lv_obj_t * _fasFactoryValTxt_pt3;
lv_obj_t * _fasFactoryValValue_pt3;
lv_obj_t * _fasCompValTxt_pt3;
lv_obj_t * _fasCompValValValue_pt3;
lv_obj_t * _fasAccuracyTxt_pt3;
lv_obj_t * _fasAccuracyValue_pt3;
lv_obj_t * _fasOkLogo_pt3;
lv_obj_t * _fasPointCont4;
lv_obj_t * _fasPointHeadlbl_pt4;
lv_obj_t * _fasFactoryValTxt_pt4;
lv_obj_t * _fasFactoryValValue_pt4;
lv_obj_t * _fasCompValTxt_pt4;
lv_obj_t * _fasCompValValValue_pt4;
lv_obj_t * _fasAccuracyTxt_pt4;
lv_obj_t * _fasAccuracyValue_pt4;
lv_obj_t * _fasOkLogo_pt4;
lv_obj_t * _fasGraphCont;
lv_obj_t * _fasGraphTitleLbl;
lv_obj_t * chart;
lv_obj_t * _fasGraphEquation;
lv_obj_t * _fasGraphReg;
lv_obj_t * _fasGraphMinX;
lv_obj_t * _fasGraphMaxX;
lv_obj_t * _fasEraseBtn;
lv_obj_t * _fasEraseBtnLabel;


/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void callFlowParameterScreen(void)
{
    
    
    scrFlowPara = lv_cont_create(NULL, NULL);
    lv_scr_load(scrFlowPara);
    lv_obj_del(crnt_screen);
    fpsParentCont = lv_obj_create(scrFlowPara, NULL);
    lv_obj_set_size(fpsParentCont, 320, 480);
    lv_obj_align(fpsParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( fpsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(fpsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(fpsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    
    _fpsContStatusBar = lv_cont_create(fpsParentCont, NULL);
    lv_obj_set_size(_fpsContStatusBar, 320, 35);
    lv_obj_align(_fpsContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_fpsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fpsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    
    __fpsTimeLabel = lv_label_create(_fpsContStatusBar, NULL);
    lv_obj_align(__fpsTimeLabel, _fpsContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__fpsTimeLabel, guiTime);

    static lv_style_t _fpsTimeLabelStyle;
    lv_style_init(&_fpsTimeLabelStyle);
    lv_style_set_text_font(&_fpsTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_fpsTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fpsTimeLabel, LV_LABEL_PART_MAIN, &_fpsTimeLabelStyle);

    //Create Label for Battery icon
    
    __fpsBatteryLabel = lv_label_create(_fpsContStatusBar, NULL);
    lv_obj_align(__fpsBatteryLabel, _fpsContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__fpsBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t _fpsBatteryLabelStyle;
    lv_style_init(&_fpsBatteryLabelStyle);
    lv_style_set_text_font(&_fpsBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&_fpsBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fpsBatteryLabel, LV_LABEL_PART_MAIN, &_fpsBatteryLabelStyle);

    //Create Label for Wifi icon
    
    __fpsWifiLabel = lv_label_create(_fpsContStatusBar, NULL);
    lv_obj_align(__fpsWifiLabel, __fpsBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__fpsWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __fpsWifiLabelStyle;
    lv_style_init(&__fpsWifiLabelStyle);
    lv_style_set_text_font(&__fpsWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__fpsWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fpsWifiLabel, LV_LABEL_PART_MAIN, &__fpsWifiLabelStyle);

    //+++++++++++++++++++++++++++++++++++++++++++++++
    //            Create Scroll Page                +++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++

    // Create a container to put all the parameters
    
    _fpsParaLblCont = lv_page_create(fpsParentCont, NULL);
    lv_obj_set_size(_fpsParaLblCont, 320, 430);
    lv_obj_align(_fpsParaLblCont, _fpsContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);
    lv_page_set_scrollbar_mode(_fpsParaLblCont, LV_SCROLLBAR_MODE_HIDE);
    lv_obj_set_style_local_bg_color(_fpsParaLblCont, LV_PAGE_PART_BG, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_width(_fpsParaLblCont, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0 );

    //+++++++++++++++++++++++++++++++++++++++++++++++


    //==============================================================================================================
    // Create a container to contain Parameter Header 

    
    _fpsHeadingCont_par = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fpsHeadingCont_par, 300, 70);
    lv_obj_set_width(_fpsHeadingCont_par, lv_page_get_width_fit(_fpsParaLblCont));
    lv_obj_align(_fpsHeadingCont_par, _fpsParaLblCont, LV_ALIGN_IN_TOP_MID, 0,2);
    lv_obj_set_style_local_bg_color(_fpsHeadingCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fpsHeadingCont_par, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    
    _fpsBackArrowLabel_par = lv_img_create(_fpsHeadingCont_par, NULL);
    lv_img_set_src(_fpsBackArrowLabel_par, &left_arrow_icon);
    lv_obj_align(_fpsBackArrowLabel_par, _fpsHeadingCont_par, LV_ALIGN_IN_LEFT_MID, 5 , 0);
    lv_obj_set_click(_fpsBackArrowLabel_par, true);
    lv_obj_set_style_local_image_recolor_opa(_fpsBackArrowLabel_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_fpsBackArrowLabel_par, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_fpsBackArrowLabel_par, _fpsBackArrow_event_handler);

    //Create Label for Parameter "Heading"
    
    _fpsParaHeading_par = lv_label_create(_fpsHeadingCont_par, NULL);
    lv_obj_align(_fpsParaHeading_par, _fpsHeadingCont_par, LV_ALIGN_IN_BOTTOM_MID, -70, -35);
    lv_label_set_text(_fpsParaHeading_par, "     Parameter    \nSensor Parameter");

    static lv_style_t _fpsParaHeadingStyle_par;
    lv_style_init(&_fpsParaHeadingStyle_par);
    lv_style_set_text_font(&_fpsParaHeadingStyle_par, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); //signal_20
    lv_style_set_text_color(&_fpsParaHeadingStyle_par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fpsParaHeading_par, LV_LABEL_PART_MAIN, &_fpsParaHeadingStyle_par);

    // //Create FAN Logo
    // lv_obj_t * _fasFlowLogo;
    // _fasFlowLogo = lv_img_create(fpsParentCont, NULL);
    // lv_img_set_src(_fasFlowLogo, &fan_icon);
    // lv_obj_align(_fasFlowLogo, fpsParentCont, LV_ALIGN_IN_TOP_RIGHT, -20 , 50);
    // lv_img_set_auto_size(_fasFlowLogo, true);

    //=====================================
    //                                      ============================================>
    //=====================================

    // Create Parameter Black Container
    
    _fasParaBlackCont = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fasParaBlackCont, 290, 350);
    lv_obj_set_click(_fasParaBlackCont, false);
    lv_obj_align(_fasParaBlackCont, _fpsHeadingCont_par, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_fasParaBlackCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasParaBlackCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );


    // Create label for Date text inside big black container
    
    __fasDateTxtLbl = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasDateTxtLbl, _fasParaBlackCont, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text(__fasDateTxtLbl, "DATE");
    lv_obj_set_style_local_text_font(__fasDateTxtLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(__fasDateTxtLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Date Value inside big black container
    
    __fasDateValLbl = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasDateValLbl, __fasDateTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_label_set_text(__fasDateValLbl, "26/03/2021");
    lv_obj_set_style_local_text_font(__fasDateValLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_obj_set_style_local_text_color(__fasDateValLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Hour Min Val inside big black container
    
    __fasHrMinValLbl = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasHrMinValLbl, __fasDateValLbl, LV_ALIGN_OUT_BOTTOM_RIGHT, -5, 2);
    lv_label_set_text(__fasHrMinValLbl, "15H35");
    lv_obj_set_style_local_text_font(__fasHrMinValLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_obj_set_style_local_text_color(__fasHrMinValLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

        //=====================================================================
        //  Blue Text Style (Name of all the parameter in blue text)
        //=====================================================================
    static lv_style_t _fpsBlueTxtStyle_Par;
    lv_style_init(&_fpsBlueTxtStyle_Par);
    lv_style_set_text_font(&_fpsBlueTxtStyle_Par, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); //signal_20
    lv_style_set_text_color(&_fpsBlueTxtStyle_Par, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35,0x9F,0xE2));
        //======================================================================

        //=====================================================================
        //  White Text Style (Value of blue text heading inside big black )
        //=====================================================================
    static lv_style_t _fpsWhiteTxtStyle_Par;
    lv_style_init(&_fpsWhiteTxtStyle_Par);
    lv_style_set_text_font(&_fpsWhiteTxtStyle_Par, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); //signal_20
    lv_style_set_text_color(&_fpsWhiteTxtStyle_Par, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        //=====================================================================
    
    // create label for Point Number Text
    
    __fasPointNumberTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasPointNumberTxt, __fasDateValLbl, LV_ALIGN_IN_BOTTOM_LEFT, 0, 50);
    lv_label_set_text(__fasPointNumberTxt, "POINT NUMBER:");
    lv_obj_add_style(__fasPointNumberTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for Point number Value
    
    __fasPointNumberVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasPointNumberVal, __fasPointNumberTxt, LV_ALIGN_OUT_RIGHT_TOP, 100, 0);
    lv_label_set_text(__fasPointNumberVal, "02");
    lv_obj_add_style(__fasPointNumberVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for Global Accuracy Text
    
    __fasGlobalAccuTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasGlobalAccuTxt, __fasPointNumberTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasGlobalAccuTxt, "GLOBAL ACUURACY:");
    lv_obj_add_style(__fasGlobalAccuTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for Global Accuracy Value
    
    __fasGlobalAccuVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasGlobalAccuVal, __fasGlobalAccuTxt, LV_ALIGN_OUT_RIGHT_TOP, 75, 0);
    lv_label_set_text( __fasGlobalAccuVal, "2,09%");
    lv_obj_add_style( __fasGlobalAccuVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for Hour Counter Text
    
    __fasHourCounterTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasHourCounterTxt, __fasGlobalAccuTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasHourCounterTxt, "HOUR COUNTER:");
    lv_obj_add_style(__fasHourCounterTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for Hour Counter Value
    
    __fasHourCounterVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasHourCounterVal, __fasHourCounterTxt, LV_ALIGN_OUT_RIGHT_TOP, 95, 0);
    lv_label_set_text( __fasHourCounterVal, "567,79H");
    lv_obj_add_style( __fasHourCounterVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for Volume Counter Text
    
    __fasVoilumeCounterTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasVoilumeCounterTxt, __fasHourCounterTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasVoilumeCounterTxt, "VOLUME COUNTER:");
    lv_obj_add_style(__fasVoilumeCounterTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for Volume Counter Value
    
    __fasValueCounterVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasValueCounterVal, __fasVoilumeCounterTxt, LV_ALIGN_OUT_RIGHT_TOP, 80, 0);
    lv_label_set_text(__fasValueCounterVal, "567,79H");
    lv_obj_add_style(__fasValueCounterVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for CURVE Text
    
    __fasCurveTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasCurveTxt, __fasVoilumeCounterTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasCurveTxt, "CURVE:");
    lv_obj_add_style(__fasCurveTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for Volume Counter Value
    
    __fasCurveTxtVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasCurveTxtVal, __fasCurveTxt, LV_ALIGN_OUT_RIGHT_TOP, 155, 0);
    lv_label_set_text(__fasCurveTxtVal, "LINEAR 3");
    lv_obj_add_style(__fasCurveTxtVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for UNIT Text
    
    __fasUnitTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasUnitTxt, __fasCurveTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasUnitTxt, "UNIT:");
    lv_obj_add_style(__fasUnitTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for  UNIT Value
    
    __fasUnitVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_set_size(__fasUnitVal, 300, 15);
    lv_obj_align( __fasUnitVal, __fasUnitTxt, LV_ALIGN_OUT_RIGHT_TOP, 168, 0);
    lv_label_set_text(__fasUnitVal, "JACK");
    lv_obj_add_style(__fasUnitVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for LOWER LIMIT Text
    
    __fasLowerLimitTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasLowerLimitTxt, __fasUnitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasLowerLimitTxt, "LOWER LIMIT:");
    lv_obj_add_style(__fasLowerLimitTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for  UNIT Value
    
    __fasLowerLimitVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasLowerLimitVal, __fasLowerLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 116, 0);
    lv_label_set_text(__fasLowerLimitVal, "NONE");
    lv_obj_add_style(__fasLowerLimitVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for HIGHER LIMIT Text
    
    __fasHigherLimitTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align(__fasHigherLimitTxt , __fasLowerLimitTxt,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__fasHigherLimitTxt, "HIGHER LIMIT:");
    lv_obj_add_style(__fasHigherLimitTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for  HIGHER LIMIT Value
    
    __fasHigherLimitVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasHigherLimitVal, __fasHigherLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 114, 0);
    lv_label_set_text(__fasHigherLimitVal, "NONE");
    lv_obj_add_style(__fasHigherLimitVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for LIMIT ALERT Text
    
    __fasLimitAlertTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasLimitAlertTxt , __fasHigherLimitTxt,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text( __fasLimitAlertTxt, "LIMIT ALERT:");
    lv_obj_add_style( __fasLimitAlertTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for  LIMIT ALERT Value
    
    __fasLimitAlertVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasLimitAlertVal, __fasLimitAlertTxt, LV_ALIGN_OUT_RIGHT_TOP, 122, 0);
    lv_label_set_text( __fasLimitAlertVal, "-10% +10%");
    lv_obj_add_style( __fasLimitAlertVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create Label for AFTER LIMIT ALERT Text
    
    __fasAfterLtAlertTxt = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasAfterLtAlertTxt , __fasLimitAlertTxt,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text( __fasAfterLtAlertTxt, "AFTER LIMIT ALERT:");
    lv_obj_add_style( __fasAfterLtAlertTxt, LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    //Create Label for AFTER  LIMIT ALERT Value
    
    __fasAfterLtAlertVal = lv_label_create(_fasParaBlackCont, NULL);
    lv_obj_align( __fasAfterLtAlertVal, __fasAfterLtAlertTxt, LV_ALIGN_OUT_RIGHT_TOP, 78, 0);
    lv_label_set_text( __fasAfterLtAlertVal, "ENABLE");
    lv_obj_add_style( __fasAfterLtAlertVal, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    //==========================================================================================
                                          
    

    //==========================================================================================
    //==========================================================================================
    //======================Point 1 Pallet Box Start============================================
    //==========================================================================================
    //==========================================================================================

    // Create Point1 Container
    
    _fasPointCont1 = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fasPointCont1, 290, 120);
    lv_obj_set_click(_fasPointCont1, false);
    lv_obj_align(_fasPointCont1, _fasParaBlackCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_fasPointCont1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasPointCont1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    
    _fasPointHeadlbl_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasPointHeadlbl_pt1, _fasPointCont1, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text( _fasPointHeadlbl_pt1, "POINT 1");
    lv_obj_add_style( _fasPointHeadlbl_pt1, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasFactoryValTxt_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasFactoryValTxt_pt1 , _fasPointHeadlbl_pt1 , LV_ALIGN_OUT_BOTTOM_LEFT, 5, 10);
    lv_label_set_text( _fasFactoryValTxt_pt1 , "FACTORY VALUE:");
    lv_obj_add_style( _fasFactoryValTxt_pt1 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasFactoryValValue_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasFactoryValValue_pt1 , _fasFactoryValTxt_pt1 , LV_ALIGN_OUT_RIGHT_TOP, 100, 0);
    lv_label_set_text( _fasFactoryValValue_pt1 , "23,36");
    lv_obj_add_style( _fasFactoryValValue_pt1 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasCompValTxt_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasCompValTxt_pt1 , _fasFactoryValTxt_pt1 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasCompValTxt_pt1 , "COMPENSATED VALUE:");
    lv_obj_add_style( _fasCompValTxt_pt1 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasCompValValValue_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasCompValValValue_pt1 , _fasCompValTxt_pt1 , LV_ALIGN_OUT_RIGHT_TOP, 62, 0);
    lv_label_set_text( _fasCompValValValue_pt1 , "23,36");
    lv_obj_add_style( _fasCompValValValue_pt1 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasAccuracyTxt_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasAccuracyTxt_pt1 , _fasCompValTxt_pt1 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasAccuracyTxt_pt1 , "ACCURACY:");
    lv_obj_add_style( _fasAccuracyTxt_pt1 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasAccuracyValue_pt1 = lv_label_create(_fasPointCont1, NULL);
    lv_obj_align( _fasAccuracyValue_pt1 , _fasAccuracyTxt_pt1 , LV_ALIGN_OUT_RIGHT_TOP, 133, 0);
    lv_label_set_text( _fasAccuracyValue_pt1 , "10,05");
    lv_obj_add_style( _fasAccuracyValue_pt1 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create OK logo
    
    _fasOkLogo_pt1 = lv_img_create(_fasPointCont1, NULL);
    lv_img_set_src(_fasOkLogo_pt1, &ok_icon);
    lv_obj_align(_fasOkLogo_pt1,_fasPointCont1, LV_ALIGN_IN_TOP_RIGHT, -5 , 5);
    lv_img_set_auto_size(_fasOkLogo_pt1, true);



    //==========================================================================================
    //======================Point 1 Pallet Box End==============================================
    //==========================================================================================



    //==========================================================================================
    //==========================================================================================
    //======================Point 2 Pallet Box Start============================================
    //==========================================================================================
    //==========================================================================================

    // Create Point2 Container
    
    _fasPointCont2 = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fasPointCont2, 290, 120);
    lv_obj_set_click(_fasPointCont2, false);
    lv_obj_align(_fasPointCont2, _fasPointCont1, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_fasPointCont2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasPointCont2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    
    _fasPointHeadlbl_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasPointHeadlbl_pt2, _fasPointCont2, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text( _fasPointHeadlbl_pt2, "POINT 2");
    lv_obj_add_style( _fasPointHeadlbl_pt2, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasFactoryValTxt_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasFactoryValTxt_pt2 , _fasPointHeadlbl_pt2 , LV_ALIGN_OUT_BOTTOM_LEFT, 5, 10);
    lv_label_set_text( _fasFactoryValTxt_pt2 , "FACTORY VALUE:");
    lv_obj_add_style( _fasFactoryValTxt_pt2 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasFactoryValValue_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasFactoryValValue_pt2 , _fasFactoryValTxt_pt2 , LV_ALIGN_OUT_RIGHT_TOP, 100, 0);
    lv_label_set_text( _fasFactoryValValue_pt2 , "23,36");
    lv_obj_add_style( _fasFactoryValValue_pt2 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasCompValTxt_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasCompValTxt_pt2 , _fasFactoryValTxt_pt2 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasCompValTxt_pt2 , "COMPENSATED VALUE:");
    lv_obj_add_style( _fasCompValTxt_pt2 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasCompValValValue_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasCompValValValue_pt2 , _fasCompValTxt_pt2 , LV_ALIGN_OUT_RIGHT_TOP, 62, 0);
    lv_label_set_text( _fasCompValValValue_pt2 , "23,36");
    lv_obj_add_style( _fasCompValValValue_pt2 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasAccuracyTxt_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasAccuracyTxt_pt2 , _fasCompValTxt_pt2 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasAccuracyTxt_pt2 , "ACCURACY:");
    lv_obj_add_style( _fasAccuracyTxt_pt2 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasAccuracyValue_pt2 = lv_label_create(_fasPointCont2, NULL);
    lv_obj_align( _fasAccuracyValue_pt2 , _fasAccuracyTxt_pt2 , LV_ALIGN_OUT_RIGHT_TOP, 133, 0);
    lv_label_set_text( _fasAccuracyValue_pt2 , "10,05");
    lv_obj_add_style( _fasAccuracyValue_pt2 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create OK logo
    
    _fasOkLogo_pt2 = lv_img_create(_fasPointCont2, NULL);
    lv_img_set_src(_fasOkLogo_pt2, &ok_icon);
    lv_obj_align(_fasOkLogo_pt2,_fasPointCont2, LV_ALIGN_IN_TOP_RIGHT, -5 , 5);
    lv_img_set_auto_size(_fasOkLogo_pt2, true);

    //==========================================================================================
    //======================Point 2 Pallet Box End==============================================
    //==========================================================================================



    //==========================================================================================
    //==========================================================================================
    //======================Point 3 Pallet Box Start============================================
    //==========================================================================================
    //==========================================================================================

    // Create Point3 Container
    
    _fasPointCont3 = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fasPointCont3, 290, 120);
    lv_obj_set_click(_fasPointCont3, false);
    lv_obj_align(_fasPointCont3, _fasPointCont2, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_fasPointCont3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasPointCont3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    
    _fasPointHeadlbl_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasPointHeadlbl_pt3, _fasPointCont3, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text( _fasPointHeadlbl_pt3, "POINT 3");
    lv_obj_add_style( _fasPointHeadlbl_pt3, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasFactoryValTxt_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasFactoryValTxt_pt3 , _fasPointHeadlbl_pt3 , LV_ALIGN_OUT_BOTTOM_LEFT, 5, 10);
    lv_label_set_text( _fasFactoryValTxt_pt3 , "FACTORY VALUE:");
    lv_obj_add_style( _fasFactoryValTxt_pt3 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasFactoryValValue_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasFactoryValValue_pt3 , _fasFactoryValTxt_pt3 , LV_ALIGN_OUT_RIGHT_TOP, 100, 0);
    lv_label_set_text( _fasFactoryValValue_pt3 , "23,36");
    lv_obj_add_style( _fasFactoryValValue_pt3 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasCompValTxt_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasCompValTxt_pt3 , _fasFactoryValTxt_pt3 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasCompValTxt_pt3 , "COMPENSATED VALUE:");
    lv_obj_add_style( _fasCompValTxt_pt3 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasCompValValValue_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasCompValValValue_pt3 , _fasCompValTxt_pt3 , LV_ALIGN_OUT_RIGHT_TOP, 62, 0);
    lv_label_set_text( _fasCompValValValue_pt3 , "23,36");
    lv_obj_add_style( _fasCompValValValue_pt3 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasAccuracyTxt_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasAccuracyTxt_pt3 , _fasCompValTxt_pt3 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasAccuracyTxt_pt3 , "ACCURACY:");
    lv_obj_add_style( _fasAccuracyTxt_pt3 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasAccuracyValue_pt3 = lv_label_create(_fasPointCont3, NULL);
    lv_obj_align( _fasAccuracyValue_pt3 , _fasAccuracyTxt_pt3 , LV_ALIGN_OUT_RIGHT_TOP, 133, 0);
    lv_label_set_text( _fasAccuracyValue_pt3 , "10,05");
    lv_obj_add_style( _fasAccuracyValue_pt3 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create OK logo
    
    _fasOkLogo_pt3 = lv_img_create(_fasPointCont3, NULL);
    lv_img_set_src(_fasOkLogo_pt3, &ok_icon);
    lv_obj_align(_fasOkLogo_pt3,_fasPointCont3, LV_ALIGN_IN_TOP_RIGHT, -5 , 5);
    lv_img_set_auto_size(_fasOkLogo_pt3, true);


    //==========================================================================================
    //======================Point 3 Pallet Box End==============================================
    //==========================================================================================



    //==========================================================================================
    //==========================================================================================
    //======================Point 4 Pallet Box Start============================================
    //==========================================================================================
    //==========================================================================================

    // Create Point3 Container
    
    _fasPointCont4 = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fasPointCont4, 290, 120);
    lv_obj_set_click(_fasPointCont4, false);
    lv_obj_align(_fasPointCont4, _fasPointCont3, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_fasPointCont4, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasPointCont4, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    
    _fasPointHeadlbl_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasPointHeadlbl_pt4, _fasPointCont4, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text( _fasPointHeadlbl_pt4, "POINT 4");
    lv_obj_add_style( _fasPointHeadlbl_pt4, LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasFactoryValTxt_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasFactoryValTxt_pt4 , _fasPointHeadlbl_pt4 , LV_ALIGN_OUT_BOTTOM_LEFT, 5, 10);
    lv_label_set_text( _fasFactoryValTxt_pt4 , "FACTORY VALUE:");
    lv_obj_add_style( _fasFactoryValTxt_pt4 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasFactoryValValue_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasFactoryValValue_pt4 , _fasFactoryValTxt_pt4 , LV_ALIGN_OUT_RIGHT_TOP, 100, 0);
    lv_label_set_text( _fasFactoryValValue_pt4 , "23,36");
    lv_obj_add_style( _fasFactoryValValue_pt4 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasCompValTxt_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasCompValTxt_pt4 , _fasFactoryValTxt_pt4 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasCompValTxt_pt4 , "COMPENSATED VALUE:");
    lv_obj_add_style( _fasCompValTxt_pt4 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasCompValValValue_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasCompValValValue_pt4 , _fasCompValTxt_pt4 , LV_ALIGN_OUT_RIGHT_TOP, 62, 0);
    lv_label_set_text( _fasCompValValValue_pt4 , "23,36");
    lv_obj_add_style( _fasCompValValValue_pt4 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    
    _fasAccuracyTxt_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasAccuracyTxt_pt4 , _fasCompValTxt_pt4 , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_label_set_text( _fasAccuracyTxt_pt4 , "ACCURACY:");
    lv_obj_add_style( _fasAccuracyTxt_pt4 , LV_LABEL_PART_MAIN, &_fpsBlueTxtStyle_Par);

    
    _fasAccuracyValue_pt4 = lv_label_create(_fasPointCont4, NULL);
    lv_obj_align( _fasAccuracyValue_pt4 , _fasAccuracyTxt_pt4 , LV_ALIGN_OUT_RIGHT_TOP, 133, 0);
    lv_label_set_text( _fasAccuracyValue_pt4 , "10,05");
    lv_obj_add_style( _fasAccuracyValue_pt4 , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //Create OK logo
    
    _fasOkLogo_pt4 = lv_img_create(_fasPointCont4, NULL);
    lv_img_set_src(_fasOkLogo_pt4, &ok_icon);
    lv_obj_align(_fasOkLogo_pt4,_fasPointCont4, LV_ALIGN_IN_TOP_RIGHT, -5 , 5);
    lv_img_set_auto_size(_fasOkLogo_pt4, true);


    //==========================================================================================
    //======================Point 3 Pallet Box End==============================================
    //==========================================================================================

    //==========================================================================================
    //==========================================================================================
    //======================Start Container for Graph Plot======================================
    //==========================================================================================
    //==========================================================================================

    // Create Graph Container
    
    _fasGraphCont = lv_cont_create(_fpsParaLblCont, NULL);
    lv_obj_set_size(_fasGraphCont, 290, 250);
    lv_obj_set_click(_fasGraphCont, false);
    lv_obj_align(_fasGraphCont, _fasPointCont4, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_fasGraphCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasGraphCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Label for Graph Title
    
    _fasGraphTitleLbl = lv_label_create(_fasGraphCont, NULL);
    lv_obj_align( _fasGraphTitleLbl , _fasGraphCont , LV_ALIGN_IN_TOP_MID, -60, 0);
    lv_label_set_text(_fasGraphTitleLbl , "Calibrated VS Factory Value");
    lv_obj_add_style( _fasGraphTitleLbl , LV_LABEL_PART_MAIN, &_fpsWhiteTxtStyle_Par);

    //=========
    //=========
    //=========
    /*Create a chart*/
    
    chart = lv_chart_create(_fasGraphCont, NULL);
    lv_obj_set_size(chart, 290, 230);
    lv_obj_set_click(chart, false);
    lv_chart_set_range(chart, 10, 50);
    lv_obj_align(chart, _fasGraphCont, LV_ALIGN_IN_TOP_MID, 0, 20);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_obj_set_style_local_bg_color(chart, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(chart, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );
    lv_chart_set_point_count(chart, 4);


    /*Add a faded are effect*/
    lv_obj_set_style_local_bg_opa(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50); /*Max. opa.*/
    lv_obj_set_style_local_bg_grad_dir(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255);    /*Max opa on the top*/
    lv_obj_set_style_local_bg_grad_stop(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);      /*Transparent on the bottom*/

    

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);

    /*Set the next points on 'ser1'*/
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 20);
    lv_chart_set_next(chart, ser1, 30);
    lv_chart_set_next(chart, ser1, 40);
    // lv_chart_set_next(chart, ser1, 50);
    // lv_chart_set_next(chart, ser1, 60);
    // lv_chart_set_next(chart, ser1, 70);
    // lv_chart_set_next(chart, ser1, 80);
    // lv_chart_set_next(chart, ser1, 90);
    // lv_chart_set_next(chart, ser1, 90);

    /*Directly set points on 'ser2'*/
    ser2->points[0] = 15;
    ser2->points[1] = 25;
    ser2->points[2] = 35;
    ser2->points[3] = 45;
    // ser2->points[4] = 55;
    // ser2->points[5] = 65;
    // ser2->points[6] = 75;
    // ser2->points[7] = 85;
    // ser2->points[8] = 95;
    // ser2->points[9] = 95;

    lv_chart_refresh(chart); /*Required after direct set*/


    //Create Label for Graph Equation
    
    _fasGraphEquation = lv_label_create(chart, NULL);
    lv_obj_align( _fasGraphEquation , chart , LV_ALIGN_IN_TOP_LEFT, 10, 10);
    lv_label_set_text(_fasGraphEquation , "Y=0,00542x");
    lv_obj_set_style_local_text_font(_fasGraphEquation, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_set_style_local_text_color(_fasGraphEquation, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //Create Label for Graph Regression
    
    _fasGraphReg = lv_label_create(chart, NULL);
    lv_obj_align( _fasGraphReg , _fasGraphEquation , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_label_set_text( _fasGraphReg , "R=0,00002");
    lv_obj_set_style_local_text_font( _fasGraphReg , LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_set_style_local_text_color( _fasGraphReg , LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //Create Label for Min X Value
    
    _fasGraphMinX = lv_label_create(_fpsParaLblCont, NULL);
    lv_obj_align( _fasGraphMinX , chart , LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text( _fasGraphMinX , "10");
    lv_obj_set_style_local_text_font( _fasGraphMinX , LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_obj_set_style_local_text_color( _fasGraphMinX , LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //Create Label for Max X Value
    
    _fasGraphMaxX = lv_label_create(_fpsParaLblCont, NULL);
    lv_obj_align( _fasGraphMaxX , chart , LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
    lv_label_set_text( _fasGraphMaxX , "50");
    lv_obj_set_style_local_text_font( _fasGraphMaxX , LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_obj_set_style_local_text_color( _fasGraphMaxX , LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
 
    //========
    //========
    //========


    
    //==========================================================================================
    //======================END Container for Graph Plot======================================
    //==========================================================================================


    //==========================================================================================
    //==========================================================================================
    //======================Start "ERASE & START NEW" BUTTON====================================
    //==========================================================================================
    //==========================================================================================

    //Creat a  Button
    
    _fasEraseBtn = lv_btn_create(_fpsParaLblCont, NULL);
    lv_obj_align(_fasEraseBtn, _fasGraphCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20 );
    lv_obj_set_size(_fasEraseBtn, 290, 44);
    lv_obj_set_event_cb(_fasEraseBtn, _fpsEraseBTN_event_handler);
    lv_obj_set_style_local_radius(_fasEraseBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(_fasEraseBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEB, 0x3B, 0x5A)); //#Eb3B5A
    //lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);
  
    //Creat a stop Button Label
    
    _fasEraseBtnLabel = lv_label_create(_fasEraseBtn, NULL);
    lv_obj_align(_fasEraseBtnLabel, _fasEraseBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_fasEraseBtnLabel, "ERASE & START NEW");
    lv_obj_set_style_local_text_font(_fasEraseBtnLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_text_color(_fasEraseBtnLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //==========================================================================================
    //======================END "ERASE & START NEW" BUTTON======================================
    //==========================================================================================

    crnt_screen = scrFlowPara; //scrFlowPara

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

//_xBackArrow_event_handler
static void _fpsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Clicked\n");
        CallMetroMenuScreen();
    }
    
}

static void _fpsEraseBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Clicked\n");
        callMetroFlowSettingScreen();
    }
    
}

/**********************
 *    ERROR ASSERT
 ********************/