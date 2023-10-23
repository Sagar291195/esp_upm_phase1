/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-10
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
#define TAG             "SUMMARY SAMPLE"
#define SYMBOL_SIGNAL   "\uf012"

// Declare Fonts
LV_FONT_DECLARE(signal_20)

// Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)
LV_IMG_DECLARE(cross_icon)
LV_IMG_DECLARE(ok_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void __sssTimeLabel_refr_func(lv_task_t *__sssrefresherTask);
static void __sssBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void QuitBTN_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t *scrSummarySample;
lv_obj_t *sssParentContainer;
lv_obj_t *_sssContStatusBar;
lv_obj_t *__sssTimeLabel;
lv_obj_t *__sssBatteryLabel;
lv_obj_t *__sssWifiLabel;
lv_obj_t *__sssSignalLabel;
lv_obj_t *_sssSummaryParentScroll;
lv_obj_t *__sssParaHeadingCont;
lv_obj_t *___sssBackArrowLabel;
lv_obj_t *___sssSummaryHeadingLbl;
lv_obj_t *__sssSequenceNumCont;
lv_obj_t *___sssSequenceTxt;
lv_obj_t *___sssSlash;
lv_obj_t *___sssTotalSeq;
lv_obj_t *___sssCurrentSeq;
lv_obj_t *__sssSeqSumCont;
lv_obj_t *___sssStatusMark_SeqSum;
lv_obj_t *___sssSeqSummarryText;
lv_obj_t *___sssStrtTxt;
lv_obj_t *___sssStpTxt;
lv_obj_t *___sssStrtVal;
lv_obj_t *___sssStpVal;
lv_obj_t *___sssVolCountTxt;
lv_obj_t *___sssSTSPTxtValCont;
lv_obj_t *___sssStrtBelowTxt;
lv_obj_t *___sssStrtBelowVal;
lv_obj_t *___sssStpBelowTxt;
lv_obj_t *___sssStpBelowVal;
lv_obj_t *___sssTarEffVarValCont;
lv_obj_t *___sssTargetVal;
lv_obj_t *___sssEffectiveVal;
lv_obj_t *___sssVariationVal;
lv_obj_t *___sssTargetTxt;
lv_obj_t *___sssEffectiveTxt;
lv_obj_t *___sssVariationTxt;
lv_obj_t *hor_line;
lv_obj_t *___sssHorCountTxt;
lv_obj_t *___sssSTSPTxt2ValCont;
lv_obj_t *___sssStrtBelowTxt2;
lv_obj_t *___sssStrtBelowVal2;
lv_obj_t *___sssStpBelowTxt2;
lv_obj_t *___sssStpBelowVal2;
lv_obj_t *___sssTarEffVarValContHC;
lv_obj_t *___sssTargetVal2;
lv_obj_t *___sssEffectiveVal2;
lv_obj_t *___sssVariationVal2;
lv_obj_t *___sssTargetTxt2;
lv_obj_t *___sssEffectiveTxt2;
lv_obj_t *___sssVariationTxt2;
lv_obj_t *___sssAirflowVolCont;
lv_obj_t *___sssStatusMark_AFV;
lv_obj_t *___sssAirFlowTxtLbl;
lv_obj_t *_sssSetPtTxt_AFV;
lv_obj_t *_sssSetPtVar_AFV;
lv_obj_t *_sssMeanTxt_AFV;
lv_obj_t *_sssMeanVar_AFV;
lv_obj_t *_sssMaxValTxt_AFV;
lv_obj_t *_sssUpdownSignal_AFV;
lv_obj_t *_sssMaxValVar_AFV;
lv_obj_t *_sssMinValTxt_AFV;
lv_obj_t *_sssUpdownSignal2_AFV;
lv_obj_t *_sssMinValVar_AFV;
lv_obj_t *_sssVariationTxt_AFV;
lv_obj_t *_sssPercentSymbol_AFV;
lv_obj_t *_sssVariationVal_AFV;
lv_obj_t *___sssAmbTempCont;
lv_obj_t *___sssStatusMark_AT;
lv_obj_t *___sssAmbTempTxtLbl;
lv_obj_t *_sssLIMITTxt_AT;
lv_obj_t *_sssLimitLower_AT;
lv_obj_t *_ssstoTxt_AT;
lv_obj_t *_sssLimitllast_AT;
lv_obj_t *_sssMeanTxt_AT;
lv_obj_t *_sssMeanVar_AT;
lv_obj_t *_sssMaxValTxt_AT;
lv_obj_t *_sssUpdownSignal_AT;
lv_obj_t *_sssMaxValVar_AT;
lv_obj_t *_sssMinValTxt_AT;
lv_obj_t *_sssUpdownSignal2_AT;
lv_obj_t *_sssMinValVar_AT;
lv_obj_t *_sssVariationTxt_AT;
lv_obj_t *_sssPercentSymbol_AT;
lv_obj_t *_sssVariationVal_AT;
lv_obj_t *___sssAmbPressCont;
lv_obj_t *___sssStatusMark_AP;
lv_obj_t *___sssAmbPressTxtLbl;
lv_obj_t *_sssLIMITTxt_AP;
lv_obj_t *_sssLimitLower_AP;
lv_obj_t *_ssstoTxt_AP;
lv_obj_t *_sssLimitllast_AP;
lv_obj_t *_sssMeanTxt_AP;
lv_obj_t *_sssMeanVar_AP;
lv_obj_t *_sssMaxValTxt_AP;
lv_obj_t *_sssUpdownSignal_AP;
lv_obj_t *_sssMaxValVar_AP;
lv_obj_t *_sssMinValTxt_AP;
lv_obj_t *_sssUpdownSignal2_AP;
lv_obj_t *_sssMinValVar_AP;
lv_obj_t *_sssVariationTxt_AP;
lv_obj_t *_sssPercentSymbol_AP;
lv_obj_t *_sssVariationVal_AP;
lv_obj_t *___sssAmbHumiCont;
lv_obj_t *___sssStatusMark_AH;
lv_obj_t *___sssAmbHumiTxtLbl;
lv_obj_t *_sssLIMITTxt_AH;
lv_obj_t *_sssLimitLower_AH;
lv_obj_t *_ssstoTxt_AH;
lv_obj_t *_sssLimitllast_AH;
lv_obj_t *_sssMeanTxt_AH;
lv_obj_t *_sssMeanVar_AH;
lv_obj_t *_sssMaxValTxt_AH;
lv_obj_t *_sssUpdownSignal_AH;
lv_obj_t *_sssMaxValVar_AH;
lv_obj_t *_sssMinValTxt_AH;
lv_obj_t *_sssUpdownSignal2_AH;
lv_obj_t *_sssMinValVar_AH;
lv_obj_t *_sssVariationTxt_AH;
lv_obj_t *_sssPercentSymbol_AH;
lv_obj_t *_sssVariationVal_AH;
lv_obj_t *___sssHeadLossCont;
lv_obj_t *___sssStatusMark_HL;
lv_obj_t *___sssHeadLossTxtLbl;
lv_obj_t *_sssLIMITTxt_HL;
lv_obj_t *_sssLimitLower_HL;
lv_obj_t *_ssstoTxt_HL;
lv_obj_t *_sssLimitllast_HL;
lv_obj_t *_sssMeanTxt_HL;
lv_obj_t *_sssMeanVar_HL;
lv_obj_t *_sssMaxValTxt_HL;
lv_obj_t *_sssUpdownSignal_HL;
lv_obj_t *_sssMaxValVar_HL;
lv_obj_t *_sssMinValTxt_HL;
lv_obj_t *_sssUpdownSignal2_HL;
lv_obj_t *_sssMinValVar_HL;
lv_obj_t *_sssVariationTxt_HL;
lv_obj_t *_sssPercentSymbol_HL;
lv_obj_t *_sssVariationVal_HL;
lv_obj_t *_sssQuitBtn;
lv_obj_t *_sssQuitBtnLabel;
lv_obj_t *_sssBackArrowCont;

lv_task_t *__sssrefresherTask;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
static uint32_t uSampleNumber = 0;      /* sample data to be get from the database */
static uint8_t uSequnceNumber = 0;      /* sequnce data to be get from the database */\
sequenceSummary_t xSequenceSummary;     /* data variable to store the data from the database */

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void sssSummarySampleScreen(void)
{
        scrSummarySample = lv_obj_create(NULL, NULL);
        lv_scr_load(scrSummarySample);
        if (crnt_screen != NULL)
        {
                lv_obj_del(crnt_screen);
                crnt_screen = NULL;
        }

        ESP_LOGI(TAG, "Loading Summary Sample screen");

        sssParentContainer = lv_cont_create(scrSummarySample, NULL);
        // lv_scr_load(sssParentContainer);
        lv_obj_set_size(sssParentContainer, 320, 480);
        lv_obj_set_click(sssParentContainer, false);
        lv_obj_align(sssParentContainer, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_local_bg_color(sssParentContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
        lv_obj_set_style_local_border_opa(sssParentContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_radius(sssParentContainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status
        _sssContStatusBar = lv_cont_create(sssParentContainer, NULL);
        lv_obj_set_size(_sssContStatusBar, 320, 35);
        lv_obj_align(_sssContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_set_style_local_bg_color(_sssContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
        lv_obj_set_style_local_border_opa(_sssContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

        // Create Watch upper left corner
        __sssTimeLabel = lv_label_create(_sssContStatusBar, NULL);
        lv_obj_align(__sssTimeLabel, _sssContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
        lv_label_set_text(__sssTimeLabel, guiTime);

        static lv_style_t _sssTimeLabelStyle;
        lv_style_init(&_sssTimeLabelStyle);
        lv_style_set_text_font(&_sssTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
        lv_style_set_text_color(&_sssTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(__sssTimeLabel, LV_LABEL_PART_MAIN, &_sssTimeLabelStyle);
        __sssrefresherTask = lv_task_create(__sssTimeLabel_refr_func, 1000, LV_TASK_PRIO_LOW, NULL);

        // Create Label for Battery icon
        __sssBatteryLabel = lv_label_create(_sssContStatusBar, NULL);
        lv_obj_align(__sssBatteryLabel, _sssContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
        lv_label_set_text(__sssBatteryLabel, LV_SYMBOL_BATTERY_FULL); // LV_SYMBOL_BATTERY_FULL

        static lv_style_t _sssBatteryLabelStyle;
        lv_style_init(&_sssBatteryLabelStyle);
        lv_style_set_text_font(&_sssBatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
        lv_style_set_text_color(&_sssBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(__sssBatteryLabel, LV_LABEL_PART_MAIN, &_sssBatteryLabelStyle);

        // Create Label for Wifi icon
        __sssWifiLabel = lv_label_create(_sssContStatusBar, NULL);
        lv_obj_align(__sssWifiLabel, __sssBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
        lv_label_set_text(__sssWifiLabel, LV_SYMBOL_WIFI);

        static lv_style_t _sssWifiLabelStyle;
        lv_style_init(&_sssWifiLabelStyle);
        lv_style_set_text_font(&_sssWifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
        lv_style_set_text_color(&_sssWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(__sssWifiLabel, LV_LABEL_PART_MAIN, &_sssWifiLabelStyle);

        // Create Label for Signal icon
        __sssSignalLabel = lv_label_create(_sssContStatusBar, NULL);
        lv_obj_align(__sssSignalLabel, __sssWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
        lv_label_set_text(__sssSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

        static lv_style_t _sssSignalLabelStyle;
        lv_style_init(&_sssSignalLabelStyle);
        lv_style_set_text_font(&_sssSignalLabelStyle, LV_STATE_DEFAULT, &signal_20); // signal_20
        lv_style_set_text_color(&_sssSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(__sssSignalLabel, LV_LABEL_PART_MAIN, &_sssSignalLabelStyle);

        // Create a Sliding page  to put all the Summary in it
        _sssSummaryParentScroll = lv_page_create(sssParentContainer, NULL);
        lv_obj_set_size(_sssSummaryParentScroll, 320, 430);
        lv_obj_align(_sssSummaryParentScroll, _sssContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);
        lv_page_set_scrollbar_mode(_sssSummaryParentScroll, LV_SCROLLBAR_MODE_HIDE);
        lv_obj_set_style_local_bg_color(_sssSummaryParentScroll, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
        lv_obj_set_style_local_border_width(_sssSummaryParentScroll, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

        // Crate a container to contain Summary Start Header
        __sssParaHeadingCont = lv_cont_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(__sssParaHeadingCont, 300, 70);
        lv_obj_set_width(__sssParaHeadingCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(__sssParaHeadingCont, _sssSummaryParentScroll, LV_ALIGN_IN_TOP_MID, 0, 2);
        lv_obj_set_style_local_bg_color(__sssParaHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
        lv_obj_set_style_local_border_width(__sssParaHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Back Arrow Container
        _sssBackArrowCont = lv_cont_create(__sssParaHeadingCont, NULL);
        lv_obj_set_size(_sssBackArrowCont, 60, 60);
        // lv_obj_set_width(_sssBackArrowCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(_sssBackArrowCont, __sssParaHeadingCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_obj_set_style_local_bg_color(_sssBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
        lv_obj_set_style_local_border_width(_sssBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_event_cb(_sssBackArrowCont, __sssBackArrow_event_handler);

        // Create Back arrow img
        ___sssBackArrowLabel = lv_img_create(_sssBackArrowCont, NULL);
        lv_img_set_src(___sssBackArrowLabel, &left_arrow_icon);
        lv_obj_align(___sssBackArrowLabel, __sssParaHeadingCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_obj_set_click(___sssBackArrowLabel, true);
        lv_obj_set_style_local_image_recolor_opa(___sssBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
        lv_obj_set_style_local_image_recolor(___sssBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_event_cb(___sssBackArrowLabel, __sssBackArrow_event_handler);

        // Create Label for Sequences "Heading"
        ___sssSummaryHeadingLbl = lv_label_create(__sssParaHeadingCont, NULL);
        lv_obj_align(___sssSummaryHeadingLbl, __sssParaHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -60, -35);
        lv_label_set_text_fmt(___sssSummaryHeadingLbl, "Sample N°%d", uSampleNumber); // setting the sample number here

        static lv_style_t _sssParameterHeadingStyle;
        lv_style_init(&_sssParameterHeadingStyle);
        lv_style_set_text_font(&_sssParameterHeadingStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
        lv_style_set_text_color(&_sssParameterHeadingStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(___sssSummaryHeadingLbl, LV_LABEL_PART_MAIN, &_sssParameterHeadingStyle);

        // Create a container for sequence and Sequence Number
        __sssSequenceNumCont = lv_cont_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(__sssSequenceNumCont, 300, 70);
        lv_obj_set_width(__sssSequenceNumCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(__sssSequenceNumCont, __sssParaHeadingCont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_set_style_local_bg_color(__sssSequenceNumCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
        lv_obj_set_style_local_border_width(__sssSequenceNumCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Label for Sequences "Heading"
        ___sssSequenceTxt = lv_label_create(__sssSequenceNumCont, NULL);
        lv_obj_align(___sssSequenceTxt, __sssSequenceNumCont, LV_ALIGN_IN_TOP_MID, -40, 0);
        lv_label_set_text(___sssSequenceTxt, "Sequence");

        static lv_style_t _sssSequenceTxtStyle;
        lv_style_init(&_sssSequenceTxtStyle);
        lv_style_set_text_font(&_sssSequenceTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
        lv_style_set_text_color(&_sssSequenceTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(___sssSequenceTxt, LV_LABEL_PART_MAIN, &_sssSequenceTxtStyle);

        // Create label for '/' between total sequences & current sequences
        ___sssSlash = lv_label_create(__sssSequenceNumCont, NULL);
        lv_obj_align(___sssSlash, ___sssSequenceTxt, LV_ALIGN_OUT_BOTTOM_MID, 5, 2);
        lv_label_set_text(___sssSlash, "/");

        static lv_style_t _sssSlashStyle;
        lv_style_init(&_sssSlashStyle);
        lv_style_set_text_font(&_sssSlashStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
        lv_style_set_text_color(&_sssSlashStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
        lv_obj_add_style(___sssSlash, LV_LABEL_PART_MAIN, &_sssSlashStyle);

        ___sssTotalSeq = lv_label_create(__sssSequenceNumCont, NULL);
        lv_obj_align(___sssTotalSeq, ___sssSlash, LV_ALIGN_OUT_LEFT_TOP, 0, 0);
        char buff[10];
        itoa(get_no_of_sequence_in_array(), buff, 10);
        lv_label_set_text_fmt(___sssTotalSeq, "%s", buff);

        static lv_style_t _sssTotalSeqStyle;
        lv_style_init(&_sssTotalSeqStyle);
        lv_style_set_text_font(&_sssTotalSeqStyle, LV_STATE_DEFAULT, &lv_font_montserrat_18); // signal_20
        lv_style_set_text_color(&_sssTotalSeqStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(___sssTotalSeq, LV_LABEL_PART_MAIN, &_sssTotalSeqStyle);

        ___sssCurrentSeq = lv_label_create(__sssSequenceNumCont, NULL);
        lv_obj_align(___sssCurrentSeq, ___sssSlash, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
        itoa(uSequnceNumber, buff, 10);
        lv_label_set_text_fmt(___sssCurrentSeq, "%s", buff); // seting the current sequence number here

        static lv_style_t _sssCurrentStyle;
        lv_style_init(&_sssCurrentStyle);
        lv_style_set_text_font(&_sssCurrentStyle, LV_STATE_DEFAULT, &lv_font_montserrat_18); // signal_20
        lv_style_set_text_color(&_sssCurrentStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
        lv_obj_add_style(___sssCurrentSeq, LV_LABEL_PART_MAIN, &_sssCurrentStyle);

        //==============================================================================================================
        //                                 SEQUENCE SUMMARY
        //==============================================================================================================
        __sssSeqSumCont = lv_cont_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(__sssSeqSumCont, 300, 300);
        lv_obj_set_click(__sssSeqSumCont, false);
        lv_obj_set_width(__sssSeqSumCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(__sssSeqSumCont, __sssSequenceNumCont, LV_ALIGN_OUT_BOTTOM_MID, 0, -15);
        lv_obj_set_style_local_bg_color(__sssSeqSumCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(__sssSeqSumCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Status icon img
        ___sssStatusMark_SeqSum = lv_img_create(__sssSeqSumCont, NULL);
        /* checking if sequnece run sucessfully and set the ok or cross sign accordingly */
        sequence_t *pSeq = get_sequencedata(uSequnceNumber);
        if (pSeq->bSucessfullyRun)
        {
                lv_img_set_src(___sssStatusMark_SeqSum, &ok_icon);
        }
        else
                lv_img_set_src(___sssStatusMark_SeqSum, &cross_icon);
        lv_obj_align(___sssStatusMark_SeqSum, __sssSeqSumCont, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

        static lv_style_t _sssBlueHeadingTxtStyle;
        lv_style_init(&_sssBlueHeadingTxtStyle);
        lv_style_set_text_font(&_sssBlueHeadingTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
        lv_style_set_text_color(&_sssBlueHeadingTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
        // lv_obj_add_style(___sssCurrentSeq, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        ___sssSeqSummarryText = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssSeqSummarryText, __sssSeqSumCont, LV_ALIGN_IN_TOP_LEFT, 5, 8);
        lv_label_set_text(___sssSeqSummarryText, "SEQUENCE SUMMARY");
        lv_obj_add_style(___sssSeqSummarryText, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        static lv_style_t ___sssStrtStpTxtStyle;
        lv_style_init(&___sssStrtStpTxtStyle);
        lv_style_set_text_font(&___sssStrtStpTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12); // signal_20
        lv_style_set_text_color(&___sssStrtStpTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

        ___sssStrtTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssStrtTxt, ___sssSeqSummarryText, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
        lv_label_set_text(___sssStrtTxt, "Start:");
        lv_obj_add_style(___sssStrtTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStpTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssStpTxt, ___sssStrtTxt, LV_ALIGN_OUT_RIGHT_MID, 120, 0);
        lv_label_set_text(___sssStpTxt, "Stop:");
        lv_obj_add_style(___sssStpTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        static lv_style_t ___sssStrtStpValStyle;
        lv_style_init(&___sssStrtStpValStyle);
        lv_style_set_text_font(&___sssStrtStpValStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16); // signal_20
        lv_style_set_text_color(&___sssStrtStpValStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

        ___sssStrtVal = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssStrtVal, ___sssStrtTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
        lv_label_set_text_fmt(___sssStrtVal, "%s", xSequenceSummary.summary.cStartDate); // setting the start date
        lv_obj_add_style(___sssStrtVal, LV_LABEL_PART_MAIN, &___sssStrtStpValStyle);

        ___sssStpVal = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssStpVal, ___sssStpTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
        lv_label_set_text_fmt(___sssStpVal, "%s", xSequenceSummary.summary.cStopDate); // setting the stop date
        lv_obj_add_style(___sssStpVal, LV_LABEL_PART_MAIN, &___sssStrtStpValStyle);

        ___sssVolCountTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssVolCountTxt, ___sssStrtVal, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
        lv_label_set_text(___sssVolCountTxt, "Volume COUNTER");
        lv_obj_add_style(___sssVolCountTxt, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        ___sssSTSPTxtValCont = lv_cont_create(__sssSeqSumCont, NULL);
        lv_obj_set_size(___sssSTSPTxtValCont, 280, 20);
        lv_obj_set_click(___sssSTSPTxtValCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssSTSPTxtValCont, ___sssVolCountTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        lv_obj_set_style_local_bg_color(___sssSTSPTxtValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssSTSPTxtValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        ___sssStrtBelowTxt = lv_label_create(___sssSTSPTxtValCont, NULL);
        lv_obj_align(___sssStrtBelowTxt, ___sssSTSPTxtValCont, LV_ALIGN_IN_LEFT_MID, 0, 5);
        lv_label_set_text(___sssStrtBelowTxt, "Start:");
        lv_obj_add_style(___sssStrtBelowTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStrtBelowVal = lv_label_create(___sssSTSPTxtValCont, NULL);
        lv_obj_align(___sssStrtBelowVal, ___sssStrtBelowTxt, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
        lv_label_set_text_fmt(___sssStrtBelowVal, "%0.2lf L", xSequenceSummary.summary.xVolumeCounter.fStartVolume); // sequence start volume counter
        lv_obj_add_style(___sssStrtBelowVal, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStpBelowTxt = lv_label_create(___sssSTSPTxtValCont, NULL);
        lv_obj_align(___sssStpBelowTxt, ___sssStrtBelowTxt, LV_ALIGN_OUT_RIGHT_TOP, 120, 0);
        lv_label_set_text(___sssStpBelowTxt, "Stop:");
        lv_obj_add_style(___sssStpBelowTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStpBelowVal = lv_label_create(___sssSTSPTxtValCont, NULL);
        lv_obj_align(___sssStpBelowVal, ___sssStpBelowTxt, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
        lv_label_set_text_fmt(___sssStpBelowVal, "%0.2lf L", xSequenceSummary.summary.xVolumeCounter.fEndVolume); // sequence stop volume counter
        lv_obj_add_style(___sssStpBelowVal, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssTarEffVarValCont = lv_cont_create(__sssSeqSumCont, NULL);
        lv_obj_set_size(___sssTarEffVarValCont, 280, 20);
        lv_obj_set_click(___sssTarEffVarValCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssTarEffVarValCont, ___sssSTSPTxtValCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
        lv_obj_set_style_local_bg_color(___sssTarEffVarValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssTarEffVarValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        ___sssTargetVal = lv_label_create(___sssTarEffVarValCont, NULL);
        lv_obj_align(___sssTargetVal, ___sssTarEffVarValCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_label_set_text_fmt(___sssTargetVal, "%0.2lf L", xSequenceSummary.summary.xVolumeCounter.fTargetVolume); // liter traget
        lv_obj_add_style(___sssTargetVal, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssEffectiveVal = lv_label_create(___sssTarEffVarValCont, NULL);
        lv_obj_align(___sssEffectiveVal, ___sssTarEffVarValCont, LV_ALIGN_CENTER, -8, 0);
        lv_label_set_text_fmt(___sssEffectiveVal, "%0.2lf L", xSequenceSummary.summary.xVolumeCounter.fEffectiveVolume); // liter effective
        lv_obj_add_style(___sssEffectiveVal, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssVariationVal = lv_label_create(___sssTarEffVarValCont, NULL);
        lv_obj_align(___sssVariationVal, ___sssTarEffVarValCont, LV_ALIGN_IN_RIGHT_MID, -25, 0);
        lv_label_set_text_fmt(___sssVariationVal, "%0.2f%", xSequenceSummary.summary.xVolumeCounter.fVariation); // liter Variation
        lv_obj_add_style(___sssVariationVal, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssTargetTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssTargetTxt, ___sssTarEffVarValCont, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 0);
        lv_label_set_text(___sssTargetTxt, "Target");
        lv_obj_add_style(___sssTargetTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssEffectiveTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssEffectiveTxt, ___sssTarEffVarValCont, LV_ALIGN_OUT_BOTTOM_MID, -12, 0);
        lv_label_set_text(___sssEffectiveTxt, "EFFECTIVE");
        lv_obj_add_style(___sssEffectiveTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssVariationTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssVariationTxt, ___sssTarEffVarValCont, LV_ALIGN_OUT_BOTTOM_RIGHT, -38, 0);
        lv_label_set_text(___sssVariationTxt, "VARIATION");
        lv_obj_add_style(___sssVariationTxt, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        // Seprator line
        // Create Horizontal Line
        hor_line = lv_line_create(__sssSeqSumCont, NULL);
        static lv_point_t p[] = {{-20, 5}, {238, 5}};
        lv_line_set_points(hor_line, p, 2);
        lv_obj_align(hor_line, ___sssTarEffVarValCont, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

        static lv_style_t style_hor_line;
        lv_style_init(&style_hor_line);
        lv_style_set_line_color(&style_hor_line, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x70, 0x70, 0x70));
        lv_style_set_line_width(&style_hor_line, LV_STATE_DEFAULT, 2);
        lv_style_set_line_rounded(&style_hor_line, LV_STATE_DEFAULT, true);
        lv_obj_add_style(hor_line, LV_LINE_PART_MAIN, &style_hor_line);

        ___sssHorCountTxt = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssHorCountTxt, hor_line, LV_ALIGN_OUT_BOTTOM_LEFT, -20, 10);
        lv_label_set_text(___sssHorCountTxt, "Hour COUNTER");
        lv_obj_add_style(___sssHorCountTxt, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        ___sssSTSPTxt2ValCont = lv_cont_create(__sssSeqSumCont, NULL);
        lv_obj_set_size(___sssSTSPTxt2ValCont, 280, 20);
        lv_obj_set_click(___sssSTSPTxt2ValCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssSTSPTxt2ValCont, ___sssHorCountTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        lv_obj_set_style_local_bg_color(___sssSTSPTxt2ValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssSTSPTxt2ValCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        ___sssStrtBelowTxt2 = lv_label_create(___sssSTSPTxt2ValCont, NULL);
        lv_obj_align(___sssStrtBelowTxt2, ___sssSTSPTxt2ValCont, LV_ALIGN_IN_LEFT_MID, 0, 5);
        lv_label_set_text(___sssStrtBelowTxt2, "Start:"); // Start oof Hour Counter
        lv_obj_add_style(___sssStrtBelowTxt2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStrtBelowVal2 = lv_label_create(___sssSTSPTxt2ValCont, NULL);
        lv_obj_align(___sssStrtBelowVal2, ___sssStrtBelowTxt2, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
        lv_label_set_text_fmt(___sssStrtBelowVal2, "%0.2f H", xSequenceSummary.summary.xHourCounter.fStartHour); // Start of Hour Counter
        lv_obj_add_style(___sssStrtBelowVal2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStpBelowTxt2 = lv_label_create(___sssSTSPTxt2ValCont, NULL);
        lv_obj_align(___sssStpBelowTxt2, ___sssStrtBelowTxt2, LV_ALIGN_OUT_RIGHT_TOP, 120, 0);
        lv_label_set_text(___sssStpBelowTxt2, "Stop:"); // Start of hour counter
        lv_obj_add_style(___sssStpBelowTxt2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssStpBelowVal2 = lv_label_create(___sssSTSPTxt2ValCont, NULL);
        lv_obj_align(___sssStpBelowVal2, ___sssStpBelowTxt2, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
        lv_label_set_text_fmt(___sssStpBelowVal2, "%0.2f H", xSequenceSummary.summary.xHourCounter.fEndHour); // End of Hour Counter
        lv_obj_add_style(___sssStpBelowVal2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssTarEffVarValContHC = lv_cont_create(__sssSeqSumCont, NULL);
        lv_obj_set_size(___sssTarEffVarValContHC, 280, 20);
        lv_obj_set_click(___sssTarEffVarValContHC, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssTarEffVarValContHC, ___sssSTSPTxt2ValCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
        lv_obj_set_style_local_bg_color(___sssTarEffVarValContHC, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssTarEffVarValContHC, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        ___sssTargetVal2 = lv_label_create(___sssTarEffVarValContHC, NULL);
        lv_obj_align(___sssTargetVal2, ___sssTarEffVarValContHC, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_label_set_text_fmt(___sssTargetVal2, "%0.2f H", xSequenceSummary.summary.xHourCounter.fTargetHour); // target hour
        lv_obj_add_style(___sssTargetVal2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssEffectiveVal2 = lv_label_create(___sssTarEffVarValContHC, NULL);
        lv_obj_align(___sssEffectiveVal2, ___sssTarEffVarValContHC, LV_ALIGN_CENTER, -8, 0);
        lv_label_set_text_fmt(___sssEffectiveVal2, "%0.2f H", xSequenceSummary.summary.xHourCounter.fEffectiveHour); // effective hour
        lv_obj_add_style(___sssEffectiveVal2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssVariationVal2 = lv_label_create(___sssTarEffVarValContHC, NULL);
        lv_obj_align(___sssVariationVal2, ___sssTarEffVarValContHC, LV_ALIGN_IN_RIGHT_MID, -25, 0);
        lv_label_set_text_fmt(___sssVariationVal2, "%0.2f %", xSequenceSummary.summary.xHourCounter.fVariation); // variation hour
        lv_obj_add_style(___sssVariationVal2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssTargetTxt2 = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssTargetTxt2, ___sssTarEffVarValContHC, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 0);
        lv_label_set_text(___sssTargetTxt2, "Target");
        lv_obj_add_style(___sssTargetTxt2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssEffectiveTxt2 = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssEffectiveTxt2, ___sssTarEffVarValContHC, LV_ALIGN_OUT_BOTTOM_MID, -12, 0);
        lv_label_set_text(___sssEffectiveTxt2, "EFFECTIVE");
        lv_obj_add_style(___sssEffectiveTxt2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssVariationTxt2 = lv_label_create(__sssSeqSumCont, NULL);
        lv_obj_align(___sssVariationTxt2, ___sssTarEffVarValContHC, LV_ALIGN_OUT_BOTTOM_RIGHT, -38, 0);
        lv_label_set_text(___sssVariationTxt2, "VARIATION");
        lv_obj_add_style(___sssVariationTxt2, LV_LABEL_PART_MAIN, &___sssStrtStpTxtStyle);

        ___sssAirflowVolCont = lv_obj_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(___sssAirflowVolCont, 292, 100);
        lv_obj_set_click(___sssAirflowVolCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssAirflowVolCont, __sssSeqSumCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
        lv_obj_set_style_local_bg_color(___sssAirflowVolCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssAirflowVolCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Status icon img
        ___sssStatusMark_AFV = lv_img_create(___sssAirflowVolCont, NULL);
        /* set the icon for the air flow, either ok or not ok */
        if (xSequenceSummary.airflowVolumetric.bIsInRange)
        {
                lv_img_set_src(___sssStatusMark_AFV, &ok_icon);
        }
        else
        {
                lv_img_set_src(___sssStatusMark_AFV, &cross_icon);
        }
        lv_obj_align(___sssStatusMark_AFV, ___sssAirflowVolCont, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

        ___sssAirFlowTxtLbl = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(___sssAirFlowTxtLbl, ___sssAirflowVolCont, LV_ALIGN_IN_TOP_LEFT, 5, 5);
        lv_label_set_text(___sssAirFlowTxtLbl, "AIRFLOW VOLUMETRIC L/min");
        lv_obj_add_style(___sssAirFlowTxtLbl, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        //-------------------Text style small fix text under small boxes--------------------------
        static lv_style_t _sssSamllFixTxtStyle;
        lv_style_init(&_sssSamllFixTxtStyle);
        lv_style_set_text_font(&_sssSamllFixTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12); // signal_20
        lv_style_set_text_color(&_sssSamllFixTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

        //-------------------Text style small Variable text under small boxes---------------------
        static lv_style_t _sssSamllVarTxtStyle;
        lv_style_init(&_sssSamllVarTxtStyle);
        lv_style_set_text_font(&_sssSamllVarTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
        lv_style_set_text_color(&_sssSamllVarTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

        //-------------------Text style for up/down symbol---------------------
        static lv_style_t _sssUpDownSymbolStyle;
        lv_style_init(&_sssUpDownSymbolStyle);
        lv_style_set_text_font(&_sssUpDownSymbolStyle, LV_STATE_DEFAULT, &lv_font_montserrat_18); // signal_20
        lv_style_set_text_color(&_sssUpDownSymbolStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

        // Create label for Set Point Text
        _sssSetPtTxt_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssSetPtTxt_AFV, ___sssAirFlowTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 4);
        lv_label_set_text(_sssSetPtTxt_AFV, "SETPOINT:");
        lv_obj_add_style(_sssSetPtTxt_AFV, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for setpoint variable value
        _sssSetPtVar_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssSetPtVar_AFV, _sssSetPtTxt_AFV, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssSetPtVar_AFV, "%0.2f", xSequenceSummary.airflowVolumetric.fAirflowSetPoint); // display flow set point value
        lv_obj_add_style(_sssSetPtVar_AFV, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MEAN Txt
        _sssMeanTxt_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssMeanTxt_AFV, _sssSetPtTxt_AFV, LV_ALIGN_OUT_RIGHT_TOP, 80, 0);
        lv_label_set_text(_sssMeanTxt_AFV, "MEAN:");
        lv_obj_add_style(_sssMeanTxt_AFV, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for MEAN variable value
        _sssMeanVar_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssMeanVar_AFV, _sssMeanTxt_AFV, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssMeanVar_AFV, "%0.2f", xSequenceSummary.airflowVolumetric.fAirflowMean); // display flow mean value

        lv_obj_add_style(_sssMeanVar_AFV, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        //--------------------------------------------------------------------------------

        // Create label for MAX VALUE Fix Txt

        _sssMaxValTxt_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssMaxValTxt_AFV, _sssSetPtTxt_AFV, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
        lv_label_set_text(_sssMaxValTxt_AFV, "MAX VALUE");
        lv_obj_add_style(_sssMaxValTxt_AFV, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssUpdownSignal_AFV, _sssMaxValTxt_AFV, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal_AFV, LV_SYMBOL_UPLOAD);
        lv_obj_add_style(_sssUpdownSignal_AFV, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMaxValVar_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssMaxValVar_AFV, _sssUpdownSignal_AFV, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMaxValVar_AFV, "%0.2f", xSequenceSummary.airflowVolumetric.fAirflowMaxValue); // display flow max value"
        lv_obj_add_style(_sssMaxValVar_AFV, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MIN VALUE Fix Txt
        _sssMinValTxt_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssMinValTxt_AFV, _sssMaxValTxt_AFV, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssMinValTxt_AFV, "MIN VALUE");
        lv_obj_add_style(_sssMinValTxt_AFV, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal2_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssUpdownSignal2_AFV, _sssMinValTxt_AFV, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal2_AFV, LV_SYMBOL_DOWNLOAD);
        lv_obj_add_style(_sssUpdownSignal2_AFV, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMinValVar_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssMinValVar_AFV, _sssUpdownSignal2_AFV, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMinValVar_AFV, "%0.2f", xSequenceSummary.airflowVolumetric.fAirflowMinValue); // display flow min value"
        lv_obj_add_style(_sssMinValVar_AFV, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for VARIATION Fix Txt
        _sssVariationTxt_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssVariationTxt_AFV, _sssMinValTxt_AFV, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssVariationTxt_AFV, "VARIATION");
        lv_obj_add_style(_sssVariationTxt_AFV, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssPercentSymbol_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssPercentSymbol_AFV, _sssVariationTxt_AFV, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssPercentSymbol_AFV, "%");
        lv_obj_add_style(_sssPercentSymbol_AFV, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssVariationVal_AFV = lv_label_create(___sssAirflowVolCont, NULL);
        lv_obj_align(_sssVariationVal_AFV, _sssPercentSymbol_AFV, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssVariationVal_AFV, "%0.2f", xSequenceSummary.airflowVolumetric.fAirflowVariation); // display flow variation value"
        lv_obj_add_style(_sssVariationVal_AFV, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        ___sssAmbTempCont = lv_obj_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(___sssAmbTempCont, 292, 100);
        lv_obj_set_click(___sssAmbTempCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssAmbTempCont, ___sssAirflowVolCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
        lv_obj_set_style_local_bg_color(___sssAmbTempCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssAmbTempCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Status icon img
        ___sssStatusMark_AT = lv_img_create(___sssAmbTempCont, NULL);
        /* set the icon for the air flow, either ok or not ok */
        if (xSequenceSummary.ambientTemperature.bIsInRange)
        {
                lv_img_set_src(___sssStatusMark_AT, &ok_icon);
        }
        else
        {
                lv_img_set_src(___sssStatusMark_AT, &cross_icon);
        }

        lv_obj_align(___sssStatusMark_AT, ___sssAmbTempCont, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

        ___sssAmbTempTxtLbl = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(___sssAmbTempTxtLbl, ___sssAmbTempCont, LV_ALIGN_IN_TOP_LEFT, 5, 5);
        lv_label_set_text(___sssAmbTempTxtLbl, "AMBIENT TEMPERATURE °C");
        lv_obj_add_style(___sssAmbTempTxtLbl, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        // Create label for Set Point Text
        _sssLIMITTxt_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssLIMITTxt_AT, ___sssAmbTempTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 4);
        lv_label_set_text(_sssLIMITTxt_AT, "LIMIT:");
        lv_obj_add_style(_sssLIMITTxt_AT, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for Satrt limit value
        _sssLimitLower_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssLimitLower_AT, _sssLIMITTxt_AT, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssLimitLower_AT, "%d", EXTERNAL_SENSOR_TEMPERATURE_MIN_VALUE); // display external sensor min value"
        lv_obj_add_style(_sssLimitLower_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for to text
        _ssstoTxt_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_ssstoTxt_AT, _sssLimitLower_AT, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text(_ssstoTxt_AT, "to");
        lv_obj_add_style(_ssstoTxt_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for last limit value
        _sssLimitllast_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssLimitllast_AT, _ssstoTxt_AT, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssLimitllast_AT, "%d", EXTERNAL_SENSOR_TEMPERATURE_MAX_VALUE); // display external sensor max value"
        lv_obj_add_style(_sssLimitllast_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MEAN Txt
        _sssMeanTxt_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssMeanTxt_AT, _sssLIMITTxt_AT, LV_ALIGN_OUT_RIGHT_TOP, 110, 0);
        lv_label_set_text(_sssMeanTxt_AT, "MEAN:");
        lv_obj_add_style(_sssMeanTxt_AT, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for MEAN variable value
        _sssMeanVar_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssMeanVar_AT, _sssMeanTxt_AT, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssMeanVar_AT, "%0.2f", xSequenceSummary.ambientTemperature.fMeanTemperature); // display mean value of external temeprature
        lv_obj_add_style(_sssMeanVar_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MAX VALUE Fix Txt
        _sssMaxValTxt_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssMaxValTxt_AT, _sssLIMITTxt_AT, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
        lv_label_set_text(_sssMaxValTxt_AT, "MAX VALUE");
        lv_obj_add_style(_sssMaxValTxt_AT, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssUpdownSignal_AT, _sssMaxValTxt_AT, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal_AT, LV_SYMBOL_UPLOAD);
        lv_obj_add_style(_sssUpdownSignal_AT, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMaxValVar_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssMaxValVar_AT, _sssUpdownSignal_AT, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMaxValVar_AT, "%0.2f", xSequenceSummary.ambientTemperature.fMaxTemperature); // display max value of external temeprature
        lv_obj_add_style(_sssMaxValVar_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MIN VALUE Fix Txt
        _sssMinValTxt_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssMinValTxt_AT, _sssMaxValTxt_AT, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssMinValTxt_AT, "MIN VALUE");
        lv_obj_add_style(_sssMinValTxt_AT, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal2_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssUpdownSignal2_AT, _sssMinValTxt_AT, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal2_AT, LV_SYMBOL_DOWNLOAD);
        lv_obj_add_style(_sssUpdownSignal2_AT, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMinValVar_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssMinValVar_AT, _sssUpdownSignal2_AT, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMinValVar_AT, "%0.2f", xSequenceSummary.ambientTemperature.fMinTemperature); // display min value of external temeprature
        lv_obj_add_style(_sssMinValVar_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for VARIATION Fix Txt
        _sssVariationTxt_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssVariationTxt_AT, _sssMinValTxt_AT, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssVariationTxt_AT, "VARIATION");
        lv_obj_add_style(_sssVariationTxt_AT, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssPercentSymbol_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssPercentSymbol_AT, _sssVariationTxt_AT, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssPercentSymbol_AT, "%");
        lv_obj_add_style(_sssPercentSymbol_AT, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssVariationVal_AT = lv_label_create(___sssAmbTempCont, NULL);
        lv_obj_align(_sssVariationVal_AT, _sssPercentSymbol_AT, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssVariationVal_AT, "%0.2f", xSequenceSummary.ambientTemperature.fTemperatureVariation);
        lv_obj_add_style(_sssVariationVal_AT, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        ___sssAmbPressCont = lv_obj_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(___sssAmbPressCont, 292, 100);
        lv_obj_set_click(___sssAmbPressCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssAmbPressCont, ___sssAmbTempCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
        lv_obj_set_style_local_bg_color(___sssAmbPressCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssAmbPressCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Status icon img
        ___sssStatusMark_AP = lv_img_create(___sssAmbPressCont, NULL);
        /* set the icon for the air flow, either ok or not ok */
        if (xSequenceSummary.ambientPressure.bIsInRange)
        {
                lv_img_set_src(___sssStatusMark_AP, &ok_icon);
        }
        else
        {
                lv_img_set_src(___sssStatusMark_AP, &cross_icon);
        }
        lv_obj_align(___sssStatusMark_AP, ___sssAmbPressCont, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

        ___sssAmbPressTxtLbl = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(___sssAmbPressTxtLbl, ___sssAmbPressCont, LV_ALIGN_IN_TOP_LEFT, 5, 5);
        lv_label_set_text(___sssAmbPressTxtLbl, "AMBIENT PRESSURE hPa");
        lv_obj_add_style(___sssAmbPressTxtLbl, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        // Create label for Set Point Text
        _sssLIMITTxt_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssLIMITTxt_AP, ___sssAmbPressTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 4);
        lv_label_set_text(_sssLIMITTxt_AP, "LIMIT:");
        lv_obj_add_style(_sssLIMITTxt_AP, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for Satrt limit value
        _sssLimitLower_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssLimitLower_AP, _sssLIMITTxt_AP, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssLimitLower_AP, "%d", EXTERNAL_SENSOR_PRESSURE_MIN_VALUE); // setting the extrnal pressure sensor min value
        lv_obj_add_style(_sssLimitLower_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for to text
        _ssstoTxt_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_ssstoTxt_AP, _sssLimitLower_AP, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text(_ssstoTxt_AP, "to");
        lv_obj_add_style(_ssstoTxt_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for last limit value
        _sssLimitllast_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssLimitllast_AP, _ssstoTxt_AP, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssLimitllast_AP, "%d", EXTERNAL_SENOSR_PRESSURE_MAX_VALUE); // setting the external pressure sensor max value
        lv_obj_add_style(_sssLimitllast_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MEAN Txt
        _sssMeanTxt_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssMeanTxt_AP, _sssLIMITTxt_AP, LV_ALIGN_OUT_RIGHT_TOP, 110, 0);
        lv_label_set_text(_sssMeanTxt_AP, "MEAN:");
        lv_obj_add_style(_sssMeanTxt_AP, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for MEAN variable value
        _sssMeanVar_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssMeanVar_AP, _sssMeanTxt_AP, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssMeanVar_AP, "%0.2f", xSequenceSummary.ambientPressure.fMeanAmbientPressure); // setting the mean value of ambient pressure
        lv_obj_add_style(_sssMeanVar_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MAX VALUE Fix Txt
        _sssMaxValTxt_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssMaxValTxt_AP, _sssLIMITTxt_AP, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
        lv_label_set_text(_sssMaxValTxt_AP, "MAX VALUE");
        lv_obj_add_style(_sssMaxValTxt_AP, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssUpdownSignal_AP, _sssMaxValTxt_AP, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal_AP, LV_SYMBOL_UPLOAD);
        lv_obj_add_style(_sssUpdownSignal_AP, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMaxValVar_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssMaxValVar_AP, _sssUpdownSignal_AP, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMaxValVar_AP, "%0.2f", xSequenceSummary.ambientPressure.fMaxAmbientPressure); // setting the max value of ambient pressure
        lv_obj_add_style(_sssMaxValVar_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MIN VALUE Fix Txt
        _sssMinValTxt_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssMinValTxt_AP, _sssMaxValTxt_AP, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssMinValTxt_AP, "MIN VALUE");
        lv_obj_add_style(_sssMinValTxt_AP, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal2_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssUpdownSignal2_AP, _sssMinValTxt_AP, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal2_AP, LV_SYMBOL_DOWNLOAD);
        lv_obj_add_style(_sssUpdownSignal2_AP, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMinValVar_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssMinValVar_AP, _sssUpdownSignal2_AP, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMinValVar_AP, "%0.2f", xSequenceSummary.ambientPressure.fMinAmbientPressure); // setting the min value of ambient pressure
        lv_obj_add_style(_sssMinValVar_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for VARIATION Fix Txt
        _sssVariationTxt_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssVariationTxt_AP, _sssMinValTxt_AP, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssVariationTxt_AP, "VARIATION");
        lv_obj_add_style(_sssVariationTxt_AP, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssPercentSymbol_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssPercentSymbol_AP, _sssVariationTxt_AP, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssPercentSymbol_AP, "%");
        lv_obj_add_style(_sssPercentSymbol_AP, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssVariationVal_AP = lv_label_create(___sssAmbPressCont, NULL);
        lv_obj_align(_sssVariationVal_AP, _sssPercentSymbol_AP, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssVariationVal_AP, "%0.2f", xSequenceSummary.ambientPressure.fAmbientPressureVariation); // setting the variation value of ambient pressure
        lv_obj_add_style(_sssVariationVal_AP, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        ___sssAmbHumiCont = lv_obj_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(___sssAmbHumiCont, 292, 100);
        lv_obj_set_click(___sssAmbHumiCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssAmbHumiCont, ___sssAmbPressCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
        lv_obj_set_style_local_bg_color(___sssAmbHumiCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssAmbHumiCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Status icon img
        ___sssStatusMark_AH = lv_img_create(___sssAmbHumiCont, NULL);
        /* set the icon for the air flow, either ok or not ok */
        if (xSequenceSummary.ambientHumidity.bIsInRange)
        {
                lv_img_set_src(___sssStatusMark_AH, &ok_icon);
        }
        else
        {
                lv_img_set_src(___sssStatusMark_AH, &cross_icon);
        }
        lv_obj_align(___sssStatusMark_AH, ___sssAmbHumiCont, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

        ___sssAmbHumiTxtLbl = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(___sssAmbHumiTxtLbl, ___sssAmbHumiCont, LV_ALIGN_IN_TOP_LEFT, 5, 5);
        lv_label_set_text(___sssAmbHumiTxtLbl, "AMBIENT HUMIDITY %");
        lv_obj_add_style(___sssAmbHumiTxtLbl, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        // Create label for Set Point Text
        _sssLIMITTxt_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssLIMITTxt_AH, ___sssAmbHumiTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 4);
        lv_label_set_text(_sssLIMITTxt_AH, "LIMIT:");
        lv_obj_add_style(_sssLIMITTxt_AH, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for Satrt limit value
        _sssLimitLower_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssLimitLower_AH, _sssLIMITTxt_AH, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssLimitLower_AH, "%d", EXTERNAL_SENSOR_HUMIDITY_MIN_VALUE); // setting the ambient humidity lower limit value
        lv_obj_add_style(_sssLimitLower_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for to text
        _ssstoTxt_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_ssstoTxt_AH, _sssLimitLower_AH, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text(_ssstoTxt_AH, "to");
        lv_obj_add_style(_ssstoTxt_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for last limit value
        _sssLimitllast_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssLimitllast_AH, _ssstoTxt_AH, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssLimitllast_AH, "%d", EXTERNAL_SENSOR_HUMIDITY_MAX_VALUE); // seting the ambient humidity upper limit value
        lv_obj_add_style(_sssLimitllast_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MEAN Txt
        _sssMeanTxt_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssMeanTxt_AH, _sssLIMITTxt_AH, LV_ALIGN_OUT_RIGHT_TOP, 110, 0);
        lv_label_set_text(_sssMeanTxt_AH, "MEAN:");
        lv_obj_add_style(_sssMeanTxt_AH, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for MEAN variable value
        _sssMeanVar_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssMeanVar_AH, _sssMeanTxt_AH, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssMeanVar_AH, "%0.2f", xSequenceSummary.ambientHumidity.fMeanAmbientHumidity); // setting the mean value of ambient humidity
        lv_obj_add_style(_sssMeanVar_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MAX VALUE Fix Txt
        _sssMaxValTxt_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssMaxValTxt_AH, _sssLIMITTxt_AH, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
        lv_label_set_text(_sssMaxValTxt_AH, "MAX VALUE");
        lv_obj_add_style(_sssMaxValTxt_AH, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssUpdownSignal_AH, _sssMaxValTxt_AH, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal_AH, LV_SYMBOL_UPLOAD);
        lv_obj_add_style(_sssUpdownSignal_AH, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMaxValVar_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssMaxValVar_AH, _sssUpdownSignal_AH, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMaxValVar_AH, "%0.2f", xSequenceSummary.ambientHumidity.fMaxAmbientHumidity); // setting the max value of ambient humidity
        lv_obj_add_style(_sssMaxValVar_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MIN VALUE Fix Txt
        _sssMinValTxt_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssMinValTxt_AH, _sssMaxValTxt_AH, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssMinValTxt_AH, "MIN VALUE");
        lv_obj_add_style(_sssMinValTxt_AH, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal2_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssUpdownSignal2_AH, _sssMinValTxt_AH, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal2_AH, LV_SYMBOL_DOWNLOAD);
        lv_obj_add_style(_sssUpdownSignal2_AH, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMinValVar_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssMinValVar_AH, _sssUpdownSignal2_AH, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMinValVar_AH, "%0.2f", xSequenceSummary.ambientHumidity.fMinAmbientHumidity); // setting the min value of ambient humidity
        lv_obj_add_style(_sssMinValVar_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for VARIATION Fix Txt
        _sssVariationTxt_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssVariationTxt_AH, _sssMinValTxt_AH, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssVariationTxt_AH, "VARIATION");
        lv_obj_add_style(_sssVariationTxt_AH, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssPercentSymbol_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssPercentSymbol_AH, _sssVariationTxt_AH, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssPercentSymbol_AH, "%");
        lv_obj_add_style(_sssPercentSymbol_AH, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssVariationVal_AH = lv_label_create(___sssAmbHumiCont, NULL);
        lv_obj_align(_sssVariationVal_AH, _sssPercentSymbol_AH, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssVariationVal_AH, "%0.2f", xSequenceSummary.ambientHumidity.fAmbientHumidityVariation); // setting the variation value of ambient humidity
        lv_obj_add_style(_sssVariationVal_AH, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        ___sssHeadLossCont = lv_obj_create(_sssSummaryParentScroll, NULL);
        lv_obj_set_size(___sssHeadLossCont, 292, 100);
        lv_obj_set_click(___sssHeadLossCont, false);
        // lv_obj_set_width(___sssSTSPTxtValCont, lv_page_get_width_fit(_sssSummaryParentScroll));
        lv_obj_align(___sssHeadLossCont, ___sssAmbHumiCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
        lv_obj_set_style_local_bg_color(___sssHeadLossCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // 5f615f, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D), _xColorBGParas
        lv_obj_set_style_local_border_width(___sssHeadLossCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        // Create Status icon img
        ___sssStatusMark_HL = lv_img_create(___sssHeadLossCont, NULL);
        /* set the icon for the air flow, either ok or not ok */
        if (xSequenceSummary.headLoss.bIsInRange)
        {
                lv_img_set_src(___sssStatusMark_HL, &ok_icon);
        }
        else
        {
                lv_img_set_src(___sssStatusMark_HL, &cross_icon);
        }
        lv_obj_align(___sssStatusMark_HL, ___sssHeadLossCont, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

        ___sssHeadLossTxtLbl = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(___sssHeadLossTxtLbl, ___sssHeadLossCont, LV_ALIGN_IN_TOP_LEFT, 5, 5);
        lv_label_set_text(___sssHeadLossTxtLbl, "HEAD LOSS mbar");
        lv_obj_add_style(___sssHeadLossTxtLbl, LV_LABEL_PART_MAIN, &_sssBlueHeadingTxtStyle);

        // Create label for Set Point Text
        _sssLIMITTxt_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssLIMITTxt_HL, ___sssHeadLossTxtLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 4);
        lv_label_set_text(_sssLIMITTxt_HL, "LIMIT:");
        lv_obj_add_style(_sssLIMITTxt_HL, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for Satrt limit value
        _sssLimitLower_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssLimitLower_HL, _sssLIMITTxt_HL, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssLimitLower_HL, "-%d", HEAD_LOSS_MIN_VALUE);
        lv_obj_add_style(_sssLimitLower_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for to text
        _ssstoTxt_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_ssstoTxt_HL, _sssLimitLower_HL, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text(_ssstoTxt_HL, "to");
        lv_obj_add_style(_ssstoTxt_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for last limit value
        _sssLimitllast_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssLimitllast_HL, _ssstoTxt_HL, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssLimitllast_HL, "%d", HEAD_LOSS_MAX_VALUE);
        lv_obj_add_style(_sssLimitllast_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MEAN Txt
        _sssMeanTxt_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssMeanTxt_HL, _sssLIMITTxt_HL, LV_ALIGN_OUT_RIGHT_TOP, 110, 0);
        lv_label_set_text(_sssMeanTxt_HL, "MEAN:");
        lv_obj_add_style(_sssMeanTxt_HL, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        // Create label for MEAN variable value
        _sssMeanVar_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssMeanVar_HL, _sssMeanTxt_HL, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_label_set_text_fmt(_sssMeanVar_HL, "%0.2f", xSequenceSummary.headLoss.fMeanHeadLoss); // setting the mean value of head loss
        lv_obj_add_style(_sssMeanVar_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MAX VALUE Fix Txt
        _sssMaxValTxt_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssMaxValTxt_HL, _sssLIMITTxt_HL, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
        lv_label_set_text(_sssMaxValTxt_HL, "MAX VALUE");
        lv_obj_add_style(_sssMaxValTxt_HL, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssUpdownSignal_HL, _sssMaxValTxt_HL, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal_HL, LV_SYMBOL_UPLOAD);
        lv_obj_add_style(_sssUpdownSignal_HL, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMaxValVar_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssMaxValVar_HL, _sssUpdownSignal_HL, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMaxValVar_HL, "%0.2f", xSequenceSummary.headLoss.fMaxHeadLoss); // setting the max value of head loss
        lv_obj_add_style(_sssMaxValVar_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for MIN VALUE Fix Txt
        _sssMinValTxt_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssMinValTxt_HL, _sssMaxValTxt_HL, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssMinValTxt_HL, "MIN VALUE");
        lv_obj_add_style(_sssMinValTxt_HL, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssUpdownSignal2_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssUpdownSignal2_HL, _sssMinValTxt_HL, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssUpdownSignal2_HL, LV_SYMBOL_DOWNLOAD);
        lv_obj_add_style(_sssUpdownSignal2_HL, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssMinValVar_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssMinValVar_HL, _sssUpdownSignal2_HL, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssMinValVar_HL, "%0.2f", xSequenceSummary.headLoss.fMinHeadLoss); // setting the min value of head loss
        lv_obj_add_style(_sssMinValVar_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create label for VARIATION Fix Txt
        _sssVariationTxt_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssVariationTxt_HL, _sssMinValTxt_HL, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);
        lv_label_set_text(_sssVariationTxt_HL, "VARIATION");
        lv_obj_add_style(_sssVariationTxt_HL, LV_LABEL_PART_MAIN, &_sssSamllFixTxtStyle);

        _sssPercentSymbol_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssPercentSymbol_HL, _sssVariationTxt_HL, LV_ALIGN_OUT_TOP_LEFT, 5, -5);
        lv_label_set_text(_sssPercentSymbol_HL, "%");
        lv_obj_add_style(_sssPercentSymbol_HL, LV_LABEL_PART_MAIN, &_sssUpDownSymbolStyle);

        _sssVariationVal_HL = lv_label_create(___sssHeadLossCont, NULL);
        lv_obj_align(_sssVariationVal_HL, _sssPercentSymbol_HL, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
        lv_label_set_text_fmt(_sssVariationVal_HL, "%0.2f", xSequenceSummary.headLoss.fHeadLossVariation); // setting the variation value of head loss
        lv_obj_add_style(_sssVariationVal_HL, LV_LABEL_PART_MAIN, &_sssSamllVarTxtStyle);

        // Create QUIT Buttons
        _sssQuitBtn = lv_btn_create(_sssSummaryParentScroll, NULL);
        lv_obj_align(_sssQuitBtn, ___sssHeadLossCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
        lv_obj_set_size(_sssQuitBtn, 290, 44);
        lv_obj_set_event_cb(_sssQuitBtn, QuitBTN_event_handler);
        // lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);

        static lv_style_t _sssQuitBtnStyle;
        lv_style_init(&_sssQuitBtnStyle);
        lv_style_set_radius(&_sssQuitBtnStyle, LV_STATE_DEFAULT, 3);
        // lv_style_set_bg_color(&_xStopBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEb, 0x3B, 0x5A));
        lv_style_set_bg_color(&_sssQuitBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #359Fe2
        lv_style_set_border_width(&_sssQuitBtnStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_border_opa(&_sssQuitBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
        lv_obj_add_style(_sssQuitBtn, LV_BTN_PART_MAIN, &_sssQuitBtnStyle);
        // lv_obj_set_event_cb(_xaValidBtn, valid_btn_event_handler);

        // Creat a Valid Button Label
        _sssQuitBtnLabel = lv_label_create(_sssQuitBtn, NULL);
        lv_obj_align(_sssQuitBtnLabel, _sssQuitBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
        lv_label_set_text(_sssQuitBtnLabel, "QUIT");

        static lv_style_t _sssQuitBtnLabelStyle;
        lv_style_init(&_sssQuitBtnLabelStyle);
        lv_style_set_text_font(&_sssQuitBtnLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
        lv_style_set_text_color(&_sssQuitBtnLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_obj_add_style(_sssQuitBtnLabel, LV_LABEL_PART_MAIN, &_sssQuitBtnLabelStyle);

        crnt_screen = scrSummarySample; // scrSummarySample
        screenid = SCR_SUMMARY_SAMPLE;
}

static void __sssTimeLabel_refr_func(lv_task_t *__sssrefresherTask)
{
        if (lv_obj_get_screen(__sssTimeLabel) == lv_scr_act())
        {
                lv_label_set_text(__sssTimeLabel, guiTime);
        }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void __sssBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
        if (event == LV_EVENT_RELEASED)
        {
                lv_task_del(__sssrefresherTask);
                xseSummaryEndScreen();
        }
}

static void QuitBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
        if (event == LV_EVENT_RELEASED)
        {
                lv_task_del(__sssrefresherTask);
                dashboardflg = 0;
                pxDashboardScreen();
        }
}

void vSetSampleNumberAndSequnceNumberSampleSummary(uint32_t uSmpNum, uint8_t uSeqNumber, sequenceSummary_t *pSeq)
{
        uSampleNumber = uSmpNum;
        uSequnceNumber = uSeqNumber;
        memcpy(&xSequenceSummary, pSeq, sizeof(sequenceSummary_t));
}

/**********************
 *    ERROR ASSERT
 **********************/
