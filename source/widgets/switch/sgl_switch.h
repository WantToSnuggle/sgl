/* source/widgets/sgl_switch.h
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

#ifndef __SGL_SWITCH_H__
#define __SGL_SWITCH_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl switch struct
 * @obj: sgl general object
 * @bg_desc: pointer to sgl_draw_rect_t descriptor
 * @knob_desc: pointer to sgl_draw_circle_t descriptor
 * @statue: switch status, false: off, true: on
 */
typedef struct sgl_switch {
    sgl_obj_t         obj;
    sgl_draw_rect_t   bg_desc;
    sgl_draw_rect_t   knob_desc;
    sgl_area_t        knob_rect;
    sgl_color_t       color;
    sgl_color_t       bg_color;
    bool              status;
}sgl_switch_t;


/**
 * @brief create a switch object
 * @param parent parent of the switch
 * @return switch object
 */
sgl_obj_t* sgl_switch_create(sgl_obj_t* parent);


/**
 * @brief set switch status
 * @param obj switch object
 * @param status switch status, true means on, false means off
 * @return none
 */
void sgl_switch_set_status(sgl_obj_t *obj, bool status);


/**
 * @brief  Get switch status
 * @param  obj: switch object
 * @return switch status, true means on, false means off
 */
bool sgl_switch_get_status(sgl_obj_t *obj);


/**
 * @brief set switch object style
 * @param obj switch object
 * @param type style type
 * @param value style value
 */
void sgl_switch_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get switch object style
 * @param obj switch object
 * @param type style type
 * @return style value
 */
size_t sgl_switch_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_SWITCH_H__
