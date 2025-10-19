/* source/widgets/sgl_listview.c
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
#include "sgl_listview.h"
#include <sgl.h>


/**
 * @brief set listview object style
 * @param obj pointer to the object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_listview_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_listview_t *listview = (sgl_listview_t*)obj; 

    switch((int)type) {
    case SGL_STYLE_POS_X:
        sgl_obj_set_pos_x(obj, value);
        break;

    case SGL_STYLE_POS_Y:
        sgl_obj_set_pos_y(obj, value);
        break;
    
    case SGL_STYLE_SIZE_W:
        sgl_obj_set_width(obj, value);
        break;
    
    case SGL_STYLE_SIZE_H:
        sgl_obj_set_height(obj, value);
        break;

    case SGL_STYLE_COLOR:
        listview->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_RADIUS:
        listview->desc.radius = value;
        break;
    
    case SGL_STYLE_ALPHA:
        listview->desc.alpha = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        listview->desc.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        listview->desc.border = value;
        break;

    case SGL_STYLE_PIXMAP:
        listview->desc.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_FONT:
        listview->font = (sgl_font_t*)value;
        break;

    default:
        SGL_LOG_WARN("sgl_listview_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get listview object style
 * @param obj pointer to the object
 * @param type style type
 * @return style value
 */
size_t sgl_listview_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_listview_t *listview = (sgl_listview_t*)obj;

    switch((int)type) {
    case SGL_STYLE_POS_X:
        return sgl_obj_get_pos_x(obj);

    case SGL_STYLE_POS_Y:
        return sgl_obj_get_pos_y(obj);
    
    case SGL_STYLE_SIZE_W:
        return sgl_obj_get_width(obj);
    
    case SGL_STYLE_SIZE_H:
        return sgl_obj_get_height(obj);

    case SGL_STYLE_FONT:
        return (size_t)listview->font;

    default:
        SGL_LOG_WARN("sgl_listview_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


static void sgl_listview_event_cb(sgl_event_t *event)
{
    sgl_listview_t *listview = (sgl_listview_t*)event->param;
    sgl_obj_t *parent = &listview->obj;
    sgl_obj_t *child = NULL;

    if(event->type == SGL_EVENT_RELEASED) {
        //int diff = item_height + listview->coords_offset;
        child = parent->child;
        if(listview->coords_offset > 0) {
            sgl_obj_for_each_child(child, parent) {
                child->coords.y1 -= listview->coords_offset;
                child->coords.y2 -= listview->coords_offset;
                child->area.y1 -= listview->coords_offset;
                child->area.y2 -= listview->coords_offset;
            }
            listview->coords_offset = 0;
        }
        // else if(child->coords.y2 < item_height) {
        //     diff = item_height - child->coords.y2;
        //     sgl_obj_for_each_child(child, parent) {
        //         child->coords.y1 += diff;
        //         child->coords.y2 += diff;
        //         child->area.y1 += diff;
        //         child->area.y2 += diff;
        //     }
        //     listview->coords_offset = (-diff);
        // }
        sgl_obj_set_dirty(parent);
    }
}


static void sgl_listview_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_listview_t *listview = (sgl_listview_t*)obj;
    //int coords_end = 0;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &obj->coords, &listview->desc);
    }
    else if(evt->type == SGL_EVENT_MOVE_UP) {
        sgl_listview_set_offset(obj, - evt->distance);
    }
    else if(evt->type == SGL_EVENT_MOVE_DOWN) {
        sgl_listview_set_offset(obj, evt->distance);
    }
    else if(evt->type == SGL_EVENT_RELEASED) {

    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief create listview object
 * @param parent pointer to the parent object
 * @return pointer to the listview object
 */
sgl_obj_t* sgl_listview_create(sgl_obj_t* parent)
{
    sgl_listview_t *listview = sgl_malloc(sizeof(sgl_listview_t));
    if(listview == NULL) {
        SGL_LOG_ERROR("sgl_listview_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(listview, 0, sizeof(sgl_listview_t));

    sgl_obj_t *obj = &listview->obj;
    sgl_obj_init(&listview->obj, parent);
    obj->construct_fn = sgl_listview_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_listview_set_style;
    obj->get_style = sgl_listview_get_style;
#endif
    sgl_obj_set_movable(obj);

    listview->desc.alpha = SGL_ALPHA_MAX;
    listview->desc.border = 0;
    listview->desc.radius = 0;
    listview->font = NULL;

    listview->item_count = 0;
    listview->item_height = 23;
    listview->item_selected = -1;

    sgl_obj_set_event_cb(obj, sgl_listview_event_cb, (size_t)listview);

    return obj;
}


sgl_obj_t* sgl_listview_add_item(sgl_obj_t *listview, sgl_icon_pixmap_t *icon, const char *text)
{
    sgl_listview_t *plistview = (sgl_listview_t *)listview;

    sgl_obj_t *new_obj = sgl_button_create(listview);
    new_obj->coords.x1 = listview->coords.x1;
    new_obj->coords.x2 = listview->coords.x2;
    new_obj->coords.y1 = listview->coords.y1 + plistview->item_count * plistview->item_height;
    new_obj->coords.y2 = new_obj->coords.y1 + plistview->item_height;

    sgl_button_set_style(new_obj, SGL_STYLE_TEXT, (size_t)text);
    sgl_button_set_style(new_obj, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_COLOR_BLACK));
    sgl_button_set_style(new_obj, SGL_STYLE_ALIGN, SGL_ALIGN_LEFT_MID);
    sgl_button_set_style(new_obj, SGL_STYLE_TEXT_X_OFFSET, 10);
    if(icon) {
        sgl_button_set_style(new_obj, SGL_STYLE_ICON, (size_t)icon);
    }
    sgl_button_set_style(new_obj, SGL_STYLE_FONT, (size_t)plistview->font);
    sgl_button_set_style(new_obj, SGL_STYLE_BORDER_WIDTH, 1);
    sgl_button_set_style(new_obj, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_COLOR_BLACK));
    sgl_button_set_style(new_obj, SGL_STYLE_COLOR, SGL_COLOR(SGL_COLOR_BLUE));

    sgl_obj_set_event_cb(new_obj, sgl_listview_event_cb, (size_t)plistview);

    plistview->item_count ++;

    return new_obj;
}


void sgl_listview_set_child_style(sgl_obj_t *listview, sgl_style_type_t type, size_t value)
{
    sgl_obj_t *child = NULL;

    sgl_obj_for_each_child(child, listview) {
        sgl_button_set_style(child, type, value);
    }
}


void sgl_listview_set_offset(sgl_obj_t *listview, int16_t offset)
{
    sgl_listview_t *plistview = (sgl_listview_t *)listview;
    plistview->coords_offset += offset;
    sgl_obj_t *child = NULL;

    sgl_obj_for_each_child(child, listview) {
        child->coords.y1 += offset;
        child->coords.y2 += offset;
        child->area.y1 += offset;
        child->area.y2 += offset;
    }
}
