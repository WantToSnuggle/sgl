/* source/widgets/sgl_arc.h
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

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


typedef struct sgl_arc {
    sgl_obj_t       obj;
    sgl_draw_arc_t  desc;
    int16_t         width;
}sgl_arc_t;



/**
 * @brief create an arc object
 * @param parent parent object
 * @return arc object
 */
sgl_obj_t* sgl_arc_create(sgl_obj_t* parent);


/**
 * @brief Set the style of the arc
 * @param obj The arc object
 * @param type The style type
 * @param value The value of the style
 * @return None
 */
void sgl_arc_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief Get the style of the arc
 * @param obj The arc object
 * @param type The style type
 * @return The value of the style
 */
size_t sgl_arc_get_style(sgl_obj_t *obj, sgl_style_type_t type);

