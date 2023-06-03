#ifndef __SCANNERI2C_H__
#define __SCANNERI2C_H__

#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

void scanner_task(void *pvParameters);


#endif