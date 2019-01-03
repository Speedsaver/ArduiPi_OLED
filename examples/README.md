Some examples about the library :

- **oled_demo** : the original demo. Exhibit the C++ interface
- **oled_cdemo** : C portage of the demo. Exhibit the C interface
- **oled_ctrl** : companion to control the display. Unfortunately, it is a bit useless for the moment as the library clear the display at startup. To be corrected.

## CAUTION ##

If you're not providing the right display type, there is some possibilities to crash the display. In this case, the only solution seems to do a power cycle.
