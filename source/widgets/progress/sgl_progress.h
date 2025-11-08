/* source/widgets/sgl_progress.h
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

#ifndef __SGL_PROGRESS_H__
#define __SGL_PROGRESS_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl progress struct
 * @obj: sgl general object
 * @value: progress value
 * @desc: progress description
 */
typedef struct sgl_progress {
    sgl_obj_t          obj;
    sgl_draw_rect_t    body;
    sgl_color_t        color;
    uint8_t            value;
    uint8_t            alpha;
    uint8_t            interval;
    uint8_t            shift;
    uint8_t            knob_radius;
    uint8_t            knob_width;
}sgl_progress_t;


/**
 * @brief create a progress object
 * @param parent parent object of the progress
 * @return progress object
 */
sgl_obj_t* sgl_progress_create(sgl_obj_t* parent);


/**
 * @brief set progress object style
 * @param obj progress object
 * @param type style type
 * @param value style value
 */
void sgl_progress_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get progress object style
 * @param obj progress object
 * @param type style type
 * @return style value
 */
size_t sgl_progress_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_PROGRESS_H__
