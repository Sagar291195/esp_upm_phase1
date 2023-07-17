/**
 * @file dataMangement.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file is the implementaion of the database management. For the data base we have to implement the sqlite database.
 * @version 1.1
 * @date 2022-05-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dataMangement.h>
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "vfs_fat_internal.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include <esp_spi_flash.h>
#include <esp_flash_spi_init.h>
#include <esp_spiffs.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdbool.h>
#include <esp_littlefs.h>
#include <esp_err.h>>

/****************************************defines**********************************/

#define TAG "dataMangement"

#define DB_LOCATION "/card"

#define PARTITION_NAME "DB"

/**
 * @brief pin configuration for the external flash
 *
 */
#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_23
#define PIN_NUM_CLK GPIO_NUM_18
#define PIN_CS GPIO_NUM_12

#define SPI_DMA_CHAN 1

#define BASE_PATH "/spiffs"

/**
 * @brief file correspond to the db
 *
 */
#define DB_FILENAME "/spiffs/file.db"

/*************************************variables********************************************/

/**
 * @brief Since many task writing to the db at the same time so we need mutual exclusion.
 *
 */
SemaphoreHandle_t mutexForTheDb = NULL;

/**
 * @brief db handle for the raw measurement data
 *
 */
sqlite3 *dbRawMeasurement = NULL;

/**
 * @brief db handle for the archive data
 *
 */
sqlite3 *dbArchivedMeasurement = NULL;

const char *data = "Callback function has been called";

char *zErrMsg;

/*************************************functions prototype********************************************/

void vCreateDataBase();

esp_err_t vInitializeSpiffs();

/*********************************************fucntions****************************************/

esp_err_t initializeSDCard()
{
    sdmmc_card_t *card;
    esp_err_t ret;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {

        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};

    const char mount_point[] = DB_LOCATION;
    ESP_LOGI(TAG, "Initializing SD card");

    ESP_LOGI(TAG, "Using SPI peripheral");

    // sd card host based configuration
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = HSPI_HOST;
    // host.max_freq_khz = 10000; // maximum frequecy supported by the sd card module

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    // spi_bus_config_t bus_cfg;

    // bus_cfg.mosi_io_num = PIN_NUM_MOSI;
    // bus_cfg.miso_io_num = PIN_NUM_MISO;
    // bus_cfg.sclk_io_num = PIN_NUM_CLK;
    // bus_cfg.quadwp_io_num = -1;
    /**
     * @brief initializing the spi bus
     *
     */
    // ret = spi_bus_initialize((spi_host_device_t)host.slot, &bus_cfg, SPI_DMA_CHAN);
    // if (ret != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Failed to initialize bus.");
    //     return ret;
    // }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = (gpio_num_t)PIN_CS;
    slot_config.host_id = (spi_host_device_t)host.slot;

    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem. ");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
        }
        return ret;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
    return ESP_OK;
}

