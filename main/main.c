#include "Display_SPD2010.h"
#include "PCF85063.h"
#include "QMI8658.h"
#include "SD_MMC.h"
#include "Wireless.h"
#include "TCA9554PWR.h"
#include "BAT_Driver.h"
#include "PWR_Key.h"
#include "PCM5101.h"
#include "ui/ui.h"
#include "lvgl.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "Record_Audio.c"
#define SD_SPI_HOST SPI2_HOST
#define SD_SPI_MOSI GPIO_NUM_11
#define SD_SPI_MISO GPIO_NUM_13
#define SD_SPI_SCK GPIO_NUM_12
#define SD_SPI_CS GPIO_NUM_10
#define MOUNT_POINT "/sdcard"

// Modify your SD_Init function to return status

void Driver_Loop(void *parameter)
{
    Wireless_Init();
    while (1)
    {
        QMI8658_Loop();
        PCF85063_Loop();
        BAT_Get_Volts();
        PWR_Loop();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}
void Driver_Init(void)
{
    PWR_Init();
    BAT_Init();
    I2C_Init();
    EXIO_Init(); // Example Initialize EXIO
    Flash_Searching();
    PCF85063_Init();
    QMI8658_Init();
    xTaskCreatePinnedToCore(
        Driver_Loop,
        "Other Driver task",
        4096,
        NULL,
        3,
        NULL,
        0);
}

void app_main(void)
{
    Driver_Init();
    LCD_Init();
    LVGL_Init();
    ui_init();
    //Audio_Init(); // no need to initialize this it's speaker
    // both of the following functions are in the same folder we can edit that later
    // like adding the start_recording call to sd card initialization (optional).
    sd_card_initialization();
    start_recording();
}
