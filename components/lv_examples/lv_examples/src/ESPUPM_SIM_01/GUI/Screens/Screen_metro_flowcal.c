/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-23
 */

/**
*  @file screen_metro_flowcal.c 
*  @brief This Screen is for flow calibration
*  @details This Screen is for flow calibration
*/


/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"

/*********************
 *      DEFINES
 *********************/

#define SYMBOL_SIGNAL "\uf012"
#define TAG "FLOW CALIBRATION"
//Declare Fonts here
LV_FONT_DECLARE(signal_20);

//Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)	;
LV_IMG_DECLARE(fan_icon);
LV_IMG_DECLARE(ok_icon);
LV_IMG_DECLARE(cross_icon);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void  __fasBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void  __fasValidBTN_event_handler(lv_obj_t * obj, lv_event_t event);

static void  __fasPlusBTN_event_handler( lv_obj_t * obj, lv_event_t event);
static void  __fasMinusBTN_event_handler(lv_obj_t * obj, lv_event_t event);

static void lv_spinbox_increment_event_cb(lv_obj_t * btn, lv_event_t e);
static void lv_spinbox_decrement_event_cb(lv_obj_t * btn, lv_event_t e);


/**********************
 *  STATIC VARIABLES
 **********************/
float flowPoints[10];
float flowPointXasis[10];
int __fasValidBTNCount = 0;
int _fasDutyCycle = 30000;
bool metroFlowCalStarted ;
float flow_value;
//char guiTime[10];


lv_obj_t * crnt_screen;
lv_obj_t * scrFlowcal;
lv_obj_t * fasParentCont;                   
lv_obj_t * _fasContStatusBar;               
lv_obj_t * __fasTimeLabel;                  
lv_obj_t * __fasBatteryLabel;
lv_obj_t * __fasWifiLabel;
lv_obj_t * __fasSignalLabel;
lv_obj_t * _fasFlowHeadingCont;
lv_obj_t * __fasBackArrowLabel;
lv_obj_t * __fasFlowHeadingLbl;
lv_obj_t * _fasFlowLogo;
lv_obj_t * _fasFactoryValTxt;
lv_obj_t * _fasFactoryValVar;
lv_obj_t * _fasReferenceValTxt;
lv_obj_t * _fasRefValCont;
lv_obj_t * _fasRefValInt;
lv_obj_t * _fasRefValPoint;
lv_obj_t * _fasRefValFloat;
lv_obj_t * _fasLPMTxtLbl;
lv_obj_t * _fasCorrectionTxtLbl;
lv_obj_t * _fasCorrectionValueLbl;
lv_obj_t * _fasPercentTxtLbl;
lv_obj_t * _fasStatusIcon;
lv_obj_t * _fasAdjustFlowLbl;
lv_obj_t * _fasMinusBtn;
lv_obj_t * _fasMinusTxt;
lv_obj_t * _fasPlusBtn;
lv_obj_t * _fasPlusTxt;
lv_obj_t * _fasValidBtn;
lv_obj_t * _fasValidTxt;

lv_task_t *_fasMotorTask;

lv_task_t *_fasTimeRefTask;

lv_obj_t * spinbox;

float percentError;
float flowPoint;


