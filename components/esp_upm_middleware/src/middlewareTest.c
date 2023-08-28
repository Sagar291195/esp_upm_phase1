/**
 * @file middlewareTest.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief
 * @version 1.1
 * @date 2022-05-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <motor.h>

/**********************************************defines*********************************************/

#define NVS_STORGE_NAME "storage"

#define STORAGE_KEY "PID_parameters"

#define TAG "NVS"

#include "middlewareTest.h"
#include "stdio.h"
//#include "C:/esp/esp-idf/components/nvs_flash/include/nvs.h"

float KpVal;
float KiVal;
float KdVal;

char StartByReason[25] = "Sample";
char EndByReason[25] = "Time Finished";

char StartPerson[25] = "Steve";
char StopPerson[25] = "Steve";

char ProblemType[25] = "None";

char TLiterKey[5] = "tlv";

void test(void)
{
  printf("middleware function test\n");
  fflush(NULL);
}

typedef struct cat_struct
{
  char name[30];
  int age;
  int id;
} Cat;

typedef struct nSeq_struct
{
  char seqDate[60];
  char seqTime[130];
  char seqSetPt[32];
  char seqDurHr[32];
  char seqDurMin[32];
  char seqNum[5];
  int seqStatus; // if 1, seq is done , if 0 then i has to be executed
} nSequence;

typedef struct nGenericNumInt
{
  int iGenericNum;
} nGenericNumInt;

typedef struct nGenericNumfloat
{
  float fGenericNum;
} nGenericNumfloat;

typedef struct nTotalLitVal
{
  float t_Liters;
} TLiters;

typedef struct nSampleNum
{
  int sampleNum;
} nSampleNumber;

typedef struct nTotalHourVal
{
  float TotalHr;
} nTotalVal;

typedef struct nPIDKpVal
{
  float KpVal1;
} nPIDKp;

typedef struct nPIDKiVal
{
  float KiVal1;
} nPIDKi;

typedef struct nPIDKdVal
{
  float KdVal1;
} nPIDKd;

//  Add one typedef seqDelete

void nvscheckfun1(void)
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));

  nvs_handle handle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "cat_store", NVS_READWRITE, &handle));

  char catKey[50];
  Cat cat;
  size_t catSize = sizeof(Cat);
  for (int i = 0; i < 5; i++)
  {
    sprintf(catKey, "cat_%d", i);
    esp_err_t result = nvs_get_blob(handle, catKey, (void *)&cat, &catSize);
    // printf("actual cat size=%d returned from NVS =%d\n", sizeof(Cat), catSize);

    switch (result)
    {
    case ESP_ERR_NOT_FOUND:
    case ESP_ERR_NVS_NOT_FOUND:
      ESP_LOGE(TAG, "Value not set yet");
      break;
    case ESP_OK:
      ESP_LOGI(TAG, "cat name: %s, age %d, id %d", cat.name, cat.age, cat.id);
      break;
    default:
      ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(result));
      break;
    }
  }
  for (int i = 0; i < 5; i++)
  {
    sprintf(catKey, "cat_%d", i);
    Cat newCat;
    sprintf(newCat.name, "Mr cat %d", i);
    newCat.age = i + 2;
    newCat.id = i;
    ESP_ERROR_CHECK(nvs_set_blob(handle, catKey, (void *)&newCat, sizeof(Cat)));
    ESP_ERROR_CHECK(nvs_commit(handle));
  }

  nvs_close(handle);
}

void seqWrite(char WseqDate[60], char WseqTime[130], char WseqSetPt[32], char WseqDurHr[32], char WseqDurMin[32], char WseqNum[5], int WseqStatus)
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));

  nvs_handle handle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "Sequence_store", NVS_READWRITE, &handle));

  // char SeqKey[5] = "1";

  nSequence newSeq;
  // size_t SequenceSize = sizeof(nSequence);

  sprintf(newSeq.seqDate, "%s", WseqDate);
  sprintf(newSeq.seqTime, "%s", WseqTime);
  sprintf(newSeq.seqSetPt, "%s", WseqSetPt);
  sprintf(newSeq.seqDurHr, "%s", WseqDurHr);
  sprintf(newSeq.seqDurMin, "%s", WseqDurMin);
  sprintf(newSeq.seqNum, "%s", WseqNum);
  newSeq.seqStatus = WseqStatus;

  ESP_ERROR_CHECK(nvs_set_blob(handle, newSeq.seqNum, (void *)&newSeq, sizeof(nSequence)));
  ESP_ERROR_CHECK(nvs_commit(handle));
}

