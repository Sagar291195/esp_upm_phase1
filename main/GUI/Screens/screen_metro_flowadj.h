

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
 *  @file        screen_flow_calibration.h
 *  @brief       Header file for screen_flow_calibration.c
 *  @details     Header file for screen_flow_calibration.c
 */

#ifndef SCREEN_METRO_FLOWADJ_H
#define SCREEN_METRO_FLOWADJ_H

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
    void callMetroFlowAdjustScreen(void);
    int get_flow_calibration_point_cout(void);
    void set_flow_calibration_point_cout(int value);
    /**********************
     *    ERROR ASSERT
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_METRO_FLOWADJ_H*/