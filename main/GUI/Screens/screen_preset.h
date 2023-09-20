/**
 *  @file   : screen_preset.h
 *  @brief  : This screen is to show the Sequences screen
 *  @details: This file is to show Sequences screen, this screen comes when start job button is clicked fron dashbord screen
 *            Menulist of dashbord screen
 */

#ifndef SCREEN_PRESET_H
#define SCREEN_PRESET_H

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
    // void xsPresetScreen(void);
    void xsPresetScreenAdvance(void);
    void defaultParametrs(void);
    void setRollerParameterNull(void);
    void _xaTimeLabel_Adv_refr_func(lv_task_t *_xarefresherTask);

    /**********************
     *    ERROR ASSERT
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_PRESET_H*/