// char  TLiterKey[5] = "tlv";

void write_TotalLiter(float Total_LiterInt)
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle wtlhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "TLiter_store", NVS_READWRITE, &wtlhandle));

  TLiters TLiters1;
  TLiters1.t_Liters = Total_LiterInt;
  ESP_ERROR_CHECK(nvs_set_blob(wtlhandle, TLiterKey, (void *)&TLiters1, sizeof(TLiters)));
  ESP_ERROR_CHECK(nvs_commit(wtlhandle));

  nvs_close(wtlhandle);
}

char SNKey[5] = "samN";

void writeSampleNum(int SamNum)
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle smhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "sampleNum_store", NVS_READWRITE, &smhandle));

  nSampleNumber nSampleNumber; // SampleNumber
  nSampleNumber.sampleNum = SamNum;
  ESP_ERROR_CHECK(nvs_set_blob(smhandle, SNKey, (void *)&nSampleNumber, sizeof(nSampleNumber)));
  ESP_ERROR_CHECK(nvs_commit(smhandle));

  nvs_close(smhandle);
}

int read_SampleNum(char SampleNumKey[5])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle smlhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "sampleNum_store", NVS_READWRITE, &smlhandle));
  nSampleNumber nSampleNumber; // SampleNumber
  size_t SampleNumberSize = sizeof(nSampleNumber);
  esp_err_t sampleNumResult = nvs_get_blob(smlhandle, SampleNumKey, (void *)&nSampleNumber, &SampleNumberSize);

  switch (sampleNumResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Sample Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Sample Number from NVS: %d", nSampleNumber.sampleNum);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(sampleNumResult));
    break;
  }
  nvs_close(smlhandle);
  return nSampleNumber.sampleNum;
}

float read_TotalLiter(char TLiterKey[5])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle wtlhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "TLiter_store", NVS_READWRITE, &wtlhandle));
  TLiters TLiters;
  size_t TLitersSize = sizeof(TLiters);
  esp_err_t TLresult = nvs_get_blob(wtlhandle, TLiterKey, (void *)&TLiters, &TLitersSize);

  switch (TLresult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Total Lit Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Total Liters from NVS: %f", TLiters.t_Liters);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(TLresult));
    break;
  }
  nvs_close(wtlhandle);
  return TLiters.t_Liters;
}

// char SeqKey1[5] = "1";  // Use this for testing

void seqRead(char SeqKey[5])
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));

  nvs_handle handle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "Sequence_store", NVS_READWRITE, &handle));

  nSequence newSeq;
  size_t SequenceSize = sizeof(nSequence);
  esp_err_t result = nvs_get_blob(handle, SeqKey, (void *)&newSeq, &SequenceSize);

  switch (result)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "date from NVS: %s", newSeq.seqDate);
    ESP_LOGI(TAG, "Time from NVS: %s", newSeq.seqTime);
    ESP_LOGI(TAG, "Set Point from NVS: %s", newSeq.seqSetPt);
    ESP_LOGI(TAG, "Seq Dur Hr from NVS: %s", newSeq.seqDurHr);
    ESP_LOGI(TAG, "Seq dur Min from NVS: %s", newSeq.seqDurMin);
    ESP_LOGI(TAG, "seqNum from NVS: %s", newSeq.seqNum);
    ESP_LOGI(TAG, "seqStatus from NVS: %d", newSeq.seqStatus);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(result));
    break;
  }
}

char HrCountKey[5] = "HrCnt";

void writeTotalhour(float HrValue)
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle hchandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "HourCount_store", NVS_READWRITE, &hchandle));

  nTotalVal nTotalVal; // SampleNumber
  nTotalVal.TotalHr = HrValue;
  ESP_ERROR_CHECK(nvs_set_blob(hchandle, HrCountKey, (void *)&nTotalVal, sizeof(nTotalVal)));
  ESP_ERROR_CHECK(nvs_commit(hchandle));

  nvs_close(hchandle);
}

