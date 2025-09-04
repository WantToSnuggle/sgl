/* source/include/sgl_draw_cg.h
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Li, Shanwen  (1477153217@qq.com)
 * Document reference link: www.sgl-io.cn
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

#ifndef __SGL_DRAW_CG_H__
#define __SGL_DRAW_CG_H__ 


#include <sgl_core.h>
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
void sgl_draw_fill_hline(sgl_surf_t *surf, sgl_area_t *area, int16_t y, int16_t x1, int16_t x2, int16_t width, sgl_color_t color);


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
void sgl_draw_fill_hline_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t y, int16_t x1, int16_t x2, int16_t width, sgl_color_t color, uint8_t alpha);


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
void sgl_draw_fill_vline(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y1, int16_t y2, int16_t width, sgl_color_t color);


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
void sgl_draw_fill_vline_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y1, int16_t y2, int16_t width, sgl_color_t color, uint8_t alpha);


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
void sgl_draw_fill_hori_bar(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset);


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
void sgl_draw_fill_hori_bar_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset);


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
void sgl_draw_fill_hori_bar_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset, sgl_color_t border_color, int16_t border_width);


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
void sgl_draw_fill_hori_bar_with_alpha_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset, sgl_color_t border_color, int16_t border_width);


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
void sgl_draw_fill_vert_bar(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset);


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
void sgl_draw_fill_vert_bar_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, int16_t knob_offset, sgl_color_t border_color, uint8_t border_width);


/**
 * @brief draw a vertical bar with alpha
 * @param surf          surface
 * @param area          area that you want to draw
 * @param rect          rect coordinate
 * @param fg_color      foreground color
 * @param bg_color      background color
 * @param alpha         alpha of color
 * @param knob_offset   knob offset
 * @return none
 */
void sgl_draw_fill_vert_bar_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset);


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
void sgl_draw_fill_vert_bar_with_alpha_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t fg_color, sgl_color_t bg_color, uint8_t alpha, int16_t knob_offset, sgl_color_t border_color, uint8_t border_width);


/**
 * @brief fill rect on surface
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @return none
 */
void sgl_draw_fill_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color);


/**
 * @brief fill rect on surface with alpha
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @param alpha alpha of rect
 * @return none
 */
void sgl_draw_fill_rect_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, uint8_t alpha);


/**
 * @brief fill rect on surface with pixmap
 * @param surf  surface pointer
 * @param area  area that you want to fill
 * @param rect  rect that you want to fill
 * @param pixmap pixmap pointer
 * @return none
 */
void sgl_draw_fill_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, sgl_pixmap_t *pixmap);


/**
 * @brief fill rect on surface with pixmap and alpha
 * @param surf  surface pointer
 * @param area  area that you want to fill
 * @param rect  rect that you want to fill
 * @param pixmap pixmap pointer
 * @param alpha alpha
 * @return none
 */
void sgl_draw_fill_rect_pixmap_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, sgl_pixmap_t *pixmap, uint8_t alpha);


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
void sgl_draw_fill_round_rect_with_alpha_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, sgl_color_t border_color, uint8_t border_width, uint8_t alpha);


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
void sgl_draw_fill_round_rect_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, uint8_t alpha);


/**
 * @brief fill a round rectangle
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param color color of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color);


/**
 * @brief fill a round rectangle with border
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param color color of rectangle
 * @param border_color color of border
 * @param border_width width of border
 * @return none
 */
void sgl_draw_fill_round_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, sgl_color_t border_color, int16_t border_width);


/**
 * @brief fill a round rectangle with pixmap
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param pixmap pointer to pixmap
 * @return none
 */
void sgl_draw_fill_round_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_pixmap_t *pixmap);


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
void sgl_draw_fill_round_rect_pixmap_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_pixmap_t *pixmap, uint8_t alpha);


/**
 * @brief Draw a circle
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param color Color of the circle
 * @return 0: not need update, 1: need update
 * @return none
 */
void sgl_draw_fill_circle(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color);


/**
 * @brief Draw a circle
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param color Color of the circle
 * @param alpha Alpha of the circle
 * @return 0: not need update, 1: need update
 * @return none
 */
void sgl_draw_fill_circle_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, uint8_t alpha);


