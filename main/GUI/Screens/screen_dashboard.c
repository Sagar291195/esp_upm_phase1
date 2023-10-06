/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"
#include "esp_upm.h"

/*********************
 *      DEFINES
 *********************/
#define TAG             "DASHBOARD"
#define IW_WIDTH        (200)
#define IW_HEIGHT       (290)
#define SYMBOL_SIGNAL   "\uf012"

int global_DashbordBTNflag = 1;
int dashboardflg = 0;
char MenuBTN_SERVICE[10] = "PID TUNE";
char MenuBTN_ARCHIV[10] = "ARCHIV";
char MenuBTN_METROLOGY[10] = "METROLOGY";
char MenuBTN_PARAMETER[10] = "PARAMETER";
char MenuBTN_INFO[10] = "INFO";

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void _xTimeLabel_refr_func(lv_task_t *refresherTask); 
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/


lv_obj_t *crnt_screen;
lv_obj_t *scr_dashbord;
lv_obj_t *container;
lv_obj_t *scr2; //
lv_obj_t *scr3;
lv_obj_t *pInfoWidget;
lv_obj_t *xMenulist1;

lv_obj_t *xParentcontainer;
lv_obj_t *list_btn;
lv_anim_t a;

static lv_obj_t *IW_create = NULL;
lv_obj_t *_xStopBtn;
lv_obj_t *_xStartBtn;
lv_obj_t *xStopButtonLabel;
lv_obj_t *xStartButtonLabel;

lv_obj_t *_xContainerStatusBar;
lv_obj_t *_xTimeLabel;
lv_obj_t *_xBatteryLabel;
lv_obj_t *_xWifiLabel;
lv_obj_t *_xSignalLabel;
lv_obj_t *_xListBtn;
lv_obj_t *_xListLabelClick;
lv_obj_t *_xWelcomeMsgLabel;
lv_obj_t *_xTodaysDateLabel;

/* task responsible for update of time */
static lv_task_t *refresherTask = NULL;

char PrevguiTime[25];
char guiMinDef[32];

char *guiH;

char *dashboardBTNTxt;

int xBTN;
bool iArchORSummaryScrn = 0; // 0=Summary Screen, 1=Archive Screen
char *stop1 = "STOP";
char *startJob = "START JOB";
char *viewsummary = "VIEW SUMMARY";

/***************************
 *Declaration
 ***************************/
// Font Symbol declaration
LV_FONT_DECLARE(signal_20)

// Image declaration
LV_IMG_DECLARE(service_icon)
LV_IMG_DECLARE(archiv_icon)
LV_IMG_DECLARE(metrology_icon)
LV_IMG_DECLARE(parameter_icon)
LV_IMG_DECLARE(info_icon)
LV_IMG_DECLARE(navier_logo)

static lv_task_t *showJobFinishedTask = NULL;

// Function Prototype

static void BTN_event_handler(lv_obj_t *obj, lv_event_t event);
static void event_handler(lv_obj_t *obj, lv_event_t event);
static void event_handler_xMenulist1(lv_obj_t *obj, lv_event_t event);
static void event_handler_xListBtn(lv_obj_t *obj, lv_event_t event);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * @brief event handler Call backs to navigate menu screen items
 *
 * @param obj lvgl object of the menu
 * @param event event that has been called
 */
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        char btntext[10];
        sprintf(btntext, "%s", lv_list_get_btn_text(obj));

        if (!strcmp(btntext, MenuBTN_SERVICE))
        {
            xScreenPidTune();
        }
        if (!strcmp(btntext, MenuBTN_ARCHIV))
        {
            iArchORSummaryScrn = 1;
            lv_task_del(refresherTask);
            refresherTask = NULL;
            xseSummaryEndScreen();
        }
        if (!strcmp(btntext, MenuBTN_METROLOGY))
        {
            lv_task_del(refresherTask);
            refresherTask = NULL;
            Screen_Password(SCR_METROLOGY_PASSWORD);
        }
        if (!strcmp(btntext, MenuBTN_PARAMETER))
        {
            lv_task_del(refresherTask);
            refresherTask = NULL;

            ppxParameterScreen();
        }
        if (!strcmp(btntext, MenuBTN_INFO))
        {
            lv_task_del(refresherTask);
            refresherTask = NULL;
            CallScreenInfo();
        }
    }
}

