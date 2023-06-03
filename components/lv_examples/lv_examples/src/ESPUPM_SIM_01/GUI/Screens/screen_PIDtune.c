/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
*  @file 
*  @brief 
*  @details 
*/


/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"

#include "controller.h" 

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void pidkpRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void pidkiRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void pidkdRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void nCoffBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event);

static void pidkp_aBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void pidki_aBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void pidkd_aBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void aCoffBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event);

static void dayRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void monthRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void yearRoller_event_handler(lv_obj_t * obj, lv_event_t event);

static void HourRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void MinRoller_event_handler(lv_obj_t * obj, lv_event_t event);
static void SecRoller_event_handler(lv_obj_t * obj, lv_event_t event);

static void pidSaveBTN_event_handler(lv_obj_t * obj, lv_event_t event);
static void _pidBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void timeSaveBTN_event_handler(lv_obj_t * obj, lv_event_t event);


/**********************
 *  STATIC VARIABLES
 **********************/
LV_IMG_DECLARE(left_arrow_icon);	

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

char KpRollerBuf[5];
char KiRollerBuf[5];
char KdRollerBuf[5];
char nCoffBuf[5];

char Kp_aRollerBuf[5];
char Ki_aRollerBuf[5];
char Kd_aRollerBuf[5];
char aCoffBuf[5];

char day_Roller_Val[5];
char month_Roller_Val[5];
char year_Roller_Val[5];

char hour_Roller_Val[5];
char min_Roller_Val[5];
char sec_Roller_Val[5];



char rollerNullVal[5] = "";

char day_Roller_Default[5] = "01";
char month_Roller_Default[5] = "01";
char year_Roller_Default[5] = "2021";

char hour_Roller_Default[5] = "12";
char min_Roller_Default[5] = "30";
char sec_Roller_Default[5] = "00";

int day_Roller_int  ;
int month_Roller_int;
int year_Roller_int ;

int hour_Roller_int;
int min_Roller_int ;
int sec_Roller_int ;

float KpVal;
float KiVal;
float KdVal;
float nCoff;

float Kp_aVal;
float Ki_aVal;
float Kd_aVal;
float aCoff;

const int DD  ;
const int MM;
const int YYYY ;

const int HH;
const int min ;
const int sec ;

