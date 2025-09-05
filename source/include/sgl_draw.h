/* source/include/sgl_draw.h
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Li, Shanwen  (1477153217@qq.com)
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

/* text draw mode */
#define  SGL_DRAW_TEXT_NORMAL                  (0)
#define  SGL_DRAW_TEXT_ROLL                    (1)
#define  SGL_DRAW_TEXT_LINES                   (2)


/* arc draw mode */
#define  SGL_DRAW_ARC_NORMAL                   (0)
#define  SGL_DRAW_ARC_RING                     (1)
#define  SGL_DRAW_ARC_NORMAL_SMOOTH            (2)
#define  SGL_DRAW_ARC_RING_SMOOTH              (3)


/**
 * @brief rectangle description
 * @pixmap: pointer of pixmap
 * @color: color of rectangle
 * @radius: radius of rectangle, if radius is 0, it will draw a normal
 * @alpha: alpha of rectangle
 * @border: border width of rectangle
 * @border_color: border color of rectangle
 */
typedef struct sgl_draw_rect {
    sgl_color_t    color;
    int16_t        radius;
    uint8_t        alpha;
    uint8_t        border;
    sgl_color_t    border_color;
    sgl_pixmap_t   *pixmap;
}sgl_draw_rect_t;


/**
 * @brief rectangle description
 * @cx: center x of rectangle
 * @cy: center y of rectangle
 * @color: color of rectangle
 * @radius: radius of rectangle
 * @alpha: alpha of rectangle
 * @border: border of rectangle
 * @border_color: border color of rectangle
 * @pixmap: pixmap of rectangle
 */
typedef struct sgl_draw_circle {
    int16_t         cx;
    int16_t         cy;
    sgl_color_t     color;
    int16_t         radius;
    uint8_t         alpha;
    uint8_t         border;
    sgl_color_t     border_color;
    sgl_pixmap_t    *pixmap;
}sgl_draw_circle_t;


/**
 * @brief bar description
 * @color: color of bar
 * @bg_color: background color of bar
 * @alpha: alpha of bar
 * @border: border of bar
 * @border_color: border color of bar
 * @pixmap: pixmap of bar
 */
typedef struct sgl_draw_bar {
    int16_t         radius;
    sgl_color_t     color;
    sgl_color_t     bg_color;
    int16_t         knob_offset;
    /* 0: horizontal, 1: vertical */
    uint8_t         direct;
    uint8_t         alpha;
    uint8_t         border;
    sgl_color_t     border_color;
    sgl_pixmap_t    *pixmap;
}sgl_draw_bar_t;


/**
 * @brief text description
 * @text: text to draw
 * @font: font of text
 * @icon: icon of text
 * @color: color of text
 * @bg_color: background color of text
 * @radius: radius of text
 * @alpha: alpha of text
 * @align: align of text
 * @bg_flag: background flag
 * @mode: draw mode
 * @line_space: line space of text
 * @param margin: margin of text
 * @x_offset: x offset of text
 * @y_offset: y offset of text
 */
typedef struct sgl_draw_text {
    const char       *text;
    sgl_font_t       *font;
    sgl_icon_pixmap_t *icon;
    sgl_color_t      color;
    sgl_color_t      bg_color;
    int16_t          radius;
    uint8_t          align: 5;
    uint8_t          bg_flag : 1;
    uint8_t          mode: 2;
    uint8_t          line_space;
    uint8_t          alpha;
    uint8_t          margin;
    int16_t          x_offset;
    int16_t          y_offset;
}sgl_draw_text_t;


/**
 * @brief line draw description
 * @start: start point
 * @end: end point
 * @color: line color
 * @width: line width
 * @alpha: alpha
 */
typedef struct sgl_draw_line {
    sgl_pos_t        start;
    sgl_pos_t        end;
    sgl_color_t      color;
    int16_t          width;
    uint8_t          alpha;
}sgl_draw_line_t;


/**
 * @brief ring draw description
 * @cx: center x
 * @cy: center y
 * @radius_in: inner radius of ring
 * @radius_out: outer radius of ring
 * @color: ring color
 * @alpha: alpha of ring
 */
typedef struct sgl_draw_ring {
    int16_t          cx;
    int16_t          cy;
    int16_t          radius_in;
    int16_t          radius_out;
    sgl_color_t      color;
    uint8_t          alpha;
}sgl_draw_ring_t;


/**
 * @brief icon description
 * @icon: icon pixmap
 * @color: icon color
 * @alpha: alpha of icon
 */
typedef struct sgl_draw_icon {
    sgl_icon_pixmap_t *icon;
    sgl_color_t       color;
    uint8_t           alpha;
    uint8_t           align: 5;
}sgl_draw_icon_t;


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
}sgl_draw_arc_t;


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
 * @brief sgl to draw complete frame
 * @param page current page
 * @param dirty the dirty area that need to upate
 * @return none
 * @note this function should be called in deamon thread or cyclic thread
 */
void sgl_draw_frame(sgl_page_t *page, sgl_area_t *dirty);


/************************** draw description function ***********************************/

/**
 * @brief Draw rectangle
 * @param surf - surface
 * @param area - draw area
 * @param rect - rectangle
 * @param desc - rectangle description
 * @return none
 */
void sgl_draw_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_draw_rect_t *desc);


/**
 * @brief draw text string
 * @param surf surface
 * @param area draw area
 * @param rect draw rect area
 * @param desc draw text description
 * @return none
 */
void sgl_draw_text(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_draw_text_t *desc);


/**
 * @brief draw task, the task contains the draw information and canvas
 * @param surf surface pointer
 * @param area the area of the task
 * @param desc the draw information
 * @return none
 */
void sgl_draw_circle(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_circle_t *desc);


/**
 * @brief Draw rectangle
 * @param surf - surface
 * @param area - draw area
 * @param coords - rectangle coords
 * @param desc - bar description
 * @return none
 */
void sgl_draw_bar(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *coords, sgl_draw_bar_t *desc);


/**
 * @brief draw a line
 * @param surf surface
 * @param area draw area
 * @param desc line description
 * @return none
 */
void sgl_draw_line(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_line_t *desc);


/**
 * @brief Draw rectangle
 * @param surf - surface
 * @param area - draw area
 * @param desc - ring description
 * @return none
 */
void sgl_draw_ring(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_ring_t *desc);


/**
 * @brief draw icon by icon description
 * @param surf   surface
 * @param area   area of icon
 * @param rect   rect of icon
 * @param desc   icon description
 */
void sgl_draw_icon(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_draw_icon_t *desc);


/**
 * @brief draw an arc description
 * @param[in] surf the surface to draw
 * @param[in] area the area to draw
 * @param[in] desc the arc description
 * @return none
 */
void sgl_draw_arc(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_arc_t *desc);


#endif
