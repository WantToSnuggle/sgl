/* source/widgets/sgl_textbox.h
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

#ifndef __SGL_TEXTBOX_H__
#define __SGL_TEXTBOX_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl textbox struct
 * @desc: text description
 */
typedef struct sgl_textbox {
    sgl_obj_t       obj;
    sgl_draw_rect_t bg;
    const char       *text;
    const sgl_font_t *font;
    uint8_t          line_margin;
    uint8_t          edge_margin;
    sgl_color_t      text_color;
    int32_t          y_offset;
    sgl_draw_rect_t  scroll_bg;
    sgl_draw_rect_t  scroll_fg;
    int32_t          text_height: 31;
    int32_t          scroll_enable: 1;
}sgl_textbox_t;


/**
 * @brief create a textbox object
 * @param parent parent of the textbox
 * @return textbox object
 */
sgl_obj_t* sgl_textbox_create(sgl_obj_t* parent);


/**
 * @brief set text of the textbox
 * @param obj textbox object
 * @param text text to be set
 */
static inline void sgl_textbox_set_text(sgl_obj_t *obj, const char *text)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->text = text;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set text color of the textbox
 * @param obj textbox object
 * @param color text color to be set
 * @return none
 */
static inline void sgl_textbox_set_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->text_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set text font of the textbox
 * @param obj textbox object
 * @param font font of text
 * @return none
 */
static inline void sgl_textbox_set_font(sgl_obj_t *obj, const sgl_font_t *font)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->font = font;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set background color of the textbox
 * @param obj textbox object
 * @param color background color to be set
 * @return none
 */
static inline void sgl_textbox_set_bg_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->bg.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set radius of the textbox
 * @param obj textbox object
 * @param radius radius to be set
 * @return none
 */
static inline void sgl_textbox_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->bg.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set border color of the textbox
 * @param obj textbox object
 * @param color border color to be set
 * @return none
 */
static inline void sgl_textbox_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->bg.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set border width of the textbox
 * @param obj textbox object
 * @param width border width to be set
 * @return none
 */
static inline void sgl_textbox_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->bg.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set textbox pixmap
 * @param obj textbox object
 * @param pixmap pixmap to be set
 * @return none
 */
static inline void sgl_textbox_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->bg.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set textbox line margin
 * @param obj textbox object
 * @param margin line margin to be set
 * @return none
 */
static inline void sgl_textbox_set_line_margin(sgl_obj_t *obj, uint8_t margin)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->line_margin = margin;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set textbox edge margin
 * @param obj textbox object
 * @param margin edge margin to be set
 * @return none
 */
static inline void sgl_textbox_set_edge_margin(sgl_obj_t *obj, uint8_t margin)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    textbox->edge_margin = margin;
    sgl_obj_set_dirty(obj);
}

#endif // !__SGL_TEXTBOX_H__
