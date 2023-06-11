/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"

/*********************
 *      DEFINES
 *********************/

#define number1    10000
#define iUserPass  1234

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
// All prototypes in header file


/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *crnt_screen;
lv_obj_t * scr1=NULL;
lv_obj_t * testscr1=NULL;




static const char *btnm_map[] = { "1" , "2", "3", "\n",
                                 "4" , "5", "6", "\n",
                                 "7" , "8", "9", "\n",
                                 LV_SYMBOL_BACKSPACE , "0", LV_SYMBOL_OK , ""   //LV_SYMBOL_OK
                                 };


lv_task_t * task=NULL;

lv_obj_t *label=NULL;  // ENTER CODE Text
lv_obj_t *label1=NULL; // To print PASSWORD
lv_obj_t *label3=NULL; // check box password status
lv_obj_t *btnm=NULL;
lv_obj_t *container=NULL;
lv_obj_t *cstcontainer=NULL;
//lv_obj_t *xParentcontainer;

static lv_style_t style_btnm_bg;
static lv_style_t style_btnm;
static lv_style_t style_btnm_pressed;
static lv_style_t style_label;
static lv_style_t style_label1;
static lv_style_t style_label3;
static lv_style_t style_scr1_container;

bool passDetected = false;


/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

//variable
int global_DashbordBTNflag;


// Define All the Screens Here

// Other Variables

char * pTxt = "\0";
char * pVal;

unsigned int number;

char aPass[10]; //String to store Password
char str1[] = LV_SYMBOL_OK ;
char str2[] = LV_SYMBOL_BACKSPACE ;


static void event_handler(lv_obj_t * obj, lv_event_t event);
//static void DC_PIn_Check(void);





/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void vCallCodeScreen(void)
{
    vPassword_scr1();
}



