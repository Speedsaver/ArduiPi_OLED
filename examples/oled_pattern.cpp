/*********************************************************************
 * This example demonstrates usage of patterns in graphical functions
 *
 * 07/08/2019 Destroyedlolo (http://destroyedlolo.info)
 *	First version
 *********************************************************************/

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#include <getopt.h>
#include <time.h>

#define PRG_NAME        "oled_pattern"
#define PRG_VERSION     "1.0"

// Instantiate the display
ArduiPi_OLED display;

// Config Option
struct s_opts
{
	int oled;
	int verbose;
	const char *port;
};

struct s_opts opts = {
	OLED_ADAFRUIT_I2C_128x64,	// Default oled
	true,					// Not verbose
	"/dev/i2c-2"			// default port
};

int sleep_divisor = 1 ;

void testdrawline(){
	display.clearDisplay();
	for (int16_t i=0; i<display.width(); i+=4) {
		display.drawLine(0, 0, i, display.height()-1, WHITE, 0xaaaa);
		display.display();
	}
	for (int16_t i=0; i<display.height(); i+=4) {
		display.drawLine(0, 0, display.width()-1, i, WHITE, 0x8888);
		display.display();
	}
	usleep(250000/sleep_divisor);
}

void testdrawrect(void){
	display.clearDisplay();
	for (int16_t i=0; i<display.height()/2; i+=2) {
		display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE, 0xaaaa);
		display.display();
	}
	usleep(250000/sleep_divisor);
}

void testfillrect(void) {
	display.clearDisplay();
	display.fillRect( 5,5, display.width()-10, display.height()-10, WHITE, 0x8888);
	display.display();
	usleep(250000/sleep_divisor);
}

void testdrawcircle(void) {
	uint16_t pat=0;
	display.clearDisplay();
	for (int16_t i=0; i<display.height(); i+=2) {
		if(pat == 0xaaaa)
			pat=0x8888;
		else
			pat=0xaaaa;

		display.drawCircle(display.width()/2, display.height()/2, i, WHITE, pat);
		display.display();
	}
	usleep(250000/sleep_divisor);
}

/* ======================================================================
Function: usage
Purpose : display usage
Input 	: program name
Output	: -
Comments: 
====================================================================== */
void usage( char * name)
{
	printf("%s\n", name );
	printf("Usage is: %s --oled type [options]\n", name);
	printf("  --<o>led type\nOLED type are:\n");
	for (int i=0; i<OLED_LAST_OLED;i++)
		printf("  %1d %s\n", i, oled_type_str[i]);
	
	printf("Options are:\n");
	printf("  --<v>erbose  : speak more to user\n");
	printf("  --<h>elp\n");
	printf("<?> indicates the equivalent short option.\n");
	printf("Short options are prefixed by \"-\" instead of by \"--\".\n");
	printf("Example :\n");
	printf( "%s -o 1 use a %s OLED\n\n", name, oled_type_str[1]);
	printf( "%s -o 4 -v use a %s OLED being verbose\n", name, oled_type_str[4]);
}

/* ======================================================================
Function: parse_args
Purpose : parse argument passed to the program
Input 	: -
Output	: -
Comments: 
====================================================================== */
void parse_args(int argc, char *argv[])
{
	static struct option longOptions[] =
	{
		{"oled", required_argument, 0, 'o'},
		{"port", required_argument, 0, 'p'},
		{"verbose", no_argument, 0, 'v'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	int optionIndex = 0;
	int c;

	while(1){
		/* no default error messages printed. */
		opterr = 0;

		c = getopt_long(argc, argv, "vho:p:", longOptions, &optionIndex);
		if (c < 0)
			break;

		switch(c){
			case 'p': 
				opts.port = strdup(optarg);
				if(!opts.port){
					fprintf(stderr,"Can't duplicate port ... Leaving");
					exit( EXIT_FAILURE );
				}
				break;
			case 'v': opts.verbose = true	;	break;
			case 'o':
				opts.oled = (int) atoi(optarg);
				if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED )
				{
						fprintf(stderr, "--oled %d ignored must be 0 to %d.\n", opts.oled, OLED_LAST_OLED-1);
						fprintf(stderr, "--oled set to 0 now\n");
						opts.oled = 0;
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
	} /* while */

	if (opts.verbose)
	{
		printf("%s v%s\n", PRG_NAME, PRG_VERSION);
		printf("-- OLED params -- \n");
		printf("Oled is    : %s\n", oled_type_str[opts.oled]);
		printf("On 		: %s\n", opts.port);
		printf("-- Other Stuff -- \n");
		printf("verbose is : %s\n", opts.verbose? "yes" : "no");
		printf("\n");
	}	
}


/* ======================================================================
Function: main
Purpose : Main entry Point
Input 	: -
Output	: -
Comments: 
====================================================================== */
int main(int argc, char **argv){

	// Oled supported display in ArduiPi_SSD1306.h
	// Get OLED type
	parse_args(argc, argv);

	if(!display.init(opts.oled, opts.port))
			exit(EXIT_FAILURE);
	display.begin();

	if (opts.oled == 5){
		// showing on this display is very slow (the driver need to be optimized)
		sleep_divisor = 4;

		for(char i=0; i < 12 ; i++){
			display.setSeedTextXY(i,0);  //set Cursor to ith line, 0th column
			display.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
			display.putSeedString("Hello World"); //Print Hello World
		}
		
		sleep(2);
	}

	srand(time(NULL));

	testdrawline();
	testdrawrect();
	testfillrect();
	testdrawcircle();

	display.SaveToPBM("/tmp/tst.pbm");
}
