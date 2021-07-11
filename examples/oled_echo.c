/**
 * oled_echo
 *
 * Displays command line argument string on the OLED display.
 * This is useful in a "headless" system where the only visual output device
 * is the OLED itself.  System utilities can use this to display short
 * messages to the user - for example, during system update.
 *
 * Example usage:
 * oled_echo -s2 -w3 -jl "hello\nworld"
 * oled_echo -s0 -w3 -jl "hello\nworld"
 * oled_echo -s0 -w3 -jc "hello\nworld"
 * oled_echo -s0 -w3 -jr "hello\nworld"
 * oled_echo -s0 -w3 -jr "hello\nyou"
 * oled_echo -s0 -w3 -jc "hello\nyou"
 * oled_echo -s0 -w3 -jc "hello\nyou"
 * oled_echo -s0 -w3 -jc "hello\nyou"
 * oled_echo -s1 -w3 -jc "hello\nyou"
 * oled_echo -s1 -w1 -jc "hello\nyou"
 * oled_echo -s1 -w1 -jc "hello\nyou"
 * oled_echo -c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#include "ArduiPi_OLED_C.h"

// Defines
// =======
#define PRG_NAME        "oled_echo"
#define PRG_VERSION     "1.0"

#define MAX_LINES     10

#define SCREEN_WIDTH  PiOLED_DisplayWidth()
#define SCREEN_HEIGHT PiOLED_DisplayHeight()

#define ASIZE(x)  (sizeof(x)/sizeof(*x))

// Types
// =====
struct s_opts {
  int oled;
  const char *port;
};

typedef struct {
  int fontsize;
  int exitdelay;
  char justification;
  const char *message;
  int clear;
} progOptions_t;


typedef enum {
  ex_ok = EXIT_SUCCESS,
  ex_bad_args,  // command line params don't parse
  ex_range,     // value out of range, especially size
  ex_wont_fit,  // text won't fit the display
  ex_bad_device,// connection to OLED failed
} exitcode_t;

// Const data
// ==========
static const struct s_opts opts = {
  OLED_TYPE,      // Default oled
  I2C_DEV         // default port
};

static const struct {
  int width;
  int height;
} fontsizes[] = {
  { 6, 8 },       // 0
  { 6, 8 },       // 1
  { 12, 16 },
  { 18, 24 },     // 3
  { 24, 32 },
  { 30, 40 },     // 5
  { 36, 48 },
  { 42, 56 },     // 7
  { 48, 64 },
  { 56, 64 },     // 9 (not really 64 high, but it does fit on the display.
};

// Functions
// =========
exitcode_t openDevice(void) {
  exitcode_t result = ex_ok;
  if( !PiOLED_Init(opts.oled, opts.port) ) {
    fprintf(stderr,"Unable to connect OLED type %d to %s\n",
            opts.oled, opts.port);
    result = ex_bad_device;
  }
  return result;
}

void closeDevice(void) {
  PiOLED_Close();
}


size_t splitIntoLines(const char *line, char *lines[], size_t maxlines ) {
  size_t nLines = 0;
  const char *p = line;
  size_t len;
  while( nLines < (maxlines-1) &&
         (p=strstr(line,"\\n")) != NULL ) {
    len = p - line;
    lines[nLines] = calloc(1,len+1);
    strncpy(lines[nLines++], line, len);
    line = p + 2;
  }
  len = strlen(line);
  lines[nLines] = calloc(1,len+1);
  strncpy(lines[nLines++], line, len);
  return nLines;
}

void freeSplitLines(char *lines[], size_t nLines ) {
  for( size_t i = 0 ; i < nLines ; i++ ) {
    free(lines[i]);
  }
}

void clear(void) {
  PiOLED_ClearDisplay();
}

exitcode_t show(const progOptions_t *opts) {
  exitcode_t result = ex_ok;
  char *lines[MAX_LINES];

  // break string into lines by counting \n
  size_t nLines = splitIntoLines(opts->message,lines,MAX_LINES);

  // max length of all the lines
  size_t len, maxlen = 0;
  for( size_t i = 0 ; i < nLines ; i++ ) {
    len = strlen(lines[i]);
    maxlen = len > maxlen ? len : maxlen;
  }

  if( opts->fontsize < 0 || opts->fontsize > ASIZE(fontsizes) ) {
    fprintf(stderr,"Font size out of range size=0,%zd\n",
            ASIZE(fontsizes));
    result = ex_range;
  }
  if( !(opts->justification == 'l' ||
        opts->justification == 'c' ||
        opts->justification == 'r') ) {
    fprintf(stderr,"justification not one of left|centre|right\n");
    result = ex_range;
  }

  // max length of any line, * font_width <= screen width
  // number of lines * font_height <= screen height
  if( maxlen * fontsizes[opts->fontsize].width > SCREEN_WIDTH ||
      nLines * fontsizes[opts->fontsize].height > SCREEN_HEIGHT ) {
    fprintf(stderr,"Font metric=%d,%d; calc size=%d,%d\n",
            fontsizes[opts->fontsize].width,
            fontsizes[opts->fontsize].height,
            maxlen * fontsizes[opts->fontsize].width,
            nLines * fontsizes[opts->fontsize].height);
    result = ex_wont_fit;
  }

  if( result == ex_ok ) {
    // everything is centred vertically on screen
    int start_y = SCREEN_HEIGHT / 2 - (nLines * fontsizes[opts->fontsize].height) / 2;
    PiOLED_SetTextColor(WHITE);
    PiOLED_SetTextSize(opts->fontsize);
    for( size_t i = 0 ; i < nLines ; i++ ) {
      size_t len = strlen(lines[i]);
      int pixlen = len * fontsizes[opts->fontsize].width;
      int start_x = 0;
      switch( opts->justification ) {
        case 'l': start_x = 0 ; break;
        case 'c': start_x = ( SCREEN_WIDTH - pixlen ) / 2 ; break;
        case 'r': start_x = SCREEN_WIDTH - pixlen; break;
      }
      PiOLED_SetCursor(start_x,start_y);
      PiOLED_Print(lines[i]);
      start_y += fontsizes[opts->fontsize].height;
    }
    PiOLED_Display();
    if( opts->exitdelay > 0 ) {
      sleep(opts->exitdelay);
    }
  }

  freeSplitLines(lines,nLines);
  return result;
}

exitcode_t getIntArg(const char *arg, int *p ) {
  exitcode_t result = ex_ok;
  char *endp = NULL;
  errno = 0;
  long val = strtol(arg, &endp, 0);
  if ( errno != 0 || endp == arg ) {
    fprintf(stderr,"%s is not an integer\n",arg);
    result = ex_bad_args;
  } else {
    *p = (int)val;
  }
  return result;
}

/*
 * prog [-wn] [-sn] [-j[lcr]] "message"
 * prog [-c]
 */
