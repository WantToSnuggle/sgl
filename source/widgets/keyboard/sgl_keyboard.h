/* source/widgets/sgl_keyboard.h
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

#ifndef __SGL_KEYBOARD_H__
#define __SGL_KEYBOARD_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl keyboard struct
 * @obj: sgl general object
 * @body_desc: pointer to sgl_draw_rect_t descriptor
 */
typedef struct sgl_keyboard {
    sgl_obj_t        obj;
    sgl_draw_rect_t  body_desc;
    sgl_draw_rect_t  btn_desc;
    const sgl_font_t       *font;
    char             *edit;
    sgl_color_t      text_color;
    uint8_t          opcode;
    uint8_t          key_margin;
    int8_t           key_index;
    uint8_t          key_mode;
    uint32_t         edit_max_len;
} sgl_keyboard_t;


/**
 * @brief create keyboard object
 * @param parent parent of object
 * @return keyboard object
 */
sgl_obj_t* sgl_keyboard_create(sgl_obj_t* parent);


/**
 * @brief set keyboard color
 * @param obj keyboard object
 * @param color color
 * @return none
 */
static inline void sgl_keyboard_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->body_desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard alpha
 * @param obj keyboard object
 * @param alpha alpha [0 ~ 255]
 * @return none
 */
static inline void sgl_keyboard_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->body_desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard radius
 * @param obj keyboard object
 * @param radius radius [0 ~ 255]
 * @return none
 */
static inline void sgl_keyboard_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->body_desc.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard pixmap
 * @param obj keyboard object
 * @param pixmap pixmap
 * @return none
 */
static inline void sgl_keyboard_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->body_desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard border color
 * @param obj keyboard object
 * @param color border color
 * @return none
 * 
 */
static inline void sgl_keyboard_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->body_desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard border width
 * @param obj keyboard object
 * @param width border width [0 ~ 255]
 * @return none
 */
static inline void sgl_keyboard_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard text color
 * @param obj keyboard object
 * @param color text color
 * @return none
 */
static inline void sgl_keyboard_set_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->text_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard text font
 * @param obj keyboard object
 * @param font text font
 * @return none
 */
static inline void sgl_keyboard_set_text_font(sgl_obj_t *obj, const sgl_font_t *font)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->font = font;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard button radius
 * @param obj keyboard object
 * @param radius button radius [0 ~ 255]
 * @return none
 */
static inline void sgl_keyboard_set_btn_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.radius = radius;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard button alpha
 * @param obj keyboard object
 * @param alpha button alpha [0 ~ 255]
 * @return none
 */
static inline void sgl_keyboard_set_btn_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard button color
 * @param obj keyboard object
 * @param color button color
 * @return none
 */
static inline void sgl_keyboard_set_btn_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard button pixmap
 * @param obj keyboard object
 * @param pixmap button pixmap
 * @return none
 */
static inline void sgl_keyboard_set_btn_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard button border color
 * @param obj keyboard object
 * @param color button border color
 * @return none
 */
static inline void sgl_keyboard_set_btn_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set keyboard button border width
 * @param obj keyboard object
 * @param width button border width [0 ~ 255]
 * @return none
 */
static inline void sgl_keyboard_set_btn_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->btn_desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief get keyboard opcode
 * @param obj keyboard object
 * @return opcode [0 ~ 255]
 */
static inline uint8_t sgl_keyboard_get_opcode(sgl_obj_t *obj)
{
    return ((sgl_keyboard_t*)obj)->opcode;
}


/**
 * @brief set keyboard text buffer
 * @param obj keyboard object
 * @param buffer edit buffer
 * @param buf_max_len edit buffer max length
 */
static inline void sgl_keyboard_set_textarea(sgl_obj_t *obj, char *buffer, int buf_max_len)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->edit = buffer;
    keyboard->edit_max_len = buf_max_len;
}


#endif // !__SGL_KEYBOARD_H__
