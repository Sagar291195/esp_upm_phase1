/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "esp_upm.h"
#include "buzzer.h"

 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define BUZZER      15
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
 static void buzzer_init_gpio(void);

 /********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *  
 ********************************************************************************************/
static void buzzer_init_gpio(void)
{
    ledc_channel_config_t ledc_channel_left = {0};
    ledc_channel_left.gpio_num = BUZZER;
    ledc_channel_left.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel_left.channel = LEDC_CHANNEL_1;
    ledc_channel_left.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_left.timer_sel = LEDC_TIMER_1;
    ledc_channel_left.duty = 0;

    ledc_timer_config_t ledc_timer = {0};
    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.timer_num = LEDC_TIMER_1;
    ledc_timer.freq_hz = 1000;

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_left));
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *  
 ********************************************************************************************/
void buzzer_initialization(void)
{
    static bool init = false;

    if(init == false)
    {
        init = true;
        gpio_pad_select_gpio(BUZZER);
        gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);
        gpio_set_level(BUZZER, 0);
        buzzer_init_gpio();
    }

}


/********************************************************************************************
 *  
 ********************************************************************************************/
void readyModeBuzzBeep(void)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
    vTaskDelay(300);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void workInProgressBuzzBeep(void)
{
    for (int i = 0; i < 6; i++)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
    }
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void jobFinishedModeBuzzBeep(void)
{
    for (int i = 0; i < 4; i++)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(1000);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
    }
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void waitModeBuzzBeep(void)
{
    for (int i = 0; i < 3; i++)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(400);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
    }
}