/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void CallMetroFlowCalibrationScreen(void)
{
    ESP_LOGI(TAG, "Loading Screen");
    scrFlowcal = lv_obj_create(NULL, NULL);
    lv_scr_load(scrFlowcal);
     if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    fasParentCont = lv_obj_create(scrFlowcal, NULL);  
    lv_obj_set_size(fasParentCont, 320, 480);
    lv_obj_align(fasParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( fasParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(fasParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(fasParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status
    _fasContStatusBar = lv_cont_create(fasParentCont, NULL);
    lv_obj_set_size(_fasContStatusBar, 320, 35);
    lv_obj_align(_fasContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_fasContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    __fasTimeLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasTimeLabel, _fasContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__fasTimeLabel, guiTime);


    static lv_style_t _fasTimeLabelStyle;
    lv_style_init(&_fasTimeLabelStyle);
    lv_style_set_text_font(&_fasTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_fasTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasTimeLabel, LV_LABEL_PART_MAIN, &_fasTimeLabelStyle);

    _fasTimeRefTask = lv_task_create(_fasTimeRefTask_Call, 100, LV_TASK_PRIO_LOW, NULL);

    //Create Label for Battery icon
    __fasBatteryLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasBatteryLabel, _fasContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__fasBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t _fasBatteryLabelStyle;
    lv_style_init(&_fasBatteryLabelStyle);
    lv_style_set_text_font(&_fasBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&_fasBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasBatteryLabel, LV_LABEL_PART_MAIN, &_fasBatteryLabelStyle);

    //Create Label for Wifi icon
    __fasWifiLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasWifiLabel, __fasBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__fasWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __fasWifiLabelStyle;
    lv_style_init(&__fasWifiLabelStyle);
    lv_style_set_text_font(&__fasWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__fasWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasWifiLabel, LV_LABEL_PART_MAIN, &__fasWifiLabelStyle);

    //Create Label for Signal icon
    __fasSignalLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasSignalLabel,  __fasWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__fasSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __fasSignalLabelStyle;
    lv_style_init(&__fasSignalLabelStyle);
    lv_style_set_text_font(&__fasSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&__fasSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasSignalLabel, LV_LABEL_PART_MAIN, &__fasSignalLabelStyle);

    //============================================================================================

    //Crate a container to contain FLOW Header
    _fasFlowHeadingCont = lv_cont_create(fasParentCont, NULL);
    lv_obj_set_size(_fasFlowHeadingCont, 300, 70);
    lv_obj_align(_fasFlowHeadingCont, _fasContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_set_style_local_bg_color(_fasFlowHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fasFlowHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    //Black arrow Container
    lv_obj_t *_fasBlackArrowCont;
    _fasBlackArrowCont =  lv_cont_create(_fasFlowHeadingCont, NULL);
    lv_obj_set_size(_fasBlackArrowCont, 60, 60);
    lv_obj_align(_fasBlackArrowCont, _fasFlowHeadingCont, LV_ALIGN_IN_LEFT_MID, 0 , 0);
    lv_obj_set_style_local_bg_color(_fasBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fasBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );
    lv_obj_set_event_cb(_fasBlackArrowCont, __fasBackArrow_event_handler);


    // Create Back arrow img
    __fasBackArrowLabel = lv_img_create(_fasBlackArrowCont, NULL);
    lv_img_set_src(__fasBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__fasBackArrowLabel, _fasFlowHeadingCont, LV_ALIGN_IN_LEFT_MID, 0 , 0);
    lv_obj_set_click(__fasBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__fasBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__fasBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__fasBackArrowLabel, __fasBackArrow_event_handler);

    //Create Label for FLOW "Heading"
    __fasFlowHeadingLbl = lv_label_create(_fasFlowHeadingCont, NULL);
    lv_obj_align(__fasFlowHeadingLbl, _fasFlowHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -10, -35);
    lv_label_set_text(__fasFlowHeadingLbl, "Flow");

    static lv_style_t __fasFlowHeadingLblStyle;
    lv_style_init(&__fasFlowHeadingLblStyle);
    lv_style_set_text_font(&__fasFlowHeadingLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__fasFlowHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasFlowHeadingLbl, LV_LABEL_PART_MAIN, &__fasFlowHeadingLblStyle);

    //Create FAN Logo
    _fasFlowLogo = lv_img_create(fasParentCont, NULL);
    lv_img_set_src(_fasFlowLogo, &fan_icon);
    lv_obj_align(_fasFlowLogo, fasParentCont, LV_ALIGN_IN_TOP_RIGHT, -25 , 55);
    lv_img_set_auto_size(_fasFlowLogo, true);

    // Create label for "Factory Value" Text 
    _fasFactoryValTxt = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasFactoryValTxt, _fasFlowHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 5 ,20);
    lv_label_set_text(_fasFactoryValTxt, "FACTORY VALUE:");

    static lv_style_t _fasFactoryValTxtStyle;
    lv_style_init(&_fasFactoryValTxtStyle);
    lv_style_set_text_font(&_fasFactoryValTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_fasFactoryValTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_BLACK);
    lv_obj_add_style(_fasFactoryValTxt, LV_LABEL_PART_MAIN, &_fasFactoryValTxtStyle);

    // Create label for "Factory Value" Value
    _fasFactoryValVar = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasFactoryValVar, _fasFactoryValTxt, LV_ALIGN_OUT_RIGHT_MID, 15 ,0);
    lv_label_set_text(_fasFactoryValVar, "7,56");

    static lv_style_t _fasFactoryValVarStyle;
    lv_style_init(&_fasFactoryValVarStyle);
    lv_style_set_text_font(&_fasFactoryValVarStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); //signal_20
    lv_style_set_text_color(&_fasFactoryValVarStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasFactoryValVar, LV_LABEL_PART_MAIN, &_fasFactoryValVarStyle);

    // Create label for "Factory Value" Value
    _fasReferenceValTxt = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasReferenceValTxt, _fasFactoryValTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0 ,20);
    lv_label_set_text(_fasReferenceValTxt, "REFERENCE VALUE:");

    static lv_style_t _fasReferenceValTxtStyle;
    lv_style_init(&_fasReferenceValTxtStyle);
    lv_style_set_text_font(&_fasReferenceValTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_fasReferenceValTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_BLACK);
    lv_obj_add_style(_fasReferenceValTxt, LV_LABEL_PART_MAIN, &_fasReferenceValTxtStyle);

    //Create reference Value Number Containers
    _fasRefValCont = lv_cont_create(fasParentCont, NULL);
    lv_obj_align(_fasRefValCont, _fasReferenceValTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_obj_set_size(_fasRefValCont, 150, 50);
    lv_obj_set_style_local_bg_color(_fasRefValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x36, 0x3a));
    lv_obj_set_style_local_border_width(_fasRefValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    //Create Label for Int par of Reference value
    _fasRefValInt = lv_label_create(_fasRefValCont, NULL);
    lv_obj_align(_fasRefValInt, _fasRefValCont, LV_ALIGN_IN_TOP_LEFT, 20 ,6);
    lv_label_set_text_fmt(_fasRefValInt, "%0.2f",  flowPoints[0]);

    flowPoint   = flowPoints[0];

    static lv_style_t __fasRefValIntStyle;
    lv_style_init(&__fasRefValIntStyle);
    lv_style_set_text_font(&__fasRefValIntStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_36); //signal_20
    lv_style_set_text_color(&__fasRefValIntStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasRefValInt, LV_LABEL_PART_MAIN, &__fasRefValIntStyle);

    //Create Label for Int part of Reference value
    _fasRefValPoint = lv_label_create(_fasRefValCont, NULL);
    lv_obj_align(_fasRefValPoint, _fasRefValInt, LV_ALIGN_OUT_RIGHT_MID, 0 ,-7);
    lv_label_set_text(_fasRefValPoint, "");

    static lv_style_t _fasRefValPointStyle;
    lv_style_init(&_fasRefValPointStyle);
    lv_style_set_text_font(&_fasRefValPointStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_30); //signal_20
    lv_style_set_text_color(&_fasRefValPointStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasRefValPoint, LV_LABEL_PART_MAIN, &_fasRefValPointStyle);

    //Create Label for Float part of Reference value
    _fasRefValFloat = lv_label_create(_fasRefValCont, NULL);
    lv_obj_align(_fasRefValFloat, _fasRefValInt, LV_ALIGN_OUT_RIGHT_MID, 7 , 0);
    lv_label_set_text(_fasRefValFloat, "");

    static lv_style_t _fasRefValFloatStyle;
    lv_style_init(&_fasRefValFloatStyle);
    lv_style_set_text_font(&_fasRefValFloatStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); //signal_20
    lv_style_set_text_color(&_fasRefValFloatStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasRefValFloat, LV_LABEL_PART_MAIN, &_fasRefValFloatStyle);

    //Create Label for LPM Text
    _fasLPMTxtLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasLPMTxtLbl, _fasRefValCont, LV_ALIGN_OUT_RIGHT_BOTTOM, 40 , -15);
    lv_label_set_text(_fasLPMTxtLbl, "LPM");

    static lv_style_t _fasLPMTxtLblStyle;
    lv_style_init(&_fasLPMTxtLblStyle);
    lv_style_set_text_font(&_fasLPMTxtLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); //signal_20
    lv_style_set_text_color(&_fasLPMTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasLPMTxtLbl, LV_LABEL_PART_MAIN, &_fasLPMTxtLblStyle);

    // Create Label For "CORRECTION" Text
    _fasCorrectionTxtLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasCorrectionTxtLbl, _fasRefValCont, LV_ALIGN_OUT_BOTTOM_LEFT, 40, 30);
    lv_label_set_text(_fasCorrectionTxtLbl, "CORRECTION:");

    static lv_style_t _fasCorrectionTxtLblStyle;
    lv_style_init(&_fasCorrectionTxtLblStyle);
    lv_style_set_text_font(&_fasCorrectionTxtLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); 
    lv_style_set_text_color(&_fasCorrectionTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasCorrectionTxtLbl, LV_LABEL_PART_MAIN, &_fasCorrectionTxtLblStyle);

    // Create Label For "CORRECTION" Text
    _fasCorrectionValueLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasCorrectionValueLbl, _fasCorrectionTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 10, -5);
    lv_label_set_text_fmt(_fasCorrectionValueLbl, "%0.2f", percentError);

    static lv_style_t _fasCorrValueLblStyle;
    lv_style_init(&_fasCorrValueLblStyle);
    lv_style_set_text_font(&_fasCorrValueLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_fasCorrValueLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_add_style(_fasCorrectionValueLbl, LV_LABEL_PART_MAIN, &_fasCorrValueLblStyle);

    // Create Label For "%" Text
    _fasPercentTxtLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasPercentTxtLbl, _fasCorrectionValueLbl, LV_ALIGN_OUT_RIGHT_MID, 10, -5);
    lv_label_set_text(_fasPercentTxtLbl, "  %");

    static lv_style_t _fasPercentTxtLblStyle;
    lv_style_init(&_fasPercentTxtLblStyle);
    lv_style_set_text_font(&_fasPercentTxtLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_fasPercentTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_add_style(_fasPercentTxtLbl, LV_LABEL_PART_MAIN, &_fasPercentTxtLblStyle);

    // Create OK Icon
    _fasStatusIcon = lv_img_create(fasParentCont, NULL);
    lv_img_set_src(_fasStatusIcon, &ok_icon);
    lv_obj_align(_fasStatusIcon, _fasPercentTxtLbl, LV_ALIGN_OUT_RIGHT_MID, 20 , 4);

    // Create Label for "Adjust Flow" Text
    _fasAdjustFlowLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasAdjustFlowLbl, _fasCorrectionTxtLbl, LV_ALIGN_OUT_BOTTOM_MID, -40, 20);
    lv_label_set_text(_fasAdjustFlowLbl, "ADJUST PUMP SPEED :");

    static lv_style_t _fasAdjustFlowLblStyle;
    lv_style_init(&_fasAdjustFlowLblStyle);
    lv_style_set_text_font(&_fasAdjustFlowLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_fasAdjustFlowLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_BLACK);
    lv_obj_add_style(_fasAdjustFlowLbl, LV_LABEL_PART_MAIN, &_fasAdjustFlowLblStyle);

    //Create "-" minus button
    _fasMinusBtn = lv_btn_create(fasParentCont, NULL);
    lv_obj_align(_fasMinusBtn, _fasAdjustFlowLbl, LV_ALIGN_OUT_BOTTOM_LEFT, -25, 20);
    lv_obj_set_size(_fasMinusBtn, 100, 50);
    lv_obj_set_event_cb(_fasMinusBtn, __fasMinusBTN_event_handler);
    lv_obj_set_style_local_radius(_fasMinusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_bg_color(_fasMinusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));

    _fasMinusTxt = lv_label_create(_fasMinusBtn, NULL);
    lv_obj_align(_fasMinusTxt, _fasMinusBtn, LV_ALIGN_CENTER, 0, 0); //5,-18
    lv_label_set_text(_fasMinusTxt, "-");
   

    static lv_style_t _fasMinusTxtStyle;
    lv_style_init(&_fasMinusTxtStyle);
    lv_style_set_text_font(&_fasMinusTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_40); 
    lv_style_set_text_color(&_fasMinusTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasMinusTxt, LV_LABEL_PART_MAIN, &_fasMinusTxtStyle);

    //Create "+" Plus button
    _fasPlusBtn = lv_btn_create(fasParentCont, NULL);
    lv_obj_align(_fasPlusBtn, _fasMinusBtn, LV_ALIGN_OUT_RIGHT_TOP, 80, 0);
    lv_obj_set_size(_fasPlusBtn, 100, 50);
    lv_obj_set_event_cb(_fasPlusBtn, __fasPlusBTN_event_handler);
    lv_obj_set_style_local_radius(_fasPlusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_bg_color(_fasPlusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));

    // Create label for 
    _fasPlusTxt = lv_label_create(_fasPlusBtn, NULL);
    lv_obj_align(_fasPlusTxt, _fasPlusBtn, LV_ALIGN_CENTER, 5,-18);
    lv_label_set_text(_fasPlusTxt, "+");
    

    static lv_style_t _fasPlusTxtStyle;
    lv_style_init(&_fasPlusTxtStyle);
    lv_style_set_text_font(&_fasPlusTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_40); 
    lv_style_set_text_color(&_fasPlusTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasPlusTxt, LV_LABEL_PART_MAIN, &_fasPlusTxtStyle);

    //====================
    //====================
    // spinbox = lv_spinbox_create(fasParentCont, NULL);
    // //lv_spinbox_set_range(spinbox, -1000, 90000);
    // lv_spinbox_set_range(spinbox, -1, +10000);
    // lv_spinbox_set_digit_format(spinbox, 5, 3);
    // lv_spinbox_step_prev(spinbox);
    // lv_obj_set_width(spinbox, 110);
    // lv_obj_set_height(spinbox,110);
    // lv_obj_align(spinbox, _fasAdjustFlowLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    // lv_obj_set_style_local_text_font(spinbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24 );
    // //lv_obj_set_style_local_

    // lv_coord_t h = lv_obj_get_height(spinbox);
    // lv_obj_t * btn = lv_btn_create(fasParentCont, NULL);
    // lv_obj_set_size(btn, h+40, h);
    // lv_obj_align(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    // lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
    // lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    // lv_obj_set_event_cb(btn, lv_spinbox_increment_event_cb);

    // btn = lv_btn_create(fasParentCont, btn);
    // lv_obj_align(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    // lv_obj_set_event_cb(btn, lv_spinbox_decrement_event_cb);
    // lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);


    //====================
    //====================
    // Creat a stop Button
    _fasValidBtn = lv_btn_create(fasParentCont, NULL);
    lv_obj_align(_fasValidBtn, fasParentCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );
    lv_obj_set_size(_fasValidBtn, 300, 44);
    lv_obj_set_style_local_radius(_fasValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(_fasValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_set_event_cb(_fasValidBtn, __fasValidBTN_event_handler);
   
    // Creat a stop Button Label
    _fasValidTxt = lv_label_create(_fasValidBtn, NULL);
    lv_obj_align(_fasValidTxt, _fasValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_fasValidTxt, "VALID");
    lv_obj_set_style_local_text_font(_fasValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_text_color(_fasValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    metroFlowCalStarted = true;

    //_fasMotorTask = lv_task_create(_fas_MotorTask_Call, 10, LV_TASK_PRIO_HIGH, NULL);
    crnt_screen = scrFlowcal;
    screenid = SCR_FLOW_CALIBRATION;
}


void _fasTimeRefTask_Call(lv_task_t _fasTimeRefTask) 
{
    if(screenid == SCR_FLOW_CALIBRATION)
    {
        lv_label_set_text(__fasTimeLabel, guiTime);
        lv_label_set_text_fmt(_fasCorrectionValueLbl, "%0.2f", percentError);

        //if(  -10.0 < percentError < 10.0)
        if(   percentError > -10.0 &&   percentError < 10.0     )
        {
            lv_img_set_src(_fasStatusIcon, &ok_icon);
        }
        else 
        {
            lv_img_set_src(_fasStatusIcon, &cross_icon);
        }

    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void  __fasBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) 
    {
        lv_task_del(_fasTimeRefTask);
        CallMetroMenuScreen();
    }
}

static void  __fasPlusBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) 
    {
        if(_fasDutyCycle < 65536){
            _fasDutyCycle = _fasDutyCycle + 500;
        }else{
            _fasDutyCycle = 30000;
        }
        
    }
}

static void  __fasMinusBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) 
    {
        if(_fasDutyCycle > 30000){
            _fasDutyCycle = _fasDutyCycle - 500;
        }else{

        }
        
    }
}

static void  __fasValidBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) 
    {
        
        __fasValidBTNCount++;

        if(__fasValidBTNCount >= NUM_OF_FLOW_CALIBRATION_POINT)
        {
            flowPointXasis[__fasValidBTNCount-1] = flow_value;
            __fasValidBTNCount = 0;
            metroFlowCalStarted = false;
            int i;
            ESP_LOGI(TAG, "X & Y Points are following : \n");
            for( i=0; i < NUM_OF_FLOW_CALIBRATION_POINT; i++)
            {
                ESP_LOGI(TAG, "Y[%d] = %f, x[%d] = %f\n", i,  flowPoints[i], i,  flowPointXasis[i] );
                fflush(NULL);
                
            }
            //Calculate Coefficients
            int pts = global_CurveDegree;
            int deg = global_CurveDegree;
            iPolynomialNew(deg, pts, flowPoints, flowPointXasis);
            // iWriteNVSIntNum(global_CurveDegree, "cDeg");
            CallMetroMenuScreen();

        }else{

            lv_label_set_text_fmt(_fasRefValInt, "%0.2f",  flowPoints[__fasValidBTNCount]);
            flowPoint   = flowPoints[__fasValidBTNCount];
            flowPointXasis[__fasValidBTNCount-1] = flow_value;
            callMetroFlowAdjustScreen(); 
        }
    }
}


