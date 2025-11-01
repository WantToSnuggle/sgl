/* source/draw/sgl_draw_line.c
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
 * @brief draw a horizontal line
 * @param surf surface
 * @param area area that you want to draw
 * @param y y coordinate
 * @param x1 x start coordinate
 * @param x2 x end coordinate
 * @param width line width
 * @param color line color
 * @return none
 */
void sgl_draw_fill_hline(sgl_surf_t *surf, sgl_area_t *area, int16_t y, int16_t x1, int16_t x2, int16_t width, sgl_color_t color)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    sgl_area_t coords = {
        .x1 = x1,
        .y1 = y,
        .x2 = x2,
        .y2 = y + width - 1,
    };

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&coords, &clip)) {
        return;
    }

    for (int y = coords.y1; y <= coords.y2; y++) {
        buf = sgl_surf_get_buf(surf,  coords.x1 - surf->x, y - surf->y);
        for (int x = coords.x1; x <= coords.x2; x++, buf++) {
            *buf = color;
        }
    }
}


/**
 * @brief draw a horizontal line with alpha
 * @param surf surface
 * @param area area that you want to draw
 * @param y y coordinate
 * @param x1 x start coordinate
 * @param x2 x end coordinate
 * @param width line width
 * @param color line color
 * @param alpha alpha of color
 * @return none
 */
void sgl_draw_fill_hline_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t y, int16_t x1, int16_t x2, int16_t width, sgl_color_t color, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    sgl_area_t coords = {
        .x1 = x1,
        .y1 = y,
        .x2 = x2,
        .y2 = y + width - 1,
    };

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&coords, &clip)) {
        return;
    }

    for (int y = coords.y1; y <= coords.y2; y++) {
        buf = sgl_surf_get_buf(surf,  coords.x1 - surf->x, y - surf->y);
        for (int x = coords.x1; x <= coords.x2; x++, buf++) {
            *buf = sgl_color_mixer(color, *buf, alpha);
        }
    }
}


/**
 * @brief draw a vertical line
 * @param surf surface
 * @param area area that you want to draw
 * @param x x coordinate
 * @param y1 y start coordinate
 * @param y2 y end coordinate
 * @param width line width
 * @param color line color
 * @return none
 */
void sgl_draw_fill_vline(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y1, int16_t y2, int16_t width, sgl_color_t color)
{
    sgl_area_t clip;
    sgl_area_t coords = {
        .x1 = x,
        .y1 = y1,
        .x2 = x + width - 1,
        .y2 = y2,
    };

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&coords, &clip)) {
        return;
    }

    for (int i = coords.y1; i <= coords.y2; i++) {
        sgl_surf_hline(surf, i - surf->y, coords.x1, coords.x2 + 1, color);
    }
}


/**
 * @brief draw a vertical line with alpha
 * @param surf surface
 * @param area area that you want to draw
 * @param x x coordinate
 * @param y1 y start coordinate
 * @param y2 y end coordinate
 * @param width line width
 * @param color line color
 * @param alpha alpha of color
 * @return none
 */
void sgl_draw_fill_vline_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y1, int16_t y2, int16_t width, sgl_color_t color, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_area_t coords = {
        .x1 = x,
        .y1 = y1,
        .x2 = x + width - 1,
        .y2 = y2,
    };

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&coords, &clip)) {
        return;
    }

    for (int i = coords.y1; i <= coords.y2; i++) {
        for (int j = coords.x1; j <= coords.x2; j++) {
            sgl_surf_set_pixel(surf, j, i - surf->y, sgl_color_mixer(color, sgl_surf_get_pixel(surf, j, i - surf->y), alpha));
        }
    }
}


void sgl_draw_fill_line_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x1, int16_t x2, int16_t y1, int16_t y2, int16_t width, sgl_color_t color, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_area_t coords = {
        .x1 = x1,
        .y1 = y1,
        .x2 = x2,
        .y2 = y2,
    };

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&coords, &clip)) {
        return;
    }

    SGL_UNUSED(width);
    SGL_UNUSED(color);
    SGL_UNUSED(alpha);
}


/**
 * @brief draw a line
 * @param surf surface
 * @param area draw area
 * @param desc line description
 * @return none
 */
void sgl_draw_line(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_line_t *desc)
{
    uint8_t alpha = desc->alpha;

    int16_t x1 = desc->start.x;
    int16_t y1 = desc->start.y;
    int16_t x2 = desc->end.x;
    int16_t y2 = desc->end.y;

    if (y1 == y2) {
        if (alpha == SGL_ALPHA_MAX) {
            sgl_draw_fill_hline(surf, area, y1, x1, x2, desc->width, desc->color);
        }
        else if (alpha > SGL_ALPHA_MIN) {
            sgl_draw_fill_hline_with_alpha(surf, area, y1, x1, x2, desc->width, desc->color, alpha);
        }
        else {
            return;
        }
    }
    else if (x1 == x2) {
        if (alpha == SGL_ALPHA_MAX) {
            sgl_draw_fill_vline(surf, area, x1, y1, y2, desc->width, desc->color);
        }
        else if (alpha > SGL_ALPHA_MIN) {
            sgl_draw_fill_vline_with_alpha(surf, area, x1, y1, y2, desc->width, desc->color, alpha);
        }
        else {
            return;
        }
    }
    else {
        sgl_draw_fill_line_with_alpha(surf, area, x1, x2, y1, y2, desc->width, desc->color, alpha);
    }
}
