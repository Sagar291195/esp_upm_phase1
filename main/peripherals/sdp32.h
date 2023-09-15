#ifndef __SDP32_H__
#define __SDP32_H__

#include <stdbool.h>
//#include <i2cdev.h>
#include <esp_err.h>
#include "../lvgl_esp32_drivers/lvgl_touch/i2cdev.h"

#define SDP32_I2C_ADDRESS 0x21

// convert two 8 bit values to one word
#define BIU16(data, start) (((uint16_t)(data)[start]) << 8 | ((data)[start + 1]))

typedef struct
{
    i2c_dev_t i2c_dev;                                  //!< I2C device descriptor    
} sdp32_t;

esp_err_t sdp32_init_desc(sdp32_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);
int sdp32_send_cmd_read(sdp32_t *dev, uint8_t *cmd);
int sdp32_read_pressure(sdp32_t *dev, uint8_t *read_data);

/**
 * @brief Construct this function will use the crc to check the value of the data from the sdp32 sensor
 *
 * @param data data from the sdp32 sensor.
 * @param size size of the data. usually its 2
 * @param checksum checksum of the data, which we take from the sdp32 sensor after reading the data.
 * @return true if the crc is correct, false if not.
 */
bool CheckCrc(const uint8_t data[], uint8_t size, uint8_t checksum);


#endif