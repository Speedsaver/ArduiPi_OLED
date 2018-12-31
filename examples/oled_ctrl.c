/*************
 * Turn oled LCD On or Off
 *
 *  Written by Destroyedlolo (http://destroyedlolo.info)
 *	BSD license, check license.txt for more information
 * All text above must be included in any redistribution
 *
 * 25/12/2018	Destroyedlolo (http://destroyedlolo.info)
 * 		First version
 */

#include "ArduiPi_OLED_C.h"

#include <getopt.h>
#include <stdbool.h>

#define PRG_NAME        "oled_OnOff"
#define PRG_VERSION     "1.0"

struct s_opts {
	int oled;
	bool verbose;
	const char *port;
};

struct s_opts opts = {
	OLED_SH1106_I2C_128x64,	// Default oled
	true,					// Not verbose
	"/dev/i2c-2"			// default port
};

/****
 *  Parse arguments
 ****/
void usage( char * name){
	printf("%s\n", name );
	printf("Usage is: %s --oled type [options] {1|0}\n", name);
	puts("  --<o>led type\nOLED type are:");
	for (int i=0; i<OLED_LAST_OLED;i++)
		printf("  %1d %s\n", i, oled_type_str[i]);
	puts("NOTEZ-BIEN : SPI displays are not supported yet\n");
	
	puts("Options are:");
	puts("  --<v>erbose  : speak more to user");
	puts("  --<h>elp");
	puts("<?> indicates the equivalent short option.");
	puts("Short options are prefixed by \"-\" instead of by \"--\".");
	puts("Example :");
	printf( "%s -o 3    --> use a %s OLED\n", name, oled_type_str[3]);
	printf( "%s -o 4 -v --> use a %s OLED being verbose\n", name, oled_type_str[4]);
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
			fputs("Unrecognized option.\n", stderr);
			fputs("Run with '--help'.\n", stderr);
			exit(EXIT_FAILURE);
		}
	}

	if(opts.verbose){
		printf("%s v%s\n", PRG_NAME, PRG_VERSION);
		printf("-- OLED params -- \n");
		printf("Oled is    : %s\n", oled_type_str[opts.oled]);
		printf("On 		: %s\n", opts.port);
		printf("-- Other Stuff -- \n");
		printf("verbose is : %s\n", opts.verbose? "yes" : "no");
		printf("\n");
	}	
}

/*****
 * Let's go 
 *****/

int main( int ac, char **av ){
	char val='?';
	parse_args( ac, av );

	if(optind < ac)
		val = *av[optind];

	if( !PiOLED_Init(opts.oled, opts.port) )
		exit(EXIT_FAILURE);

	switch(val){
	case '0':
		if(opts.verbose)
			puts("Off");
		PiOLED_OnOff(false);
		break;
	case '1':
		if(opts.verbose)
			puts("On");
		PiOLED_OnOff(true);
		break;
#if 1
	case 't':
		if(opts.verbose)
			puts("Test");
		PiOLED_sendCommand( 0xe4 );
		break;
#endif
	default :
		fputs("Unknown mode provided\n", stderr);
		fputs("Run with '--help'.\n", stderr);
	}

	PiOLED_Close();	// Free resources
}
