/*
 * Handle I2C
 *
 * 11/04/2018  jonesman
 * 	trying to convert IO to /dev/i2c
 *
 * 23/12/2018  Destroyedlolo (http://destroyedlolo.info)
 * 	The I2C device is passed in argument to lcd_dev_open()
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include "dev_io.h"

int i2c_fd = -1;

int lcd_dev_open(const char *dev) {
 
    switch(DEV_TYPE) {
        
        case DEV_TYPE_I2C:
            i2c_fd = open(dev, O_RDWR);
            if(ioctl(i2c_fd, I2C_SLAVE, I2C_ADDR) < 0) {
                printf("I2C ioctl error : %s\r\n", strerror(errno));
                return 0;
            }
            return 1;
            
        case DEV_TYPE_SPI:
            
            break;
        
    }
    
    return 0;
    
}

int lcd_dev_write(uint8_t* data, int len) {
    
    switch(DEV_TYPE) {
        
        case DEV_TYPE_I2C:
            if(write(i2c_fd, data, len) != len) {
                printf("I2C write error : %s\r\n", strerror(errno));
                    return 0;
            }
            return len;
            
        case DEV_TYPE_SPI:
            
            break;
        
    }

    return 0;
    
}

void lcd_dev_close() {
 
    switch(DEV_TYPE) {
        
        case DEV_TYPE_I2C:
            close(i2c_fd);
            return;
            
        case DEV_TYPE_SPI:
            break;
    }
    
}

