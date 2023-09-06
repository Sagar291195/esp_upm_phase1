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

#include <userCompensationLayer.h>
#include <motor.h>
#include <sensorManagement.h>
#include <calibration.h>

/*********************
 *      DEFINES
 *********************/

#define SYMBOL_SIGNAL "\uf012"
#define TAG "FLOW CALIBRATION"
// Declare Fonts here
LV_FONT_DECLARE(signal_20);

// Declare Images Here

LV_IMG_DECLARE(left_arrow_icon);
LV_IMG_DECLARE(fan_icon);
LV_IMG_DECLARE(ok_icon);
LV_IMG_DECLARE(cross_icon);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void __fasBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void __fasValidBTN_event_handler(lv_obj_t *obj, lv_event_t event);

static void __fasPlusBTN_event_handler(lv_obj_t *obj, lv_event_t event);
static void __fasMinusBTN_event_handler(lv_obj_t *obj, lv_event_t event);
static void _fas_MotorTask_Call(lv_task_t *_fasMotorTask);

/**********************
 *  STATIC VARIABLES
 **********************/
extern float reference_flowcalibration_Points[NUM_OF_FLOW_CALIBRATION_POINT];
int _fasDutyCycle = 15000;
bool metroFlowCalStarted;
float flow_value;

lv_obj_t *crnt_screen;
lv_obj_t *scrFlowcal;
lv_obj_t *fasParentCont;
lv_obj_t *_fasContStatusBar;
lv_obj_t *__fasTimeLabel;
lv_obj_t *__fasBatteryLabel;
lv_obj_t *__fasWifiLabel;
lv_obj_t *__fasSignalLabel;
lv_obj_t *_fasFlowHeadingCont;
lv_obj_t *__fasBackArrowLabel;
lv_obj_t *__fasFlowHeadingLbl;
lv_obj_t *_fasFlowLogo;
lv_obj_t *_fasFactoryValTxt;
lv_obj_t *_fasFactoryValVar;
lv_obj_t *_fasReferenceValTxt;
lv_obj_t *_fasRefValCont;
lv_obj_t *_fasRefValInt;
lv_obj_t *_fasRefValPoint;
lv_obj_t *_fasRefValFloat;
lv_obj_t *_fasLPMTxtLbl;
lv_obj_t *_fasCorrectionTxtLbl;
lv_obj_t *_fasCorrectionValueLbl;
lv_obj_t *_fasPercentTxtLbl;
lv_obj_t *_fasStatusIcon;
lv_obj_t *_fasAdjustFlowLbl;
lv_obj_t *_fasMinusBtn;
lv_obj_t *_fasMinusTxt;
lv_obj_t *_fasPlusBtn;
lv_obj_t *_fasPlusTxt;
lv_obj_t *_fasValidBtn;
lv_obj_t *_fasValidTxt;

lv_task_t *_fasMotorTask;

lv_task_t *_fasTimeRefTask;

lv_obj_t *spinbox;

