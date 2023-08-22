#include <pico/stdlib.h>
#include <pico/time.h>
#include <stdint.h>
#include <string.h>

#include "image.h"
#include "lcd.h"

static uint32_t frame_buf[(LCD_WIDTH * LCD_HEIGHT) / 4];

int main() {
    stdio_init_all();

    memcpy(frame_buf, image_map, LCD_WIDTH * LCD_HEIGHT);
    lcd_init(frame_buf, 0, pio0);

    while (true) {
        for (int i = 0; i < (LCD_WIDTH * LCD_HEIGHT) / 4; i++) {
            sleep_us(1000);
            frame_buf[i] ^= 0xffffffff;
        }
    }
}
