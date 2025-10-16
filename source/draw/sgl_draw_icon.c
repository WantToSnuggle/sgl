/* source/draw/sgl_draw_icon.c
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
 * @brief draw one icon
 * @param surf   surface
 * @param area   area
 * @param x      x coordinate
 * @param y      y coordinate
 * @param color  color of icon
 * @param bg_color  background color
 * @param icon   icon pixmap
 */
void sgl_draw_one_icon( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, sgl_color_t bg_color, const sgl_icon_pixmap_t *icon)
{
    const uint8_t *dot = icon->bitmap;
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    int rel_x, rel_y, byte_x, dot_index;
    uint8_t alpha_dot;

    sgl_area_t icon_rect = {
        .x1 = x,
        .x2 = x + icon->width - 1,
        .y1 = y,
        .y2 = y + icon->height - 1,
    };

    if (!sgl_surf_clip(surf, &icon_rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    if (icon->bpp == 4) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - icon_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - icon_rect.x1;

                byte_x = rel_x >> 1;
                dot_index = byte_x + (rel_y * (icon->width >> 1));

                if (rel_x & 1) {
                    alpha_dot = dot[dot_index] & 0xF;
                } else {
                    alpha_dot = (dot[dot_index] >> 4) & 0xF;
                }

                *buf = sgl_color_mixer(color, bg_color, alpha_dot * 16);
                buf++;
            }
        }
    }
    else if (icon->bpp == 8) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - icon_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - icon_rect.x1;

                byte_x = rel_x >> 1;
                dot_index = byte_x + (rel_y * (icon->width >> 1));

                *buf = sgl_color_mixer(color, bg_color, dot[dot_index]);
                buf++;
            }
        }
    }
}


/**
 * @brief draw icon on the background
 * @param surf   surface
 * @param area   area of icon
 * @param x      x coordinate
 * @param y      y coordinate
 * @param color  color of icon
 * @param icon   icon pixmap
 */
void sgl_draw_icon_on_bg( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, const sgl_icon_pixmap_t *icon)
{
    const uint8_t *dot = icon->bitmap;
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    int rel_x, rel_y, byte_x, dot_index;
    uint8_t alpha_dot;

    sgl_area_t icon_rect = {
        .x1 = x,
        .x2 = x + icon->width - 1,
        .y1 = y,
        .y2 = y + icon->height - 1,
    };

    if (!sgl_surf_clip(surf, &icon_rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    if (icon->bpp == 4) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - icon_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - icon_rect.x1;

                byte_x = rel_x >> 1;
                dot_index = byte_x + (rel_y * (icon->width >> 1));

                if (rel_x & 1) {
                    alpha_dot = dot[dot_index] & 0xF;
                } else {
                    alpha_dot = (dot[dot_index] >> 4) & 0xF;
                }

                *buf = sgl_color_mixer(color, *buf, alpha_dot * 16);
                buf++;
            }
        }
    }
    else if (icon->bpp == 8) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - icon_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - icon_rect.x1;

                byte_x = rel_x >> 1;
                dot_index = byte_x + (rel_y * (icon->width >> 1));

                *buf = sgl_color_mixer(color, *buf, dot[dot_index]);
                buf++;
            }
        }
    }
}


/**
 * @brief draw icon with alpha
 * @param surf   surface
 * @param area   area of icon
 * @param x      x coordinate
 * @param y      y coordinate
 * @param color  color of icon
 * @param alpha  alpha of icon
 * @param icon   icon pixmap
 */
void sgl_draw_icon_with_alpha( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, uint8_t alpha, const sgl_icon_pixmap_t *icon)
{
    const uint8_t *dot = icon->bitmap;
    sgl_color_t color_mix;
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    int rel_x, rel_y, byte_x, dot_index;
    uint8_t alpha_dot;

    sgl_area_t icon_rect = {
        .x1 = x,
        .x2 = x + icon->width - 1,
        .y1 = y,
        .y2 = y + icon->height - 1,
    };

    if (!sgl_surf_clip(surf, &icon_rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    if (icon->bpp == 4) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - icon_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - icon_rect.x1;

                byte_x = rel_x >> 1;
                dot_index = byte_x + (rel_y * (icon->width >> 1));

                if (rel_x & 1) {
                    alpha_dot = dot[dot_index] & 0xF;
                } else {
                    alpha_dot = (dot[dot_index] >> 4) & 0xF;
                }

                color_mix = sgl_color_mixer(color, *buf, alpha_dot * 16);
                *buf = sgl_color_mixer(color_mix, *buf, alpha);
                buf++;
            }
        }
    }
    else if(icon->bpp == 8) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - icon_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - icon_rect.x1;

                byte_x = rel_x >> 1;
                dot_index = byte_x + (rel_y * (icon->width >> 1));

                *buf = sgl_color_mixer(color, *buf, dot[dot_index]);
                buf++;
            }
        }
    }
}


/**
 * @brief draw icon by icon description
 * @param surf   surface
 * @param area   area of icon
 * @param rect   rect of icon
 * @param desc   icon description
 */
void sgl_draw_icon(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_draw_icon_t *desc)
{
    sgl_area_t clip;
    uint8_t alpha = desc->alpha;
    sgl_pos_t  icon_pos;

    SGL_ASSERT(desc->icon != NULL);

    if (!sgl_area_clip(area, rect, &clip)) {
        return;
    }

    icon_pos = sgl_get_icon_pos(area, desc->icon, 0, (sgl_align_type_t)desc->align);

    if (alpha == SGL_ALPHA_MAX) {
        sgl_draw_icon_on_bg(surf, &clip, icon_pos.x, icon_pos.y, desc->color, desc->icon);
    }
    else if (alpha > SGL_ALPHA_MIN) {
        sgl_draw_icon_with_alpha(surf, &clip, icon_pos.x, icon_pos.y, desc->color, alpha, desc->icon);
    }
    else {
        return;
    }
}
