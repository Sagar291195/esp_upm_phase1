/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-01
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"
#include <esp_log.h>
/*********************
 *   DEFINES/CONST
 *********************/

#define SYMBOL_SIGNAL "\uf012"
#define TAG "SCREEN PRESET"

/*********************
 *      DEFINES
 *********************/

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
static void _xaTimeLabel_Adv_refr_func(lv_task_t *_xarefresherTask);
static void valid_btn_event_handler(lv_obj_t *obj, lv_event_t event);
static void _xsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void _NextSeq_event_handler(lv_obj_t *obj, lv_event_t event);

static void _xaDateroller_event_handler(lv_obj_t *obj, lv_event_t event);
static void _xaHourroller_event_handler(lv_obj_t *obj, lv_event_t event);
static void _xaMinutroller_event_handler(lv_obj_t *obj, lv_event_t event);
static void _xsSProller_event_handler(lv_obj_t *obj, lv_event_t event);
static void _xsDurHourroller_event_handler(lv_obj_t *obj, lv_event_t event);
static void _xsDurMinutroller_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t *scr_preset;
lv_obj_t *xaParentContainer_Adv;
lv_obj_t *_xaContainerStatusBar_Adv;
lv_obj_t *_xaTimeLabel_Adv;
lv_obj_t *_xaBatteryLabel_Adv;
lv_obj_t *_xaWifiLabel_Adv;
lv_obj_t *_xaSignalLabel_Adv;
lv_obj_t *_xaParaHeadingCont_Adv;
lv_obj_t *_xaBackArrowLabel_Adv;
lv_obj_t *_xaParameterHeading_Adv;
lv_obj_t *_xaGoToNextCont_Adv;
lv_obj_t *_xaGoToNextLbl_Adv;
lv_obj_t *_xaStartLabel_Adv;
lv_obj_t *_xaDMYCon_Adv;
lv_obj_t *_xaDateroller;
lv_obj_t *_xaHourroller;
lv_obj_t *_xaMinutroller;
lv_obj_t *_xaDotLabel_Adv;
lv_obj_t *_xaSPDurCon_Adv;
lv_obj_t *_xaSPLabel_Adv;
lv_obj_t *_xaDurLabel_Adv;
lv_obj_t *_xsSProller;
lv_obj_t *_xaLPMTxtLabel_Adv;
lv_obj_t *_xsDurHourroller;
lv_obj_t *_xaDurHourTxtLabel_Adv;
lv_obj_t *_xsDurMinutroller;
lv_obj_t *_xaDurMinTxtLabel_Adv;
lv_obj_t *_xaValidBtn;
lv_obj_t *_xaValidButtonLabel;
lv_obj_t *_xaSeq_Num_Adv;
lv_obj_t *_xaBackArrowCont;

lv_task_t *_xarefresherTask;

bool startTimeLabelAdgFlg = false;
bool rollerMovCkFlag = false;
char *Seq_Name = "Sequence";
char *Seq_Number = "1";
int SeqCounter = 0;
char _xaSecondrollerbuf[5] = "00";
char _xaDaterollerbuf[40];
char _xaHourMinrollerbuf[130];
char _xaHourrollerbuf[60];
char _xaMinutrollerbuf[32];

char _xsSProllerbuf[32];
char _xsDurHourrollerbuf[32];
char _xsDurMinutrollerbuf[32];

//====================================

char _xaDaterollerNull[10] = "";
char _xaHourMinrollerNull[130] = "";
char _xaHourrollerNull[60] = "";
char _xaMinutrollerNull[32] = "";

char _xsSProllerNull[32] = "";
char _xsDurHourrollerNull[32] = "";
char _xsDurMinutrollerNull[32] = "";

//====================================

char defaultSP[5] = "7.0";
char defaultDurHr[5] = "24";
char defaultDurMin[5] = "00";
char WseqDate1[60] = "2021-06-06";
char WseqTime1[130] = "16:28:00";
char WseqSetPt1[32] = "5.0";
char WseqDurHr1[32] = "24";
char WseqDurMin1[32] = "00";
int WseqStatus1 = 0;

int startHr = 0;
int startMin = 0;
int stopHr = 0;
int durHr = 0;
int durMin = 0;



