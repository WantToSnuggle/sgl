/* source/widgets/sgl_msgbox.h
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

#ifndef __SGL_MSGBOX_H__
#define __SGL_MSGBOX_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl message box struct
 * @obj: sgl general object
 * @body_desc: pointer to sgl_draw_rect_t descriptor
 */
typedef struct sgl_msgbox {
    sgl_obj_t        obj;
    sgl_draw_rect_t  body_desc;
    const sgl_font_t *font;

    const char       *title_text;
    sgl_color_t      title_color;
    const sgl_icon_pixmap_t *title_icon;

    const char       *msg_text;
    sgl_color_t      msg_color;
    uint8_t          msg_line_margin;

    uint8_t          status;
    sgl_color_t      btn_text_color;
    sgl_color_t      apply_color;
    sgl_color_t      close_color;
    const char       *apply_text;
    const char       *close_text;
    const sgl_icon_pixmap_t *apply_icon;
    const sgl_icon_pixmap_t *close_icon;

}sgl_msgbox_t;


/**
 * @brief create a message box object
 * @param parent parent of the message box
 * @return message box object
 */
sgl_obj_t* sgl_msgbox_create(sgl_obj_t* parent);

/**
 * @brief set message box color
 * @param obj message box object
 * @param color message box color
 * @return none
 */
static inline void sgl_msgbox_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->body_desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box alpha
 * @param obj message box object
 * @param alpha message box alpha
 * @return none
 */
static inline void sgl_msgbox_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->body_desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box radius
 * @param obj message box object
 * @param radius message box radius
 * @return none
 */
static inline void sgl_msgbox_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->body_desc.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box border width
 * @param obj message box object
 * @param width message box border width
 * @return none
 */
static inline void sgl_msgbox_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->body_desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box border color
 * @param obj message box object
 * @param color message box border color
 * @return none
 */
static inline void sgl_msgbox_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->body_desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box pixmap
 * @param obj message box object
 * @param pixmap message box pixmap
 * @return none
 */
static inline void sgl_msgbox_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->body_desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box all text font
 * @param obj message box object
 * @param font message box font
 * @return none
 */
static inline void sgl_msgbox_set_font(sgl_obj_t *obj, const sgl_font_t *font)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->font = font;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box title text
 * @param obj message box object
 * @param text message box title text
 * @return none
 */
static inline void sgl_msgbox_set_title_text(sgl_obj_t *obj, const char *text)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->title_text = text;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box title text color
 * @param obj message box object
 * @param color message box title text color
 * @return none
 */
static inline void sgl_msgbox_set_title_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->title_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box title icon
 * @param obj message box object
 * @param icon message box title icon
 * @return none
 */
static inline void sgl_msgbox_set_title_icon(sgl_obj_t *obj, const sgl_icon_pixmap_t *icon)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->title_icon = icon;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box message text
 * @param obj message box object
 * @param text message box message text
 * @return none
 */
static inline void sgl_msgbox_set_msg_text(sgl_obj_t *obj, const char *text)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->msg_text = text;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box message text color
 * @param obj message box object
 * @param color message box message text color
 * @return none
 */
static inline void sgl_msgbox_set_msg_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->msg_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box message text line margin
 * @param obj message box object
 * @param margin message box message text line margin
 * @return none
 */
static inline void sgl_msgbox_set_msg_line_margin(sgl_obj_t *obj, uint8_t margin)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->msg_line_margin = margin;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box apply button text
 * @param obj message box object
 * @param text message box apply button text
 * @return none
 */
static inline void sgl_msgbox_set_apply_text(sgl_obj_t *obj, const char *text)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->apply_text = text;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box apply button icon
 * @param obj message box object
 * @param icon message box apply button icon
 * @return none
 */
static inline void sgl_msgbox_set_apply_icon(sgl_obj_t *obj, const sgl_icon_pixmap_t *icon)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->apply_icon = icon;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box apply button text color
 * @param obj message box object
 * @param color message box apply button text color
 * @return none
 */
static inline void sgl_msgbox_set_apply_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->btn_text_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box apply button color
 * @param obj message box object
 * @param color message box apply button color
 * @return none
 */
static inline void sgl_msgbox_set_apply_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->apply_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box close button text
 * @param obj message box object
 * @param text message box close button text
 * @return none
 */
static inline void sgl_msgbox_set_close_text(sgl_obj_t *obj, const char *text)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->close_text = text;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box close button icon
 * @param obj message box object
 * @param icon message box close button icon
 * @return none
 */
static inline void sgl_msgbox_set_close_icon(sgl_obj_t *obj, const sgl_icon_pixmap_t *icon)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->close_icon = icon;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set message box close button text color
 * @param obj message box object
 * @param color message box close button text color
 * @return none
 */
static inline void sgl_msgbox_set_close_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    msgbox->btn_text_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief get message box exit answer
 * @param obj message box object
 * @return true if exit answer is yes, false otherwise
 */
static inline bool sgl_msgbox_get_exit_answer(sgl_obj_t *obj)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    return msgbox->status;
}

#endif // !__SGL_MSGBOX_H__