/**********************Write a function to to store Sample structure in NVS ***************************/

void writeSampleDataNVS()
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle wrtSamHandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "SampleDataInfo", NVS_READWRITE, &wrtSamHandle));

  // nTotalVal nTotalVal; // SampleNumber
  // nTotalVal.TotalHr = HrValue;

  storeSample_t sampleStore;

  sprintf(sampleStore.startDateStruct, "%s", startDateEnd);
  sprintf(sampleStore.startTimeStruct, "%s", startTimeEnd);
  sprintf(sampleStore.stopDateStruct, "%s", stopDateEnd);
  sprintf(sampleStore.stopTimeStruct, "%s", stopTimeEnd);
  sampleStore.sampleNumberStruct = sampleNumber;
  sprintf(sampleStore.FlowSetPtStruct, "%s", _xsSProllerbuf);
  sprintf(sampleStore.SeqNumberStruct, "%s", WseqNum1);
  sprintf(sampleStore.durationHrStruct, "%s", _xsDurHourrollerbuf);
  sprintf(sampleStore.durationMinStruct, "%s", _xsDurMinutrollerbuf);
  sprintf(sampleStore.startByStruct, "%s", StartByReason);
  sprintf(sampleStore.endByStruct, "%s", EndByReason);
  sprintf(sampleStore.startPersonStruct, "%s", StartPerson);
  sprintf(sampleStore.stopPersonStruct, "%s", StopPerson);
  sprintf(sampleStore.problemStruct, "%s", ProblemType);

  sampleStore.startLiterStruct = StartLTRCountVal;
  sampleStore.stopLiterStruct = StopLTRCountVal;
  sampleStore.literTargetStruct = targetLiters;
  sampleStore.literEffecStruct = effectiveLiters;
  sampleStore.literVarStruct = variationLiters;
  sampleStore.startHourStruct = StartHrEND;
  sampleStore.stopHourStruct = StopHrEND;
  sampleStore.hourTargetStruct = targetHr;
  sampleStore.hourEffecStruct = effectiveHr;
  sampleStore.hourVarStruct = variationHr;

  ESP_ERROR_CHECK(nvs_set_blob(wrtSamHandle, "SampleData", (void *)&sampleStore, sizeof(sampleStore)));
  ESP_ERROR_CHECK(nvs_commit(wrtSamHandle));

  nvs_close(wrtSamHandle);
}

void readSampleDataNVS()
{
}

/******************************************************************************************************/

float read_Totalhour(char HrKey[5])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle hchandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "HourCount_store", NVS_READWRITE, &hchandle));
  nTotalVal nTotalVal; // SampleNumber
  size_t nTotalValSize = sizeof(nTotalVal);
  esp_err_t HourCntResult = nvs_get_blob(hchandle, HrKey, (void *)&nTotalVal, &nTotalValSize);

  switch (HourCntResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Sample Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Total Hour count Val from NVS: %f", nTotalVal.TotalHr);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(HourCntResult));
    break;
  }
  nvs_close(hchandle);
  return nTotalVal.TotalHr;
}

// char KpKey[5] = "Kp";

void writeKpValue(float Kp1, char KpKey[6])
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));

  nvs_handle Kphandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "KpValue_Store", NVS_READWRITE, &Kphandle));

  nPIDKp nPIDKp;
  nPIDKp.KpVal1 = Kp1;

  ESP_ERROR_CHECK(nvs_set_blob(Kphandle, KpKey, (void *)&nPIDKp, sizeof(nPIDKp)));
  ESP_ERROR_CHECK(nvs_commit(Kphandle));

  nvs_close(Kphandle);
}

float ReadKpValue(char KpKey[6])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle Kphandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "KpValue_Store", NVS_READWRITE, &Kphandle));

  nPIDKp nPIDKp;
  size_t nPIDKpSize = sizeof(nPIDKp);
  esp_err_t KpValResult = nvs_get_blob(Kphandle, KpKey, (void *)&nPIDKp, &nPIDKpSize);

  switch (KpValResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Sample Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Total Hour count Val from NVS: %f", nPIDKp.KpVal1);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(KpValResult));
    break;
  }
  nvs_close(Kphandle);
  return nPIDKp.KpVal1;
}

