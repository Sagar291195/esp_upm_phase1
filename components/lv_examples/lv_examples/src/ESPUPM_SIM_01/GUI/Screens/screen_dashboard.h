/**
 * @file screen_dashboard.h
 *
 */

//   SCREEN_CODE_H

#ifndef SCREEN_DASHBOARD_H
#define SCREEN_DASHBOARD_H

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
    extern lv_obj_t *xStopButtonLabel;
    extern lv_obj_t *_xStopBtn;
    extern lv_task_t *ResInfoPerChange_task;
    extern lv_obj_t *_xStopBtn;

    extern bool PumpStopForcefully;
    extern int xBTN;
    /**********************
     *  GLOBAL PROTOTYPES
     **********************/

    void DashboardInfoWidget(void);
    void pxDashboardScreen(void);
    void BigButton(void);
    void startJobBTN(void);
    void stopBTN(void);

    void _xTimeLabel_refr_func(lv_task_t *refresherTask); /*This function wil be called periodically to refresh the guiTime label*/
    void vResInfoPerChangeTask(void);
    void ResInfoPerChange_task_cb(lv_task_t *ResInfoPerChange_task);

    void vStopUpdatingValuesToDashbordScreen();

    void vCleanupAllDashboardScreen();

    void vUpdateDashboardScreen();
    /**
     * @brief this fuction will display the job finished on the screen
     *
     */
    void vShowJobFinishedDashboardScreen();

    /**********************
     *    ERROR ASSERT
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_DASHBOARD_H*/