/* source/draw/sgl_draw_bar.c
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
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_cfgfix.h>
#include <sgl_mm.h>
#include <string.h>


/**
 * @brief draw a horizontal bar
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param knob_offset   knob offset
 * @return none
 */
void sgl_draw_fill_hori_bar(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset)
{
    int16_t radius = (rect->y2 - rect->y1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect(surf, area, rect, radius, bg_color);
    }
    else if (knob_offset == (rect->x2 - rect->x1)) {
        sgl_draw_fill_round_rect(surf, area, rect, radius, fg_color);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x1 + knob_offset,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_t bg_area = {
            .x1 = fg_area.x2,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect(surf, &bg_area, rect, radius, bg_color);
        sgl_draw_fill_round_rect(surf, &fg_area, rect, radius, fg_color);
    }
}


/**
 * @brief draw a vertical bar with alpha
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param alpha         alpha
 * @param knob_offset   knob offset
 * @return none
 */
void sgl_draw_fill_hori_bar_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset)
{
    int16_t radius = (rect->y2 - rect->y1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect_with_alpha(surf, area, rect, radius, bg_color, alpha);
    }
    else if (knob_offset == (rect->x2 - rect->x1)) {
        sgl_draw_fill_round_rect_with_alpha(surf, area, rect, radius, fg_color, alpha);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x1 + knob_offset,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_t bg_area = {
            .x1 = fg_area.x2 + 1,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect_with_alpha(surf, &bg_area, rect, radius, bg_color, alpha);
        sgl_draw_fill_round_rect_with_alpha(surf, &fg_area, rect, radius, fg_color, alpha);
    }
}


/**
 * @brief draw a horizontal bar
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param knob_offset   knob offset
 * @param border_color  border color
 * @param border_width  border width
 * @return none
 */
void sgl_draw_fill_hori_bar_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset, sgl_color_t border_color, int16_t border_width)
{
    int16_t radius = (rect->y2 - rect->y1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect_with_border(surf, area, rect, radius, bg_color, border_color, border_width);
    }
    else if (knob_offset == (rect->x2 - rect->x1)) {
        sgl_draw_fill_round_rect_with_border(surf, area, rect, radius, fg_color, border_color, border_width);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x1 + knob_offset,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_t bg_area = {
            .x1 = fg_area.x2,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect_with_border(surf, &bg_area, rect, radius, bg_color, border_color, border_width);
        sgl_draw_fill_round_rect_with_border(surf, &fg_area, rect, radius, fg_color, border_color, border_width);
    }
}


/**
 * @brief draw a vertical bar with alpha and border
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param alpha         alpha
 * @param knob_offset   knob offset
 * @param border_color  border color
 * @param border_width  border width
 * @return none
 */
void sgl_draw_fill_hori_bar_with_alpha_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset, sgl_color_t border_color, int16_t border_width)
{
    int16_t radius = (rect->y2 - rect->y1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect_with_alpha_border(surf, area, rect, radius, bg_color, border_color, border_width, alpha);
    }
    else if (knob_offset == (rect->x2 - rect->x1)) {
        sgl_draw_fill_round_rect_with_alpha_border(surf, area, rect, radius, fg_color, border_color, border_width, alpha);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x1 + knob_offset,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_t bg_area = {
            .x1 = fg_area.x2 + 1,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = rect->y2
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect_with_alpha_border(surf, &bg_area, rect, radius, bg_color, border_color, border_width, alpha);
        sgl_draw_fill_round_rect_with_alpha_border(surf, &fg_area, rect, radius, fg_color, border_color, border_width, alpha);
    }
}


/**
 * @brief draw a vertical bar
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param knob_offset   knob offset
 * @return none
 */
void sgl_draw_fill_vert_bar(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset)
{
    int16_t radius = (rect->x2 - rect->x1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect(surf, area, rect, radius, bg_color);
    }
    else if (knob_offset == (rect->y2 - rect->y1)) {
        sgl_draw_fill_round_rect(surf, area, rect, radius, fg_color);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y2 - knob_offset,
            .y2 = rect->y2,
        };

        sgl_area_t bg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = fg_area.y1 - 1,
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect(surf, &bg_area, rect, radius, bg_color);
        sgl_draw_fill_round_rect(surf, &fg_area, rect, radius, fg_color);
    }
}


/**
 * @brief draw a vertical bar
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param knob_offset   knob offset
 * @param border_color  border color
 * @param border_width  border width
 * @return none
 */