void vPassword_scr1(void)
{
    scr1 =  lv_obj_create(NULL, NULL);
    lv_scr_load(scr1);
    if(crnt_screen != NULL){
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    container = lv_cont_create(scr1, NULL);
    lv_obj_set_size(container, 320, 480);
    lv_obj_align(container, NULL, LV_ALIGN_CENTER, 0,0);

    lv_style_init(&style_scr1_container);
    lv_style_set_bg_color(&style_scr1_container,LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_opa(&style_scr1_container, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&style_scr1_container, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(container, LV_CONT_PART_MAIN, &style_scr1_container);

    btnm = lv_btnmatrix_create(container, NULL);
    lv_obj_align(btnm, container, LV_ALIGN_IN_LEFT_MID , 32, -70);
    lv_btnmatrix_set_map(btnm, btnm_map);
    lv_obj_set_height(btnm, 340);
    lv_obj_set_width(btnm, 250);
    lv_obj_set_event_cb(btnm, event_handler);

    //This is style for BTNMATRIX
    lv_style_init(&style_btnm_bg);
    lv_style_set_bg_color(&style_btnm_bg, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_border_opa(&style_btnm_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_border_color(&style_btnm_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&style_btnm_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style_btnm_bg, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(btnm, LV_BTNMATRIX_PART_BG, &style_btnm_bg);
    
    lv_style_init(&style_btnm);
    lv_style_set_bg_color(&style_btnm, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_opa(&style_btnm, LV_STATE_DEFAULT, 0);
    lv_style_set_border_color(&style_btnm, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_opa(&style_btnm, LV_STATE_DEFAULT, 255);
    lv_style_set_radius(&style_btnm, LV_STATE_DEFAULT, 50);
    lv_style_set_border_width(&style_btnm, LV_STATE_DEFAULT, 2);
    lv_style_set_text_color(&style_btnm, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_btnm, LV_STATE_DEFAULT, &lv_font_montserrat_28);

    lv_style_init(&style_btnm_pressed);
    lv_style_set_bg_color(&style_btnm_pressed, LV_STATE_PRESSED, LV_COLOR_BLUE);
    lv_style_set_bg_opa(&style_btnm_pressed, LV_STATE_PRESSED, 255);
    lv_style_set_border_color(&style_btnm_pressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_border_opa(&style_btnm_pressed, LV_STATE_PRESSED, 255);
    lv_style_set_radius(&style_btnm_pressed, LV_STATE_PRESSED, 50);
    lv_style_set_border_width(&style_btnm_pressed, LV_STATE_PRESSED, 2);
    lv_style_set_text_color(&style_btnm_pressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_btnm_pressed, LV_STATE_PRESSED, &lv_font_montserrat_28);
    
    lv_obj_add_style(btnm, LV_BTNMATRIX_PART_BTN, &style_btnm);
    lv_obj_add_style(btnm, LV_BTNMATRIX_PART_BTN, &style_btnm_pressed);

    label = lv_label_create(container, NULL);
    lv_label_set_text(label, "Enter code");
    lv_obj_align(label, btnm, LV_ALIGN_OUT_TOP_MID, -25, -65);

    lv_style_init(&style_label);
    lv_style_set_text_color(&style_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_label, LV_STATE_DEFAULT  ,&lv_font_montserrat_28);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &style_label);

    label1 = lv_label_create(container, NULL);
    lv_obj_align(label1, btnm, LV_ALIGN_OUT_TOP_MID, -30, -30);
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
    //lv_obj_set_size(label1, 100, 120);
    lv_label_set_text(label1, "____" );

    lv_style_init(&style_label1);
    lv_style_set_text_font(&style_label1, LV_STATE_DEFAULT  ,&lv_font_montserrat_40);
    lv_style_set_text_letter_space(&style_label1, LV_LABEL_PART_MAIN, 8);
    lv_style_set_text_color(&style_label1, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &style_label1);

    label3 = lv_label_create(container, NULL);
    lv_obj_align(label3, label1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_label_set_text(label3, LV_SYMBOL_EDIT);
    lv_obj_set_size(label3, 200, 200);

    lv_style_init(&style_label3);
    lv_style_set_text_color(&style_label3, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_label3, LV_STATE_DEFAULT  ,&lv_font_montserrat_28);
    lv_obj_add_style(label3, LV_LABEL_PART_MAIN, &style_label3);
    crnt_screen = scr1;
}

// void vChangeScrTest(void)
// {

//     //lv_obj_invalidate(scr1);
//     //lv_obj_set_hidden(scr1, true);
//     //lv_obj_del(scr1);
//     testscr1 =  lv_obj_create(NULL, NULL);
//     //lv_scr_load(scr1);
//     //lv_obj_del(container);
//     lv_scr_load(testscr1);
//     cstcontainer = lv_cont_create(testscr1, NULL);
//     lv_obj_set_size(cstcontainer, 320, 480);
//     lv_obj_align(cstcontainer, NULL, LV_ALIGN_CENTER, 0,0);
//     //lv_obj_invalidate(scr1);
//     //lv_obj_del(scr1);
// }

void vClearBTN(void)
{
    //strcpy(pVal, "");
    strcpy(pVal, "");
    lv_label_set_text(label1, "____" );
    lv_label_set_text(label3, LV_SYMBOL_EDIT );
    //lv_label_set_text(label, "Wrong code");
    lv_label_set_text(label, "Enter code");
    //lv_obj_align(label, btnm, LV_ALIGN_OUT_TOP_MID, 0, -40);
    //printf("pressed : %p", pVal);
}


void vPasswordLine(void)
{
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_line_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_width(&style, LV_STATE_DEFAULT, 2);
    lv_style_set_line_rounded(&style, LV_STATE_DEFAULT, true);
#if LV_USE_LINE
    /*Create an object with the new style*/
    lv_obj_t * obj = lv_line_create(container, NULL);
    lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style);

    static lv_point_t p[] = {{15, 65}, {110, 65}};
    lv_line_set_points(obj, p, 2);

    lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
#endif
}


 //---------------------------------------------------------------------------------

void task_cb(lv_task_t *t)
{

    vClearBTN();

}

void vCallTask(void)
{
	//The code to display the startup information is written here
	task = lv_task_create(task_cb, 10000, LV_TASK_PRIO_MID, NULL);
    lv_task_once(task);

}

void task_cb_Code_Correct(lv_task_t *t)
{

    pxDashboardScreen();
    //lv_scr_load(xParentcontainer);

}

void vCallTask_Code_Correct(void)
{
	//The code to display the startup information is written here
	task = lv_task_create(task_cb_Code_Correct, 10000, LV_TASK_PRIO_MID, NULL);
    lv_task_once(task);

}



/**********************
 *   STATIC FUNCTIONS
 **********************/

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) {
        pTxt = (char *)lv_btnmatrix_get_active_btn_text(obj);
        pVal = strcat(aPass, pTxt);
        number = atoi(pVal); // convert char pointer to number
        lv_label_set_text(label1, pVal );
        if( number < 10 ){
            lv_label_set_text(label1, "*___" );
        }else if (number > 10 && number< 100){
            lv_label_set_text(label1, "**__" );
        }else if (number > 100 && number< 1000){
            lv_label_set_text(label1, "***_" );
        }else if (number > 1000 && number< 10000){
            lv_label_set_text(label1, "****" );
            vTaskDelay(5000/portMAX_DELAY);
            passDetected = true;
        }else if (number == 0){
            lv_label_set_text(label1, "" );
        }else{
            lv_label_set_text(label1, "" );
        }

        if(number == iUserPass && passDetected == true )
            {
                lv_label_set_text(label3, LV_SYMBOL_OK);
                lv_label_set_text(label1, "****" );
                lv_label_set_text(label, "Right Code");
                //printf("Password Detected\n");

                global_DashbordBTNflag = 1;
                //pxDashboardScreen();
                vCallTask_Code_Correct();
            }
            else
            {
                if(number > 1000 && number< 10000 )
                {
                    //printf("wrong Password\n");
                    //lv_label_set_recolor(label, true);
                    lv_label_set_text(label, "Wrong code");
                    lv_obj_align(label, btnm, LV_ALIGN_OUT_TOP_MID, 0, -50);
                    lv_label_set_text(label3, LV_SYMBOL_WARNING );
                    //delay(1);
                    //vClearBTN();
                    strcpy(pVal, "");
                    vCallTask();
                }
            }
//#if 0

        bool i = strcmp(pTxt, str1); //OK Button Flag check

        if(  i == 0)   //Check if Ok button is pressed
        {
            //printf( "OK Button Pressed\n" );

            if(number == iUserPass )
            {
                //printf("Password Correct");
                lv_label_set_text(label3, LV_SYMBOL_OK);
                strcpy(pVal, "");
                //vTickMark();
                // delay(1);
                //vTestScreen();
            }
            else
            {

                //printf("Password incorrect");
                //lv_label_set_recolor(label, true);
                lv_label_set_text(label, "Wrong code");
                //lv_obj_align(label, btnm, LV_ALIGN_OUT_TOP_MID, 0, -50);
                lv_label_set_text(label3, LV_SYMBOL_WARNING );
                strcpy(pVal, "");
                //delay(1);
                //vClearBTN();
            }
        }

        bool j = strcmp(pTxt, str2); //Clear Button Flag check
        if (j == 0)
        {
            vClearBTN();
        }

        //printf("btn_matrix_cb _complete\n");

//#endif
     }
}




// static void DC_PIn_Check(void)
// {
//     // Set GPIO as OUTPUT
//     gpio_pad_select_gpio(14);
//     gpio_set_direction(14, GPIO_MODE_OUTPUT);     // WakeMode
//     gpio_set_level(14, 0);
//     printf("Hi DC Pin Check LOW \n");

//     vTaskDelay(1/10);

//     gpio_set_level(14, 1);
//     printf("Hi DC Pin Check HIGH \n");

// }

/**********************
 *    ERROR ASSERT
 **********************/


