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

************

Wrapper to C language

	--- European time format ---
24/12/2018	Destroyedlolo (http://destroyedlolo.info)
	Add this missing header
25/12/2018	Destroyedlolo (http://destroyedlolo.info)
	Add OnOff()
28/12/2018	Destroyedlolo (http://destroyedlolo.info)
	expose PiOLED_sendCommand() as well
*********************************************************************/

#ifndef ARDUIPI_OLED_C_H
#define ARDUIPI_OLED_C_H

#include "ArduiPi_OLED_lib.h"

#define BLACK 0
#define WHITE 1

#ifdef __cplusplus
extern "C" {
#endif

extern boolean PiOLED_Init(int oledType, const char *dev);
extern void PiOLED_sendCommand(uint8_t c);
extern void PiOLED_Close();
extern void PiOLED_Display();
extern void PiOLED_ClearDisplay();
extern void PiOLED_Invert( boolean );
extern void PiOLED_OnOff( boolean );
extern void PiOLED_Flip( boolean );
extern boolean SaveToPBM(const char *);

extern void PiOLED_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
extern void PiOLED_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
extern void PiOLED_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
extern void PiOLED_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
extern void PiOLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
extern void PiOLED_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
extern void PiOLED_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
extern void PiOLED_DrawVerticalBargraph(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t percent);
extern void PiOLED_DrawHorizontalBargraph(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t percent);
extern void PiOLED_FillScreen(uint16_t color);
extern void PiOLED_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
extern void PiOLED_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
extern void PiOLED_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void PiOLED_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void PiOLED_DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
extern void PiOLED_Write(byte c);
extern void PiOLED_Print(const char *string);
extern void PiOLED_Printf(const char *format, ...);
extern void PiOLED_DrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
extern void PiOLED_SetCursor(int16_t x, int16_t y);
extern void PiOLED_SetTextSize(uint8_t s);
extern void PiOLED_SetTextColor(uint16_t c);
extern void PiOLED_SetTextColor2(uint16_t c, uint16_t b);
extern void PiOLED_SetTextWrap(boolean w);
extern void PiOLED_DrawPixel(int16_t x, int16_t y, uint16_t color);
extern void PiOLED_getPixel(int16_t x, int16_t y);
extern void PiOLED_StartScrollLeft(uint8_t start, uint8_t stop);
extern void PiOLED_StartScrollRight(uint8_t start, uint8_t stop);
extern void PiOLED_StartScrollDiagLeft(uint8_t start, uint8_t stop);
extern void PiOLED_StartScrollDiagRight(uint8_t start, uint8_t stop);
extern void PiOLED_SetHorizontalScrollProperties(boolean direction, uint8_t startRow, uint8_t endRow, uint8_t startColumn, uint8_t endColumn, uint8_t scrollSpeed);
extern void PiOLED_StopScroll();
extern int16_t PiOLED_DisplayWidth();
extern int16_t PiOLED_DisplayHeight();

#ifdef __cplusplus
}
#endif

#endif

