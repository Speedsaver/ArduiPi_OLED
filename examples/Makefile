#*********************************************************************
# This is the makefile for the Adafruit SSD1306 OLED library driver
#
#	02/18/2013 	Charles-Henri Hallard (http://hallard.me)
#							Modified for compiling and use on Raspberry ArduiPi Board
#							LCD size and connection are now passed as arguments on 
#							the command line (no more #define on compilation needed)
#							ArduiPi project documentation http://hallard.me/arduipi
#
# 08/26/2015    Lorenzo Delana (lorenzo.delana@gmail.com)
#                                                       added bananapi specific CCFLAGS and conditional macro BANANPI
#
#	--- European times format ---
# 23/12/2018    Destroyedlolo (http://destroyedlolo.info)
# 		Make platform agnostic
# *********************************************************************

# Makefile itself dir
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

prefix := /usr/local

# define all programs
PROGRAMS = oled_demo
SOURCES = ${PROGRAMS:=.cpp}

CPROGRAMS = oled_cdemo oled_ctrl
CSOURCES = ${CPROGRAMS:=.c}

CXX=g++
CC=gcc

# notez-bien : in case of cross or distributed compilation, 
# -march HAS TO BE CHANGED to match the target architecture.
# I.e.: for A20 based machines (BananaPI, ...), should be :
# -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard
CFLAGS=-march=native -Ofast -pipe -Wall

all: ${PROGRAMS} ${CPROGRAMS}

${PROGRAMS}: ${SOURCES}
	$(CXX) $(CFLAGS) -Wall -I../ -L../ $@.cpp -o $@ -lArduiPi_OLED

${CPROGRAMS}: ${CSOURCES}
	$(CC) $(CFLAGS) -Wall -I../ -L../ $@.c -o $@ -lArduiPi_OLED

clean:
	rm -rf $(PROGRAMS) $(CPROGRAMS) 

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS) $(CPROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install


