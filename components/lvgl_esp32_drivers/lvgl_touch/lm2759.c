#include <esp_log.h>
#include "lm2759.h"

#define I2C_FREQ_HZ 100000 // Max 1MHz for esp-idf
#define SDA_GPIO    21
#define SCL_GPIO    22

static const char *TAG = "lm2759";

extern SemaphoreHandle_t main_mutex;

/**
 * lm2759 registers
 */
#define lm2759_REG_GENERAL          0x10     /* bits: 3-0 */
#define lm2759_REG_T_CURRENT        0xA0

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

inline static esp_err_t write_register8(i2c_dev_t *dev, uint8_t addr, uint8_t value)
{
    return i2c_dev_write_reg(dev, addr, &value, 1);
}


esp_err_t lm2759_init_desc(lm2759_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    CHECK_ARG(dev);

    if (addr != lm2759_I2C_ADDRESS)
    {
        ESP_LOGE(TAG, "Invalid I2C address");
        return ESP_ERR_INVALID_ARG;
    }

    dev->i2c_dev.port = port;
    dev->i2c_dev.addr = addr;
    dev->i2c_dev.cfg.sda_io_num = sda_gpio;
    dev->i2c_dev.cfg.scl_io_num = scl_gpio;
    dev->i2c_dev.cfg.master.clk_speed = I2C_FREQ_HZ;

    return i2c_dev_create_mutex(&dev->i2c_dev);
}

esp_err_t lm2759_set_mode(lm2759_t *dev)
{
    CHECK_ARG(dev);
    // Set torch mode
    CHECK_LOGE(dev, write_register8(&dev->i2c_dev, lm2759_REG_GENERAL, 0x01), "Failed to init LM2759");
    return ESP_OK;
}

esp_err_t lm2759_set_current(lm2759_t *dev, uint8_t current)
{
    //i2cdev_init();
    CHECK_ARG(dev);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);

    // Set torch mode
    CHECK_LOGE(dev, write_register8(&dev->i2c_dev, lm2759_REG_T_CURRENT, current), "Failed to set current");

    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);

    return ESP_OK;
}

void lcd_led_driver_init(void)
{
    ESP_LOGI(TAG, "Init LCD driver current");
    lm2759_t dev;
    memset(&dev, 0, sizeof(lm2759_t));
    
    ESP_ERROR_CHECK(lm2759_init_desc(&dev, lm2759_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO));    
    ESP_ERROR_CHECK(lm2759_set_mode(&dev));    
    ESP_ERROR_CHECK(lm2759_set_current(&dev, 0x05));  
   
    ESP_LOGI(TAG, "Init LCD driver finish");
}

void lcd_set_current(uint8_t current)
{
    if(xSemaphoreTake(main_mutex, portMAX_DELAY)){

    if (current >= 0x08) return;    
    lm2759_t dev;
    memset(&dev, 0, sizeof(lm2759_t));

    ESP_ERROR_CHECK(lm2759_init_desc(&dev, lm2759_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO));
    ESP_ERROR_CHECK(lm2759_set_current(&dev, current));

    xSemaphoreGive(main_mutex);}
}