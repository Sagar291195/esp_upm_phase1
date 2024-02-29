/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/ 
#include "flash.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                 "FLASH"
#define DB_FILENAME         "/spiffs/test.db"

#define SETTINGS_STORGE_NAME     "settings"
#define MODE_STORAGE        "configstorage"
#define KEY_DEVICE_MODE     "devicemode"
/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
device_settings_t devicesettings;

/********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
static SemaphoreHandle_t mutexForTheDb = NULL; /* Since many task writing to the db at the same time so we need mutual exclusion */
static sqlite3 *dbRawMeasurement = NULL;   /* db handle for the raw measurement data */
static sqlite3 *dbArchivedMeasurement = NULL;  /* db handle for the archive data */
const char *data = "Callback function has been called";
static char *zErrMsg;
static char dev_settings_key[] = "dev-setting";  // key to store data in flash


/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void vCreateDataBase(void);
static esp_err_t vInitializeSpiffs(void);
static int db_open(const char *filename, sqlite3 **db);
static int callback(void *data, int argc, char **argv, char **azColName);
static int db_exec(sqlite3 *db, const char *sql);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *      
 ********************************************************************************************/
static esp_err_t vInitializeSpiffs()
{
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "storage",
        // .partition_label = NULL,
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info("storage", &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ret;
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
        return ret;
    }
    return ret;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
static int db_open(const char *filename, sqlite3 **db)
{
    int rc = sqlite3_open(filename, db);
    if (rc)
    {
        ESP_LOGE(TAG, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return rc;
    }
    else
    {
        ESP_LOGI(TAG, "Opened database successfully");
    }
    return rc;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    ESP_LOGD(TAG, "%s: ", (const char *)data);
    ESP_LOGE(TAG, "NO OF argument is %d", argc);

    for (i = 0; i < argc; i++)
    {
        ESP_LOGW(TAG, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); // printing the table for debugging purpose
    }
    ESP_LOGW(TAG, "\n");
    return 0;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
static int db_exec(sqlite3 *db, const char *sql)
{

    ESP_LOGD(TAG, "Executing query: %s", sql);

    int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        ESP_LOGE(TAG, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        ESP_LOGI(TAG, "DB Operation done successfully");
    }
    return rc;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
static void vCreateDataBase(void)
{
    mutexForTheDb = xSemaphoreCreateMutex();     // creating the semaphore for thread safe execution
    sqlite3_initialize();                       // initializing the sqlite3

    FILE *dbFile = fopen(DB_FILENAME, "r");     /* checks if file is present or not, if not present, it will create the file */
    if (dbFile == NULL)
    {
        ESP_LOGE(TAG, "File not present may be due to the first run not creating it!");
        dbFile = fopen(DB_FILENAME, "w");
    }
    fclose(dbFile);

    if(db_open(DB_FILENAME, &dbRawMeasurement))    /*  opening the database at the given location */    
    {
        ESP_LOGE(TAG, "file open error");
    }
    char cCreateRawMeasurementTable[] = "CREATE TABLE IF NOT EXISTS rawValuesDataBase(Sample_number INT PRIMARY KEY,Sequence_number INT,timestamp TEXT,average_sdp_value REAL,flowrate REAL,Internal_Pressure_BME REAL,Internal_Temperature_BME REAL,Internal_Humidity_BME REAL,AirDensity REAL,TotalLiters REAL,totalHour REAL,C1BusVolt REAL,C1ShuntVolt REAL,C1ShuntCurr REAL,C2BusVolt REAL,C2ShuntVolt REAL,C2ShuntCur  REAL,C3BusVolt REAL,C3ShuntVolt REAL,C3ShuntCurr REAL,External_Pressure_BME REAL,External_Temperature_BME REAL,External_Humidity_BME REAL);";  /* creating the raw measurement database */
    int rc = db_exec(dbRawMeasurement, cCreateRawMeasurementTable);
    if (!rc)
    {
        ESP_LOGI(TAG, "Raw data table created");
    }
    sqlite3_close(dbRawMeasurement); // closing the database
    
    if(db_open(DB_FILENAME, &dbArchivedMeasurement))      /* creating the data base for the archived data */
    {
        ESP_LOGE(TAG, "file open error");
    }
    char cCreateArchiveTable[] = "CREATE TABLE IF NOT EXISTS archiveData(Sample_number INT, Sequence_number INT, SequenceSummary BLOB);";
    rc = db_exec(dbArchivedMeasurement, cCreateArchiveTable);
    if (!rc)
    {
        ESP_LOGI(TAG, "Archive Data table created");
    }
    sqlite3_close(dbArchivedMeasurement); // closing the database

}

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvsread_value_parameter(char *storagename, char *key, void *value)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(storagename, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }

    size_t required_size = 0;
    err = nvs_get_blob(my_handle, key, NULL, &required_size);
    if (err != ESP_OK)
    {
        nvs_close(my_handle);
        ESP_LOGE(TAG, "Error (%s) getting size NVS handle!", esp_err_to_name(err));
        return false;
    }

    if (required_size == 0)
        return false;

    err = nvs_get_blob(my_handle, key, value, &required_size);
    if (err != ESP_OK)
    {
        nvs_close(my_handle);
        ESP_LOGE(TAG, "Error (%s) getting value NVS handle!", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);

    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvswrite_value_float(char *storagename, char *key, float value)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(storagename, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(my_handle, (char *)key, &value, sizeof(value));
    if (err != ESP_OK)
    {
        nvs_close(my_handle);
        ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        nvs_close(my_handle);
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);
    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvswrite_value_parameters(char *storagename, char *key, void *value, uint16_t length)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(storagename, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(my_handle, (char *)key, value, length);
    if (err != ESP_OK)
    {
        nvs_close(my_handle);
        ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        nvs_close(my_handle);
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);
    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvsread_value_u32(char *storagename, char *key, uint32_t *value)
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(storagename, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }
    
    err = nvs_get_u32(nvsHandle, key, value);
    if (err != ESP_OK)
    {
        nvs_close(nvsHandle);
        ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s", esp_err_to_name(err), __func__);
        return false;
    }
    nvs_close(nvsHandle);
    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvswrite_value_u32(char *storagename, char *key, uint32_t value)
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(storagename, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }
    
    err = nvs_set_u32(nvsHandle, key, value);
    if (err != ESP_OK)
    {
        nvs_close(nvsHandle);
        ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s", esp_err_to_name(err), __func__);
        return false;
    }

    err = nvs_commit(nvsHandle);
    if (err != ESP_OK)
    {
        nvs_close(nvsHandle);
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(nvsHandle);
    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvsread_value_u8(char *storagename, char *key, uint8_t *value)
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(storagename, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }
    
    err = nvs_get_u8(nvsHandle, key, value);
    if (err != ESP_OK)
    {
        nvs_close(nvsHandle);
        ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s", esp_err_to_name(err), __func__);
        return false;
    }
    nvs_close(nvsHandle);
    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvswrite_value_u8(char *storagename, char *key, uint8_t value)
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(storagename, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }
    
    err = nvs_set_u8(nvsHandle, key, value);
    if (err != ESP_OK)
    {
        nvs_close(nvsHandle);
        ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s", esp_err_to_name(err), __func__);
        return false;
    }

    err = nvs_commit(nvsHandle);
    if (err != ESP_OK)
    {
        nvs_close(nvsHandle);
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(nvsHandle);
    return true;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvsread_device_mode_settings( device_state_t *devicestate )
{
    bool ret = false;

    ret = nvsread_value_parameter( MODE_STORAGE, KEY_DEVICE_MODE, devicestate );
    if( devicestate->startbyte == 0xFE && ret == true )
    {
        ret = true;
    }
    return ret;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvswrite_device_mode_settings(device_state_t *devicestate)
{
    bool ret = false;

    devicestate->startbyte = 0xFE;
    ret = nvswrite_value_parameters(MODE_STORAGE, KEY_DEVICE_MODE, devicestate, sizeof(device_state_t));
    return ret;
}
/********************************************************************************************
 *      
 ********************************************************************************************/
void nvs_storage_initialize(void)
{
    esp_err_t err = vInitializeSpiffs();        /* initializing our spiffs */
    if (err == ESP_OK)                          /* Creating the db only when we have initialize the spiffs sucessfully */
    {
        ESP_LOGI(TAG, "creating the database");
        vCreateDataBase();
    }
}

/********************************************************************************************
 *      
 ********************************************************************************************/
void database_save_sequence_summary(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t sequenceSummary)
{
    sqlite3_stmt *stmt = NULL;       //  generating the sqlite statement to be executed in the database

    ESP_LOGI(TAG, "Sample number is %d and sequnce number is %d", sampleNumber, sequenceNumber);
    ESP_LOGI(TAG, "Hour counter target and effective values and variation  are %.2f and %.2f  %.2f ", sequenceSummary.summary.xHourCounter.fTargetHour, sequenceSummary.summary.xHourCounter.fEffectiveHour, sequenceSummary.summary.xHourCounter.fVariation);    
    if (xSemaphoreTake(mutexForTheDb, portMAX_DELAY) == pdPASS)
    {
        int rc = sqlite3_open(DB_FILENAME, &dbArchivedMeasurement);
        if (rc != SQLITE_OK) {
            printf("Cannot open database: %s\n", sqlite3_errmsg(dbArchivedMeasurement));
            sqlite3_close(dbArchivedMeasurement);
        }

        char *command = "INSERT INTO archiveData values (?,?,?);";
        ESP_LOGI(TAG, "SQLite command for archive data  value is %s", command);        
        rc = sqlite3_prepare_v2(dbArchivedMeasurement, command, -1, &stmt, NULL);   /* preparing the statement for the execution */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in preparing the statement and error message is %s", sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }
        
        sqlite3_bind_int(stmt, 1, sampleNumber);    /* binding the sample number to the satement */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s", 1, sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        sqlite3_bind_int(stmt, 2, sequenceNumber);       /* binding the sequence number to the satement */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s", 2, sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        rc = sqlite3_bind_blob(stmt, 3, &sequenceSummary, sizeof(sequenceSummary), SQLITE_STATIC);   /* binding the blob values(third values) to the statement.SQLITE_STATIC because the statement is finalized before the buffer is freed: */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s", 3, sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        rc = sqlite3_step(stmt);        /* executing the statement */
        if (rc != SQLITE_OK && rc != SQLITE_DONE)
        {
            ESP_LOGE(TAG, "Error in step statement and error message is: %s with error code is %d", sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        rc = sqlite3_finalize(stmt);        /* finalizing the statement of sqlite */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in finalizing statement and error message is %s", sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        sqlite3_close(dbArchivedMeasurement); // closing the database
        xSemaphoreGive(mutexForTheDb);
        ESP_LOGD(TAG, "Releasing the db lock");
    }
    else
    {
        ESP_LOGW(TAG, "Could not get the db lock for writing the archive results");
    }
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool database_get_sequence_summary(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t *sequenceSummary)
{
    bool bReadData = false;
    int rc = 0;
    
    ESP_LOGI(TAG, "Sample number is %d and sequnce number is %d", sampleNumber, sequenceNumber);
    sqlite3_stmt *stmt = NULL;      

    if (xSemaphoreTake(mutexForTheDb, portMAX_DELAY) == pdPASS)     //  generating the sqlite statement to be executed in the database
    {    
        rc = sqlite3_open(DB_FILENAME, &dbArchivedMeasurement);
        if (rc != SQLITE_OK) {
            printf("Cannot open database: %s\n", sqlite3_errmsg(dbArchivedMeasurement));
            sqlite3_close(dbArchivedMeasurement);
        }

        char *command = "SELECT SequenceSummary FROM archiveData WHERE Sample_number = ? AND Sequence_number = ?;";
        rc = sqlite3_prepare_v2(dbArchivedMeasurement, command, -1, &stmt, NULL);       /* preparing the statement for the execution */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in preparing the statement and error message is %s and error code is %d", sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return false;
        }
        
        sqlite3_bind_int(stmt, 1, sampleNumber);     /* binding the sample number to the satement */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s and error code is %d", 1, sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return false;
        }

        sqlite3_bind_int(stmt, 2, sequenceNumber);      /* binding the sequence number to the satement */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s and error code is %d", 2, sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return false;
        }

        do  /* executing the statement to get the number of rows. Since the number of rows expected are only one, but to be safe, we are using the while loop */
        {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW)
            {
                ESP_LOGI(TAG, "Row has been found here with column count %d", sqlite3_column_count(stmt));
                ESP_LOGI(TAG, "Number of bytes returned are %d and size of sequence is %d", sqlite3_column_bytes(stmt, 0), sizeof(sequenceSummary_t));
                if (sqlite3_column_bytes(stmt, 0))
                {
                    ESP_LOGI(TAG, "database read success");
                    memcpy(sequenceSummary, sqlite3_column_blob(stmt, 0), sizeof(sequenceSummary_t));
                    bReadData = true;
                }
            }
        } while (rc == SQLITE_ROW);

        rc = sqlite3_finalize(stmt);     /* finalizing the statement of sqlite */
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in finalizing statement and error message is %s", sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return bReadData;
        }

        sqlite3_close(dbArchivedMeasurement); // closing the database
        xSemaphoreGive(mutexForTheDb);
        ESP_LOGD(TAG, "Releasing the db lock");
        ESP_LOGI(TAG, "Hour counter target and effective values and variation  are %.2f and %.2f  %.2f ", sequenceSummary->summary.xHourCounter.fTargetHour, sequenceSummary->summary.xHourCounter.fEffectiveHour, sequenceSummary->summary.xHourCounter.fVariation);

        return bReadData;
    }
    else
    {
        ESP_LOGW(TAG, "Could not get the db lock for writing the archive results");
        return bReadData;
    }
    return bReadData;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvsread_device_settings(void)
{
    bool ret = false;

    ret = nvsread_value_parameter(SETTINGS_STORGE_NAME, dev_settings_key, (char *)&devicesettings);
    if ( ret == false || devicesettings.startbyte != 0xFE)
    {   
        devicesettings.startbyte = 0xFE;
        memcpy(devicesettings.screen_lock_password, "1234", strlen("1234"));
        memcpy(devicesettings.metrology_lock_password, "1234", strlen("1234"));
        memcpy(devicesettings.wifi_ssid, "espupm_firmware", strlen("espupm_firmware"));
        memcpy(devicesettings.wifi_password, "12345678", strlen("12345678"));
        devicesettings.buzzer_enable = 0;
        devicesettings.led_enable = 0;
        devicesettings.wifi_enable = 0;
        devicesettings.external_fan_enable = 0;
        devicesettings.screen_sleepmode_enable = 0;
        devicesettings.selected_language = ENGLISH;
        devicesettings.screen_timeout_value = 15;
        devicesettings.luminosity_value = 80;
        devicesettings.contrast_value = 80;

        ret = nvswrite_device_settings(&devicesettings);
        if( ret )
        {
            ESP_LOGI(TAG, "default device settings are saved");
        }
    }
    return ret;
}

/********************************************************************************************
 *      
 ********************************************************************************************/
bool nvswrite_device_settings(device_settings_t *settingsbuffer)
{   
    bool ret = false;
    ret = nvswrite_value_parameters(SETTINGS_STORGE_NAME, dev_settings_key, settingsbuffer, sizeof(device_settings_t));
    if( ret == false)
    {
        ESP_LOGE(TAG, "device settings write error");
    }
    return ret;
}   