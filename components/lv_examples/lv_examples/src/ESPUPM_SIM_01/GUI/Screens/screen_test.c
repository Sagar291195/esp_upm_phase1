/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"


/*********************
 *      DEFINES
 *********************/

#define IW_WIDTH  (200)
#define IW_HEIGHT (290)

#define SYMBOL_SIGNAL "\uf012"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_style_t style_scr1_container2;
lv_obj_t *container3;
static lv_style_t style_scr1_Arc;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
lv_obj_t * scr2; // 
lv_obj_t * scr3;
lv_obj_t * label2; //
lv_obj_t * pInfoWidget;

lv_obj_t * list1;


/***************************
 *Declaration
 ***************************/

LV_FONT_DECLARE(signal_20)

/**********************
 *   GLOBAL FUNCTIONS
 **********************/



static void arc_loader(lv_task_t * t)
{
    static int16_t a = 270;

    a+=5;

    lv_arc_set_end_angle(t->user_data, a);

    if(a >= 270 + 300) {
        lv_task_del(t);
        return;
    }
}

void vTestScreen(void)
{
    scr2 = lv_obj_create(NULL, NULL);
    lv_scr_load(scr2);

    // Make a container
    lv_obj_t *container2 = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(container2, IW_WIDTH, IW_HEIGHT); // (200, 290)
    //lv_obj_set_size(container2, 300, 290);
    lv_obj_align(container2, NULL, LV_ALIGN_CENTER, 0,0);
    
    //Set container style
    lv_style_init(&style_scr1_container2);
    lv_style_set_bg_color(&style_scr1_container2,LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_add_style(container2, LV_CONT_PART_MAIN, &style_scr1_container2);
    
    //Make an Arc
    lv_obj_t * arc = lv_arc_create(container2, NULL);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_arc_set_angles(arc, 270, 270);
    lv_obj_set_size(arc, 200, 200);
    lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_line_width(arc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 10 );

    //Set Arc Style
    lv_style_init(&style_scr1_Arc);
    lv_style_set_bg_color(&style_scr1_Arc,LV_ARC_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_color(&style_scr1_Arc, LV_ARC_PART_BG, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_opa(&style_scr1_Arc, LV_ARC_PART_BG, LV_OPA_MIN);
    lv_style_set_line_width(&style_scr1_Arc,LV_ARC_PART_BG, 10);
    lv_style_set_line_width(&style_scr1_Arc,LV_ARC_PART_INDIC, 10);
    
    //lv_obj_set_style_local_border_opa(arc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);
    //lv_obj_set_style_local_bg_opa(arc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);
    

    //lv_style_set_outline_width(&style_scr1_Arc, LV_ARC_PART_BG, 10 );
    lv_style_set_line_opa(&style_scr1_Arc, LV_STATE_DEFAULT, LV_OPA_10);
    
    //lv_style_set_line_width(&style_scr1_Arc,_LV_ARC_, 5);
    lv_obj_add_style(arc, LV_ARC_PART_BG, &style_scr1_Arc);

    lv_task_create(arc_loader, 20, LV_TASK_PRIO_LOWEST, arc);
    
    // Create label for status
    lv_obj_t * Status_Label = lv_label_create(container2, NULL);
    lv_obj_align(Status_Label, container2, LV_ALIGN_IN_TOP_LEFT, 10,20);
    lv_label_set_text(Status_Label, "Work in progress");
 
    static lv_style_t style_Status_label;
    lv_style_init(&style_Status_label);
    lv_style_set_text_font(&style_Status_label, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    //lv_style_set_text_letter_space(&style_Status_label, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_Status_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(Status_Label, LV_LABEL_PART_MAIN, &style_Status_label);

    //create percent label

    lv_obj_t * percent_Label = lv_label_create(container2, NULL);
    lv_obj_align(percent_Label, container2, LV_ALIGN_IN_TOP_MID, 0, 110);
    lv_label_set_text(percent_Label, "");
 
    static lv_style_t style_percent_label;
    lv_style_init(&style_percent_label);
    lv_style_set_text_font(&style_percent_label, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    //lv_style_set_text_letter_space(&style_Status_label, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_percent_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(percent_Label, LV_LABEL_PART_MAIN, &style_percent_label);

    //Create Hour Label

    lv_obj_t * hour_Label = lv_label_create(container2, NULL);
    lv_obj_align(hour_Label, container2, LV_ALIGN_IN_TOP_MID, -22, 142);
    lv_label_set_text(hour_Label, "1 Hr 22 Min");
 
    static lv_style_t style_hour_label;
    lv_style_init(&style_hour_label);
    lv_style_set_text_font(&style_hour_label, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); 
    //lv_style_set_text_letter_space(&style_Status_label, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_hour_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(hour_Label, LV_LABEL_PART_MAIN, &style_hour_label);

    //Create label for Task Number

    lv_obj_t * taskNum_Label = lv_label_create(container2, NULL);
    lv_obj_align(taskNum_Label, container2, LV_ALIGN_IN_TOP_MID, 0, 165);
    lv_label_set_text(taskNum_Label, "01/01");
 
    static lv_style_t style_taskNum_Label;
    lv_style_init(&style_taskNum_Label);
    lv_style_set_text_font(&style_taskNum_Label, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); 
    //lv_style_set_text_letter_space(&style_Status_label, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_taskNum_Label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(taskNum_Label, LV_LABEL_PART_MAIN, &style_hour_label);

    lv_obj_t * hor_line = lv_line_create(container2, NULL);
   
    //Create Horizontal Line
    static lv_point_t p[] = {{0, 25}, {0, 65}};
    lv_line_set_points(hor_line, p, 2);
    lv_obj_align(hor_line, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    static lv_style_t style_hor_line;
    lv_style_init(&style_hor_line);
    lv_style_set_line_color(&style_hor_line, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_width(&style_hor_line, LV_STATE_DEFAULT, 3);
    lv_style_set_line_rounded(&style_hor_line, LV_STATE_DEFAULT, true);
    lv_obj_add_style(hor_line, LV_LINE_PART_MAIN, &style_hor_line);

    //Create Total Liters label Fix

    lv_obj_t * fix_liter_Label = lv_label_create(container2, NULL);
    lv_obj_align(fix_liter_Label, container2, LV_ALIGN_IN_BOTTOM_LEFT, 10, -40);
    lv_label_set_text(fix_liter_Label, "Total Liters");
 
    static lv_style_t style_fix_liter_Label;
    lv_style_init(&style_fix_liter_Label);
    lv_style_set_text_font(&style_fix_liter_Label, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); 
    //lv_style_set_text_letter_space(&style_Status_label, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_fix_liter_Label, LV_LABEL_PART_MAIN, LV_COLOR_CYAN);
    lv_obj_add_style(fix_liter_Label, LV_LABEL_PART_MAIN, &style_fix_liter_Label);

    //Create total hour label fix

    lv_obj_t * fix_hour_Label = lv_label_create(container2, NULL);
    lv_obj_align(fix_hour_Label, container2, LV_ALIGN_IN_BOTTOM_RIGHT, -60, -40);
    lv_label_set_text(fix_hour_Label, "Total Hours");
 
    static lv_style_t style_fix_hour_Label;
    lv_style_init(&style_fix_hour_Label);
    lv_style_set_text_font(&style_fix_hour_Label, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); 
    //lv_style_set_text_letter_space(&style_Status_label, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_fix_hour_Label, LV_LABEL_PART_MAIN, LV_COLOR_CYAN);
    lv_obj_add_style(fix_hour_Label, LV_LABEL_PART_MAIN, &style_fix_hour_Label);

    //Create Total Liters label Var

    lv_obj_t * var_liter_Label = lv_label_create(container2, NULL);
    lv_obj_align(var_liter_Label, container2, LV_ALIGN_IN_BOTTOM_LEFT, 30, -20);
    //lv_obj_set_width(var_liter_Label, 200);
    lv_label_set_text(var_liter_Label, "1000");
    lv_label_set_align(var_liter_Label, LV_LABEL_ALIGN_CENTER);
    
 
    static lv_style_t style_var_liter_Label;
    lv_style_init(&style_var_liter_Label);
    lv_style_set_text_font(&style_var_liter_Label, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&style_var_liter_Label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(var_liter_Label, LV_LABEL_PART_MAIN, &style_var_liter_Label);

    //Create total hour label Var

    lv_obj_t * var_hour_Label = lv_label_create(container2, NULL);
    lv_obj_align(var_hour_Label, container2, LV_ALIGN_IN_BOTTOM_RIGHT, -40, -20);
    lv_label_set_text(var_hour_Label, "1500");
 
    static lv_style_t style_var_hour_Label;
    lv_style_init(&style_var_hour_Label);
    lv_style_set_text_font(&style_var_hour_Label, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&style_var_hour_Label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(var_hour_Label, LV_LABEL_PART_MAIN, &style_var_hour_Label);

}


void vTestScreen_test(void)
{
    
    scr3 = lv_obj_create(NULL, NULL);
    lv_scr_load(scr3);

    // Make a container
    container3 = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(container3, IW_WIDTH, IW_HEIGHT); // (200, 290)
    //lv_obj_set_size(container2, 300, 290);
    lv_obj_align(container3, NULL, LV_ALIGN_CENTER, 0,0);
    
    //Set container style
    static lv_style_t style_scr1_container3;
    lv_style_init(&style_scr1_container3);
    lv_style_set_bg_color(&style_scr1_container3,LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_add_style(container3, LV_CONT_PART_MAIN, &style_scr1_container2);
    
    //pxCreateResumeInfo(scr3);
}

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked: %s\n", lv_list_get_btn_text(obj));
    }
}


//event_handler_xListBtn


bool i = 0;
static void event_handler_xListBtn(lv_obj_t * obj, lv_event_t event)
{
    
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");

        if (i == 0)
        {
            lv_obj_set_hidden(list1, false) ;
            //i = 1;
        }
        if (i==1)
        {
            lv_obj_set_hidden(list1, true) ;
            
        }

        if (i == 0)
        { 
            i = i+1;  
        } else {
            i = 0;
        }   
          
    }
    // else if(event == LV_EVENT_VALUE_CHANGED) {
    //     printf("Toggled\n");
    // }
}

void layout1(void)
{
    lv_obj_t * container = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(container, 320, 480);
    lv_obj_align(container, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color(container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x5D, 0x5D, 0x5D) );
    
    lv_obj_t * _xContainerStatusBar = lv_cont_create(container, NULL);
    lv_obj_set_size(_xContainerStatusBar, 320, 75);
    lv_obj_align(_xContainerStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_xContainerStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x5D, 0x5D, 0x5D) );
    lv_obj_set_style_local_border_opa(_xContainerStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner

    lv_obj_t * _xTimeLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xTimeLabel, _xContainerStatusBar, LV_ALIGN_IN_TOP_LEFT, 9,5);
    lv_label_set_text(_xTimeLabel, "04:55");

    static lv_style_t _xTimeLabelStyle;
    lv_style_init(&_xTimeLabelStyle);
    lv_style_set_text_font(&_xTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_xTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xTimeLabel, LV_LABEL_PART_MAIN, &_xTimeLabelStyle);

    //Create Label for Battery icon
    lv_obj_t * _xBatteryLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xBatteryLabel, _xContainerStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(_xBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t _xBatteryLabelStyle;
    lv_style_init(&_xBatteryLabelStyle);
    lv_style_set_text_font(&_xBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&_xBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xBatteryLabel, LV_LABEL_PART_MAIN, &_xBatteryLabelStyle);

    //Create Label for Wifi icon
    //Create Label for Signal icon
    lv_obj_t * _xWifiLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xWifiLabel, _xBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 3);
    lv_label_set_text(_xWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t _xWifiLabelStyle;
    lv_style_init(&_xWifiLabelStyle);
    lv_style_set_text_font(&_xWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&_xWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xWifiLabel, LV_LABEL_PART_MAIN, &_xWifiLabelStyle);

    //Create Label for Signal icon
    lv_obj_t * _xSignalLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xSignalLabel, _xWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 3);
    lv_label_set_text(_xSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t _xSignalLabelStyle;
    lv_style_init(&_xSignalLabelStyle);
    lv_style_set_text_font(&_xSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&_xSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xSignalLabel, LV_LABEL_PART_MAIN, &_xSignalLabelStyle);

    //=======================================================================================

    //Create List Button

    lv_obj_t * _xListBtn = lv_btn_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xListBtn, _xTimeLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 3, 5);
    lv_obj_set_size(_xListBtn, 44, 44);
    lv_obj_set_event_cb(_xListBtn, event_handler_xListBtn);
    //lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);

    static lv_style_t _xListBtnStyle;
    lv_style_init(&_xListBtnStyle);
    lv_style_set_radius(&_xListBtnStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&_xListBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_color(&_xListBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_opa(&_xListBtnStyle, LV_STATE_DEFAULT, LV_OPA_MIN );
    lv_obj_add_style(_xListBtn, LV_BTN_PART_MAIN, &_xListBtnStyle);

    // create labnel for list

    lv_obj_t * _xListLabelClick = lv_label_create(_xListBtn, NULL);
    lv_obj_align(_xListLabelClick, _xTimeLabel, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_label_set_text(_xListLabelClick, LV_SYMBOL_LIST);

    static lv_style_t _xListLabelClickStyle;
    lv_style_init(&_xListLabelClickStyle);
    lv_style_set_text_font(&_xListLabelClickStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_36); 
    lv_style_set_text_color(&_xListLabelClickStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xListLabelClick, LV_LABEL_PART_MAIN, &_xListLabelClickStyle);

    //=======================================================================================

    lv_obj_t * _xWelcomeMsgLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xWelcomeMsgLabel, _xWifiLabel, LV_ALIGN_OUT_BOTTOM_MID, -90, 0);
    lv_label_set_text(_xWelcomeMsgLabel, "Welcome Steve");

    static lv_style_t _xWelcomeMsgLabelStyle;
    lv_style_init(&_xWelcomeMsgLabelStyle);
    lv_style_set_text_font(&_xWelcomeMsgLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_xWelcomeMsgLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_xWelcomeMsgLabel, LV_LABEL_PART_MAIN, &_xWelcomeMsgLabelStyle);

    //Create todays date label

    lv_obj_t * _xTodaysDateLabel = lv_label_create(_xContainerStatusBar, NULL);
    lv_obj_align(_xTodaysDateLabel, _xWelcomeMsgLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(_xTodaysDateLabel, "Today is 19, DEC 2021");

    static lv_style_t _xTodaysDateLabelStyle;
    lv_style_init(&_xTodaysDateLabelStyle);
    lv_style_set_text_font(&_xTodaysDateLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); 
    lv_style_set_text_color(&_xTodaysDateLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35,0x9F,0xE2));
    lv_obj_add_style(_xTodaysDateLabel, LV_LABEL_PART_MAIN, &_xTodaysDateLabelStyle);

    //=================================================================================================================================

    lv_obj_t *IW_create = pxCreateResumeInfo(container);
    lv_obj_align(IW_create, NULL, LV_ALIGN_CENTER, 0,0 );
    vSetResumeInfoState(IW_create, RESUMEINFO_WORK_IN_PROGRESS , "Work Progress");
    vSetResumeInfoPercent(IW_create, 50);
    vSetResumeInfoLitersInt(IW_create, 4050);
    vSetResumeInfoLitersFloat(IW_create, 85);
    vSetResumeInfoArcTime(IW_create, 25, 52);
    vSetResumeInfoArcTask(IW_create, 10, 8);
    //vSetResumeInfoHourInt(IW_create, 6500);
    //vSetResumeInfoHourFloat(IW_create, 88);
    vSetResumeInfoHour(IW_create, 1100, 58);
    lv_obj_set_style_local_bg_color(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x38, 0x38, 0x38) );
    lv_obj_set_style_local_border_opa(IW_create, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //===================================================================================================================================

    //Creat a stop Button

    lv_obj_t * _xStopBtn = lv_btn_create(container, NULL);
    lv_obj_align(_xStopBtn, IW_create, LV_ALIGN_OUT_BOTTOM_MID, -85, 15 );
    lv_obj_set_size(_xStopBtn, 300, 44);
    //lv_obj_reset_style_list(_xStopBtn, LV_BTN_PART_MAIN);

    static lv_style_t _xStopBtnStyle;
    lv_style_init(&_xStopBtnStyle);
    lv_style_set_radius(&_xStopBtnStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_bg_color(&_xStopBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEb, 0x3B, 0x5A));
    lv_obj_add_style(_xStopBtn, LV_BTN_PART_MAIN, &_xStopBtnStyle);

    //Creat a stop Button Label

    lv_obj_t * xStopButtonLabel = lv_label_create(_xStopBtn, NULL);
    lv_obj_align(xStopButtonLabel, _xStopBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(xStopButtonLabel, "Stop");

    static lv_style_t xStopButtonLabelStyle;
    lv_style_init(&xStopButtonLabelStyle);
    lv_style_set_text_font(&xStopButtonLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&xStopButtonLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(xStopButtonLabel, LV_LABEL_PART_MAIN, &xStopButtonLabelStyle);

//========================================================================================
    //Create a list
    list1 = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_size(list1, 160, 200);
    lv_obj_align(list1, _xListBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_hidden(list1, true);

    static lv_style_t listStyle;
    lv_style_init(&listStyle);


    /*Add buttons to the list*/
    lv_obj_t * list_btn;

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_FILE, "SERVICE");
    lv_obj_set_event_cb(list_btn, event_handler);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, "ARCHIV");
    lv_obj_set_event_cb(list_btn, event_handler);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "METROLOGY");
    lv_obj_set_event_cb(list_btn, event_handler);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_EDIT, "PARAMETER");
    lv_obj_set_event_cb(list_btn, event_handler);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_SAVE, "INFO");
    lv_obj_set_event_cb(list_btn, event_handler);


//===================================================================================

}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
