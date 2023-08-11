/**
 * @file stateMachine.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file contain the implementation of the state machine of the machine. Each state is being consider as the event of in the system. What ever are the 
 * state machine in the system are devine here.
 * @version 1.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stateMachine.h>
#include <esp_event.h>
#include <esp_log.h>

/****************************************defines****************************************/

#define TAG "stateMachine"

/**
 * @brief defining the state of the machine
 *
 */
ESP_EVENT_DEFINE_BASE(MACHINE_STATE);

/*********************************************variables***********************************/

/**********************************functions**********************************************/

void vStateMachineEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case MODE_STORAGE:
        // make the led blink for 2 seconds
        // buzzer set for a second
        // led colour should be orange
        // send alert to the user using gsm
        //update the things board with the current state of the machine
        break;

    case MODE_WAKEUP:
        break;

    case MODE_WAIT:
        break;

    case MODE_SHUT_DOWN_LOW_POWER:
        break;

    case MODE_LOW_BATTERY_NO_POWER:
        break;

    case MODE_ETALON:
        break;

    case MODE_READY:
        break;

    case MODE_BUSY_SAMPLE_IN_PROGRESS:
        break;

    case MODE_RECAP_END:
        break;

    case MODE_AUTO_TEST:
        break;

    default:
        break;
    }
}

void vInitiateTheStateMachine()
{
     // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_instance_register(MACHINE_STATE, ESP_EVENT_ANY_ID, vStateMachineEventHandler, NULL, NULL));

}