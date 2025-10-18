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
    sgl_draw_text_t desc;
    sgl_draw_rect_t scroll_bg;
    sgl_draw_rect_t scroll_fg;
    int32_t         text_height: 31;
    int32_t         scroll_enable: 1;
}sgl_textbox_t;


/**
 * @brief create a textbox object
 * @param parent parent of the textbox
 * @return textbox object
 */
sgl_obj_t* sgl_textbox_create(sgl_obj_t* parent);


/**
 * @brief set the style of the textline object
 * @param obj pointer to the textline object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_textline_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get the style of the textline object
 * @param obj pointer to the textline object
 * @param type style type
 * @return style value
 */
size_t sgl_textline_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_TEXTBOX_H__
