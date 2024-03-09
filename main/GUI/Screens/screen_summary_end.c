/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-05
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"
#include "sequenceManagement.h"
#include "sampleManagement.h"
#include "timeManagement.h"

/*********************
 *      DEFINES
 *********************/
#define TAG             "SUMMARY END"
#define SYMBOL_SIGNAL   "\uf012"
#define _xColorBGParas  LV_COLOR_MAKE(0x38, 0x38, 0x38) // LV_COLOR_MAKE(0x5f, 0x61, 0x38)
#define x_align         -78

// Declare Fonts
LV_FONT_DECLARE(signal_20)

// Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)
LV_IMG_DECLARE(cross_icon)
LV_IMG_DECLARE(archiv_icon)
LV_IMG_DECLARE(ok_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void xDrawSummaryHeading(void);
static void xDrawArchHeadNav(void);

static void __xseTimeLabel_se_refr_func(lv_task_t *__xserefresherTask);
static void __xseBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void Valid_BTN_event_handler(lv_obj_t *obj, lv_event_t event);
static void SeqWidgetTriBTN_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t *scrSummaryEnd;
lv_obj_t *xseParentContainer_se;
lv_obj_t *_xseContStatusBar_se;
lv_obj_t *__xseTimeLabel_se;
lv_obj_t *__xseBatteryLabel_se;
lv_obj_t *__xseWifiLabel_se;
lv_obj_t *__xseSignalLabel_se;
lv_obj_t *_xseSummaryParent_se;
lv_obj_t *__xseParaHeadingCont_se;
lv_obj_t *___xseBackArrowLabel_se;
lv_obj_t *___xseSummaryHeadingLbl_se;
lv_obj_t *__xseBaseContainer_se;
lv_obj_t *___xseMark_se;
lv_obj_t *___xseStartTxt_se;
lv_obj_t *___xseStopTxt_se;
lv_obj_t *___xseStartDateTxt_se;
lv_obj_t *___xseStopDateTxt_se;
lv_obj_t *___xseStrtTmCont_se;
lv_obj_t *____xseStrtTmVal_se;
lv_obj_t *___xseStopTmCont_se;
lv_obj_t *____xseStopTmVal_se;
lv_obj_t *_xseSampleNumTxt_se;
lv_obj_t *_xseSampleNumVal_se;
lv_obj_t *_xseContFSP_se;
lv_obj_t *__xseFLPTxtLbl_se;
lv_obj_t *__xseVarFLPTxtLbl_se;
lv_obj_t *_xseContSQNum_se;
lv_obj_t *__xseSQNumLbl_se;
lv_obj_t *__xseVarSQNumLbl_se;
lv_obj_t *_xseContSQDur_se;
lv_obj_t *__xseSQDurLbl_se;
lv_obj_t *__xseVarSQDurLbl_se;
lv_obj_t *_xseContStart_se;
lv_obj_t *__xseStartLbl_se;
lv_obj_t *__xseVarStartLbl_se;
lv_obj_t *_xseContEndBy_se;
lv_obj_t *__xseContEndByLbl_se;
lv_obj_t *__xseVarEndByLbl_se;
lv_obj_t *_xseContStrtPerson_se;
lv_obj_t *__xseStrtPersonLbl_se;
lv_obj_t *__xseVarStrtPersonLbl_se;
lv_obj_t *_xseContStopPerson_se;
lv_obj_t *__xseStopPersonLbl_se;
lv_obj_t *__xseVarStopPersonLbl_se;
lv_obj_t *_xseContProblem_se;
lv_obj_t *__xseProblemLbl_se;
lv_obj_t *__xseVarProblemLbl_se;
lv_obj_t *hor_line;
lv_obj_t *_xseContVolumeCount_se;
lv_obj_t *__xseVolCountLbl_se;
lv_obj_t *_xseStartStopTxtValCont_se;
lv_obj_t *__xseStartTxt_se;
lv_obj_t *__xseStartVal_se;
lv_obj_t *__xseStopTxt_se;
lv_obj_t *__xseStopVal_se;
lv_obj_t *__xseTargetVal_se;
lv_obj_t *__xseTargetTxt_se;
lv_obj_t *__xseEffectiveVal_se;
lv_obj_t *__xseEffectiveTxt_se;
lv_obj_t *__xseVariationVal_se;
lv_obj_t *__xseVariationText_se;
lv_obj_t *hor_line1;
lv_obj_t *_xseContHourCount_se;
lv_obj_t *__xseHourCountLbl_se;
lv_obj_t *_xseHrStartStopTxtValCont_se;
lv_obj_t *__xseHrStartTxt_se;
lv_obj_t *__xseHrStartVal_se;
lv_obj_t *__xseHrStopTxt_se;
lv_obj_t *__xseHrStopVal_se;
lv_obj_t *__xseHrTargetVal_se;
lv_obj_t *__xseHrTargetTxt_se;
lv_obj_t *__xseHrEffectiveVal_se;
lv_obj_t *__xseHrEffectiveTxt_se;
lv_obj_t *__xseHrVariationVal_se;
lv_obj_t *__xseHrVariationText_se;
lv_obj_t *hor_line2;
lv_obj_t *_xseSeque1_se;
lv_obj_t *Seq;
lv_obj_t *_xseSeque2_se;
lv_obj_t *Seq1;
lv_obj_t *_xseBTNCont_se;
lv_obj_t *_xseBTNUSB_se;
lv_obj_t *_xseBTNUSBLbl_se;
lv_obj_t *_xseBTNValid_se;
lv_obj_t *_xseBTNValidLbl_se;

lv_task_t *__xserefresherTask;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
static uint8_t sequence_number[MAXIMUM_NO_OF_SAMPLES] = {0};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void xseSummaryEndScreen(void)
{

    scrSummaryEnd = lv_obj_create(NULL, NULL);
    lv_scr_load(scrSummaryEnd);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    ESP_LOGI(TAG, "Loading Summary end screen");
    xseParentContainer_se = lv_cont_create(scrSummaryEnd, NULL);
    // lv_scr_load(xseParentContainer_se);
    lv_obj_set_size(xseParentContainer_se, 320, 480);
    lv_obj_set_click(xseParentContainer_se, false);
    lv_obj_align(xseParentContainer_se, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(xseParentContainer_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(xseParentContainer_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(xseParentContainer_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    _xseContStatusBar_se = lv_cont_create(xseParentContainer_se, NULL);
    lv_obj_set_size(_xseContStatusBar_se, 320, 35);
    lv_obj_align(_xseContStatusBar_se, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xseContStatusBar_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_xseContStatusBar_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    __xseTimeLabel_se = lv_label_create(_xseContStatusBar_se, NULL);
    lv_obj_align(__xseTimeLabel_se, _xseContStatusBar_se, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__xseTimeLabel_se, guiTime);

    static lv_style_t _xseTimeLabelStyle_se;
    lv_style_init(&_xseTimeLabelStyle_se);
    lv_style_set_text_font(&_xseTimeLabelStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xseTimeLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseTimeLabel_se, LV_LABEL_PART_MAIN, &_xseTimeLabelStyle_se);

    __xserefresherTask = lv_task_create(__xseTimeLabel_se_refr_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create Label for Battery icon
    __xseBatteryLabel_se = lv_label_create(_xseContStatusBar_se, NULL);
    lv_obj_align(__xseBatteryLabel_se, _xseContStatusBar_se, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__xseBatteryLabel_se, get_battery_symbol());

    static lv_style_t _xseBatteryLabelStyle_se;
    lv_style_init(&_xseBatteryLabelStyle_se);
    lv_style_set_text_font(&_xseBatteryLabelStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_xseBatteryLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseBatteryLabel_se, LV_LABEL_PART_MAIN, &_xseBatteryLabelStyle_se);

    // Create Label for Wifi icon
    __xseWifiLabel_se = lv_label_create(_xseContStatusBar_se, NULL);
    lv_obj_align(__xseWifiLabel_se, __xseBatteryLabel_se, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__xseWifiLabel_se, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(__xseWifiLabel_se, true);

    static lv_style_t _xseWifiLabelStyle_se;
    lv_style_init(&_xseWifiLabelStyle_se);
    lv_style_set_text_font(&_xseWifiLabelStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xseWifiLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseWifiLabel_se, LV_LABEL_PART_MAIN, &_xseWifiLabelStyle_se);

    // Create Label for Signal icon
    __xseSignalLabel_se = lv_label_create(_xseContStatusBar_se, NULL);
    lv_obj_align(__xseSignalLabel_se, __xseWifiLabel_se, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__xseSignalLabel_se, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(__xseSignalLabel_se, true);

    static lv_style_t _xseSignalLabelStyle_se;
    lv_style_init(&_xseSignalLabelStyle_se);
    lv_style_set_text_font(&_xseSignalLabelStyle_se, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xseSignalLabelStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__xseSignalLabel_se, LV_LABEL_PART_MAIN, &_xseSignalLabelStyle_se);

    // Create a Sliding page  to put all the Summary in it
    _xseSummaryParent_se = lv_page_create(xseParentContainer_se, NULL);
    lv_obj_set_size(_xseSummaryParent_se, 320, 430);
    lv_obj_align(_xseSummaryParent_se, _xseContStatusBar_se, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);
    lv_page_set_scrollbar_mode(_xseSummaryParent_se, LV_SCROLLBAR_MODE_HIDE);
    lv_obj_set_style_local_bg_color(_xseSummaryParent_se, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_width(_xseSummaryParent_se, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    if (get_archiv_or_summary_screen_stat() == 0)
    {
        xDrawSummaryHeading();
    }
    else
    {
        xDrawArchHeadNav();
    }

    __xseBaseContainer_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_set_size(__xseBaseContainer_se, 300, 550);
    lv_obj_set_click(__xseBaseContainer_se, false);
    lv_obj_set_width(__xseBaseContainer_se, lv_page_get_width_fit(_xseSummaryParent_se));
    lv_obj_align(__xseBaseContainer_se, __xseParaHeadingCont_se, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_set_style_local_bg_color(__xseBaseContainer_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
    lv_obj_set_style_local_border_width(__xseBaseContainer_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Cross icon img
    ___xseMark_se = lv_img_create(__xseBaseContainer_se, NULL);
    /* checking whether to put the cross sign or the ok sign */
    if (bIsSampleRunsWithoutProblem())
        lv_img_set_src(___xseMark_se, &ok_icon);
    else
        lv_img_set_src(___xseMark_se, &cross_icon);

    lv_obj_align(___xseMark_se, __xseBaseContainer_se, LV_ALIGN_IN_TOP_RIGHT, -5, 5);
    // lv_obj_set_style_local_image_recolor_opa(___xseMark_se, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    // lv_obj_set_style_local_image_recolor(___xseMark_se, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    static lv_style_t _xStartStopTxtStyle;
    lv_style_init(&_xStartStopTxtStyle);
    lv_style_set_text_font(&_xStartStopTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xStartStopTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    /* loading the values of the end summary variable */
    sample_summary_t xSampleSummary;
    vGetEndSummaryVariable(&xSampleSummary);

    ___xseStartTxt_se = lv_label_create(__xseBaseContainer_se, NULL);
    lv_obj_align(___xseStartTxt_se, __xseBaseContainer_se, LV_ALIGN_IN_TOP_LEFT, 5, 40);
    lv_label_set_text(___xseStartTxt_se, "START:");
    lv_obj_add_style(___xseStartTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtStyle);

    ___xseStopTxt_se = lv_label_create(__xseBaseContainer_se, NULL);
    lv_obj_align(___xseStopTxt_se, ___xseStartTxt_se, LV_ALIGN_OUT_RIGHT_MID, 120, 0);
    lv_label_set_text(___xseStopTxt_se, "STOP:");
    lv_obj_add_style(___xseStopTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtStyle);

    static lv_style_t _xStartStopDateTxtStyle;
    lv_style_init(&_xStartStopDateTxtStyle);
    lv_style_set_text_font(&_xStartStopDateTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_style_set_text_color(&_xStartStopDateTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    ___xseStartDateTxt_se = lv_label_create(__xseBaseContainer_se, NULL);
    lv_obj_align(___xseStartDateTxt_se, ___xseStartTxt_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(___xseStartDateTxt_se, xSampleSummary.genericsummary.cStartDate); // setting start date
    lv_obj_add_style(___xseStartDateTxt_se, LV_LABEL_PART_MAIN, &_xStartStopDateTxtStyle);

    ___xseStopDateTxt_se = lv_label_create(__xseBaseContainer_se, NULL);
    lv_obj_align(___xseStopDateTxt_se, ___xseStopTxt_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(___xseStopDateTxt_se, xSampleSummary.genericsummary.cStopDate); // entering stop date
    lv_obj_add_style(___xseStopDateTxt_se, LV_LABEL_PART_MAIN, &_xStartStopDateTxtStyle);

    ___xseStrtTmCont_se = lv_cont_create(__xseBaseContainer_se, NULL);
    lv_obj_align(___xseStrtTmCont_se, ___xseStartDateTxt_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(___xseStrtTmCont_se, lv_obj_get_width(___xseStartDateTxt_se), 15);
    lv_obj_set_style_local_bg_color(___xseStrtTmCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas);
    lv_obj_set_style_local_radius(___xseStrtTmCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(___xseStrtTmCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Label for Start Time of sequence
    ____xseStrtTmVal_se = lv_label_create(___xseStrtTmCont_se, NULL);
    lv_obj_align(____xseStrtTmVal_se, ___xseStrtTmCont_se, LV_ALIGN_IN_TOP_RIGHT, -30, 0);
    lv_label_set_text(____xseStrtTmVal_se, xSampleSummary.starttime); // End time for sample
    lv_obj_set_style_local_text_font(____xseStrtTmVal_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(____xseStrtTmVal_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    ___xseStopTmCont_se = lv_cont_create(__xseBaseContainer_se, NULL);
    lv_obj_align(___xseStopTmCont_se, ___xseStopDateTxt_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(___xseStopTmCont_se, lv_obj_get_width(___xseStopDateTxt_se), 15);
    lv_obj_set_style_local_bg_color(___xseStopTmCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas);
    lv_obj_set_style_local_radius(___xseStopTmCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(___xseStopTmCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Label for Stop Time of sequence
    ____xseStopTmVal_se = lv_label_create(___xseStopTmCont_se, NULL);
    lv_obj_align(____xseStopTmVal_se, ___xseStopTmCont_se, LV_ALIGN_IN_TOP_RIGHT, -30, 0);
    lv_label_set_text(____xseStopTmVal_se, xSampleSummary.cStopTime); // enter stop time
    lv_obj_set_style_local_text_font(____xseStopTmVal_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(____xseStopTmVal_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    _xseSampleNumTxt_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseSampleNumTxt_se, ___xseStrtTmCont_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_xseSampleNumTxt_se, "SAMPLE N°");
    lv_obj_set_style_local_text_font(_xseSampleNumTxt_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_xseSampleNumTxt_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    _xseSampleNumVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseSampleNumVal_se, _xseSampleNumTxt_se, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text_fmt(_xseSampleNumVal_se, "%d", uGetCurrentSampleNumber()); // getting sample number
    lv_obj_set_style_local_text_font(_xseSampleNumVal_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_xseSampleNumVal_se, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create style for Fix Text Parameters //+++++++++++++++++++Style+++++++++++++++++++++++++++++++++
    static lv_style_t _xFixTxtStyle_se;
    lv_style_init(&_xFixTxtStyle_se);
    lv_style_set_text_font(&_xFixTxtStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xFixTxtStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    static lv_style_t _xVarTxtStyle_se;
    lv_style_init(&_xVarTxtStyle_se);
    lv_style_set_text_font(&_xVarTxtStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xVarTxtStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    // Create Container to Flow Set Point
    _xseContFSP_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContFSP_se, _xseSampleNumTxt_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_size(_xseContFSP_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContFSP_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContFSP_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContFSP_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // // Create label for flow set point text
    __xseFLPTxtLbl_se = lv_label_create(_xseContFSP_se, NULL);
    lv_obj_align(__xseFLPTxtLbl_se, _xseContFSP_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseFLPTxtLbl_se, "FLOW SET POINT:");
    lv_obj_add_style(__xseFLPTxtLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarFLPTxtLbl_se = lv_label_create(_xseContFSP_se, NULL);
    lv_obj_align(__xseVarFLPTxtLbl_se, _xseContFSP_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0); // # define x_align  -70
    char _xFLP_se[30];
    sprintf(_xFLP_se, "%.1f L/min", xSampleSummary.fFlowSetPoint);
    lv_label_set_text(__xseVarFLPTxtLbl_se, _xFLP_se); // setting the flow point value
    lv_obj_add_style(__xseVarFLPTxtLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for Sequence number
    _xseContSQNum_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContSQNum_se, _xseContFSP_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContSQNum_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContSQNum_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContSQNum_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContSQNum_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create label for Sequence number text
    __xseSQNumLbl_se = lv_label_create(_xseContSQNum_se, NULL);
    lv_obj_align(__xseSQNumLbl_se, _xseContSQNum_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseSQNumLbl_se, "SEQUENCE NUMBER:");
    lv_obj_add_style(__xseSQNumLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    //------------------->
    // Put Var Text label here
    __xseVarSQNumLbl_se = lv_label_create(_xseContSQNum_se, NULL);
    lv_obj_align(__xseVarSQNumLbl_se, _xseContSQNum_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    char _xSQNum_se[30];
    lv_label_set_text(__xseVarSQNumLbl_se, itoa(xSampleSummary.uSequenceNumber, _xSQNum_se, 10)); // setting the sequence number value
    lv_obj_add_style(__xseVarSQNumLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for DURATION Text
    _xseContSQDur_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContSQDur_se, _xseContSQNum_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContSQDur_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContSQDur_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContSQDur_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContSQDur_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create label for DURATION text
    __xseSQDurLbl_se = lv_label_create(_xseContSQDur_se, NULL);
    lv_obj_align(__xseSQDurLbl_se, _xseContSQDur_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseSQDurLbl_se, "DURATION:");
    lv_obj_add_style(__xseSQDurLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarSQDurLbl_se = lv_label_create(_xseContSQDur_se, NULL);
    lv_obj_align(__xseVarSQDurLbl_se, _xseContSQDur_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    lv_label_set_text(__xseVarSQDurLbl_se, xSampleSummary.cDuration); // entering the duration
    lv_obj_add_style(__xseVarSQDurLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for START Text
    _xseContStart_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContStart_se, _xseContSQDur_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContStart_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContStart_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContStart_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContStart_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    __xseStartLbl_se = lv_label_create(_xseContStart_se, NULL);
    lv_obj_align(__xseStartLbl_se, _xseContStart_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseStartLbl_se, "START:");
    lv_obj_add_style(__xseStartLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarStartLbl_se = lv_label_create(_xseContStart_se, NULL);
    lv_obj_align(__xseVarStartLbl_se, _xseContStart_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    lv_label_set_text(__xseVarStartLbl_se, xSampleSummary.cStartPerson); // start person
    lv_obj_add_style(__xseVarStartLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for END BY Text
    _xseContEndBy_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContEndBy_se, _xseContStart_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContEndBy_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContEndBy_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContEndBy_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContEndBy_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for END BY Text
    __xseContEndByLbl_se = lv_label_create(_xseContEndBy_se, NULL);
    lv_obj_align(__xseContEndByLbl_se, _xseContEndBy_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseContEndByLbl_se, "END BY:");
    lv_obj_add_style(__xseContEndByLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarEndByLbl_se = lv_label_create(_xseContEndBy_se, NULL);
    lv_obj_align(__xseVarEndByLbl_se, _xseContEndBy_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    lv_label_set_text(__xseVarEndByLbl_se, xSampleSummary.cEndPerson); // end by person
    lv_obj_add_style(__xseVarEndByLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for START PERSON Text
    _xseContStrtPerson_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContStrtPerson_se, _xseContEndBy_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContStrtPerson_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContStrtPerson_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContStrtPerson_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContStrtPerson_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for START PERSON Text
    __xseStrtPersonLbl_se = lv_label_create(_xseContStrtPerson_se, NULL);
    lv_obj_align(__xseStrtPersonLbl_se, _xseContStrtPerson_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseStrtPersonLbl_se, "START PERSON:");
    lv_obj_add_style(__xseStrtPersonLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarStrtPersonLbl_se = lv_label_create(_xseContStrtPerson_se, NULL);
    lv_obj_align(__xseVarStrtPersonLbl_se, _xseContStrtPerson_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    lv_label_set_text(__xseVarStrtPersonLbl_se, "Mark");
    lv_obj_add_style(__xseVarStrtPersonLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for STOP PERSON Text
    _xseContStopPerson_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContStopPerson_se, _xseContStrtPerson_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContStopPerson_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContStopPerson_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContStopPerson_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContStopPerson_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for STOP PERSON Text
    __xseStopPersonLbl_se = lv_label_create(_xseContStopPerson_se, NULL);
    lv_obj_align(__xseStopPersonLbl_se, _xseContStopPerson_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseStopPersonLbl_se, "STOP PERSON:");
    lv_obj_add_style(__xseStopPersonLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarStopPersonLbl_se = lv_label_create(_xseContStopPerson_se, NULL);
    lv_obj_align(__xseVarStopPersonLbl_se, _xseContStopPerson_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    lv_label_set_text(__xseVarStopPersonLbl_se, "Steve");
    lv_obj_add_style(__xseVarStopPersonLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Create Container for PROBLEM Text
    _xseContProblem_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContProblem_se, _xseContStopPerson_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(_xseContProblem_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContProblem_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContProblem_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContProblem_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for PROBLEM Text
    __xseProblemLbl_se = lv_label_create(_xseContProblem_se, NULL);
    lv_obj_align(__xseProblemLbl_se, _xseContProblem_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseProblemLbl_se, "PROBLEM:");
    lv_obj_add_style(__xseProblemLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    // Put Var Text label here
    __xseVarProblemLbl_se = lv_label_create(_xseContProblem_se, NULL);
    lv_obj_align(__xseVarProblemLbl_se, _xseContProblem_se, LV_ALIGN_IN_RIGHT_MID, x_align, 0);
    lv_label_set_text(__xseVarProblemLbl_se, xSampleSummary.hasProblem); // problem descriptors
    lv_obj_add_style(__xseVarProblemLbl_se, LV_LABEL_PART_MAIN, &_xVarTxtStyle_se);

    // Seprator line
    // Create Horizontal Line
    hor_line = lv_line_create(_xseSummaryParent_se, NULL);
    static lv_point_t p[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line, p, 2);
    lv_obj_align(hor_line, _xseContProblem_se, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    static lv_style_t style_hor_line;
    lv_style_init(&style_hor_line);
    lv_style_set_line_color(&style_hor_line, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
    lv_style_set_line_width(&style_hor_line, LV_STATE_DEFAULT, 2);
    lv_style_set_line_rounded(&style_hor_line, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line, LV_LINE_PART_MAIN, &style_hor_line);

    //=========================================================================================================================
    //                                    VOLUME COUNTER HEADING
    //=========================================================================================================================
    // Create a container for Volume Counter Heading
    _xseContVolumeCount_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContVolumeCount_se, _xseContProblem_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);
    lv_obj_set_size(_xseContVolumeCount_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContVolumeCount_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContVolumeCount_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContVolumeCount_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for VOLUME COUNTER Text
    __xseVolCountLbl_se = lv_label_create(_xseContVolumeCount_se, NULL);
    lv_obj_align(__xseVolCountLbl_se, _xseContVolumeCount_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseVolCountLbl_se, "VOLUME COUNTER:");
    lv_obj_add_style(__xseVolCountLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    //==========================================================================================================================
    //                                TEXT UNDER VOLUME COUNTER HEADING
    //==========================================================================================================================
    // Create Container for Start Stop Text & Value
    _xseStartStopTxtValCont_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseStartStopTxtValCont_se, _xseContVolumeCount_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(_xseStartStopTxtValCont_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseStartStopTxtValCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseStartStopTxtValCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseStartStopTxtValCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // STYLE FOR TEXTS Under _xseStartStopTxtValCont_se Container
    static lv_style_t _xStartStopTxtValStyle_se;
    lv_style_init(&_xStartStopTxtValStyle_se);
    lv_style_set_text_font(&_xStartStopTxtValStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xStartStopTxtValStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    // Create a label for start text
    __xseStartTxt_se = lv_label_create(_xseStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseStartTxt_se, _xseStartStopTxtValCont_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseStartTxt_se, "START:  ");
    lv_obj_add_style(__xseStartTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for start Value
    __xseStartVal_se = lv_label_create(_xseStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseStartVal_se, __xseStartTxt_se, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text_fmt(__xseStartVal_se, "%0.2lf L", xSampleSummary.genericsummary.xVolumeCounter.fStartVolume); // start volume counter
    lv_obj_add_style(__xseStartVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for Stop text
    __xseStopTxt_se = lv_label_create(_xseStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseStopTxt_se, __xseStartVal_se, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_label_set_text(__xseStopTxt_se, "STOP:  ");
    lv_obj_add_style(__xseStopTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for stop Value
    __xseStopVal_se = lv_label_create(_xseStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseStopVal_se, __xseStopTxt_se, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text_fmt(__xseStopVal_se, "%0.2lf L", xSampleSummary.genericsummary.xVolumeCounter.fEndVolume); //  stop vlume counter
    lv_obj_add_style(__xseStopVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for Target Value
    __xseTargetVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseTargetVal_se, _xseStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 10);
    lv_label_set_text_fmt(__xseTargetVal_se, "%0.2lf L", xSampleSummary.genericsummary.xVolumeCounter.fTargetVolume); // target volume counter
    lv_obj_add_style(__xseTargetVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a Target Heading
    __xseTargetTxt_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseTargetTxt_se, __xseTargetVal_se, LV_ALIGN_OUT_BOTTOM_LEFT, -2, 0);
    lv_label_set_text(__xseTargetTxt_se, "TARGET");
    lv_obj_add_style(__xseTargetTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for EFFECTIVE Value
    __xseEffectiveVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseEffectiveVal_se, _xseStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_MID, -10, 10);
    lv_label_set_text_fmt(__xseEffectiveVal_se, "%0.2lf L", xSampleSummary.genericsummary.xVolumeCounter.fEffectiveVolume); // effective volume counter
    lv_obj_add_style(__xseEffectiveVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for EFFECTIVE Text
    __xseEffectiveTxt_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseEffectiveTxt_se, __xseEffectiveVal_se, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 0);
    lv_label_set_text(__xseEffectiveTxt_se, "EFFECTIVE");
    lv_obj_add_style(__xseEffectiveTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for VARIATION Value
    __xseVariationVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseVariationVal_se, _xseStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_RIGHT, -30, 10);
    lv_label_set_text_fmt(__xseVariationVal_se, "%0.2f ", xSampleSummary.genericsummary.xVolumeCounter.fVariation); // variation for the volume
    lv_obj_add_style(__xseVariationVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    lv_obj_t *varPerSymb = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(varPerSymb, __xseVariationVal_se, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text(varPerSymb, "%");
    lv_obj_add_style(varPerSymb, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for VARIATION TEXT
    __xseVariationText_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseVariationText_se, __xseVariationVal_se, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 0);
    lv_label_set_text(__xseVariationText_se, "VARIATION");
    lv_obj_add_style(__xseVariationText_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Seprator line
    // Create Horizontal Line
    hor_line1 = lv_line_create(_xseSummaryParent_se, NULL);
    static lv_point_t p1[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line1, p1, 2);
    lv_obj_align(hor_line1, _xseContProblem_se, LV_ALIGN_OUT_BOTTOM_MID, 0, 130);
    lv_obj_add_style(hor_line1, LV_LINE_PART_MAIN, &style_hor_line);

    // Create a container for HOUR COUNTER Heading
    _xseContHourCount_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseContHourCount_se, _xseStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 80);
    lv_obj_set_size(_xseContHourCount_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseContHourCount_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseContHourCount_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseContHourCount_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label for HOUR COUNTER Text
    __xseHourCountLbl_se = lv_label_create(_xseContHourCount_se, NULL);
    lv_obj_align(__xseHourCountLbl_se, _xseContHourCount_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseHourCountLbl_se, "HOUR COUNTER:");
    lv_obj_add_style(__xseHourCountLbl_se, LV_LABEL_PART_MAIN, &_xFixTxtStyle_se);

    //==========================================================================================================================
    //                                TEXT UNDER Hour COUNTER HEADING
    //==========================================================================================================================
    // Create Container for Start Stop Text & Value
    _xseHrStartStopTxtValCont_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseHrStartStopTxtValCont_se, _xseContHourCount_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(_xseHrStartStopTxtValCont_se, 282, 20);
    lv_obj_set_style_local_bg_color(_xseHrStartStopTxtValCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, _xColorBGParas); //_xColorBGParas
    lv_obj_set_style_local_radius(_xseHrStartStopTxtValCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(_xseHrStartStopTxtValCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create a label for start text
    __xseHrStartTxt_se = lv_label_create(_xseHrStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseHrStartTxt_se, _xseHrStartStopTxtValCont_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(__xseHrStartTxt_se, "START:  ");
    lv_obj_add_style(__xseHrStartTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for start Value
    __xseHrStartVal_se = lv_label_create(_xseHrStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseHrStartVal_se, __xseHrStartTxt_se, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text_fmt(__xseHrStartVal_se, "%0.2fH", xSampleSummary.genericsummary.xHourCounter.fStartHour); // start hour
    lv_obj_add_style(__xseHrStartVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for Stop text
    __xseHrStopTxt_se = lv_label_create(_xseHrStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseHrStopTxt_se, __xseHrStartVal_se, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_label_set_text(__xseHrStopTxt_se, "STOP:  ");
    lv_obj_add_style(__xseHrStopTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for stop Value
    __xseHrStopVal_se = lv_label_create(_xseHrStartStopTxtValCont_se, NULL);
    lv_obj_align(__xseHrStopVal_se, __xseHrStopTxt_se, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text_fmt(__xseHrStopVal_se, "%0.2fH", xSampleSummary.genericsummary.xHourCounter.fEndHour); // end hour
    lv_obj_add_style(__xseHrStopVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for Target Value
    __xseHrTargetVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseHrTargetVal_se, _xseHrStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 10);
    lv_label_set_text_fmt(__xseHrTargetVal_se, "%0.2fH", xSampleSummary.genericsummary.xHourCounter.fTargetHour); // target hour
    lv_obj_add_style(__xseHrTargetVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a Target Heading
    __xseHrTargetTxt_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseHrTargetTxt_se, __xseHrTargetVal_se, LV_ALIGN_OUT_BOTTOM_LEFT, -2, 0);
    lv_label_set_text(__xseHrTargetTxt_se, "TARGET");
    lv_obj_add_style(__xseHrTargetTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for EFFECTIVE Value
    __xseHrEffectiveVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseHrEffectiveVal_se, _xseHrStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_MID, -10, 10);
    lv_label_set_text_fmt(__xseHrEffectiveVal_se, "%0.2fH", xSampleSummary.genericsummary.xHourCounter.fEffectiveHour); // effective hour
    lv_obj_add_style(__xseHrEffectiveVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for EFFECTIVE Text
    __xseHrEffectiveTxt_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseHrEffectiveTxt_se, __xseHrEffectiveVal_se, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 0);
    lv_label_set_text(__xseHrEffectiveTxt_se, "EFFECTIVE");
    lv_obj_add_style(__xseHrEffectiveTxt_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for VARIATION Value
    __xseHrVariationVal_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseHrVariationVal_se, _xseHrStartStopTxtValCont_se, LV_ALIGN_OUT_BOTTOM_RIGHT, -30, 10);
    lv_label_set_text_fmt(__xseHrVariationVal_se, "%0.2f%%", xSampleSummary.genericsummary.xHourCounter.fVariation); // variation in hour  in %age
    lv_obj_add_style(__xseHrVariationVal_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Create a label for VARIATION TEXT
    __xseHrVariationText_se = lv_label_create(_xseSummaryParent_se, NULL);
    lv_obj_align(__xseHrVariationText_se, __xseHrVariationVal_se, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 0);
    lv_label_set_text(__xseHrVariationText_se, "VARIATION");
    lv_obj_add_style(__xseHrVariationText_se, LV_LABEL_PART_MAIN, &_xStartStopTxtValStyle_se);

    // Seprator line
    // Create Horizontal Line
    hor_line2 = lv_line_create(_xseSummaryParent_se, NULL);
    static lv_point_t p2[] = {{-20, 5}, {238, 5}};
    lv_line_set_points(hor_line2, p2, 2);
    lv_obj_align(hor_line2, __xseBaseContainer_se, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    static lv_style_t style_hor_line1;
    lv_style_init(&style_hor_line1);
    lv_style_set_line_color(&style_hor_line1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_width(&style_hor_line1, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line1, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line2, LV_LINE_PART_MAIN, &style_hor_line);

    SequenceWidgetArrange();

    // Button Container
    _xseBTNCont_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_align(_xseBTNCont_se, _xseSeque2_se, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(_xseBTNCont_se, 290, 68);
    lv_obj_set_style_local_bg_color(_xseBTNCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); //_xColorBGParas
    lv_obj_set_style_local_border_width(_xseBTNCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Export USB Button
    _xseBTNUSB_se = lv_btn_create(_xseBTNCont_se, NULL);
    lv_obj_align(_xseBTNUSB_se, _xseBTNCont_se, LV_ALIGN_IN_LEFT_MID, 10, 0);
    lv_obj_set_size(_xseBTNUSB_se, 120, 55);
    // lv_obj_set_event_cb(_xseBTNUSB_se, BTN_event_handler);
    lv_obj_set_style_local_radius(_xseBTNUSB_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_bg_color(_xseBTNUSB_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #Eb3B5A
    lv_obj_set_style_local_bg_color(_xseBTNUSB_se, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // #Eb3B5A
    // lv_style_set_bg_color(&_xssValidJobBtnStyle_ss, LV_STATE_DEFAULT | LV_STATE_PRESSED | LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_width(_xseBTNUSB_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_opa(_xseBTNUSB_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Creat a Export USB Button Label
    _xseBTNUSBLbl_se = lv_label_create(_xseBTNUSB_se, NULL);
    lv_obj_align(_xseBTNUSBLbl_se, _xseBTNUSB_se, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(_xseBTNUSBLbl_se, "EXPORT USB");
    lv_obj_set_style_local_text_font(_xseBTNUSBLbl_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_xseBTNUSBLbl_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create Valid Button
    _xseBTNValid_se = lv_btn_create(_xseBTNCont_se, NULL);
    lv_obj_align(_xseBTNValid_se, _xseBTNUSB_se, LV_ALIGN_OUT_RIGHT_TOP, 30, 0);
    lv_obj_set_size(_xseBTNValid_se, 120, 55);
    // lv_obj_set_event_cb(_xseBTNUSB_se, BTN_event_handler);
    lv_obj_set_event_cb(_xseBTNValid_se, Valid_BTN_event_handler);
    lv_obj_set_style_local_radius(_xseBTNValid_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_bg_color(_xseBTNValid_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48)); // #Eb3B5A
    lv_obj_set_style_local_border_width(_xseBTNValid_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_opa(_xseBTNValid_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Creat a Valid Button Label
    _xseBTNValidLbl_se = lv_label_create(_xseBTNValid_se, NULL);
    lv_obj_align(_xseBTNValidLbl_se, _xseBTNValid_se, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(_xseBTNValidLbl_se, "VALID");
    lv_obj_set_style_local_text_font(_xseBTNValidLbl_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_xseBTNValidLbl_se, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    crnt_screen = scrSummaryEnd; // scrSummaryEnd
    screenid = SCR_SUMMARY_END;
}

static void xDrawSummaryHeading(void)
{
    // Crate a container to contain Summary Start Header
    __xseParaHeadingCont_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_set_size(__xseParaHeadingCont_se, 300, 70);
    lv_obj_set_width(__xseParaHeadingCont_se, lv_page_get_width_fit(_xseSummaryParent_se));
    lv_obj_align(__xseParaHeadingCont_se, _xseSummaryParent_se, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_bg_color(__xseParaHeadingCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(__xseParaHeadingCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Black Arrow Container
    lv_obj_t *_xseBackArrowContainer;
    _xseBackArrowContainer = lv_cont_create(__xseParaHeadingCont_se, NULL);
    lv_obj_set_size(_xseBackArrowContainer, 60, 60);
    lv_obj_set_width(_xseBackArrowContainer, lv_page_get_width_fit(_xseSummaryParent_se));
    lv_obj_align(_xseBackArrowContainer, __xseParaHeadingCont_se, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xseBackArrowContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xseBackArrowContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_xseBackArrowContainer, __xseBackArrow_event_handler);

    // Create Back arrow img
    ___xseBackArrowLabel_se = lv_img_create(_xseBackArrowContainer, NULL);
    lv_img_set_src(___xseBackArrowLabel_se, &left_arrow_icon);
    lv_obj_align(___xseBackArrowLabel_se, _xseBackArrowContainer, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(___xseBackArrowLabel_se, true);
    lv_obj_set_style_local_image_recolor_opa(___xseBackArrowLabel_se, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(___xseBackArrowLabel_se, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(___xseBackArrowLabel_se, __xseBackArrow_event_handler);

    // Create Label for Sequences "Heading"
    ___xseSummaryHeadingLbl_se = lv_label_create(__xseParaHeadingCont_se, NULL);
    lv_obj_align(___xseSummaryHeadingLbl_se, __xseParaHeadingCont_se, LV_ALIGN_IN_BOTTOM_MID, -60, -35);
    lv_label_set_text(___xseSummaryHeadingLbl_se, "   Summary  ");

    static lv_style_t _xseParameterHeadingStyle_se;
    lv_style_init(&_xseParameterHeadingStyle_se);
    lv_style_set_text_font(&_xseParameterHeadingStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&_xseParameterHeadingStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(___xseSummaryHeadingLbl_se, LV_LABEL_PART_MAIN, &_xseParameterHeadingStyle_se);
}

/* Create the heading for the archive screen (Summary) */
static void xDrawArchHeadNav(void)
{
    __xseParaHeadingCont_se = lv_cont_create(_xseSummaryParent_se, NULL);
    lv_obj_set_size(__xseParaHeadingCont_se, 300, 130);
    lv_obj_set_width(__xseParaHeadingCont_se, lv_page_get_width_fit(_xseSummaryParent_se));
    lv_obj_align(__xseParaHeadingCont_se, _xseSummaryParent_se, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_bg_color(__xseParaHeadingCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(__xseParaHeadingCont_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Black Arrow Container
    lv_obj_t *_xseBackArrowContainer;
    _xseBackArrowContainer = lv_cont_create(__xseParaHeadingCont_se, NULL);
    lv_obj_set_size(_xseBackArrowContainer, 60, 60);
    lv_obj_set_width(_xseBackArrowContainer, lv_page_get_width_fit(_xseSummaryParent_se));
    lv_obj_align(_xseBackArrowContainer, __xseParaHeadingCont_se, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_set_style_local_bg_color(_xseBackArrowContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xseBackArrowContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_xseBackArrowContainer, __xseBackArrow_event_handler);

    // Create Back arrow img
    ___xseBackArrowLabel_se = lv_img_create(_xseBackArrowContainer, NULL);
    lv_img_set_src(___xseBackArrowLabel_se, &left_arrow_icon);
    lv_obj_align(___xseBackArrowLabel_se, _xseBackArrowContainer, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(___xseBackArrowLabel_se, true);
    lv_obj_set_style_local_image_recolor_opa(___xseBackArrowLabel_se, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(___xseBackArrowLabel_se, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(___xseBackArrowLabel_se, __xseBackArrow_event_handler);

    // Create Label for Sequences "Heading"
    ___xseSummaryHeadingLbl_se = lv_label_create(__xseParaHeadingCont_se, NULL);
    lv_obj_align(___xseSummaryHeadingLbl_se, __xseParaHeadingCont_se, LV_ALIGN_IN_TOP_MID, -40, 10);
    lv_label_set_text(___xseSummaryHeadingLbl_se, "   Archiv  ");

    static lv_style_t _xseParameterHeadingStyle_se;
    lv_style_init(&_xseParameterHeadingStyle_se);
    lv_style_set_text_font(&_xseParameterHeadingStyle_se, LV_STATE_DEFAULT, &lv_font_montserrat_26); // signal_20
    lv_style_set_text_color(&_xseParameterHeadingStyle_se, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(___xseSummaryHeadingLbl_se, LV_LABEL_PART_MAIN, &_xseParameterHeadingStyle_se);

    lv_obj_t *xArchieveLogo;
    xArchieveLogo = lv_img_create(__xseParaHeadingCont_se, NULL);
    lv_img_set_src(xArchieveLogo, &archiv_icon);
    lv_obj_align(xArchieveLogo, __xseParaHeadingCont_se, LV_ALIGN_IN_TOP_RIGHT, -30, 10);
    lv_obj_set_click(xArchieveLogo, true);
    lv_obj_set_style_local_image_recolor_opa(xArchieveLogo, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(xArchieveLogo, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    lv_obj_t *_xseContNavBTNSamNArch;
    _xseContNavBTNSamNArch = lv_cont_create(__xseParaHeadingCont_se, NULL);
    lv_obj_set_size(_xseContNavBTNSamNArch, 320, 60);
    // lv_obj_set_width(_xseBackArrowContainerArch, lv_page_get_width_fit(xseParentContainer_seArch));
    lv_obj_align(_xseContNavBTNSamNArch, __xseParaHeadingCont_se, LV_ALIGN_IN_TOP_LEFT, 0, 65);
    lv_obj_set_style_local_bg_color(_xseContNavBTNSamNArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xseContNavBTNSamNArch, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_t *xPrevNavBTNArch;
    xPrevNavBTNArch = lv_label_create(_xseContNavBTNSamNArch, NULL);
    lv_obj_align(xPrevNavBTNArch, _xseContNavBTNSamNArch, LV_ALIGN_IN_TOP_LEFT, 10, 2);
    lv_label_set_text(xPrevNavBTNArch, LV_SYMBOL_PREV);
    lv_obj_set_style_local_text_font(xPrevNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(xPrevNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *xNextNavBTNArch;
    xNextNavBTNArch = lv_label_create(_xseContNavBTNSamNArch, NULL);
    lv_obj_align(xNextNavBTNArch, _xseContNavBTNSamNArch, LV_ALIGN_IN_TOP_RIGHT, -30, 2);
    lv_label_set_text(xNextNavBTNArch, LV_SYMBOL_NEXT);
    lv_obj_set_style_local_text_font(xNextNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(xNextNavBTNArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *xSampleNoLblArch;
    xSampleNoLblArch = lv_label_create(_xseContNavBTNSamNArch, NULL);
    lv_obj_align(xSampleNoLblArch, _xseContNavBTNSamNArch, LV_ALIGN_IN_TOP_MID, -20, 2);
    lv_label_set_text(xSampleNoLblArch, "Sample \n N°1002");
    lv_obj_set_style_local_text_font(xSampleNoLblArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(xSampleNoLblArch, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void SequenceWidgetArrange(void)
{
    lv_obj_t *curr_obj = __xseBaseContainer_se;
    sequence_t *sequencedata = get_sequence_array();
    uint8_t totalseq = get_no_of_sequence_in_array();
    memset(&sequence_number, 0x00, sizeof(sequence_number));
    for (uint8_t i = 0; i < totalseq; i++)
    {
        _xseSeque1_se = lv_cont_create(_xseSummaryParent_se, NULL);
        lv_obj_align(_xseSeque1_se, curr_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
        lv_obj_set_size(_xseSeque1_se, 290, 100);
        lv_obj_set_style_local_bg_color(_xseSeque1_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); //_xColorBGParas
        lv_obj_set_style_local_border_width(_xseSeque1_se, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        /*this will create the widget with the data of the sequence */
        sequence_number[i] = i+1;
        Seq = sqCreateSequence(_xseSeque1_se, &sequence_number[i]);
        lv_obj_align(Seq, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_local_bg_color(Seq, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38)); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
        lv_obj_set_style_local_border_opa(Seq, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);
        sqSquenceNum(Seq, i + 1);
        sqSetFlowSetPoint(Seq, sequencedata[i].fFlowSetPoint); 
        sqSetDuration(Seq, sequencedata[i].uDurationHour);     
        if (sequencedata[i].bSucessfullyRun)
        {
            sqSetProblem(Seq, "NONE");
            sqSetBtnColor(Seq, STATE_OK);
        }
        else
        {
            sqSetProblem(Seq, "PROBLEM");
            sqSetBtnColor(Seq, STATE_NOTOK);
        }

        SeqWigetBTNEventCallback(Seq, SeqWidgetTriBTN_event_handler);
        curr_obj = _xseSeque1_se;
    }
}

static void __xseTimeLabel_se_refr_func(lv_task_t *__xserefresherTask)
{
    if (lv_obj_get_screen(__xseTimeLabel_se) == lv_scr_act())
    {
        lv_label_set_text(__xseTimeLabel_se, guiTime);
        lv_label_set_text(__xseBatteryLabel_se, get_battery_symbol());
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void __xseBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(__xserefresherTask);
        __xserefresherTask = NULL;
        global_DashbordBTNflag = 1;
        set_archiv_or_summary_screen(0);
        xsPresetScreenAdvance();
    }
}

static void Valid_BTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(__xserefresherTask);
        __xserefresherTask = NULL;
        dashboardflg = 0;
        set_archiv_or_summary_screen(0);
        pxDashboardScreen();
    }
}

static void SeqWidgetTriBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        sequenceSummary_t xSequnceSummary;
        uint8_t seqnumber = *(uint8_t*)lv_obj_get_user_data(obj);

        /* if sample exits in the database then show */
        if (database_get_sequence_summary(uGetCurrentSampleNumber(), seqnumber, &xSequnceSummary))
        {
            lv_task_del(__xserefresherTask);
            __xserefresherTask = NULL;
            vSetSampleNumberAndSequnceNumberSampleSummary(uGetCurrentSampleNumber(), seqnumber, &xSequnceSummary);
            sssSummarySampleScreen();
        }
        else
        {
            printf("Error in getting the sequence data\n");
        }
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
