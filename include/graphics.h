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

void rgba2bgra(uint8_t *pixels, uint32_t width, uint32_t height, uint32_t image_channels);

img_type get_image_type(FILE *fp, uint8_t *data);

void get_fb_size(uint32_t *fb_w, uint32_t *fb_h);

bool draw_image(int32_t offset_x, int32_t offset_y, uint32_t img_width, uint32_t img_height, const char *image_path );

void draw_pixel(int32_t x, int32_t y, uint32_t color);

void draw_rect_round(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, uint32_t borderRadius, uint32_t borderColor);

void draw_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, uint32_t borderColor);

#endif
