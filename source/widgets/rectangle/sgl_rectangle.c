/* source/widgets/sgl_rectangle.c
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
#include <sgl_theme.h>
#include <sgl_cfgfix.h>
#include <string.h>
#include "sgl_rectangle.h"


/**
 * @brief set style of rectangle object
 * @param  obj: object
 * @param  type: style type
 * @param  value: style value
 * @retval none
 */
void sgl_rect_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t*)obj; 

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
        rect->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_RADIUS:
        rect->desc.radius = sgl_obj_fix_radius(obj, value);
        break;
    
    case SGL_STYLE_ALPHA:
        rect->desc.alpha = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        rect->desc.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        rect->desc.border = value;
        break;

    case SGL_STYLE_PIXMAP:
        rect->desc.pixmap = (sgl_pixmap_t*)value;
        break;

    default:
        SGL_LOG_WARN("sgl_rectangle_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get style of rectangle object
 * @param  obj: object
 * @param  type: style type
 * @retval style value
 */
size_t sgl_rect_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t*)obj; 

    switch((int)type) {
    case SGL_STYLE_POS_X:
        return sgl_obj_get_pos_x(obj);

    case SGL_STYLE_POS_Y:
        return sgl_obj_get_pos_y(obj);
    
    case SGL_STYLE_SIZE_W:
        return sgl_obj_get_width(obj);
    
    case SGL_STYLE_SIZE_H:
        return sgl_obj_get_height(obj);

    case SGL_STYLE_COLOR:
        return sgl_color2int(rect->desc.color);
    
    case SGL_STYLE_RADIUS:
        return rect->desc.radius;
    
    case SGL_STYLE_ALPHA:
        return rect->desc.alpha;

    case SGL_STYLE_BORDER_COLOR:
        return sgl_color2int(rect->desc.border_color);
    
    case SGL_STYLE_BORDER_WIDTH:
        return rect->desc.border;
    
    case SGL_STYLE_PIXMAP:
        return (size_t)rect->desc.pixmap;

    default:
        SGL_LOG_WARN("sgl_rectangle_set_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief rectangle construct callback
 * @param  surf: surface
 * @param  obj: object
 * @param  evt: event parameter
 * @retval none
 */
static void sgl_rectangle_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_rectangle_t *rect = (sgl_rectangle_t*)obj;
        sgl_draw_rect(surf, &obj->area, &obj->coords, &rect->desc);
    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        if(sgl_obj_is_flexible(obj)) {
            obj->coords.x1 -= 2;
            obj->coords.x2 += 2;
            obj->coords.y1 -= 2;
            obj->coords.y2 += 2;
        }

        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_RELEASED) {
        if(sgl_obj_is_flexible(obj)) {
            obj->coords.x1 += 2;
            obj->coords.x2 -= 2;
            obj->coords.y1 += 2;
            obj->coords.y2 -= 2;
        }

        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
}


/**
 * @brief  create a rectangle
 * @param  parent: parent object
 * @retval rectangle object
 */
sgl_obj_t* sgl_rect_create(sgl_obj_t* parent)
{
    sgl_rectangle_t *rect = sgl_malloc(sizeof(sgl_rectangle_t));
    if(rect == NULL) {
        SGL_LOG_ERROR("sgl_rect_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(rect, 0, sizeof(sgl_rectangle_t));

    sgl_obj_t *obj = &rect->obj;
    sgl_obj_init(&rect->obj, parent);
    sgl_obj_set_unflexible(obj);
    
    obj->construct_fn = sgl_rectangle_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_rect_set_style;
    obj->get_style = sgl_rect_get_style;
#endif
    rect->desc.alpha = SGL_THEME_ALPHA;
    rect->desc.color = SGL_THEME_COLOR;
    rect->desc.border = SGL_THEME_BORDER_WIDTH;
    rect->desc.border_color = SGL_THEME_BORDER_COLOR;
    rect->desc.pixmap = NULL;

    return obj;
}