exitcode_t doWork(int argc, char *argv[]) {
  exitcode_t result = ex_ok;
  progOptions_t progOptions = {
    0, 0, 'l', 0, 0
  };
  int opt;
  while( result == ex_ok &&
         (opt = getopt(argc, argv, "w:s:cj:")) != -1) {
    switch(opt) {
      case 'w':
        result = getIntArg(optarg, &progOptions.exitdelay);
        break;
      case 's':
        result = getIntArg(optarg, &progOptions.fontsize);
        break;
      case 'c':
        progOptions.clear = 1;
        break;
      case 'j':
        progOptions.justification = tolower(optarg[0]);
        break;
      default:
        fprintf(stderr,"Usage: %s [-c] [-wn] [-sn] [-j[lcr]] \"message\"\n",
                argv[0]);
        result = ex_bad_args;
    }
  }
  if ( optind < argc ) {
    progOptions.message = argv[optind];
  }
//   printf("Options: %d %d %d %c\n%s\n",
//           progOptions.fontsize, progOptions.exitdelay,
//           progOptions.clear, progOptions.justification,
//           progOptions.message);

  result = openDevice();
  if(result == ex_ok) {
    if( progOptions.clear ) {
      clear();
    } else {
      result = show(&progOptions);
    }
    closeDevice();
  }
  return result;
}

int main ( int argc, char *argv[] ) {
  exitcode_t result = ex_ok;
  result = doWork(argc,argv);
  return result;
}
