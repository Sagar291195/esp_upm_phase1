
/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-10-01
 */

/**
*  @file 
*  @brief 
*  @details 
*/

/*
*       y1 = a + b * ( x )                                                              One degree polynimial
*       y2 = a + b * ( x ) + c * ( x * x )                                              Two degree polynomial
*       y3 = a + b * ( x ) + c * ( x * x ) + d * (x * x * x)                            Three degree polynomial
*       y4 = a + b * ( x ) + c * ( x * x ) + d * (x * x * x) + e * (x * x * x * x)      Four degree polynomial
*/


/*********************
 *      INCLUDES
 *********************/

#include "middleware.h"
//#include<conio.h>

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

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

float FlowPloyCoffA;
float FlowPloyCoffB;
float FlowPloyCoffC;
float FlowPloyCoffD;
float FlowPloyCoffE;
float FlowPloyCoffF;
float FlowPloyCoffG;
float FlowPloyCoffH;
float FlowPloyCoffI;
float FlowPloyCoffJ;
int FlowPolyDeg;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

float iCalPolyOne( float a, float b, float xValue)
{
    float y1 = a + b * xValue;
    //polyOut = y1;
    return y1;
}

float iCalPolyTwo( float a, float b, float c, float xValue)
{
    float y2 = a + b * xValue  + c * xValue * xValue ;
    //polyOut = y2;
    return y2;
}

float iCalPolyThree( float a, float b, float c, float d, float xValue)
{
    float y3 = a + b * xValue  + c * xValue * xValue + d * xValue * xValue * xValue;
    //polyOut = y3;
    return y3;
}

float iCalPolyFour( float a, float b, float c, float d, float e, float xValue)
{
    float y4 = a + b * xValue  + c * xValue * xValue + d *  xValue * xValue * xValue +  e *  xValue * xValue  * xValue * xValue;
    //polyOut = y4;
    return y4;
}



float iCalPolyFive( float a, float b, float c, float d, float e, float f, float xValue)
{
    float y5 = a + b* xValue + c*pow(xValue,2) + d*pow(xValue,3) +  e* pow(xValue,4) + f* pow(xValue,5);
    //polyOut = y5;
    return y5;
}

float iCalPolySix( float a, float b, float c, float d, float e, float f, float g, float xValue)
{
    float y6 = a + b* xValue + c* pow(xValue,2) 
                             + d* pow(xValue,3) 
                             + e* pow(xValue,4) 
                             + f* pow(xValue,5) 
                             + g* pow(xValue,6);
    //polyOut = y6;
    return y6;
}
float iCalPolySeven( float a, float b, float c, float d, float e, float f, float g, float h, float xValue)
{
    float y7 = a + b* xValue + c* pow(xValue,2) 
                             + d* pow(xValue,3) 
                             + e* pow(xValue,4) 
                             + f* pow(xValue,5) 
                             + g* pow(xValue,6)
                             + h* pow(xValue,7);
    //polyOut = y7;
    return y7;
}

float iCalPolyEight( float a, float b, float c, float d, float e, float f, float g, float h, float i, float xValue)
{
    float y8 = a + b* xValue + c* pow(xValue,2) 
                             + d* pow(xValue,3) 
                             + e* pow(xValue,4) 
                             + f* pow(xValue,5) 
                             + g* pow(xValue,6)
                             + h* pow(xValue,7)
                             + i* pow(xValue,8);
    //polyOut = y8;
    return y8;
}

float iCalibXdegPoly(int polyDegree, float Xaxis )
{
    float polyOut1 = 0;

    float xA = FlowPloyCoffA ; // Read This Value from NVS at boot time
    float xB = FlowPloyCoffB ; // Read This Value from NVS at boot time
    float xC = FlowPloyCoffC ; // Read This Value from NVS at boot time
    float xD = FlowPloyCoffD ; // Read This Value from NVS at boot time
    float xE = FlowPloyCoffE ; // Read This Value from NVS at boot time
    float xF = FlowPloyCoffF ;
    float xG = FlowPloyCoffG ;
    float xH = FlowPloyCoffH ;
    float xI = FlowPloyCoffI ;

    switch(polyDegree)
    {
        case 1:
            polyOut1 = iCalPolyOne(xA, xB, Xaxis);
            break;

        case 2:
            polyOut1 = iCalPolyTwo(xA, xB, xC, Xaxis);
            break;

        case 3:
            polyOut1 = iCalPolyThree(xA, xB, xC, xD, Xaxis);
            break;

        case 4:
            polyOut1 = iCalPolyFour(xA, xB, xC, xD, xE, Xaxis);
            break;

        case 5:
            polyOut1 = iCalPolyFive(xA, xB, xC, xD, xE, xF, Xaxis);
            break;

        case 6:
            polyOut1 = iCalPolySix(xA, xB, xC, xD, xE, xF, xG, Xaxis);
            break;

        case 7:
            polyOut1 = iCalPolySeven(xA, xB, xC, xD, xE, xF, xG, xH, Xaxis);
            break;

        case 8:
            polyOut1 = iCalPolyEight(xA, xB, xC, xD, xE, xF, xG, xH, xI, Xaxis);
            break;

    }

    return polyOut1;

}


