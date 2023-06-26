/**
 * @file lv_ex_conf.h
 * Configuration file for v7.4.0
 *
 */
/*
 * COPY THIS FILE AS lv_ex_conf.h
 */

#if 1 /*Set it to "1" to enable the content*/

#ifndef LV_EX_CONF_H
#define LV_EX_CONF_H


/*******************
 * GENERAL SETTING
 *******************/

/* Enable printf-ing data in demoes and examples */
#ifdef CONFIG_LV_EX_PRINTF
#define LV_EX_PRINTF       1
#else
#define LV_EX_PRINTF       0
#endif

#define LV_EX_KEYBOARD     0       /*Add PC keyboard support to some examples (`lv_drivers` repository is required)*/
#define LV_EX_MOUSEWHEEL   0       /*Add 'encoder' (mouse wheel) support to some examples (`lv_drivers` repository is required)*/

/*********************
 * DEMO USAGE
 *********************/



#endif /*LV_EX_CONF_H*/

#endif /*End of "Content enable"*/

