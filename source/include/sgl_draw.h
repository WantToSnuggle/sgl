/* source/include/sgl_draw.h
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

#ifndef __SGL_DRAW_H__
#define __SGL_DRAW_H__ 


#include <sgl_core.h>


#ifdef __cplusplus
extern "C" {
#endif


#define  SGL_ARC_MODE_NORMAL                                (0)
#define  SGL_ARC_MODE_RING                                  (1)
#define  SGL_ARC_MODE_NORMAL_SMOOTH                         (2)
#define  SGL_ARC_MODE_RING_SMOOTH                           (3)


/**
 * @brief rect description
 * @color: color of rect
 * @alpha: alpha of rect
 * @border: border of rect
 * @border_color: border color of rect
 * @pixmap: pixmap of rect
 */
typedef struct sgl_draw_rect {
    sgl_color_t             color;
    int16_t                 radius;
    uint8_t                 alpha;
    uint8_t                 border;
    sgl_color_t             border_color;
    const sgl_pixmap_t      *pixmap;
} sgl_draw_rect_t;


/**
 * @brief arc description
 * @cx: center x
 * @cy: center y
 * @radius_in: inner radius of arc
 * @radius_out: outer radius of arc
 * @start_angle: start angle of arc
 * @end_angle: end angle of arc
 * @color: color of arc
 * @alpha: alpha of arc
 * @mode: mode of arc
 * @bg_color: background color of arc
 */
typedef struct sgl_draw_arc {
    int16_t          cx;
    int16_t          cy;
    int16_t          radius_in;
    int16_t          radius_out;
    sgl_color_t      color;
    uint8_t          alpha;
    uint32_t         start_angle: 9;
    uint32_t         end_angle: 9;
    uint32_t         mode: 2;
    sgl_color_t      bg_color;
} sgl_draw_arc_t;


/** 
 * @brief clip area width of surface
 * @note if you want to check the area is overlap with surface, you can use this macro
 *       it will direct return if the area is not overlap with surface, otherwise, continue
 */
#if (CONFIG_SGL_USE_FULL_FB)
#define sgl_surf_clip_area_return(surf, rect, clip)         do {} while(0)
#else
#define sgl_surf_clip_area_return(surf, rect, clip)         if (!sgl_surf_clip(surf, rect, clip)) return
#endif


/**
 * @brief set pixel on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y: y coordinate
 * @param color: color of pixel
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline void sgl_surf_set_pixel(sgl_surf_t *surf, int16_t x, int16_t y, sgl_color_t color) 
{
    surf->buffer[y * surf->w + x] = color;
}


/**
 * @brief get start buffer address that to set pixel on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y: y coordinate
 * @return pointer of start buffer address
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline sgl_color_t* sgl_surf_get_buf(sgl_surf_t *surf, int16_t x, int16_t y)
{
    return &surf->buffer[y * surf->w + x];
}


/**
 * @brief get pixel on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y: y coordinate
 * @return color of pixel
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline sgl_color_t sgl_surf_get_pixel(sgl_surf_t *surf, int16_t x, int16_t y) 
{
    return surf->buffer[y * surf->w + x];
}


/**
 * @brief draw a horizontal line on surface
 * @param surf: pointer of surface
 * @param y: y coordinate
 * @param x1: x1 coordinate
 * @param x2: x2 coordinate
 * @param color: color of line
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline void sgl_surf_hline(sgl_surf_t *surf, int16_t y, int16_t x1, int16_t x2, sgl_color_t color) 
{
    sgl_color_t *dst = surf->buffer + y * surf->w + x1;
    for (int16_t i = x1; i <= x2; i++) {
        *dst = color;
        dst++;
    }
}


/**
 * @brief draw a vertical line on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y1: y1 coordinate
 * @param y2: y2 coordinate
 * @param color: color of line
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline void sgl_surf_vline(sgl_surf_t *surf, int16_t x, int16_t y1, int16_t y2, sgl_color_t color) 
{
    sgl_color_t *dst = surf->buffer + y1 * surf->w + x;
    for (int16_t i = y1; i <= y2; i++) {
        *dst = color;
        dst += surf->w;
    }
}


/**
 * @brief fill rect on surface with alpha
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @param alpha alpha of rect
 * @return none
 */
void sgl_draw_fill_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, uint8_t alpha);


/**
 * @brief fill rect on surface with alpha
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @param border_color color of border
 * @param border_width width of border
 * @param alpha alpha of rect
 * @return none
 */
void sgl_draw_fill_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, sgl_color_t border_color, int16_t border_width, uint8_t alpha);


/**
 * @brief fill rect on surface with pixmap and alpha
 * @param surf  surface pointer
 * @param area  area that you want to fill
 * @param rect  rect that you want to fill
 * @param pixmap pixmap pointer
 * @param alpha alpha
 * @return none
 */
void sgl_draw_fill_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, const sgl_pixmap_t *pixmap, uint8_t alpha);


/**
 * @brief fill a round rectangle with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param color color of rectangle
 * @param alpha alpha of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, uint8_t alpha);


/**
 * @brief fill a round rectangle with alpha and border
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param color color of rectangle
 * @param border_color color of border
 * @param border_width width of border
 * @param alpha alpha of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, sgl_color_t border_color, uint8_t border_width, uint8_t alpha);


/**
 * @brief fill a round rectangle pixmap with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param pixmap pixmap of rectangle
 * @param alpha alpha of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, const sgl_pixmap_t *pixmap, uint8_t alpha);


/**
 * @brief fill a round rectangle with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param desc rectangle description
 * @return none
 */
void sgl_draw_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, sgl_draw_rect_t *desc);


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
void sgl_draw_character( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, uint8_t alpha, const sgl_font_t *font);


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
void sgl_draw_string(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font);


/**
 * @brief draw a ring on surface with alpha
 * @param surf: pointer of surface
 * @param area: pointer of area
 * @param cx: ring center x
 * @param cy: ring center y
 * @param radius_in: ring inner radius
 * @param radius_out: ring outer radius
 * @param color: ring color
 * @param alpha: ring alpha
 * @return none
 */
void sgl_draw_fill_ring(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius_in, int16_t radius_out, sgl_color_t color, uint8_t alpha);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
