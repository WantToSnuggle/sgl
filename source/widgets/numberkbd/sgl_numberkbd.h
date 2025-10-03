/* source/widgets/sgl_numberkbd.h
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

#ifndef __SGL_NUMBERKBD_H__
#define __SGL_NUMBERKBD_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl number keyboard struct
 * @obj: sgl general object
 * @body_desc: pointer to sgl_draw_rect_t descriptor
 */
typedef struct sgl_numberkbd {
    sgl_obj_t        obj;
    sgl_draw_rect_t  body_desc;
    sgl_draw_rect_t  btn_desc;
    sgl_font_t       *font;
    sgl_color_t      text_color;
    uint8_t          margin;
    uint8_t          opcode;
}sgl_numberkbd_t;


/**
 * @brief create numberkbd object
 * @param parent parent of object
 * @return numberkbd object
 */
sgl_obj_t* sgl_numberkbd_create(sgl_obj_t* parent);


/**
 * @brief Set style of numberkbd
 * @param obj numberkbd object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_numberkbd_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief Get style of numberkbd
 * @param obj numberkbd object
 * @param type style type
 * @return style value
 */
size_t sgl_numberkbd_get_style(sgl_obj_t *obj, sgl_style_type_t type);


/**
 * @brief get numberkbd opcode
 * @param obj numberkbd object
 * @return opcode [0 ~ 255]
 */
uint8_t sgl_numberkbd_get_opcode(sgl_obj_t *obj);


#endif // !__SGL_NUMBERKBD_H__