// char KiKey[5] = "Ki";

void writeKiValue(float Ki1, char KiKey[5])
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));

  nvs_handle Kihandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "KiValue_Store", NVS_READWRITE, &Kihandle));

  nPIDKi nPIDKi;
  nPIDKi.KiVal1 = Ki1;

  ESP_ERROR_CHECK(nvs_set_blob(Kihandle, KiKey, (void *)&nPIDKi, sizeof(nPIDKi)));
  ESP_ERROR_CHECK(nvs_commit(Kihandle));

  nvs_close(Kihandle);
}

float ReadKiValue(char KiKey[5])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle Kihandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "KiValue_Store", NVS_READWRITE, &Kihandle));

  nPIDKi nPIDKi;
  size_t nPIDKiSize = sizeof(nPIDKi);
  esp_err_t KiValResult = nvs_get_blob(Kihandle, KiKey, (void *)&nPIDKi, &nPIDKiSize);

  switch (KiValResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Sample Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Total Hour count Val from NVS: %f", nPIDKi.KiVal1);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(KiValResult));
    break;
  }
  nvs_close(Kihandle);
  return nPIDKi.KiVal1;
}

char KdKey[5] = "Kd";

void writeKdValue(float Kd1, char KdKey[5])
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));

  nvs_handle Kdhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "KdValue_Store", NVS_READWRITE, &Kdhandle));

  nPIDKd nPIDKd;
  nPIDKd.KdVal1 = Kd1;

  ESP_ERROR_CHECK(nvs_set_blob(Kdhandle, KdKey, (void *)&nPIDKd, sizeof(nPIDKd)));
  ESP_ERROR_CHECK(nvs_commit(Kdhandle));

  nvs_close(Kdhandle);
}

float ReadKdValue(char KdKey[5])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle Kdhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "KdValue_Store", NVS_READWRITE, &Kdhandle));

  nPIDKd nPIDKd;
  size_t nPIDKdSize = sizeof(nPIDKd);
  esp_err_t KdValResult = nvs_get_blob(Kdhandle, KdKey, (void *)&nPIDKd, &nPIDKdSize);

  switch (KdValResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Sample Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Total Hour count Val from NVS: %f", nPIDKd.KdVal1);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(KdValResult));
    break;
  }
  nvs_close(Kdhandle);
  return nPIDKd.KdVal1;
}

void iWriteNVSIntNum(int genericNumber, char UniqueKey[20])
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle genNumhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "GenNum_store", NVS_READWRITE, &genNumhandle));

  nGenericNumInt nGenericNumInt;
  nGenericNumInt.iGenericNum = genericNumber;

  ESP_ERROR_CHECK(nvs_set_blob(genNumhandle, UniqueKey, (void *)&nGenericNumInt, sizeof(nGenericNumInt)));
  ESP_ERROR_CHECK(nvs_commit(genNumhandle));

  nvs_close(genNumhandle);
}

int iReadNVSIntNum(char UniqueKey[20])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle genNumhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "GenNum_store", NVS_READWRITE, &genNumhandle));
  nGenericNumInt nGenericNumInt;
  size_t nGenericNumSize = sizeof(nGenericNumInt);
  esp_err_t iGenericNumResult = nvs_get_blob(genNumhandle, UniqueKey, (void *)&nGenericNumInt, &nGenericNumSize);

  switch (iGenericNumResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Sample Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Sample Number from NVS: %d", nGenericNumInt.iGenericNum);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(iGenericNumResult));
    break;
  }
  nvs_close(genNumhandle);
  return nGenericNumInt.iGenericNum;
}

/**
 * Write a Float number in NVS
 * @param genericNumber Float Number to store in NVS
 * @param UniqueKey   Give a unique Key Store the float number
 */

