#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "vendor/stb_image.h"
#include "vendor/stb_image_resize2.h"
#include "graphics.h"
#include "utils.h"

#define READ_SIZE 4096

static struct
{
    FILE *fb;
    char *fb_name;
    uint32_t fb_w;
    uint32_t fb_h;
} graphics;

bool use_fb(char* fb_)
{
    graphics.fb_name = fb_;
    get_fb_size(&graphics.fb_w, &graphics.fb_h);

    char *fb_path = malloc(strlen(fb_) + 10);
    sprintf(fb_path, "/dev/%s", fb_);

    graphics.fb = fopen(fb_path, "w");
    if (!graphics.fb) return false;
    return true;
}

void close_fb()
{
    fclose(graphics.fb);
}

void rgba2bgra(uint8_t *pixels, uint32_t width, uint32_t height, uint32_t image_channels)
{
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t i = (x + (y * width)) * image_channels;
            uint8_t b = pixels[i];
            uint8_t r = pixels[i + 2];
            pixels[i] = r;
            pixels[i + 2] = b;
        }
    }
}


img_type get_image_type(FILE *fp, uint8_t *data)
{
    fread(data, 1, 8, fp);

    if (ferror(fp)) return UNKNOWN;
    if (feof(fp)) return UNKNOWN;

    if (memcmp((char *)data, "\xFF\xD8\xFF", 3) == 0)
        return JPEG;
    if (memcmp((char *)data, "\x89PNG\x0D\x0A\x1A\x0A", 8) == 0)
        return PNG;

    return UNKNOWN;
}

void get_fb_size(uint32_t *fb_w, uint32_t *fb_h)
{
    char *size_path = malloc(strlen(graphics.fb_name) + 50);
    sprintf(size_path, "/sys/class/graphics/%s/virtual_size", graphics.fb_name);

    FILE *size = fopen(size_path, "r");
    free(size_path);
    if (!size) return;

    size_t i = 0;
    bool b = 0;
    char *w_ = malloc(32);
    char *h_ = malloc(32);

    /* parse height and width */
    while (1) {
        char c = getc(size);
        if (isdigit(c)) {
            if (b) { h_[i++] = c; }
            else   { w_[i++] = c; }
        } else if (!b) {
            b = 1; i = 0;
        } else break;
    }

    fclose(size);
    *fb_w = (uint32_t) atoi(w_);
    *fb_h = (uint32_t) atoi(h_);
}

bool draw_image(int32_t offset_x, int32_t offset_y, uint32_t img_width, uint32_t img_height, const char *image_path) {
    uint32_t image_w, image_h, image_channels;

    if (access(image_path, F_OK) != 0) return verror("Image file does not exists");

    unsigned char* image_pixels = stbi_load(image_path, &image_w, &image_h, &image_channels, 0);
    if (!image_pixels) return verror("Failed to load the image\n");

     unsigned char *resized_image = (unsigned char *)malloc(img_width * img_height * image_channels);

    if (!stbir_resize_uint8_linear(image_pixels, image_w, image_h, 0,
                             resized_image, img_width, img_height, 0, image_channels))
    {
        stbi_image_free(image_pixels);
        free(resized_image);
        return verror("Failed to resize the image");
    }

    stbi_image_free(image_pixels);

    uint8_t pixelColor[4];
    for (int y = 0; y < img_height; ++y) {
        for (int x = 0; x < img_width; ++x) {
            uint32_t color = 0;
            for (int c = 0; c < image_channels; ++c) {
                color |= (uint32_t)resized_image[(x + y * img_width) * image_channels + c] << (c * 8);
            }

            fseek(graphics.fb, ((x + offset_x) + (y + offset_y) * graphics.fb_w) * 4, SEEK_SET);
            pixelColor[0] = (color >> 16) & 0xFF; // blue
            pixelColor[1] = (color >> 8) & 0xFF;  // red
            pixelColor[2] = color & 0xFF;         // green
            pixelColor[3] = (color >> 24) & 0xFF; // alpha
            fwrite(pixelColor, 1, 4, graphics.fb);
        }
    }

    free(resized_image);
    return true;
}

void draw_pixel(int32_t x, int32_t y, uint32_t color)
{
    if (x < 0 || x >= (int32_t)graphics.fb_w || y < 0 || y >= (int32_t)graphics.fb_h) {return; }

    fseek(graphics.fb, (x + y * graphics.fb_w) * 4, SEEK_SET);

    uint8_t pixelColor[4];
    pixelColor[0] = color & 0xFF;         // red
    pixelColor[1] = (color >> 8) & 0xFF;  // blue
    pixelColor[2] = (color >> 16) & 0xFF; // green
    pixelColor[3] = (color >> 24) & 0xFF; // alpha

    fwrite(pixelColor, 1, 4, graphics.fb);
}

void draw_rect_round(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, uint32_t borderRadius, uint32_t borderColor)
{
    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            uint32_t dx = j < borderRadius ? borderRadius - j : (j >= width - borderRadius ? j - (width - borderRadius - 1) : 0);
            uint32_t dy = i < borderRadius ? borderRadius - i : (i >= height - borderRadius ? i - (height - borderRadius - 1) : 0);

            uint32_t distance = sqrt(dx * dx + dy * dy);

            if (distance <= borderRadius) {
                if (distance >= borderRadius - borderWidth*0.5)
                    draw_pixel(x + j, y + i, borderColor);
            }
        }
    }
}

void draw_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, uint32_t borderColor)
{
    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            if (i < borderWidth || i >= height - borderWidth || j < borderWidth || j >= width - borderWidth)
                draw_pixel(x + j, y + i, borderColor);
        }
    }
}