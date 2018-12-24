/*
 * DEV_IO header
 *
 * 11/04/2018  jonesman
 * 	trying to convert IO to /dev/i2c
 *
 * 23/12/2018  Destroyedlolo (http://destroyedlolo.info)
 * 	The I2C device is passed in argument to lcd_dev_open()
 */

#ifndef DEV_IO_H
#define DEV_IO_H

#include <stdint.h>

#define DEV_TYPE_I2C 1
#define DEV_TYPE_SPI 2

#ifdef BANANAPI
#	define I2C_DEV "/dev/i2c-2"
#else
#	define I2C_DEV "/dev/i2c-0"
#endif
#define I2C_ADDR 0x3c

#define DEV_TYPE 1

#if defined (__cplusplus)
extern "C" {
#endif

    int lcd_dev_open(const char *dev);
    int lcd_dev_write(uint8_t* data, int len);
    void lcd_dev_close();

#if defined (__cplusplus)
}
#endif

#endif