void iWriteNVSFloatNum(float genericNumber, char UniqueKey[20])
{
  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle genfloatNumhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "GenFloatN_store", NVS_READWRITE, &genfloatNumhandle));

  nGenericNumfloat nGenericNumfloat;
  nGenericNumfloat.fGenericNum = genericNumber;

  ESP_ERROR_CHECK(nvs_set_blob(genfloatNumhandle, UniqueKey, (void *)&nGenericNumfloat, sizeof(nGenericNumfloat)));
  ESP_ERROR_CHECK(nvs_commit(genfloatNumhandle));

  nvs_close(genfloatNumhandle);
}
/**
 * @brief this function reads the float value stored in the nvs flash
 *
 * @param UniqueKey name of the value to be read
 * @return float stroed value
 */
float iReadNVSFloatNum(char UniqueKey[20])
{

  vTaskDelay(10 / portTICK_PERIOD_MS);

  ESP_ERROR_CHECK(nvs_flash_init_partition("nvs"));
  nvs_handle genfloatNumhandle;
  ESP_ERROR_CHECK(nvs_open_from_partition("nvs", "GenFloatN_store", NVS_READWRITE, &genfloatNumhandle));
  nGenericNumfloat nGenericNumfloat;
  size_t nGenericNumfloatSize = sizeof(nGenericNumfloat);
  esp_err_t iGenericNumResult = nvs_get_blob(genfloatNumhandle, UniqueKey, (void *)&nGenericNumfloat, &nGenericNumfloatSize);

  switch (iGenericNumResult)
  {
  case ESP_ERR_NOT_FOUND:
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGE(TAG, "Num Value not set yet");
    break;
  case ESP_OK:
    ESP_LOGI(TAG, "Number from NVS: %f", nGenericNumfloat.fGenericNum);
    break;
  default:
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(iGenericNumResult));
    break;
  }
  nvs_close(genfloatNumhandle);
  return nGenericNumfloat.fGenericNum;
}

void vSetPIDParametersToNvs(struct_PID_parameters_t *paramaters)
{

  nvs_handle_t my_handle;
  esp_err_t err;

  // Open
  err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    return;
  }
  /**
   * @brief setting the value to the nvs flash
   *
   */
  err = nvs_set_blob(my_handle, STORAGE_KEY, (void *)paramaters, sizeof(struct_PID_parameters_t));
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
    return;
  }

  // Commit
  err = nvs_commit(my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
  }
  // Close
  nvs_close(my_handle);
}

void vGetPIDParametersFromNvs(struct_PID_parameters_t *paramaters)
{
  nvs_handle_t my_handle;
  esp_err_t err;

  // Open
  err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
   
    return;
  }

  // Read run time blob
  size_t required_size = 0; // value will default to 0, if not set yet in NVS
                            // obtain required memory space to store blob being read from NVS

  err = nvs_get_blob(my_handle, STORAGE_KEY, NULL, &required_size);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) getting blob NVS handle!\n", esp_err_to_name(err));
     /**
     * @brief setting the default value of pid parameters, if storage is not set yet
     *
     */
    ESP_LOGW(TAG, "no value in the flash, loading the default values");
    paramaters->fKp = motorPID_DEFAULT_KP;
    paramaters->fKi = motorPID_DEFAULT_KI;
    paramaters->fKd = motorPID_DEFAULT_KD;
    paramaters->fAkp = motorPID_DEFAULT_AKP;
    paramaters->fAki = motorPID_DEFAULT_AKI;
    paramaters->fAkd = motorPID_DEFAULT_AKD;
    paramaters->fACoff = 1;
    paramaters->fNcoff = 1;
    return;
  }
  if (required_size == 0)
  {
    /**
     * @brief no value has been set in the nvs flash loading the default values
     *
     */
    ESP_LOGW(TAG, "no value in the flash, loading the default values");
    paramaters->fKp = motorPID_DEFAULT_KP;
    paramaters->fKi = motorPID_DEFAULT_KI;
    paramaters->fKd = motorPID_DEFAULT_KD;
    paramaters->fAkp = motorPID_DEFAULT_AKP;
    paramaters->fAki = motorPID_DEFAULT_AKI;
    paramaters->fAkd = motorPID_DEFAULT_AKD;
    paramaters->fACoff = 1;
    paramaters->fNcoff = 1;
    return;
  }

  // Read blob
  err = nvs_get_blob(my_handle, STORAGE_KEY, (void *)paramaters, &required_size);
  // Close
  nvs_close(my_handle);
}