lv_obj_t *crnt_screen;
lv_obj_t *CurrKpLblVal;
lv_obj_t *CurrKiLblVal;
lv_obj_t *CurrKdLblVal;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void xScreenPidTune(void)
{
    lv_obj_t * xPidMainParentCont;
    xPidMainParentCont = lv_obj_create(NULL, NULL);
    lv_scr_load(xPidMainParentCont);
    //lv_obj_del(crnt_screen); // crnt_screen = xPidMainParentCont,
    //lv_obj_del(crnt_screen);
    lv_obj_t *xPidParentCont;
    xPidParentCont = lv_cont_create(xPidMainParentCont, NULL);  
    lv_obj_set_size(xPidParentCont, 320, 480);
    lv_obj_align(xPidParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( xPidParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(xPidParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(xPidParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Black arrow Container

    lv_obj_t *_pidBlackArrowCont;
    _pidBlackArrowCont =  lv_cont_create(xPidParentCont, NULL);
    lv_obj_set_size(_pidBlackArrowCont, 60, 60);
    lv_obj_align(_pidBlackArrowCont, xPidParentCont, LV_ALIGN_IN_TOP_LEFT, 15 , 0);
    lv_obj_set_style_local_bg_color(_pidBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_pidBlackArrowCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );
    lv_obj_set_event_cb(_pidBlackArrowCont, _pidBackArrow_event_handler);


    // Create Back arrow img

    lv_obj_t *_pidBlackArrow;
    _pidBlackArrow = lv_img_create(_pidBlackArrowCont, NULL);
    lv_img_set_src(_pidBlackArrow, &left_arrow_icon);
    lv_obj_align(_pidBlackArrow, _pidBlackArrowCont, LV_ALIGN_IN_LEFT_MID, 0 , 0);
    lv_obj_set_click(_pidBlackArrow, true);
    lv_obj_set_style_local_image_recolor_opa(_pidBlackArrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(_pidBlackArrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(_pidBlackArrow, _pidBackArrow_event_handler);

    // Create Sliding page 

    lv_obj_t  * _pidSloderCont;

    _pidSloderCont = lv_page_create(xPidParentCont, NULL);
    lv_obj_set_size(_pidSloderCont, 320, 420);
    lv_obj_set_drag(_pidSloderCont, false);
    lv_obj_align(_pidSloderCont, xPidParentCont, LV_ALIGN_IN_TOP_MID, 0, 61);
    lv_page_set_scrollbar_mode(_pidSloderCont, LV_SCROLLBAR_MODE_HIDE);
    lv_obj_set_style_local_bg_color(_pidSloderCont, LV_PAGE_PART_BG, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x39, 0x89, 0xBD)); //LV_COLOR_MAKE(0x38, 0x38, 0x38)
    lv_obj_set_style_local_border_width(_pidSloderCont, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0 );

    // Create Main Container inside slider page

    lv_obj_t *_pidMainCont;
    _pidMainCont =  lv_cont_create(_pidSloderCont, NULL);
    lv_obj_set_height(_pidMainCont,  1020);
    lv_obj_set_click(_pidMainCont, false);
    lv_obj_set_drag(_pidMainCont, false);
    lv_obj_set_width(_pidMainCont, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_align(_pidMainCont, _pidSloderCont, LV_ALIGN_IN_TOP_LEFT, 0 , 0);
    lv_obj_set_style_local_bg_color(_pidMainCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_pidMainCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );
    


    //current Kp Text Label val

    lv_obj_t *CurrKpLbl;
    CurrKpLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKpLbl, _pidMainCont, LV_ALIGN_IN_TOP_LEFT, 20, 5);
    lv_obj_set_width(CurrKpLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKpLbl, "Kp_n  =");
    lv_obj_set_style_local_text_font(CurrKpLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKpLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //lv_obj_t *CurrKpLblVal;
    CurrKpLblVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKpLblVal, CurrKpLbl, LV_ALIGN_OUT_RIGHT_TOP, 7, 0);
    lv_obj_set_width(CurrKpLblVal, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text_fmt(CurrKpLblVal, "%0.1f", KpVal); // KpVal
    lv_obj_set_style_local_text_font(CurrKpLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKpLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKiLbl;
    CurrKiLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKiLbl, CurrKpLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_width(CurrKiLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKiLbl, "Ki_n   =");
    lv_obj_set_style_local_text_font(CurrKiLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKiLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //lv_obj_t *CurrKiLblVal;
    CurrKiLblVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKiLblVal, CurrKiLbl, LV_ALIGN_OUT_RIGHT_TOP, 8, 0);
    lv_obj_set_width(CurrKiLblVal, lv_page_get_width_fit(_pidSloderCont));
    //lv_label_set_text(CurrKiLblVal, "0");
    lv_label_set_text_fmt(CurrKiLblVal, "%0.1f", KiVal);
    lv_obj_set_style_local_text_font(CurrKiLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKiLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKdLbl;
    CurrKdLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKdLbl, CurrKiLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_width(CurrKdLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKdLbl, "Kd_n  =");
    lv_obj_set_style_local_text_font(CurrKdLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKdLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //lv_obj_t *CurrKdLblVal;
    CurrKdLblVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKdLblVal, CurrKdLbl, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_width(CurrKdLblVal, lv_page_get_width_fit(_pidSloderCont));
    //lv_label_set_text(CurrKdLblVal, "0");
    lv_label_set_text_fmt(CurrKdLblVal, "%0.1f", KdVal);
    lv_obj_set_style_local_text_font(CurrKdLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKdLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *nCoffTxtLbl;
    nCoffTxtLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(nCoffTxtLbl, CurrKdLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_width(nCoffTxtLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(nCoffTxtLbl, "nCoff =");
    lv_obj_set_style_local_text_font(nCoffTxtLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(nCoffTxtLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *nCoffLbl;
    nCoffLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(nCoffLbl, nCoffTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 7, 0);
    lv_obj_set_width(nCoffLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(nCoffLbl, "00");
    lv_obj_set_style_local_text_font(nCoffLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(nCoffLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);



    //================
    //=================
    //================

    lv_obj_t *CurrKp_aLbl;
    CurrKp_aLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKp_aLbl, CurrKpLbl, LV_ALIGN_OUT_RIGHT_TOP, 60, 0);
    lv_obj_set_width(CurrKp_aLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKp_aLbl, "|| Kp_a   =");
    lv_obj_set_style_local_text_font(CurrKp_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKp_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKp_aVal;
    CurrKp_aVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKp_aVal, CurrKp_aLbl, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_width(CurrKp_aVal, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKp_aVal, "00");
    lv_obj_set_style_local_text_font(CurrKp_aVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKp_aVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // lv_obj_t *CurrKp_aLblVal;
    // CurrKp_aLblVal = lv_label_create(_pidMainCont, NULL);
    // lv_obj_align(CurrKp_aLblVal, CurrKp_aLbl, LV_ALIGN_OUT_RIGHT_TOP, 60, 0);
    // lv_obj_set_width(CurrKp_aLblVal, lv_page_get_width_fit(_pidSloderCont));
    // lv_label_set_text(CurrKp_aLblVal, "|| Kp_a   =");
    // lv_obj_set_style_local_text_font(CurrKp_aLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    // lv_obj_set_style_local_text_color(CurrKp_aLblVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKi_aLbl;
    CurrKi_aLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKi_aLbl, CurrKp_aLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_width(CurrKi_aLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKi_aLbl, "|| Ki_a    =");
    lv_obj_set_style_local_text_font(CurrKi_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKi_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKi_aVal;
    CurrKi_aVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKi_aVal, CurrKi_aLbl, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_width(CurrKi_aVal, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKi_aVal, "00");
    lv_obj_set_style_local_text_font(CurrKi_aVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKi_aVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKd_aLbl;
    CurrKd_aLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKd_aLbl, CurrKi_aLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_width(CurrKd_aLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKd_aLbl, "|| Kd_a  =");
    lv_obj_set_style_local_text_font(CurrKd_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKd_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *CurrKd_aVal;
    CurrKd_aVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(CurrKd_aVal, CurrKd_aLbl, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_width(CurrKd_aVal, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(CurrKd_aVal, "00");
    lv_obj_set_style_local_text_font(CurrKd_aVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(CurrKd_aVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *aCoffTxtLbl;
    aCoffTxtLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(aCoffTxtLbl, CurrKd_aLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_width(aCoffTxtLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(aCoffTxtLbl, "|| aCoff =");
    lv_obj_set_style_local_text_font(aCoffTxtLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(aCoffTxtLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *aCoffTxtVal;
    aCoffTxtVal = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(aCoffTxtVal, aCoffTxtLbl, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_width(aCoffTxtVal, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(aCoffTxtVal, "00");
    lv_obj_set_style_local_text_font(aCoffTxtVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(aCoffTxtVal, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

  


    //================
    //================
    //================


    lv_obj_t *SetKpLbl;
    SetKpLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetKpLbl, CurrKdLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);
    lv_obj_set_width(SetKpLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetKpLbl, "Kp_n");
    lv_obj_set_style_local_text_font(SetKpLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetKpLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //================
    //================

    lv_obj_t *pidkpRoller;
    pidkpRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidkpRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    

    lv_roller_set_visible_row_count(pidkpRoller, 3);
    lv_obj_align(pidkpRoller,SetKpLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidkpRoller, lv_page_get_width_fit(_pidSloderCont));
    //lv_obj_set_size(pidkpRoller, 100, 150);
    lv_obj_set_click(pidkpRoller, false);
    lv_obj_set_style_local_bg_color(pidkpRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidkpRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidkpRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidkpRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidkpRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidkpRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidkpRoller, pidkpRoller_event_handler);


    //=================
    //=================

    lv_obj_t *SetKiLbl;
    SetKiLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetKiLbl, SetKpLbl, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_width(SetKiLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetKiLbl, "Ki_n");
    lv_obj_set_style_local_text_font(SetKiLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetKiLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidkiRoller;
    pidkiRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidkiRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidkiRoller, 3);
    //lv_obj_set_size(pidkiRoller, 100, 150);
    lv_obj_align(pidkiRoller,SetKiLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidkiRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(pidkiRoller, false);
    lv_obj_set_style_local_bg_color(pidkiRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidkiRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidkiRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidkiRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidkiRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidkiRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidkiRoller, pidkiRoller_event_handler);


    lv_obj_t *SetKdLbl;
    SetKdLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetKdLbl, SetKiLbl, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_width(SetKdLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetKdLbl, "Kd_n");
    lv_obj_set_style_local_text_font(SetKdLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetKdLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidkdRoller;
    pidkdRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidkdRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidkdRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(pidkdRoller,SetKdLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidkdRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(pidkdRoller, false);
    lv_obj_set_style_local_bg_color(pidkdRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidkdRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidkdRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidkdRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidkdRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidkdRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidkdRoller, pidkdRoller_event_handler);

    lv_obj_t *SetnCoffLbl;
    SetnCoffLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetnCoffLbl, SetKdLbl, LV_ALIGN_OUT_RIGHT_TOP, 16, 0);
    lv_obj_set_width(SetnCoffLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetnCoffLbl, "nCoff");
    lv_obj_set_style_local_text_font(SetnCoffLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetnCoffLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidnCoffRoller;
    pidnCoffRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidnCoffRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidnCoffRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(pidnCoffRoller,SetnCoffLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidnCoffRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(pidnCoffRoller, false);
    lv_obj_set_style_local_bg_color(pidnCoffRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidnCoffRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidnCoffRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidnCoffRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidnCoffRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidnCoffRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidnCoffRoller, nCoffBuffRoller_event_handler);


    //=============================
    //=============================
    //============================
    //============================

    lv_obj_t *SetKp_aLbl;
    SetKp_aLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetKp_aLbl, SetKpLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 130);
    lv_obj_set_width(SetKp_aLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetKp_aLbl, "Kp_a");
    lv_obj_set_style_local_text_font(SetKp_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetKp_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidkp_aRoller;
    pidkp_aRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidkp_aRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidkp_aRoller, 3);
    lv_obj_align(pidkp_aRoller,SetKp_aLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidkp_aRoller, lv_page_get_width_fit(_pidSloderCont));
    //lv_obj_set_size(pidkpRoller, 100, 150);
    lv_obj_set_click(pidkp_aRoller, false);
    lv_obj_set_style_local_bg_color(pidkp_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidkp_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidkp_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidkp_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidkp_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidkp_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidkp_aRoller, pidkp_aBuffRoller_event_handler);

    lv_obj_t *SetKi_aLbl;
    SetKi_aLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetKi_aLbl, SetKiLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 130);
    lv_obj_set_width(SetKi_aLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetKi_aLbl, "Ki_a");
    lv_obj_set_style_local_text_font(SetKi_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetKi_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidki_aRoller;
    pidki_aRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidki_aRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidki_aRoller, 3);
    lv_obj_align(pidki_aRoller,SetKi_aLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidki_aRoller, lv_page_get_width_fit(_pidSloderCont));
    //lv_obj_set_size(pidkpRoller, 100, 150);
    lv_obj_set_click(pidki_aRoller, false);
    lv_obj_set_style_local_bg_color(pidki_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidki_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidki_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidki_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidki_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidki_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidki_aRoller, pidki_aBuffRoller_event_handler);

    lv_obj_t *SetKd_aLbl;
    SetKd_aLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetKd_aLbl, SetKdLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 130);
    lv_obj_set_width(SetKd_aLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetKd_aLbl, "Kd_a");
    lv_obj_set_style_local_text_font(SetKd_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetKd_aLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidkd_aRoller;
    pidkd_aRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidkd_aRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidkd_aRoller, 3);
    lv_obj_align(pidkd_aRoller,SetKd_aLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidkd_aRoller, lv_page_get_width_fit(_pidSloderCont));
    //lv_obj_set_size(pidkpRoller, 100, 150);
    lv_obj_set_click(pidkd_aRoller, false);
    lv_obj_set_style_local_bg_color(pidkd_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidkd_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidkd_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidkd_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidkd_aRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidkd_aRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidkd_aRoller, pidkd_aBuffRoller_event_handler);

    lv_obj_t *SetaCoffLbl; //SetnCoffLbl
    SetaCoffLbl = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetaCoffLbl, SetnCoffLbl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 130);
    lv_obj_set_width(SetaCoffLbl, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetaCoffLbl, "aCoff");
    lv_obj_set_style_local_text_font(SetaCoffLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetaCoffLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *pidaCoffRoller; //pidnCoffRoller
    pidaCoffRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(pidaCoffRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n"
                                        "60\n" "61\n" "62\n" "63\n" "64\n" "66\n" "66\n" "67\n" "68\n" "69\n"
                                        "70\n" "71\n" "72\n" "73\n" "74\n" "75\n" "75\n" "77\n" "78\n" "79\n"
                                        "80\n" "81\n" "82\n" "83\n" "84\n" "85\n" "86\n" "87\n" "88\n" "89\n"
                                        "90\n" "91\n" "92\n" "93\n" "94\n" "95\n" "96\n" "97\n" "98\n" "99\n"
                                        "100\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(pidaCoffRoller, 3);
    lv_obj_align(pidaCoffRoller,SetaCoffLbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_width(pidaCoffRoller, lv_page_get_width_fit(_pidSloderCont));
    //lv_obj_set_size(pidkpRoller, 100, 150);
    lv_obj_set_click(pidaCoffRoller, false);
    lv_obj_set_style_local_bg_color(pidaCoffRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(pidaCoffRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(pidaCoffRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(pidaCoffRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(pidaCoffRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(pidaCoffRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(pidaCoffRoller, aCoffBuffRoller_event_handler);

    //=============================
    //=============================
    //============================
    //============================




    lv_obj_t *pidSaveBTN;
    pidSaveBTN = lv_btn_create(_pidMainCont, NULL);
    lv_obj_align(pidSaveBTN, pidkpRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 180 );
    //lv_obj_set_width(pidSaveBTN, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_width(pidSaveBTN, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_size(pidSaveBTN, 260, 44);
    lv_obj_set_style_local_radius(pidSaveBTN, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(pidSaveBTN, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_set_event_cb(pidSaveBTN, pidSaveBTN_event_handler);

    lv_obj_t *pidSaveLabel;
    pidSaveLabel = lv_label_create(pidSaveBTN, NULL);
    lv_obj_align(pidSaveLabel, pidSaveBTN, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_width(pidSaveLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(pidSaveLabel, "Save PID");
    lv_obj_set_style_local_text_font(pidSaveLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_30);
    lv_obj_set_style_local_text_color(pidSaveLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *sepratorLabel;
    sepratorLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(sepratorLabel, pidSaveBTN, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 10);
    lv_obj_set_width(sepratorLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(sepratorLabel, "--------------------");
    lv_obj_set_style_local_text_font(sepratorLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_30);
    lv_obj_set_style_local_text_color(sepratorLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *SetTimeLabel;
    SetTimeLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetTimeLabel, sepratorLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 0);
    lv_obj_set_width(SetTimeLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetTimeLabel, "Set RTC Time & Date");
    lv_obj_set_style_local_text_font(SetTimeLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetTimeLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //===============//==================
    //===============//==================

    lv_obj_t *piddateDDRoller;
    piddateDDRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(piddateDDRoller,  "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(piddateDDRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(piddateDDRoller,SetTimeLabel, LV_ALIGN_OUT_BOTTOM_LEFT, -5, 10);
    lv_obj_set_width(piddateDDRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(piddateDDRoller, false);
    lv_obj_set_style_local_bg_color(piddateDDRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(piddateDDRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(piddateDDRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(piddateDDRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(piddateDDRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(piddateDDRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(piddateDDRoller, dayRoller_event_handler);

    lv_obj_t *SetDDTxtLabel;
    SetDDTxtLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetDDTxtLabel, piddateDDRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_width(SetDDTxtLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetDDTxtLabel, "DD");
    lv_obj_set_style_local_text_font(SetDDTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetDDTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //==============//================
    //==============//================

        //===============//==================
    //===============//==================

    lv_obj_t *piddateMMRoller;
    piddateMMRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(piddateMMRoller,  "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(piddateMMRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(piddateMMRoller,piddateDDRoller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_width(piddateMMRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(piddateMMRoller, false);
    lv_obj_set_style_local_bg_color(piddateMMRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(piddateMMRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(piddateMMRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(piddateMMRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(piddateMMRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(piddateMMRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(piddateMMRoller, monthRoller_event_handler);

    lv_obj_t *SetMMTxtLabel;
    SetMMTxtLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetMMTxtLabel, piddateMMRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_width(SetMMTxtLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetMMTxtLabel, "MM");
    lv_obj_set_style_local_text_font(SetMMTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetMMTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //==============//================
    //==============//================

    //===============//==================
    //===============//==================

    lv_obj_t *piddateYYYYRoller;
    piddateYYYYRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(piddateYYYYRoller,  "2021\n" "2022\n" "2023\n" "2024\n" "2025\n" "2026\n" "2027\n" "2028\n" "2029\n" 
                                        "2030\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(piddateYYYYRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(piddateYYYYRoller,piddateMMRoller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_width(piddateYYYYRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(piddateYYYYRoller, false);
    lv_obj_set_style_local_bg_color(piddateYYYYRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(piddateYYYYRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(piddateYYYYRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(piddateYYYYRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(piddateYYYYRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(piddateYYYYRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(piddateYYYYRoller, yearRoller_event_handler);

    lv_obj_t *SetYYYYTxtLabel;
    SetYYYYTxtLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetYYYYTxtLabel, piddateYYYYRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_width(SetYYYYTxtLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetYYYYTxtLabel, "YYYY");
    lv_obj_set_style_local_text_font(SetYYYYTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetYYYYTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //==============//================
    //==============//================

    //==============//===============
    //   Setting Hour Roller
    //==============//===============


    lv_obj_t *SetHourRoller;
    SetHourRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(SetHourRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(SetHourRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(SetHourRoller,piddateDDRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 40);
    lv_obj_set_width(SetHourRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(SetHourRoller, false);
    lv_obj_set_style_local_bg_color(SetHourRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(SetHourRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(SetHourRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(SetHourRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(SetHourRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(SetHourRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(SetHourRoller, HourRoller_event_handler);

    lv_obj_t *SetHourTxtLabel;
    SetHourTxtLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetHourTxtLabel, SetHourRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_width(SetHourTxtLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetHourTxtLabel, "HOUR");
    lv_obj_set_style_local_text_font(SetHourTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetHourTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //==============//================
    //==============//================

    //==============//================
    //    Setting Minute Roller
    //==============//================

    lv_obj_t *SetMinuteRoller;
    SetMinuteRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(SetMinuteRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(SetMinuteRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(SetMinuteRoller,SetHourRoller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_width(SetMinuteRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(SetMinuteRoller, false);
    lv_obj_set_style_local_bg_color(SetMinuteRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(SetMinuteRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(SetMinuteRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(SetMinuteRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(SetMinuteRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(SetMinuteRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(SetMinuteRoller, MinRoller_event_handler);

    lv_obj_t *SetMinuteTxtLabel;
    SetMinuteTxtLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetMinuteTxtLabel, SetMinuteRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_width(SetMinuteTxtLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetMinuteTxtLabel, "MIN");
    lv_obj_set_style_local_text_font(SetMinuteTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetMinuteTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    //==============//================
    //==============//================

    //==============//================
    //    Setting Second Roller
    //==============//================

    lv_obj_t *SetSecondRoller;
    SetSecondRoller = lv_roller_create(_pidMainCont, NULL);
    lv_roller_set_options(SetSecondRoller,  "00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" 
                                        "10\n" "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n"
                                        "20\n" "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n"
                                        "30\n" "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n"
                                        "40\n" "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n"
                                        "50\n" "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59\n" 
                                        , LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(SetSecondRoller, 3);
    //lv_obj_set_size(pidkdRoller, 100, 150);
    lv_obj_align(SetSecondRoller,SetMinuteRoller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_width(SetSecondRoller, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_click(SetSecondRoller, false);
    lv_obj_set_style_local_bg_color(SetSecondRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(SetSecondRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(SetSecondRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(SetSecondRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22);  //22
    lv_obj_set_style_local_text_color(SetSecondRoller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(SetSecondRoller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(SetSecondRoller, SecRoller_event_handler);

    lv_obj_t *SetSecondTxtLabel;
    SetSecondTxtLabel = lv_label_create(_pidMainCont, NULL);
    lv_obj_align(SetSecondTxtLabel, SetSecondRoller, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_width(SetSecondTxtLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(SetSecondTxtLabel, "SEC");
    lv_obj_set_style_local_text_font(SetSecondTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(SetSecondTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    //==============//================
    //==============//================

    lv_obj_t *pidTimeSaveBTN;
    pidTimeSaveBTN = lv_btn_create(_pidMainCont, NULL);
    lv_obj_align(pidTimeSaveBTN, _pidMainCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -30 );
    //lv_obj_set_width(pidSaveBTN, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_width(pidTimeSaveBTN, lv_page_get_width_fit(_pidSloderCont));
    lv_obj_set_size(pidTimeSaveBTN, 250, 44);
    lv_obj_set_style_local_radius(pidTimeSaveBTN, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(pidTimeSaveBTN, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_obj_set_event_cb(pidTimeSaveBTN, timeSaveBTN_event_handler);

    lv_obj_t *pidTimeSaveLabel;
    pidTimeSaveLabel = lv_label_create(pidTimeSaveBTN, NULL);
    lv_obj_align(pidTimeSaveLabel, pidTimeSaveBTN, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_width(pidTimeSaveLabel, lv_page_get_width_fit(_pidSloderCont));
    lv_label_set_text(pidTimeSaveLabel, "Update Time");
    lv_obj_set_style_local_text_font(pidTimeSaveLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_30);
    lv_obj_set_style_local_text_color(pidTimeSaveLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    //lv_obj_set_event_cb(SetSecondRoller, timeSaveBTN_event_handler);


    crnt_screen = xPidMainParentCont;

}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void pidkpRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, KpRollerBuf, sizeof(KpRollerBuf));

    }
}

static void pidkiRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, KiRollerBuf, sizeof(KiRollerBuf));

    }
}

static void pidkdRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, KdRollerBuf, sizeof(KdRollerBuf));

    }
}

static void nCoffBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, nCoffBuf, sizeof(nCoffBuf));

    }
}

static void pidkp_aBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, Kp_aRollerBuf, sizeof(Kp_aRollerBuf));

    }
}

static void pidki_aBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, Ki_aRollerBuf, sizeof(Ki_aRollerBuf));

    }
}

static void pidkd_aBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, Kd_aRollerBuf, sizeof(Kd_aRollerBuf));

    }
}

static void aCoffBuffRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, aCoffBuf, sizeof(aCoffBuf));

    }
}

static void dayRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, day_Roller_Val, sizeof(day_Roller_Val));

    }
}

static void monthRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, month_Roller_Val, sizeof(month_Roller_Val));

    }
}

static void yearRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, year_Roller_Val, sizeof(year_Roller_Val));

    }
}


static void HourRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, hour_Roller_Val, sizeof(hour_Roller_Val));

    }
}

static void MinRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, min_Roller_Val, sizeof(min_Roller_Val));

    }
}

static void SecRoller_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        lv_roller_get_selected_str(obj, sec_Roller_Val, sizeof(sec_Roller_Val));

    }
}


static void pidSaveBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Set Kp_n Value is = %s\n", KpRollerBuf);
        printf("Set Ki_n Value is = %s\n", KiRollerBuf);
        printf("Set Kd_n Value is = %s\n", KdRollerBuf);
        printf("Set nCoff Value is = %s\n", nCoffBuf);

        printf("Set Kp_a Value is = %s\n", Kp_aRollerBuf);
        printf("Set Ki_a Value is = %s\n", Ki_aRollerBuf);
        printf("Set Kd_a Value is = %s\n", Kd_aRollerBuf);
        printf("Set aCoff Value is = %s\n", aCoffBuf);

        
        

        fflush(NULL);

        lv_label_set_text_fmt(CurrKpLblVal, "%s", KpRollerBuf );
        lv_label_set_text_fmt(CurrKiLblVal, "%s", KiRollerBuf );
        lv_label_set_text_fmt(CurrKdLblVal, "%s", KdRollerBuf );

        KpVal = atof(KpRollerBuf);
        KiVal = atof(KiRollerBuf);
        KdVal= atof(KdRollerBuf);
        nCoff= atof(nCoffBuf);

        Kp_aVal = atof(Kp_aRollerBuf);
        Ki_aVal = atof(Ki_aRollerBuf);
        Kd_aVal= atof(Kd_aRollerBuf);
        aCoff= atof(aCoffBuf);

        /**
         * @brief setting the pid parameters 
         * 
         */
        vSetPIDParameters(KpVal, KiVal, KdVal, Kp_aVal, Ki_aVal, Kd_aVal, nCoff,aCoff);
        

    }
}


static void _pidBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("PID Tune Screen Back Arrow Pressed\n");
        fflush(NULL);
        pxDashboardScreen();

    }
}

static void timeSaveBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        

        if(!strcmp(day_Roller_Val, rollerNullVal)){ sprintf(day_Roller_Val, day_Roller_Default);}
        if(!strcmp(month_Roller_Val, rollerNullVal)){sprintf(month_Roller_Val,month_Roller_Default);}
        if(!strcmp(year_Roller_Val, rollerNullVal)){sprintf(year_Roller_Val,year_Roller_Default);}

        if(!strcmp(hour_Roller_Val, rollerNullVal)){ sprintf(hour_Roller_Val, hour_Roller_Default);}
        if(!strcmp(min_Roller_Val, rollerNullVal)){sprintf(min_Roller_Val,min_Roller_Default);}
        if(!strcmp(sec_Roller_Val, rollerNullVal)){sprintf(sec_Roller_Val,sec_Roller_Default);}
       

        printf("Set Day Value is   = %s\n", day_Roller_Val);
        printf("Set Month Value is = %s\n", month_Roller_Val);
        printf("Set Year Value is  = %s\n", year_Roller_Val);

        printf("Set Hour Value is   = %s\n", hour_Roller_Val);
        printf("Set Minute Value is = %s\n", min_Roller_Val);
        printf("Set Secons Value is = %s\n", sec_Roller_Val);

        day_Roller_int   = atoi(day_Roller_Val);
        month_Roller_int = atoi(month_Roller_Val);
        year_Roller_int  = atoi(year_Roller_Val);

        hour_Roller_int  = atoi(hour_Roller_Val);
        min_Roller_int   = atoi(min_Roller_Val);
        sec_Roller_int   = atoi(sec_Roller_Val);

        

                
        navier_set_time = true;
        
        fflush(NULL);
    }
}


/**********************
 *    ERROR ASSERT
 **********************/


