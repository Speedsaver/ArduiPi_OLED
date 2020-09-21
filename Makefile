
#*********************************************************************
# This is the makefile for the ArduiPi OLED library driver
#
#	02/18/2013 	Charles-Henri Hallard (http://hallard.me)
#							Modified for compiling and use on Raspberry ArduiPi Board
#							LCD size and connection are now passed as arguments on 
#							the command line (no more #define on compilation needed)
#							ArduiPi project documentation http://hallard.me/arduipi
# 
# 07/26/2013	Charles-Henri Hallard
#							modified name for generic library using different OLED type
#
# 08/26/2015	Lorenzo Delana (lorenzo.delana@gmail.com)
#							added bananapi specific CCFLAGS and conditional macro BANANPI
#
#	--- European times format ---
# 23/12/2018    Destroyedlolo (http://destroyedlolo.info)
# 		Make platform agnostic
# 21/09/2020	JG1UAA (http://github.com/jg1uaa)
#		add default setting for BananaPi (for compatibility)
#		rename dev_io.[ch] -> ArduiPi_OLED_devio.[ch]
# *********************************************************************

# Makefile itself dir
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

# Where you want it installed when you do 'make install'
PREFIX=/usr/local

# Library parameters
# where to put the lib
LIBDIR=$(PREFIX)/lib
# lib name 
LIB=libArduiPi_OLED
# shared library name
LIBSUB=.7
LIBNAME=$(LIB).so.2${LIBSUB}

# notez-bien : in case of cross or distributed compilation, 
# -march HAS TO BE CHANGED to match the target architecture.
# I.e.: for A20 based machines (BananaPI, ...), should be :
# -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard
CFLAGS=-march=native -Ofast -pipe

CXX=g++
CC=gcc
CFLAGS=$(CCFLAGS)

# board specific settings (only for Banana Pi)
ifeq ($(BOARD), bananapi)
CFLAGS+=-DBANANAPI
CCFLAGS+=-DBANANAPI
endif

# make all
all: ArduiPi_OLED 

# Make the library
ArduiPi_OLED: ArduiPi_OLED.o Adafruit_GFX.o ArduiPi_OLED_C.o ArduiPi_OLED_devio.o
	$(CXX) -shared -Wl,-soname,$(LIB).so.2 $(CFLAGS) $(LDFLAGS)  -o ${LIBNAME} $^
	@ln -sf ${LIBNAME} ${LIB}.so.2
	@ln -sf ${LIBNAME} ${LIB}.so

# Library parts (use -fno-rtti flag to avoid link problem)
ArduiPi_OLED.o: ArduiPi_OLED.cpp ArduiPi_OLED_devio.h ArduiPi_OLED_lib.h ArduiPi_OLED.h Adafruit_GFX.h
	$(CXX) -Wall -fPIC -fno-rtti $(CCFLAGS) -c $^

Adafruit_GFX.o: Adafruit_GFX.cpp ArduiPi_OLED_devio.h ArduiPi_OLED_lib.h
	$(CXX) -Wall -fPIC -fno-rtti $(CCFLAGS) -c $^

ArduiPi_OLED_devio.o: ArduiPi_OLED_devio.c ArduiPi_OLED_devio.h
	$(CC) -Wall -fPIC $(CFLAGS) -c $^

ArduiPi_OLED_C.o: ArduiPi_OLED_C.cpp ArduiPi_OLED_lib.h Adafruit_GFX.h ArduiPi_OLED.h ArduiPi_OLED_C.h ArduiPi_OLED_devio.h
	$(CXX) -Wall -fPIC -fno-rtti $(CCFLAGS) -c $^

# Install the library to LIBPATH
install: 
	@echo "[Install Library]"
	@if ( test ! -d $(PREFIX)/lib ) ; then mkdir -p $(PREFIX)/lib ; fi
	@install -m 0755 ${LIBNAME} ${LIBDIR}
	@ln -sf ${LIBDIR}/${LIBNAME} ${LIBDIR}/${LIB}.so.2
	@ln -sf ${LIBDIR}/${LIBNAME} ${LIBDIR}/${LIB}.so
	@ldconfig
	@rm -rf ${LIB}.*

	@echo "[Install Headers]"
	@if ( test ! -d $(PREFIX)/include ) ; then mkdir -p $(PREFIX)/include ; fi
	@cp -f  Adafruit_*.h $(PREFIX)/include
	@cp -f  ArduiPi_*.h $(PREFIX)/include


# Uninstall the library 
uninstall: 
	@echo "[Uninstall Library]"
	@rm -f ${LIBDIR}/${LIB}.*

	@echo "[Uninstall Headers]"
	@rm -rf  $(PREFIX)/include/ArduiPi_OLED*

# clear build files
clean:
	rm -rf *.o *.gch ${LIB}.* ${LIBDIR}/${LIB}.*
