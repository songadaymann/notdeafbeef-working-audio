#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../include/visual_types.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 12

// Simple 8x12 bitmap font for ASCII characters
// Each character is represented as 12 bytes (rows), each byte represents 8 pixels
static const uint8_t ASCII_FONT[128][CHAR_HEIGHT] = {
    // Space (32)
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    // Exclamation mark (33)
    ['!'] = {0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00},
    
    // Hash/pound (35) - for terrain
    ['#'] = {0x00, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x00, 0x00, 0x00},
    
    // Percent (37)
    ['%'] = {0x00, 0x60, 0x66, 0x0C, 0x18, 0x30, 0x60, 0x66, 0x06, 0x00, 0x00, 0x00},
    
    // Ampersand (38)
    ['&'] = {0x00, 0x1C, 0x36, 0x1C, 0x38, 0x6F, 0x66, 0x66, 0x3B, 0x00, 0x00, 0x00},
    
    // Plus (43)
    ['+'] = {0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    
    // Minus/dash (45)
    ['-'] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    // Equals (61)
    ['='] = {0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    // At symbol (64)
    ['@'] = {0x00, 0x3C, 0x66, 0x6E, 0x6A, 0x6E, 0x60, 0x62, 0x3C, 0x00, 0x00, 0x00},
    
    // Letters A-Z
    ['A'] = {0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00},
    ['B'] = {0x00, 0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00},
    ['C'] = {0x00, 0x3C, 0x66, 0x60, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['D'] = {0x00, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00, 0x00, 0x00},
    ['E'] = {0x00, 0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x7E, 0x00, 0x00, 0x00},
    ['F'] = {0x00, 0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00},
    ['G'] = {0x00, 0x3C, 0x66, 0x60, 0x60, 0x6E, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['H'] = {0x00, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00},
    ['I'] = {0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00},
    ['J'] = {0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00},
    ['K'] = {0x00, 0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x66, 0x00, 0x00, 0x00},
    ['L'] = {0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00, 0x00, 0x00},
    ['M'] = {0x00, 0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00},
    ['N'] = {0x00, 0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00},
    ['O'] = {0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['P'] = {0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00},
    ['Q'] = {0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x6A, 0x6C, 0x36, 0x00, 0x00, 0x00},
    ['R'] = {0x00, 0x7C, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0x66, 0x00, 0x00, 0x00},
    ['S'] = {0x00, 0x3C, 0x66, 0x60, 0x3C, 0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['T'] = {0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00},
    ['U'] = {0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['V'] = {0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00},
    ['W'] = {0x00, 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x63, 0x00, 0x00, 0x00},
    ['X'] = {0x00, 0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00},
    ['Y'] = {0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00},
    ['Z'] = {0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x7E, 0x00, 0x00, 0x00},
    
    // Numbers 0-9
    ['0'] = {0x00, 0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['1'] = {0x00, 0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00},
    ['2'] = {0x00, 0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00, 0x00, 0x00},
    ['3'] = {0x00, 0x3C, 0x66, 0x06, 0x1C, 0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['4'] = {0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0x6C, 0x7E, 0x0C, 0x0C, 0x00, 0x00, 0x00},
    ['5'] = {0x00, 0x7E, 0x60, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['6'] = {0x00, 0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['7'] = {0x00, 0x7E, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00},
    ['8'] = {0x00, 0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00},
    ['9'] = {0x00, 0x3C, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00, 0x00, 0x00},
    
    // Special characters for terrain and effects
    ['|'] = {0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00},
    ['*'] = {0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['^'] = {0x00, 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['~'] = {0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['?'] = {0x00, 0x3C, 0x66, 0x06, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00},
};

// Draw a single character at position with color and alpha
void draw_ascii_char(uint32_t *pixels, int x, int y, char c, uint32_t color, int alpha) {
    if (c < 0 || c >= 128) return; // Out of font range
    if (x < 0 || x >= VIS_WIDTH - CHAR_WIDTH || y < 0 || y >= VIS_HEIGHT - CHAR_HEIGHT) return;
    
    // Extract RGB components
    uint32_t r = (color >> 16) & 0xFF;
    uint32_t g = (color >> 8) & 0xFF;
    uint32_t b = color & 0xFF;
    
    // Apply alpha
    r = (r * alpha) / 255;
    g = (g * alpha) / 255;
    b = (b * alpha) / 255;
    
    uint32_t final_color = 0xFF000000 | (r << 16) | (g << 8) | b;
    
    const uint8_t *char_data = ASCII_FONT[(int)c];
    
    for (int row = 0; row < CHAR_HEIGHT; row++) {
        uint8_t byte = char_data[row];
        for (int col = 0; col < CHAR_WIDTH; col++) {
            if (byte & (0x80 >> col)) { // Test bit from left to right
                int px = x + col;
                int py = y + row;
                if (px >= 0 && px < VIS_WIDTH && py >= 0 && py < VIS_HEIGHT) {
                    pixels[py * VIS_WIDTH + px] = final_color;
                }
            }
        }
    }
}

// Draw a string at position
void draw_ascii_string(uint32_t *pixels, int x, int y, const char *str, uint32_t color, int alpha) {
    int pos_x = x;
    for (const char *p = str; *p; p++) {
        if (*p == '\n') {
            pos_x = x;
            y += CHAR_HEIGHT;
        } else {
            draw_ascii_char(pixels, pos_x, y, *p, color, alpha);
            pos_x += CHAR_WIDTH;
        }
    }
}

// Draw a circle using ASCII characters
void draw_ascii_circle(uint32_t *pixels, int cx, int cy, int radius, char fill_char, uint32_t color, int alpha) {
    // Sample points around circle circumference
    int num_points = radius * 6; // More points for larger circles
    if (num_points < 12) num_points = 12;
    
    for (int i = 0; i < num_points; i++) {
        float angle = (2.0f * M_PI * i) / num_points;
        int x = cx + (int)(cosf(angle) * radius);
        int y = cy + (int)(sinf(angle) * radius);
        
        // Align to character grid
        x = (x / CHAR_WIDTH) * CHAR_WIDTH;
        y = (y / CHAR_HEIGHT) * CHAR_HEIGHT;
        
        draw_ascii_char(pixels, x, y, fill_char, color, alpha);
    }
}

// Draw a filled rectangle using characters
void draw_ascii_rect(uint32_t *pixels, int x, int y, int width, int height, char fill_char, uint32_t color, int alpha) {
    int char_cols = width / CHAR_WIDTH;
    int char_rows = height / CHAR_HEIGHT;
    
    for (int row = 0; row < char_rows; row++) {
        for (int col = 0; col < char_cols; col++) {
            int px = x + col * CHAR_WIDTH;
            int py = y + row * CHAR_HEIGHT;
            draw_ascii_char(pixels, px, py, fill_char, color, alpha);
        }
    }
}

// Get character dimensions
int get_char_width(void) { return CHAR_WIDTH; }
int get_char_height(void) { return CHAR_HEIGHT; }