float percentError;
static float flow_coeffA1 = 0;
static float flow_coeffA2 = 0;
static float flow_coeffA3 = 0;
static float flow_coeffB1 = 0;
static float flow_coeffB2 = 0;
static float flow_coeffB3 = 0;
static float reference_sensorValue[NUM_OF_FLOW_CALIBRATION_POINT];

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
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    fasParentCont = lv_obj_create(scrFlowcal, NULL);
    lv_obj_set_size(fasParentCont, 320, 480);
    lv_obj_align(fasParentCont, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(fasParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 3989BD
    lv_obj_set_style_local_border_opa(fasParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(fasParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status
    _fasContStatusBar = lv_cont_create(fasParentCont, NULL);
    lv_obj_set_size(_fasContStatusBar, 320, 35);
    lv_obj_align(_fasContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_fasContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fasContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    __fasTimeLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasTimeLabel, _fasContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__fasTimeLabel, guiTime);

    static lv_style_t _fasTimeLabelStyle;
    lv_style_init(&_fasTimeLabelStyle);
    lv_style_set_text_font(&_fasTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_fasTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasTimeLabel, LV_LABEL_PART_MAIN, &_fasTimeLabelStyle);

    _fasTimeRefTask = lv_task_create(_fasTimeRefTask_Call, 100, LV_TASK_PRIO_LOW, NULL);

    // Create Label for Battery icon
    __fasBatteryLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasBatteryLabel, _fasContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__fasBatteryLabel, LV_SYMBOL_BATTERY_FULL); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _fasBatteryLabelStyle;
    lv_style_init(&_fasBatteryLabelStyle);
    lv_style_set_text_font(&_fasBatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_fasBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasBatteryLabel, LV_LABEL_PART_MAIN, &_fasBatteryLabelStyle);

    // Create Label for Wifi icon
    __fasWifiLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasWifiLabel, __fasBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__fasWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __fasWifiLabelStyle;
    lv_style_init(&__fasWifiLabelStyle);
    lv_style_set_text_font(&__fasWifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&__fasWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasWifiLabel, LV_LABEL_PART_MAIN, &__fasWifiLabelStyle);

    // Create Label for Signal icon
    __fasSignalLabel = lv_label_create(_fasContStatusBar, NULL);
    lv_obj_align(__fasSignalLabel, __fasWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__fasSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __fasSignalLabelStyle;
    lv_style_init(&__fasSignalLabelStyle);
    lv_style_set_text_font(&__fasSignalLabelStyle, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&__fasSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasSignalLabel, LV_LABEL_PART_MAIN, &__fasSignalLabelStyle);

    // Crate a container to contain FLOW Header
    _fasFlowHeadingCont = lv_cont_create(fasParentCont, NULL);
    lv_obj_set_size(_fasFlowHeadingCont, 300, 70);
    lv_obj_align(_fasFlowHeadingCont, _fasContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_fasFlowHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fasFlowHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Black arrow Container
    lv_obj_t *_fasBlackArrowCont;
    _fasBlackArrowCont = lv_cont_create(_fasFlowHeadingCont, NULL);
    lv_obj_set_size(_fasBlackArrowCont, 60, 60);
    lv_obj_align(_fasBlackArrowCont, _fasFlowHeadingCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_fasBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fasBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_fasBlackArrowCont, __fasBackArrow_event_handler);

    // Create Back arrow img
    __fasBackArrowLabel = lv_img_create(_fasBlackArrowCont, NULL);
    lv_img_set_src(__fasBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__fasBackArrowLabel, _fasFlowHeadingCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(__fasBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__fasBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__fasBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__fasBackArrowLabel, __fasBackArrow_event_handler);

    // Create Label for FLOW "Heading"
    __fasFlowHeadingLbl = lv_label_create(_fasFlowHeadingCont, NULL);
    lv_obj_align(__fasFlowHeadingLbl, _fasFlowHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -10, -35);
    lv_label_set_text(__fasFlowHeadingLbl, "Flow");

    static lv_style_t __fasFlowHeadingLblStyle;
    lv_style_init(&__fasFlowHeadingLblStyle);
    lv_style_set_text_font(&__fasFlowHeadingLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&__fasFlowHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fasFlowHeadingLbl, LV_LABEL_PART_MAIN, &__fasFlowHeadingLblStyle);

    // Create FAN Logo
    _fasFlowLogo = lv_img_create(fasParentCont, NULL);
    lv_img_set_src(_fasFlowLogo, &fan_icon);
    lv_obj_align(_fasFlowLogo, fasParentCont, LV_ALIGN_IN_TOP_RIGHT, -25, 55);
    lv_img_set_auto_size(_fasFlowLogo, true);

    // Create label for "Factory Value" Text
    _fasFactoryValTxt = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasFactoryValTxt, _fasFlowHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 20);
    lv_label_set_text(_fasFactoryValTxt, "REFERENCE VALUE:");

    static lv_style_t _fasFactoryValTxtStyle;
    lv_style_init(&_fasFactoryValTxtStyle);
    lv_style_set_text_font(&_fasFactoryValTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_fasFactoryValTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_BLACK);
    lv_obj_add_style(_fasFactoryValTxt, LV_LABEL_PART_MAIN, &_fasFactoryValTxtStyle);

    // Create label for "Factory Value" Value
    _fasFactoryValVar = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasFactoryValVar, _fasFactoryValTxt, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
    lv_label_set_text_fmt(_fasFactoryValVar, "%0.2f", reference_flowcalibration_Points[get_flow_calibration_point_cout()]);

    static lv_style_t _fasFactoryValVarStyle;
    lv_style_init(&_fasFactoryValVarStyle);
    lv_style_set_text_font(&_fasFactoryValVarStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16); // signal_20
    lv_style_set_text_color(&_fasFactoryValVarStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasFactoryValVar, LV_LABEL_PART_MAIN, &_fasFactoryValVarStyle);

    // Create label for "Factory Value" Value
    _fasReferenceValTxt = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasReferenceValTxt, _fasFactoryValTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_label_set_text(_fasReferenceValTxt, "SENSOR VALUE:");

    static lv_style_t _fasReferenceValTxtStyle;
    lv_style_init(&_fasReferenceValTxtStyle);
    lv_style_set_text_font(&_fasReferenceValTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_fasReferenceValTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_BLACK);
    lv_obj_add_style(_fasReferenceValTxt, LV_LABEL_PART_MAIN, &_fasReferenceValTxtStyle);

    // Create reference Value Number Containers
    _fasRefValCont = lv_cont_create(fasParentCont, NULL);
    lv_obj_align(_fasRefValCont, _fasReferenceValTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_obj_set_size(_fasRefValCont, 150, 50);
    lv_obj_set_style_local_bg_color(_fasRefValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x36, 0x3a));
    lv_obj_set_style_local_border_width(_fasRefValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for Int par of Reference value
    _fasRefValInt = lv_label_create(_fasRefValCont, NULL);
    lv_obj_align(_fasRefValInt, _fasRefValCont, LV_ALIGN_IN_TOP_LEFT, 20, 6);
    lv_label_set_text_fmt(_fasRefValInt, "%0.2f", "0.0");

    static lv_style_t __fasRefValIntStyle;
    lv_style_init(&__fasRefValIntStyle);
    lv_style_set_text_font(&__fasRefValIntStyle, LV_STATE_DEFAULT, &lv_font_montserrat_36); // signal_20
    lv_style_set_text_color(&__fasRefValIntStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasRefValInt, LV_LABEL_PART_MAIN, &__fasRefValIntStyle);

    // Create Label for Int part of Reference value
    _fasRefValPoint = lv_label_create(_fasRefValCont, NULL);
    lv_obj_align(_fasRefValPoint, _fasRefValInt, LV_ALIGN_OUT_RIGHT_MID, 0, -7);
    lv_label_set_text(_fasRefValPoint, "");

    static lv_style_t _fasRefValPointStyle;
    lv_style_init(&_fasRefValPointStyle);
    lv_style_set_text_font(&_fasRefValPointStyle, LV_STATE_DEFAULT, &lv_font_montserrat_30); // signal_20
    lv_style_set_text_color(&_fasRefValPointStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasRefValPoint, LV_LABEL_PART_MAIN, &_fasRefValPointStyle);

    // Create Label for Float part of Reference value
    _fasRefValFloat = lv_label_create(_fasRefValCont, NULL);
    lv_obj_align(_fasRefValFloat, _fasRefValInt, LV_ALIGN_OUT_RIGHT_MID, 7, 0);
    lv_label_set_text(_fasRefValFloat, "");

    static lv_style_t _fasRefValFloatStyle;
    lv_style_init(&_fasRefValFloatStyle);
    lv_style_set_text_font(&_fasRefValFloatStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20); // signal_20
    lv_style_set_text_color(&_fasRefValFloatStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasRefValFloat, LV_LABEL_PART_MAIN, &_fasRefValFloatStyle);

    // Create Label for LPM Text
    _fasLPMTxtLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasLPMTxtLbl, _fasRefValCont, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, -15);
    lv_label_set_text(_fasLPMTxtLbl, "LPM");

    static lv_style_t _fasLPMTxtLblStyle;
    lv_style_init(&_fasLPMTxtLblStyle);
    lv_style_set_text_font(&_fasLPMTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24); // signal_20
    lv_style_set_text_color(&_fasLPMTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasLPMTxtLbl, LV_LABEL_PART_MAIN, &_fasLPMTxtLblStyle);

    // Create Label For "CORRECTION" Text
    _fasCorrectionTxtLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasCorrectionTxtLbl, _fasRefValCont, LV_ALIGN_OUT_BOTTOM_LEFT, 40, 30);
    lv_label_set_text(_fasCorrectionTxtLbl, "CORRECTION:");

    static lv_style_t _fasCorrectionTxtLblStyle;
    lv_style_init(&_fasCorrectionTxtLblStyle);
    lv_style_set_text_font(&_fasCorrectionTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&_fasCorrectionTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasCorrectionTxtLbl, LV_LABEL_PART_MAIN, &_fasCorrectionTxtLblStyle);

    // Create Label For "CORRECTION" Text
    _fasCorrectionValueLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasCorrectionValueLbl, _fasCorrectionTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 10, -5);
    lv_label_set_text_fmt(_fasCorrectionValueLbl, "%0.2f", percentError);

    static lv_style_t _fasCorrValueLblStyle;
    lv_style_init(&_fasCorrValueLblStyle);
    lv_style_set_text_font(&_fasCorrValueLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_fasCorrValueLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_obj_add_style(_fasCorrectionValueLbl, LV_LABEL_PART_MAIN, &_fasCorrValueLblStyle);

    // Create Label For "%" Text
    _fasPercentTxtLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasPercentTxtLbl, _fasCorrectionValueLbl, LV_ALIGN_OUT_RIGHT_MID, 10, -5);
    lv_label_set_text(_fasPercentTxtLbl, "  %");

    static lv_style_t _fasPercentTxtLblStyle;
    lv_style_init(&_fasPercentTxtLblStyle);
    lv_style_set_text_font(&_fasPercentTxtLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_fasPercentTxtLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_obj_add_style(_fasPercentTxtLbl, LV_LABEL_PART_MAIN, &_fasPercentTxtLblStyle);

    // Create OK Icon
    _fasStatusIcon = lv_img_create(fasParentCont, NULL);
    lv_img_set_src(_fasStatusIcon, &ok_icon);
    lv_obj_align(_fasStatusIcon, _fasPercentTxtLbl, LV_ALIGN_OUT_RIGHT_MID, 20, 4);

    // Create Label for "Adjust Flow" Text
    _fasAdjustFlowLbl = lv_label_create(fasParentCont, NULL);
    lv_obj_align(_fasAdjustFlowLbl, _fasCorrectionTxtLbl, LV_ALIGN_OUT_BOTTOM_MID, -40, 20);
    lv_label_set_text(_fasAdjustFlowLbl, "ADJUST PUMP SPEED :");

    static lv_style_t _fasAdjustFlowLblStyle;
    lv_style_init(&_fasAdjustFlowLblStyle);
    lv_style_set_text_font(&_fasAdjustFlowLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_fasAdjustFlowLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_BLACK);
    lv_obj_add_style(_fasAdjustFlowLbl, LV_LABEL_PART_MAIN, &_fasAdjustFlowLblStyle);

    // Create "-" minus button
    _fasMinusBtn = lv_btn_create(fasParentCont, NULL);
    lv_obj_align(_fasMinusBtn, _fasAdjustFlowLbl, LV_ALIGN_OUT_BOTTOM_LEFT, -25, 20);
    lv_obj_set_size(_fasMinusBtn, 100, 50);
    lv_obj_set_event_cb(_fasMinusBtn, __fasMinusBTN_event_handler);
    lv_obj_set_style_local_radius(_fasMinusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_bg_color(_fasMinusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));

    _fasMinusTxt = lv_label_create(_fasMinusBtn, NULL);
    lv_obj_align(_fasMinusTxt, _fasMinusBtn, LV_ALIGN_CENTER, 0, 0); // 5,-18
    lv_label_set_text(_fasMinusTxt, "-");

    static lv_style_t _fasMinusTxtStyle;
    lv_style_init(&_fasMinusTxtStyle);
    lv_style_set_text_font(&_fasMinusTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_style_set_text_color(&_fasMinusTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasMinusTxt, LV_LABEL_PART_MAIN, &_fasMinusTxtStyle);

    // Create "+" Plus button
    _fasPlusBtn = lv_btn_create(fasParentCont, NULL);
    lv_obj_align(_fasPlusBtn, _fasMinusBtn, LV_ALIGN_OUT_RIGHT_TOP, 80, 0);
    lv_obj_set_size(_fasPlusBtn, 100, 50);
    lv_obj_set_event_cb(_fasPlusBtn, __fasPlusBTN_event_handler);
    lv_obj_set_style_local_radius(_fasPlusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_bg_color(_fasPlusBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));

    // Create label for
    _fasPlusTxt = lv_label_create(_fasPlusBtn, NULL);
    lv_obj_align(_fasPlusTxt, _fasPlusBtn, LV_ALIGN_CENTER, 5, -18);
    lv_label_set_text(_fasPlusTxt, "+");

    static lv_style_t _fasPlusTxtStyle;
    lv_style_init(&_fasPlusTxtStyle);
    lv_style_set_text_font(&_fasPlusTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_style_set_text_color(&_fasPlusTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fasPlusTxt, LV_LABEL_PART_MAIN, &_fasPlusTxtStyle);

    // Creat a stop Button
    _fasValidBtn = lv_btn_create(fasParentCont, NULL);
    lv_obj_align(_fasValidBtn, fasParentCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_size(_fasValidBtn, 300, 44);
    lv_obj_set_style_local_radius(_fasValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(_fasValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_obj_set_event_cb(_fasValidBtn, __fasValidBTN_event_handler);

    // Creat a stop Button Label
    _fasValidTxt = lv_label_create(_fasValidBtn, NULL);
    lv_obj_align(_fasValidTxt, _fasValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_fasValidTxt, "VALID");
    lv_obj_set_style_local_text_font(_fasValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_text_color(_fasValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    metroFlowCalStarted = true;

    _fasMotorTask = lv_task_create(_fas_MotorTask_Call, 10, LV_TASK_PRIO_HIGH, NULL);
    crnt_screen = scrFlowcal;
    screenid = SCR_FLOW_CALIBRATION;
}

static void _fas_MotorTask_Call(lv_task_t *_fasMotorTask)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, _fasDutyCycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
}

void _fasTimeRefTask_Call(lv_task_t *_fasTimeRefTask)
{
    float sensorvalue = 0;
    float reference_value = 0;

    if (screenid == SCR_FLOW_CALIBRATION)
    {
        sensorvalue = fGetSdp32DiffPressureAverageValue();
        reference_value = reference_flowcalibration_Points[get_flow_calibration_point_cout()];
        percentError = (((reference_value - sensorvalue) / reference_value) * 100);

        lv_label_set_text(__fasTimeLabel, guiTime);
        lv_label_set_text_fmt(_fasRefValInt, "%0.2f", sensorvalue);
        lv_label_set_text_fmt(_fasCorrectionValueLbl, "%0.2f", percentError);

        if (percentError > -10.0 && percentError < 10.0)
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
static void __fasBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(_fasMotorTask);
        lv_task_del(_fasTimeRefTask);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
        CallMetroMenuScreen();
    }
}

static void __fasPlusBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        if (_fasDutyCycle < 65536)
        {
            _fasDutyCycle = _fasDutyCycle + 500;
        }
        else
        {
            _fasDutyCycle = 15000;
        }
    }
}

static void __fasMinusBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        if (_fasDutyCycle > 15000)
        {
            _fasDutyCycle = _fasDutyCycle - 500;
        }
        else
        {
            _fasDutyCycle = 15000;
        }
    }
}

static void __fasValidBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    int calibration_count = 0;
    float previous_sensor_reading = 0;
    float current_sensor_reading = 0;
    float previous_reference_value = 0;
    float current_reference_value = 0;
    float coeffA = 0;
    float coeffB = 0;

    if (event == LV_EVENT_RELEASED)
    {
        calibration_count = get_flow_calibration_point_cout();
        if (calibration_count == 0)
        {
            previous_sensor_reading = 0;
            previous_reference_value = 0;
        }
        else
        {
            previous_reference_value = reference_flowcalibration_Points[calibration_count - 1];
            previous_sensor_reading = reference_sensorValue[calibration_count - 1];
        }

        current_reference_value = reference_flowcalibration_Points[calibration_count];
        current_sensor_reading = fGetSdp32DiffPressureAverageValue();

        coeffA = (previous_sensor_reading - current_sensor_reading) / (previous_reference_value - current_reference_value);
        coeffB = current_reference_value - (coeffA * current_sensor_reading);

        if (calibration_count == 0)
        {
            flow_coeffA1 = coeffA;
            flow_coeffB1 = coeffB;
        }
        else if (calibration_count == 1)
        {
            flow_coeffA2 = coeffA;
            flow_coeffB2 = coeffB;
        }
        else if (calibration_count == 2)
        {
            flow_coeffA3 = coeffA;
            flow_coeffB3 = coeffB;
        }

        reference_sensorValue[calibration_count] = current_sensor_reading;
        calibration_count++;

        if (calibration_count >= NUM_OF_FLOW_CALIBRATION_POINT)
        {
            calibration_count = 0;
            metroFlowCalStarted = false;

            setcalibrationvalue_flow_coeffA1(flow_coeffA1);
            setcalibrationvalue_flow_coeffA2(flow_coeffA2);
            setcalibrationvalue_flow_coeffA3(flow_coeffA3);
            setcalibrationvalue_flow_coeffB1(flow_coeffB1);
            setcalibrationvalue_flow_coeffB2(flow_coeffB2);
            setcalibrationvalue_flow_coeffB3(flow_coeffB3);

            setcalibration_flow_reference_sensorvalue1(reference_sensorValue[0]);
            setcalibration_flow_reference_sensorvalue2(reference_sensorValue[1]);
            setcalibration_flow_reference_sensorvalue3(reference_sensorValue[2]);

            lv_task_del(_fasTimeRefTask);
            lv_task_del(_fasMotorTask);
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 100));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
            set_flow_calibration_point_cout(calibration_count);
            CallMetroMenuScreen();
        }
        else
        {
            lv_task_del(_fasTimeRefTask);
            lv_task_del(_fasMotorTask);
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 100));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
            set_flow_calibration_point_cout(calibration_count);
            callMetroFlowAdjustScreen();
        }
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
