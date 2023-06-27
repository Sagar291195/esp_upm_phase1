/**
 * @file wifi.h
 */

#ifndef WIFI_H
#define WIFI_H

#include "esp_event.h"
#include "freertos/FreeRTOS.h"

#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASS CONFIG_WIFI_PASSWORD

void initialise_wifi(const char *running_partition_label);

#endif /* WIFI_H */
