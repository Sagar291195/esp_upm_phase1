/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
*  @file screen_flow_calibration.c 
*  @brief This Screen is for flow calibration
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
lv_task_t *task1; 


static void btn1_event_handler(lv_obj_t * obj, lv_event_t event);
static void btn2_event_handler(lv_obj_t * obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void cScrTransitCheck(void)
{
    vTestScreen1();
    vCallTask1();
    //usleep(5 * 1000);
    //vTestScreen2();
    //usleep(5 * 1000);

}


lv_obj_t * vTestScreen1(void)
{
    printf("Calling Screen 1\n");
    lv_obj_t * scr1;
    scr1 = lv_obj_create(lv_scr_act(), NULL);
    //lv_scr_load(scr1);
    lv_obj_t * parentCont_scr1;
    parentCont_scr1 = lv_cont_create(scr1, NULL);
    lv_obj_set_size(parentCont_scr1, 320, 480);
    lv_obj_align(parentCont_scr1, NULL, LV_ALIGN_CENTER, 0,0);

    static lv_style_t style_scr1_container;
    lv_style_init(&style_scr1_container);
    lv_style_set_bg_color(&style_scr1_container,LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_add_style(parentCont_scr1, LV_CONT_PART_MAIN, &style_scr1_container);

    lv_obj_t *btn_scr1;
    btn_scr1 = lv_btn_create(parentCont_scr1, NULL);
    lv_obj_set_size(btn_scr1, 200, 80);
    lv_obj_align(btn_scr1, parentCont_scr1, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_event_cb(btn_scr1, btn1_event_handler);

    lv_obj_t *btnlvl_scr1;
    btnlvl_scr1 = lv_label_create(btn_scr1, NULL);
    lv_obj_align(btnlvl_scr1,btn_scr1, LV_ALIGN_CENTER, 0,0 );
    lv_label_set_text(btnlvl_scr1, "Screen 1");

    return scr1;
   
}

lv_obj_t * vTestScreen2(void)
{
    printf("Calling Scree2\n");
    lv_obj_t * scr2;
    scr2 = lv_obj_create(lv_scr_act(), NULL);
    //lv_scr_load(scr2);
    lv_obj_t * parentCont_scr2;
    parentCont_scr2 = lv_cont_create(scr2, NULL);
    lv_obj_set_size(parentCont_scr2, 320, 480);
    lv_obj_align(parentCont_scr2, NULL, LV_ALIGN_CENTER, 0,0);

    static lv_style_t style_scr2_container;
    lv_style_init(&style_scr2_container);
    lv_style_set_bg_color(&style_scr2_container,LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_add_style(parentCont_scr2, LV_CONT_PART_MAIN, &style_scr2_container);

    lv_obj_t *btn_scr2;
    btn_scr2 = lv_btn_create(parentCont_scr2, NULL);
    lv_obj_set_size(btn_scr2, 200, 80);
    lv_obj_align(btn_scr2, parentCont_scr2, LV_ALIGN_CENTER, 0,100);
    lv_obj_set_event_cb(btn_scr2, btn2_event_handler);

    lv_obj_t *btnlvl_scr2;
    btnlvl_scr2 = lv_label_create(btn_scr2, NULL);
    lv_obj_align(btnlvl_scr2,btn_scr2, LV_ALIGN_CENTER, 0,0 );
    lv_label_set_text(btnlvl_scr2, "Screen 2");

    return scr2;

}


void task_cb1(lv_task_t *t)                                                         
{                                                                                       
	                                                              
    vTestScreen2();                                                                       
	                                                                                                                                                     
}                                                                                         
                                                                                          
void vCallTask1(void)                                                     
{                                                                                         
	//The code to display the startup information is written here                                                                                                                   
	task1 = lv_task_create(task_cb1, 2000, LV_TASK_PRIO_MID, NULL);
    lv_task_once(task1);                   
                                                                                                                                                         	                                                                                     
} 


static void btn1_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {      
        lv_obj_t *  scr2 = vTestScreen2();
        lv_scr_load(scr2);
    }
}

static void btn2_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        //vTestScreen1();
        //lv_task_once(task1); 
    }
}

        

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