void gaussEliminationLS(int m, int n, double a[m][n], double x[n-1])
{
    int i,j,k;
    for(i=0;i<m-1;i++){
        //Partial Pivoting
        for(k=i+1;k<m;k++){
            //If diagonal element(absolute vallue) is smaller than any of the terms below it
            if(fabs(a[i][i])<fabs(a[k][i])){
                //Swap the rows
                for(j=0;j<n;j++){                
                    double temp;
                    temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                }
            }
        }
        //Begin Gauss Elimination
        for(k=i+1;k<m;k++){
            double  term=a[k][i]/ a[i][i];
            for(j=0;j<n;j++){
                a[k][j]=a[k][j]-term*a[i][j];
            }
        }
         
    }
    //Begin Back-substitution
    for(i=m-1;i>=0;i--){
        x[i]=a[i][n-1];
        for(j=i+1;j<n-1;j++){
            x[i]=x[i]-a[i][j]*x[j];
        }
        x[i]=x[i]/a[i][i];
    }
             
}
/*******
Function that prints the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void printMatrix(int m, int n, double matrix[m][n])
{
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%lf\t",matrix[i][j]);
        }
        printf("\n");
    } 
}


     

/**
 * @brief This is the function to calculate polynomial Curve
 * 
 * @param polyDegree  : Enter the degree of polynomial
 * @param xyPoints  : Enter the number of points in (x,y) pair
 * @param x : Array containng X Values
 * @param y : Array Contaning Y Values
 * @return : No Return
 */


void iPolynomialNew(int polyDegree, int xyPoints, float x[20], float y[20])
{

   int m = polyDegree;          // degree of Polynomial
   int n = xyPoints;            // Points to 

   int i,j,k;
   float u, a[10], c[20][20], power, r;

    // m = 3; // degree of Polynomial
    // n = 5; // Points to 


    // float x[20] = { 2.0, 3.0, 4.0, 5.0, 6.0};
    // float y[20] = { 1.95, 3.5, 4.25, 4.85, 6.2};

   for(j=1;j<=m+1;j++)
   for(k=1;k<=m+1;k++){
      c[j][k]=0;
      for(i=1;i<=n;i++)
      {
         power=pow(x[i],j+k-2);
         c[j][k]=c[j][k]+power;
      }
   }
   for(j=1;j<=m+1;j++){
      c[j][m+2]=0;
      for(i=1;i<=n;i++)
      {
         r=pow(x[i],j-1);
         c[j][m+2]=c[j][m+2]+y[i]*r;
      }
   }
   for(i=1;i<=m+1;i++){
      for(j=1;j<=m+2;j++)
      {
         printf("%.2f\t",c[i][j]);
      }
      printf("\n");
   }
   for(k=1;k<=m+1;k++)
      for(i=1;i<=m+1;i++)
      {
         if(i!=k)
         {
            u=c[i][k]/c[k][k];
            for(j=k;j<=m+2;j++)
            {
               c[i][j]=c[i][j]-u*c[k][j];
            }
        }
   }
   printf(" Coefficients are following: \n");
   for(i=1;i<=m+1;i++)
   {
      a[i]=c[i][m+2]/c[i][i];
      //printf("a[%d]=%f\n",i,a[i]);

      switch (i)
      {
          case 1:
            iWriteNVSFloatNum(a[i], "CoffA"); // A+Bx+Cx^2+Dx^3 = y
            printf("CoffA : %f\n", a[i]);
            break;
          case 2:
            iWriteNVSFloatNum(a[i], "CoffB");
            printf("CoffB : %f\n", a[i]);
            break;
          case 3:
            iWriteNVSFloatNum(a[i], "CoffC");
            printf("CoffC : %f\n", a[i]);
            break;
          case 4:
            iWriteNVSFloatNum(a[i], "CoffD");
            printf("CoffD : %f\n", a[i]);
            break;
          case 5:
            iWriteNVSFloatNum(a[i], "CoffE");
            printf("CoffE : %f\n", a[i]);
            break;
          case 6:
            iWriteNVSFloatNum(a[i], "CoffF");
            printf("CoffF : %f\n", a[i]);
            break;
          case 7:
            iWriteNVSFloatNum(a[i], "CoffG");
            printf("CoffG : %f\n", a[i]);
            break;
          case 8:
            iWriteNVSFloatNum(a[i], "CoffH");
            printf("CoffH : %f\n", a[i]);
            break;
          case 9:
            iWriteNVSFloatNum(a[i], "CoffI");
            printf("CoffI : %f\n", a[i]);
            break;
          case 10:
            iWriteNVSFloatNum(a[i], "CoffJ");
            printf("CoffJ : %f\n", a[i]);
            break;
      }
      
   }
   
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
