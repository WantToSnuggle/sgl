/* source/widgets/sgl_line.h
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

#ifndef __SGL_LINE_H__
#define __SGL_LINE_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl line struct
 * @obj: sgl general object
 * @desc: pointer to line draw descriptor
 */
typedef struct sgl_line {
    sgl_obj_t       obj;
    sgl_draw_line_t desc;
}sgl_line_t;


/**
 * @brief create a line object
 * @param parent parent of the line
 * @return line object
 */
sgl_obj_t* sgl_line_create(sgl_obj_t* parent);


/**
 * @brief set line object style
 * @param obj line object
 * @param type style type
 * @param value style value
 */
void sgl_line_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get line object style
 * @param obj line object
 * @param type style type
 * @return style value
 */
size_t sgl_line_get_style(sgl_obj_t *obj, sgl_style_type_t type);


void sgl_line_set_start_point(sgl_obj_t *obj, int16_t x, int16_t y);
void sgl_line_set_end_point(sgl_obj_t *obj, int16_t x, int16_t y);
void sgl_line_set_width(sgl_obj_t *obj, uint16_t width);


#endif // !__SGL_LINE_H__
