# LC79400+LC79430 drivers

This is some code to drive the LCD displays found in bulk in a box at ETA.

The pinout is as follows:

1. FLM - Frame load
2. LOAD - Load row
3. CP - Clock pulse, load 4 pixels
4. NC - Not connected (other modles have M-line, to toggle for contrast)
5. Contrast, potentiometer wiper from VEE to GND, to give range around -17V to -30V
6. VDD - 5V
7. GND
8. VEE - LCD drive voltage. VDD - VEE <= 32V, so VEE should be around -25V
9. DI 1 - Data line 1
10. DI 2 - Data line 2
11. DI 3 - Data line 3
12. DI 4 - Data line 4
13. \DISP OFF - inverted display off. Connect to VDD
14. NC - Not connected

The `rp2040` and `nrf` folders have code for the Raspberry Pi Pico and some nrf chips respectively. The rp2040 code uses a PIO state machine anlong with 2 DMA channels in order to draw to the screen completely in the background without any interraction by the core.

The backlight is a CCFL tube, so unless you are in the mood to play with hundreds of volts you should probably swap it for some LEDs.
