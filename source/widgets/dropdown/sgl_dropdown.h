/* source/widgets/sgl_dropdown.h
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

#ifndef __SGL_DROPDOWN_H__
#define __SGL_DROPDOWN_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


typedef struct sgl_dropdown_option {
    struct sgl_dropdown_option *next;
    const char *text;
    const sgl_icon_pixmap_t *icon;
} sgl_dropdown_option_t;


/**
 * @brief sgl dropdown struct
 * @desc: text description
 */
typedef struct sgl_dropdown {
    sgl_obj_t       obj;
    sgl_draw_rect_t body_desc;
    const sgl_font_t *font;
    sgl_color_t     text_color;
    sgl_dropdown_option_t *head;
    int32_t         option_num;
    int32_t         selected;
    int16_t         option_h;
    int16_t         expand_h;
    sgl_dropdown_option_t *expend_start;
    bool            is_open;
}sgl_dropdown_t;


/**
 * @brief create a dropdown object
 * @param parent parent of the dropdown
 * @return dropdown object
 */
sgl_obj_t* sgl_dropdown_create(sgl_obj_t* parent);


/**
 * @brief set the style of the dropdown object
 * @param obj pointer to the dropdown object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_dropdown_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get the style of the dropdown object
 * @param obj pointer to the dropdown object
 * @param type style type
 * @return style value
 */
size_t sgl_dropdown_get_style(sgl_obj_t *obj, sgl_style_type_t type);


void sgl_dropdown_add_option(sgl_obj_t *obj, const char *text, const sgl_icon_pixmap_t *icon);

#endif // !__SGL_DROPDOWN_H__
