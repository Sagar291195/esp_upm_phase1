/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-09-2021
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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void WTPScreen(void)
{
    
    lv_obj_t *wtpmainParentCont;
    wtpmainParentCont = lv_cont_create(NULL,NULL);
    lv_scr_load(wtpmainParentCont);
    lv_obj_del(crnt_screen);

    lv_obj_t *wtpParentCont;
    wtpParentCont = lv_cont_create(wtpmainParentCont, NULL);
    //lv_scr_load(xssParentContainer_ss);
    lv_obj_set_size(wtpParentCont, 320, 480);
    lv_obj_set_click(wtpParentCont, false);
    lv_obj_align(wtpParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color(wtpParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_MAKE(0x5D, 0x5D, 0x5D) );
    lv_obj_set_style_local_border_opa(wtpParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(wtpParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //lv_obj_t *wtpMsgLabel;
    // crnt_screen = wtpmainParentCont; //scrSummaryStart
    // dashboardflg = 1;
    // global_DashbordBTNflag = 2;
    // isMotor =true;
    // sprintf(startDateEnd, "%s", guiDate );
    // sprintf(startTimeEnd, "%sH%sM", guiHrDef, guiMinDef);
    // StartLTRCountVal = total_liters1;
    // startTimer(); // Start The Job Timer
    // pxDashboardScreen(); // Open Dashboard Screen


}



/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/