/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <esp_log.h>
#include "sdp32.h"

 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG             "SDP32"
#define I2C_FREQ_HZ     800000 // Max 1MHz for esp-idf
#define CHECK(x)        do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL)  do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)
#define CHECK_LOGE(dev, x, msg, ...) do { \
        esp_err_t __; \
        if ((__ = x) != ESP_OK) { \
            I2C_DEV_GIVE_MUTEX(&dev->i2c_dev); \
            ESP_LOGE(TAG, msg, ## __VA_ARGS__); \
            return __; \
        } \
    } while (0)


/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                
 ********************************************************************************************/
esp_err_t sdp32_init_desc(sdp32_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    CHECK_ARG(dev);

    dev->i2c_dev.port = port;
    dev->i2c_dev.addr = addr;
    dev->i2c_dev.cfg.sda_io_num = sda_gpio;
    dev->i2c_dev.cfg.scl_io_num = scl_gpio;
    dev->i2c_dev.cfg.master.clk_speed = I2C_FREQ_HZ;

    return i2c_dev_create_mutex(&dev->i2c_dev);
}

int sdp32_send_cmd_read(sdp32_t *dev, uint8_t *cmd)
{
    int return_value = 0;

    CHECK_ARG(dev);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);

    // Set command
    // CHECK_LOGE(dev, i2c_dev_write(&dev->i2c_dev, NULL, 0, cmd, 2), "Failed to send cmd read"); // 2->3
    esp_err_t ret = i2c_dev_write(&dev->i2c_dev, NULL, 0, cmd, 2);
    if(ret != ESP_OK)
        return_value = 1;

    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);   
    return return_value; 
}

/********************************************************************************************
 *                
 ********************************************************************************************/
int sdp32_read_pressure(sdp32_t *dev, uint8_t *read_data)
{
    CHECK_ARG(dev);
    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    // Set command
    CHECK_LOGE(dev, i2c_dev_read(&dev->i2c_dev, NULL, 0, read_data, 9), "Failed to read diff pressure");
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return 0;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
bool CheckCrc(const uint8_t data[], uint8_t size, uint8_t checksum)
{
    uint8_t crc = 0xFF;

    // calculates 8-Bit checksum with given polynomial 0x31 (x^8 + x^5 + x^4 + 1)
    for (int i = 0; i < size; i++)
    {
        crc ^= (data[i]);
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }

    // verify checksum
    return (crc == checksum) ? true : false;
}