bool get_archiv_or_summary_screen_stat(void)
{
    return iArchORSummaryScrn;
}

void set_archiv_or_summary_screen(bool val)
{
    iArchORSummaryScrn = val;
}

// Envent handler for menuscreen touching off
static void event_handler_xMenulist1(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_obj_set_hidden(xMenulist1, true);
        lv_obj_align(container, NULL, LV_ALIGN_CENTER, 0, 0);
    }
}

// Event handler associated to menu icon btn to call menu screen

bool bxCatchMenuClick = 0;
static void event_handler_xListBtn(lv_obj_t *obj, lv_event_t event)
{

    if (event == LV_EVENT_RELEASED)
    {

        if (bxCatchMenuClick == 0)
        {
            lv_obj_set_hidden(xMenulist1, false);
            lv_obj_align(container, NULL, LV_ALIGN_CENTER, 250, 0);
        }
        if (bxCatchMenuClick == 1)
        {
            lv_obj_set_hidden(xMenulist1, true);
            lv_obj_align(container, NULL, LV_ALIGN_CENTER, 0, 0);
            // lv_anim_start(&a);
        }
        if (bxCatchMenuClick == 0)
        {
            bxCatchMenuClick = bxCatchMenuClick + 1;
        }
        else
        {
            bxCatchMenuClick = 0;
        }
    }
}

