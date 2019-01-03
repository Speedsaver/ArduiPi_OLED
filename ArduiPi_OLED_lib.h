/******************************************************************
 This is the common ArduiPi include file for ArduiPi board
 
02/18/2013  Charles-Henri Hallard (http://hallard.me)
            Modified for compiling and use on Raspberry ArduiPi Board
            LCD size and connection are now passed as arguments on 
            the command line (no more #define on compilation needed)
            ArduiPi project documentation http://hallard.me/arduipi

07/26/2013  Charles-Henri Hallard (http://hallard.me)
            Done generic library for different OLED type
            
 Written by Charles-Henri Hallard for Fun .
 All text above must be included in any redistribution.

	--- European time format ---
25/12/2018	Destroyedlolo (http://destroyedlolo.info)
	Remove SPI as not supported

 ******************************************************************/

#ifndef _ArduiPi_OLED_lib_H
#define _ArduiPi_OLED_lib_H

#include <stdio.h>
#include <stdarg.h>  
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stdint.h>
#include "dev_io.h"

// Oled supported display
enum { 
	OLED_ADAFRUIT_I2C_128x32 = 0,
	OLED_ADAFRUIT_I2C_128x64,
	OLED_SEEED_I2C_128x64,
	OLED_SEEED_I2C_96x96,
	OLED_SH1106_I2C_128x64,
	OLED_LAST_OLED
};

static const char * oled_type_str[] = {
    "SSD1306 (Adafruit) I2C 128x32",
    "SSD1306 (Adafruit) I2C 128x64",
    "SSD1308 (Seeed) I2C 128x64",
    "SSD1308 (Seeed) I2C 96x96",
    "SH1106 I2C 128x64"
};

// Arduino Compatible type
typedef uint8_t boolean;
typedef uint8_t byte;

// Arduino Compatible Macro
#define _BV(bit) (1 << (bit))

// GCC Missing
#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

#endif
