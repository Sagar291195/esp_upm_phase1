#include "esp_upm.h"
#include "lvgl_helpers.h"

#include "gui/generated/gui_guider.h"
#include "gui/custom/custom.h"

static test_state_t devicestate = STATE_IDLE;

void sensorReadTask(void *pvParameter)
{
    while(1)
    {
        switch(devicestate)
        {
            case STATE_IDLE:
                break;

            case STATE_START_TEST:
                break;

            case STATE_BME680_TEST:
                break;

            case STATE_BME280_TEST:
                break;

            case STATE_SDP32_TEST:
                break;

            case STATE_LED_TEST:
                break;               

            case STATE_STOP_TEST:
                break;

            default:
                break;    
        }
        vTaskDelay(10);
    }
}


void set_device_state( test_state_t state )
{
    devicestate = state;
}


test_state_t get_device_state(void)
{
    return devicestate;
}   