static void lv_spinbox_increment_event_cb(lv_obj_t * btn, lv_event_t e)
{
    ESP_LOGI(TAG, "spin box event");
    if(e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(spinbox);
        //_fasDutyCycle = (int)lv_spinbox_get_value(spinbox);

        //   y  ^
        //      |
        //      |                       * (100, 65535) ~ (X2, Y2)
        //      |                   *
        //      |               *
        //      |           *
        //      |       *
        //      |   *
        //      *(0, 20000) ~ (X1, Y1)
        //      |______________________________>
        // X1 =     Lower % Motor Speed, X2 = Highest % Motor Speed
        // Y1 =     Lower limit of duty cycle, Y2 = Highest Limit of Dutycycle
        //point 1 (0, 20000) , Point 2 (100, 65535)
        //Equation y = 455.36 *x + 20000
        float IncBTN = 455.36 * (float)lv_spinbox_get_value(spinbox) + 20000;
        _fasDutyCycle = (int)IncBTN;
    }
}

static void lv_spinbox_decrement_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(spinbox);
        //_fasDutyCycle = (int)lv_spinbox_get_value(spinbox);
        float DecBTN = 455.36 * (float)lv_spinbox_get_value(spinbox) + 20000;
        _fasDutyCycle = (int)DecBTN;

    }
}


/**********************
 *    ERROR ASSERT
 **********************/
