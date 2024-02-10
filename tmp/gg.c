#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vendor/stb_truetype.h"
#include "graphics.h"

// Assume you have the stb_truetype.h file in your "vendor" directory

static struct
{
    FILE *fb;
    char *fb_name;
    uint32_t fb_w;
    uint32_t fb_h;
} graphics;

bool use_fb(char *fb_)
{
    graphics.fb_name = fb_;
    get_fb_size(&graphics.fb_w, &graphics.fb_h);

    char *fb_path = malloc(strlen(fb_) + 10);
    sprintf(fb_path, "/dev/%s", fb_);

    graphics.fb = fopen(fb_path, "w");
    if (!graphics.fb)
        return false;

    return true;
}

void close_fb()
{
    fclose(graphics.fb);
}

void get_fb_size(uint32_t *fb_w, uint32_t *fb_h)
{
    // Implement your logic to get frame buffer size
    // This might include reading from sysfs, ioctl calls, or any other method specific to your system.
    // For this example, we'll set some arbitrary values.
    *fb_w = 800;
    *fb_h = 600;
}

void draw_pixel(int32_t x, int32_t y, uint32_t color)
{
    if (x < 0 || x >= (int32_t)graphics.fb_w || y < 0 || y >= (int32_t)graphics.fb_h)
    {
        return;
    }

    fseek(graphics.fb, (x + y * graphics.fb_w) * 4, SEEK_SET);

    uint8_t pixelColor[4];
    pixelColor[2] = (color >> 16) & 0xFF; // red
    pixelColor[1] = (color >> 8) & 0xFF;  // green
    pixelColor[0] = color & 0xFF;         // blue
    pixelColor[3] = (color >> 24) & 0xFF; // alpha

    fwrite(pixelColor, 1, 4, graphics.fb);
}

// Render text using stb_truetype
void render_text(const char *text, stbtt_fontinfo *font, int font_size, int x, int y, uint32_t color)
{
    float scale = stbtt_ScaleForPixelHeight(font, font_size);
    int baseline, ascent, descent;
    stbtt_GetFontVMetrics(font, &ascent, &descent, &baseline);

    int start_x = x;
    while (*text)
    {
        int codepoint = (unsigned char)*text;
        int advance, left_side_bearing;
        stbtt_GetCodepointHMetrics(font, codepoint, &advance, &left_side_bearing);

        int glyph_width, glyph_height;
        unsigned char *bitmap = stbtt_GetCodepointBitmap(font, scale, scale, codepoint, &glyph_width, &glyph_height, 0, 0);

        for (int i = 0; i < glyph_height; ++i)
        {
            for (int j = 0; j < glyph_width; ++j)
            {
                if (bitmap[i * glyph_width + j] > 0)
                {
                    int px = x + j + left_side_bearing;
                    int py = y - i - baseline;
                    draw_pixel(px, py, color);
                }
            }
        }

        x += advance;
        ++text;

        free(bitmap);
    }
}

int main()
{
    // Example usage
    if (use_fb("your_framebuffer_name"))
    {
        // Load TrueType font
        const char *font_file_path = "path/to/your/font.ttf"; // Replace with the actual path
        FILE *font_file = fopen(font_file_path, "rb");
        if (!font_file)
        {
            fprintf(stderr, "Failed to open font file.\n");
            return -1;
        }

        fseek(font_file, 0, SEEK_END);
        long font_file_size = ftell(font_file);
        fseek(font_file, 0, SEEK_SET);

        unsigned char *font_data = (unsigned char *)malloc(font_file_size);
        if (!font_data)
        {
            fclose(font_file);
            fprintf(stderr, "Memory allocation failed.\n");
            return -1;
        }

        fread(font_data, 1, font_file_size, font_file);
        fclose(font_file);

        stbtt_fontinfo font;
        if (!stbtt_InitFont(&font, font_data, 0))
        {
            fprintf(stderr, "Failed to initialize font.\n");
            free(font_data);
            return -1;
        }

        // Render text onto framebuffer
        const char *text = "Hello, stb_truetype!";
        int font_size = 24;
        int text_x = 10;
        int text_y = 30;
        uint32_t text_color = 0xFFFFFFFF; // White color

        render_text(text, &font, font_size, text_x, text_y, text_color);

        // Close framebuffer
        close_fb();
    }
    else
    {
        fprintf(stderr, "Failed to initialize framebuffer.\n");
        return -1;
    }

    return 0;
}
