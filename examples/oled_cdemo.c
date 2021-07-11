/*************
 * This in an to use a monochrome OLEDs from C language.
 *
 * Support Adafruit that provided the original library and visit
 *   http://www.adafruit.com/category/63_98
 * to buy the related hardware.
 *
 *  Written by Destroyedlolo (http://destroyedlolo.info)
 *  BSD license, check license.txt for more information
 * All text above must be included in any redistribution
 *
 * 24/12/2018  Destroyedlolo (http://destroyedlolo.info)
 *     Port from oled_demo.cpp
 * 30/12/2018  Destroyedlolo (http://destroyedlolo.info)
 *     Add Flip() test
 * 25/09/2020  JG1UAA (http://github.com/jg1uaa)
 *    opts.oled and opts.port is defined as symbol
 **********/

#include "ArduiPi_OLED_C.h"

#include <getopt.h>
#include <stdbool.h>

#define PRG_NAME        "oled_demoC"
#define PRG_VERSION     "1.0"

struct s_opts {
  int oled;
  bool verbose;
  const char *port;
};

struct s_opts opts = {
  OLED_TYPE,      // Default oled
  true,           // Not verbose
  I2C_DEV         // default port
};


/****
 * All the tests
 ****/

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static unsigned char logo16_glcd_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void testsimplebitmap(){
  PiOLED_ClearDisplay();
  PiOLED_DrawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  PiOLED_Display();
  sleep(1);
}

void testinvert(){
  PiOLED_Invert(true);
  sleep(1);
  PiOLED_Invert(false);
  sleep(1);
}

void testflip(){
  if(opts.verbose)
    puts("Flip tests\n"
      "----------");
  PiOLED_Flip(true);
  sleep(2);
  PiOLED_Flip(false);
  sleep(1);
}

void testmessages(){
  PiOLED_ClearDisplay();
  PiOLED_SetTextSize(2);
  PiOLED_SetTextColor(WHITE);

  PiOLED_SetCursor(0,0);
  PiOLED_Print("Hello,\n   world!\n");
  PiOLED_SetTextColor2(BLACK, WHITE); // 'inverted' text
  PiOLED_Printf("%f\n", 3.141592);
  PiOLED_SetTextColor(WHITE);
  PiOLED_Printf("0x%8X\n", 0xDEADBEEF);

  PiOLED_Display();
  sleep(5);
}

void testscrolltext(){
  PiOLED_ClearDisplay();
  PiOLED_SetTextSize(2);
  PiOLED_SetTextColor(WHITE);
  PiOLED_SetCursor(10,0);

  if(opts.oled == OLED_SH1106_I2C_128x64)
    PiOLED_Print("No scroll\non SH1106");
  else
    PiOLED_Print("scroll");
  PiOLED_Display();

  PiOLED_StartScrollRight(0x00, 0x0F);
  sleep(2);
  PiOLED_StopScroll();
  sleep(1);
  PiOLED_StartScrollLeft(0x00, 0x0F);
  sleep(2);
  PiOLED_StopScroll();
  sleep(1);
  PiOLED_StartScrollDiagRight(0x00, 0x07);
  sleep(2);
  PiOLED_StartScrollDiagLeft(0x00, 0x07);
  sleep(2);
  PiOLED_StopScroll();

  sleep(2);
}

void testdrawchar(){
  uint8_t i;

  if(opts.verbose)
    puts("Text drawing tests\n"
       "------------------");

  PiOLED_ClearDisplay();

  PiOLED_SetTextSize(1);
  PiOLED_SetTextColor(WHITE);
  PiOLED_SetCursor(0,0);

  for(i=0; i<168; i++){
    if(i == '\n')
      continue;
    PiOLED_Write(i);
    if( i && !(i % 20))
      PiOLED_Print("\n");
  }
  PiOLED_Display();
  sleep(2);
}

void testdrawline(){
  int16_t i;

  if(opts.verbose)
    puts("Drawline tests\n"
       "--------------");

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayWidth(); i+=4){
    PiOLED_DrawLine(0, 0, i, PiOLED_DisplayHeight()-1, WHITE);
    PiOLED_Display();
  }
  for(i=0; i<PiOLED_DisplayHeight(); i+=4) {
    PiOLED_DrawLine(0, 0, PiOLED_DisplayWidth()-1, i, WHITE);
    PiOLED_Display();
    }
  sleep(1);

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayWidth(); i+=4){
    PiOLED_DrawLine(0, PiOLED_DisplayHeight()-1, i, 0, WHITE);
    PiOLED_Display();
  }
  for(i=PiOLED_DisplayHeight()-1; i>=0; i-=4){
    PiOLED_DrawLine(0, PiOLED_DisplayHeight()-1, PiOLED_DisplayWidth()-1, i, WHITE);
    PiOLED_Display();
  }
  sleep(1);

  PiOLED_ClearDisplay();
  for(i=PiOLED_DisplayWidth()-1; i>=0; i-=4){
    PiOLED_DrawLine(PiOLED_DisplayWidth()-1, PiOLED_DisplayHeight()-1, i, 0, WHITE);
    PiOLED_Display();
  }
  for(i=PiOLED_DisplayHeight()-1; i>=0; i-=4) {
    PiOLED_DrawLine(PiOLED_DisplayWidth()-1, PiOLED_DisplayHeight()-1, 0, i, WHITE);
    PiOLED_Display();
    }
  sleep(1);

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayHeight(); i+=4){
    PiOLED_DrawLine(PiOLED_DisplayWidth()-1, 0, 0, i, WHITE);
    PiOLED_Display();
  }
  for(i=PiOLED_DisplayWidth(); i>=0; i-=4){
    PiOLED_DrawLine(PiOLED_DisplayWidth()-1, 0, i, PiOLED_DisplayHeight()-1, WHITE);
    PiOLED_Display();
  }
  sleep(1);
}

