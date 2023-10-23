#ifndef SEQUENCE_WIDGET_H_
#define SEQUENCE_WIDGET_H_

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 #include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "../lvgl/lvgl.h"

 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#ifndef SEQUENCE_CONTAINER_WIDTH
#define SEQUENCE_CONTAINER_WIDTH (290)
#endif

#ifndef SEQUENCE_CONTAINER_HEIGHT
#define SEQUENCE_CONTAINER_HEIGHT (100)
#endif

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
typedef enum
{
    STATE_OK,
    STATE_NOTOK,
} SequenceState_t;

typedef struct
{
    lv_obj_t *sqContainer;
    lv_obj_t *_sqSequenceNumTxtVal;
    lv_obj_t *_sqSequenceNumValue;
    lv_obj_t *_sqFlowSetPtValue;
    lv_obj_t *_sqDurationValue;
    lv_obj_t *_sqProblemReasValue;

    SequenceState_t xBTNState;
} SequenceExt_t;

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/**
 * Create a info widget
 * @param sqParent pointer to an object, it will be the parent of the new object
 * @param seqNum sequence number
 * @return pointer to the created object
 */
lv_obj_t *sqCreateSequence(lv_obj_t *sqParent, uint8_t seqNum);

/**
 * Set the Flow Set Point Number
 * @param sqObj pointer to a widget object
 * @param sqSetpNumber a new value Set Point Number
 */
// void sqSetFlowSetPoint( lv_obj_t * sqObj, uint32_t sqSetpNumber);
void sqSetFlowSetPoint(lv_obj_t *sqObj, float sqSetpNumber);

/**
 * Set the Flow Set Point Number
 * @param sqObj pointer to a widget object
 * @param sqDration a new value to duration
 */
void sqSetDuration(lv_obj_t *sqObj, uint32_t sqDuration);

/**
 * Set the Flow Set Point Number
 * @param sqObj pointer to a widget object
 * @param sqProb a new Text to Problem
 */
void sqSetProblem(lv_obj_t *sqObj, const char *sqProb);

/**
 * Set the Flow Set Point Number
 * @param sqObj pointer to a widget object
 * @param sqProb a new Text to Sequence Number
 */
void sqSquenceNum(lv_obj_t *sqObj, uint32_t sqSeqNum);

/**
 * Set the Heading Text
 * @param sqObj pointer to a widget object
 * @param sqHeading a new Text to Problem
 */
void sqSetHeading(lv_obj_t *sqObj, const char *sqHeading);

void sqSetBtnColor(lv_obj_t *pxObj, SequenceState_t xBTNState);

void SeqWigetBTNEventCallback(lv_obj_t *obj1, lv_event_cb_t event_cb1);

#endif /* SEQUENCE_WIDGET_H_ */
