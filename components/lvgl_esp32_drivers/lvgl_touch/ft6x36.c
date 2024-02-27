/*
* Copyright © 2020 Wolfgang Christl
* Permission is hereby granted, free of charge, to any person obtaining a copy of this 
* software and associated documentation files (the “Software”), to deal in the Software 
* without restriction, including without limitation the rights to use, copy, modify, merge, 
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons 
* to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or 
* substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
* PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
* SOFTWARE.
*/

/*
* Add I2C Proto 
*
*
*/

#include <esp_log.h>
#include <driver/i2c.h>
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include <lvgl.h>
#else
#include <lvgl/lvgl.h>
#endif
#include "ft6x36.h"
#include "tp_i2c.h"
#include "../lvgl_i2c_conf.h"

#define TAG "FT6X36"

extern SemaphoreHandle_t i2c_communication_semaphore;

#define lm2759_I2C_ADDRESS 0x53
#define lm2759_REG_GENERAL 0x10 /* bits: 3-0 */
#define lm2759_REG_T_CURRENT 0xA0

#define I2C_FREQ_HZ 100000 // Max 1MHz for esp-idf
#define SDA_GPIO    21
#define SCL_GPIO    22

#define CHECK_ARG(ARG)                  \
    do                                  \
    {                                   \
        if (!ARG)                       \
            return ESP_ERR_INVALID_ARG; \
    } while (0)
    
#define CHECK(x)                \
    do                          \
    {                           \
        esp_err_t __;           \
        if ((__ = x) != ESP_OK) \
            return __;          \
    } while (0)


#define CHECK_LOGE(dev, x, msg, ...) do { \
        esp_err_t __; \
        if ((__ = x) != ESP_OK) { \
            I2C_DEV_GIVE_MUTEX(&dev->i2c_dev); \
            ESP_LOGE(TAG, msg, ## __VA_ARGS__); \
            return __; \
        } \
    } while (0)

ft6x36_status_t ft6x36_status;
ft6x36_t ft6x36_dev;//

uint8_t current_dev_addr;       // set during init
static uint32_t touch_count = 0;

esp_err_t read_reg_8(ft6x36_t *dev, uint8_t reg, uint8_t *val)
{
    CHECK_ARG(val);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    I2C_DEV_CHECK(&dev->i2c_dev, i2c_dev_read_reg(&dev->i2c_dev, reg, val, 1));
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}
esp_err_t read_reg_16(ft6x36_t *dev, uint8_t reg, uint16_t *val)
{
    CHECK_ARG(val);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    I2C_DEV_CHECK(&dev->i2c_dev, i2c_dev_read_reg(&dev->i2c_dev, reg, val, 2));
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}

esp_err_t ft6x06_i2c_read8(uint8_t slave_addr, uint8_t register_addr, uint8_t *data_buf) {
    // Sync config register
     return  read_reg_8(&ft6x36_dev, register_addr, data_buf);    
}

/**
  * @brief  Read the FT6x36 gesture ID. Initialize first!
  * @param  dev_addr: I2C FT6x36 Slave address.
  * @retval The gesture ID or 0x00 in case of failure
  */
uint8_t ft6x36_get_gesture_id() {
    if (!ft6x36_status.inited) {
        ESP_LOGE(TAG, "Init first!");
        return 0x00;
    }
    uint8_t data_buf;
    esp_err_t ret;
    if ((ret = ft6x06_i2c_read8(current_dev_addr, FT6X36_GEST_ID_REG, &data_buf) != ESP_OK))
        ESP_LOGE(TAG, "Error reading from device: %s", esp_err_to_name(ret));
    return data_buf;
}

/**
  * @brief  Initialize for FT6x36 communication via I2C
  * @param  dev_addr: Device address on communication Bus (I2C slave address of FT6X36).
  * @retval None
  */
