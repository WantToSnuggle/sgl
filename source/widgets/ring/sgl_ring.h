/* source/widgets/sgl_ring.h
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

#ifndef __SGL_RING_H__
#define __SGL_RING_H__


#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl ring
 * @obj: sgl general object
 * @desc: ring draw description
 */
typedef struct sgl_ring {
    sgl_obj_t       obj;
    sgl_draw_ring_t desc;
}sgl_ring_t;


/**
 * @brief Create a ring object
 * @param parent The parent object of the ring
 * @return The ring object
 */
sgl_obj_t* sgl_ring_create(sgl_obj_t* parent);


/**
 * @brief Set the style of the ring
 * @param obj The ring object
 * @param type The style type
 * @param value The value of the style
 * @return None
 */
void sgl_ring_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief Get the style of the ring
 * @param obj The ring object
 * @param type The style type
 * @return The value of the style
 */
size_t sgl_ring_get_style(sgl_obj_t *obj, sgl_style_type_t type);


#endif // !__SGL_RING_H__
