#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "stb_image.h"
#include "graphics.h"

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

void rgba2bgra(uint8_t *pixels, uint32_t width, uint32_t height)
{
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t i = (x + (y * width)) * 4;
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

bool draw_image(int32_t offset_x, int32_t offset_y, const char *image_path )
{

    FILE *image_stream;
    image_stream = fopen(image_path, "r");
    
    if (!image_stream) {
        perror("Cannot open image");
        return false;
    }

    uint32_t image_w;
    uint32_t image_h;
    uint8_t *image_pixels;
    uint8_t *image_data = malloc(8);
    
    img_type type = get_image_type(image_stream, image_data);

    if (type != JPEG && type != PNG) {
        printf("Image isn't jpeg, or png\n");
        fclose(image_stream);
        return false;
    }

    size_t len = 8;
    while (1) {
        if (feof(image_stream))
            break;

        if (ferror(image_stream)) {
            fclose(image_stream);
            return false;
        }

        image_data = realloc(image_data, len + READ_SIZE);
        if (!image_data) {
            fclose(image_stream);
            return false;
        }
        
        len += fread(image_data + len, 1, READ_SIZE, image_stream);
    }

    int image_bpp;
    image_pixels = stbi_load_from_memory(image_data, len + READ_SIZE - 1, (int *)&image_w, (int *)&image_h, &image_bpp, STBI_rgb_alpha);

    if (!image_pixels) {
        printf("Couldn't decode %s image: %s\n", type == PNG ? "png" : "jpeg", stbi_failure_reason());
        fclose(image_stream);
        return false;
    }

    rgba2bgra(image_pixels, image_w, image_h);
    
    fclose(image_stream);

    // draw image
    if (offset_y >= -(int32_t)image_h) {
        for (uint32_t y = offset_y < 0 ? -offset_y : 0; y < (offset_y < 0 ? -offset_y : 0) + image_h && y + offset_y < graphics.fb_h; ++y) {
            fseek(graphics.fb, ((offset_x < 0 ? 0 : offset_x) + (y + offset_y) * graphics.fb_w) * 4, SEEK_SET);
            uint32_t w = image_w + (offset_x < 0 ? offset_x : 0) - (offset_x + image_w >= graphics.fb_w ? offset_x - (graphics.fb_w - image_w) : 0);
            uint32_t i = y * image_w - (offset_x < 0 ? offset_x : 0);

            if (i > image_w * image_h) break;

            if (offset_x >= -(int32_t)image_w && w > 0 && offset_x < graphics.fb_w)
                fwrite(image_pixels + i * 4, 1, w * 4, graphics.fb);
        }
    }

    stbi_image_free(image_pixels);

    return true;
}


void draw_pixel(int32_t x, int32_t y, uint32_t color)
{
    if (x < 0 || x >= (int32_t)graphics.fb_w || y < 0 || y >= (int32_t)graphics.fb_h) {return; }

    fseek(graphics.fb, (x + y * graphics.fb_w) * 4, SEEK_SET);

    uint8_t pixelColor[4];
    pixelColor[2] = (color >> 16) & 0xFF; // red
    pixelColor[1] = (color >> 8) & 0xFF;  // green
    pixelColor[0] = color & 0xFF;         // blue
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

void move_cursor(int row, int col)
{
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}