void ft6x06_init(uint16_t dev_addr) {

    lcd_led_driver_init();
    vTaskDelay(100 / portTICK_RATE_MS);
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY)) 
    {
        ESP_LOGI(TAG, "ft6x06_init_start");
    
        if (!ft6x36_status.inited) {
            esp_err_t code = ESP_OK;
            if (code != ESP_OK) {
                ft6x36_status.inited = false;
                ESP_LOGE(TAG, "Error during I2C init %s", esp_err_to_name(code));
            } else {
                ft6x36_status.inited = true;
                current_dev_addr = dev_addr;
                
                memset(&ft6x36_dev, 0, sizeof(ft6x36_t));//
                ESP_ERROR_CHECK(ft6x36_init_desc(&ft6x36_dev, dev_addr, 0, SDA_GPIO, SCL_GPIO));   // registration touch in i2cdev 

                uint8_t data_buf;
                esp_err_t ret;
                ESP_LOGI(TAG, "Found touch panel controller");
                if ((ret = ft6x06_i2c_read8(dev_addr, FT6X36_PANEL_ID_REG, &data_buf) != ESP_OK))
                    ESP_LOGE(TAG, "Error reading from device: %s",
                            esp_err_to_name(ret));    // Only show error the first time
                ESP_LOGI(TAG, "Device ID: 0x%02x", data_buf);

                ft6x06_i2c_read8(dev_addr, FT6X36_CHIPSELECT_REG, &data_buf);
                ESP_LOGI(TAG, "Chip ID: 0x%02x", data_buf);

                ft6x06_i2c_read8(dev_addr, FT6X36_DEV_MODE_REG, &data_buf);
                ESP_LOGI(TAG, "Device mode: 0x%02x", data_buf);

                ft6x06_i2c_read8(dev_addr, FT6X36_FIRMWARE_ID_REG, &data_buf);
                ESP_LOGI(TAG, "Firmware ID: 0x%02x", data_buf);

                ft6x06_i2c_read8(dev_addr, FT6X36_RELEASECODE_REG, &data_buf);
                ESP_LOGI(TAG, "Release code: 0x%02x", data_buf);
            }
        }
        ESP_LOGI(TAG, "ft6x06_init_finish");
        xSemaphoreGive(i2c_communication_semaphore);
    }
}

/**
  * @brief  Get the touch screen X and Y positions values. Ignores multi touch
  * @param  drv:
  * @param  data: Store data here
  * @retval Always false
  */
bool ft6x36_read(lv_indev_drv_t *drv, lv_indev_data_t *data) 
{
    uint8_t data_xy[4];        // 2 bytes X | 2 bytes Y
    uint8_t touch_pnt_cnt;        // Number of detected touch points
    static int16_t last_x = 0;  // 12bit pixel value
    static int16_t last_y = 0;  // 12bit pixel value
    esp_err_t ret;

    ft6x06_i2c_read8(current_dev_addr, FT6X36_TD_STAT_REG, &touch_pnt_cnt);
    if( touch_pnt_cnt > 0)
    {
        if( get_lcdsleep_status() == true)
        {
            lcd_set_wakeup();
        }
        else{
            touch_count++;
        }
    }

    if (touch_pnt_cnt != 1) {    // ignore no touch & multi touch
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
        return false;
    }  

    int16_t rawX;
    ret=read_reg_16(&ft6x36_dev,FT6X36_P1_XH_REG, (uint16_t *)&rawX);
    data_xy[1]=(int8_t)((rawX>>8)& 0xff);
    data_xy[0]=(int8_t)(rawX&0xff);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error getting X coordinates: %s", esp_err_to_name(ret));
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;   // no touch detected
        return false;
    }

    int16_t rawY;
    ret=read_reg_16(&ft6x36_dev,FT6X36_P1_YH_REG, (uint16_t *)&rawY);
    data_xy[3]=(int8_t)((rawY>>8)& 0xff);
    data_xy[2]=(int8_t)(rawY&0xff);
    if (ret != ESP_OK) {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;   // no touch detected
        return false;
    }

    last_x = ((data_xy[0] & FT6X36_MSB_MASK) << 8) | (data_xy[1] & FT6X36_LSB_MASK);
    last_y = ((data_xy[2] & FT6X36_MSB_MASK) << 8) | (data_xy[3] & FT6X36_LSB_MASK);

#if CONFIG_LV_FT6X36_SWAPXY
    int16_t swap_buf = last_x;
    last_x = last_y;
    last_y = swap_buf;
#endif
#if CONFIG_LV_FT6X36_INVERT_X
    last_x =  LV_HOR_RES - last_x;
#endif
#if CONFIG_LV_FT6X36_INVERT_Y
    last_y = LV_VER_RES - last_y;
#endif
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = LV_INDEV_STATE_PR;

    return false;
}

uint32_t get_touchcount(void)
{
    return touch_count;
}

void reset_touchcount(void)
{
    touch_count = 0;
}


//=======================new Function
esp_err_t ft6x36_init_desc(ft6x36_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
   // registration touch
    ESP_LOGV(TAG, "ft6x36_init_desc");
    CHECK_ARG(dev);
    dev->i2c_dev.port = port;
    dev->i2c_dev.addr = addr;
    dev->i2c_dev.cfg.sda_io_num = sda_gpio;
    dev->i2c_dev.cfg.scl_io_num = scl_gpio;
    dev->i2c_dev.cfg.master.clk_speed = I2C_FREQ_HZ;
    return i2c_dev_create_mutex(&dev->i2c_dev);
}



