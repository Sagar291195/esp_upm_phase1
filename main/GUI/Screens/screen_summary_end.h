#ifndef SCREEN_SUMMARY_END_H
#define SCREEN_SUMMARY_END_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
lv_obj_t *crnt_screen;
lv_obj_t *scrSummaryEnd;
lv_obj_t *xseParentContainer_se;
lv_obj_t *_xseContStatusBar_se;
lv_obj_t *__xseTimeLabel_se;
lv_obj_t *__xseBatteryLabel_se;
lv_obj_t *__xseWifiLabel_se;
lv_obj_t *__xseSignalLabel_se;
lv_obj_t *_xseSummaryParent_se;
lv_obj_t *__xseParaHeadingCont_se;
lv_obj_t *___xseBackArrowLabel_se;
lv_obj_t *___xseSummaryHeadingLbl_se;
lv_obj_t *__xseBaseContainer_se;
lv_obj_t *___xseMark_se;
lv_obj_t *___xseStartTxt_se;
lv_obj_t *___xseStopTxt_se;
lv_obj_t *___xseStartDateTxt_se;
lv_obj_t *___xseStopDateTxt_se;
lv_obj_t *___xseStrtTmCont_se;
lv_obj_t *____xseStrtTmVal_se;
lv_obj_t *___xseStopTmCont_se;
lv_obj_t *____xseStopTmVal_se;
lv_obj_t *_xseSampleNumTxt_se;
lv_obj_t *_xseSampleNumVal_se;
lv_obj_t *_xseContFSP_se;
lv_obj_t *__xseFLPTxtLbl_se;
lv_obj_t *__xseVarFLPTxtLbl_se;
lv_obj_t *_xseContSQNum_se;
lv_obj_t *__xseSQNumLbl_se;
lv_obj_t *__xseVarSQNumLbl_se;
lv_obj_t *_xseContSQDur_se;
lv_obj_t *__xseSQDurLbl_se;
lv_obj_t *__xseVarSQDurLbl_se;
lv_obj_t *_xseContStart_se;
lv_obj_t *__xseStartLbl_se;
lv_obj_t *__xseVarStartLbl_se;
lv_obj_t *_xseContEndBy_se;
lv_obj_t *__xseContEndByLbl_se;
lv_obj_t *__xseVarEndByLbl_se;
lv_obj_t *_xseContStrtPerson_se;
lv_obj_t *__xseStrtPersonLbl_se;
lv_obj_t *__xseVarStrtPersonLbl_se;
lv_obj_t *_xseContStopPerson_se;
lv_obj_t *__xseStopPersonLbl_se;
lv_obj_t *__xseVarStopPersonLbl_se;
lv_obj_t *_xseContProblem_se;
lv_obj_t *__xseProblemLbl_se;
lv_obj_t *__xseVarProblemLbl_se;
lv_obj_t *hor_line;
lv_obj_t *_xseContVolumeCount_se;
lv_obj_t *__xseVolCountLbl_se;
lv_obj_t *_xseStartStopTxtValCont_se;
lv_obj_t *__xseStartTxt_se;
lv_obj_t *__xseStartVal_se;
lv_obj_t *__xseStopTxt_se;
lv_obj_t *__xseStopVal_se;
lv_obj_t *__xseTargetVal_se;
lv_obj_t *__xseTargetTxt_se;
lv_obj_t *__xseEffectiveVal_se;
lv_obj_t *__xseEffectiveTxt_se;
lv_obj_t *__xseVariationVal_se;
lv_obj_t *__xseVariationText_se;
lv_obj_t *hor_line1;
lv_obj_t *_xseContHourCount_se;
lv_obj_t *__xseHourCountLbl_se;
lv_obj_t *_xseHrStartStopTxtValCont_se;
lv_obj_t *__xseHrStartTxt_se;
lv_obj_t *__xseHrStartVal_se;
lv_obj_t *__xseHrStopTxt_se;
lv_obj_t *__xseHrStopVal_se;
lv_obj_t *__xseHrTargetVal_se;
lv_obj_t *__xseHrTargetTxt_se;
lv_obj_t *__xseHrEffectiveVal_se;
lv_obj_t *__xseHrEffectiveTxt_se;
lv_obj_t *__xseHrVariationVal_se;
lv_obj_t *__xseHrVariationText_se;
lv_obj_t *hor_line2;
lv_obj_t *_xseSeque1_se;
lv_obj_t *Seq;
lv_obj_t *_xseSeque2_se;
lv_obj_t *Seq1;
lv_obj_t *_xseBTNCont_se;
lv_obj_t *_xseBTNUSB_se;
lv_obj_t *_xseBTNUSBLbl_se;
lv_obj_t *_xseBTNValid_se;
lv_obj_t *_xseBTNValidLbl_se;

lv_task_t *__xserefresherTask;

char guiMinDef[32];
char _xsDurHourRollerbuf[32];

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
void xseSummaryEndScreen(void);
void SequenceWidgetArrange(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_SUMMARY_END_H*/