void testdrawrect(){
  int16_t i;

  if(opts.verbose)
    puts("Rectangle tests\n"
       "---------------");

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayHeight()/2; i+=2){
    PiOLED_DrawRect(i, i, PiOLED_DisplayWidth()-2*i, PiOLED_DisplayHeight()-2*i, WHITE);
    PiOLED_Display();
  }
  sleep(1);
}

void testfillrect(){
  int16_t i;
  uint8_t color = 1;

  if(opts.verbose)
    puts("Filled Rectangle tests\n"
       "----------------------");

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayHeight()/2; i+=2){
    PiOLED_FillRect(i, i, PiOLED_DisplayWidth()-2*i, PiOLED_DisplayHeight()-2*i, color++%2);
    PiOLED_Display();
  }
  sleep(1);
}

void testdrawroundrect(){
  int16_t i;

  if(opts.verbose)
    puts("Rounded Rectangle tests\n"
       "-----------------------");

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayHeight()/2-2; i+=4){
    PiOLED_DrawRoundRect(i, i, PiOLED_DisplayWidth()-2*i, PiOLED_DisplayHeight()-2*i, PiOLED_DisplayHeight()/4, WHITE);
    PiOLED_Display();
  }
  sleep(1);
}

void testfillroundrect(){
  int16_t i;
  uint8_t color = 1;

  if(opts.verbose)
    puts("Filled Rounded Rectangle tests\n"
       "------------------------------");

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayHeight()/2-2; i+=4){
    PiOLED_FillRoundRect(i, i, PiOLED_DisplayWidth()-2*i, PiOLED_DisplayHeight()-2*i, PiOLED_DisplayHeight()/4, color++%2);
    PiOLED_Display();
  }
  sleep(1);
}

void testdrawtriangle(){
  int16_t i;

  if(opts.verbose)
    puts("Triangles tests\n"
       "---------------");

  PiOLED_ClearDisplay();
  for (i=min(PiOLED_DisplayWidth(),PiOLED_DisplayHeight())/2; i>0; i-=5){
    PiOLED_DrawTriangle(
      PiOLED_DisplayWidth()/2, PiOLED_DisplayHeight()/2-i,
      PiOLED_DisplayWidth()/2-i, PiOLED_DisplayHeight()/2+i,
      PiOLED_DisplayWidth()/2+i, PiOLED_DisplayHeight()/2+i, WHITE
    );
    PiOLED_Display();
  }
  sleep(1);
}

void testfilltriangle(){
  int16_t i;
  uint8_t color = 1;

  if(opts.verbose)
    puts("Filled Triangles tests\n"
       "----------------------");

  PiOLED_ClearDisplay();
  for (i=min(PiOLED_DisplayWidth(),PiOLED_DisplayHeight())/2; i>0; i-=5){
    PiOLED_FillTriangle(
      PiOLED_DisplayWidth()/2, PiOLED_DisplayHeight()/2-i,
      PiOLED_DisplayWidth()/2-i, PiOLED_DisplayHeight()/2+i,
      PiOLED_DisplayWidth()/2+i, PiOLED_DisplayHeight()/2+i, color++%2
    );
    PiOLED_Display();
  }
  sleep(1);
}

void testdrawcircle(){
  int16_t i;

  if(opts.verbose)
    puts("Circle tests\n"
       "------------");

  PiOLED_ClearDisplay();
  for(i=0; i<PiOLED_DisplayHeight(); i+=2){
    PiOLED_DrawCircle(PiOLED_DisplayWidth()/2, PiOLED_DisplayHeight()/2, i, WHITE);
    PiOLED_Display();
  }
  sleep(1);
}

void testfillcircle(){
  int16_t i;
  uint8_t color = 1;

  if(opts.verbose)
    puts("Filled Circle tests\n"
       "-------------------");

  PiOLED_ClearDisplay();
  for(i=PiOLED_DisplayHeight(); i>=5; i-=4){
    PiOLED_FillCircle(PiOLED_DisplayWidth()/2, PiOLED_DisplayHeight()/2, i, color++%2);
    PiOLED_Display();
  }
  sleep(1);

}

