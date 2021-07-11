/*
 * Handle I2C
 *
 * 11/04/2018  jonesman
 *   trying to convert IO to /dev/i2c
 *
 * 23/12/2018  Destroyedlolo (http://destroyedlolo.info)
 *   The I2C device is passed in argument to lcd_dev_open()
 *
 * 21/09/2020  JG1UAA (https://github.com/jg1uaa)
 *  Rename dev_io.h -> ArduiPi_OLED_devio.h
 *  Add lcd_dev_default_device()
 *
 * 26/09/2020  JG1UAA (https://github.com/jg1uaa)
 *  Add NetBSD I2C ioctl support
 *  thanks to http://www.yagoto-urayama.jp/~oshimaya/netbsd/rpi_i2c.html
 *
 * 04/10/2020  JG1UAA (https://github.com/jg1uaa)
 *  Add ARDUIPI_OLED_I2CDEV variable to change default I2C device
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#if defined(__linux__)
#include <linux/i2c-dev.h>
#elif defined(__NetBSD__)
#include <dev/i2c/i2c_io.h>
#endif
#include <string.h>
#include "ArduiPi_OLED_devio.h"

static int i2c_fd = -1;

int lcd_dev_open(const char *dev) {

    switch(DEV_TYPE) {

        case DEV_TYPE_I2C:
            i2c_fd = open(dev, O_RDWR);
#if defined(__linux__)
            if(ioctl(i2c_fd, I2C_SLAVE, I2C_ADDR) < 0) {
                printf("I2C ioctl error : %s\r\n", strerror(errno));
                return 0;
            }
#elif defined(__NetBSD__)
            if(i2c_fd < 0) {
                printf("I2C open error\r\n");
                return 0;
            }
#endif
            return 1;

        case DEV_TYPE_SPI:

            break;

    }

    return 0;

}

int lcd_dev_write(uint8_t* data, int len) {

#if defined(__NetBSD__)
    i2c_ioctl_exec_t cmd = {
        .iie_op = I2C_OP_WRITE_WITH_STOP,
        .iie_addr = I2C_ADDR,
        .iie_cmd = &data[0],
        .iie_cmdlen = 1,
        .iie_buf = &data[1],
        .iie_buflen = len - 1,
    };
#endif

    switch(DEV_TYPE) {

        case DEV_TYPE_I2C:
#if defined(__linux__)
            if(write(i2c_fd, data, len) != len) {
                printf("I2C write error : %s\r\n", strerror(errno));
                    return 0;
            }
#elif defined(__NetBSD__)
            if(ioctl(i2c_fd, I2C_IOCTL_EXEC, &cmd) < 0) {
                printf("I2C write error : %s\r\n", strerror(errno));
                    return 0;
            }
#endif
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

char *lcd_dev_default_device() {
    char *p;

    p = getenv("ARDUIPI_OLED_I2CDEV");
    return (p != NULL) ? p : I2C_DEV;
}
