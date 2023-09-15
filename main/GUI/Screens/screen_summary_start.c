/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-04
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"

/*********************
 *      DEFINES
 *********************/
#define TAG             "SUMMARY START"
#define SYMBOL_SIGNAL   "\uf012"
#define _xmargin        -60

/*********************
 *   DECLARATION
 *********************/
// Declare Fonts here
LV_FONT_DECLARE(signal_20)

// Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void __xssBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void stbBTN_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
char *_pStartDate_ = "04/03/2021";
char *_pStopDate_ = "05/03/2021";
char *_pStartTime_ = "15H35M";
char *_pStopTime_ = "20H35M";
char *_pSampleNumber_ = "SAMPLE N°1356";
char *_pSetPointValue_ = "7 L/min";
char *_pSeqNum_ = "2";
char *_pDurationVal_ = "12 H";
char *_pPersonName_ = "Mark";
char *_pLtrCountVal_ = "10012.00 L";
char *_pHourCountVal_ = "2300.13 H";

char startTime[100];
char stopTime[25];

char startDateEnd[55];
char stopDateEnd[45];
char startTimeEnd[60];
char stopTimeEnd[60];

char guiHrDef[25];
char guiMinDef[32];

int xSpacebwline = 2;
int global_DashbordBTNflag;
bool isMotor;
int strttmrcount = 0;
bool PumpStopForcefully;


float targetHr;
float effectiveHr;
float variationHr;
bool defaultParaSelected;

// double StopLTRCountVal;

/**
 * @brief refers to the current ongoing screen
 *
 */
lv_obj_t *crnt_screen;
lv_obj_t *scrSummaryStart;
lv_obj_t *xssParentContainer_ss;
lv_obj_t *_xssContStatusBar_ss;
lv_obj_t *__xssTimeLabel_ss;
lv_obj_t *__xssBatteryLabel_ss;
lv_obj_t *__xssWifiLabel_ss;
lv_obj_t *__xssSignalLabel_ss;
lv_obj_t *_xssParaHeadingCont_ss;
lv_obj_t *__xssBackArrowLabel_ss;
lv_obj_t *__xssSummaryHeadingLbl_ss;
lv_obj_t *_xssSummayCont_ss;
lv_obj_t *__xssStartTxtLbl_ss;
lv_obj_t *__xssStopTxtLbl_ss;
lv_obj_t *__xssStartDateLbl_ss;
lv_obj_t *__xssStopDateLbl_ss;
lv_obj_t *__xssStartTimeLbl_ss;
lv_obj_t *__xssStopTimeLbl_ss;
lv_obj_t *__xssSampleNunLbl_ss;
lv_obj_t *_xssFlowStPBaseCont_ss;
lv_obj_t *__xssFlowStPTxtLbl_ss;
lv_obj_t *__xssFlowStPValueLbl_ss;
lv_obj_t *_xssSeqNumBaseCont_ss;
lv_obj_t *__xssSeqNumTxtLbl_ss;
lv_obj_t *__xssSeqNumValueLbl_ss;
lv_obj_t *_xssDurationCont_ss;
lv_obj_t *__xssDurationTxtLbl_ss;
lv_obj_t *__xssDurationValueLbl_ss;
lv_obj_t *_xssPersonBaseCont_ss;
lv_obj_t *__xssPersonTxtLbl_ss;
lv_obj_t *__xssPersonNameLbl_ss;
lv_obj_t *_xssLtrCntrBaseCont_ss;
lv_obj_t *__xssLtrCntrTxtLbl_ss;
lv_obj_t *__xssLtrCntrValLbl_ss;
lv_obj_t *_xssHrCntrBaseCont_ss;
lv_obj_t *__xssHrCntrTxtLbl_ss;
lv_obj_t *__xssHrCntrValLbl_ss;
lv_obj_t *_xssValidJobBtn_ss;
lv_obj_t *__xssStartJobTxtLabel_ss;
lv_obj_t *_xssBlackArrowCont;