void initializeSDPartSEcond()
{
    esp_err_t ret;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;
    const char mount_point[] = DB_LOCATION;
    ESP_LOGI(TAG, "Initializing SD card");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    // Use POSIX and C standard library functions to work with files.

    // First create a file.
    const char *file_hello = DB_LOCATION "/hello.txt";

    ESP_LOGI(TAG, "Opening file %s", file_hello);
    FILE *f = fopen(file_hello, "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello %s!\n", card->cid.name);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    const char *file_foo = DB_LOCATION "/foo.txt";

    // Check if destination file exists before renaming
    struct stat st;
    if (stat(file_foo, &st) == 0)
    {
        // Delete it if it exists
        unlink(file_foo);
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file %s to %s", file_hello, file_foo);
    if (rename(file_hello, file_foo) != 0)
    {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file %s", file_foo);
    f = fopen(file_foo, "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }

    // Read a line from file
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);

    // Strip newline
    char *pos = strchr(line, '\n');
    if (pos)
    {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    // All done, unmount partition and disable SPI peripheral
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");

    // deinitialize the bus after all devices are removed
    spi_bus_free(host.slot);
}

void vInitializeDataManagementApi()
{
    //  initializeSDPartSEcond();
    esp_log_level_set(TAG,ESP_LOG_WARN);

    /* initializing our spiffs */
    esp_err_t err = vInitializeSpiffs();
    /* Creating the db only when we have initialize the spiffs sucessfully */
    if (err == ESP_OK)
    {
        vCreateDataBase();
    }
}

esp_err_t vInitializeSpiffs()
{
    ESP_LOGW(TAG, "Initializing SPIFFS, for first time it will take 5-10 mins to format....");
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "storage",
        //   .partition_label = NULL,
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
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info("storage", &total, &used);
    if (ret != ESP_OK){
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ret;
    }else{
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
        return ret;
    }
    return ret;
}

/**
 * @brief Open the database
 * @param filename filename in the spiffs, sdcard or flash. File contain the database.
 * @param db double pointer to the database name
 * @return int error code
 */
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

/**
 * @brief This callback is used to check the value into db each time when it is called
 *
 * @param data data is the user data has been passed for any purpose
 * @param argc no of colums presents
 * @param argv  value in the column
 * @param azColName column name
 * @return int
 */
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

/**
 * @brief Execute the db query
 *
 * @param db datebase on which we need to execute the query
 * @param sql sql command
 * @return int error code
 */
static int db_exec(sqlite3 *db, const char *sql)
{

    ESP_LOGD(TAG, "Executing query: %s", sql);

    int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        ESP_LOGE(TAG, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        ESP_LOGI(TAG, "DB Operation done successfully\n");
    }

    return rc;
}

void vCreateDataBase()
{
    // creating the semaphore for thread safe execution
    mutexForTheDb = xSemaphoreCreateMutex();

    sqlite3_initialize(); // initializing the sqlite3
    /* checks if file is present or not, if not present, it will create the file */
    FILE *dbFile = fopen(DB_FILENAME, "r");
    if (dbFile == NULL)
    {
        ESP_LOGW(TAG, "File not present may be due to the first run not creating it!");
        dbFile = fopen(DB_FILENAME, "w");
    }
    fclose(dbFile);

    /* opening the file for the writing purposes */
    dbFile = fopen(DB_FILENAME, "rw");

    /*  opening the database at the given location */
    db_open(DB_FILENAME, &dbRawMeasurement);

    /* creating the raw measurement database */
    char cCreateRawMeasurementTable[] = "CREATE TABLE IF NOT EXISTS rawValuesDataBase(Sample_number INT PRIMARY KEY,Sequence_number INT,timestamp TEXT,average_sdp_value REAL,flowrate REAL,Internal_Pressure_BME REAL,Internal_Temperature_BME REAL,Internal_Humidity_BME REAL,AirDensity REAL,TotalLiters REAL,totalHour REAL,C1BusVolt REAL,C1ShuntVolt REAL,C1ShuntCurr REAL,C2BusVolt REAL,C2ShuntVolt REAL,C2ShuntCur  REAL,C3BusVolt REAL,C3ShuntVolt REAL,C3ShuntCurr REAL,External_Pressure_BME REAL,External_Temperature_BME REAL,External_Humidity_BME REAL);";

    int rc = db_exec(dbRawMeasurement, cCreateRawMeasurementTable);
    if (!rc){
        ESP_LOGI(TAG, "Raw data table created");
    }

    sqlite3_close(dbRawMeasurement); // closing the database

    /* creating the data base for the archived data */
    db_open(DB_FILENAME, &dbArchivedMeasurement);

    char cCreateArchiveTable[] = "CREATE TABLE IF NOT EXISTS archiveData(Sample_number INT,Sequence_number INT, SequenceSummary BLOB);";
    rc = db_exec(dbArchivedMeasurement, cCreateArchiveTable);
    if (!rc)
    {
        ESP_LOGI(TAG, "Archive Data table created");
    }

    sqlite3_close(dbArchivedMeasurement); // closing the database
}

void vInsertSequenceSummaryIntoDataBase(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t sequenceSummary)
{
    ESP_LOGI(TAG, "Sample number is %d and sequnce number is %d", sampleNumber, sequenceNumber);
    ESP_LOGI(TAG, "Hour counter target and effective values and variation  are %.2f and %.2f  %.2f ", sequenceSummary.summary.xHourCounter.fTargetHour, sequenceSummary.summary.xHourCounter.fEffectiveHour, sequenceSummary.summary.xHourCounter.fVariation);

    //  generating the sqlite statement to be executed in the database
    sqlite3_stmt *stmt = NULL;

    if (xSemaphoreTake(mutexForTheDb, portMAX_DELAY) == pdPASS)
    {
        /* opening the database */
        db_open(DB_FILENAME, &dbArchivedMeasurement);
        ESP_LOGI(TAG, "Inserting data into database");
        char *command = "INSERT INTO archiveData values (?,?,?);";
        ESP_LOGI(TAG, "SQLite command for archive data  value is %s", command);
        /* preparing the statement for the execution */
        int rc = sqlite3_prepare_v2(dbArchivedMeasurement, command, -1, &stmt, NULL);

        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in preparing the statement and error message is %s", sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        /* binding the sample number to the satement */
        sqlite3_bind_int(stmt, 1, sampleNumber);
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s", 1, sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        /* binding the sequence number to the satement */
        sqlite3_bind_int(stmt, 2, sequenceNumber);
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s", 2, sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        /* binding the blob values(third values) to the statement.SQLITE_STATIC because the statement is finalized before the buffer is freed: */
        rc = sqlite3_bind_blob(stmt, 3, &sequenceSummary, sizeof(sequenceSummary), SQLITE_STATIC);

        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s", 3, sqlite3_errmsg(dbArchivedMeasurement));
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        /* executing the statement */
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_OK && rc != SQLITE_DONE)
        {
            ESP_LOGE(TAG, "Error in step statement and error message is: %s with error code is %d", sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return;
        }

        /* finalizing the statement of sqlite */
        rc = sqlite3_finalize(stmt);
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

bool vGetSequenceSummaryFromDataBase(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t *sequenceSummary)
{
    bool bReadData = false;
    ESP_LOGI(TAG, "Sample number is %d and sequnce number is %d", sampleNumber, sequenceNumber);
    //  generating the sqlite statement to be executed in the database
    sqlite3_stmt *stmt = NULL;
    if (xSemaphoreTake(mutexForTheDb, portMAX_DELAY) == pdPASS)
    {
        /* opening the database */
        db_open(DB_FILENAME, &dbArchivedMeasurement);
        ESP_LOGI(TAG, "Getting data from database");
        char *command = "SELECT SequenceSummary FROM archiveData WHERE Sample_number = ? AND Sequence_number = ?;";
        ESP_LOGI(TAG, "SQLite command for archive data  value is %s", command);

        /* preparing the statement for the execution */
        int rc = sqlite3_prepare_v2(dbArchivedMeasurement, command, -1, &stmt, NULL);
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in preparing the statement and error message is %s and error code is %d", sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return false;
        }

        /* binding the sample number to the satement */
        sqlite3_bind_int(stmt, 1, sampleNumber);
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s and error code is %d", 1, sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return false;
        }

        /* binding the sequence number to the satement */
        sqlite3_bind_int(stmt, 2, sequenceNumber);
        if (rc != SQLITE_OK)
        {
            ESP_LOGE(TAG, "Error in binding blob to statement at index %d and error message is %s and error code is %d", 2, sqlite3_errmsg(dbArchivedMeasurement), rc);
            xSemaphoreGive(mutexForTheDb);
            return false;
        }

        /* executing the statement to get the number of rows. Since the number of rows expected are only one, but to be safe, we are using the while loop */
        do
        {
            rc = sqlite3_step(stmt);

            ESP_LOGI(TAG, "Row has been found here with column count %d", sqlite3_column_count(stmt));
            ESP_LOGI(TAG, "Number of bytes returned are %d and size of sequence is %d", sqlite3_column_bytes(stmt, 0), sizeof(sequenceSummary_t));
            if (sqlite3_column_bytes(stmt, 0))
            {
                memcpy(sequenceSummary, sqlite3_column_blob(stmt, 0), sizeof(sequenceSummary_t));
                bReadData = true;
            }

        } while (rc == SQLITE_ROW);


        /* finalizing the statement of sqlite */
        rc = sqlite3_finalize(stmt);
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