/**
 * @brief Draw a circle with pixmap
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param pixmap Pixmap of image
 * @param px  center X coordinate of the pixmap
 * @param py  center Y coordinate of the pixmap
 * @return none
 */
void sgl_draw_fill_circle_pixmap(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_pixmap_t *pixmap, int16_t px, int16_t py);


/**
 * @brief Draw a circle with pixmap and alpha
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param pixmap Pixmap of image
 * @param alpha Alpha of the circle
 * @param px  center X coordinate of the pixmap
 * @param py  center Y coordinate of the pixmap
 * @return none
 */
void sgl_draw_fill_circle_pixmap_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_pixmap_t *pixmap, uint8_t alpha, int16_t px, int16_t py);


/**
 * @brief Draw a circle with border
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param color Color of the circle
 * @param border_color Color of the border
 * @param border_width Width of the border
 * @return none
 */
void sgl_draw_fill_circle_with_border(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, sgl_color_t border_color, int16_t border_width);


/**
 * @brief Draw a circle with alpha and border
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param color Color of the circle
 * @param border_color Color of the border
 * @param border_width Width of the border
 * @param alpha Alpha of the circle
 * @return none
 */
void sgl_draw_fill_circle_with_alpha_border(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, sgl_color_t border_color, int16_t border_width, uint8_t alpha);


/**
 * @brief draw a ring on surface
 * @param surf: pointer of surface
 * @param area: pointer of area
 * @param cx: ring center x
 * @param cy: ring center y
 * @param radius_in: ring inner radius
 * @param radius_out: ring outer radius
 * @param color: ring color
 * @return none
 */
void sgl_draw_fill_ring(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius_in, int16_t radius_out, sgl_color_t color);


/**
 * @brief draw a ring on surface with alpha
 * @param surf: pointer of surface
 * @param area: pointer of area
 * @param cx: ring center x
 * @param cy: ring center y
 * @param radius_in: ring inner radius
 * @param radius_out: ring outer radius
 * @param color: ring color
 * @param alpha: alpha
 * @return none
 */
void sgl_draw_fill_ring_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius_in, int16_t radius_out, sgl_color_t color, uint8_t alpha);


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
void sgl_draw_character(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, sgl_color_t bg_color, sgl_font_t *font);


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
void sgl_draw_character_on_bg(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, sgl_font_t *font);


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
void sgl_draw_character_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, uint8_t alpha, sgl_font_t *font);


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
void sgl_draw_string(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, sgl_color_t bg_color, sgl_font_t *font);


/**
 * @brief Draw a string on the surface with background color
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param font Pointer to the font structure containing character data
 * @param margin Margin between x and y edge
 * @return none
 */
void sgl_draw_string_on_bg(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, sgl_font_t *font, int16_t margin);


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
 * @param margin Margin between x and y edge
 * @return none
 */
void sgl_draw_string_lines(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, sgl_color_t bg_color, sgl_font_t *font, uint8_t line_space, int16_t margin);


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
 * @param margin Margin between x and y edge
 * @return none
 */
void sgl_draw_string_lines_on_bg(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, sgl_font_t *font, uint8_t line_space, int16_t margin);


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
 * @param margin Margin between x and y edge
 * @return none
 */
void sgl_draw_string_lines_with_alpha(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, sgl_font_t *font, uint8_t line_space, int16_t margin);


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
void sgl_draw_one_icon( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, sgl_color_t bg_color, sgl_icon_pixmap_t *icon);


/**
 * @brief draw icon on the background
 * @param surf   surface
 * @param area   area of icon
 * @param x      x coordinate
 * @param y      y coordinate
 * @param color  color of icon
 * @param icon   icon pixmap
 */
void sgl_draw_icon_on_bg( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, sgl_icon_pixmap_t *icon);


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
void sgl_draw_icon_with_alpha( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, uint8_t alpha, sgl_icon_pixmap_t *icon);


/**
 * @brief draw an arc
 * @param surf pointer to surface
 * @param area pointer to area
 * @param desc pointer to arc description
 * @return none
 */
void sgl_draw_fill_arc(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_arc_t *desc);


/**
 * @brief draw an arc with alpha
 * @param surf pointer to surface
 * @param area pointer to area
 * @param desc pointer to arc description
 * @return none
 */
void sgl_draw_fill_arc_with_alpha(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_arc_t *desc);


#endif // !__SGL_DRAW_CG_H__
