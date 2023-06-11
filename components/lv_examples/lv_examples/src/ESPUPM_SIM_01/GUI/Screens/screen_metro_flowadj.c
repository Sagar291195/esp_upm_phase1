/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
*  @file        screen_flow_calibration.c 
*  @brief       This Screen is for flow calibration
*  @details     This Screen is for flow calibration
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

static void __fcsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void __fcsValidBTN_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/

int global_CurveDegree;
int metroflowUnit;
float flowPoints[10];
static int validBTNCount = 0;

char *getSetPt;

lv_obj_t *crnt_screen;
lv_obj_t *scrFlowCal;
lv_obj_t *fcsParentCont;
lv_obj_t *_fcsContStatusBar;
lv_obj_t *__fcsTimeLabel;
lv_obj_t *__fcsBatteryLabel;
lv_obj_t *__fcsWifiLabel;
lv_obj_t *__fcsSignalLabel;
lv_obj_t *_fcsMetroHeadingCont;
lv_obj_t *__fcsBackArrowLabel;
lv_obj_t *__fcsMetroHeadingLbl;
lv_obj_t *_fcsMetroLogo;
lv_obj_t *_fcsCalPtXLbl;
lv_obj_t *_fcsEnterCalValTA;
lv_obj_t *_fcsValidBtn;
lv_obj_t *_fcsValidBtnLbl;
lv_obj_t *_fcsKeyBord;

/*Create a custom map for the keyboard*/

static const char *fcs_kb_map[] = {
    "1", "2", "3", LV_SYMBOL_BACKSPACE, "\n",

    "4", "5", "6", ".", "\n",

    "7", "8", "9", "0", "\n", ""

};

// static const char * fcs_kb_map[] = {
//           "1", "2", "3",    "\n",

//           "4", "5", "6",    "\n",

//           "7", "8", "9",     "\n" , ""
//           LV_SYMBOL_BACKSPACE  , "0", "#", ""
// };

static const lv_btnmatrix_ctrl_t fcs_kb_ctrl[] = {
    80,
    80,
    80,
    80,
    80,
    80,
    80,
    80,
    80,
    80,
    80,
    80,

};