void testhbar(){
  int i;

  PiOLED_SetTextSize(1);

  for(i=0; i<=100; i++){
    PiOLED_ClearDisplay();
    PiOLED_SetTextColor2(BLACK,WHITE);
    PiOLED_SetCursor(0,0);
    PiOLED_Print("Gauge Graph!");
    PiOLED_SetTextColor(WHITE);
    PiOLED_SetCursor(20,10);
    PiOLED_Printf("%03d %%", i);
    PiOLED_DrawHorizontalBargraph(0,20, (int16_t) PiOLED_DisplayWidth(),10,1, i);
    PiOLED_Display();
  }

  sleep(1);
}

void testvbar(){
  int i;

  PiOLED_SetTextSize(1);

  for(i=0; i<=100; i++){
    PiOLED_ClearDisplay();
    PiOLED_SetTextColor2(BLACK,WHITE);
    PiOLED_SetCursor(0,0);
    PiOLED_Print("Gauge Graph!");
    PiOLED_SetTextColor(WHITE);
    PiOLED_SetCursor(20,10);
    PiOLED_Printf("%03d %%", i);
    PiOLED_DrawVerticalBargraph(114,0,8,(int16_t) PiOLED_DisplayHeight(),1, i);
    PiOLED_Display();
  }

  sleep(1);
}

/****
 *  Parse arguments
 ****/
void usage( char * name){
  printf("%s\n", name );
  printf("Usage is: %s --oled type [options]\n", name);
  puts("  --<o>led type\nOLED type are:");
  for (int i=0; i<OLED_LAST_OLED;i++)
    printf("  %1d %s\n", i, PiOLED_GetOledDisplayName(i));
  puts("NOTEZ-BIEN : SPI displays are not supported yet\n");

  puts("Options are:");
  puts("  --<v>erbose  : speak more to user");
  puts("  --<h>elp");
  puts("<?> indicates the equivalent short option.");
  puts("Short options are prefixed by \"-\" instead of by \"--\".");
  puts("Example :");
  printf( "%s -o 1    --> use a %s OLED\n", name, PiOLED_GetOledDisplayName(1));
  printf( "%s -o 4 -v --> use a %s OLED being verbose\n", name, PiOLED_GetOledDisplayName(4));

}

void parse_args(int argc, char *argv[]){
  static struct option longOptions[] = {
    {"oled", required_argument, 0, 'o'},
    {"port", required_argument, 0, 'p'},
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  for(;;){
    /* no default error messages printed. */
    int optionIndex;
    int c = getopt_long(argc, argv, "vho:p:", longOptions, &optionIndex);
    if (c < 0)
      break;

    switch(c){
    case 'p':
      if(!( opts.port = strdup(optarg) )){
        fprintf(stderr,"Can't duplicate port ... Leaving");
        exit( EXIT_FAILURE );
      }
      break;
    case 'v':
      opts.verbose = true;
      break;
    case 'o':
      opts.oled = atoi(optarg);
      if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED ){
        fprintf(stderr, "--oled %d ignored must be 0 to %d.\n", opts.oled, OLED_LAST_OLED-1);
        fprintf(stderr, "--oled set to 1 now\n");
        opts.oled = 1;
      }
      break;
    case 'h':
      usage(argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case '?':
    default:
      fprintf(stderr, "Unrecognized option.\n");
      fprintf(stderr, "Run with '--help'.\n");
      exit(EXIT_FAILURE);
    }
  }

  if(opts.verbose){
    printf("%s v%s\n", PRG_NAME, PRG_VERSION);
    printf("-- OLED params -- \n");
    printf("Oled is    : %s\n", PiOLED_GetOledDisplayName(opts.oled));
    printf("On     : %s\n", opts.port);
    printf("-- Other Stuff -- \n");
    printf("verbose is : %s\n", opts.verbose? "yes" : "no");
    printf("\n");
  }
}


/*****
 * Let's go
 *****/

int main( int ac, char **av ){
  parse_args( ac, av );

  if( !PiOLED_Init(opts.oled, opts.port) )
    exit(EXIT_FAILURE);

#if 0
  /* Laurent : I don't own this device and specifics functions are not yet
   * present in the wrapper.
   * To avoid loosing time, I disable this code.
   * Don't hesitate to participate :)
   */
  if (opts.oled == 5) {
    // showing on this display is very slow (the driver need to be optimized)
    sleep_divisor = 4;

    for(char i=0; i < 12 ; i++){
      display.setSeedTextXY(i,0);  //set Cursor to ith line, 0th column
      display.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
      display.putSeedString("Hello World"); //Print Hello World
    }
    sleep(2);
  }
#endif

  testmessages();
  testflip();
  testdrawchar();
  testscrolltext();
  testdrawline();
  testdrawrect();
  testfillrect();
  testdrawroundrect();
  testfillroundrect();
  testdrawtriangle();
  testfilltriangle();
  testdrawcircle();
  testfillcircle();
  testhbar();
  testvbar();
  testsimplebitmap();
  testinvert();

  PiOLED_OnOff(0);  // Save display life

  PiOLED_Close();  // Free resources
}
