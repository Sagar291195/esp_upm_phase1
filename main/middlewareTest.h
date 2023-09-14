

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Ankit Bansal
 *  @date      2021
 */

/**
 *  @file file
 *  @brief Short description
 *  @details Long description
 */

#ifndef MIDDLEWARETEST_H
#define MIDDLEWARETEST_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "middleware.h"
#include "../../../main/GUI/Screens/screen_includes.h"
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *     TYPEDEFS
     **********************/

    typedef struct storeSample_struct
    {
        char startDateStruct[60];
        char startTimeStruct[100];
        char stopDateStruct[60];
        char stopTimeStruct[100];
        int sampleNumberStruct;
        char FlowSetPtStruct[32];
        char SeqNumberStruct[5];
        char durationHrStruct[32];
        char durationMinStruct[32];
        char startByStruct[25];
        char endByStruct[25];
        char startPersonStruct[25];
        char stopPersonStruct[25];
        char problemStruct[25];
        double startLiterStruct;
        double stopLiterStruct;
        double literTargetStruct;
        double literEffecStruct;
        double literVarStruct;
        float startHourStruct;
        float stopHourStruct;
        float hourTargetStruct;
        float hourEffecStruct;
        float hourVarStruct;
        // int     numberOfSeqs;

    } storeSample_t;
    /**
     * @brief Stores and retrieves the pid values from the nvs flash
     *
     */
    typedef struct struct_PID_parameters
    {
        float fKp;
        float fKi;
        float fKd;
        float fAkp;
        float fAki;
        float fAkd;
        float fNcoff;
        float fACoff;
    } struct_PID_parameters_t;

    /**********************
     *  GLOBAL MACROS
     **********************/

    /**********************
     *  GLOBAL VARIABLES
     **********************/

    char StartByReason[25];
    char EndByReason[25];

    char StartPerson[25];
    char StopPerson[25];

    char ProblemType[25];

    /**********************
     *  GLOBAL PROTOTYPES
     **********************/
    void test(void);
    void nvscheckfun1(void);
    // void stroreSequence(void);
    void seqWrite(char WseqDate[60], char WseqTime[130],
                  char WseqSetPt[32], char WseqDurHr[32],
                  char WseqDurMin[32], char WseqNum[5],
                  int WseqStatus);
    void seqRead(char SeqKey[5]);
    void write_TotalLiter(float Total_LiterInt);
    float read_TotalLiter(char TLiterKey[5]);
    void writeSampleNum(int SamNum);
    int read_SampleNum(char SampleNumKey[5]);
    void writeTotalhour(float HrValue);
    float read_Totalhour(char HrKey[5]);

    // void writeKpValue(float Kp1);
    void writeKpValue(float Kp1, char KpKey[6]);
    float ReadKpValue(char KpKey[6]);

    // void writeKiValue(float Ki1);
    void writeKiValue(float Ki1, char KiKey[5]);
    float ReadKiValue(char KiKey[5]);

    // void writeKdValue(float Kd1);
    void writeKdValue(float Kd1, char KdKey[5]);
    float ReadKdValue(char KdKey[5]);

    // Function to store Generic integer Number

    void iWriteNVSIntNum(int genericNumber, char UniqueKey[20]);

    int iReadNVSIntNum(char UniqueKey[20]);

    void writeSampleDataNVS();
    void readSampleDataNVS();

    /**
     * @brief Set the pid parameters on the nvs flash
     *
     * @param paramaters parametes to be saved
     */
    void vSetPIDParametersToNvs(struct_PID_parameters_t* paramaters);

    /**
     * @brief Get the pid parameters from the nvs flash
     * @param paramaters parametes to be saved
     **/
    void vGetPIDParametersFromNvs(struct_PID_parameters_t* paramaters);

    

    /**********************
     *    ERROR ASSERT
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MIDDLEWARETEST_H*/
