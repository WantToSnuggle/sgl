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
#include <stdio.h>


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
 * @brief Draw a character on the surface
 * @param surf Pointer to the surface where the character will be drawn
 * @param area Pointer to the area where the character will be drawn
 * @param x X coordinate where the character will be drawn
 * @param y Y coordinate where the character will be drawn
 * @param ch_index Index of the character in the font table
 * @param color Foreground color of the character
 * @param bg_color Background color of the character
 * @param font Pointer to the font structure containing character data
 * @return none
 * @note this function is only support bpp:4
 */
void sgl_draw_character( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index,
                        sgl_color_t color, sgl_color_t bg_color, const sgl_font_t *font)
{
    int offset_y2 = font->font_height - font->table[ch_index].ofs_y;
    const uint8_t *dot = &font->bitmap[font->table[ch_index].bitmap_index];
    const uint8_t font_w = font->table[ch_index].box_w;
    int rel_x, rel_y, byte_x, dot_index;
    uint16_t alpha_dot;
    sgl_area_t clip;
    sgl_color_t *buf = NULL;

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
            alpha_dot = (rel_x & 1) ? dot[dot_index] & 0xF : (dot[dot_index] >> 4) & 0xF;
            alpha_dot *= 16;
            
            alpha_dot = alpha_dot > 255 ? 255 : alpha_dot;
            *buf = sgl_color_mixer(color, bg_color, alpha_dot);
            buf++;
        }
    }
}


/**
 * @brief Draw a character on the surface with background color
 * @param surf Pointer to the surface where the character will be drawn
 * @param area Pointer to the area where the character will be drawn
 * @param x X coordinate where the character will be drawn
 * @param y Y coordinate where the character will be drawn
 * @param ch_index Index of the character in the font table
 * @param color Foreground color of the character
 * @param font Pointer to the font structure containing character data
 * @return none
 * @note this function is only support bpp:4
 */
void sgl_draw_character_on_bg( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index,
                              sgl_color_t color, const sgl_font_t *font)
{
    int offset_y2 = font->font_height - font->table[ch_index].ofs_y;
    const uint8_t *dot = &font->bitmap[font->table[ch_index].bitmap_index];
    const uint8_t font_w = font->table[ch_index].box_w;
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    int rel_x, rel_y, byte_x, dot_index;
    uint16_t alpha_dot;

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
            alpha_dot = (rel_x & 1) ? dot[dot_index] & 0xF : (dot[dot_index] >> 4) & 0xF;
            alpha_dot *= 16;

            alpha_dot = alpha_dot > 255 ? 255 : alpha_dot;
            *buf = sgl_color_mixer(color, *buf, alpha_dot);
            buf++;
        }
    }
}


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
void sgl_draw_character_with_alpha( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index,
                                   sgl_color_t color, uint8_t alpha, const sgl_font_t *font)
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
            alpha_dot = (rel_x & 1) ? dot[dot_index] & 0xF : (dot[dot_index] >> 4) & 0xF;
            alpha_dot *= 16;

            alpha_dot = alpha_dot > 255 ? 255 : alpha_dot;
            color_mix = sgl_color_mixer(color, *buf, alpha_dot);
            *buf = sgl_color_mixer(color_mix, *buf, alpha);
            buf++;
        }
    }
}


/**
 * @brief Draw a string on the surface
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate where the string will be drawn
 * @param y Y coordinate where the string will be drawn
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param bg_color Background color of the string
 * @param font Pointer to the font structure containing character data
 * @return none
 */
void sgl_draw_string(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str,
                    sgl_color_t color, sgl_color_t bg_color, const sgl_font_t *font)
{
    int16_t offset_x = x;
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
        sgl_draw_character(surf, area, offset_x, y, ch_index, color, bg_color, font);
        offset_x += font->table[ch_index].box_w;
    }
}


/**
 * @brief Draw a string on the surface with background color
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param font Pointer to the font structure containing character data
 * @return none
 */
void sgl_draw_string_on_bg(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str,
                          sgl_color_t color, const sgl_font_t *font, int16_t margin)
{
    int16_t offset_x = x + margin;
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
        sgl_draw_character_on_bg(surf, area, offset_x, y, ch_index, color, font);
        offset_x += font->table[ch_index].box_w;
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
void sgl_draw_string_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str,
                               sgl_color_t color, uint8_t alpha, const sgl_font_t *font, int16_t margin)
{
    int16_t offset_x = x + margin;
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
        sgl_draw_character_with_alpha(surf, area, offset_x, y, ch_index, color, alpha, font);
        offset_x += font->table[ch_index].box_w;
    }
}


/**
 * @brief Draw a string on the surface that will draw multiple lines
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate where the string will be drawn
 * @param y Y coordinate where the string will be drawn
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param bg_color Background color of the string
 * @param font Pointer to the font structure containing character data
 * @param line_space Line spacing between characters
 * @return none
 */
void sgl_draw_string_lines(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str,
                    sgl_color_t color, sgl_color_t bg_color, const sgl_font_t *font, uint8_t line_space, int16_t margin)
{
    int16_t offset_x = x + margin;
    int16_t ch_index;
    int16_t ch_width;

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

        ch_width = font->table[ch_index].box_w;

        if ((offset_x + ch_width + margin) > area->x2) {
            offset_x = x + margin;
            y += (font->font_height + line_space);
        }

        sgl_draw_character(surf, area, offset_x, y, ch_index, color, bg_color, font);

        offset_x += ch_width;
    }
}


