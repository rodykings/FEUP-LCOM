
#include <stdint.h>


#ifndef LAB5_GRAPHIC_H
#define LAB5_GRAPHIC_H

void* (vg_init)(uint16_t mode);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

uint32_t setColor(uint16_t mode, uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step);

#endif //LAB5_GRAPHIC_H
