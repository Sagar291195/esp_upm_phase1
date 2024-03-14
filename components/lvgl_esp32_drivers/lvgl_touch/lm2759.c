#include <esp_log.h>
#include "lm2759.h"
#include "gui/screens/screen_includes.h"

#define I2C_FREQ_HZ 100000 // Max 1MHz for esp-idf
#define SDA_GPIO    21
#define SCL_GPIO    22

#define LCD_BACKLIGHT            GPIO_NUM_2     /* wakeup GPIO old hardware*/

static const char *TAG = "lm2759";

static bool lm2759_available = false;
static bool lcd_sleepstatus = false;
static lm2759_t dev;

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

esp_err_t lm2759_set_torchmode(lm2759_t *dev)
{
    CHECK_ARG(dev);
    return write_register8(&dev->i2c_dev, lm2759_REG_GENERAL, 0x01);
}

esp_err_t lm2759_set_shutdownmode(lm2759_t *dev)
{
    CHECK_ARG(dev);
    return write_register8(&dev->i2c_dev, lm2759_REG_GENERAL, 0x00);
}

esp_err_t lm2759_set_current(lm2759_t *dev, uint8_t current)
{
    CHECK_ARG(dev);
    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    CHECK_LOGE(dev, write_register8(&dev->i2c_dev, lm2759_REG_T_CURRENT, current), "Failed to set current");
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}

void lcd_led_driver_init(void)
{
    ESP_LOGI(TAG, "Init LCD driver current");
    memset(&dev, 0, sizeof(lm2759_t));
    
    ESP_LOGI( TAG, "initialization of lm27590 driver");
    ESP_ERROR_CHECK(lm2759_init_desc(&dev, lm2759_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO));    
    ESP_LOGI( TAG, "set mode of lm2759");

    if( lm2759_set_torchmode(&dev) == ESP_OK )
    {
        ESP_LOGI( TAG, "lm2759 is available, and setting current for lcd");
        lm2759_available = true;
    }
    else{
        ESP_LOGI(TAG, "turning on lcd backlight gpio");
        lm2759_available = false;
        // gpio_pad_select_gpio(GPIO_NUM_2);                 // Set GPIO as OUTPUT
        // gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT); // WakeMode
        // gpio_set_level(GPIO_NUM_2, 1);

        ledc_channel_config_t ledc_channel_left = {0};
        ledc_channel_left.gpio_num = GPIO_NUM_2;
        ledc_channel_left.speed_mode = LEDC_HIGH_SPEED_MODE;
        ledc_channel_left.channel = LEDC_CHANNEL_3;
        ledc_channel_left.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_left.timer_sel = LEDC_TIMER_2;
        ledc_channel_left.duty = 0;

        ledc_timer_config_t ledc_timer = {0};
        ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
        ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
        ledc_timer.timer_num = LEDC_TIMER_2;
        ledc_timer.freq_hz = 1000;

        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_left));
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
        
        
    }
    lcd_set_contrast(devicesettings.contrast_value);
    ESP_LOGI(TAG, "Init LCD driver finish");
}


void lcd_set_contrast(int dutycycle)
{
    if(lm2759_available == true)
    {
        uint8_t current_value;
        if(dutycycle > 93)
        {
            current_value = 0x0F;
        }
        else if(dutycycle > 86)
        {
            current_value = 0x0E;
        }
        else if(dutycycle > 80)
        {
            current_value = 0x0D;
        }
        else if(dutycycle > 73)
        {
            current_value = 0x0C;
        }
        else if(dutycycle > 66)
        {
            current_value = 0x0B;
        }
        else if(dutycycle > 59)
        {
            current_value = 0x0A;
        }
         else if(dutycycle > 52)
        {
            current_value = 0x09;
        }
        else if(dutycycle > 46)
        {
            current_value = 0x08;
        }
        else if(dutycycle > 40)
        {
            current_value = 0x07;
        }
         else if(dutycycle > 33)
        {
            current_value = 0x06;
        }
        else if(dutycycle > 27)
        {
            current_value = 0x05;
        }
        else if(dutycycle > 22)
        {
            current_value = 0x04;
        }
         else if(dutycycle > 15)
        {
            current_value = 0x03;
        }
        else if(dutycycle > 10)
        {
            current_value = 0x02;
        }
        else  if(dutycycle > 5)
        {
            current_value = 0x01;
        }
        else{
            current_value = 0x00;
        }
        ESP_ERROR_CHECK(lm2759_set_current(&dev, current_value));  
    }
    else
    {
        uint32_t duty_cycle_value = (((pow(2, LEDC_TIMER_10_BIT)) - 1) * dutycycle) / 100;
        ESP_LOGI(TAG, "lcd duty cycle is %d", duty_cycle_value);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, duty_cycle_value));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3));
}
    }
    

void lcd_set_sleep(void)
{
    if( lm2759_available == true )
    {   
        lm2759_set_shutdownmode(&dev);
    }else{
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3));
    }
    lcd_sleepstatus = true;
}


void lcd_set_wakeup(void)
{
    if( lm2759_available == true )
    {
        lm2759_set_torchmode(&dev);
    }else{
        lcd_set_contrast(devicesettings.contrast_value);
    }
    lcd_sleepstatus = false;


    if(screenid != SCR_PASSWORD && screenid != SCR_METROLOGY_PASSWORD && screenid != SCR_PASSWORD_SAMPLE_STOP)
    {
        Screen_Password(SCR_PASSWORD_WAKEUP);
    }
}

bool get_lcdsleep_status(void)
{
    return lcd_sleepstatus;
}