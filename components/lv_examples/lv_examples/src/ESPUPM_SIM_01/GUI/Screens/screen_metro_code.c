/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-05
 */
/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"

/*********************
 *      DEFINES
 *********************/
#define MetroScreenBGColor LV_COLOR_MAKE(0x39, 0x89, 0xBD)
//#define oksymb LV_IMG_DECLARE(ok_icon)

#define number1    10000


int iUserPass = 1234;

#define statussymbol &ok_icon

//Declare Images Here

LV_IMG_DECLARE(ok_icon)
LV_IMG_DECLARE(cross_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void event_handler(lv_obj_t * obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t * scrMetroCode;
lv_task_t *pDelaytask; 

lv_obj_t * metroBaseCont;
lv_obj_t * _metroCodeMsgLbl;
lv_obj_t * _metroPassSpaceLbl;
lv_obj_t * _metroBtnm;

lv_obj_t * _metroOKMark;
lv_obj_t * _metroCrossMark;
lv_obj_t * _metroStatusSymbol;


static const char *btnm_map[] = { "1" , "2", "3", "\n",
                                 "4" , "5", "6", "\n",
                                 "7" , "8", "9", "\n",
                                 LV_SYMBOL_BACKSPACE , "0", LV_SYMBOL_OK , ""   //LV_SYMBOL_OK
                                 };


// Other Variables

char * pTxt_metro = "\0"; 
int xPasswordStatus;

unsigned int number;

char aPass_metro[10]; //String to store Password
char str1_metro[] = LV_SYMBOL_OK ;
char str2_metro[] = LV_SYMBOL_BACKSPACE ;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

char *pVal_metro = "";

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void metroCodeScreen(void)
{
    scrMetroCode = lv_obj_create(NULL, NULL);
    lv_scr_load(scrMetroCode);
    if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }
    
    metroBaseCont = lv_cont_create(scrMetroCode, NULL);
    lv_obj_set_size(metroBaseCont, 320, 480);
    lv_obj_align(metroBaseCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( metroBaseCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, MetroScreenBGColor ); //3989BD
    lv_obj_set_style_local_border_opa(metroBaseCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(metroBaseCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    
    _metroBtnm = lv_btnmatrix_create(metroBaseCont, NULL);
    lv_obj_align(_metroBtnm, metroBaseCont, LV_ALIGN_CENTER , 0, -75);
    lv_btnmatrix_set_map(_metroBtnm, btnm_map);
    lv_obj_set_height(_metroBtnm, 340);
    lv_obj_set_width(_metroBtnm, 250);
    lv_obj_set_event_cb(_metroBtnm, event_handler);

    //This is style for BTNMATRIX
    static lv_style_t style_btnm_bg;
    lv_style_init(&style_btnm_bg);
    lv_style_set_bg_color(&style_btnm_bg, LV_STATE_DEFAULT, MetroScreenBGColor);
    lv_style_set_border_opa(&style_btnm_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_border_color(&style_btnm_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&style_btnm_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_btnm_bg, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(_metroBtnm, LV_BTNMATRIX_PART_BG, &style_btnm_bg);
    
    static lv_style_t style_btnm;
    lv_style_init(&style_btnm);
    lv_style_set_bg_color(&style_btnm, LV_STATE_DEFAULT, MetroScreenBGColor);
    lv_style_set_bg_opa(&style_btnm, LV_STATE_DEFAULT, 0);
    lv_style_set_border_color(&style_btnm, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_opa(&style_btnm, LV_STATE_DEFAULT, 255);
    lv_style_set_radius(&style_btnm, LV_STATE_DEFAULT, 50);
    lv_style_set_border_width(&style_btnm, LV_STATE_DEFAULT, 2);
    lv_style_set_text_color(&style_btnm, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_btnm, LV_STATE_DEFAULT, &lv_font_montserrat_28);

    static lv_style_t style_btnm_pressed;
    lv_style_init(&style_btnm_pressed);
    lv_style_set_bg_color(&style_btnm_pressed, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_opa(&style_btnm_pressed, LV_STATE_PRESSED, 255);
    lv_style_set_border_color(&style_btnm_pressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_border_opa(&style_btnm_pressed, LV_STATE_PRESSED, 255);
    lv_style_set_radius(&style_btnm_pressed, LV_STATE_PRESSED, 50);
    lv_style_set_border_width(&style_btnm_pressed, LV_STATE_PRESSED, 2);
    lv_style_set_text_color(&style_btnm_pressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_btnm_pressed, LV_STATE_PRESSED, &lv_font_montserrat_28);

    lv_obj_add_style(_metroBtnm, LV_BTNMATRIX_PART_BTN, &style_btnm);
    lv_obj_add_style(_metroBtnm, LV_BTNMATRIX_PART_BTN, &style_btnm_pressed);


    _metroCodeMsgLbl = lv_label_create(metroBaseCont, NULL);
    lv_label_set_text(_metroCodeMsgLbl, "Enter code");
    lv_obj_align(_metroCodeMsgLbl,_metroBtnm, LV_ALIGN_OUT_TOP_MID, -25, -65);

    static lv_style_t _metroCodeMsgLblStyle;
    lv_style_init(&_metroCodeMsgLblStyle);
    lv_style_set_text_color(&_metroCodeMsgLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(&_metroCodeMsgLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_28);
    lv_obj_add_style(_metroCodeMsgLbl, LV_LABEL_PART_MAIN, &_metroCodeMsgLblStyle);

    _metroPassSpaceLbl = lv_label_create(metroBaseCont, NULL);
    lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
    lv_label_set_align(_metroPassSpaceLbl, LV_LABEL_ALIGN_CENTER);
    //lv_obj_set_size(label1, 100, 120);
    lv_label_set_text(_metroPassSpaceLbl, "____" );

    static lv_style_t _metroPassSpaceStyle;
    lv_style_init(&_metroPassSpaceStyle);
    lv_style_set_text_font(&_metroPassSpaceStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_40); 
    lv_style_set_text_letter_space(&_metroPassSpaceStyle, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&_metroPassSpaceStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(_metroPassSpaceLbl, LV_LABEL_PART_MAIN, &_metroPassSpaceStyle);

    //==============================
    
    _metroStatusSymbol = lv_label_create(metroBaseCont, NULL);
    lv_obj_align(_metroStatusSymbol, _metroPassSpaceLbl, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_label_set_text(_metroStatusSymbol, LV_SYMBOL_EDIT);
    lv_obj_set_size(_metroStatusSymbol, 200, 200);

    static lv_style_t _metroStatusSymbolStyle;
    lv_style_init(&_metroStatusSymbolStyle);
    lv_style_set_text_color(&_metroStatusSymbolStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(&_metroStatusSymbolStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_28);
    lv_obj_add_style(_metroStatusSymbol, LV_LABEL_PART_MAIN, &_metroStatusSymbolStyle);

    //==============================

    //_metroOKSymbol();

    crnt_screen = scrMetroCode;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        
        pTxt_metro = (char *)lv_btnmatrix_get_active_btn_text(obj);
        //printf("%s was pressed\n", txt );
        pVal_metro = strcat(aPass_metro, pTxt_metro);   
        //lv_label_set_text(label1, pVal );
        number = atoi(pVal_metro); // convert char pointer to number
        //printf("%d was pressed\n", number );

        if( number < 10 )
        {
            //printf( "size is: %d \n", sizeof(pVal_metro) );
            //lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
            lv_label_set_text(_metroPassSpaceLbl, "*___" );  //label1 
        } 
        else if (number > 10 && number< 100)
        {
            //lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
            lv_label_set_text(_metroPassSpaceLbl, "**__" );
        }
        else if (number > 100 && number< 1000)
        {
            //lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
            lv_label_set_text(_metroPassSpaceLbl, "***_" );
        }
        else if (number > 1000 && number< 10000)
        {
            //lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
            lv_label_set_text(_metroPassSpaceLbl, "****" );
            vTaskDelay(1);
        }
        else if (number == 0)
        {
            //lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
            lv_label_set_text(_metroPassSpaceLbl, "" );
        }
        else
        {
            //lv_obj_align(_metroPassSpaceLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
            lv_label_set_text(_metroPassSpaceLbl, "" );
        }
        if(number == iUserPass )
            {    
                global_DashbordBTNflag = 1;
                xPasswordStatus = 1;
                strcpy(pVal_metro, "");
                CallMetroMenuScreen();       
            }
            else
            {
                if(number > 1000 && number< 10000 )
                {
                    lv_label_set_recolor(_metroCodeMsgLbl, true);
                    xPasswordStatus = 0;
                    lv_label_set_text(_metroCodeMsgLbl, "Wrong code");
                    lv_obj_align(_metroCodeMsgLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, 0, -50);
                    lv_label_set_text(_metroStatusSymbol, LV_SYMBOL_WARNING );
                    //delay(1);
                    //vClearBTN_metro();
                    vCallTask_metro();
                }
            }

        bool i = strcmp(pTxt_metro, str1_metro); //OK Button Flag check

        if(  i == 0)   //Check if Ok button is pressed
        {
            printf( "GotIt\n" );
            
            if(number == iUserPass )
            {    
                //lv_label_set_text(label3, LV_SYMBOL_OK);
                //vTickMark();
                //delay(1);
                //vTestScreen();    
            }
            else
            {
                lv_label_set_recolor(_metroCodeMsgLbl, true);
                xPasswordStatus = 0;
                lv_label_set_text(_metroCodeMsgLbl, "Wrong code");
                lv_obj_align(_metroCodeMsgLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, 0, -50);
                lv_label_set_text(_metroStatusSymbol, LV_SYMBOL_WARNING );
                //delay(1);
                //vClearBTN_metro();    
            }
        }

        bool j = strcmp(pTxt_metro, str2_metro); //Clear Button Flag check
        if (j == 0)
        {
            vClearBTN_metro();
        }
    }
}

void vClearBTN_metro(void)
{
    //strcpy(pVal, "");
    strcpy(pVal_metro, "");
    xPasswordStatus =  2;
    lv_label_set_text(_metroPassSpaceLbl, "____" );
    lv_label_set_text(_metroStatusSymbol, LV_SYMBOL_EDIT );
    lv_obj_align(_metroCodeMsgLbl,_metroBtnm, LV_ALIGN_OUT_TOP_MID, -25, -65);
    lv_obj_align(_metroCodeMsgLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, 0, -45);
    lv_label_set_text(_metroCodeMsgLbl, "Enter code");
    //lv_obj_align(_metroCodeMsgLbl, _metroBtnm, LV_ALIGN_OUT_TOP_MID, 0, -40);
    printf("pressed : %p", pVal_metro);

}

 //---------------------------------------------------------------------------------

void task_cb_metro(lv_task_t *t)                                                         
{                                                                                                                                            
    vClearBTN_metro();                                                                	                                                                                                                                                     
}                                                                                         
                                                                                          
void vCallTask_metro(void)                                                     
{                                                                                         
	//The code to display the startup information is written here                                                                                                                   
	pDelaytask = lv_task_create(task_cb_metro, 2000, LV_TASK_PRIO_MID, NULL);
    lv_task_once(pDelaytask);                                                                                                                                                                        	                                                                                     
} 

/**********************
 *    ERROR ASSERT
 **********************/
