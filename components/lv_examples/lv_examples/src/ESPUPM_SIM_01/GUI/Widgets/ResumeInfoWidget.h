#ifndef RESUMEINFOWIDGET_H_
#define RESUMEINFOWIDGET_H_

/*********************
 *      INCLUDES
 *********************/
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
//#include "lvgl/lvgl.h"

#include "../../../../../../lvgl/lvgl.h"
#include "../Screens/screen_includes.h"

//C:/esp/esp-idf/myPrograms/lvgl_esp_codes/lv_port_esp32_old/components/lvgl/lvgl.h
/*********************
 *      DEFINES
 *********************/
#ifndef RESUMEINFO_CONTAINER_WIDTH
#define RESUMEINFO_CONTAINER_WIDTH (300)
#endif


#ifndef RESUMEINFO_CONTAINER_HEIGHT
#define RESUMEINFO_CONTAINER_HEIGHT (320)
#endif

#ifndef RESUMEINFO_ARC_HEIGHT
#define RESUMEINFO_ARC_HEIGHT (220)
#endif

#ifndef RESUMEINFO_ARC_WIDTH
#define RESUMEINFO_ARC_WIDTH (220)
#endif

#ifndef RESUMEINFO_CIRCLE_SIZE
#define RESUMEINFO_CIRCLE_SIZE (30)
#endif

#ifndef RESUMEINFO_ARC_WIDTH
#define RESUMEINFO_ARC_WIDTH (8)
#endif

#ifndef RESUMEINFO_PERCENT_OFFSET
#define RESUMEINFO_PERCENT_OFFSET (50)
#endif

#ifndef RESUMEINFO_SYMBOL_OFFSET_X
#define RESUMEINFO_SYMBOL_OFFSET_X (2)
#endif

#ifndef RESUMEINFO_SYMBOL_OFFSET_Y
#define RESUMEINFO_SYMBOL_OFFSET_Y (5)
#endif


//define all the font sizes

#define pxTextLabel_font  &lv_font_montserrat_22
#define _xPercentLabelStyle_font &lv_font_montserrat_40
#define _xSymbolStyle_font &lv_font_montserrat_20

/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
	RESUMEINFO_READY,
    RESUMEINFO_PROBLEM,
	RESUMEINFO_METROLOGY_IN_PROGRESS,
	RESUMEINFO_WORK_IN_PROGRESS,
	RESUMEINFO_WAIT,
	RESUMEINFO_JOB_FINISHED, 		//== Added
	RESUMEINFO_ALERT_SERVICE,		//== Added
	RESUMEINFO_METROLOGY_NEEDED,	//== Added
	RESUMEINFO_EXPORT_DATA,			//== Added
} ResumeInfoState_t;

/* Data of battery */
typedef struct
{
	lv_obj_t * pxContainer;
	lv_obj_t * pxPercentValue;
	lv_obj_t * pxPercentSymbol;
	lv_obj_t * pxTextLabel;
	lv_obj_t * pxProgressArc;
	lv_obj_t * pxWhiteCircle;
	lv_obj_t * pxVar_liter_value;
	lv_obj_t * pxVar_liter_valueFloat;
	lv_obj_t * pxIntHour_Value;
	lv_obj_t * pxFloatHour_Value;
	lv_obj_t * pxHourValueArc;
	lv_obj_t * pxMinutValueArc;	
	lv_obj_t * pxTotalTaskNumberValue;
	lv_obj_t * pxCurrentTaskNumberValue;
	lv_obj_t * pxRemainingHour;
	lv_obj_t * pxRemainingminut;

	ResumeInfoState_t xState;
} ResumeInfoExt_t;

/**********************
 *  GLOBAL MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

extern char * dashboardBTNTxt;
extern bool arcloded ;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**
 * Create a info widget
 * @param pxParent pointer to an object, it will be the parent of the new object
 * @return pointer to the created object
 */
lv_obj_t * pxCreateResumeInfo(lv_obj_t * pxParent);

/**
 * Set the state for the info widget object
 * @param pxObj pointer to a widget object
 * @param xState a new state
 */
void vSetResumeInfoState(lv_obj_t * pxObj, ResumeInfoState_t xState, const char * text);

/**
 * Set the percent value
 * @param pxObj pointer to a widget object
 * @param usPercent a new value
 */
void vSetResumeInfoPercent(lv_obj_t * pxObj, int usPercent);

/**
 * Set the Liters value
 * @param pxObj pointer to a widget object
 * @param usLiters a new value
 */


void vSetResumeInfoLitersInt(lv_obj_t * pxObj, uint32_t usLiters);

void vSetResumeInfoLitersFloat(lv_obj_t * pxObj, uint32_t usLitersF);


/**
 * Set the Int Hour value
 * @param pxObj pointer to a widget object
 * @param usHourInt a Float part of liter
 */
void vSetResumeInfoHourInt(lv_obj_t * pxObj, uint32_t usHourInt);

/**
 * Set the Float Hour value
 * @param pxObj pointer to a widget object
 * @param usHourInt a Float part of liter
 */
void vSetResumeInfoHourFloat(lv_obj_t * pxObj, uint32_t usHourInt);


/**
 * Set the HourInt & HourFloat value
 * @param pxObj pointer to a widget object
 * @param usHourInt a Int part of Hour
 * @param usHourFloat a float part of hour
 */
void vSetResumeInfoHour(lv_obj_t * pxObj, uint32_t usHourInt, uint32_t usHourFloat);

/**
 * Set the Total Hours value (Hour & Min Both)
 * @param pxObj pointer to a widget object
 * @param usHour a part of Hour
 * @param usMinutes a part of Min
 */
void vSetResumeInfoArcTime(lv_obj_t * pxObj, uint32_t usHour, uint32_t usMinute);

/**
 * Set the Total Task/ Current Task Inside Arc
 * @param pxObj pointer to a widget object
 * @param  usTotalTask number of total tasks
 * @param usCurrentTask Current task number
 */

void vSetResumeInfoArcTask(lv_obj_t * pxObj, uint32_t usTotalTask, uint32_t usCurrentTask);

/**
 * Set the Remaining Hopur Value
 * @param pxObj pointer to a widget object
 * @param usRemHour a Hour value
 */
void vSetResumeInfoRemainingHour(lv_obj_t * pxObj, uint32_t usRemHour);

/**
 * Set the Remaining Minute Value
 * @param pxObj pointer to a widget object
 * @param usRemHour a Minute value
 */
void vSetResumeInfoRemainingMinute(lv_obj_t * pxObj, uint32_t usRemMin);

/**
 * Set the Total sequence number
 * @param pxObj pointer to a widget object
 * @param usHourInt a total sequence number
 */
void vSetResumeInfoTotalSeq(lv_obj_t * pxObj, uint32_t usTotalSeqNum);

/**
 * Set the Current sequence number
 * @param pxObj pointer to a widget object
 * @param usHourInt a current sequence number
 */
void vSetResumeInfoCurrentSeq(lv_obj_t * pxObj, uint32_t usCurrentSeqNum);

#endif /* RESUMEINFOWIDGET_H_ */
