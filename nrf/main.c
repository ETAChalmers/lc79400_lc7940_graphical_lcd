#include <stdint.h>
#include <string.h>
#include "image.h"
#include "lcd.h"

uint8_t frame_buf[LCD_WIDTH * LCD_HEIGHT];

int main(void) {
	int t;

	lcd_init();
	memset(frame_buf, 0x00, LCD_WIDTH * LCD_HEIGHT);

	t = 0;
	while(1) {
		lcd_write(frame_buf);

		memmove(frame_buf+LCD_WIDTH, frame_buf, LCD_WIDTH*(LCD_HEIGHT+1));
		memcpy(frame_buf, image_map+LCD_WIDTH*t, LCD_WIDTH);
		t = (t+LCD_HEIGHT-1)%LCD_HEIGHT;
	}

	return 0;
}
