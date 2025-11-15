/* source/draw/sgl_draw_text.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Document reference link: docs directory
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <sgl_core.h>
#include <sgl_log.h>
#include <sgl_draw.h>
#include <sgl_math.h>

/**
    font bitmap example is:
    <----------------Width--------------->   font is 4bpp Anti-Aliasing.   
    |                                    |
    |                                    |  
    |               x..@@@@@@@..x        |
  Height            x@.........@x        |
    |               @@.........@@        |
    |               @@.........@@        |
    |               .@@@.....@@@@        | 
    |               ....@@@@@..@@        |
    |               ...........@@        |
    |               ...........@@        |
    |               .@@.......@@.        |
    |<--offset_x-->|x..@@@@@@@..x        |
    |              |       ^             |
    |              |       |             |
    |              |    offset_y         |
    |              |       |             |
    |              |       V             |
    --------------------------------------
***/


/**
 * @brief Draw a character on the surface with alpha blending
 * @param surf Pointer to the surface where the character will be drawn
 * @param area Pointer to the area where the character will be drawn
 * @param x X coordinate where the character will be drawn
 * @param y Y coordinate where the character will be drawn
 * @param ch_index Index of the character in the font table
 * @param color Foreground color of the character
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @return none
 * @note this function is only support bpp:4
 */
void sgl_draw_character( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, uint8_t alpha, const sgl_font_t *font)
{
    int offset_y2 = font->font_height - font->table[ch_index].ofs_y;
    const uint8_t *dot = &font->bitmap[font->table[ch_index].bitmap_index];
    const uint8_t font_w = font->table[ch_index].box_w;
    int rel_x, rel_y, byte_x, dot_index;
    uint16_t alpha_dot;
    sgl_color_t color_mix;
    sgl_color_t *buf = NULL;
    sgl_area_t clip;

    sgl_area_t text_rect = {
        .x1 = x + font->table[ch_index].ofs_x,
        .x2 = x + font_w - 1,
        .y1 = y + offset_y2 - font->table[ch_index].box_h,
        .y2 = y + offset_y2 - 1,
    };

    if (!sgl_surf_clip(surf, &text_rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
        rel_y = y - text_rect.y1;

        for (int x = clip.x1; x <= clip.x2; x++) {
            rel_x = x - text_rect.x1;

            byte_x = rel_x >> 1;
            dot_index = byte_x + (rel_y * (font_w >> 1));
            alpha_dot = (rel_x & 1) ? dot[dot_index] & 0xF : (dot[dot_index] >> 4);
            alpha_dot = alpha_dot | (alpha_dot << 4);
            color_mix = sgl_color_mixer(color, *buf, alpha_dot);
            *buf = sgl_color_mixer(color_mix, *buf, alpha);
            buf++;
        }
    }
}


/**
 * @brief Draw a string on the surface with alpha blending
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @return none
 */
void sgl_draw_string(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font)
{
    uint32_t ch_index;
    #if CONFIG_SGL_TEXT_UTF8
    uint32_t unicode = 0;
    #endif

    while (*str) {
        #if CONFIG_SGL_TEXT_UTF8
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        #else
        ch_index = ((uint32_t)*str) - 32;
        str++;
        #endif
        sgl_draw_character(surf, area, x, y, ch_index, color, alpha, font);
        x += font->table[ch_index].box_w;
    }
}


/**
 * @brief Draw a string on the surface with alpha blending and multiple lines
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @param edge_margin Margin between characters
 * @param line_margin Margin between lines
 * @return none
 */
void sgl_draw_string_mult_line(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font, uint8_t edge_margin, uint8_t line_margin)
{
    int16_t ch_index, ch_width;
    #if CONFIG_SGL_TEXT_UTF8
    uint32_t unicode = 0;
    #endif
    x += edge_margin;

    while (*str) {
        if (*str == '\n') {
            x = (area->x1 + edge_margin);
            y += (font->font_height + line_margin);
            str ++;
            continue;
        }

        #if CONFIG_SGL_TEXT_UTF8
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        #else
        ch_index = ((uint32_t)*str) - 32;
        str++;
        #endif

        ch_width = font->table[ch_index].box_w;

        if ((x + ch_width + edge_margin) > area->x2) {
            x = area->x1 + edge_margin;
            y += (font->font_height + line_margin);
        }

        sgl_draw_character(surf, area, x, y, ch_index, color, alpha, font);
        x += ch_width;
    }
}