void pxDashboardScreen(void)
{

    scr_dashbord = lv_obj_create(NULL, NULL);
    lv_scr_load(scr_dashbord);

    if ((dashboardflg != 1) && (crnt_screen != NULL))
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    xParentcontainer = lv_cont_create(scr_dashbord, NULL);
    lv_obj_set_size(xParentcontainer, 320, 480);
    lv_obj_align(xParentcontainer, NULL, LV_ALIGN_CENTER, 0, 0);                                                            //====================>>>>>
    lv_obj_set_style_local_bg_color(xParentcontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); // 0x39, 0x89, 0xBD,, 0x5D, 0x5D, 0x5D
    lv_obj_set_style_local_border_opa(xParentcontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(xParentcontainer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    container = lv_cont_create(xParentcontainer, NULL);
    lv_obj_set_size(container, 320, 480);
    lv_obj_align(container, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); //====================>>>>>
    lv_obj_set_style_local_border_opa(container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    _xContainerStatusBar = lv_cont_create(container, NULL);
    lv_obj_set_size(_xContainerStatusBar, 320, 75);
    lv_obj_align(_xContainerStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_xContainerStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); //====================>>>>>
    lv_obj_set_style_local_border_opa(_xContainerStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    _xTimeLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xTimeLabel, _xContainerStatusBar, LV_ALIGN_IN_TOP_LEFT, 8, 5);
    // lv_label_set_text(_xTimeLabel, "4:30");
    lv_label_set_text(_xTimeLabel, guiTime);

    static lv_style_t _xTimeLabelStyle;
    lv_style_init(&_xTimeLabelStyle);
    lv_style_set_text_font(&_xTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xTimeLabel, LV_LABEL_PART_MAIN, &_xTimeLabelStyle);
    refresherTask = lv_task_create(_xTimeLabel_refr_func, 1000, LV_TASK_PRIO_HIGHEST, NULL);

    // Create Label for Battery icon
    _xBatteryLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xBatteryLabel, _xContainerStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(_xBatteryLabel, LV_SYMBOL_BATTERY_FULL); // LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xBatteryLabelStyle;
    lv_style_init(&_xBatteryLabelStyle);
    lv_style_set_text_font(&_xBatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&_xBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xBatteryLabel, LV_LABEL_PART_MAIN, &_xBatteryLabelStyle);

    // Create Label for Wifi icon
    _xWifiLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xWifiLabel, _xBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(_xWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t _xWifiLabelStyle;
    lv_style_init(&_xWifiLabelStyle);
    lv_style_set_text_font(&_xWifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xWifiLabel, LV_LABEL_PART_MAIN, &_xWifiLabelStyle);

    // Create Label for Signal icon
    _xSignalLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xSignalLabel, _xWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(_xSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t _xSignalLabelStyle;
    lv_style_init(&_xSignalLabelStyle);
    lv_style_set_text_font(&_xSignalLabelStyle, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&_xSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xSignalLabel, LV_LABEL_PART_MAIN, &_xSignalLabelStyle);

    //=======================================================================================

    // Create List Button
    _xListBtn = lv_btn_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xListBtn, _xTimeLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -17);
    lv_obj_set_size(_xListBtn, 44, 44);
    lv_obj_set_event_cb(_xListBtn, event_handler_xListBtn);
    // lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);

    static lv_style_t _xListBtnStyle;
    lv_style_init(&_xListBtnStyle);
    lv_style_set_radius(&_xListBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&_xListBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)); //====================>>>>>
    // lv_style_set_border_color(&_xListBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_opa(&_xListBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN);
    lv_style_set_border_width(&_xListBtnStyle, LV_STATE_PRESSED, 0);
    lv_obj_add_style(_xListBtn, LV_BTN_PART_MAIN, &_xListBtnStyle);

    // create label for list
    _xListLabelClick = lv_label_create(_xListBtn, NULL);
    lv_obj_align(_xListLabelClick, _xTimeLabel, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_click(_xListLabelClick, false);
    lv_label_set_text(_xListLabelClick, LV_SYMBOL_LIST);

    static lv_style_t _xListLabelClickStyle;
    lv_style_init(&_xListLabelClickStyle);
    lv_style_set_text_font(&_xListLabelClickStyle, LV_STATE_DEFAULT, &lv_font_montserrat_36);
    lv_style_set_text_color(&_xListLabelClickStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&_xListLabelClickStyle, LV_LABEL_PART_MAIN, 0);
    lv_style_set_border_opa(&_xListLabelClickStyle, LV_LABEL_PART_MAIN, 0);
    lv_obj_add_style(_xListLabelClick, LV_LABEL_PART_MAIN, &_xListLabelClickStyle);

    //=======================================================================================

    _xWelcomeMsgLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xWelcomeMsgLabel, _xWifiLabel, LV_ALIGN_OUT_BOTTOM_MID, -90, 0);
    lv_label_set_text(_xWelcomeMsgLabel, "Welcome Steve");

    static lv_style_t _xWelcomeMsgLabelStyle;
    lv_style_init(&_xWelcomeMsgLabelStyle);
    lv_style_set_text_font(&_xWelcomeMsgLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_xWelcomeMsgLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xWelcomeMsgLabel, LV_LABEL_PART_MAIN, &_xWelcomeMsgLabelStyle);

    // Create todays date label
    _xTodaysDateLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xTodaysDateLabel, _xWelcomeMsgLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xTodaysDateLabel, today_Date_Msg); // today_Date_Msg, "Today is 19, DEC 2021"

    static lv_style_t _xTodaysDateLabelStyle;
    lv_style_init(&_xTodaysDateLabelStyle);
    lv_style_set_text_font(&_xTodaysDateLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_xTodaysDateLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_add_style(_xTodaysDateLabel, LV_LABEL_PART_MAIN, &_xTodaysDateLabelStyle);

    //============================================================================================

    // IW_create = pxCreateResumeInfo(container);
    // lv_obj_align(IW_create, NULL, LV_ALIGN_CENTER, 0,0 );
    // vSetResumeInfoState(IW_create, RESUMEINFO_READY , "Ready");
    // vSetResumeInfoState(IW_create, RESUMEINFO_WORK_IN_PROGRESS , "    Work in progress");
    // vSetResumeInfoState(IW_create, RESUMEINFO_WAIT , "  Wait in progress");
    // vSetResumeInfoState(IW_create, RESUMEINFO_EXPORT_DATA , "      Export In Progress");
    // vSetResumeInfoState(IW_create, RESUMEINFO_METROLOGY_NEEDED , "     Metrology needed");
    // vSetResumeInfoState(IW_create, RESUMEINFO_ALERT_SERVICE , "Alert Service");
    // vSetResumeInfoState(IW_create, RESUMEINFO_JOB_FINISHED , "Job finished");
    // vSetResumeInfoState(IW_create, RESUMEINFO_PROBLEM , "Problem");
    // vSetResumeInfoPercent(IW_create, 62)    ;
    // vSetResumeInfoLitersInt(IW_create, 4050);
    // vSetResumeInfoLitersFloat(IW_create, 65);
    // vSetResumeInfoHour(IW_create, 1100, 58) ;
    // lv_obj_set_style_local_bg_color(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x38, 0x38, 0x38) );
    // lv_obj_set_style_local_border_opa(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    DashboardInfoWidget();

    //===================================================================================================================================

    // Creat a stop Button

    _xStopBtn = lv_btn_create(container, NULL);
    lv_obj_align(_xStopBtn, IW_create, LV_ALIGN_OUT_BOTTOM_MID, -85, 20);
    lv_obj_set_size(_xStopBtn, 300, 44);
    lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_BTN_STATE_PRESSED, LV_COLOR_BLUE);
    lv_obj_set_event_cb(_xStopBtn, BTN_event_handler);
    // lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);

    static lv_style_t _xStopBtnStyle;
    lv_style_init(&_xStopBtnStyle);
    lv_style_set_radius(&_xStopBtnStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_radius(&_xStopBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&_xStopBtnStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&_xStopBtnStyle, LV_STATE_DEFAULT, 0);
    // lv_style_set_radius(&xStopButtonLabelStyle, LV_LABEL_PART_MAIN,0);
    lv_style_set_bg_color(&_xStopBtnStyle, LV_STATE_DEFAULT | LV_STATE_PRESSED | LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    // LV_COLOR_MAKE(0x5D, 0xAF, 0x48)

    //   !strcmp(btntext , MenuBTN_PARAMETER )
    char *stop1 = "STOP";
    if (!strcmp(dashboardBTNTxt, stop1))
    {
        lv_style_set_bg_color(&_xStopBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEB, 0x3B, 0x5A)); // #359Fe2, 0x35, 0x9F, 0xE2
    }
    else
    {
        lv_style_set_bg_color(&_xStopBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2)); // #359Fe2, 0xEB, 0x3B, 0x5A
    }

    // lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEB, 0x3B, 0x5A));

    lv_obj_add_style(_xStopBtn, LV_BTN_PART_MAIN, &_xStopBtnStyle);

    // Creat a stop Button Label
    xStopButtonLabel = lv_label_create(_xStopBtn, NULL);
    lv_obj_align(xStopButtonLabel, _xStopBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(xStopButtonLabel, dashboardBTNTxt); // dashboardBTNTxt

    static lv_style_t xStopButtonLabelStyle;
    lv_style_init(&xStopButtonLabelStyle);
    lv_style_set_text_font(&xStopButtonLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&xStopButtonLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_border_width(&xStopButtonLabelStyle, LV_LABEL_PART_MAIN, 0);
    lv_style_set_border_opa(&xStopButtonLabelStyle, LV_LABEL_PART_MAIN, 0);
    lv_style_set_radius(&xStopButtonLabelStyle, LV_LABEL_PART_MAIN, 0);
    lv_obj_add_style(xStopButtonLabel, LV_LABEL_PART_MAIN, &xStopButtonLabelStyle);
    // lv_obj_set_style_local_border_width(_xStartBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);

    //========================================================================================
    // Create a Menu List list
    xMenulist1 = lv_list_create(xParentcontainer, NULL);
    // lv_obj_set_size(xMenulist1, 160, 200);
    lv_obj_align(xMenulist1, xParentcontainer, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_set_height(xMenulist1, 480);
    lv_obj_set_width(xMenulist1, 250);
    lv_obj_set_hidden(xMenulist1, true);
    lv_obj_set_click(xMenulist1, true);
    lv_obj_set_event_cb(xMenulist1, event_handler_xMenulist1);
    // lv_obj_set_style_local_margin_all(xMenulist1, LV_LIST_PART_BG ,LV_STATE_DEFAULT,5);

    static lv_style_t listStyle;
    lv_style_init(&listStyle);
    lv_style_set_bg_color(&listStyle, LV_LIST_PART_BG, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_style_set_bg_opa(&listStyle, LV_LIST_PART_BG, LV_OPA_MIN);
    lv_style_set_border_opa(&listStyle, LV_LIST_PART_EDGE_FLASH, LV_OPA_MIN);
    lv_style_set_border_width(&listStyle, LV_LIST_PART_EDGE_FLASH, 0);
    lv_obj_add_style(xMenulist1, LV_STATE_DEFAULT, &listStyle);

    /*Add buttons to the list*/
    // lv_obj_t * list_btn;
    list_btn = lv_list_add_btn(xMenulist1, &navier_logo, NULL); // LV_SYMBOL_FILE
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_click(list_btn, false);
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));

    list_btn = lv_list_add_btn(xMenulist1, NULL, NULL); // LV_SYMBOL_FILE
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_click(list_btn, false);
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_opa(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    list_btn = lv_list_add_btn(xMenulist1, &service_icon, "PID TUNE"); // LV_SYMBOL_FILE
    // lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN ,LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35,0x9F,0xE2));
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
    // lv_obj_set_style_local_bg_opa(list_btn, LV_BTN_PART_MAIN ,LV_STATE_DEFAULT,LV_OPA_60);
    lv_obj_set_style_local_margin_all(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);

    list_btn = lv_list_add_btn(xMenulist1, &archiv_icon, "ARCHIV");
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
    // lv_obj_set_style_local_bg_opa(list_btn, LV_BTN_PART_MAIN ,LV_STATE_DEFAULT,LV_OPA_60);
    lv_obj_set_style_local_margin_all(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);

    list_btn = lv_list_add_btn(xMenulist1, &metrology_icon, "METROLOGY");
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
    // lv_obj_set_style_local_bg_opa(list_btn, LV_BTN_PART_MAIN ,LV_STATE_DEFAULT,LV_OPA_60);
    lv_obj_set_style_local_margin_all(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);

    list_btn = lv_list_add_btn(xMenulist1, &parameter_icon, "PARAMETER");
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
    // lv_obj_set_style_local_bg_opa(list_btn, LV_BTN_PART_MAIN ,LV_STATE_DEFAULT,LV_OPA_60);
    lv_obj_set_style_local_margin_all(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);

    list_btn = lv_list_add_btn(xMenulist1, &info_icon, "INFO");
    lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_event_cb(list_btn, event_handler);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
    // lv_obj_set_style_local_bg_opa(list_btn, LV_BTN_PART_MAIN ,LV_STATE_DEFAULT,LV_OPA_60);
    lv_obj_set_style_local_margin_all(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 4);
    lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);

    //===================================================================================

    //=========================================Language Test Code=========================

    //=====================================================================================
    crnt_screen = scr_dashbord;
    screenid = SCR_DASHBOARD;
}

void DashboardInfoWidget(void)
{
    switch (dashboardflg)
    {
    case 0: // Ready Mode
        xBTN = 0;
        IW_create = pxCreateResumeInfo(container);
        lv_obj_align(IW_create, NULL, LV_ALIGN_CENTER, 0, 0);
        vSetResumeInfoState(IW_create, RESUMEINFO_READY, "Ready");
        vSetResumeInfoLitersInt(IW_create, uGetTotalLiterIntegerPart());
        vSetResumeInfoLitersFloat(IW_create, uGetTotalLiterFloatPart());
        vSetResumeInfoHour(IW_create, uGetTotalHoursIntegerPart(), uGetTotalHoursFloatPart());
        lv_obj_set_style_local_bg_color(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
        lv_obj_set_style_local_border_opa(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);
        readyModeBuzzBeep();
        break;

    case 1: // Work in progress
        xBTN = 3;
        IW_create = pxCreateResumeInfo(container);
        lv_obj_align(IW_create, NULL, LV_ALIGN_CENTER, 0, 0);
        vSetResumeInfoState(IW_create, RESUMEINFO_WORK_IN_PROGRESS, "    Work in progress");
        vSetResumeInfoPercent(IW_create, 0);
        vSetResumeInfoLitersInt(IW_create, uGetTotalLiterIntegerPart());
        vSetResumeInfoLitersFloat(IW_create, uGetTotalLiterFloatPart());
        // vSetResumeInfoHour(IW_create, 0000, 00)  ;
        vSetResumeInfoHour(IW_create, uGetTotalHoursIntegerPart(), uGetTotalHoursFloatPart());
        vSetResumeInfoRemainingHour(IW_create, 0);
        vSetResumeInfoRemainingMinute(IW_create, 00);

        vSetResumeInfoTotalSeq(IW_create, uGetCurrentRunningSequenceNumber());
        vSetResumeInfoCurrentSeq(IW_create, uGetNoOfSequenceInArray());
        lv_obj_set_style_local_bg_color(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
        lv_obj_set_style_local_border_opa(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

        lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
        lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEB, 0x3B, 0x5A));
        workInProgressBuzzBeep();
        break;

    case 2: // Work Finished
        xBTN = 5;
        IW_create = pxCreateResumeInfo(container);
        lv_obj_align(IW_create, NULL, LV_ALIGN_CENTER, 0, 0);
        vSetResumeInfoState(IW_create, RESUMEINFO_JOB_FINISHED, "Job finished");
        vSetResumeInfoLitersInt(IW_create, uGetTotalLiterIntegerPart());
        vSetResumeInfoLitersFloat(IW_create, uGetTotalLiterFloatPart());
        vSetResumeInfoHour(IW_create, uGetTotalHoursIntegerPart(), uGetTotalHoursFloatPart());
        lv_obj_set_style_local_bg_color(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
        lv_obj_set_style_local_border_opa(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);
        /* Setting the label text to the view summary */
        lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
        lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
        jobFinishedModeBuzzBeep();
        break;

    case 3: // Wait In Progress
        xBTN = 4;
        IW_create = pxCreateResumeInfo(container);
        lv_obj_align(IW_create, NULL, LV_ALIGN_CENTER, 0, 0);
        vSetResumeInfoState(IW_create, RESUMEINFO_WAIT, "  Wait in progress");
        vSetResumeInfoLitersInt(IW_create, uGetTotalLiterIntegerPart());
        vSetResumeInfoLitersFloat(IW_create, uGetTotalLiterFloatPart());
        vSetResumeInfoHour(IW_create, uGetTotalHoursIntegerPart(), uGetTotalHoursFloatPart());
        lv_obj_set_style_local_bg_color(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
        lv_obj_set_style_local_border_opa(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

        // lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
        // lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
        waitModeBuzzBeep();
        break;
    }
}

/**
 * @brief this task function will update the time in the upper left corner
 *
 * @param refresherTask task handle
 */
static void _xTimeLabel_refr_func(lv_task_t *refresherTask)
{
    if (lv_obj_get_screen(_xTimeLabel) == lv_scr_act())
    {
        lv_label_set_text(_xTimeLabel, guiTime);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief This is the handler of the button which displayed on the botton of the dashboard screen
 *
 * @param obj object of the button
 * @param event event of the button
 */
static void BTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        ESP_LOGD(TAG, "Button Pressed : %d", xBTN);
        switch (xBTN)
        {
        case 0:
            lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_BTN_STATE_PRESSED, LV_COLOR_BLUE);
            fflush(NULL);
            lv_task_del(refresherTask);
            refresherTask = NULL;
            xsPresetScreenAdvance();
            break;

        case 1:
            // printf("Problem State\n");
            break;

        case 2:
            // printf("Metrology In Progress State\n");
            break;

        case 3:
            lv_task_del(refresherTask);
            refresherTask = NULL;
            vControllerSampleStop();
            xseSummaryEndScreen();
            break;

        case 4:
            lv_task_del(refresherTask);
            refresherTask = NULL;
            vControllerSampleStop();
            xseSummaryEndScreen();
            break;

        case 5:
            lv_task_del(refresherTask);
            refresherTask = NULL;
            xseSummaryEndScreen();
            break;
        case 6:
            break;

        case 7:
            break;

        case 8:
            break;
        }
    }
}


void vUpdateDashboardScreen(void)
{
    /* updating the total liters and total hours */
    vSetResumeInfoLitersInt(IW_create, uGetTotalLiterIntegerPart());
    vSetResumeInfoLitersFloat(IW_create, uGetTotalLiterFloatPart());
    vSetResumeInfoHour(IW_create, uGetTotalHoursIntegerPart(), uGetTotalHoursFloatPart());

    /* updating the pecentage completed */
    vSetResumeInfoPercent(IW_create, fGetPercentageOfJobDone());

    /* need to add function to get the remainin hours and minutes from backend */
    uint16_t hour = 0;
    uint8_t min = 0;
    vGetNumberOfHourAndMinutesLeftInStartingSequence(&hour, &min);
    vSetResumeInfoRemainingHour(IW_create, hour);
    vSetResumeInfoRemainingMinute(IW_create, min);
}

static void vShowJobFinishedDashBoardScreenTask(lv_task_t *showJobFinishedTask)
{
    dashboardflg = 2;
    DashboardInfoWidget();
    lv_task_del(showJobFinishedTask);
    showJobFinishedTask = NULL;
}

void vShowJobFinishedDashboardScreen()
{
    showJobFinishedTask = lv_task_create(vShowJobFinishedDashBoardScreenTask, 1 * 1000, LV_TASK_PRIO_MID, NULL);
}

/**********************
 *    ERROR ASSERT
 **********************/
