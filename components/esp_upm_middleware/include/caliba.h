

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Name (mail or git)
 *  @date      Year
 */

/**
*  @file file
*  @brief Short description
*  @details Long description
*/

#ifndef CALIBA_H
#define CALIBA_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/

#include "middleware.h"
#include <stdbool.h>
#include <math.h>

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

float iCalPolyOne( float a,   float b, float xValue);
float iCalPolyTwo( float a,   float b, float c, float xValue);
float iCalPolyThree( float a, float b, float c, float d, float xValue);
float iCalPolyFour( float a, float b, float c, float d, float e, float xValue);
float iCalPolyFive( float a, float b, float c, float d, float e, float f, float xValue);
float iCalPolySix( float a, float b, float c, float d, float e, float f, float g, float xValue);
float iCalPolySeven( float a, float b, float c, float d, float e, float f, float g, float h, float xValue);
float iCalPolyEight( float a, float b, float c, float d, float e, float f, float g, float h, float i, float xValue);



float iCalibXdegPoly(int polyDegree, float Xaxis );


void gaussEliminationLS(int m, int n, double a[m][n], double x[n-1]);

void printMatrix(int m, int n, double matrix[m][n]);



/**
 * @brief This is the function to calculate polynomial Curve
 * 
 * @param polyDegree  : Enter the degree of polynomial
 * @param xyPoints  : Enter the number of points in (x,y) pair
 * @param x : Array containng X Values
 * @param y : Array Contaning Y Values
 * @return : No Return
 */
void iPolynomialNew(int polyDegree, int xyPoints, float x[20], float y[20]);

/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*CALIBA_H*/


