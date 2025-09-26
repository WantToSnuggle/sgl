/* source/widgets/sgl_checkbox.h
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

#ifndef __SGL_CHECKBOX_H__
#define __SGL_CHECKBOX_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


typedef struct sgl_checkbox {
    sgl_obj_t          obj;
    sgl_draw_text_t    text;
    bool               status;
}sgl_checkbox_t;


/**
 * @brief create a checkbox object
 * @param parent parent of the checkbox
 * @return checkbox object
 */
sgl_obj_t* sgl_checkbox_create(sgl_obj_t* parent);


/**
 * @brief set style of checkbox
 * @param obj checkbox object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_checkbox_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get style of checkbox
 * @param obj checkbox object
 * @param type style type
 * @return style value
 */
size_t sgl_checkbox_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_CHECKBOX_H__
