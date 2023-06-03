#ifndef ESP_UPM_VARIABLES_H
#define ESP_UPM_VARIABLES_H

// struct sensors data
typedef struct
{
    float       bme280_temp;
    float       bme280_humid;
    float       bme280_press;
    float       bme680_temp;    
    float       bme680_press;       
    float       bme680_humid;       
    float       bme680_gas_resist; 
    uint16_t    sdp32_diff_press;
    uint16_t    sdp32_temp;
    uint16_t    motor_rpm;
} sensor_data_t;

// if sensor connect - true
typedef struct
{
    bool bme280;
    bool bme680;
    bool ds3231;
    bool ina3231;
    bool mpu6050;
    bool sdp32;
    bool buzzer;
} sensor_present_t;


//extern char *guiH = "";
//extern char *guiM = "";
//extern char *guiS = "";




#endif // ESP_UPM_VARIABLES_H