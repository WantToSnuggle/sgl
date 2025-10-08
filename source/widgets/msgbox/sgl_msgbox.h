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

    sgl_draw_text_t  title_desc;
    sgl_rect_t       title_coords;

    sgl_draw_line_t  title_line_desc;

    sgl_draw_text_t  text_desc;
    sgl_rect_t       text_coords;

    uint8_t          status;

    sgl_draw_text_t  apply_text;
    sgl_rect_t       apply_coords;
    sgl_draw_text_t  close_text;
    sgl_rect_t       close_coords;
    sgl_area_t       button_coords;
}sgl_msgbox_t;


/**
 * @brief create a message box object
 * @param parent parent of the message box
 * @return message box object
 */
sgl_obj_t* sgl_msgbox_create(sgl_obj_t* parent);


/**
 * @brief Set style of message box
 * @param obj pointer to message box object
 * @param type style type
 * @param value value of style
 * @return none
 */
void sgl_msgbox_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief Get style of message box
 * @param obj pointer to message box object
 * @param type style type
 * @return value of style
 */
size_t sgl_msgbox_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_MSGBOX_H__
