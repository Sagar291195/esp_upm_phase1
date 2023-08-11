/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-10
 */

/*********************
 *      INCLUDES
 *********************/

#include "sequence_widget.h"

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

lv_obj_t *_sqSequenceNumTxtLabel;
lv_obj_t *_sqSequenceNumLabel;
lv_obj_t *_sqFlowSetPtTxt;
lv_obj_t *_sqFlowSetPtVar;
lv_obj_t *_sqFlowSetPtUnit;
lv_obj_t *_sqDurationTxt;
lv_obj_t *_sqDurationVar;
lv_obj_t *_sqDurationUnit;
lv_obj_t *_sqProblemTxt;
lv_obj_t *_sqProblemReas;
lv_obj_t *_sqBTNTria;
lv_obj_t *_sqBTNTriaSignLbl;

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a info widget
 * @param sqParent pointer to an object, it will be the parent of the new object
 * @param sequence_number add the sequnece number to the button so that we can identify the sequence
 * @return pointer to the created object
 */

lv_obj_t *sqCreateSequence(lv_obj_t *sqParent,uint8_t sequence_number)
{
    lv_obj_t *sqContainer = lv_obj_create(sqParent, NULL);
    lv_obj_set_size(sqContainer, SEQUENCE_CONTAINER_WIDTH, SEQUENCE_CONTAINER_HEIGHT);

    // Allocate the object type specific extended data
    SequenceExt_t *sqExt = lv_obj_allocate_ext_attr(sqContainer, sizeof(SequenceExt_t));
    LV_ASSERT_MEM(sqExt);
    if (sqExt == NULL)
    {
        return NULL;
    }

    // Create label for SEQUENCE Heading Label

    _sqSequenceNumTxtLabel = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqSequenceNumTxtLabel, sqParent, LV_ALIGN_IN_TOP_LEFT, 5, 5);
    lv_label_set_text(_sqSequenceNumTxtLabel, "SEQUENCE");
    lv_obj_set_style_local_text_font(_sqSequenceNumTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqSequenceNumTxtLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    _sqSequenceNumLabel = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqSequenceNumLabel, _sqSequenceNumTxtLabel, LV_ALIGN_OUT_RIGHT_MID, 4, 0);
    lv_label_set_text(_sqSequenceNumLabel, "");
    lv_obj_set_style_local_text_font(_sqSequenceNumLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqSequenceNumLabel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    // Create label for Flow Set Point Text
    _sqFlowSetPtTxt = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqFlowSetPtTxt, _sqSequenceNumTxtLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_label_set_text(_sqFlowSetPtTxt, "Flow Set Point:");
    lv_obj_set_style_local_text_font(_sqFlowSetPtTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqFlowSetPtTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Flow Set Point Value
    _sqFlowSetPtVar = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqFlowSetPtVar, _sqFlowSetPtTxt, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
    lv_label_set_text(_sqFlowSetPtVar, "");
    lv_obj_set_style_local_text_font(_sqFlowSetPtVar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqFlowSetPtVar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for L/Min Text
    _sqFlowSetPtUnit = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqFlowSetPtUnit, _sqFlowSetPtVar, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text(_sqFlowSetPtUnit, "L/min");
    lv_obj_set_style_local_text_font(_sqFlowSetPtUnit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqFlowSetPtUnit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Duration Text
    _sqDurationTxt = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqDurationTxt, _sqFlowSetPtTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_label_set_text(_sqDurationTxt, "Duration:");
    lv_obj_set_style_local_text_font(_sqDurationTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqDurationTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Duration Value
    _sqDurationVar = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqDurationVar, _sqDurationTxt, LV_ALIGN_OUT_RIGHT_MID, 67, 0);
    lv_label_set_text(_sqDurationVar, "2");
    lv_obj_set_style_local_text_font(_sqDurationVar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqDurationVar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Duration Unit
    _sqDurationUnit = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqDurationUnit, _sqDurationVar, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_label_set_text(_sqDurationUnit, "h");
    lv_obj_set_style_local_text_font(_sqDurationUnit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqDurationUnit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Problem Txt
    _sqProblemTxt = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqProblemTxt, _sqDurationTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_label_set_text(_sqProblemTxt, "Problem:");
    lv_obj_set_style_local_text_font(_sqProblemTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqProblemTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create label for Problem Reason
    _sqProblemReas = lv_label_create(sqParent, NULL);
    lv_obj_align(_sqProblemReas, _sqProblemTxt, LV_ALIGN_OUT_RIGHT_MID, 68, 0);
    lv_label_set_text(_sqProblemReas, "None");
    lv_obj_set_style_local_text_font(_sqProblemReas, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_obj_set_style_local_text_color(_sqProblemReas, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create Triangle Sign Button
    _sqBTNTria = lv_btn_create(sqParent, NULL);
    lv_obj_align(_sqBTNTria, sqParent, LV_ALIGN_IN_RIGHT_MID, 80, -30);
    lv_obj_set_size(_sqBTNTria, 50, SEQUENCE_CONTAINER_HEIGHT);
    // lv_obj_set_event_cb(_xseBTNUSB_se, BTN_event_handler);
    lv_obj_set_style_local_radius(_sqBTNTria, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 7);
    lv_obj_set_style_local_bg_color(_sqBTNTria, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEB, 0x3B, 0x5A)); //#Eb3B5A
    lv_obj_set_style_local_border_width(_sqBTNTria, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_opa(_sqBTNTria, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_user_data(_sqBTNTria, &sequence_number);

    // Create a Triangle Sign Label
    _sqBTNTriaSignLbl = lv_label_create(_sqBTNTria, NULL);
    lv_obj_align(_sqBTNTriaSignLbl, _sqBTNTria, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(_sqBTNTriaSignLbl, LV_SYMBOL_NEXT);
    lv_obj_set_style_local_text_font(_sqBTNTriaSignLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(_sqBTNTriaSignLbl, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    sqExt->sqContainer = sqContainer;
    sqExt->_sqSequenceNumTxtVal = _sqSequenceNumTxtLabel;
    sqExt->_sqFlowSetPtValue = _sqFlowSetPtVar;
    sqExt->_sqDurationValue = _sqDurationVar;
    sqExt->_sqProblemReasValue = _sqProblemReas;
    sqExt->_sqSequenceNumValue = _sqSequenceNumLabel;

    sqExt->xBTNState = STATE_OK;

    return sqContainer;
}

/**
 * Set the Flow Set Point Number
 * @param sqObj pointer to a widget object
 * @param sqSetpNumber a new value Set Point Number
 */

// void sqSetFlowSetPoint( lv_obj_t * sqObj, uint32_t sqSetpNumber)
void sqSetFlowSetPoint(lv_obj_t *sqObj, float sqSetpNumber)
{
    SequenceExt_t *sqExt = lv_obj_get_ext_attr(sqObj);
    char chsqNumber[5];
    sprintf(chsqNumber, "%0.2f", sqSetpNumber);
    lv_label_set_text(sqExt->_sqFlowSetPtValue, chsqNumber);
    lv_obj_align(_sqFlowSetPtVar, _sqFlowSetPtTxt, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
    lv_obj_align(_sqFlowSetPtUnit, _sqFlowSetPtVar, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
}

/**
 * Set the Set point Duration
 * @param sqObj pointer to a widget object
 * @param sqDration a new value to duration
 */
void sqSetDuration(lv_obj_t *sqObj, uint32_t sqDuration)
{
    SequenceExt_t *sqExt = lv_obj_get_ext_attr(sqObj);
    char chsqDuration[5];
    sprintf(chsqDuration, "%d", sqDuration);
    lv_label_set_text(sqExt->_sqDurationValue, chsqDuration);
    lv_obj_align(_sqDurationVar, _sqDurationTxt, LV_ALIGN_OUT_RIGHT_MID, 67, 0);
    lv_obj_align(_sqDurationUnit, _sqDurationVar, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
}

/**
 * Set the Problem Type Text
 * @param sqObj pointer to a widget object
 * @param sqProb a new Text to Problem
 */
void sqSetProblem(lv_obj_t *sqObj, const char *sqProb)
{
    SequenceExt_t *sqExt = lv_obj_get_ext_attr(sqObj);
    lv_label_set_text(sqExt->_sqProblemReasValue, sqProb);
}

/**
 * Set the Heading Text
 * @param sqObj pointer to a widget object
 * @param sqHeading a new Text to Problem
 */
void sqSetHeading(lv_obj_t *sqObj, const char *sqHeading)
{
    SequenceExt_t *sqExt = lv_obj_get_ext_attr(sqObj);
    lv_label_set_text(sqExt->_sqSequenceNumTxtVal, sqHeading);
}

/**
 * Set the Sequence Number
 * @param sqObj pointer to a widget object
 * @param sqProb a new Text to Sequence Number
 */
void sqSquenceNum(lv_obj_t *sqObj, uint32_t sqSeqNum)
{
    SequenceExt_t *sqExt = lv_obj_get_ext_attr(sqObj);
    char chsqSeqNum[5];
    sprintf(chsqSeqNum, "%d", sqSeqNum);
    lv_label_set_text(sqExt->_sqSequenceNumValue, chsqSeqNum);
}

/**
 * Set the Big Button Color
 * @param sqObj pointer to a widget object
 * @param
 */

void sqSetBtnColor(lv_obj_t *sqObj, SequenceState_t xBTNState)
{
    SequenceExt_t *sqExt = lv_obj_get_ext_attr(sqObj);

    sqExt->xBTNState = xBTNState;

    switch (sqExt->xBTNState)
    {
    case STATE_OK:
        lv_obj_set_style_local_bg_color(_sqBTNTria, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));

        break;
    case STATE_NOTOK:
        lv_obj_set_style_local_bg_color(_sqBTNTria, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEB, 0x3B, 0x5A));
        break;
    }
}

/**
 * Set the Big Button Callback
 * @param sqObj pointer to a widget object
 * @param
 */

void SeqWigetBTNEventCallback(lv_obj_t *obj1, lv_event_cb_t event_cb1)
{   
    /**
     * @brief add the event handler to the button(triangle one)
     */
     
    lv_obj_set_event_cb(_sqBTNTria, event_cb1);
    // lv_obj_set_event_cb(obj1, event_cb1);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
