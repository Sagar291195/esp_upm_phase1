/**
 *  @file   : screen_summary_start.h
 *  @brief  : This screen is to show the summary of entered preset values
 *  @details: This file is to show summary screen, this screen comes when Valid button is clicked from Preset  screen
 *
 */

#ifndef SCREEN_SUMMARY_START_H
#define SCREEN_SUMMARY_START_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*********************
     *      INCLUDES
     *********************/

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *     TYPEDEFS
     **********************/

    /**********************
     *  GLOBAL MACROS
     **********************/

    /**********************
     *  GLOBAL VARIABLES
     **********************/

    /**********************
     *  GLOBAL PROTOTYPES
     **********************/
    void xssSummaryStartScreen(void);
    void __xssTimeLabel_ss_refr_func(lv_task_t *__xssTMrefresherTask);
    void __xssStartJobBTN_refr_func(lv_task_t *__xssStartBTNCountTask);
    void start_timer_callback(void *args);
    void startTimer(void);
    void vUpdateInfoWidgetTask(void);
    void infoWidgetUpdateTask_cb(lv_task_t *infoWidgetUpdateTask);

    /**********************
     *    ERROR ASSERT
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_SUMMARY_START_H*/