/**
 * @brief Draw a string on the surface with background color that will draw multiple lines
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param font Pointer to the font structure containing character data
 * @param line_space Line spacing between characters
 * @return none
 */
void sgl_draw_string_lines_on_bg(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str,
                          sgl_color_t color, const sgl_font_t *font, uint8_t line_space, int16_t margin)
{
    int16_t offset_x = x + margin;
    int16_t ch_index;
    int16_t ch_width;

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

        ch_width = font->table[ch_index].box_w;

        if ((offset_x + ch_width + margin) > area->x2) {
            offset_x = x + margin;
            y += (font->font_height + line_space);
        }

        sgl_draw_character_on_bg(surf, area, offset_x, y, ch_index, color, font);
        
        offset_x += ch_width;
    }
}


/**
 * @brief Draw a string on the surface with alpha blending that will draw multiple lines
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @param line_space Line spacing between characters
 * @return none
 */
void sgl_draw_string_lines_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str,
                               sgl_color_t color, uint8_t alpha, const sgl_font_t *font, uint8_t line_space, int16_t margin)
{
    int16_t offset_x = x + margin;
    int16_t ch_index;
    int16_t ch_width;

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

        ch_width = font->table[ch_index].box_w;

        if ((offset_x + ch_width + margin) > area->x2) {
            offset_x = x + margin;
            y += (font->font_height + line_space);
        }

        sgl_draw_character_with_alpha(surf, area, offset_x, y, ch_index, color, alpha, font);

        offset_x += ch_width;
    }
}


/**
 * @brief draw text string
 * @param surf surface
 * @param area draw area
 * @param rect draw rect area
 * @param desc draw text description
 * @return none
 */
void sgl_draw_text(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_draw_text_t *desc)
{
    sgl_area_t clip;
    sgl_pos_t align_pos;
    uint8_t alpha = desc->alpha;
    const sgl_font_t *font = desc->font;
    int text_x = 0, icon_y = 0;

    SGL_ASSERT(desc->text != NULL);
    SGL_ASSERT(font != NULL);

    if (!sgl_area_clip(area, rect, &clip)) {
        return;
    }

    if (alpha == SGL_ALPHA_MAX) {
        if (desc->bg_flag) {
            if (desc->radius > 0) {
                sgl_draw_fill_round_rect(surf, &clip, rect, desc->radius, desc->bg_color);
            }
            else {
                sgl_draw_fill_rect(surf, &clip, rect, desc->bg_color);
            }
        }

        if (desc->mode == SGL_DRAW_TEXT_NORMAL) {
            if (desc->icon) {
                text_x = desc->icon->width + 2;
            }

            align_pos = sgl_get_text_pos(rect, font, desc->text, text_x, (sgl_align_type_t)desc->align);

            if (desc->icon) {
                icon_y = ((rect->y2 - rect->y1) - (desc->icon->height)) / 2 + 1;
                sgl_draw_icon_on_bg(surf, &clip, align_pos.x + desc->x_offset, rect->y1 + icon_y + desc->y_offset, desc->color, desc->icon);
            }

            sgl_draw_string_on_bg(surf, &clip, align_pos.x + desc->x_offset + text_x, align_pos.y + desc->y_offset, desc->text, desc->color, font, desc->margin);
        }
        else if (desc->mode == SGL_DRAW_TEXT_ROLL) {
            /* TODO: add roll text */
        }
        else if (desc->mode == SGL_DRAW_TEXT_LINES) {
            sgl_draw_string_lines_on_bg(surf, &clip, clip.x1 + desc->x_offset, clip.y1 + desc->y_offset, desc->text, desc->color, font, desc->line_space, desc->margin);
        }
    }
    else if (alpha > SGL_ALPHA_MIN) {
        if (desc->bg_flag) {
            if (desc->radius > 0) {
                sgl_draw_fill_round_rect_with_alpha(surf, &clip, rect, desc->radius, desc->bg_color, alpha);
            }
            else {
                sgl_draw_fill_rect_with_alpha(surf, &clip, rect, desc->bg_color, alpha);
            }
        }

        if (desc->mode == SGL_DRAW_TEXT_NORMAL) {
            if (desc->icon) {
                text_x = desc->icon->width + 2;
            }

            align_pos = sgl_get_text_pos(rect, font, desc->text, text_x, (sgl_align_type_t)desc->align);

            if (desc->icon) {
                icon_y = ((rect->y2 - rect->y1) - (desc->icon->height)) / 2;
                sgl_draw_icon_with_alpha(surf, &clip, align_pos.x + desc->x_offset, rect->y1 + icon_y + desc->y_offset, desc->color, alpha, desc->icon);
            }

            sgl_draw_string_with_alpha(surf, &clip, align_pos.x + desc->x_offset + text_x, align_pos.y + desc->y_offset, desc->text, desc->color, alpha, font, desc->margin);
        }
        else if (desc->mode == SGL_DRAW_TEXT_ROLL) {
            /* TODO: add roll text */
        }
        else if (desc->mode == SGL_DRAW_TEXT_LINES) {
            sgl_draw_string_lines_with_alpha(surf, &clip, clip.x1 + desc->x_offset, clip.y1 + desc->y_offset, desc->text, desc->color, alpha, font, desc->line_space, desc->margin); 
        }
    }
    else {
        return;
    }
}
