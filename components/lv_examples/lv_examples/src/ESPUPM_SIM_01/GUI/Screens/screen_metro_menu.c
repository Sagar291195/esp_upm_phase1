/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-05
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
LV_IMG_DECLARE(metrology_icon)	

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void  __smmBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void  new_cal_event_handler(lv_obj_t * obj, lv_event_t event);
static void  flow_cal_event_handler(lv_obj_t * obj, lv_event_t event);
static void  flow_adjust_event_handler(lv_obj_t * obj, lv_event_t event);
static void  __smmTriangeBtn_event_handler(lv_obj_t * obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/


lv_obj_t *crnt_screen;
lv_obj_t * scrMetroScreen;
lv_obj_t * smmPatrentCont;
lv_obj_t * _smmContStatusBar;
lv_obj_t * __smmTimeLabel;
lv_obj_t * __smmBatteryLabel;
lv_obj_t * __smmWifiLabel;
lv_obj_t * __smmSignalLabel;
lv_obj_t * _smmMetroHeadingCont;
lv_obj_t * __smmBackArrowLabel;
lv_obj_t * __smmMetroHeadingLbl;
lv_obj_t * _smmMetroLogo;
lv_obj_t * _smmSensorParBtn;
lv_obj_t * _smmSensorParBtnLbl;
lv_obj_t * _smmNewCalBtn;
lv_obj_t * _smmNewCalBtnLbl;
lv_obj_t * _smmNewVerTestBtn;
lv_obj_t * _smmNewVerTestBtnLbl;
lv_obj_t * _smmValidationTestCont;
lv_obj_t * _smmVTC;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void CallMetroMenuScreen(void)
{
    scrMetroScreen = lv_obj_create(NULL, NULL);
    lv_scr_load(scrMetroScreen);
    if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    //Create Base container
    smmPatrentCont = lv_cont_create(scrMetroScreen, NULL);
    lv_obj_set_size(smmPatrentCont, 320, 480);
    lv_obj_align(smmPatrentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( smmPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(smmPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(smmPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    _smmContStatusBar = lv_cont_create(smmPatrentCont, NULL);
    lv_obj_set_size(_smmContStatusBar, 320, 35);
    lv_obj_align(_smmContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_smmContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_smmContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    
    __smmTimeLabel = lv_label_create(_smmContStatusBar, NULL);
    lv_obj_align(__smmTimeLabel, _smmContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__smmTimeLabel, guiTime);

    static lv_style_t _smmTimeLabelStyle;
    lv_style_init(&_smmTimeLabelStyle);
    lv_style_set_text_font(&_smmTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_smmTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__smmTimeLabel, LV_LABEL_PART_MAIN, &_smmTimeLabelStyle);

    //Create Label for Battery icon
    __smmBatteryLabel = lv_label_create(_smmContStatusBar, NULL);
    lv_obj_align(__smmBatteryLabel, _smmContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__smmBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t _smmBatteryLabelStyle;
    lv_style_init(&_smmBatteryLabelStyle);
    lv_style_set_text_font(&_smmBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&_smmBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__smmBatteryLabel, LV_LABEL_PART_MAIN, &_smmBatteryLabelStyle);

    //Create Label for Wifi icon
    __smmWifiLabel = lv_label_create(_smmContStatusBar, NULL);
    lv_obj_align(__smmWifiLabel, __smmBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__smmWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __smmWifiLabelStyle;
    lv_style_init(&__smmWifiLabelStyle);
    lv_style_set_text_font(&__smmWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__smmWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__smmWifiLabel, LV_LABEL_PART_MAIN, &__smmWifiLabelStyle);
    
    //Create Label for Signal icon
    
    __smmSignalLabel = lv_label_create(_smmContStatusBar, NULL);
    lv_obj_align(__smmSignalLabel, __smmWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__smmSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __smmSignalLabelStyle;
    lv_style_init(&__smmSignalLabelStyle);
    lv_style_set_text_font(&__smmSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&__smmSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__smmSignalLabel, LV_LABEL_PART_MAIN, &__smmSignalLabelStyle);

    //===============================================================================================================

    //Crate a container to contain Summary Start Header
    _smmMetroHeadingCont = lv_cont_create(smmPatrentCont, NULL);
    lv_obj_set_size(_smmMetroHeadingCont, 300, 70);
    lv_obj_align(_smmMetroHeadingCont, _smmContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_set_style_local_bg_color(_smmMetroHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_smmMetroHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    //Black arrow Container

    lv_obj_t *_smmBlackArrowCont;
    _smmBlackArrowCont =  lv_cont_create(_smmMetroHeadingCont, NULL);
    lv_obj_set_size(_smmBlackArrowCont, 60, 60);
    lv_obj_align(_smmBlackArrowCont, _smmMetroHeadingCont, LV_ALIGN_IN_LEFT_MID, 5 , 0);
    lv_obj_set_style_local_bg_color(_smmBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_smmBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );
    lv_obj_set_event_cb(_smmBlackArrowCont, __smmBackArrow_event_handler);


    // Create Back arrow img
    
    __smmBackArrowLabel = lv_img_create(_smmBlackArrowCont, NULL);
    lv_img_set_src(__smmBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__smmBackArrowLabel, _smmBlackArrowCont, LV_ALIGN_IN_LEFT_MID, 0 , 0);
    lv_obj_set_click(__smmBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__smmBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__smmBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__smmBackArrowLabel, __smmBackArrow_event_handler);

    //Create Label for Sequences "Heading"
    __smmMetroHeadingLbl = lv_label_create(_smmMetroHeadingCont, NULL);
    lv_obj_align(__smmMetroHeadingLbl, _smmMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -60, -35);
    lv_label_set_text(__smmMetroHeadingLbl, "Metrology");

    static lv_style_t __smmMetroHeadingLblStyle;
    lv_style_init(&__smmMetroHeadingLblStyle);
    lv_style_set_text_font(&__smmMetroHeadingLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__smmMetroHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__smmMetroHeadingLbl, LV_LABEL_PART_MAIN, &__smmMetroHeadingLblStyle);

    //Create Metrology Logo
    
    _smmMetroLogo = lv_img_create(smmPatrentCont, NULL);
    lv_img_set_src(_smmMetroLogo, &metrology_icon);
    lv_obj_align(_smmMetroLogo, smmPatrentCont, LV_ALIGN_IN_TOP_RIGHT, -15 , 50);

    //=====================
    //=====================


    lv_obj_t * _smmSensorParBtn1;
    _smmSensorParBtn1 = lv_btn_create(smmPatrentCont, NULL);
    lv_obj_align(_smmSensorParBtn1, _smmMetroHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10 );
    lv_obj_set_size(_smmSensorParBtn1, 300, 44);
    lv_obj_set_event_cb(_smmSensorParBtn1, flow_cal_event_handler);
    //lv_obj_set_event_cb(_xsValidBtn, BTN_event_handler);
  

    static lv_style_t _smmSensorParBtnStyle1;
    lv_style_init(&_smmSensorParBtnStyle1);
    lv_style_set_radius(&_smmSensorParBtnStyle1, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&_smmSensorParBtnStyle1, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D,0x3D,0x3D)); //#359Fe2
    lv_style_set_border_width(&_smmSensorParBtnStyle1, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_smmSensorParBtnStyle1, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_smmSensorParBtn1, LV_BTN_PART_MAIN, &_smmSensorParBtnStyle1);

    lv_obj_t * _smmSensorParBtnLbl1;
    _smmSensorParBtnLbl1 = lv_label_create(_smmSensorParBtn1, NULL);
    lv_obj_align(_smmSensorParBtnLbl1, _smmSensorParBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_smmSensorParBtnLbl1, "CHANGE PASSWORD");

    static lv_style_t _smmSensorParBtnLblStyle1;
    lv_style_init(&_smmSensorParBtnLblStyle1);
    lv_style_set_text_font(&_smmSensorParBtnLblStyle1, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_text_color(&_smmSensorParBtnLblStyle1, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_smmSensorParBtnLbl1, LV_LABEL_PART_MAIN, &_smmSensorParBtnLblStyle1);



    //=====================
    //=====================


    // Create Sensor Parameter Button

    
    _smmSensorParBtn = lv_btn_create(smmPatrentCont, NULL);
    lv_obj_align(_smmSensorParBtn,  _smmSensorParBtn1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15 );// _smmSensorParBtn1 ,, _smmMetroHeadingCont
    lv_obj_set_size(_smmSensorParBtn, 300, 44);
    lv_obj_set_event_cb(_smmSensorParBtn, flow_cal_event_handler);
    //lv_obj_set_event_cb(_xsValidBtn, BTN_event_handler);
  

    static lv_style_t _smmSensorParBtnStyle;
    lv_style_init(&_smmSensorParBtnStyle);
    lv_style_set_radius(&_smmSensorParBtnStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&_smmSensorParBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D,0x3D,0x3D)); //#359Fe2
    lv_style_set_border_width(&_smmSensorParBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_smmSensorParBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_smmSensorParBtn, LV_BTN_PART_MAIN, &_smmSensorParBtnStyle);

    //Creat a Sensor Parameter Button Label

    
    _smmSensorParBtnLbl = lv_label_create(_smmSensorParBtn, NULL);
    lv_obj_align(_smmSensorParBtnLbl, _smmSensorParBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_smmSensorParBtnLbl, "SET SERVICE");

    static lv_style_t _smmSensorParBtnLblStyle;
    lv_style_init(&_smmSensorParBtnLblStyle);
    lv_style_set_text_font(&_smmSensorParBtnLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_text_color(&_smmSensorParBtnLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_smmSensorParBtnLbl, LV_LABEL_PART_MAIN, &_smmSensorParBtnLblStyle);

    // Create New Calibration Button
    
    _smmNewCalBtn = lv_btn_create(smmPatrentCont, NULL);
    lv_obj_align(_smmNewCalBtn, _smmSensorParBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15 );
    lv_obj_set_size(_smmNewCalBtn, 300, 44);
    lv_obj_set_event_cb(_smmNewCalBtn, new_cal_event_handler); // Flow Calibration 
  

    static lv_style_t  _smmNewCalBtnStyle;
    lv_style_init(&_smmNewCalBtnStyle);
    lv_style_set_radius(&_smmNewCalBtnStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&_smmNewCalBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D,0x3D,0x3D)); //#359Fe2
    lv_style_set_border_width(&_smmNewCalBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_smmNewCalBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_smmNewCalBtn, LV_BTN_PART_MAIN, &_smmNewCalBtnStyle);

    //Creat a Sensor Parameter Button Label
    
    _smmNewCalBtnLbl = lv_label_create(_smmNewCalBtn, NULL);
    lv_obj_align(_smmNewCalBtnLbl, _smmSensorParBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_smmNewCalBtnLbl, "CALIBRATION");

    static lv_style_t _smmNewCalBtnLbllStyle;
    lv_style_init(&_smmNewCalBtnLbllStyle);
    lv_style_set_text_font(&_smmNewCalBtnLbllStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_text_color(&_smmNewCalBtnLbllStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_smmNewCalBtnLbl, LV_LABEL_PART_MAIN, &_smmNewCalBtnLbllStyle);

    // Create New Verification Test Button
    _smmNewVerTestBtn = lv_btn_create(smmPatrentCont, NULL);
    lv_obj_align(_smmNewVerTestBtn, _smmNewCalBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15 );
    lv_obj_set_size(_smmNewVerTestBtn, 300, 44);
    lv_obj_set_event_cb(_smmNewVerTestBtn, flow_adjust_event_handler);
  

    static lv_style_t  _smmNewVerTestBtnStyle;
    lv_style_init(&_smmNewVerTestBtnStyle);
    lv_style_set_radius(&_smmNewVerTestBtnStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&_smmNewVerTestBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D,0x3D,0x3D)); //#359Fe2
    lv_style_set_border_width(&_smmNewVerTestBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_smmNewVerTestBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_smmNewVerTestBtn, LV_BTN_PART_MAIN, &_smmNewVerTestBtnStyle);

    //Creat a Sensor Parameter Button Label
    
    _smmNewVerTestBtnLbl = lv_label_create(_smmNewVerTestBtn, NULL);
    lv_obj_align(_smmNewVerTestBtnLbl, _smmNewVerTestBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_smmNewVerTestBtnLbl, "NEW VALIDATION TEST");
    
    static lv_style_t _smmNewVerTestBtnLbStyle;
    lv_style_init(&_smmNewVerTestBtnLbStyle);
    lv_style_set_text_font(&_smmNewVerTestBtnLbStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_text_color(&_smmNewVerTestBtnLbStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_smmNewVerTestBtnLbl, LV_LABEL_PART_MAIN, &_smmNewVerTestBtnLbStyle);

    //===================================>>>>>>
    //===================================>>>>>>

    _smmValidationTestCont = lv_cont_create(smmPatrentCont, NULL);
    lv_obj_align(_smmValidationTestCont, _smmNewVerTestBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20 );
    lv_obj_set_size(_smmValidationTestCont, 300, 100 );
    lv_obj_set_style_local_bg_color(_smmValidationTestCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); //_xColorBGParas
    lv_obj_set_style_local_border_width(_smmValidationTestCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _smmVTC = sqCreateSequence(_smmValidationTestCont,0);
    lv_obj_align(_smmVTC, NULL, LV_ALIGN_CENTER, 0,0 );
    lv_obj_set_style_local_bg_color(_smmVTC, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x38, 0x38, 0x38) ); //LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_opa(_smmVTC, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );
    //sqSquenceNum(Seq, 1);
    sqSetFlowSetPoint(_smmVTC, 12);
    sqSetDuration(_smmVTC, 5);
    sqSetProblem(_smmVTC, "NONE");
    sqSetBtnColor(_smmVTC, STATE_OK);
    sqSetHeading(_smmVTC, "VALIDATION TEST");
    lv_obj_set_event_cb(_smmVTC, __smmTriangeBtn_event_handler);

    
    crnt_screen = scrMetroScreen; //scrMetroScreen
    screenid = SCR_METROLOGY_MENU;
    //===================================
    //===================================


}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void  __smmBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        //printf("Back to Dashbord from presetscrn\n");
        pxDashboardScreen();
    }
}

static void  new_cal_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        callMetroTempSettingScreen();
    }
}

static void  flow_cal_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Go to password change screen\n");
        //xCallFlowCalibrationScreen();
        xMetroPswdScrn();
    }
}

static void  flow_adjust_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        //printf("Back to Dashbord from presetscrn\n");
        //xCallFlowAdjustScreen();
    }
}

static void  __smmTriangeBtn_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        //printf("Back to Dashbord from presetscrn\n");
        //callFlowParameterScreen();
    }
}
/**********************
 *    ERROR ASSERT
 **********************/
