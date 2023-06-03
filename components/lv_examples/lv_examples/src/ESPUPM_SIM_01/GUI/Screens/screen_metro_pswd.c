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

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

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

char * pass ;
char * Re_pass;

int iPass;
int iRe_pass;

bool passNo = false;
int iUserPass;


lv_obj_t * pswdmsg;
lv_obj_t *mpsEnterCalValTA; 

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void xMetroPswdScrn(void)
{
    lv_obj_t * mpsMetroPswd;
    mpsMetroPswd = lv_cont_create(NULL, NULL);
    lv_scr_load(mpsMetroPswd);
    //lv_obj_del(crnt_screen);


    lv_obj_t * mpsPatrentCont
    ;
    mpsPatrentCont = lv_cont_create(mpsMetroPswd, NULL);
    lv_obj_set_size(mpsPatrentCont, 320, 480);
    lv_obj_set_click(mpsPatrentCont, false);
    lv_obj_align(mpsPatrentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( mpsPatrentCont , LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(mpsPatrentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(mpsPatrentCont    , LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    
    pswdmsg = lv_label_create(mpsPatrentCont, NULL);
    lv_obj_align(pswdmsg, mpsPatrentCont, LV_ALIGN_IN_TOP_MID, -100,10);
    lv_label_set_text(pswdmsg,"           Enter-NEW        \nMetrology Password");
    lv_obj_set_style_local_text_font( pswdmsg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24 );
    lv_obj_set_style_local_text_color( pswdmsg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE );

    // Create Text Area
    
    mpsEnterCalValTA = lv_textarea_create(mpsPatrentCont, NULL); 
    lv_obj_set_size( mpsEnterCalValTA, 210, 45); 
    lv_obj_align(  mpsEnterCalValTA,  pswdmsg, LV_ALIGN_OUT_BOTTOM_LEFT, 18, 5); 
    lv_obj_set_click(mpsEnterCalValTA, false); 
    lv_textarea_set_pwd_mode(mpsEnterCalValTA, true); 
    lv_textarea_set_cursor_hidden(mpsEnterCalValTA, true); 
    lv_textarea_set_text(mpsEnterCalValTA, "");  
    //lv_obj_set_pos(mpsEnterCalValTA, 5, 20);
    lv_textarea_set_one_line(mpsEnterCalValTA, true);
    //lv_textarea_set_placeholder_text(mpsEnterCalValTA, "****");
    lv_textarea_set_text_align(mpsEnterCalValTA, LV_LABEL_ALIGN_CENTER);
    //lv_obj_set_event_cb(_fcsEnterCalValTA, _mtaEnterRefValTA_event_cb);
    lv_obj_set_style_local_text_color(mpsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(mpsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, &lv_font_montserrat_48);
    lv_obj_set_style_local_bg_color(mpsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));//LV_COLOR_MAKE(0x39, 0x89, 0xBD)
    lv_obj_set_style_local_border_width(mpsEnterCalValTA, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(mpsEnterCalValTA, passwordcheck_event_handler);

    lv_obj_t * underlineLbl;
    underlineLbl = lv_label_create(mpsEnterCalValTA, NULL);
    lv_obj_align(underlineLbl, mpsEnterCalValTA, LV_ALIGN_IN_TOP_MID,10,12);
    lv_label_set_text(underlineLbl, "____");
    lv_obj_set_style_local_text_color(underlineLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(underlineLbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_38);

    /*Create a keyboard*/
    lv_obj_t * mpsKeyBoard;
    mpsKeyBoard = lv_keyboard_create(mpsPatrentCont, NULL);
    lv_obj_align(mpsKeyBoard,mpsEnterCalValTA, LV_ALIGN_OUT_BOTTOM_LEFT, -20, 5 );
    lv_obj_set_size(mpsKeyBoard,  250, 320); 
    lv_keyboard_set_mode(mpsKeyBoard,                LV_KEYBOARD_MODE_NUM                );
    lv_keyboard_set_map(mpsKeyBoard,                 LV_KEYBOARD_MODE_NUM,     fcs_kb_map);
    lv_keyboard_set_ctrl_map(mpsKeyBoard,            LV_KEYBOARD_MODE_NUM,    fcs_kb_ctrl);
    lv_keyboard_set_ctrl_map(mpsKeyBoard,            LV_KEYBOARD_MODE_NUM,fcs_tgl_kb_ctrl);
    lv_keyboard_set_textarea(mpsKeyBoard,            mpsEnterCalValTA                    );
    lv_obj_set_style_local_radius(mpsKeyBoard,       LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 120                            ); 
    lv_obj_set_style_local_bg_color(mpsKeyBoard,     LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_bg_color(mpsKeyBoard,     LV_KEYBOARD_PART_BG , LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD));
    lv_obj_set_style_local_text_font(mpsKeyBoard,    LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, &lv_font_montserrat_32         );
    lv_obj_set_style_local_text_color(mpsKeyBoard,   LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_WHITE                 );
    lv_obj_set_style_local_border_width(mpsKeyBoard, LV_KEYBOARD_PART_BG , LV_STATE_DEFAULT, 0                              );
    lv_obj_set_style_local_border_opa(mpsKeyBoard,   LV_KEYBOARD_PART_BG , LV_STATE_DEFAULT, 200                            );
    lv_obj_set_style_local_pad_all(mpsKeyBoard,      LV_KEYBOARD_PART_BTN, LV_STATE_DEFAULT, 170                            );
    //lv_obj_set_event_cb(mpsKeyBoard, passwordcheck_event_handler);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void  passwordcheck_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        //printf("Key Board Clicked\n");

        //passNo

        if(!passNo)
        {
            pass    = lv_textarea_get_text(mpsEnterCalValTA);

            int passLength = strlen(pass);

            printf("%s \n" , pass);

            if(passLength == 4)
            {
                iPass = atoi(pass);
                lv_textarea_set_text(mpsEnterCalValTA, "");
                lv_label_set_text(pswdmsg,"         Re-Enter        \nMetrology Password");
                printf("Password entered \n");
                passNo = true;
            }

        }

        if( passNo)
        {
            Re_pass = lv_textarea_get_text(mpsEnterCalValTA);

            int RepassLength = strlen(Re_pass);

            printf("%s \n" , Re_pass);

            if(RepassLength == 4)
            {
                iRe_pass = atoi(Re_pass);
                lv_textarea_set_text(mpsEnterCalValTA, "");
                lv_label_set_text(pswdmsg,"       Enter-NEW        \nMetrology Password");
                printf("Password Re entered \n");
                passNo = false;
                //pass = "";

                if(iPass ==  iRe_pass)
                {
                    printf("Password matched \n");

                    iUserPass = iPass;

                    metroCodeScreen();

                }else{

                    printf("Password not matched \n");

                }
                
            }

        }
        
        

    }
}

/**********************
 *    ERROR ASSERT
 **********************/
