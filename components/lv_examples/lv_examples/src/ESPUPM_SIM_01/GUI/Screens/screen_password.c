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
#include <esp_log.h>

/*********************
 *      DEFINES
 *********************/
#define TAG             "PASSWORD"


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int screen_password = 1234;
static int metrology_password = 1234;

static void  passwordcheck_event_handler(lv_obj_t * obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/

static const char * fcs_kb_map[] = {
              "1"                  , "2", "3",    "\n",
              "4"                  , "5", "6",    "\n",
              "7"                  , "8", "9",     "\n" , ""
              LV_SYMBOL_BACKSPACE  , "0", "#",     ""
    };

static const lv_btnmatrix_ctrl_t fcs_kb_ctrl[] = {
           80, 80, 80, 80,
           80, 80, 80, 80,
           80, 80, 80, 80,
           
    };

static const lv_btnmatrix_ctrl_t fcs_tgl_kb_ctrl[] = {
           LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
           LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
           LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
           
    };

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

static char *pass;
static char *Re_pass;
bool passNo = false;

lv_task_t *timer_task = NULL;
lv_obj_t *pswdmsg;
lv_obj_t *background;
lv_obj_t *mpsEnterCalValTA; 
lv_obj_t *mpsMetroPswd;
lv_obj_t *labelsymbol;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void Screen_Password(uint8_t screen_id)
{
    //Create Base container
    mpsMetroPswd = lv_obj_create(NULL, NULL);
    lv_scr_load(mpsMetroPswd);
    if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
   //Write style LV_OBJ_PART_MAIN for screen
	static lv_style_t style_screen_main1;
	lv_style_reset(&style_screen_main1);

	//Write style state: LV_STATE_DEFAULT for style_screen_main
	lv_style_set_bg_color(&style_screen_main1, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_main1, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(mpsMetroPswd, LV_OBJ_PART_MAIN, &style_screen_main1);

    //Write codes screen_led_1
	background = lv_led_create(mpsMetroPswd, NULL);
	//Write style LV_LED_PART_MAIN for screen_led_1
	static lv_style_t style_screen_led_1_main;
	lv_style_reset(&style_screen_led_1_main);
	//Write style state: LV_STATE_DEFAULT for style_screen_led_1_main
	lv_style_set_radius(&style_screen_led_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_led_1_main, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_grad_color(&style_screen_led_1_main, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_grad_dir(&style_screen_led_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_led_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_shadow_color(&style_screen_led_1_main, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_shadow_width(&style_screen_led_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_border_color(&style_screen_led_1_main, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_border_width(&style_screen_led_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_border_opa(&style_screen_led_1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(background, LV_LED_PART_MAIN, &style_screen_led_1_main);
	lv_obj_set_pos(background, 0, 0);
	lv_obj_set_size(background, 320, 480);
	lv_led_set_bright(background, 255);

    pswdmsg = lv_label_create(mpsMetroPswd, NULL);
    lv_label_set_align(pswdmsg, LV_LABEL_ALIGN_CENTER);
    lv_label_set_long_mode(pswdmsg, LV_LABEL_LONG_BREAK);
    if(screen_id == SCR_CHANGE_PASSWORD){
        lv_label_set_text(pswdmsg, "Enter New Metrology Code");
    } else if(screen_id == SCR_PASSWORD){
        lv_label_set_text(pswdmsg, "Enter Code ");
    } else if(screen_id == SCR_METROLOGY_PASSWORD){
        lv_label_set_text(pswdmsg, "Enter Metrology Code");
    }

    static lv_style_t style_label_title;
	lv_style_reset(&style_label_title);
	//Write style state: LV_STATE_DEFAULT for style_bluetooth_labelinstruction_main
	lv_style_set_radius(&style_label_title, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_label_title, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_grad_color(&style_label_title, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_grad_dir(&style_label_title, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_label_title, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_label_title, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_label_title, LV_STATE_DEFAULT, &lv_font_montserrat_24);
	lv_style_set_text_letter_space(&style_label_title, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_label_title, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_label_title, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_label_title, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_label_title, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(pswdmsg, LV_LABEL_PART_MAIN, &style_label_title);
    lv_obj_set_pos(pswdmsg, 40, 10);
    lv_obj_set_size(pswdmsg, 240, 0);

    // Create Text Area
    mpsEnterCalValTA = lv_textarea_create(mpsMetroPswd, NULL); 
	lv_obj_set_size(mpsEnterCalValTA, 100, 60);
	lv_obj_set_pos(mpsEnterCalValTA, 120, 70);
    lv_obj_set_click(mpsEnterCalValTA, false); 
    lv_textarea_set_pwd_mode(mpsEnterCalValTA, true); 
    lv_textarea_set_text(mpsEnterCalValTA, "");    /*Set an initial text*/
	lv_textarea_set_one_line(mpsEnterCalValTA, true);
    lv_textarea_set_cursor_hidden(mpsEnterCalValTA, true);
    lv_textarea_set_text_align(mpsEnterCalValTA, LV_LABEL_ALIGN_LEFT);
	
	static lv_style_t txtareastyle;
	lv_style_reset(&txtareastyle);
    lv_style_set_bg_color(&txtareastyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_opa(&txtareastyle, LV_STATE_DEFAULT, 255);
	lv_style_set_border_opa(&txtareastyle, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&txtareastyle, LV_STATE_DEFAULT, 1);
	lv_style_set_text_color(&txtareastyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&txtareastyle, LV_STATE_DEFAULT, &lv_font_montserrat_48);
    lv_style_set_text_letter_space(&txtareastyle, LV_STATE_DEFAULT, 7);
	lv_style_set_pad_left(&txtareastyle, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&txtareastyle, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&txtareastyle, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(mpsEnterCalValTA, LV_TEXTAREA_PART_BG, &txtareastyle);      /*Default button style*/
	
	static lv_style_t txtareastyle_focused;
	lv_style_reset(&txtareastyle_focused);
    lv_style_set_bg_color(&txtareastyle_focused, LV_STATE_FOCUSED, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_opa(&txtareastyle_focused, LV_STATE_FOCUSED, 255);
	lv_style_set_border_opa(&txtareastyle_focused, LV_STATE_FOCUSED, 0);
	lv_style_set_border_width(&txtareastyle_focused, LV_STATE_FOCUSED, 1);
	lv_style_set_text_color(&txtareastyle_focused, LV_STATE_FOCUSED, LV_COLOR_WHITE);
	lv_style_set_text_font(&txtareastyle_focused, LV_STATE_FOCUSED, &lv_font_montserrat_48);
	lv_style_set_text_letter_space(&txtareastyle_focused, LV_STATE_FOCUSED, 7);
	lv_style_set_pad_left(&txtareastyle_focused, LV_STATE_FOCUSED, 0);
	lv_style_set_pad_right(&txtareastyle_focused, LV_STATE_FOCUSED, 0);
	lv_style_set_pad_top(&txtareastyle_focused, LV_STATE_FOCUSED, 0);
	lv_obj_add_style(mpsEnterCalValTA, LV_TEXTAREA_PART_BG, &txtareastyle_focused);      /*Default button style*/
    lv_obj_set_event_cb(mpsEnterCalValTA, passwordcheck_event_handler);

    lv_obj_t * underlineLbl;
    underlineLbl = lv_label_create(mpsEnterCalValTA, NULL);
    lv_obj_set_size(underlineLbl, 80, 60);
    lv_obj_set_pos(underlineLbl, 0, 10);
	lv_label_set_align(underlineLbl, LV_LABEL_ALIGN_LEFT);
	static lv_style_t style_label_main;
	lv_style_reset(&style_label_main);
	//Write style state: LV_STATE_DEFAULT for style_bluetooth_labelinstruction_main
	lv_style_set_radius(&style_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_label_main, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_grad_color(&style_label_main, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
	lv_style_set_bg_grad_dir(&style_label_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_label_main, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_label_main, LV_STATE_DEFAULT, &lv_font_montserrat_32);
	lv_style_set_text_letter_space(&style_label_main, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_left(&style_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_label_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_label_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(underlineLbl, LV_LABEL_PART_MAIN, &style_label_main);
    lv_label_set_text(underlineLbl, "____");

    labelsymbol = lv_label_create(mpsMetroPswd, NULL);
    lv_obj_align(labelsymbol, mpsEnterCalValTA, LV_ALIGN_OUT_RIGHT_MID, 5, -5);
    lv_label_set_text(labelsymbol, LV_SYMBOL_EDIT);
    lv_obj_set_size(labelsymbol, 200, 200);

    static lv_style_t style_label3;
    lv_style_init(&style_label3);
    lv_style_set_text_color(&style_label3, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_label3, LV_STATE_DEFAULT  ,&lv_font_montserrat_28);
    lv_obj_add_style(labelsymbol, LV_LABEL_PART_MAIN, &style_label3);

    /*Create a keyboard*/
    lv_obj_t * mpsKeyBoard;
    mpsKeyBoard = lv_keyboard_create(mpsMetroPswd, NULL);

    lv_obj_set_pos(mpsKeyBoard, 10, 140);
    lv_obj_set_size(mpsKeyBoard,  300, 320); 
    lv_keyboard_set_mode(mpsKeyBoard, LV_KEYBOARD_MODE_NUM );
    lv_keyboard_set_map(mpsKeyBoard, LV_KEYBOARD_MODE_NUM, fcs_kb_map);
    lv_keyboard_set_ctrl_map(mpsKeyBoard, LV_KEYBOARD_MODE_NUM, fcs_kb_ctrl);
    lv_keyboard_set_ctrl_map(mpsKeyBoard, LV_KEYBOARD_MODE_NUM, fcs_tgl_kb_ctrl);
    lv_keyboard_set_textarea(mpsKeyBoard, mpsEnterCalValTA);
    lv_obj_set_style_local_radius(mpsKeyBoard, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 140); 
    lv_obj_set_style_local_bg_color(mpsKeyBoard, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_bg_color(mpsKeyBoard, LV_KEYBOARD_PART_BG , LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_text_font(mpsKeyBoard, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, &lv_font_montserrat_32);
    lv_obj_set_style_local_text_color(mpsKeyBoard, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_width(mpsKeyBoard, LV_KEYBOARD_PART_BG , LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_opa(mpsKeyBoard, LV_KEYBOARD_PART_BG , LV_STATE_DEFAULT, 200);
    lv_obj_set_style_local_pad_all(mpsKeyBoard, LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_pad_all(mpsKeyBoard, LV_KEYBOARD_PART_BTN, LV_STATE_FOCUSED, 10);

    crnt_screen = mpsMetroPswd;
    screenid = screen_id;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void task_cb_Code_Correct(lv_task_t *t)
{
    pxDashboardScreen();
}

static void task_cb_Code_Changed(lv_task_t *t)
{
    Screen_Password(SCR_METROLOGY_PASSWORD);
}

static void task_cb_Code_InCorrect(lv_task_t *t)
{
     if(screenid == SCR_CHANGE_PASSWORD){
        lv_label_set_text(pswdmsg, "Enter New Metrology Code");
    } else if(screenid == SCR_PASSWORD){
        lv_label_set_text(pswdmsg, "Enter Code ");
    } else if(screenid == SCR_METROLOGY_PASSWORD){
        lv_label_set_text(pswdmsg, "Enter Metrology Code");
    }
    lv_label_set_text(labelsymbol, LV_SYMBOL_EDIT );
}

static void  passwordcheck_event_handler(lv_obj_t * obj, lv_event_t event)
{
    static int temppasword = 0;
    int retemppasword = 0;

    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        if(!passNo){
            pass  = lv_textarea_get_text(mpsEnterCalValTA);
            int passLength = strlen(pass);
            if(passLength == 4){
                temppasword = 0;
                temppasword = atoi(pass);
                if(screenid == SCR_PASSWORD){
                    if(temppasword == screen_password){
                        global_DashbordBTNflag = 1;
                        lv_textarea_set_text(mpsEnterCalValTA, "");
                        lv_label_set_text(labelsymbol, LV_SYMBOL_OK);
                        lv_label_set_text(pswdmsg, "Correct Code");
                        timer_task = lv_task_create(task_cb_Code_Correct, 10000, LV_TASK_PRIO_MID, NULL);
                        lv_task_once(timer_task);
                    }else{
                        ESP_LOGI(TAG, "Password not matched: %d , %d", metrology_password, temppasword);
                        lv_textarea_set_text(mpsEnterCalValTA, "");
                        lv_label_set_text(pswdmsg, "Wrong Code");
                        lv_label_set_text(labelsymbol, LV_SYMBOL_WARNING);
                        timer_task = lv_task_create(task_cb_Code_InCorrect, 10000, LV_TASK_PRIO_MID, NULL);
                        lv_task_once(timer_task);
                    }
                }else if(screenid == SCR_METROLOGY_PASSWORD){
                    if(temppasword == metrology_password){
                        lv_textarea_set_text(mpsEnterCalValTA, "");
                        global_DashbordBTNflag = 1;
                        CallMetroMenuScreen();    
                    }else{
                        ESP_LOGI(TAG, "Password not matched metrology : %d , %d", metrology_password, temppasword);
                        lv_textarea_set_text(mpsEnterCalValTA, "");
                        lv_label_set_text(pswdmsg, "Wrong Code");
                        lv_label_set_text(labelsymbol, LV_SYMBOL_WARNING);
                        timer_task = lv_task_create(task_cb_Code_InCorrect, 10000, LV_TASK_PRIO_MID, NULL);
                        lv_task_once(timer_task);
                    }
                }else if(screenid == SCR_CHANGE_PASSWORD){
                    ESP_LOGI(TAG, "Change Password 1 : %s, %d", pass, temppasword);
                    lv_textarea_set_text(mpsEnterCalValTA, "");
                    lv_label_set_text(pswdmsg,"Re Enter Metrology Code");
                    passNo = true;
                }
                
            }
        }

        if( passNo){
            Re_pass = lv_textarea_get_text(mpsEnterCalValTA);
            int RepassLength = strlen(Re_pass);
            if(RepassLength == 4){
                retemppasword = atoi(Re_pass);
                ESP_LOGI(TAG, "Change Password 2 : %s, %d", Re_pass, retemppasword);
                passNo = false;
                if(retemppasword == temppasword){
                    metrology_password = retemppasword;
                    lv_textarea_set_text(mpsEnterCalValTA, "");
                    lv_label_set_text(labelsymbol, LV_SYMBOL_OK);
                    lv_label_set_text(pswdmsg, "Code Changed");
                    timer_task = lv_task_create(task_cb_Code_Changed, 10000, LV_TASK_PRIO_MID, NULL);
                    lv_task_once(timer_task);
                }else{
                    ESP_LOGI(TAG, "Password not matched for change : %s , %s", pass, Re_pass);
                    ESP_LOGI(TAG, "Password not matched for change : %d , %d", temppasword, retemppasword);
                    lv_textarea_set_text(mpsEnterCalValTA, "");
                    lv_label_set_text(labelsymbol, LV_SYMBOL_WARNING);
                    timer_task = lv_task_create(task_cb_Code_InCorrect, 10000, LV_TASK_PRIO_MID, NULL);
                    lv_task_once(timer_task);
                }
                
            }
        }
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
