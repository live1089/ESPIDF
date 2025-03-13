#include "setup_spi.h"
#include "setup_gpio.h"
#include "esp_lcd_ili9341.h"






void setup_spi_init(void)
{
       
    // spi_bus_config_t spi_cfg = {
    //     .sclk_io_num = LCD_SCLK_NUM,
    //     .mosi_io_num = LCD_MOSI_NUM,
    //     .miso_io_num = LCD_MISO_NUM,
    //     .quadwp_io_num = GPIO_NUM_NC,
    //     .quadhd_io_num = GPIO_NUM_NC,
    //     .max_transfer_sz = LCD_H_RES_DATA * LCD_BUFFER_H_DATA * sizeof(uint16_t),
    //     .flags = SPICOMMON_BUSFLAG_MASTER |
    //              SPICOMMON_BUSFLAG_SCLK |
    //              SPICOMMON_BUSFLAG_MOSI,
    //     .intr_flags = 0
    // };
    // ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI2_HOST,&spi_cfg,SPI_DMA_CH_AUTO));
}