/*********************
 *      INCLUDES
 *********************/
#include "ResumeInfoWidget.h"
#include "esp_upm.h"
/**********************
 *  STATIC VARIABLES
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

// Prototype

static void arc_loader(lv_task_t *t);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t _xArcStyle;

lv_obj_t *_xStopBtn;

lv_obj_t *pxArc;
lv_obj_t *var_liter_Label;

lv_obj_t *pxTextLabel;

lv_obj_t *var_hour_Label_Int;
lv_obj_t *var_liter_Label_Int;
lv_obj_t *var_hour_Label_Float;
lv_obj_t *var_liter_Label_Float;

lv_obj_t *literpoint;
lv_obj_t *UnitLiter;
lv_obj_t *hourpoint;
lv_obj_t *UnitHour;

lv_obj_t *pxTaskNumberLabel;
lv_obj_t *_xCurrentTaskNumberLabel;
lv_obj_t *_xTotalTaskNumberLabel;
lv_obj_t *_xTimeLeftHourlabel;
lv_obj_t *_xTimeleftMinLabel;

lv_obj_t *pxPercentLabel;
lv_obj_t *_xTimeleftHourTextLabel;
lv_obj_t *_xTimeleftMinTextLabel;


extern char *dashboardBTNTxt;

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a info widget
 * @param pxParent pointer to an object, it will be the parent of the new object
 * @return pointer to the created object
 */