bool defaultParaSelected;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void xsPresetScreenAdvance(void)
{
    vControllerInitializeSampleArray();
    Seq_Number = "1";
    SeqCounter = 0;
    // Craete a Screen container
    sprintf(_xaDaterollerbuf, "%s", _xaDaterollerNull);
    sprintf(_xaHourrollerbuf, "%s", _xaHourrollerNull);
    sprintf(_xaMinutrollerbuf, "%s", _xaMinutrollerNull);
    sprintf(_xsSProllerbuf, "%s", _xsSProllerNull);
    sprintf(_xsDurHourrollerbuf, "%s", _xsDurHourrollerNull);
    sprintf(_xsDurHourrollerbuf, "%s", _xsDurHourrollerNull);
    sprintf(_xsDurMinutrollerbuf, "%s", _xsDurMinutrollerNull);
    ESP_LOGI(TAG, "Loading preset screen");

    scr_preset = lv_obj_create(NULL, NULL);
    lv_scr_load(scr_preset);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    xaParentContainer_Adv = lv_cont_create(scr_preset, NULL);
    // lv_scr_load(xaParentContainer_Adv);
    lv_obj_set_size(xaParentContainer_Adv, 320, 480);
    lv_obj_set_click(xaParentContainer_Adv, false);
    lv_obj_align(xaParentContainer_Adv, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(xaParentContainer_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_border_opa(xaParentContainer_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(xaParentContainer_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status
    _xaContainerStatusBar_Adv = lv_cont_create(xaParentContainer_Adv, NULL);
    lv_obj_set_size(_xaContainerStatusBar_Adv, 320, 35);
    lv_obj_align(_xaContainerStatusBar_Adv, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xaContainerStatusBar_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_xaContainerStatusBar_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    _xaTimeLabel_Adv = lv_label_create(_xaContainerStatusBar_Adv, NULL);
    lv_obj_align(_xaTimeLabel_Adv, _xaContainerStatusBar_Adv, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    // lv_label_set_text(_xaTimeLabel_Adv, "04:55");
    lv_label_set_text(_xaTimeLabel_Adv, guiTime);

    static lv_style_t _xaTimeLabelStyle_Adv;
    lv_style_init(&_xaTimeLabelStyle_Adv);
    lv_style_set_text_font(&_xaTimeLabelStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xaTimeLabelStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaTimeLabel_Adv, LV_LABEL_PART_MAIN, &_xaTimeLabelStyle_Adv);
    _xarefresherTask = lv_task_create(_xaTimeLabel_Adv_refr_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Create Label for Battery icon
    _xaBatteryLabel_Adv = lv_label_create(_xaContainerStatusBar_Adv, NULL);
    lv_obj_align(_xaBatteryLabel_Adv, _xaContainerStatusBar_Adv, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(_xaBatteryLabel_Adv, get_battery_symbol());

    static lv_style_t _xaBatteryLabelStyle_Adv;
    lv_style_init(&_xaBatteryLabelStyle_Adv);
    lv_style_set_text_font(&_xaBatteryLabelStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_xaBatteryLabelStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaBatteryLabel_Adv, LV_LABEL_PART_MAIN, &_xaBatteryLabelStyle_Adv);

    // Create Label for Wifi icon
    _xaWifiLabel_Adv = lv_label_create(_xaContainerStatusBar_Adv, NULL);
    lv_obj_align(_xaWifiLabel_Adv, _xaBatteryLabel_Adv, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(_xaWifiLabel_Adv, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(_xaWifiLabel_Adv, true);

    static lv_style_t _xaWifiLabelStyle_Adv;
    lv_style_init(&_xaWifiLabelStyle_Adv);
    lv_style_set_text_font(&_xaWifiLabelStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xaWifiLabelStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaWifiLabel_Adv, LV_LABEL_PART_MAIN, &_xaWifiLabelStyle_Adv);

    // Create Label for Signal icon
    _xaSignalLabel_Adv = lv_label_create(_xaContainerStatusBar_Adv, NULL);
    lv_obj_align(_xaSignalLabel_Adv, _xaWifiLabel_Adv, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(_xaSignalLabel_Adv, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(_xaSignalLabel_Adv, true);

    static lv_style_t _xaSignalLabelStyle_Adv;
    lv_style_init(&_xaSignalLabelStyle_Adv);
    lv_style_set_text_font(&_xaSignalLabelStyle_Adv, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xaSignalLabelStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaSignalLabel_Adv, LV_LABEL_PART_MAIN, &_xaSignalLabelStyle_Adv);

    //==============================================================================================================
    // Create a container to contain Sequences Header
    _xaParaHeadingCont_Adv = lv_cont_create(xaParentContainer_Adv, NULL);
    lv_obj_set_size(_xaParaHeadingCont_Adv, 300, 70);
    lv_obj_align(_xaParaHeadingCont_Adv, _xaContainerStatusBar_Adv, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xaParaHeadingCont_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xaParaHeadingCont_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _xaBackArrowCont = lv_cont_create(_xaParaHeadingCont_Adv, NULL);
    lv_obj_set_size(_xaBackArrowCont, 60, 60);
    lv_obj_align(_xaBackArrowCont, _xaParaHeadingCont_Adv, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_obj_set_style_local_bg_color(_xaBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xaBackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(_xaBackArrowCont, _xsBackArrow_event_handler);

    // Create Back arrow img
    _xaBackArrowLabel_Adv = lv_img_create(_xaBackArrowCont, NULL);
    lv_img_set_src(_xaBackArrowLabel_Adv, &left_arrow_icon);
    lv_obj_align(_xaBackArrowLabel_Adv, _xaBackArrowCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(_xaBackArrowLabel_Adv, true);
    lv_obj_set_style_local_image_recolor_opa(_xaBackArrowLabel_Adv, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_xaBackArrowLabel_Adv, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xaBackArrowLabel_Adv, _xsBackArrow_event_handler);

    // Create Label for Sequences "Heading"
    _xaParameterHeading_Adv = lv_label_create(_xaParaHeadingCont_Adv, NULL);
    lv_obj_align(_xaParameterHeading_Adv, _xaParaHeadingCont_Adv, LV_ALIGN_IN_BOTTOM_MID, -50, -35);
    lv_label_set_text(_xaParameterHeading_Adv, Seq_Name);

    // Create Label for Sequences "Number"
    _xaSeq_Num_Adv = lv_label_create(_xaParaHeadingCont_Adv, NULL);
    lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 32, 0);

    lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);

    static lv_style_t _xaParameterHeadingStyle_Adv;
    lv_style_init(&_xaParameterHeadingStyle_Adv);
    lv_style_set_text_font(&_xaParameterHeadingStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&_xaParameterHeadingStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaParameterHeading_Adv, LV_LABEL_PART_MAIN, &_xaParameterHeadingStyle_Adv);
    lv_obj_add_style(_xaSeq_Num_Adv, LV_LABEL_PART_MAIN, &_xaParameterHeadingStyle_Adv);

    // Create Container for Next Sequence Navigation BTN
    _xaGoToNextCont_Adv = lv_cont_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaGoToNextCont_Adv, _xaContainerStatusBar_Adv, LV_ALIGN_OUT_BOTTOM_RIGHT, 2, 10);
    lv_obj_set_size(_xaGoToNextCont_Adv, 50, 50);
    lv_obj_set_event_cb(_xaGoToNextCont_Adv, _NextSeq_event_handler);

    static lv_style_t _xaGoToNextContStyle_Adv;
    lv_style_init(&_xaGoToNextContStyle_Adv);
    lv_style_set_bg_color(&_xaGoToNextContStyle_Adv, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38)); // LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_style_set_border_width(&_xaGoToNextContStyle_Adv, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(_xaGoToNextCont_Adv, LV_CONT_PART_MAIN, &_xaGoToNextContStyle_Adv);

    // create next label icon
    _xaGoToNextLbl_Adv = lv_label_create(_xaGoToNextCont_Adv, NULL);
    lv_obj_align(_xaGoToNextLbl_Adv, _xaGoToNextCont_Adv, LV_ALIGN_IN_TOP_MID, 3, 5);
    lv_label_set_text(_xaGoToNextLbl_Adv, LV_SYMBOL_PLUS);

    static lv_style_t _xaGoToNextStyle_Adv;
    lv_style_init(&_xaGoToNextStyle_Adv);
    lv_style_set_bg_color(&_xaGoToNextStyle_Adv, LV_STATE_DEFAULT, LV_COLOR_YELLOW);         // LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_style_set_text_font(&_xaGoToNextStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_30); // signal_20
    lv_style_set_text_color(&_xaGoToNextStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaGoToNextLbl_Adv, LV_LABEL_PART_MAIN, &_xaGoToNextStyle_Adv);

    // Create Label to "START:" Text
    _xaStartLabel_Adv = lv_label_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaStartLabel_Adv, _xaBackArrowLabel_Adv, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);
    lv_label_set_text(_xaStartLabel_Adv, "START : ");

    static lv_style_t _xaStartLabelStyle_Adv;
    lv_style_init(&_xaStartLabelStyle_Adv);
    lv_style_set_text_font(&_xaStartLabelStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_18); // signal_20
    lv_style_set_text_color(&_xaStartLabelStyle_Adv, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xaStartLabel_Adv, LV_LABEL_PART_MAIN, &_xaStartLabelStyle_Adv);

    // Craete container to hold Date Hour & Minut
    _xaDMYCon_Adv = lv_cont_create(xaParentContainer_Adv, NULL);
    lv_obj_set_size(_xaDMYCon_Adv, 310, 120);
    lv_obj_align(_xaDMYCon_Adv, _xaStartLabel_Adv, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 5);
    lv_obj_set_style_local_bg_color(_xaDMYCon_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xaDMYCon_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Roller to hold date
    _xaDateroller = lv_roller_create(_xaDMYCon_Adv, NULL);
    lv_roller_set_options(_xaDateroller, GuiDateRollerStr, LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_selected(_xaDateroller, 0, LV_ANIM_OFF);
    lv_roller_set_visible_row_count(_xaDateroller, 3);
    lv_obj_align(_xaDateroller, _xaDMYCon_Adv, LV_ALIGN_IN_LEFT_MID, 4, 1);
    lv_obj_set_click(_xaDateroller, false);
    lv_obj_set_style_local_bg_color(_xaDateroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(_xaDateroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(_xaDateroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(_xaDateroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(_xaDateroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(_xaDateroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xaDateroller, _xaDateroller_event_handler);

    // Roller to hold Hour

    _xaHourroller = lv_roller_create(_xaDMYCon_Adv, NULL);
    lv_roller_set_options(_xaHourroller, " - \n"
                                         "00\n"
                                         "01\n"
                                         "02\n"
                                         "03\n"
                                         "04\n"
                                         "05\n"
                                         "06\n"
                                         "07\n"
                                         "08\n"
                                         "09\n"
                                         "10\n"
                                         "11\n"
                                         "12\n"
                                         "13\n"
                                         "14\n"
                                         "15\n"
                                         "16\n"
                                         "17\n"
                                         "18\n"
                                         "19\n"
                                         "20\n"
                                         "21\n"
                                         "22\n"
                                         "23\n",
                          LV_ROLLER_MODE_INIFINITE);

    struct tm current_time = get_current_time();
    lv_roller_set_visible_row_count(_xaHourroller, 3);
    lv_roller_set_selected(_xaHourroller, current_time.tm_hour + 1, LV_ANIM_OFF);
    lv_obj_align(_xaHourroller, _xaDateroller, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    // lv_obj_set_size(_xaHourroller, 20, 60);
    lv_obj_set_click(_xaHourroller, false);
    lv_obj_set_style_local_bg_color(_xaHourroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(_xaHourroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(_xaHourroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(_xaHourroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_text_color(_xaHourroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(_xaHourroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xaHourroller, _xaHourroller_event_handler);

    // Roller to hold Minut
    _xaMinutroller = lv_roller_create(_xaDMYCon_Adv, NULL);
    lv_roller_set_options(_xaMinutroller, " - \n"
                                          "00\n"
                                          "01\n"
                                          "02\n"
                                          "03\n"
                                          "04\n"
                                          "05\n"
                                          "06\n"
                                          "07\n"
                                          "08\n"
                                          "09\n"
                                          "10\n"
                                          "11\n"
                                          "12\n"
                                          "13\n"
                                          "14\n"
                                          "15\n"
                                          "16\n"
                                          "17\n"
                                          "18\n"
                                          "19\n"
                                          "20\n"
                                          "21\n"
                                          "22\n"
                                          "23\n"
                                          "24\n"
                                          "25\n"
                                          "26\n"
                                          "27\n"
                                          "28\n"
                                          "29\n"
                                          "30\n"
                                          "31\n"
                                          "32\n"
                                          "33\n"
                                          "34\n"
                                          "35\n"
                                          "36\n"
                                          "37\n"
                                          "38\n"
                                          "39\n"
                                          "40\n"
                                          "41\n"
                                          "42\n"
                                          "43\n"
                                          "44\n"
                                          "45\n"
                                          "46\n"
                                          "47\n"
                                          "48\n"
                                          "49\n"
                                          "50\n"
                                          "51\n"
                                          "52\n"
                                          "53\n"
                                          "54\n"
                                          "55\n"
                                          "56\n"
                                          "57\n"
                                          "58\n"
                                          "59\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(_xaMinutroller, 3);
    lv_roller_set_selected(_xaMinutroller, current_time.tm_min + 1, LV_ANIM_OFF);
    lv_obj_align(_xaMinutroller, _xaHourroller, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    // lv_obj_set_size(_xaMinutroller, 20, 60);
    lv_obj_set_click(_xaMinutroller, false);
    lv_obj_set_style_local_bg_color(_xaMinutroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(_xaMinutroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(_xaMinutroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(_xaMinutroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_text_color(_xaMinutroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(_xaMinutroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xaMinutroller, _xaMinutroller_event_handler);

    // Create a label for ":"
    _xaDotLabel_Adv = lv_label_create(_xaDMYCon_Adv, NULL);
    lv_obj_align(_xaDotLabel_Adv, _xaDMYCon_Adv, LV_ALIGN_CENTER, 100, -13);
    lv_label_set_text(_xaDotLabel_Adv, ":");

    static lv_style_t _xaSlashLabelStyle_Adv;
    lv_style_init(&_xaSlashLabelStyle_Adv);
    lv_style_set_text_font(&_xaSlashLabelStyle_Adv, LV_STATE_DEFAULT, &lv_font_montserrat_30);
    lv_style_set_text_color(&_xaSlashLabelStyle_Adv, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(_xaDotLabel_Adv, LV_LABEL_PART_MAIN, &_xaSlashLabelStyle_Adv);

    // Create Horizontal Line between Date & Hour
    lv_obj_t *hor_line;
    hor_line = lv_line_create(_xaDMYCon_Adv, NULL);
    static lv_point_t p[] = {{0, 25}, {0, 300}};
    lv_line_set_points(hor_line, p, 2);

    lv_obj_align(hor_line, _xaDMYCon_Adv, LV_ALIGN_IN_TOP_MID, 30, -50);

    static lv_style_t style_hor_line;
    lv_style_init(&style_hor_line);

    lv_style_set_line_color(&style_hor_line, LV_STATE_DEFAULT, LV_COLOR_GRAY); // LV_COLOR_MAKE(0xAC, 0xAC, 0xAC)
    lv_style_set_line_width(&style_hor_line, LV_STATE_DEFAULT, 1);
    lv_style_set_line_rounded(&style_hor_line, LV_STATE_DEFAULT, true);

    lv_obj_add_style(hor_line, LV_LINE_PART_MAIN, &style_hor_line);

    // Create Horizontal Line between Hour and Minut
    lv_obj_t *hor_line1;
    hor_line1 = lv_line_create(_xaDMYCon_Adv, NULL);
    static lv_point_t p1[] = {{0, 25}, {0, 300}};
    lv_line_set_points(hor_line1, p1, 2);
    lv_obj_align(hor_line1, _xaDMYCon_Adv, LV_ALIGN_IN_TOP_MID, 95, -50);
    lv_obj_add_style(hor_line1, LV_LINE_PART_MAIN, &style_hor_line);

    // Craete container to hold Date Hour & Minut
    _xaSPDurCon_Adv = lv_cont_create(xaParentContainer_Adv, NULL);
    lv_obj_set_size(_xaSPDurCon_Adv, 310, 120);
    lv_obj_align(_xaSPDurCon_Adv, _xaDMYCon_Adv, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);
    lv_obj_set_style_local_bg_color(_xaSPDurCon_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_xaSPDurCon_Adv, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Label to "SETPOINT:" Text
    _xaSPLabel_Adv = lv_label_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaSPLabel_Adv, _xaSPDurCon_Adv, LV_ALIGN_OUT_TOP_LEFT, 8, -10);
    lv_label_set_text(_xaSPLabel_Adv, "SETPOINT : ");
    lv_obj_add_style(_xaSPLabel_Adv, LV_LABEL_PART_MAIN, &_xaStartLabelStyle_Adv);

    // Create Label to "DURATIO:" Text
    _xaDurLabel_Adv = lv_label_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaDurLabel_Adv, _xaSPDurCon_Adv, LV_ALIGN_OUT_TOP_MID, 0, -10);
    lv_label_set_text(_xaDurLabel_Adv, "DURATION : ");
    lv_obj_add_style(_xaDurLabel_Adv, LV_LABEL_PART_MAIN, &_xaStartLabelStyle_Adv);

    // Roller to hold SETPOINT Value
    _xsSProller = lv_roller_create(_xaSPDurCon_Adv, NULL);
    lv_roller_set_options(_xsSProller,
                          "5.0\n"
                          "5.1\n"
                          "5.2\n"
                          "5.3\n"
                          "5.4\n"
                          "5.5\n"
                          "5.6\n"
                          "5.7\n"
                          "5.8\n"
                          "5.9\n"
                          "6.0\n"
                          "6.1\n"
                          "6.2\n"
                          "6.3\n"
                          "6.4\n"
                          "6.6\n"
                          "6.6\n"
                          "6.7\n"
                          "6.8\n"
                          "6.9\n"
                          "7.0\n"
                          "7.1\n"
                          "7.2\n"
                          "7.3\n"
                          "7.4\n"
                          "7.7\n"
                          "7.7\n"
                          "7.7\n"
                          "7.8\n"
                          "7.9\n"
                          "8.0\n"
                          "8.1\n"
                          "8.2\n"
                          "8.3\n"
                          "8.4\n"
                          "8.8\n"
                          "8.8\n"
                          "8.8\n"
                          "8.8\n"
                          "8.9\n"
                          "9.0\n"
                          "9.1\n"
                          "9.2\n"
                          "9.3\n"
                          "9.4\n"
                          "9.9\n"
                          "9.9\n"
                          "9.9\n"
                          "9.9\n"
                          "9.9\n"
                          "10\n"
                          "1.0\n"
                          "1.1\n"
                          "1.2\n"
                          "1.3\n"
                          "1.4\n"
                          "1.5\n"
                          "1.6\n"
                          "1.7\n"
                          "1.8\n"
                          "1.9\n"
                          "2.0\n"
                          "2.1\n"
                          "2.2\n"
                          "2.3\n"
                          "2.4\n"
                          "2.5\n"
                          "2.6\n"
                          "2.7\n"
                          "2.8\n"
                          "2.9\n"
                          "3.0\n"
                          "3.1\n"
                          "3.2\n"
                          "3.3\n"
                          "3.4\n"
                          "3.5\n"
                          "3.6\n"
                          "3.7\n"
                          "3.8\n"
                          "3.9\n"
                          "4.0\n"
                          "4.1\n"
                          "4.2\n"
                          "4.3\n"
                          "4.4\n"
                          "4.5\n"
                          "4.6\n"
                          "4.7\n"
                          "4.8\n"
                          "4.9\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(_xsSProller, 3);
    lv_roller_set_selected(_xsSProller, 20, LV_ANIM_OFF);
    lv_obj_align(_xsSProller, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 8, 0);
    lv_obj_set_click(_xsSProller, false);
    lv_obj_set_style_local_bg_color(_xsSProller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(_xsSProller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(_xsSProller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(_xsSProller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(_xsSProller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(_xsSProller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xsSProller, _xsSProller_event_handler);

    // Create Label to "L/Min:" Text
    _xaLPMTxtLabel_Adv = lv_label_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaLPMTxtLabel_Adv, _xsSProller, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_label_set_text(_xaLPMTxtLabel_Adv, "L/Min");

    static lv_style_t xaDurationTxtStyle;
    lv_style_init(&xaDurationTxtStyle);
    lv_style_set_text_font(&xaDurationTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&xaDurationTxtStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_add_style(_xaLPMTxtLabel_Adv, LV_LABEL_PART_MAIN, &xaDurationTxtStyle);

    // Roller to hold Duration Hours Value
    _xsDurHourroller = lv_roller_create(_xaSPDurCon_Adv, NULL);
    lv_roller_set_options(_xsDurHourroller, " 24 \n"
                                            " 0 \n"
                                            " 1 \n"
                                            " 2 \n"
                                            " 3 \n"
                                            " 4 \n"
                                            " 5 \n"
                                            " 6 \n"
                                            " 7 \n"
                                            " 8 \n"
                                            " 9 \n"
                                            " 10 \n"
                                            " 11 \n"
                                            " 12 \n"
                                            " 13 \n"
                                            " 14 \n"
                                            " 15 \n"
                                            " 16 \n"
                                            " 17 \n"
                                            " 18 \n"
                                            " 19 \n"
                                            " 20 \n"
                                            " 21 \n"
                                            " 22 \n"
                                            " 23 \n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(_xsDurHourroller, 3);
    lv_obj_align(_xsDurHourroller, _xaLPMTxtLabel_Adv, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_set_click(_xsDurHourroller, false);
    lv_obj_set_style_local_bg_color(_xsDurHourroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(_xsDurHourroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(_xsDurHourroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(_xsDurHourroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(_xsDurHourroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(_xsDurHourroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xsDurHourroller, _xsDurHourroller_event_handler);

    // Create Label to "L/Min:" Text
    _xaDurHourTxtLabel_Adv = lv_label_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaDurHourTxtLabel_Adv, _xsDurHourroller, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_label_set_text(_xaDurHourTxtLabel_Adv, "hour");
    lv_obj_add_style(_xaDurHourTxtLabel_Adv, LV_LABEL_PART_MAIN, &xaDurationTxtStyle);

    // Roller to hold Duration Minut Value
    _xsDurMinutroller = lv_roller_create(_xaSPDurCon_Adv, NULL);
    lv_roller_set_options(_xsDurMinutroller, " 00 \n"
                          " 1 \n"
                          " 2 \n"
                          " 3 \n"
                          " 4 \n"
                          " 5 \n"
                          " 6 \n"
                          " 7 \n"
                          " 8 \n"
                          " 9 \n"
                          " 10 \n"
                          " 11 \n"
                          " 12 \n"
                          " 13 \n"
                          " 14 \n"
                          " 15 \n"
                          " 16 \n"
                          " 17 \n"
                          " 18 \n"
                          " 19 \n"
                          " 20 \n"
                          " 21 \n"
                          " 22 \n"
                          " 23 \n"
                          " 24 \n"
                          " 25 \n"
                          " 26 \n"
                          " 27 \n"
                          " 28 \n"
                          " 29 \n"
                          " 30 \n"
                          " 31 \n"
                          " 32 \n"
                          " 33 \n"
                          " 34 \n"
                          " 35 \n"
                          " 36 \n"
                          " 37 \n"
                          " 38 \n"
                          " 39 \n"
                          " 40 \n"
                          " 41 \n"
                          " 42 \n"
                          " 43 \n"
                          " 44 \n"
                          " 45 \n"
                          " 46 \n"
                          " 47 \n"
                          " 48 \n"
                          " 49 \n"
                          " 50 \n"
                          " 51 \n"
                          " 52 \n"
                          " 53 \n"
                          " 54 \n"
                          " 55 \n"
                          " 56 \n"
                          " 57 \n"
                          " 58 \n"
                          " 59 \n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(_xsDurMinutroller, 3);
    lv_obj_align(_xsDurMinutroller, _xaDurHourTxtLabel_Adv, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_set_click(_xsDurMinutroller, false);
    lv_obj_set_style_local_bg_color(_xsDurMinutroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(_xsDurMinutroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(_xsDurMinutroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(_xsDurMinutroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(_xsDurMinutroller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(_xsDurMinutroller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_xsDurMinutroller, _xsDurMinutroller_event_handler);

    // Create Label to "Min:" Text
    _xaDurMinTxtLabel_Adv = lv_label_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaDurMinTxtLabel_Adv, _xsDurMinutroller, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_label_set_text(_xaDurMinTxtLabel_Adv, "min");
    lv_obj_add_style(_xaDurMinTxtLabel_Adv, LV_LABEL_PART_MAIN, &xaDurationTxtStyle);

    // Create Vertical Lines
    static lv_point_t L[] = {{1, 0}, {70, 0}};

    lv_obj_t *ver_line = lv_line_create(_xaSPDurCon_Adv, NULL);
    lv_line_set_points(ver_line, L, 2);
    lv_obj_align(ver_line, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 15, -23);

    static lv_style_t style_ver_line;
    lv_style_init(&style_ver_line);
    lv_style_set_line_color(&style_ver_line, LV_STATE_DEFAULT, LV_COLOR_GRAY); // LV_COLOR_MAKE(0xAC, 0xAC, 0xAC)
    lv_style_set_line_width(&style_ver_line, LV_STATE_DEFAULT, 1);
    lv_style_set_line_rounded(&style_ver_line, LV_STATE_DEFAULT, true);

    lv_obj_add_style(ver_line, LV_LINE_PART_MAIN, &style_ver_line);

    lv_obj_t *ver_line1 = lv_line_create(_xaSPDurCon_Adv, NULL);
    lv_line_set_points(ver_line1, L, 2);
    lv_obj_align(ver_line1, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 15, 23);
    lv_obj_add_style(ver_line1, LV_LINE_PART_MAIN, &style_ver_line);

    static lv_point_t L1[] = {{1, 0}, {40, 0}};
    lv_obj_t *ver_line2 = lv_line_create(_xaSPDurCon_Adv, NULL);
    lv_line_set_points(ver_line2, L1, 2);
    lv_obj_align(ver_line2, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 140, -23);
    lv_obj_add_style(ver_line2, LV_LINE_PART_MAIN, &style_ver_line);

    lv_obj_t *ver_line3 = lv_line_create(_xaSPDurCon_Adv, NULL);
    lv_line_set_points(ver_line3, L1, 2);
    lv_obj_align(ver_line3, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 140, 23);
    lv_obj_add_style(ver_line3, LV_LINE_PART_MAIN, &style_ver_line);

    lv_obj_t *ver_line4 = lv_line_create(_xaSPDurCon_Adv, NULL);
    lv_line_set_points(ver_line4, L1, 2);
    lv_obj_align(ver_line4, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 232, -23);
    lv_obj_add_style(ver_line4, LV_LINE_PART_MAIN, &style_ver_line);

    lv_obj_t *ver_line5 = lv_line_create(_xaSPDurCon_Adv, NULL);
    lv_line_set_points(ver_line5, L1, 2);
    lv_obj_align(ver_line5, _xaSPDurCon_Adv, LV_ALIGN_IN_LEFT_MID, 232, 23);
    lv_obj_add_style(ver_line5, LV_LINE_PART_MAIN, &style_ver_line);

    //==========================Valid Button =================================================================

    // Create Valid Button
    _xaValidBtn = lv_btn_create(xaParentContainer_Adv, NULL);
    lv_obj_align(_xaValidBtn, _xaSPDurCon_Adv, LV_ALIGN_OUT_BOTTOM_LEFT, 5, -1);
    lv_obj_set_size(_xaValidBtn, 300, 44);

    static lv_style_t _xaValidBtnStyle;
    lv_style_init(&_xaValidBtnStyle);
    lv_style_set_radius(&_xaValidBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&_xaValidBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48)); // #359Fe2
    lv_style_set_bg_color(&_xaValidBtnStyle, LV_STATE_DEFAULT | LV_STATE_PRESSED | LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_width(&_xaValidBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_xaValidBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_obj_add_style(_xaValidBtn, LV_BTN_PART_MAIN, &_xaValidBtnStyle);

    lv_obj_set_event_cb(_xaValidBtn, valid_btn_event_handler);

    // Creat a Valid Button Label
    _xaValidButtonLabel = lv_label_create(_xaValidBtn, NULL);
    lv_obj_align(_xaValidButtonLabel, _xaValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_xaValidButtonLabel, "VALID");

    static lv_style_t _xaValidButtonLabelStyle;
    lv_style_init(&_xaValidButtonLabelStyle);
    lv_style_set_text_font(&_xaValidButtonLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&_xaValidButtonLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xaValidButtonLabel, LV_LABEL_PART_MAIN, &_xaValidButtonLabelStyle);
    crnt_screen = scr_preset; // scr_preset
    screenid = SCR_PRESET;
}


static void _xaTimeLabel_Adv_refr_func(lv_task_t *_xarefresherTask)
{
    if (lv_obj_get_screen(_xaTimeLabel_Adv) == lv_scr_act())
    {
        lv_label_set_text(_xaTimeLabel_Adv, guiTime);
        lv_label_set_text(_xaBatteryLabel_Adv, get_battery_symbol());
    }
}


/**************************
 *   STATIC FUNCTIONS     *
 **************************/

static void _xsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(_xarefresherTask); 
        _xarefresherTask = NULL;
        rollerMovCkFlag = false;
        global_DashbordBTNflag = 1;
        dashboardflg = 0; // Set Dashboard resume info into Ready Mode
        pxDashboardScreen();
    }
}


static void valid_btn_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        SeqCounter++;
        lv_task_del(_xarefresherTask);
        _xarefresherTask = NULL; 
        defaultParametrs();
        if (rollerMovCkFlag == true)
        {
            ESP_LOGD(TAG, "roller moved flag is true");
            startTimeLabelAdgFlg = true;
            defaultParaSelected = false;

            vSaveSampleValues(get_sequence_number_tosave(), _xaDaterollerbuf, atoi(_xaHourrollerbuf), atoi(_xaMinutrollerbuf), atof(_xsSProllerbuf), atoi(_xsDurHourrollerbuf), atoi(_xsDurMinutrollerbuf), "Mark");

            if (strcmp(_xaDaterollerbuf, _xaDaterollerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xaDaterollerbuf, "%s", guiDate); // Default Date is Current data
            }

            if (strcmp(_xaHourrollerbuf, _xaHourrollerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xaHourrollerbuf, "%s", guiHrDef);
            }

            if (strcmp(_xaMinutrollerbuf, _xaMinutrollerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xaMinutrollerbuf, "%s", guiMinDef);
            }

            if (strcmp(_xsSProllerbuf, _xsSProllerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xsSProllerbuf, "%s", defaultSP);
            }
            if (strcmp(_xsDurHourrollerbuf, _xsDurHourrollerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xsDurHourrollerbuf, "%s", defaultDurHr);
            }
            if (strcmp(_xsDurHourrollerbuf, _xsDurHourrollerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xsDurHourrollerbuf, "%s", defaultDurHr);
            }
            if (strcmp(_xsDurMinutrollerbuf, _xsDurMinutrollerNull) == 0)
            {
                defaultParaSelected = true;
                sprintf(_xsDurMinutrollerbuf, "%s", defaultDurMin);
            }
            startHr = atoi(_xaHourrollerbuf);
            startMin = atoi(_xaMinutrollerbuf);

            struct tm current_time = get_current_time();
            if (startMin == current_time.tm_min)
                startMin += 1;

            durHr = atoi(_xsDurHourrollerbuf);
            durMin = atoi(_xsDurMinutrollerbuf);
            getSeqStopDT(durHr, durMin, startHr, startMin);
            getStopDate(durHr, stopHr);
        }

        //=======If only few parameters entered======================
        if (rollerMovCkFlag == false)
        {
            ESP_LOGD(TAG, "roller moved flag is false");
            startTimeLabelAdgFlg = true;
            defaultParaSelected = false;
            defaultParametrs();
            vSaveSampleValues(get_sequence_number_tosave(), _xaDaterollerbuf, atoi(_xaHourrollerbuf), atoi(_xaMinutrollerbuf), atof(_xsSProllerbuf), atoi(_xsDurHourrollerbuf), atoi(_xsDurMinutrollerbuf), "Mark");
        }
        xssSummaryStartScreen();
    }
}


void set_rollermovck_flag(bool val)
{
    rollerMovCkFlag = val;
}

void defaultParametrs(void)
{
    lv_roller_get_selected_str(_xaHourroller, _xaHourrollerbuf, sizeof(_xaHourrollerbuf));
    lv_roller_get_selected_str(_xaMinutroller, _xaMinutrollerbuf, sizeof(_xaMinutrollerbuf));
    lv_roller_get_selected_str(_xaDateroller, _xaDaterollerbuf, sizeof(_xaDaterollerbuf));
    lv_roller_get_selected_str(_xsSProller, _xsSProllerbuf, sizeof(_xsSProllerbuf));
    lv_roller_get_selected_str(_xsDurHourroller, _xsDurHourrollerbuf, sizeof(_xsDurHourrollerbuf));
    lv_roller_get_selected_str(_xsDurMinutroller, _xsDurMinutrollerbuf, sizeof(_xsDurMinutrollerbuf));

    startHr = atoi(_xaHourrollerbuf);
    startMin = atoi(_xaMinutrollerbuf);
    durHr = atoi(_xsDurHourrollerbuf);
    durMin = atoi(_xsDurMinutrollerbuf);

    getSeqStopDT(durHr, durMin, startHr, startMin);
    getStopDate(durHr, stopHr);
     ESP_LOGD(TAG, "start time = %s", _xaDaterollerbuf);
}

/**
 * @brief event handler for the + icon in the screen. this should save the current sample and save into the memory
 *
 * @param obj
 * @param event
 */
static void _NextSeq_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        SeqCounter++;
        defaultParametrs();
        // save value of previous sequence
        vSaveSampleValues(get_sequence_number_tosave(), _xaDaterollerbuf, atoi(_xaHourrollerbuf), atoi(_xaMinutrollerbuf), atof(_xsSProllerbuf), atoi(_xsDurHourrollerbuf), atoi(_xsDurMinutrollerbuf), "Mark");

        ESP_LOGD(TAG, "_NextSeq_event_handler call = %d", SeqCounter);
        lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
        lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);

        switch (SeqCounter)
        {
        case 1:
            Seq_Number = "2";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 2:
            Seq_Number = "3";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 3:
            Seq_Number = "4";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 4:
            Seq_Number = "5";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 5:
            Seq_Number = "6";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 6:
            Seq_Number = "7";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 7:
            Seq_Number = "8";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 8:
            Seq_Number = "9";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 9:
            Seq_Number = "10";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 10:
            Seq_Number = "11";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 11:
            Seq_Number = "12";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 12:
            Seq_Number = "13";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 13:
            Seq_Number = "14";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 14:
            Seq_Number = "15";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 15:
            Seq_Number = "16";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 16:
            Seq_Number = "17";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 17:
            Seq_Number = "18";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 18:
            Seq_Number = "19";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 19:
            Seq_Number = "20";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        case 20:
            Seq_Number = "1";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;

        default:
            Seq_Number = "1";
            lv_label_set_text(_xaSeq_Num_Adv, Seq_Number);
            lv_obj_align(_xaSeq_Num_Adv, _xaParameterHeading_Adv, LV_ALIGN_OUT_RIGHT_TOP, 2, 0);
            break;
        }
        if (SeqCounter == 20)
        {
            SeqCounter = 0;
        }
    }
}

static void _xaDateroller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        rollerMovCkFlag = true;
        lv_roller_get_selected_str(obj, _xaDaterollerbuf, sizeof(_xaDaterollerbuf));
        // printf("Selected month: %s\n", _xaDaterollerbuf);
    }
}

static void _xaHourroller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        rollerMovCkFlag = true;
        lv_roller_get_selected_str(obj, _xaHourrollerbuf, sizeof(_xaHourrollerbuf));
        startHr = atoi(_xaHourrollerbuf);
    }
}

static void _xaMinutroller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        rollerMovCkFlag = true;
        lv_roller_get_selected_str(obj, _xaMinutrollerbuf, sizeof(_xaMinutrollerbuf));
        sprintf(_xaHourMinrollerbuf, "%s:%s:%s", _xaHourrollerbuf, _xaMinutrollerbuf, _xaSecondrollerbuf);
        startMin = atoi(_xaMinutrollerbuf);
    }
}

static void _xsSProller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        rollerMovCkFlag = true;
        lv_roller_get_selected_str(obj, _xsSProllerbuf, sizeof(_xsSProllerbuf));
    }
}

static void _xsDurHourroller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        rollerMovCkFlag = true;
        lv_roller_get_selected_str(obj, _xsDurHourrollerbuf, sizeof(_xsDurHourrollerbuf));
        durHr = atoi(_xsDurHourrollerbuf);
    }
}

static void _xsDurMinutroller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        rollerMovCkFlag = true;
        lv_roller_get_selected_str(obj, _xsDurMinutrollerbuf, sizeof(_xsDurMinutrollerbuf));
        durMin = atoi(_xsDurMinutrollerbuf);
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
