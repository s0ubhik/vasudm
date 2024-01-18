#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <stdbool.h>

typedef enum {
    JPEG,
    PNG,
    UNKNOWN,
} img_type;

bool use_fb(char* fb_);

void close_fb();

void move_cursor(int row, int col);

void get_fb_size(uint32_t *fb_w, uint32_t *fb_h);

void draw_pixel(int32_t x, int32_t y, uint32_t color);

bool draw_image(int32_t offset_x, int32_t offset_y, const char *image_path);

void draw_rect_round(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, uint32_t borderRadius, uint32_t borderColor);

void draw_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, uint32_t borderColor);

#endif