/*
 * DEV_IO header
 *
 * 11/04/2018  jonesman
 * 	trying to convert IO to /dev/i2c
 *
 * 23/12/2018  Destroyedlolo (http://destroyedlolo.info)
 * 	The I2C device is passed in argument to lcd_dev_open()
 *
 * 21/09/2020  JG1UAA (https://github.com/jg1uaa)
 *	Rename dev_io.h -> ArduiPi_OLED_devio.h
 *	Remove I2C_DEV, add lcd_dev_default_device()
 */

#ifndef ArduiPi_OLED_devio_H
#define ArduiPi_OLED_devio_H

#include <stdint.h>

#define DEV_TYPE_I2C 1
#define DEV_TYPE_SPI 2

#define I2C_ADDR 0x3c

#define DEV_TYPE 1

#if defined (__cplusplus)
extern "C" {
#endif

    int lcd_dev_open(const char *dev);
    int lcd_dev_write(uint8_t* data, int len);
    void lcd_dev_close();
    char *lcd_dev_default_device();

#if defined (__cplusplus)
}
#endif

#endif
