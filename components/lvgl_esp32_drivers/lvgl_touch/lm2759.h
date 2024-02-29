/**
 * @file lm2759.h
 * @defgroup lm2759 lm2759
 * @{
 *
 * ESP-IDF driver for lm2759/BME280 digital pressure sensor
 *
 * Ported from esp-open-rtos
 *
 * Copyright (C) 2016 sheinz <https://github.com/sheinz>\n
 * Copyright (C) 2018 Ruslan V. Uss <https://github.com/UncleRus>
 *
 * MIT Licensed as described in the file LICENSE
 */
#ifndef __lm2759_H__
#define __lm2759_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <esp_err.h>
#include "i2cdev.h"

#ifdef __cplusplus
extern "C" {
#endif

#define lm2759_I2C_ADDRESS   0x53 

/**
 * Device descriptor
 */
typedef struct {
    i2c_dev_t i2c_dev;  //!< I2C device descriptor    
} lm2759_t;

/**
 * Initialize lm2759 module, probes for the device, soft resets the device,
 * reads the calibration constants, and configures the device using the supplied
 * parameters. Returns `ESP_OK` on success.
 *
 * This may be called again to soft reset the device and initialize it again.
 */
esp_err_t lm2759_set_torchmode(lm2759_t *dev);
esp_err_t lm2759_set_shutdownmode(lm2759_t *dev);

/**
 * @brief Initialize device descriptior
 * @param[out] dev Pointer to device descriptor
 * @param[in] addr lm2759 address
 * @param[in] port I2C port number
 * @param[in] sda_gpio GPIO pin for SDA
 * @param[in] scl_gpio GPIO pin for SCL
 * @return `ESP_OK` on success
 */
esp_err_t lm2759_init_desc(lm2759_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);

void lcd_led_driver_init(void);

void lcd_set_sleep(void);

void lcd_set_wakeup(void);

bool get_lcdsleep_status(void);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif  // __lm2759_H__
