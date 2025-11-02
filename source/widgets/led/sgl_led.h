/* source/widgets/sgl_led.h
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

#ifndef __SGL_LED_H__
#define __SGL_LED_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl led struct
 * @obj: sgl general object
 * @desc: pointer to led draw descriptor
 */
typedef struct sgl_led {
    sgl_obj_t       obj;
    int16_t         cx;
    int16_t         cy;
    uint8_t         alpha;
    bool            status;
    sgl_color_t     color;
    sgl_color_t     bg_color;

}sgl_led_t;


/**
 * @brief create a led object
 * @param parent parent of the led
 * @return led object
 */
sgl_obj_t* sgl_led_create(sgl_obj_t* parent);


/**
 * @brief set led object style
 * @param obj led object
 * @param type style type
 * @param value style value
 */
void sgl_led_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get led object style
 * @param obj led object
 * @param type style type
 * @return style value
 */
size_t sgl_led_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_LED_H__