static const lv_btnmatrix_ctrl_t fcs_tgl_kb_ctrl[] = {
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,
    LV_BTNMATRIX_CTRL_NO_REPEAT,

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

void xCallFlowCalibrationScreen(void)
{
    
    scrFlowCal = lv_obj_create(NULL, NULL);
    lv_scr_load(scrFlowCal);
    if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    fcsParentCont = lv_obj_create(scrFlowCal, NULL);
    lv_obj_set_size(fcsParentCont, 320, 480);
    lv_obj_align(fcsParentCont, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(fcsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); //3989BD
    lv_obj_set_style_local_border_opa(fcsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(fcsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    _fcsContStatusBar = lv_cont_create(fcsParentCont, NULL);
    lv_obj_set_size(_fcsContStatusBar, 320, 35);
    lv_obj_align(_fcsContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_fcsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_fcsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    //Create Watch upper left corner

    __fcsTimeLabel = lv_label_create(_fcsContStatusBar, NULL);
    lv_obj_align(__fcsTimeLabel, _fcsContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__fcsTimeLabel, guiTime);

    static lv_style_t _fcsTimeLabelStyle;
    lv_style_init(&_fcsTimeLabelStyle);
    lv_style_set_text_font(&_fcsTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_fcsTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fcsTimeLabel, LV_LABEL_PART_MAIN, &_fcsTimeLabelStyle);

    //Create Label for Battery icon

    __fcsBatteryLabel = lv_label_create(_fcsContStatusBar, NULL);
    lv_obj_align(__fcsBatteryLabel, _fcsContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__fcsBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t _fcsBatteryLabelStyle;
    lv_style_init(&_fcsBatteryLabelStyle);
    lv_style_set_text_font(&_fcsBatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_fcsBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fcsBatteryLabel, LV_LABEL_PART_MAIN, &_fcsBatteryLabelStyle);

    //Create Label for Wifi icon

    __fcsWifiLabel = lv_label_create(_fcsContStatusBar, NULL);
    lv_obj_align(__fcsWifiLabel, __fcsBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__fcsWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t _fcsWifiLabelStyle;
    lv_style_init(&_fcsWifiLabelStyle);
    lv_style_set_text_font(&_fcsWifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_fcsWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fcsWifiLabel, LV_LABEL_PART_MAIN, &_fcsWifiLabelStyle);

    //Create Label for Signal icon

    __fcsSignalLabel = lv_label_create(_fcsContStatusBar, NULL);
    lv_obj_align(__fcsSignalLabel, __fcsWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__fcsSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __fcsSignalLabelStyle;
    lv_style_init(&__fcsSignalLabelStyle);
    lv_style_set_text_font(&__fcsSignalLabelStyle, LV_STATE_DEFAULT, &signal_20); //signal_20
    lv_style_set_text_color(&__fcsSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fcsSignalLabel, LV_LABEL_PART_MAIN, &__fcsSignalLabelStyle);

    //============================================================================================

    //Crate a container to contain FLOW Header

    _fcsMetroHeadingCont = lv_cont_create(fcsParentCont, NULL);
    lv_obj_set_size(_fcsMetroHeadingCont, 300, 70);
    lv_obj_align(_fcsMetroHeadingCont, _fcsContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_fcsMetroHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fcsMetroHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    //Black arrow Container

    lv_obj_t *_fcsBlackArrowCont;
    _fcsBlackArrowCont = lv_cont_create(_fcsMetroHeadingCont, NULL);
    lv_obj_set_size(_fcsBlackArrowCont, 60, 60);
    lv_obj_align(_fcsBlackArrowCont, _fcsMetroHeadingCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_fcsBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_fcsBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_fcsBlackArrowCont, __fcsBackArrow_event_handler);

    // Create Back arrow img

    __fcsBackArrowLabel = lv_img_create(_fcsBlackArrowCont, NULL);
    lv_img_set_src(__fcsBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__fcsBackArrowLabel, _fcsMetroHeadingCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(__fcsBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__fcsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__fcsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__fcsBackArrowLabel, __fcsBackArrow_event_handler);

    //Create Label for FLOW "Heading"

    __fcsMetroHeadingLbl = lv_label_create(_fcsMetroHeadingCont, NULL);
    lv_obj_align(__fcsMetroHeadingLbl, _fcsMetroHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -10, -35);
    lv_label_set_text(__fcsMetroHeadingLbl, "Flow");

    static lv_style_t __fcsMetroHeadingLblStyle;
    lv_style_init(&__fcsMetroHeadingLblStyle);
    lv_style_set_text_font(&__fcsMetroHeadingLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__fcsMetroHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__fcsMetroHeadingLbl, LV_LABEL_PART_MAIN, &__fcsMetroHeadingLblStyle);

    //Create FAN Logo

    _fcsMetroLogo = lv_img_create(fcsParentCont, NULL);
    lv_img_set_src(_fcsMetroLogo, &fan_icon);
    lv_obj_align(_fcsMetroLogo, fcsParentCont, LV_ALIGN_IN_TOP_RIGHT, -25, 55);
    lv_img_set_auto_size(_fcsMetroLogo, true);

    //Create Label for "CALIBRATE POINT X" Text

    _fcsCalPtXLbl = lv_label_create(fcsParentCont, NULL);
    lv_obj_align(_fcsCalPtXLbl, _fcsMetroHeadingCont, LV_ALIGN_OUT_BOTTOM_MID, -60, 0);
    //lv_label_set_align(_fcsCalPtXLbl, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(_fcsCalPtXLbl, "CALIBRATE POINT X");

    static lv_style_t _fcsCalPtXLblStyle;
    lv_style_init(&_fcsCalPtXLblStyle);
    lv_style_set_text_font(&_fcsCalPtXLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16); //signal_20
    //lv_style_set_text_color(&_fcsCalPtXLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_fcsCalPtXLbl, LV_LABEL_PART_MAIN, &_fcsCalPtXLblStyle);

    //Create Textarea to enter value for calibration

    _fcsEnterCalValTA = lv_textarea_create(fcsParentCont, NULL);
    lv_obj_set_size(_fcsEnterCalValTA, 210, 50);
    lv_obj_align(_fcsEnterCalValTA, _fcsCalPtXLbl, LV_ALIGN_OUT_BOTTOM_LEFT, -70, 20);
    lv_obj_set_click(_fcsEnterCalValTA, false);
    lv_textarea_set_text(_fcsEnterCalValTA, ""); //"Enter REFERENCE Value"
    lv_textarea_set_text_align(_fcsEnterCalValTA, LV_LABEL_ALIGN_CENTER);
    //lv_obj_set_event_cb(_fcsEnterCalValTA, _mtaEnterRefValTA_event_cb);
    lv_obj_set_style_local_text_color(_fcsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(_fcsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, &lv_font_montserrat_26);
    lv_obj_set_style_local_bg_color(_fcsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_obj_set_style_local_border_width(_fcsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(_fcsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_textarea_set_cursor_blink_time(_fcsEnterCalValTA, 0);

    // Create valid button

    _fcsValidBtn = lv_btn_create(fcsParentCont, NULL);
    lv_obj_set_size(_fcsValidBtn, 90, 54);
    lv_obj_align(_fcsValidBtn, _fcsEnterCalValTA, LV_ALIGN_OUT_RIGHT_MID, -5, 0);
    lv_obj_set_style_local_radius(_fcsValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_event_cb(_fcsValidBtn, __fcsValidBTN_event_handler);
    lv_obj_set_style_local_border_width(_fcsValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
     lv_obj_set_style_local_border_color(_fcsValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_color(_fcsValidBtn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_BLUE);
    lv_obj_set_style_local_bg_color(_fcsValidBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));

    //CREATE LABEL FOR VALID TXT ON BUTTON

    _fcsValidBtnLbl = lv_label_create(_fcsValidBtn, NULL);
    lv_obj_align(_fcsValidBtnLbl, _fcsValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_fcsValidBtnLbl, "VALID");

    static lv_style_t _fcsValidBtnLblStyle;
    lv_style_init(&_fcsValidBtnLblStyle);
    lv_style_set_text_font(&_fcsValidBtnLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&_fcsValidBtnLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_color(&_fcsValidBtnLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_fcsValidBtnLblStyle, LV_LABEL_PART_MAIN, 0);
    lv_obj_add_style(_fcsValidBtnLbl, LV_LABEL_PART_MAIN, &_fcsValidBtnLblStyle);

    /* Create a keyboard*/

    _fcsKeyBord = lv_keyboard_create(fcsParentCont, NULL);
    lv_obj_align(_fcsKeyBord, _fcsEnterCalValTA, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_obj_set_size(_fcsKeyBord, 300, 290); // (300, 290)
    lv_keyboard_set_mode(_fcsKeyBord, LV_KEYBOARD_MODE_NUM);
    lv_keyboard_set_map(_fcsKeyBord, LV_KEYBOARD_MODE_NUM, fcs_kb_map);
    lv_keyboard_set_ctrl_map(_fcsKeyBord, LV_KEYBOARD_MODE_NUM, fcs_kb_ctrl); //tgl_kb_ctrl
    lv_keyboard_set_ctrl_map(_fcsKeyBord, LV_KEYBOARD_MODE_NUM, fcs_tgl_kb_ctrl);
    lv_keyboard_set_textarea(_fcsKeyBord, _fcsEnterCalValTA);
    lv_obj_set_style_local_radius(_fcsKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 120); //LV_RADIUS_CIRCLE 50
    lv_obj_set_style_local_bg_color(_fcsKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_bg_color(_fcsKeyBord, LV_KEYBOARD_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_text_font(_fcsKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, &lv_font_montserrat_32);
    lv_obj_set_style_local_text_color(_fcsKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_width(_fcsKeyBord, LV_KEYBOARD_PART_BG, LV_STATE_DEFAULT, 0);
    //lv_obj_set_style_local_border_width(_fcsKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_border_opa(_fcsKeyBord, LV_KEYBOARD_PART_BG, LV_STATE_DEFAULT, 200);
    lv_obj_set_style_local_pad_all(_fcsKeyBord, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 170);
    //lv_obj_set_style_local_

    crnt_screen = scrFlowCal;
    screenid =  SCR_FLOW_ADJUST;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void __fcsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        //printf("Back to Dashbord from presetscrn\n");
        xCallFlowAdjustScreen();
    }
}

static void __fcsValidBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        float SetPt;
        validBTNCount++;
        printf("Button pressed %d\n", validBTNCount);
        getSetPt = lv_textarea_get_text(_fcsEnterCalValTA);
        //SetPt = atof(getSetPt);

        char *toCompare = "";
        if (!strcmp(getSetPt, toCompare))
        {
            SetPt = 0;
        }
        else
        {

            SetPt = atof(getSetPt);
        }

        printf("Entered Set Point Value is: %f\n", SetPt);

        flowPoints[validBTNCount - 1] = SetPt;

        lv_textarea_set_text(_fcsEnterCalValTA, "");
        if (validBTNCount >= global_CurveDegree)
        {
            for (int j = 0; j < global_CurveDegree; j++)
            {
                printf("flowPoints[%d] = %f\n", j, flowPoints[j]);
            }
            //global_CurveDegree = 0;
            validBTNCount = 0;
            printf("Go to Flow Calibarate Screen\n");
            xCallFlowAdjustScreen();
        }
    }
}



/**********************
 *    ERROR ASSERT
 **********************/

