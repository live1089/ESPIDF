#ifndef SETUP_SPI_H
#define SETUP_SPI_H

#include "driver/spi_master.h"

#define LCD_SPI2_HOST SPI2_HOST

#define LCD_H_RES_DATA  (320)

#define LCD_BUFFER_H_DATA (240)

void setup_spi_init(void);


#endif
