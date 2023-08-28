/**
 * @file stateMachine.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file contain the state of the machine and  the declarateion of the function it uses
 * @version 1.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <stdio.h>
#include <stdint.h>
#include <esp_event.h>

/**
 * @brief These are the state of our machine
 *
 */
enum
{

    /**
     * @brief device is totally off, just wake up when shock or movement happened, to avoid stolen, the device will connect to network to send the state everyday ( state is : position, battery state, and update all setting and parameter file )
     *
     */
    MODE_STORAGE,

    /**
     * @brief the device is ready with and wait for password (password can be deleted if no pass is active
     *
     */
    MODE_WAKEUP,
    /**
     * @brief during this mode device is waiting for the job already programmed by user, led is blinking yellow, screen is off and the pump is locked another user can’t stop the program
     *
     */
    MODE_WAIT,

    /**
     * @brief led color depend on problem during work (blinking green: no problem, blinking orange: problem during sample , same as storage mode except scan wifi every 30 sec, mode shutdown is when job is finished but END is not valid from user, it scan wifi every 30 sec to check if somebody have access
     *
     */
    MODE_SHUT_DOWN_LOW_POWER,

    /**
     * @brief led is double quick blinking red, this mode is strictly during sample ONLY.low bat no power is only during sample, it's quite like critical alert.it mean that the device don't have enought power to finish the task
     *
     */
    MODE_LOW_BATTERY_NO_POWER,

    /**
     * @brief  led is blinking Cyan, this mode is enable during metrology validation test

     *
     */
    MODE_ETALON,

    /**
     * @brief led is Cyan, this mode is enable during metrology is enable
     *
     */
    MODE_METROLOGY,

    /**
     * @brief led is green , all is on device is ready for a new programing work, wifi is enable fully.
     *
     */
    MODE_READY,

    /**
     * @brief led is blinking slow blue, work is in progress , scan wifi every 30 sec
     *
     */
    MODE_BUSY_SAMPLE_IN_PROGRESS,

    /**
     * @brief you can valid the end or export data there
     *
     */
    MODE_RECAP_END,

    /**
     * @brief  led is blinking cyan, red, green, blue, yellow,test all the sensor and gsm gps network, motor is started at max value and compare to factory value if error is less than 5% result is ok
     *
     */
    MODE_AUTO_TEST,

};

// Declare an event base
ESP_EVENT_DECLARE_BASE(MACHINE_STATE);


/**
 * @brief Initiate the state machine of the system
 * 
 */
void vInitiateTheStateMachine();


#endif // __STATEMACHINE_H__