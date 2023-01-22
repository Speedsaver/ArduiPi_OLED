This fork mades the library totally platform agnostic : no need to declare the host platform or the I2C bus to use at library build time but you have to pass as an argument the said port (i.e. **/dev/i2c-2** on the *BananaPI*).
If your host has more than one I2C bus exposed (like on the *BananaPro*), you may have several tiny screens without cheating.

I also added some neat functionality (getPixel(), SaveToPBM(), ...) and make the C interface working.

Geek short installation guide :
-------------------------------

Hardware installation procedure can be found in [Hallard's blog][2]. To make and install the library, the procedure is the following :

* Even if the default configuration may suite 99% of installation, you may customize the **Makefile**, especially for following variables :
  - `PREFIX=` where the library will be installed (default `/usr/local`)
  - `CFLAGS=` to specify compiler option. The default one will rely on build host architecture **that may not be suitable for cross or distributed compilation**.
* then build
```
git clone https://github.com/Speedsaver/ArduiPi_OLED.git && cd ArduiPi_OLED.git && make clean && make
```
* and finally install **as root** (probably using `sudo`):
~~~~
make install
~~~~

CAUTION :
---------

- Again **the Makefile has to be changed if you're cross compiling or if you're using heterogeneous distributed compilation**.
- SPI is not supported : some sequels of the previous code remain but may be cleaned in future release (but if someone wants to take care of SPI in a portable way).
- I discovered lot of issues, misbehaviours in includes dependencies and in Makefile themselves. I corrected the most obvious ones but some work remains : if you're experiencing "*bizarre features*" ((tm) micro$oft), do
```
make clean && make
```


This version has been tested on a BananaPI running Gentoo using an SSD1306 0.96'' screen.

Enjoy !!

Please find bellow comments from previous authors with all my thanks.

jonesman comment's
===================

The original library has been modified to write to the I2C devices at /dev/i2c-X
instead of driving the IO pins directly.
Device and address can be set in dev_io.h
Not implemented for SPI.

[04/2018] Tested on an Orange Pi Zero H2+ running Armbian
(Armbian has something in the boot config called "overlays" which enable IO ports, in this case it was 'i2c0')

Thanks to the original author, what follows is the original README:


Raspberry PI OLED Library Driver
================================

This is the port of some of the most used OLED drivers such as the Adafruit, Seeedstudio, Tindie, Generic... to
the Raspberry Pi. It can drive OLED with chipset such as SSD1306, SH1106, SSD1327 and SSD1308

This library replace the previous one named [ArduiPi_SSD1306][1]

This has been ported to work with the ArduiPi Project but it can work on almost any Raspberry PI.


Installation
============

Everything is documented on this dedicated [post][2] my [blog][3]


[1]: https://github.com/hallard/ArduiPi_SSD1306
[2]: http://hallard.me/adafruit-oled-display-driver-for-pi/
[3]: https://hallard.me

