/* source/widgets/sgl_listview.h
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

#ifndef __SGL_LISTVIEW_H__
#define __SGL_LISTVIEW_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl listview
 * @obj: sgl general object
 * @desc: listview draw description
 */
typedef struct sgl_listview {  
    sgl_obj_t           obj;
    sgl_draw_rect_t     desc;
    sgl_font_t          *font;
    int16_t             item_height;
    int16_t             item_count;
    int16_t             item_selected;
    int16_t             coords_offset;
}sgl_listview_t;


/**
 * @brief create listview object
 * @param parent pointer to the parent object
 * @return pointer to the listview object
 */
sgl_obj_t* sgl_listview_create(sgl_obj_t* parent);


/**
 * @brief set listview object style
 * @param obj pointer to the object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_listview_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);


/**
 * @brief get listview object style
 * @param obj pointer to the object
 * @param type style type
 * @return style value
 */
size_t sgl_listview_get_style(sgl_obj_t *obj, sgl_style_type_t type);


sgl_obj_t* sgl_listview_add_item(sgl_obj_t *listview, sgl_icon_pixmap_t *icon, const char *text);


void sgl_listview_set_child_style(sgl_obj_t *listview, sgl_style_type_t type, size_t value);


void sgl_listview_set_offset(sgl_obj_t *listview, int16_t offset);

#endif // !__SGL_LISTVIEW_H__
