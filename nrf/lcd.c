#include "nrf.h"
#include "lcd.h"

/*
 * LCD pinout:
 * 1 - FLM - Frame load
 * 2 - LOAD - Load row
 * 3 - CP - Clock pulse, load 4 pixels
 * 4 - NC - Not connected (other modles have M-line, to toggle for contrast)
 * 5 - Contrast, potentiometer wiper from VEE to GND, to give range around -17V to -30V
 * 6 - VDD - 5V
 * 7 - GND
 * 8 - VEE - LCD drive voltage. VDD - VEE <= 32V, so VEE should be around -25V
 * 9 - DI 1 - Data line 1
 * 10 - DI 2 - Data line 2
 * 11 - DI 3 - Data line 3
 * 12 - DI 4 - Data line 4
 * 13 - \DISP OFF - inverted display off. Connect to VDD
 * 14 - NC - Not connected
 *
 * Theory of operation:
 *
 * X direction:
 *
 * CP loads the values of the lines available on DI1-4 to for pixels from left
 * to right, until row is full. A pulse on LOAD loads the pixel data to the
 * screen, so the columns corresponding to the data loaded will be enabled. LOAD
 * resets the counter, and starts loading the next line from left to right.
 *
 * Y direction:
 * One row at a time is enabled, so only 1 is actually lit up. The LCD itself
 * retains the image for a short while to give contrast to the display. Thus,
 * continous update is required.
 *
 * The LOAD signal shifts the enabled rows one step down, and the topmost row
 * takes the value of the FLM pin.
 *
 * Only one row at a time should be enabled. Set FLM=1 when loading first row,
 * FLM=0 for all other rows.
 *
 * Note that LOAD is shared between X and Y direction circuitry.
 *
 * The screen is timing critical. Recommended to draw the screen from an
 * interrupt, to keep rate steady.
 *
 * Also, keep all voltage levels at 5V, by for example driving via a set of
 * buffers.
 */

#define PIN_FLM      11 /* LCD:  1 */
#define PIN_LOAD     12 /* LCD:  2 */
#define PIN_CP       13 /* LCD:  3 */
/* DI1..DI4 must be in sequence */
#define PIN_DI1      17 /* LCD:  9 */
#define PIN_DI2      18 /* LCD: 10 */
#define PIN_DI3      19 /* LCD: 11 */
#define PIN_DI4      20 /* LCD: 12 */
#define PIN_DISP_OFF 22 /* LCD: 13 */

#define PIN_DI_START (PIN_DI1)
#define PIN_DI_MASK (0xFUL<<PIN_DI_START)

#define SET_OUTPUT(_PIN)(NRF_P0->DIR |= 1 << (_PIN))
#define SET_PIN(_PIN)   (NRF_P0->OUT |= 1 << (_PIN))
#define CLEAR_PIN(_PIN) (NRF_P0->OUT &= ~(1 << (_PIN)))
#define SET_DI(_V)      (NRF_P0->OUT = (NRF_P0->OUT & ~PIN_DI_MASK) | (((_V)&0xf) << PIN_DI_START))


void lcd_init(void) {
	SET_OUTPUT(PIN_FLM);
	SET_OUTPUT(PIN_LOAD);
	SET_OUTPUT(PIN_CP);
	SET_OUTPUT(PIN_DI1);
	SET_OUTPUT(PIN_DI2);
	SET_OUTPUT(PIN_DI3);
	SET_OUTPUT(PIN_DI4);
	SET_OUTPUT(PIN_DISP_OFF);

	SET_PIN(PIN_DISP_OFF);
}

void lcd_write(const uint8_t *buf) {
	int x, y;
	const uint8_t *ptr = buf;
	SET_PIN(PIN_FLM);
	for(y=LCD_HEIGHT; y>0; y--) {
		for(x=LCD_WIDTH; x>0; x--) {
			uint8_t pxls = *(ptr++);

			SET_DI(pxls>>4);
			SET_PIN(PIN_CP); /* Repeat to give a bit more delay */
			SET_PIN(PIN_CP);
			SET_PIN(PIN_CP);
			SET_PIN(PIN_CP);
			CLEAR_PIN(PIN_CP);

			SET_DI(pxls>>0);
			SET_PIN(PIN_CP); /* Repeat to give a bit more delay */
			SET_PIN(PIN_CP);
			SET_PIN(PIN_CP);
			SET_PIN(PIN_CP);
			CLEAR_PIN(PIN_CP);
		}
		SET_PIN(PIN_LOAD);
		CLEAR_PIN(PIN_LOAD);

		CLEAR_PIN(PIN_FLM);
	}
}
