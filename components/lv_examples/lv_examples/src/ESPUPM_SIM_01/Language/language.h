/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-19
 */



#ifndef LANGUAGE_H
#define LANGUAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "lvgl.h"
//#include "../lvgl/lvgl.h"
#include "lv_ex_conf.h"

/*********************
 *      DEFINES
 *********************/

#define N 19
#define M 19


#define Welcome_id 		0 
#define Today_id 		1 
#define is_id 			2
#define Ready_id		3 
#define Problem_id		4 
#define Metrology_id	5 
#define Progress_id		6 
#define Work_id			7 
#define progress_id		8 
#define Wait_id			9 
#define SERVICE_id		10 
#define ARCHIV_id		11 
#define METROLOGY_id	12 
#define PARAMETER_id	13 
#define INFO_id			14 
#define START_id		15 
#define JOB_id			16 
#define STOP_id			17 


/**********************
 *      TYPEDEFS
 **********************/

/*******************************
 * EXTERN  Variables
 *******************************/

char lang_EN[N][M] = { "Welcome", "Today", "is", "Ready", "Problem", "Metrology" ,
"in" ,"Progress", "Work" ,"progress", "Wait", "SERVICE", "ARCHIV", 
"METROLOGY", "PARAMETER", "INFO", "START" ,"JOB", "STOP"  };


char lang_FR[N][M] = { "Bienvenue", "Aujourd", "hui", "Prêt", "Probleme", "Métrologie", "en" ,
"cours", "Travail", "cours", "Attendre", "SERVICE", "ARCHIV", "METROLOGIE", "PARAMÈTRE", 
"INFO", "DÉMARRER", "TRAVAIL", "ARRÊTER"};




char (*ptr)[M];


/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LANGUAGE_H*/

