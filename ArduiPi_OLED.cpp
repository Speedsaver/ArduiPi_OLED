/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

02/18/2013  Charles-Henri Hallard (http://hallard.me)
            Modified for compiling and use on Raspberry ArduiPi Board
            LCD size and connection are now passed as arguments on
            the command line (no more #define on compilation needed)
            ArduiPi project documentation http://hallard.me/arduipi
07/01/2013  Charles-Henri Hallard
            Reduced code size removed the Adafruit Logo (sorry guys)
            Buffer for OLED is now dynamic to LCD size
            Added support of Seeed OLED 64x64 Display

07/26/2013  Charles-Henri Hallard
            modified name for generic library using different OLED type

02/24/2015  Charles-Henri Hallard
            added support for 1.3" I2C OLED with SH1106 driver

  --- European time format ---
23/12/2018  Destroyedlolo (http://destroyedlolo.info)
      I2C device is passed as argument
      Verbose errors
      Remove warnings
25/12/2018  Destroyedlolo (http://destroyedlolo.info)
      remove SPI
      Add OnOff()
30/12/2018  Destroyedlolo (http://destroyedlolo.info)
      Add Flip()
21/09/2020  JG1UAA (https://github.com/jg1uaa)
      Add hallard's ArduiPi_OLED compatible layer

*********************************************************************/

#include "./ArduiPi_OLED_lib.h"
#include "./Adafruit_GFX.h"
#include "./ArduiPi_OLED.h"
#include "./ArduiPi_OLED_devio.h"

  // 8x8 Font ASCII 32 - 127 Implemented
// Users can modify this to support more characters(glyphs)
// BasicFont is placed in code memory.

// This font can be freely used without any restriction(It is placed in public domain)
const unsigned char seedfont[][8] =
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00},
  {0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00},
  {0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00},
  {0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00},
  {0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00},
  {0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00},
  {0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00},
  {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00},
  {0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
  {0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00},
  {0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00},
  {0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00},
  {0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00},
  {0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00},
  {0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00},
  {0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00},
  {0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00},
  {0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00},
  {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00},
  {0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},
  {0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00},
  {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00},
  {0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00},
  {0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00},
  {0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00},
  {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00},
  {0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00},
  {0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00},
  {0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00},
  {0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00},
  {0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00},
  {0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00},
  {0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00},
  {0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00},
  {0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00},
  {0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00},
  {0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00},
  {0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00},
  {0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00},
  {0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00},
  {0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00},
  {0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00},
  {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00},
  {0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00},
  {0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00},
  {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00},
  {0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00},
  {0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00},
  {0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00},
  {0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00},
  {0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00},
  {0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00},
  {0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00},
  {0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00},
  {0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00},
  {0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00},
  {0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00},
  {0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00},
  {0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00},
  {0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00},
  {0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00},
  {0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00},
  {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00},
  {0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00},
  {0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00},
  {0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00},
  {0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00}
};

// the most basic function, set a single pixel
void ArduiPi_OLED::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  uint8_t * p = poledbuff ;
  uint8_t c;

  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

    /*
    // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = WIDTH - x - 1;
    break;

    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
    break;

    case 3:
      swap(x, y);
      y = HEIGHT - y - 1;
    break;
  }
*/
  if (oled_type == OLED_SEEED_I2C_96x96 )
  {
    // Get where to do the change in the buffer
    p = poledbuff + (x + (y/2)*oled_width );

    // Get old value to not touch the other nible
    c = *p;

    // We are on High nible ?
    if ( ((y/2)&1) == 1 )
    {
      c &= 0x0F;
      c|= (color==WHITE ? grayH:0x00) << 4;
    }
    else
    {
      c &= 0xF0;
      c|= (color==WHITE ? grayL:0x00) ;
    }

    // set new nible value leaving the other untouched
    *p = c;
  }
  else
  {
    // Get where to do the change in the buffer
    p = poledbuff + (x + (y/8)*oled_width );

    // x is which column
    if (color == WHITE)
      *p |=  _BV((y%8));
    else
      *p &= ~_BV((y%8));
  }
}

// Get the value of a pixel
uint16_t ArduiPi_OLED::getPixel(int16_t x, int16_t y){
  uint8_t *p = poledbuff;
  if((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return 0;

  if(oled_type == OLED_SEEED_I2C_96x96){
    fputs("Not implemented\n", stderr);
    return 0;
  } else {
    p = poledbuff + (x + (y/8)*oled_width );
    return(!!(*p & _BV((y%8))));
  }
}

// Display instantiation
ArduiPi_OLED::ArduiPi_OLED()
{
  //fprintf(stdout,"Default ctor ArduiPi_OLED\n");
  // Init all var, and clean
  // Command I/O
  rst = 0 ;
  dc  = 0 ;
  spi =  0 ;

  // Lcd size
  oled_width  = 0;
  oled_height = 0;

  // Empty pointer to OLED buffer
  poledbuff = NULL;
  xmitcount = 0;
  xmitmax = 0;
}


// tell apps to SPI not supported
boolean ArduiPi_OLED::oled_is_spi_proto(uint8_t OLED_TYPE){
  return false;
}

// initializer for OLED Type
boolean ArduiPi_OLED::select_oled(uint8_t OLED_TYPE){
  return select_oled(OLED_TYPE, lcd_dev_default_device());
}

boolean ArduiPi_OLED::select_oled(uint8_t OLED_TYPE, const char *dev){
  // Default type
  oled_width  = 128;
  oled_height = 64;
  //_i2c_addr = 0x00;
  oled_type = OLED_TYPE;

  // default OLED are using internal boost VCC converter
  vcc_type = SSD_Internal_Vcc;

  // Oled supported display
  // Setup size and I2C address
  switch (OLED_TYPE)
  {
    case OLED_ADAFRUIT_I2C_128x32:
      oled_height = 32;
      //_i2c_addr = ADAFRUIT_I2C_ADDRESS;
    break;

    case OLED_ADAFRUIT_I2C_128x64:
      //_i2c_addr = ADAFRUIT_I2C_ADDRESS;
    break;

    case OLED_SEEED_I2C_128x64:
      //_i2c_addr = SEEED_I2C_ADDRESS ;
      vcc_type = SSD_External_Vcc;
    break;

    case OLED_SEEED_I2C_96x96:
      oled_width  = 96;
      oled_height = 96;
      //_i2c_addr = SEEED_I2C_ADDRESS ;
    break;

    case OLED_SH1106_I2C_128x64:
      //_i2c_addr = SH1106_I2C_ADDRESS;
    break;

    // houston, we have a problem
    default:
    fputs("Unknown display", stderr);
      return false;
    break;
  }

  // Buffer size differ from OLED type, 1 pixel is one bit
  // execpt for 96x96 seed, 1 pixel is 1 nible
  oled_buff_size = oled_width * oled_height ;

  if ( OLED_TYPE == OLED_SEEED_I2C_96x96 )
    oled_buff_size = oled_buff_size / 2 ;
  else
    oled_buff_size = oled_buff_size / 8;

  // De-Allocate memory for OLED buffer if any
  if (poledbuff)
    free(poledbuff);

  // Allocate memory for OLED buffer
  poledbuff = (uint8_t *) malloc ( oled_buff_size );

  if (!poledbuff){
    perror("buffer malloc()");
    return false;
  }

  // Init IO
  if (!lcd_dev_open(dev)){
    fputs("lcd_dev_open() failed\n", stderr);
    return false;
  }

  // Dump the instance
  //fprintf(stdout,"Selected OLED=%s\n", dev);
  //fprintf(stdout,"Buffer=%p, bufsize=%d\n", (void*)poledbuff, oled_buff_size);
  //fprintf(stdout,"I2C=%d %d %d %d\n", _i2c_addr, dc, rst, spi);
  //fprintf(stdout,"Width=%d, Height=%d\n", oled_width, oled_height);
  //fprintf(stdout,"Vcc=%d, type=%d\n", vcc_type, oled_type);
  //fprintf(stdout,"Grays=%d %d\n", grayH, grayL );

  return true;
}

boolean ArduiPi_OLED::init(int8_t dc, int8_t rst, int8_t cs, uint8_t oled_type){
    return false;  // SPI not supported
}

boolean ArduiPi_OLED::init(int8_t rst, uint8_t oled_type){
    return select_oled(oled_type);
}

boolean ArduiPi_OLED::init(uint8_t oled_type, const char *dev){
    return select_oled(oled_type, dev);
}

void ArduiPi_OLED::close(void)
{
  // De-Allocate memory for OLED buffer if any
  if (poledbuff)
    free(poledbuff);

  poledbuff = NULL;

  lcd_dev_close();
}

void ArduiPi_OLED::xmitflush() {
  lcd_dev_write(xmitbuff, xmitcount);
  if(xmitcount>xmitmax)
    xmitmax = xmitcount;
  xmitcount = 0;
}

void ArduiPi_OLED::begin( void )
{
  uint8_t multiplex;
  uint8_t chargepump;
  uint8_t compins;
  uint8_t contrast;
  uint8_t precharge;

  constructor(oled_width, oled_height);

  // depends on OLED type configuration
  if (oled_height == 32)
  {
    multiplex = 0x1F;
    compins   = 0x02;
    contrast  = 0x8F;
  }
  else
  {
    if (oled_type == OLED_SEEED_I2C_96x96 )
    {
      multiplex = 0x5F;
      compins   = 0x12;
      contrast  = 0x53;
    }
    // So 128x64
    else
    {
      multiplex = 0x3F;
      compins   = 0x12;

      if (oled_type == OLED_SH1106_I2C_128x64)
        contrast = 0x80;
      else
        contrast = (vcc_type==SSD_External_Vcc?0x9F:0xCF);
    }
  }

  if (vcc_type == SSD_External_Vcc)
  {
    chargepump = 0x10;
    precharge  = 0x22;
  }
  else
  {
    chargepump = 0x14;
    precharge  = 0xF1;
  }

  if (oled_type == OLED_SEEED_I2C_96x96 )
    sendCommand(SSD1327_Set_Command_Lock, 0x12); // Unlock OLED driver IC MCU interface from entering command. i.e: Accept commands

  sendCommand(SSD_Display_Off);
  sendCommand(SSD_Set_Muliplex_Ratio, multiplex);

  if (oled_type == OLED_SEEED_I2C_96x96 )
  {
    sendCommand(SSD1327_Set_Display_Clock_Div, 0x01);
    sendCommand(SSD1327_Set_Display_Start_Line    , 0   );
    sendCommand(SSD1327_Set_Display_Offset, 96  );
    sendCommand(SSD_Set_Segment_Remap     , 0x46);

    sendCommand(0xAB); // set vdd internal
    sendCommand(0x01); //

    sendCommand(0xB1); // Set Phase Length
    sendCommand(0X51); //

    sendCommand(0xB9); //

    sendCommand(0xBC); // set pre_charge voltage/VCOMH
    sendCommand(0x08); // (0x08);

    sendCommand(0xBE); // set VCOMH
    sendCommand(0X07); // (0x07);

    sendCommand(0xB6); // Set second pre-charge period
    sendCommand(0x01); //

    sendCommand(0xD5); // enable second precharge and enternal vsl
    sendCommand(0X62); // (0x62);

    // Set Normal Display Mode
    sendCommand(SSD1327_Normal_Display);

    // Row Address
    // Start 0 End 95
    sendCommand(SSD1327_Set_Row_Address, 0, 95);

    // Column Address
    // Start from 8th Column of driver IC. This is 0th Column for OLED
    // End at  (8 + 47)th column. Each Column has 2 pixels(segments)
    sendCommand(SSD1327_Set_Column_Address, 8, 0x37 );

    // Map to horizontal mode
    sendCommand(0xA0); // remap to
    sendCommand(0x46); // Vertical mode

    // Init gray level for text. Default:Brightest White
    grayH= 0xF0;
    grayL= 0x0F;
  }
  else if (oled_type == OLED_SH1106_I2C_128x64)
  {
    sendCommand(SSD1306_Set_Lower_Column_Start_Address|0x02); /*set lower column address*/
    sendCommand(SSD1306_Set_Higher_Column_Start_Address);     /*set higher column address*/
    sendCommand(SSD1306_Set_Start_Line);                      /*set display start line*/
    sendCommand(SH1106_Set_Page_Address);    /*set page address*/
    sendCommand(SSD_Set_Segment_Remap|0x01); /*set segment remap*/
    sendCommand(SSD1306_Normal_Display);     /*normal / reverse*/
    sendCommand(0xad);    /*set charge pump enable*/
    sendCommand(0x8b);    /*external VCC   */
    sendCommand(0x30);    /*0X30---0X33  set VPP   9V liangdu!!!!*/
    sendCommand(SSD1306_Set_Com_Output_Scan_Direction_Remap);    /*Com scan direction*/
    sendCommand(SSD1306_Set_Display_Offset);    /*set display offset*/
    sendCommand(0x00);   /*   0x20  */
    sendCommand(SSD1306_Set_Display_Clock_Div);    /*set osc division*/
    sendCommand(0x80);
    sendCommand(SSD1306_Set_Precharge_Period);    /*set pre-charge period*/
    sendCommand(0x1f);    /*0x22*/
    sendCommand(SSD1306_Set_Com_Pins);    /*set COM pins*/
    sendCommand(0x12);
    sendCommand(SSD1306_Set_Vcomh_Deselect_Level);    /*set vcomh*/
    sendCommand(0x40);
  }
  else
  {
    sendCommand(SSD1306_Charge_Pump_Setting, chargepump);
    sendCommand(SSD1306_Set_Memory_Mode, 0x00);              // 0x20 0x0 act like ks0108
    sendCommand(SSD1306_Set_Display_Clock_Div, 0x80);      // 0xD5 + the suggested ratio 0x80
    sendCommand(SSD1306_Set_Display_Offset, 0x00);        // no offset
    sendCommand(SSD1306_Set_Start_Line | 0x0);            // line #0
#if !SLOW_WRITE
    xmitflush();
#endif
    // use this two commands to flip display
    sendCommand(SSD_Set_Segment_Remap | 0x1);
    sendCommand(SSD1306_Set_Com_Output_Scan_Direction_Remap);
#if !SLOW_WRITE
    xmitflush();
#endif

    sendCommand(SSD1306_Set_Com_Pins, compins);
    sendCommand(SSD1306_Set_Precharge_Period, precharge);
    sendCommand(SSD1306_Set_Vcomh_Deselect_Level, 0x40); // 0x40 -> unknown value in datasheet
    sendCommand(SSD1306_Entire_Display_Resume);
    sendCommand(SSD1306_Normal_Display);         // 0xA6
#if !SLOW_WRITE
    xmitflush();
#endif

    // Reset to default value in case of
    // no reset pin available on OLED,
    sendCommand( SSD_Set_Column_Address, 0, 127 );
    sendCommand( SSD_Set_Page_Address, 0,   7 );
#if !SLOW_WRITE
    xmitflush();
#endif
  }

  sendCommand(SSD_Set_ContrastLevel, contrast);
#if !SLOW_WRITE
  xmitflush();
#endif

  stopscroll();

  // Empty uninitialized buffer
  clearDisplay();

  // turn on oled panel
  sendCommand(SSD_Display_On);
#if !SLOW_WRITE
  xmitflush();
#endif

  // wait 100ms
  usleep(100000);
}

// Turn the display On and Off
void ArduiPi_OLED::OnOff( boolean i ){
  sendCommand(i ? SSD_Display_On : SSD_Display_Off);
}

// Turn the display upside-down
void ArduiPi_OLED::Flip( boolean i ){
  if(i){
    sendCommand( SSD1306_Set_Seg_Direction_Nomal );
    sendCommand( SSD1306_Set_Com_Output_Scan_Direction_Normal );
  } else {
    sendCommand( SSD1306_Set_Seg_Direction_Rever );
    sendCommand( SSD1306_Set_Com_Output_Scan_Direction_Remap );
  }
}

// Only valid for Seeed 96x96 OLED
void ArduiPi_OLED::setGrayLevel(uint8_t grayLevel)
{
    grayH = (grayLevel << 4) & 0xF0;
    grayL =  grayLevel & 0x0F;
}

void ArduiPi_OLED::setSeedTextXY(unsigned char Row, unsigned char Column)
{
    //Column Address
    sendCommand(0x15);             /* Set Column Address */
    sendCommand(0x08+(Column*4));  /* Start Column: Start from 8 */
    sendCommand(0x37);             /* End Column */
    // Row Address
    sendCommand(0x75);             /* Set Row Address */
    sendCommand(0x00+(Row*8));     /* Start Row*/
    sendCommand(0x07+(Row*8));     /* End Row*/
}

void ArduiPi_OLED::putSeedChar(char C)
{
    if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
    {
        C=' '; //Space
    }

    for(int i=0;i<8;i=i+2)
    {
        for(int j=0;j<8;j++)
        {
            // Character is constructed two pixel at a time using vertical mode from the default 8x8 font
            char c=0x00;
            char bit1=( seedfont[(int)C-32][(int)i]   >> j) & 0x01;
            char bit2=( seedfont[(int)C-32][(int)i+1] >> j) & 0x01;

           // Each bit is changed to a nibble
            c|=(bit1)?grayH:0x00;
            c|=(bit2)?grayL:0x00;
            sendData(c);
        }
    }
}

void ArduiPi_OLED::putSeedString(const char *String)
{
    unsigned char i=0;
    while(String[i])
    {
        putSeedChar( String[i]);
        i++;
    }
}

void ArduiPi_OLED::setBrightness(uint8_t Brightness)
{
   sendCommand(SSD_Set_ContrastLevel);
   sendCommand(Brightness);
}


void ArduiPi_OLED::invertDisplay(uint8_t i)
{
  if (i)
    sendCommand(SSD_Inverse_Display);
  else
    sendCommand(oled_type==OLED_SEEED_I2C_96x96 ? SSD1327_Normal_Display : SSD1306_Normal_Display);
}

void ArduiPi_OLED::sendCommand(uint8_t c)
{
  uint8_t buff[2] ;

  // Clear D/C to switch to command mode
  buff[0] = SSD_Command_Mode ;
  buff[1] = c;

#if SLOW_WRITE
  // Write Data on I2C
  lcd_dev_write(buff, 2);
#else
  addxmit(buff,2);
#endif
}

void ArduiPi_OLED::sendCommand(uint8_t c0, uint8_t c1)
{
  uint8_t buff[3] ;
  buff[1] = c0;
  buff[2] = c1;

   // Clear D/C to switch to command mode
  buff[0] = SSD_Command_Mode ;

#if SLOW_WRITE
  // Write Data on I2C
  lcd_dev_write(buff, 3) ;
#else
  addxmit(buff,3);
#endif
}

void ArduiPi_OLED::sendCommand(uint8_t c0, uint8_t c1, uint8_t c2)
{
  uint8_t buff[4] ;

  buff[1] = c0;
  buff[2] = c1;
  buff[3] = c2;

  // Clear D/C to switch to command mode
  buff[0] = SSD_Command_Mode;

#if SLOW_WRITE
  // Write Data on I2C
  lcd_dev_write(buff, 4)  ;
#else
  addxmit(buff,4);
#endif
}


// startscrollright
// Activate a right handed scroll for rows start throufastI2Cwritegh stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void ArduiPi_OLED::startscrollright(uint8_t start, uint8_t stop)
{
  sendCommand(SSD_Right_Horizontal_Scroll);
  sendCommand(0X00);
  sendCommand(start);
  sendCommand(0X00);
  sendCommand(stop);
  sendCommand(0X01);
  sendCommand(0XFF);
  sendCommand(SSD_Activate_Scroll);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void ArduiPi_OLED::startscrollleft(uint8_t start, uint8_t stop)
{
  sendCommand(SSD_Left_Horizontal_Scroll);
  sendCommand(0X00);
  sendCommand(start);
  sendCommand(0X00);
  sendCommand(stop);
  sendCommand(0X01);
  sendCommand(0XFF);
  sendCommand(SSD_Activate_Scroll);
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void ArduiPi_OLED::startscrolldiagright(uint8_t start, uint8_t stop)
{
  sendCommand(SSD1306_SET_VERTICAL_SCROLL_AREA);
  sendCommand(0X00);
  sendCommand(oled_height);
  sendCommand(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  sendCommand(0X00);
  sendCommand(start);
  sendCommand(0X00);
  sendCommand(stop);
  sendCommand(0X01);
  sendCommand(SSD_Activate_Scroll);
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void ArduiPi_OLED::startscrolldiagleft(uint8_t start, uint8_t stop)
{
  sendCommand(SSD1306_SET_VERTICAL_SCROLL_AREA);
  sendCommand(0X00);
  sendCommand(oled_height);
  sendCommand(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  sendCommand(0X00);
  sendCommand(start);
  sendCommand(0X00);
  sendCommand(stop);
  sendCommand(0X01);
  sendCommand(SSD_Activate_Scroll);
}


void ArduiPi_OLED::setHorizontalScrollProperties(bool direction,uint8_t startRow, uint8_t endRow,
                                                 uint8_t startColumn, uint8_t endColumn, uint8_t scrollSpeed)
{
  if(Scroll_Right == direction)
  {
      //Scroll Right
      sendCommand(SSD_Left_Horizontal_Scroll);
  }
  else
  {
      //Scroll Left
      sendCommand(SSD_Right_Horizontal_Scroll);
  }
  sendCommand(0x00);       //Dummmy byte
  sendCommand(startRow);
  sendCommand(scrollSpeed);
  sendCommand(endRow);
  sendCommand(startColumn+8);
  sendCommand(endColumn+8);
  sendCommand(0x00);      //Dummmy byte

}

void ArduiPi_OLED::stopscroll(void)
{
  sendCommand(SSD_Deactivate_Scroll);
}

void ArduiPi_OLED::sendData(uint8_t c)
{
  uint8_t buff[2] ;

  // Setup D/C to switch to data mode
  buff[0] = SSD_Data_Mode;
  buff[1] = c;

#if SLOW_WRITE
  // Write on i2c
  lcd_dev_write( buff, 2) ;
#else
  addxmit(buff,4);
#endif
}

void ArduiPi_OLED::display(void)
{

  if (oled_type == OLED_SEEED_I2C_96x96 )
  {
    sendCommand(SSD1327_Set_Row_Address   , 0x00, 0x5F);
    sendCommand(SSD1327_Set_Column_Address, 0x08, 0x37);
  }
  else
  {
    sendCommand(SSD1306_Set_Lower_Column_Start_Address  | 0x0); // low col = 0
    sendCommand(SSD1306_Set_Higher_Column_Start_Address | 0x0); // hi col = 0
    sendCommand(SSD1306_Set_Start_Line  | 0x0); // line #0
  }
#if !SLOW_WRITE
  xmitflush();
#endif

  uint16_t i=0 ;

  // pointer to OLED data buffer
  uint8_t * p = poledbuff;

  uint8_t buff[17] ;
  uint8_t x ;

  // Setup D/C to switch to data mode
  buff[0] = SSD_Data_Mode;

  if (oled_type == OLED_SH1106_I2C_128x64)
  {
    for (uint8_t k=0; k<8; k++)
    {
      sendCommand(0xB0+k);//set page address SSD_Data_Mode;
      sendCommand(0x02) ;//set lower column address
      sendCommand(0x10) ;//set higher column address

     for( i=0; i<8; i++)
     {
        for (x=1; x<=16; x++)
          buff[x] = *p++;

#if SLOW_WRITE
        lcd_dev_write(buff, 17);
#else
        addxmit(buff,17);
#endif
      }
    }
  }
  else
  {
    // loop through all OLED buffer and
    // send a bunch of 16 data byte in one xmission
    for ( i=0; i<oled_buff_size; i+=16 )
    {
      for (x=1; x<=16; x++)
        buff[x] = *p++;

#if SLOW_WRITE
      lcd_dev_write(buff, 17);
#else
      addxmit(buff,17);
      if(i%16==0)
        xmitflush();  //!! this sucks
#endif
    }
  }
#if !SLOW_WRITE
  xmitflush();
#endif
}

// Save the display's buffer as a PBM
boolean ArduiPi_OLED::SaveToPBM(const char *fn){
  FILE *f = fopen(fn, "w");
  int16_t x,y;

  if(!f)
    return false;

  fprintf(f, "P1\n%d %d\n", width(), height());


  for(y=0;y<height();y++){
    for(x=0;x<width();x++){
      if(!(x%8))
        fputc(' ',f);
      fputc(getPixel(x,y) ? '0':'1', f);
    }
    fputc('\n',f);
  }

  fclose(f);
  return true;
}

// clear everything (in the buffer)
void ArduiPi_OLED::clearDisplay(void)
{
  memset(poledbuff, 0, oled_buff_size);
}

const char *ArduiPi_OLED::getOledDisplayName(uint8_t id)
{
  static const char * oled_type_str[] = {
    "SSD1306 (Adafruit) SPI 128x32",  /* unsupported */
    "SSD1306 (Adafruit) SPI 128x64",  /* unsupported */
    "SSD1306 (Adafruit) I2C 128x32",
    "SSD1306 (Adafruit) I2C 128x64",
    "SSD1308 (Seeed) I2C 128x64",
    "SSD1308 (Seeed) I2C 96x96",
    "SH1106 I2C 128x64"
  };
  return oled_type_str[id];
}
