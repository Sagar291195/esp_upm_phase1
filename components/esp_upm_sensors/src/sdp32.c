#include <esp_log.h>
#include "sdp32.h"

//#define I2C_FREQ_HZ 100000 // Max 1MHz for esp-idf
#define I2C_FREQ_HZ 800000 // Max 1MHz for esp-idf

static const char *TAG = "SDP32";

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)
#define CHECK_LOGE(dev, x, msg, ...) do { \
        esp_err_t __; \
        if ((__ = x) != ESP_OK) { \
            I2C_DEV_GIVE_MUTEX(&dev->i2c_dev); \
            ESP_LOGE(TAG, msg, ## __VA_ARGS__); \
            return __; \
        } \
    } while (0)

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

void sdp32_send_cmd_read(sdp32_t *dev, uint8_t *cmd)
{
    CHECK_ARG(dev);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);

    // Set command
    CHECK_LOGE(dev, i2c_dev_write(&dev->i2c_dev, NULL, 0, cmd, 2), "Failed to send cmd read"); // 2->3

    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);    
}

void sdp32_read_pressure(sdp32_t *dev, uint8_t *read_data)
{
    CHECK_ARG(dev);
    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    // Set command
    CHECK_LOGE(dev, i2c_dev_read(&dev->i2c_dev, NULL, 0, read_data, 9), "Failed to read diff pressure");
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
}

/**
 * @brief Construct this function will use the crc to check the value of the data from the sdp32 sensor
 *
 * @param data data from the sdp32 sensor.
 * @param size size of the data. usually its 2
 * @param checksum checksum of the data, which we take from the sdp32 sensor after reading the data.
 * @return true if the crc is correct, false if not.
 */
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