void sgl_draw_fill_vert_bar_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset, sgl_color_t border_color, uint8_t border_width)
{
    int16_t radius = (rect->x2 - rect->x1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect_with_border(surf, area, rect, radius, bg_color, border_color, border_width);
    }
    else if (knob_offset == (rect->y2 - rect->y1)) {
        sgl_draw_fill_round_rect_with_border(surf, area, rect, radius, fg_color, border_color, border_width);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y2 - knob_offset,
            .y2 = rect->y2,
        };

        sgl_area_t bg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = fg_area.y1 - 1,
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect_with_border(surf, &bg_area, rect, radius, bg_color, border_color, border_width);
        sgl_draw_fill_round_rect_with_border(surf, &fg_area, rect, radius, fg_color, border_color, border_width);
    }
}


/**
 * @brief draw a vertical bar with alpha
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param alpha         alpha of color
 * @param knob_offset   knob offset
 * @return              1 means that should be redraw, 0 means that no need to redraw
 */
void sgl_draw_fill_vert_bar_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset)
{
    int16_t radius = (rect->x2 - rect->x1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect_with_alpha(surf, area, rect, radius, bg_color, alpha);
    }
    else if (knob_offset == (rect->y2 - rect->y1)) {
        sgl_draw_fill_round_rect_with_alpha(surf, area, rect, radius, fg_color, alpha);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y2 - knob_offset,
            .y2 = rect->y2,
        };

        sgl_area_t bg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = fg_area.y1 - 1,
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect_with_alpha(surf, &bg_area, rect, radius, bg_color, alpha);
        sgl_draw_fill_round_rect_with_alpha(surf, &fg_area, rect, radius, fg_color, alpha);
    }
}


/**
 * @brief draw a vertical bar with alpha and border
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param alpha         alpha of color
 * @param knob_offset   knob offset
 * @param border_color  border color
 * @param border_width  border width
 * @return none
 */
void sgl_draw_fill_vert_bar_with_alpha_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset, sgl_color_t border_color, uint8_t border_width)
{
    int16_t radius = (rect->x2 - rect->x1) / 2;

    if (knob_offset == 0) {
        sgl_draw_fill_round_rect_with_alpha_border(surf, area, rect, radius, bg_color, border_color, border_width, alpha);
    }
    else if (knob_offset == (rect->y2 - rect->y1)) {
        sgl_draw_fill_round_rect_with_alpha_border(surf, area, rect, radius, fg_color, border_color, border_width, alpha);
    }
    else {
        sgl_area_t fg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y2 - knob_offset,
            .y2 = rect->y2,
        };

        sgl_area_t bg_area = {
            .x1 = rect->x1,
            .x2 = rect->x2,
            .y1 = rect->y1,
            .y2 = fg_area.y1 - 1,
        };

        sgl_area_selfclip(&fg_area, area);
        sgl_area_selfclip(&bg_area, area);

        sgl_draw_fill_round_rect_with_alpha_border(surf, &bg_area, rect, radius, bg_color, border_color, border_width, alpha);
        sgl_draw_fill_round_rect_with_alpha_border(surf, &fg_area, rect, radius, fg_color, border_color, border_width, alpha);
    }
}


/**
 * @brief Draw rectangle
 * @param surf - surface
 * @param area - draw area
 * @param coords - rectangle coords
 * @param desc - bar description
 * @return none
 */
void sgl_draw_bar(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *coords, sgl_draw_bar_t *desc)
{
    sgl_color_t color = desc->color;
    sgl_color_t bg_color = desc->bg_color;
    uint8_t alpha = desc->alpha;
    uint8_t border = desc->border;

    if (desc->direct == 0) {
        if (alpha == SGL_ALPHA_MAX) {
            if (border) {
                sgl_draw_fill_hori_bar_with_border(surf, area, coords, color, bg_color, desc->knob_offset, desc->border_color, border);
            }
            else {
                sgl_draw_fill_hori_bar(surf, area, coords, color, bg_color, desc->knob_offset);
            }
        }
        else if (alpha > SGL_ALPHA_MIN) {
            if (border) {
                sgl_draw_fill_hori_bar_with_alpha_border(surf, area, coords, color, bg_color, alpha, desc->knob_offset, desc->border_color, border);
            }
            else {
                sgl_draw_fill_hori_bar_with_alpha(surf, area, coords, color, bg_color, alpha, desc->knob_offset);
            }
        }
        else {
            return;
        }
    }
    else {
        if (alpha == SGL_ALPHA_MAX) {
            if (border) {
                sgl_draw_fill_vert_bar_with_border(surf, area, coords, color, bg_color, desc->knob_offset, desc->border_color, border);
            }
            else {
                sgl_draw_fill_vert_bar(surf, area, coords, color, bg_color, desc->knob_offset);
            }
        }
        else if (alpha > SGL_ALPHA_MIN) {
            if (border) {
                sgl_draw_fill_vert_bar_with_alpha_border(surf, area, coords, color, bg_color, alpha, desc->knob_offset, desc->border_color, border);
            }
            else {
                sgl_draw_fill_vert_bar_with_alpha(surf, area, coords, color, bg_color, alpha, desc->knob_offset);
            }
        }
        else {
            return;
        }
    }
}