lv_task_t *__xssTMrefresherTask;          // Time label refresher task
lv_task_t *__xssStartBTNCountTask = NULL; // Task to count on Button
lv_task_t *infoWidgetUpdateTask;          // Task to be called after Job time is over

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void xssSummaryStartScreen(void)
{
    scrSummaryStart = lv_obj_create(NULL, NULL);
    lv_scr_load(scrSummaryStart);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    ESP_LOGI(TAG, "Loading Summary Start Screen");
    xssParentContainer_ss = lv_cont_create(scrSummaryStart, NULL);
    // lv_scr_load(xssParentContainer_ss);
    lv_obj_set_size(xssParentContainer_ss, 320, 480);
    lv_obj_set_click(xssParentContainer_ss, false);
    lv_obj_align(xssParentContainer_ss, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(xssParentContainer_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(xssParentContainer_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(xssParentContainer_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    _xssContStatusBar_ss = lv_cont_create(xssParentContainer_ss, NULL);
    lv_obj_set_size(_xssContStatusBar_ss, 320, 35);
    lv_obj_align(_xssContStatusBar_ss, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xssContStatusBar_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_xssContStatusBar_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    __xssTimeLabel_ss = lv_label_create(_xssContStatusBar_ss, NULL);
    lv_obj_align(__xssTimeLabel_ss, _xssContStatusBar_ss, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__xssTimeLabel_ss, guiTime);

    static lv_style_t _xssTimeLabelStyle_ss;
    lv_style_init(&_xssTimeLabelStyle_ss);
    lv_style_set_text_font(&_xssTimeLabelStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xssTimeLabelStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xssTimeLabel_ss, LV_LABEL_PART_MAIN, &_xssTimeLabelStyle_ss);
    __xssTMrefresherTask = lv_task_create(__xssTimeLabel_ss_refr_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create Label for Battery icon
    __xssBatteryLabel_ss = lv_label_create(_xssContStatusBar_ss, NULL);
    lv_obj_align(__xssBatteryLabel_ss, _xssContStatusBar_ss, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__xssBatteryLabel_ss, LV_SYMBOL_BATTERY_FULL); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xssBatteryLabelStyle_ss;
    lv_style_init(&_xssBatteryLabelStyle_ss);
    lv_style_set_text_font(&_xssBatteryLabelStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_xssBatteryLabelStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xssBatteryLabel_ss, LV_LABEL_PART_MAIN, &_xssBatteryLabelStyle_ss);

    // Create Label for Wifi icon
    __xssWifiLabel_ss = lv_label_create(_xssContStatusBar_ss, NULL);
    lv_obj_align(__xssWifiLabel_ss, __xssBatteryLabel_ss, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__xssWifiLabel_ss, LV_SYMBOL_WIFI);

    static lv_style_t _xssWifiLabelStyle_ss;
    lv_style_init(&_xssWifiLabelStyle_ss);
    lv_style_set_text_font(&_xssWifiLabelStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xssWifiLabelStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xssWifiLabel_ss, LV_LABEL_PART_MAIN, &_xssWifiLabelStyle_ss);

    // Create Label for Signal icon
    __xssSignalLabel_ss = lv_label_create(_xssContStatusBar_ss, NULL);
    lv_obj_align(__xssSignalLabel_ss, __xssWifiLabel_ss, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__xssSignalLabel_ss, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t _xssSignalLabelStyle_ss;
    lv_style_init(&_xssSignalLabelStyle_ss);
    lv_style_set_text_font(&_xssSignalLabelStyle_ss, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xssSignalLabelStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xssSignalLabel_ss, LV_LABEL_PART_MAIN, &_xssSignalLabelStyle_ss);

    // Crate a container to contain Summary Start Header
    _xssParaHeadingCont_ss = lv_cont_create(xssParentContainer_ss, NULL);
    lv_obj_set_size(_xssParaHeadingCont_ss, 300, 70);
    lv_obj_align(_xssParaHeadingCont_ss, _xssContStatusBar_ss, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xssParaHeadingCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssParaHeadingCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Back Arrow container
    _xssBlackArrowCont = lv_cont_create(_xssParaHeadingCont_ss, NULL);
    lv_obj_set_size(_xssBlackArrowCont, 60, 60);
    lv_obj_align(_xssBlackArrowCont, _xssParaHeadingCont_ss, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_obj_set_style_local_bg_color(_xssBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_xssBlackArrowCont, __xssBackArrow_event_handler);

    // Create Back arrow img
    __xssBackArrowLabel_ss = lv_img_create(_xssBlackArrowCont, NULL);
    lv_img_set_src(__xssBackArrowLabel_ss, &left_arrow_icon);
    lv_obj_align(__xssBackArrowLabel_ss, _xssBlackArrowCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(__xssBackArrowLabel_ss, true);
    lv_obj_set_style_local_image_recolor_opa(__xssBackArrowLabel_ss, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__xssBackArrowLabel_ss, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__xssBackArrowLabel_ss, __xssBackArrow_event_handler);

    // Create Label for Sequences "Heading"
    __xssSummaryHeadingLbl_ss = lv_label_create(_xssParaHeadingCont_ss, NULL);
    lv_obj_align(__xssSummaryHeadingLbl_ss, _xssParaHeadingCont_ss, LV_ALIGN_IN_BOTTOM_MID, -60, -35);
    lv_label_set_text(__xssSummaryHeadingLbl_ss, "Summary Start");

    static lv_style_t _xaParameterHeadingStyle_Adv;
    lv_style_init(&_xaParameterHeadingStyle_Adv);
    lv_style_set_text_font(&_xaParameterHeadingStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&_xaParameterHeadingStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xssSummaryHeadingLbl_ss, LV_LABEL_PART_MAIN, &_xaParameterHeadingStyle_Adv);

    // Create a container to put summary massage
    _xssSummayCont_ss = lv_cont_create(xssParentContainer_ss, NULL);
    lv_obj_set_size(_xssSummayCont_ss, 300, 310);
    lv_obj_align(_xssSummayCont_ss, _xssParaHeadingCont_ss, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xssSummayCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssSummayCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create style for Start Stop text
    static lv_style_t xssStartStopLblStyle_ss;
    lv_style_init(&xssStartStopLblStyle_ss);
    lv_style_set_text_font(&xssStartStopLblStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&xssStartStopLblStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    // Create Label for Start  text
    __xssStartTxtLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    lv_obj_align(__xssStartTxtLbl_ss, _xssSummayCont_ss, LV_ALIGN_IN_TOP_LEFT, 10, 20);
    lv_label_set_text(__xssStartTxtLbl_ss, "START:");
    lv_obj_add_style(__xssStartTxtLbl_ss, LV_LABEL_PART_MAIN, &xssStartStopLblStyle_ss);

    // Create Label for Stop  text
    __xssStopTxtLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    lv_obj_align(__xssStopTxtLbl_ss, __xssStartTxtLbl_ss, LV_ALIGN_OUT_RIGHT_MID, 100, 0);
    lv_label_set_text(__xssStopTxtLbl_ss, "STOP:");
    lv_obj_add_style(__xssStopTxtLbl_ss, LV_LABEL_PART_MAIN, &xssStartStopLblStyle_ss);

    // Create style for Start Stop Date
    static lv_style_t xssStSpDateLblStyle_ss;
    lv_style_init(&xssStSpDateLblStyle_ss);
    lv_style_set_text_font(&xssStSpDateLblStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&xssStSpDateLblStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    // Create Label for Start Date
    /* geting the first sequence start date */
    sequence_t *seq = pGetAddressOfSequenceArray();

    __xssStartDateLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    lv_obj_align(__xssStartDateLbl_ss, __xssStartTxtLbl_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__xssStartDateLbl_ss, seq[0].cStartDate); // getting the start date
    lv_obj_add_style(__xssStartDateLbl_ss, LV_LABEL_PART_MAIN, &xssStSpDateLblStyle_ss);

    // Create Label for Stop Date
    char endDate[50];
    char endTime[50];
    /* sending the date of time of the 1st sequence to be run */
    vGetEndDateAndTimeOfSequence(endDate, 1, sizeof(endDate), endTime, sizeof(endTime));
    __xssStopDateLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    lv_obj_align(__xssStopDateLbl_ss, __xssStopTxtLbl_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(__xssStopDateLbl_ss, endDate); // guiSeqDate1 ,  _pStopDate_
    lv_obj_add_style(__xssStopDateLbl_ss, LV_LABEL_PART_MAIN, &xssStSpDateLblStyle_ss);

    // Create style for start stop time label
    static lv_style_t xssStSpTimeLblStyle_ss;
    lv_style_init(&xssStSpTimeLblStyle_ss);
    lv_style_set_text_font(&xssStSpTimeLblStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&xssStSpTimeLblStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    // Create Label for Start time
    __xssStartTimeLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    if (startTimeLabelAdgFlg == true)
    {
        lv_obj_align(__xssStartTimeLbl_ss, __xssStartDateLbl_ss, LV_ALIGN_OUT_BOTTOM_RIGHT, -28, 2);
    }
    if (startTimeLabelAdgFlg == false)
    {
        lv_obj_align(__xssStartTimeLbl_ss, __xssStartDateLbl_ss, LV_ALIGN_OUT_BOTTOM_RIGHT, -22, 2);
    }
    // lv_obj_align(__xssStartTimeLbl_ss, __xssStartDateLbl_ss, LV_ALIGN_OUT_BOTTOM_RIGHT, -28, 2);
    // lv_obj_align(__xssStartTimeLbl_ss, __xssStartDateLbl_ss, LV_ALIGN_OUT_BOTTOM_RIGHT, -22, -20);
    // if(startTimeLabelAdgFlg){lv_label_set_text(__xssStartTimeLbl_ss, _xaHourMinrollerbuf);}
    // lv_label_set_text(__xssStartTimeLbl_ss, _xaHourMinrollerbuf); // _xaHourMinrollerbuf  _pStartTime_
    lv_label_set_text_fmt(__xssStartTimeLbl_ss, "%dH %dM", seq[0].uStartHour, seq[0].uStartMin);
    char min1[] = "00";
    sprintf(startTime, "%s:%s:%s", _xaHourrollerbuf, _xaMinutrollerbuf, min1);
    lv_obj_add_style(__xssStartTimeLbl_ss, LV_LABEL_PART_MAIN, &xssStSpTimeLblStyle_ss);

    // Create Label for Stop time
    __xssStopTimeLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    lv_obj_align(__xssStopTimeLbl_ss, __xssStopDateLbl_ss, LV_ALIGN_OUT_BOTTOM_RIGHT, -22, 2);
    lv_label_set_text_fmt(__xssStopTimeLbl_ss, "%s\n", endTime); // stopTime , _pStopTime_
    lv_obj_add_style(__xssStopTimeLbl_ss, LV_LABEL_PART_MAIN, &xssStSpTimeLblStyle_ss);

    // Create style for Sample Number label
    static lv_style_t xssSampleNumLblStyle_ss;
    lv_style_init(&xssSampleNumLblStyle_ss);
    lv_style_set_text_font(&xssSampleNumLblStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_style_set_text_color(&xssSampleNumLblStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    // Create Sample Number Label
    __xssSampleNunLbl_ss = lv_label_create(_xssSummayCont_ss, NULL);
    lv_obj_align(__xssSampleNunLbl_ss, __xssStartTxtLbl_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
    lv_label_set_text_fmt(__xssSampleNunLbl_ss, "SAMPLE N° %d", uGetCurrentSampleNumber() + 1); // visible sample number is current sample number + 1
    lv_obj_add_style(__xssSampleNunLbl_ss, LV_LABEL_PART_MAIN, &xssSampleNumLblStyle_ss);

    static lv_style_t xssBlueFixTextStyle_ss;
    lv_style_init(&xssBlueFixTextStyle_ss);
    lv_style_set_text_font(&xssBlueFixTextStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&xssBlueFixTextStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    static lv_style_t xssWhiteVarValueStyle_ss;
    lv_style_init(&xssWhiteVarValueStyle_ss);
    lv_style_set_text_font(&xssWhiteVarValueStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&xssWhiteVarValueStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    _xssFlowStPBaseCont_ss = lv_cont_create(_xssSummayCont_ss, NULL);
    lv_obj_align(_xssFlowStPBaseCont_ss, __xssSampleNunLbl_ss, LV_ALIGN_OUT_BOTTOM_LEFT, -8, 12);
    lv_obj_set_size(_xssFlowStPBaseCont_ss, 295, 26);
    lv_obj_set_style_local_bg_color(_xssFlowStPBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssFlowStPBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Flow Set Point Txt Label
    __xssFlowStPTxtLbl_ss = lv_label_create(_xssFlowStPBaseCont_ss, NULL);                   //_xssSummayCont_ss
    lv_obj_align(__xssFlowStPTxtLbl_ss, _xssFlowStPBaseCont_ss, LV_ALIGN_IN_LEFT_MID, 0, 0); // xSpacebwline
    lv_label_set_text(__xssFlowStPTxtLbl_ss, "FLOW SET POINT");
    lv_obj_add_style(__xssFlowStPTxtLbl_ss, LV_LABEL_PART_MAIN, &xssBlueFixTextStyle_ss);

    // Create Flow Set Point Value Label
    __xssFlowStPValueLbl_ss = lv_label_create(_xssFlowStPBaseCont_ss, NULL);                           //_xssSummayCont_ss
    lv_obj_align(__xssFlowStPValueLbl_ss, _xssFlowStPBaseCont_ss, LV_ALIGN_IN_RIGHT_MID, _xmargin, 0); // #define _xmargin -60
    // lv_label_set_text(__xssFlowStPValueLbl_ss, _pSetPointValue_);
    lv_label_set_text_fmt(__xssFlowStPValueLbl_ss, "%0.f L/Min", seq[0].fFlowSetPoint);
    lv_obj_add_style(__xssFlowStPValueLbl_ss, LV_LABEL_PART_MAIN, &xssWhiteVarValueStyle_ss);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    _xssSeqNumBaseCont_ss = lv_cont_create(_xssSummayCont_ss, NULL);
    lv_obj_align(_xssSeqNumBaseCont_ss, _xssFlowStPBaseCont_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, xSpacebwline);
    lv_obj_set_size(_xssSeqNumBaseCont_ss, 295, 26);
    lv_obj_set_style_local_bg_color(_xssSeqNumBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssSeqNumBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Sequence Number Txt Label
    __xssSeqNumTxtLbl_ss = lv_label_create(_xssSeqNumBaseCont_ss, NULL);
    lv_obj_align(__xssSeqNumTxtLbl_ss, _xssSeqNumBaseCont_ss, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xssSeqNumTxtLbl_ss, "SEQUENCE NUMBER");
    lv_obj_add_style(__xssSeqNumTxtLbl_ss, LV_LABEL_PART_MAIN, &xssBlueFixTextStyle_ss);

    // Create SEQUENCE NUMBER Value Label
    __xssSeqNumValueLbl_ss = lv_label_create(_xssSeqNumBaseCont_ss, NULL);
    lv_obj_align(__xssSeqNumValueLbl_ss, _xssSeqNumBaseCont_ss, LV_ALIGN_IN_RIGHT_MID, _xmargin, 0);
    char seqNum[4];
    sprintf(seqNum, "%d", uGetNoOfSequenceInArray());
    lv_label_set_text(__xssSeqNumValueLbl_ss, seqNum); // WseqNum1  _pSeqNum_
    lv_obj_add_style(__xssSeqNumValueLbl_ss, LV_LABEL_PART_MAIN, &xssWhiteVarValueStyle_ss);

    _xssDurationCont_ss = lv_cont_create(_xssSummayCont_ss, NULL);
    lv_obj_align(_xssDurationCont_ss, _xssSeqNumBaseCont_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, xSpacebwline);
    lv_obj_set_size(_xssDurationCont_ss, 295, 26);
    lv_obj_set_style_local_bg_color(_xssDurationCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssDurationCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create DURATION Txt Label
    __xssDurationTxtLbl_ss = lv_label_create(_xssDurationCont_ss, NULL);
    lv_obj_align(__xssDurationTxtLbl_ss, _xssDurationCont_ss, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xssDurationTxtLbl_ss, "DURATION");
    lv_obj_add_style(__xssDurationTxtLbl_ss, LV_LABEL_PART_MAIN, &xssBlueFixTextStyle_ss);

    // Create SEQUENCE NUMBER Value Label
    __xssDurationValueLbl_ss = lv_label_create(_xssDurationCont_ss, NULL);
    lv_obj_align(__xssDurationValueLbl_ss, _xssDurationCont_ss, LV_ALIGN_IN_RIGHT_MID, _xmargin, 0);
    // lv_label_set_text(__xssDurationValueLbl_ss, _pDurationVal_);
    lv_label_set_text_fmt(__xssDurationValueLbl_ss, "%dH %dM ", seq[0].uDurationHour, seq[0].uDurationMinutes);
    lv_obj_add_style(__xssDurationValueLbl_ss, LV_LABEL_PART_MAIN, &xssWhiteVarValueStyle_ss);

    _xssPersonBaseCont_ss = lv_cont_create(_xssSummayCont_ss, NULL);
    lv_obj_align(_xssPersonBaseCont_ss, _xssDurationCont_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, xSpacebwline);
    lv_obj_set_size(_xssPersonBaseCont_ss, 295, 26);
    lv_obj_set_style_local_bg_color(_xssPersonBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssPersonBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create START PERSON Txt Label
    __xssPersonTxtLbl_ss = lv_label_create(_xssPersonBaseCont_ss, NULL);
    lv_obj_align(__xssPersonTxtLbl_ss, _xssPersonBaseCont_ss, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xssPersonTxtLbl_ss, "START PERSON");
    lv_obj_add_style(__xssPersonTxtLbl_ss, LV_LABEL_PART_MAIN, &xssBlueFixTextStyle_ss);

    // Create  Person Name Label
    __xssPersonNameLbl_ss = lv_label_create(_xssPersonBaseCont_ss, NULL);
    lv_obj_align(__xssPersonNameLbl_ss, _xssPersonBaseCont_ss, LV_ALIGN_IN_RIGHT_MID, _xmargin, 0);
    lv_label_set_text(__xssPersonNameLbl_ss, seq[0].cStartPerson);
    lv_obj_add_style(__xssPersonNameLbl_ss, LV_LABEL_PART_MAIN, &xssWhiteVarValueStyle_ss);

    _xssLtrCntrBaseCont_ss = lv_cont_create(_xssSummayCont_ss, NULL);
    lv_obj_align(_xssLtrCntrBaseCont_ss, _xssPersonBaseCont_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, xSpacebwline);
    lv_obj_set_size(_xssLtrCntrBaseCont_ss, 295, 26);
    lv_obj_set_style_local_bg_color(_xssLtrCntrBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssLtrCntrBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create LITER COUNTER Txt Label
    __xssLtrCntrTxtLbl_ss = lv_label_create(_xssLtrCntrBaseCont_ss, NULL);
    lv_obj_align(__xssLtrCntrTxtLbl_ss, _xssLtrCntrBaseCont_ss, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xssLtrCntrTxtLbl_ss, "LITER COUNTER");
    lv_obj_add_style(__xssLtrCntrTxtLbl_ss, LV_LABEL_PART_MAIN, &xssBlueFixTextStyle_ss);

    // Create  Liter Counter value Label
    __xssLtrCntrValLbl_ss = lv_label_create(_xssLtrCntrBaseCont_ss, NULL);
    lv_obj_align(__xssLtrCntrValLbl_ss, _xssLtrCntrBaseCont_ss, LV_ALIGN_IN_RIGHT_MID, _xmargin, 0);
    lv_label_set_text_fmt(__xssLtrCntrValLbl_ss, "%0.2lf", fGetTotalLiterCount()); 
    lv_obj_add_style(__xssLtrCntrValLbl_ss, LV_LABEL_PART_MAIN, &xssWhiteVarValueStyle_ss);

    _xssHrCntrBaseCont_ss = lv_cont_create(_xssSummayCont_ss, NULL);
    lv_obj_align(_xssHrCntrBaseCont_ss, _xssLtrCntrBaseCont_ss, LV_ALIGN_OUT_BOTTOM_LEFT, 0, xSpacebwline);
    lv_obj_set_size(_xssHrCntrBaseCont_ss, 295, 26);
    lv_obj_set_style_local_bg_color(_xssHrCntrBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xssHrCntrBaseCont_ss, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Hour COUNTER Txt Label
    __xssHrCntrTxtLbl_ss = lv_label_create(_xssHrCntrBaseCont_ss, NULL);
    lv_obj_align(__xssHrCntrTxtLbl_ss, _xssHrCntrBaseCont_ss, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xssHrCntrTxtLbl_ss, "HOUR COUNTER");
    lv_obj_add_style(__xssHrCntrTxtLbl_ss, LV_LABEL_PART_MAIN, &xssBlueFixTextStyle_ss);

    // Create  Hour Counter value Label
    __xssHrCntrValLbl_ss = lv_label_create(_xssHrCntrBaseCont_ss, NULL);
    lv_obj_align(__xssHrCntrValLbl_ss, _xssHrCntrBaseCont_ss, LV_ALIGN_IN_RIGHT_MID, _xmargin, 0);

    lv_label_set_text_fmt(__xssHrCntrValLbl_ss, "%0.2f", fGetTotalHoursCount()); 
    lv_obj_add_style(__xssHrCntrValLbl_ss, LV_LABEL_PART_MAIN, &xssWhiteVarValueStyle_ss);

    // Create Start Job Button
    _xssValidJobBtn_ss = lv_btn_create(xssParentContainer_ss, NULL);
    lv_obj_align(_xssValidJobBtn_ss, _xssSummayCont_ss, LV_ALIGN_OUT_BOTTOM_MID, -85, 7);
    lv_obj_set_size(_xssValidJobBtn_ss, 300, 44);

    lv_obj_set_event_cb(_xssValidJobBtn_ss, stbBTN_event_handler);
    // lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);
    static lv_style_t _xssValidJobBtnStyle_ss;
    lv_style_init(&_xssValidJobBtnStyle_ss);
    lv_style_set_radius(&_xssValidJobBtnStyle_ss, LV_STATE_DEFAULT, 0);
    // lv_style_set_bg_color(&_xStopBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEb, 0x3B, 0x5A));
    lv_style_set_bg_color(&_xssValidJobBtnStyle_ss, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #359Fe2
    lv_style_set_bg_color(&_xssValidJobBtnStyle_ss, LV_STATE_DEFAULT | LV_STATE_PRESSED | LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_width(&_xssValidJobBtnStyle_ss, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_xssValidJobBtnStyle_ss, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_xssValidJobBtn_ss, LV_BTN_PART_MAIN, &_xssValidJobBtnStyle_ss);

    // lv_obj_set_event_cb(_xaValidBtn, valid_btn_event_handler);
    // Creat a Valid Button Label
    __xssStartJobTxtLabel_ss = lv_label_create(_xssValidJobBtn_ss, NULL);
    lv_obj_align(__xssStartJobTxtLabel_ss, _xssValidJobBtn_ss, LV_ALIGN_IN_TOP_MID, 0, 0);
    if (defaultParaSelected == false)
    {
        lv_label_set_text(__xssStartJobTxtLabel_ss, "START JOB");
    }
    if (defaultParaSelected == true)
    {
        lv_label_set_text(__xssStartJobTxtLabel_ss, "JOB STARTING IN 10 Sec");
    }
    __xssStartBTNCountTask = lv_task_create(__xssStartJobBTN_refr_func, 1000, LV_TASK_PRIO_LOW, NULL);

    static lv_style_t __xssStartJobTxtLabelStyle_ss;
    lv_style_init(&__xssStartJobTxtLabelStyle_ss);
    lv_style_set_text_font(&__xssStartJobTxtLabelStyle_ss, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&__xssStartJobTxtLabelStyle_ss, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xssValidJobBtn_ss, LV_LABEL_PART_MAIN, &__xssStartJobTxtLabelStyle_ss);
    crnt_screen = scrSummaryStart; // scrSummaryStart
    screenid = SCR_SUMMARY_START;
}

// This is the task to GUI Time label updating

void __xssTimeLabel_ss_refr_func(lv_task_t *__xssTMrefresherTask)
{
    if (lv_obj_get_screen(__xssTimeLabel_ss) == lv_scr_act())
    {
        lv_label_set_text(__xssTimeLabel_ss, guiTime);
    }
}

void __xssStartJobBTN_refr_func(lv_task_t *__xssStartBTNCountTask)
{
    if (__xssStartBTNCountTask != NULL)
    {
        if (lv_obj_get_screen(__xssTimeLabel_ss) == lv_scr_act())
        {
            if (defaultParaSelected == true && strttmrcount <= 10)
            {
                strttmrcount++; // strttmrcount
                vTaskDelay(1000);
                int revstrttmrcount = 10 - strttmrcount;
                lv_label_set_text_fmt(__xssStartJobTxtLabel_ss, "JOB STARTING IN %d Sec", revstrttmrcount);
                lv_label_set_text_fmt(__xssStartTimeLbl_ss, "IN %d Sec", revstrttmrcount);
            }
            if (strttmrcount >= 10)
            {
                /* now sample is valid and can be saved into the memory and proceed for the sample execution */
                vControllerSampleIsValid();
                lv_task_del(__xssTMrefresherTask);
                lv_task_del(__xssStartBTNCountTask);
                __xssStartBTNCountTask = NULL;
                vTaskDelay(1);
                strttmrcount = 0;
                dashboardflg = 1;
                global_DashbordBTNflag = 2;
                // isMotor = true;
                sprintf(startDateEnd, "%s", guiDate);
                sprintf(startTimeEnd, "%sH%sM", guiHrDef, guiMinDef);
                defaultParaSelected = false;
                pxDashboardScreen(); // Open Dashboard Screen
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void __xssBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(__xssTMrefresherTask);
        lv_task_del(__xssStartBTNCountTask);
        __xssStartBTNCountTask = NULL;
        // printf("Back to Dashbord from presetscrn\n");
        global_DashbordBTNflag = 1;
        rollerMovCkFlag = false;
        // rollerMovCkFlag = false;
        xsPresetScreenAdvance();
    }
}

/**
 * @brief start job event handler
 *
 * @param obj
 * @param event
 */
static void stbBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(__xssTMrefresherTask);
        if (__xssStartBTNCountTask != NULL)
        {
            lv_task_del(__xssStartBTNCountTask);
            __xssStartBTNCountTask = NULL;
        }
        /* now sample is valid and can be saved into the memory and proceed for the sample execution */
        vControllerSampleIsValid();
        // printf("Back to Dashbord from presetscrn\n");
        // printf("xdat value is: %d \n", xdt );
        defaultParaSelected = false;
        global_DashbordBTNflag = 2;
        /* Creating the dashboard screen and setting the dashboard to the ready mode. */
        dashboardflg = 3;
        rollerMovCkFlag = false;
        pxDashboardScreen();
    }
}

void startTimer(void)
{
    // printf("Timer Start time: %s \n", guiTime);
    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = start_timer_callback,
        .name = "Job Time Counter"};
    esp_timer_handle_t JTCesp_timer_handle; // JTC = Job Time Counter
    esp_timer_create(&esp_timer_create_args, &JTCesp_timer_handle);

    esp_timer_start_once(JTCesp_timer_handle, totalSecond * 1000000);
    // printf("Timer Started for  Min = %d \n", totalSecond / 60);
}

void start_timer_callback(void *args)
{
    if (PumpStopForcefully != true)
    {
        sprintf(stopDateEnd, "%s", guiDate);
        sprintf(stopTimeEnd, "%sH%sM", guiHrDef, guiMinDef);
        vUpdateInfoWidgetTask();
    }

    PumpStopForcefully = false;
}

void infoWidgetUpdateTask_cb(lv_task_t *infoWidgetUpdateTask)
{
    dashboardflg = 2;
    DashboardInfoWidget();
    lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
    lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
}

void vUpdateInfoWidgetTask(void)
{
    infoWidgetUpdateTask = lv_task_create(infoWidgetUpdateTask_cb, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_once(infoWidgetUpdateTask);
}

/**********************
 *    ERROR ASSERT
 **********************/
