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

	--- European time format ---
26/08/2015	Lorenzo Delana <lorenzo.delana@gmail.com>
			Wrappers for functions export.

24/12/2018	Destroyedlolo (http://destroyedlolo.info)
			Change prefix to PiOLED which is more explanatory.

*********************************************************************/

#include "./ArduiPi_OLED_C.h"

#include "./Adafruit_GFX.h"
#include "./ArduiPi_OLED.h"

ArduiPi_OLED *extDisplay = NULL;

// Initialize the wrapper ( allocate the ArduiPi_OLED object and init display hardware type ).
extern "C" boolean PiOLED_Init(int oledType, const char *dev){
	if (extDisplay != NULL)	// Already done
		return 1;

	if (oledType < 0 || oledType >= OLED_LAST_OLED)
		oledType = 3;

	extDisplay = new ArduiPi_OLED();

    if (!extDisplay->init(oledType, dev))
		return 0;
    
	extDisplay->begin();

	// init done
	extDisplay->clearDisplay();   // clears the screen  buffer
	extDisplay->display();   		// display it (clear display)

	return 1;
}

extern "C" void PiOLED_Close(){
	extDisplay->close();
}

extern "C" void PiOLED_Display(){
	extDisplay->display();
}

extern "C" void PiOLED_ClearDisplay(){
	extDisplay->clearDisplay();
}

extern "C" void PiOLED_Invert( boolean i ){
	extDisplay->invertDisplay(i);
}

extern "C" void PiOLED_DrawPixel(int16_t x, int16_t y, uint16_t color){
	extDisplay->drawPixel(x, y, color);
}

extern "C" void PiOLED_StartScrollLeft(uint8_t start, uint8_t stop){
	extDisplay->startscrollleft(start, stop);
}

extern "C" void PiOLED_StartScrollRight(uint8_t start, uint8_t stop){
	extDisplay->startscrollright(start, stop);
}

extern "C" void PiOLED_StartScrollDiagLeft(uint8_t start, uint8_t stop){
	extDisplay->startscrolldiagleft(start, stop);
}

extern "C" void PiOLED_StartScrollDiagRight(uint8_t start, uint8_t stop){
	extDisplay->startscrolldiagright(start, stop);
}

extern "C" void PiOLED_SetHorizontalScrollProperties(boolean direction, uint8_t startRow, uint8_t endRow, uint8_t startColumn, uint8_t endColumn, uint8_t scrollSpeed){
	extDisplay->setHorizontalScrollProperties(direction, startRow, endRow, startColumn, endColumn, scrollSpeed);
}

extern "C" void PiOLED_StopScroll(){
	extDisplay->stopscroll();
}

extern "C" void PiOLED_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
	extDisplay->drawCircle(x0, y0, r, color);
}

extern "C" void PiOLED_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color){
	extDisplay->drawCircleHelper(x0, y0, r, cornername, color);
}

extern "C" void PiOLED_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
	extDisplay->fillCircle(x0, y0, r, color);
}

extern "C" void PiOLED_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color){
	extDisplay->fillCircleHelper(x0, y0, r, cornername, delta, color);
}

extern "C" void PiOLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){
	extDisplay->drawLine(x0, y0, x1, y1, color);
}

extern "C" void PiOLED_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	extDisplay->drawRect(x, y, w, h, color);
}

extern "C" void PiOLED_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	extDisplay->fillRect(x, y, w, h, color);
}

extern "C" void PiOLED_DrawVerticalBargraph(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t percent){
	extDisplay->drawVerticalBargraph(x, y, w, h, color, percent);
}

extern "C" void PiOLED_DrawHorizontalBargraph(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t percent){
	extDisplay->drawHorizontalBargraph(x, y, w, h, color, percent);
}

extern "C" void PiOLED_FillScreen(uint16_t color){
	extDisplay->fillScreen(color);
}

extern "C" void PiOLED_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color){
	extDisplay->drawRoundRect(x, y, w, h, r, color);
}

extern "C" void PiOLED_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color){
	extDisplay->fillRoundRect(x, y, w, h, r, color);
}

extern "C" void PiOLED_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
	extDisplay->drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

extern "C" void PiOLED_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
	extDisplay->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

extern "C" void PiOLED_DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
	extDisplay->drawBitmap(x, y, bitmap, w, h, color);
}

extern "C" void PiOLED_Write(byte c){
	extDisplay->write(c);
}

extern "C" void PiOLED_Print(const char *string){
	extDisplay->print(string);
}

extern "C" void PiOLED_Printf(const char *format, ...){
	va_list args;
	va_start (args, format);

	extDisplay->vprintf(format, args);

	va_end (args);
}

extern "C" void PiOLED_DrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size){
	extDisplay->drawChar(x, y, c, color, bg, size);
}

extern "C" void PiOLED_SetCursor(int16_t x, int16_t y){
	extDisplay->setCursor(x, y);
}

extern "C" void PiOLED_SetTextSize(uint8_t s){
	extDisplay->setTextSize(s);
}

extern "C" void PiOLED_SetTextColor(uint16_t c){
	extDisplay->setTextColor(c);
}

extern "C" void PiOLED_SetTextColor2(uint16_t c, uint16_t b){
	extDisplay->setTextColor(c,b);
}

extern "C" void PiOLED_SetTextWrap(boolean w){
	extDisplay->setTextWrap(w);
}

extern "C" int16_t PiOLED_DisplayWidth(){
	return extDisplay->width();
}

extern "C" int16_t PiOLED_DisplayHeight(){
	return extDisplay->height();
}