lv_obj_t *pxCreateResumeInfo(lv_obj_t *pxParent)
{
	lv_obj_t *pxContainer = lv_obj_create(pxParent, NULL);
	lv_obj_set_size(pxContainer, RESUMEINFO_CONTAINER_WIDTH, RESUMEINFO_CONTAINER_HEIGHT);

	// Allocate the object type specific extended data
	ResumeInfoExt_t *pxExt = lv_obj_allocate_ext_attr(pxContainer, sizeof(ResumeInfoExt_t));
	LV_ASSERT_MEM(pxExt);
	if (pxExt == NULL)
	{
		return NULL;
	}

	// create arc style

	lv_style_init(&_xArcStyle);
	lv_style_set_line_color(&_xArcStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_border_opa(&_xArcStyle, LV_BTNMATRIX_PART_BG, LV_OPA_MIN);
	lv_style_set_line_width(&_xArcStyle, LV_STATE_DEFAULT, 3);

	// Create an Arc
	pxArc = lv_arc_create(pxContainer, NULL);
	lv_obj_add_style(pxArc, LV_ARC_PART_BG, &_xArcStyle);
	lv_arc_set_angles(pxArc, 0, 360);
	lv_arc_set_bg_angles(pxArc, 0, 360);
	lv_obj_set_size(pxArc, RESUMEINFO_ARC_HEIGHT, RESUMEINFO_ARC_WIDTH);
	lv_obj_set_style_local_line_width(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 20);
	lv_obj_align(pxArc, pxContainer, LV_ALIGN_IN_TOP_MID, 0, 35);

	// Create Label for Current Status

	static lv_style_t _xTextStyle;
	lv_style_init(&_xTextStyle);
	lv_style_set_line_color(&_xTextStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTextStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTextStyle, LV_STATE_DEFAULT, pxTextLabel_font);
	lv_style_set_text_color(&_xTextStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	pxTextLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(pxTextLabel, LV_LABEL_PART_MAIN, &_xTextStyle);
	lv_label_set_long_mode(pxTextLabel, LV_LABEL_LONG_BREAK);
	lv_label_set_align(pxTextLabel, LV_LABEL_ALIGN_CENTER);
	lv_obj_set_width(pxTextLabel, RESUMEINFO_CONTAINER_WIDTH);
	lv_label_set_text(pxTextLabel, "");
	// lv_obj_align(pxTextLabel, pxParent, LV_ALIGN_IN_TOP_LEFT, -80, 10);
	lv_obj_align(pxTextLabel, pxArc, LV_ALIGN_OUT_TOP_LEFT, -80, 0);

	// Create label for percent value inside Arc

	static lv_style_t _xPercentLabelStyle;
	lv_style_init(&_xPercentLabelStyle);
	lv_style_set_line_color(&_xPercentLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xPercentLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xPercentLabelStyle, LV_STATE_DEFAULT, _xPercentLabelStyle_font);
	lv_style_set_text_color(&_xPercentLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	pxPercentLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(pxPercentLabel, LV_LABEL_PART_MAIN, &_xPercentLabelStyle);
	lv_obj_set_width(pxPercentLabel, RESUMEINFO_CONTAINER_WIDTH);
	lv_label_set_align(pxPercentLabel, LV_LABEL_ALIGN_CENTER);
	lv_label_set_text(pxPercentLabel, "0");
	lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, 0, RESUMEINFO_PERCENT_OFFSET);

	// Create Label for percent symbol inside arc

	static lv_style_t _xSymbolStyle;
	lv_style_init(&_xSymbolStyle);
	lv_style_set_line_color(&_xSymbolStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xSymbolStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xSymbolStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xSymbolStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	lv_obj_t *pxPercentSymbol = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(pxPercentSymbol, LV_LABEL_PART_MAIN, &_xSymbolStyle);
	lv_label_set_text(pxPercentSymbol, "");
	lv_obj_align(pxPercentSymbol, pxPercentLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	// Create Label for Timeleft Hour Value inside arc

	static lv_style_t _xTimeleftHourLabelStyle;
	lv_style_init(&_xTimeleftHourLabelStyle);
	lv_style_set_line_color(&_xTimeleftHourLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTimeleftHourLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTimeleftHourLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xTimeleftHourLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	_xTimeLeftHourlabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(_xTimeLeftHourlabel, LV_LABEL_PART_MAIN, &_xTimeleftHourLabelStyle);
	lv_label_set_text(_xTimeLeftHourlabel, "");
	lv_obj_align(_xTimeLeftHourlabel, pxPercentLabel, LV_ALIGN_OUT_BOTTOM_MID, -35, 10);

	// Create Label for Timeleft Hour Text inside arc
	static lv_style_t _xTimeleftHourTextLabelStyle;
	lv_style_init(&_xTimeleftHourTextLabelStyle);
	lv_style_set_line_color(&_xTimeleftHourTextLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTimeleftHourTextLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTimeleftHourTextLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xTimeleftHourTextLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	_xTimeleftHourTextLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(_xTimeleftHourTextLabel, LV_LABEL_PART_MAIN, &_xTimeleftHourTextLabelStyle);
	lv_label_set_text(_xTimeleftHourTextLabel, "");
	lv_obj_align(_xTimeleftHourTextLabel, _xTimeLeftHourlabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	// Create Label for Timeleft Min Value inside arc
	static lv_style_t _xTimeleftMinLabelStyle;
	lv_style_init(&_xTimeleftMinLabelStyle);
	lv_style_set_line_color(&_xTimeleftMinLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTimeleftMinLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTimeleftMinLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xTimeleftMinLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	_xTimeleftMinLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(_xTimeleftMinLabel, LV_LABEL_PART_MAIN, &_xTimeleftMinLabelStyle);
	lv_label_set_text(_xTimeleftMinLabel, "");
	lv_obj_align(_xTimeleftMinLabel, _xTimeleftHourTextLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	// Create Label for Timeleft Min Text inside arc
	static lv_style_t _xTimeleftMinTextLabelStyle;
	lv_style_init(&_xTimeleftMinTextLabelStyle);
	lv_style_set_line_color(&_xTimeleftMinTextLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTimeleftMinTextLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTimeleftMinTextLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xTimeleftMinTextLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	_xTimeleftMinTextLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(_xTimeleftMinTextLabel, LV_LABEL_PART_MAIN, &_xTimeleftMinTextLabelStyle);
	lv_label_set_text(_xTimeleftMinTextLabel, "");
	lv_obj_align(_xTimeleftMinTextLabel, _xTimeleftMinLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	// Create Label for Task Number Seprator inside arc

	static lv_style_t _xTaskNumberLabelStyle;
	lv_style_init(&_xTaskNumberLabelStyle);
	lv_style_set_line_color(&_xTaskNumberLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTaskNumberLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTaskNumberLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xTaskNumberLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	pxTaskNumberLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(pxTaskNumberLabel, LV_LABEL_PART_MAIN, &_xSymbolStyle);
	lv_label_set_text(pxTaskNumberLabel, "");
	lv_obj_align(pxTaskNumberLabel, _xTimeleftMinLabel, LV_ALIGN_OUT_BOTTOM_MID, -16, 5);

	// Create Label for Task Number Total inside arc

	static lv_style_t _xTotalTaskNumberLabelStyle;
	lv_style_init(&_xTotalTaskNumberLabelStyle);
	lv_style_set_line_color(&_xTotalTaskNumberLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xTotalTaskNumberLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xTotalTaskNumberLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xTotalTaskNumberLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	_xTotalTaskNumberLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(_xTotalTaskNumberLabel, LV_LABEL_PART_MAIN, &_xTotalTaskNumberLabelStyle);
	lv_label_set_text(_xTotalTaskNumberLabel, "");
	lv_obj_align(_xTotalTaskNumberLabel, pxTaskNumberLabel, LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);

	// Create Label for Task Number Current inside arc

	static lv_style_t _xCurrentTaskNumberLabelStyle;
	lv_style_init(&_xCurrentTaskNumberLabelStyle);
	lv_style_set_line_color(&_xCurrentTaskNumberLabelStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&_xCurrentTaskNumberLabelStyle, LV_STATE_DEFAULT, 3);
	lv_style_set_text_font(&_xCurrentTaskNumberLabelStyle, LV_STATE_DEFAULT, _xSymbolStyle_font);
	lv_style_set_text_color(&_xCurrentTaskNumberLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

	_xCurrentTaskNumberLabel = lv_label_create(pxContainer, NULL);
	lv_obj_add_style(_xCurrentTaskNumberLabel, LV_LABEL_PART_MAIN, &_xCurrentTaskNumberLabelStyle);
	lv_label_set_text(_xCurrentTaskNumberLabel, "");
	lv_obj_align(_xCurrentTaskNumberLabel, pxTaskNumberLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	// Create Horizontal Line

	lv_obj_t *hor_line = lv_line_create(pxContainer, NULL);
	static lv_point_t p[] = {{0, 25}, {0, 85}};
	lv_line_set_points(hor_line, p, 2);

	lv_obj_align(hor_line, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

	static lv_style_t style_hor_line;
	lv_style_init(&style_hor_line);

	lv_style_set_line_color(&style_hor_line, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&style_hor_line, LV_STATE_DEFAULT, 3);
	lv_style_set_line_rounded(&style_hor_line, LV_STATE_DEFAULT, true);

	lv_obj_add_style(hor_line, LV_LINE_PART_MAIN, &style_hor_line);
	lv_obj_set_hidden(hor_line, true);

	// Create total hour label fix
	lv_obj_t *fix_hour_Label = lv_label_create(pxContainer, NULL);
	lv_obj_align(fix_hour_Label, pxContainer, LV_ALIGN_IN_BOTTOM_RIGHT, -90, -55);
	lv_label_set_text(fix_hour_Label, "TOTAL HOURS");

	static lv_style_t style_fix_hour_Label;
	lv_style_init(&style_fix_hour_Label);
	lv_style_set_text_font(&style_fix_hour_Label, LV_STATE_DEFAULT, &lv_font_montserrat_14);
	lv_style_set_text_color(&style_fix_hour_Label, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
	lv_obj_add_style(fix_hour_Label, LV_LABEL_PART_MAIN, &style_fix_hour_Label);

	// Create total Liter label fix

	lv_obj_t *fix_liters_Label = lv_label_create(pxContainer, NULL);
	lv_obj_align(fix_liters_Label, pxContainer, LV_ALIGN_IN_BOTTOM_LEFT, 20, -55);
	lv_label_set_text(fix_liters_Label, "TOTAL LITERS");

	static lv_style_t style_fix_liters_Label;
	lv_style_init(&style_fix_liters_Label);
	lv_style_set_text_font(&style_fix_liters_Label, LV_STATE_DEFAULT, &lv_font_montserrat_14);
	lv_style_set_text_color(&style_fix_liters_Label, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
	lv_obj_add_style(fix_liters_Label, LV_LABEL_PART_MAIN, &style_fix_liters_Label);

	// Create total Liter label Variable integer part

	var_liter_Label_Int = lv_label_create(pxContainer, NULL);
	lv_obj_align(var_liter_Label_Int, fix_liters_Label, LV_ALIGN_OUT_BOTTOM_MID, -35, 10);
	lv_label_set_text(var_liter_Label_Int, "");
	lv_label_set_align(var_liter_Label_Int, LV_LABEL_ALIGN_CENTER);

	static lv_style_t style_var_liter_Label_Int;
	lv_style_init(&style_var_liter_Label_Int);
	lv_style_set_text_font(&style_var_liter_Label_Int, LV_STATE_DEFAULT, &lv_font_montserrat_22);
	lv_style_set_text_color(&style_var_liter_Label_Int, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(var_liter_Label_Int, LV_LABEL_PART_MAIN, &style_var_liter_Label_Int);

	// Create point txt
	literpoint = lv_label_create(pxContainer, NULL);
	lv_obj_align(literpoint, var_liter_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, -20, -5);
	lv_label_set_text(literpoint, ".");
	lv_label_set_align(literpoint, LV_LABEL_ALIGN_CENTER);

	static lv_style_t style_literpoint;
	lv_style_init(&style_literpoint);
	lv_style_set_text_font(&style_literpoint, LV_STATE_DEFAULT, &lv_font_montserrat_22);
	lv_style_set_text_color(&style_literpoint, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(literpoint, LV_LABEL_PART_MAIN, &style_literpoint);

	// Create total Liter label Variable float part

	var_liter_Label_Float = lv_label_create(pxContainer, NULL);
	lv_obj_align(var_liter_Label_Float, literpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_label_set_text(var_liter_Label_Float, "11");
	lv_label_set_align(var_liter_Label_Float, LV_LABEL_ALIGN_CENTER);

	static lv_style_t style_var_liter_Label_Float;
	lv_style_init(&style_var_liter_Label_Float);
	lv_style_set_text_font(&style_var_liter_Label_Float, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&style_var_liter_Label_Float, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(var_liter_Label_Float, LV_LABEL_PART_MAIN, &style_var_liter_Label_Float);

	// Create total Liter L Unit
	UnitLiter = lv_label_create(pxContainer, NULL);
	lv_obj_align(UnitLiter, var_liter_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 7, 0);
	lv_label_set_text(UnitLiter, "L");
	lv_label_set_align(UnitLiter, LV_LABEL_ALIGN_CENTER);

	static lv_style_t style_UnitLiter;
	lv_style_init(&style_UnitLiter);
	lv_style_set_text_font(&style_UnitLiter, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&style_UnitLiter, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(UnitLiter, LV_LABEL_PART_MAIN, &style_UnitLiter);

	// Create total hour label Var Inter Part
	var_hour_Label_Int = lv_label_create(pxContainer, NULL);
	lv_obj_align(var_hour_Label_Int, fix_hour_Label, LV_ALIGN_OUT_BOTTOM_MID, -40, 10);
	lv_label_set_text(var_hour_Label_Int, "");

	static lv_style_t style_var_hour_Label_Int;
	lv_style_init(&style_var_hour_Label_Int);
	lv_style_set_text_font(&style_var_hour_Label_Int, LV_STATE_DEFAULT, &lv_font_montserrat_22);
	lv_style_set_text_color(&style_var_hour_Label_Int, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(var_hour_Label_Int, LV_LABEL_PART_MAIN, &style_var_hour_Label_Int);

	// create a point txt
	hourpoint = lv_label_create(pxContainer, NULL);
	lv_obj_align(hourpoint, var_hour_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, -5);
	lv_label_set_text(hourpoint, ".");
	lv_label_set_align(hourpoint, LV_LABEL_ALIGN_CENTER);

	static lv_style_t style_hourpoint;
	lv_style_init(&style_hourpoint);
	lv_style_set_text_font(&style_hourpoint, LV_STATE_DEFAULT, &lv_font_montserrat_22);
	lv_style_set_text_color(&style_hourpoint, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(hourpoint, LV_LABEL_PART_MAIN, &style_hourpoint);

	// Create total hour label Var Float Part
	var_hour_Label_Float = lv_label_create(pxContainer, NULL);
	lv_obj_align(var_hour_Label_Float, hourpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_label_set_text(var_hour_Label_Float, "");

	static lv_style_t style_var_hour_Label_Float;
	lv_style_init(&style_var_hour_Label_Float);
	lv_style_set_text_font(&style_var_hour_Label_Float, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&style_var_hour_Label_Float, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(var_hour_Label_Float, LV_LABEL_PART_MAIN, &style_var_hour_Label_Float);

	// Create Hour Unit Text
	UnitHour = lv_label_create(pxContainer, NULL);
	lv_obj_align(UnitHour, var_hour_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_label_set_text(UnitHour, "h");

	static lv_style_t style_UnitHour;
	lv_style_init(&style_UnitHour);
	lv_style_set_text_font(&style_UnitHour, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&style_UnitHour, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
	lv_obj_add_style(UnitHour, LV_LABEL_PART_MAIN, &style_UnitHour);

	lv_obj_set_hidden(UnitHour, true);
	lv_obj_set_hidden(var_hour_Label_Float, true);
	lv_obj_set_hidden(hourpoint, true);
	lv_obj_set_hidden(var_hour_Label_Int, true);
	lv_obj_set_hidden(UnitLiter, true);
	lv_obj_set_hidden(var_liter_Label_Float, true);
	lv_obj_set_hidden(literpoint, true);
	lv_obj_set_hidden(var_liter_Label_Int, true);
	lv_obj_set_hidden(fix_hour_Label, true);
	lv_obj_set_hidden(fix_liters_Label, true);
	//-----------------------------------------------------------------------------------------------------

	pxExt->pxContainer = pxContainer;
	pxExt->pxPercentValue = pxPercentLabel;
	pxExt->pxPercentSymbol = pxPercentSymbol;
	pxExt->pxTextLabel = pxTextLabel;
	pxExt->pxProgressArc = pxArc;

	pxExt->pxVar_liter_value = var_liter_Label_Int;
	pxExt->pxVar_liter_valueFloat = var_liter_Label_Float;
	pxExt->pxIntHour_Value = var_hour_Label_Int;
	pxExt->pxFloatHour_Value = var_hour_Label_Float;

	pxExt->pxHourValueArc = _xTimeLeftHourlabel;
	pxExt->pxMinutValueArc = _xTimeleftMinLabel;

	pxExt->pxTotalTaskNumberValue = _xTotalTaskNumberLabel;
	pxExt->pxCurrentTaskNumberValue = _xCurrentTaskNumberLabel;

	pxExt->pxRemainingHour = _xTimeleftHourTextLabel;
	pxExt->pxRemainingminut = _xTimeleftMinLabel;

	pxExt->xState = RESUMEINFO_READY;

	return pxContainer;

	//----------------------------------------------------------------------------------------------
}

/**
 * Set the state for the info widget object
 * @param pxObj pointer to a widget object
 * @param xState a new state
 * @param pcText text for pxTextLabel
 */
void vSetResumeInfoState(lv_obj_t *pxObj, ResumeInfoState_t xState, const char *pcText)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);

	pxExt->xState = xState;
	lv_label_set_text(pxExt->pxTextLabel, pcText);
	lv_obj_align(pxTextLabel, pxArc, LV_ALIGN_OUT_TOP_LEFT, -100, 0);
	switch (pxExt->xState)
	{
	case RESUMEINFO_READY:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		lv_label_set_text_fmt(pxExt->pxPercentValue, "%c%c%c\n %c%c%c%c", devicesettings.device_serial_number[0],
			devicesettings.device_serial_number[1], devicesettings.device_serial_number[2], devicesettings.device_serial_number[3], 
			devicesettings.device_serial_number[4], devicesettings.device_serial_number[5], devicesettings.device_serial_number[6]);
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_30);
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		lv_obj_align(pxTextLabel, pxArc, LV_ALIGN_OUT_TOP_LEFT, -140, 0);
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, 0, 60);
		lv_task_create(arc_loader, 20, LV_TASK_PRIO_HIGH, pxArc);
		dashboardBTNTxt = "START JOB ";
		break;
	case RESUMEINFO_PROBLEM:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xC3, 0x34, 0x38));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		lv_label_set_text(pxExt->pxPercentValue, " \nPower \nissue");
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_26);
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, -3, RESUMEINFO_PERCENT_OFFSET);
		// lv_label_set_text(pxExt->pxPercentValue,  "");
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		dashboardBTNTxt = "  VIEW SUMMARY";
		break;
	case RESUMEINFO_METROLOGY_IN_PROGRESS:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x7A, 0xD0, 0xB7));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_label_set_text(pxExt->pxPercentSymbol, "0%");
		lv_obj_align(pxExt->pxPercentValue, pxExt->pxProgressArc, LV_ALIGN_IN_TOP_MID, 0, RESUMEINFO_PERCENT_OFFSET);
		lv_obj_align(pxExt->pxPercentSymbol, pxExt->pxPercentValue, LV_ALIGN_OUT_RIGHT_MID, RESUMEINFO_SYMBOL_OFFSET_X, RESUMEINFO_SYMBOL_OFFSET_Y);
		dashboardBTNTxt = "START JOB ";
		break;
	case RESUMEINFO_WORK_IN_PROGRESS:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x54, 0x83, 0xAF));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_label_set_text(pxExt->pxPercentSymbol, "0%");
		lv_obj_align(pxExt->pxPercentValue, pxExt->pxProgressArc, LV_ALIGN_IN_TOP_MID, 0, RESUMEINFO_PERCENT_OFFSET);
		lv_obj_align(pxExt->pxPercentSymbol, pxExt->pxPercentValue, LV_ALIGN_OUT_RIGHT_MID, RESUMEINFO_SYMBOL_OFFSET_X, RESUMEINFO_SYMBOL_OFFSET_Y);
		//===
		lv_label_set_text(_xTimeleftHourTextLabel, "00h");
		// lv_label_set_text_fmt(_xTimeleftHourTextLabel, "%02dh", 1);
		lv_obj_align(_xTimeleftHourTextLabel, _xTimeLeftHourlabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
		lv_label_set_text(_xTimeleftMinLabel, "00m");
		// lv_label_set_text_fmt(_xTimeleftMinLabel, "%02dm", 23);
		lv_obj_align(_xTimeleftMinLabel, _xTimeleftHourTextLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

		lv_label_set_text(pxTaskNumberLabel, "/");
		lv_obj_align(pxTaskNumberLabel, _xTimeleftMinLabel, LV_ALIGN_OUT_BOTTOM_MID, -16, 5);
		lv_label_set_text(_xTotalTaskNumberLabel, "5");
		// lv_label_set_text_fmt(_xTotalTaskNumberLabel, "%d", 5);
		lv_obj_align(_xTotalTaskNumberLabel, pxTaskNumberLabel, LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);
		lv_label_set_text(_xCurrentTaskNumberLabel, "2");
		// lv_label_set_text_fmt(_xCurrentTaskNumberLabel, "%d", 2);
		lv_obj_align(_xCurrentTaskNumberLabel, pxTaskNumberLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
		dashboardBTNTxt = "STOP";
		break;
	case RESUMEINFO_WAIT:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xD5, 0xDE, 0x54));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		
		char cStrtTime[30];
		char cStrtDate[40];
		/* Getting the date and time of the current sequence from sample management api and time management api  */
		get_start_date_time_sequence(uGetCurrentRunningSequenceNumber(), cStrtDate, sizeof(cStrtDate), cStrtTime, sizeof(cStrtTime));
		lv_label_set_text_fmt(pxExt->pxPercentValue, "\nStarts at\n   %s  \n\t\t\t%s\n", cStrtTime, cStrtDate);	
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20); // LV_FONT_MONTSERRAT_24
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, -2, RESUMEINFO_PERCENT_OFFSET);
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		lv_task_create(arc_loader, 20, LV_TASK_PRIO_HIGH, pxArc);
		dashboardBTNTxt = "FORCE STOP";
		break;
	case RESUMEINFO_JOB_FINISHED:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x67, 0xD6));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		// lv_label_set_text_fmt(pxExt->pxPercentValue, "\nStarts in\n  %02dh %02dm", 10, 15);
		lv_label_set_text(pxExt->pxPercentValue, "\n finished  \nsuccessfully");
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_22); // LV_FONT_MONTSERRAT_24
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, -2, RESUMEINFO_PERCENT_OFFSET);
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		lv_task_create(arc_loader, 20, LV_TASK_PRIO_HIGH, pxArc);
		dashboardBTNTxt = "VIEW SUMMARY";
		break;

	case RESUMEINFO_ALERT_SERVICE:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xEb, 0x3B, 0x5A));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		// lv_label_set_text_fmt(pxExt->pxPercentValue, "\nStarts in\n  %02dh %02dm", 10, 15);
		lv_label_set_text_fmt(pxExt->pxPercentValue, "  Alert  \n service \nneeded", 12);
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24); // LV_FONT_MONTSERRAT_24
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, -2, RESUMEINFO_PERCENT_OFFSET);
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		lv_task_create(arc_loader, 20, LV_TASK_PRIO_HIGH, pxArc);
		dashboardBTNTxt = "FORCE START";
		break;
	case RESUMEINFO_METROLOGY_NEEDED:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFa, 0x82, 0x31));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		// lv_label_set_text_fmt(pxExt->pxPercentValue, "\nStarts in\n  %02dh %02dm", 10, 15);
		lv_label_set_text_fmt(pxExt->pxPercentValue, "\nMetrology\n   in   \n  %02d Days", 12);
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24); // LV_FONT_MONTSERRAT_24
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, -2, RESUMEINFO_PERCENT_OFFSET);
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		lv_task_create(arc_loader, 20, LV_TASK_PRIO_HIGH, pxArc);
		dashboardBTNTxt = "START JOB";
		break;
	case RESUMEINFO_EXPORT_DATA:
		lv_obj_set_style_local_line_color(pxArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xA5, 0x5E, 0xea));
		lv_obj_set_style_local_bg_opa(pxArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_arc_set_angles(pxExt->pxProgressArc, 0, 360);
		// lv_label_set_text_fmt(pxExt->pxPercentValue, "\nStarts in\n  %02dh %02dm", 10, 15);
		lv_label_set_text_fmt(pxExt->pxPercentValue, "%s", " Export \n   in   \nprogress ");
		lv_obj_set_style_local_text_font(pxPercentLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24); // LV_FONT_MONTSERRAT_24
		lv_obj_align(pxPercentLabel, pxArc, LV_ALIGN_IN_TOP_MID, -2, RESUMEINFO_PERCENT_OFFSET);
		lv_label_set_text(pxExt->pxPercentSymbol, "");
		lv_task_create(arc_loader, 20, LV_TASK_PRIO_HIGH, pxArc);
		dashboardBTNTxt = "STOP";
		break;
	}

	lv_obj_add_style(pxExt->pxProgressArc, LV_OBJ_PART_MAIN, &_xArcStyle);
}

/**
 * Set the percent value
 * @param pxObj pointer to a widget object
 * @param usPercent a new value
 */
void vSetResumeInfoPercent(lv_obj_t *pxObj, int usPercent)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);

	int usAngle = usPercent * 360 / 100;

	if (lv_arc_get_angle_end(pxExt->pxProgressArc) == usAngle)
	{
		return;
	}

	if (pxExt->xState == RESUMEINFO_METROLOGY_IN_PROGRESS || pxExt->xState == RESUMEINFO_WORK_IN_PROGRESS)
	{
		lv_arc_set_angles(pxExt->pxProgressArc, 0, usAngle);
		char acPercent[3];
		sprintf(acPercent, "%d", usPercent);
		lv_label_set_text(pxExt->pxPercentValue, acPercent);
		lv_obj_align(pxExt->pxPercentValue, pxExt->pxProgressArc, LV_ALIGN_IN_TOP_MID, -lv_obj_get_width(pxExt->pxPercentSymbol) / 2, RESUMEINFO_PERCENT_OFFSET);
		lv_obj_align(pxExt->pxPercentSymbol, pxExt->pxPercentValue, LV_ALIGN_OUT_RIGHT_MID, RESUMEINFO_SYMBOL_OFFSET_X, RESUMEINFO_SYMBOL_OFFSET_Y);
		// lv_obj_align(pxExt->pxTextLabel, pxExt->pxPercentValue, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	}
	else if (pxExt->xState == RESUMEINFO_WAIT)
	{
		lv_arc_set_angles(pxExt->pxProgressArc, 0, usAngle);
	}
}

/******************************************************
 * Set the Liters value
 * @param pxObj pointer to a widget object
 * @param usLiters a new value integer part of liters
 *****************************************************/

void vSetResumeInfoLitersInt(lv_obj_t *pxObj, uint32_t usLiters)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLiters[11];
	sprintf(acLiters, "%06d", usLiters);
	lv_label_set_text(pxExt->pxVar_liter_value, acLiters);
	lv_obj_align(literpoint, var_liter_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(var_liter_Label_Float, literpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(UnitLiter, var_liter_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}

/**
 * Set the Liters value
 * @param pxObj pointer to a widget object
 * @param usLitersF a Float part of liter
 */

void vSetResumeInfoLitersFloat(lv_obj_t *pxObj, uint32_t usLitersF)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usLitersF);
	lv_label_set_text(pxExt->pxVar_liter_valueFloat, acLitersF);
	lv_obj_align(literpoint, var_liter_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(var_liter_Label_Float, literpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(UnitLiter, var_liter_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}

/**
 * Set the Int Hour value
 * @param pxObj pointer to a widget object
 * @param usHourInt a Float part of liter
 */

void vSetResumeInfoHourInt(lv_obj_t *pxObj, uint32_t usHourInt)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[11];
	sprintf(acLitersF, "%06d", usHourInt);
	lv_label_set_text(pxExt->pxIntHour_Value, acLitersF);
	lv_obj_align(hourpoint, var_hour_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(var_hour_Label_Float, hourpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(UnitHour, var_hour_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}

/**
 * Set the Float Hour value
 * @param pxObj pointer to a widget object
 * @param usHourInt a Float part of liter
 */
void vSetResumeInfoHourFloat(lv_obj_t *pxObj, uint32_t usHourFloat)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usHourFloat);
	lv_label_set_text(pxExt->pxFloatHour_Value, acLitersF);
	lv_obj_align(hourpoint, var_hour_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(var_hour_Label_Float, hourpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(UnitHour, var_hour_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}

//================================================================

/**
 * Set the Total sequence number
 * @param pxObj pointer to a widget object
 * @param usHourInt a total sequence number
 */
void vSetResumeInfoTotalSeq(lv_obj_t *pxObj, uint32_t usTotalSeqNum)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usTotalSeqNum);
	lv_label_set_text(pxExt->pxTotalTaskNumberValue, acLitersF);
	lv_obj_align(_xTotalTaskNumberLabel, pxTaskNumberLabel, LV_ALIGN_OUT_LEFT_BOTTOM, -2, 0);
}

/**
 * Set the Current sequence number
 * @param pxObj pointer to a widget object
 * @param usHourInt a current sequence number
 */
void vSetResumeInfoCurrentSeq(lv_obj_t *pxObj, uint32_t usCurrentSeqNum)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usCurrentSeqNum);
	lv_label_set_text(pxExt->pxCurrentTaskNumberValue, acLitersF);
	lv_obj_align(_xCurrentTaskNumberLabel, pxTaskNumberLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 2, 0);
}

/**
 * Set the Remaining Hour Value
 * @param pxObj pointer to a widget object
 * @param usRemHour a Hour value
 */
void vSetResumeInfoRemainingHour(lv_obj_t *pxObj, uint32_t usRemHour)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[5];
	sprintf(acLitersF, "%02dH", usRemHour);
	lv_label_set_text(pxExt->pxRemainingHour, acLitersF);
	lv_obj_align(_xTimeleftHourTextLabel, _xTimeLeftHourlabel, LV_ALIGN_OUT_RIGHT_BOTTOM, -2, 0);
}

/**
 * Set the Remaining Minute Value
 * @param pxObj pointer to a widget object
 * @param usRemHour a Minute value
 */
void vSetResumeInfoRemainingMinute(lv_obj_t *pxObj, uint32_t usRemMin)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersF[5];
	sprintf(acLitersF, "%02dm", usRemMin);
	lv_label_set_text(pxExt->pxRemainingminut, acLitersF);
	lv_obj_align(_xTimeleftMinLabel, _xTimeleftHourTextLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 2, 0);
}

/**
 * Set the Total Hours value (Hour & Min Both)
 * @param pxObj pointer to a widget object
 * @param usHourInt a Int part of liter
 * @param usHourFloatt a Float part of liter
 */

void vSetResumeInfoHour(lv_obj_t *pxObj, uint32_t usHourInt, uint32_t usHourFloat)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersI[11];
	sprintf(acLitersI, "%06d", usHourInt);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usHourFloat);

	lv_label_set_text(pxExt->pxIntHour_Value, acLitersI);

	lv_obj_align(hourpoint, var_hour_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(var_hour_Label_Float, hourpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(UnitHour, var_hour_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	lv_label_set_text(pxExt->pxFloatHour_Value, acLitersF);

	lv_obj_align(hourpoint, var_hour_Label_Int, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(var_hour_Label_Float, hourpoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(UnitHour, var_hour_Label_Float, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}

/**
 * Set the Total Hours value (Hour & Min Both)
 * @param pxObj pointer to a widget object
 * @param usHour a part of Hour
 * @param usMinutes a part of Min
 */

void vSetResumeInfoArcTime(lv_obj_t *pxObj, uint32_t usHour, uint32_t usMinute)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersI[5];
	sprintf(acLitersI, "%d", usHour);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usMinute);

	lv_label_set_text(pxExt->pxHourValueArc, acLitersI);

	lv_obj_align(_xTimeLeftHourlabel, pxPercentLabel, LV_ALIGN_OUT_BOTTOM_MID, -35, 10);
	lv_obj_align(_xTimeleftHourTextLabel, _xTimeLeftHourlabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(_xTimeleftMinLabel, _xTimeleftHourTextLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(_xTimeleftMinTextLabel, _xTimeleftMinLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

	lv_label_set_text(pxExt->pxMinutValueArc, acLitersF);

	lv_obj_align(_xTimeLeftHourlabel, pxPercentLabel, LV_ALIGN_OUT_BOTTOM_MID, -35, 10);
	lv_obj_align(_xTimeleftHourTextLabel, _xTimeLeftHourlabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(_xTimeleftMinLabel, _xTimeleftHourTextLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_align(_xTimeleftMinTextLabel, _xTimeleftMinLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}

/**
 * Set the Total Hours value (Hour & Min Both)
 * @param pxObj pointer to a widget object
 * @param  usTotalTask number of total tasks
 * @param usCurrentTask Current task number
 */

void vSetResumeInfoArcTask(lv_obj_t *pxObj, uint32_t usTotalTask, uint32_t usCurrentTask)
{
	ResumeInfoExt_t *pxExt = lv_obj_get_ext_attr(pxObj);
	char acLitersI[5];
	sprintf(acLitersI, "%d", usTotalTask);
	char acLitersF[5];
	sprintf(acLitersF, "%d", usCurrentTask);

	lv_label_set_text(pxExt->pxTotalTaskNumberValue, acLitersI);

	lv_label_set_text(pxExt->pxCurrentTaskNumberValue, acLitersF);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

// Dynamic Arc Loader used in ready state

static void arc_loader(lv_task_t *t)
{
	static int16_t a = 10;
	a += 15;
	lv_arc_set_bg_angles(pxArc, 0, 360);
	lv_arc_set_end_angle(t->user_data, a);
	lv_obj_set_click(_xStopBtn, false);

	if (a >= 10 + 360)
	{
		lv_task_del(t);
		lv_obj_set_click(_xStopBtn, true);
		a = 10;
		return;
	}
}
