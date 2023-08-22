#ifndef LCD_H
#define LCD_H

#include "stdint.h"

/* LCD size, height in pixels, width in bytes */
#define LCD_WIDTH (320/8)
#define LCD_HEIGHT (240)

void lcd_init(void);
void lcd_write(const uint8_t *buf);

#endif
