/* source/widgets/sgl_circle.c
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
#include "sgl_circle.h"


/**
 * @brief set style of the circle object
 * @param obj pointer to the object
 * @param type style type
 * @param value value of the style
 * @return none
 */
void sgl_circle_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_circle_t *circle = (sgl_circle_t*)obj;

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
        circle->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_RADIUS:
        circle->desc.radius = value;
        break;

    case SGL_STYLE_ALPHA:
        circle->desc.alpha = value;
        break;
    
    case SGL_STYLE_BORDER_WIDTH:
        circle->desc.border = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        circle->desc.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_PIXMAP:
        circle->desc.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_CENTER_X_OFFSET:
        circle->desc.cx = obj->coords.x1 + value;
        break;

    case SGL_STYLE_CENTER_Y_OFFSET:
        circle->desc.cy = obj->coords.y1 + value;
        break;

    default:
        SGL_LOG_WARN("sgl_circle_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty flag */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get style of the circle object
 * @param obj pointer to the object
 * @param type style type
 * @return style value
 */
size_t sgl_circle_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_circle_t *circle = (sgl_circle_t*)obj;

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
        return sgl_color2int(circle->desc.color);

    case SGL_STYLE_RADIUS:
        return circle->desc.radius;

    case SGL_STYLE_ALPHA:
        return circle->desc.alpha;

    case SGL_STYLE_BORDER_WIDTH:
        return circle->desc.border;

    case SGL_STYLE_BORDER_COLOR:
        return sgl_color2int(circle->desc.border_color);

    case SGL_STYLE_PIXMAP:
        return (size_t)circle->desc.pixmap;

    case SGL_STYLE_CENTER_X_OFFSET:
        return circle->desc.cx;

    case SGL_STYLE_CENTER_Y_OFFSET:
        return circle->desc.cy;

    default:
        SGL_LOG_WARN("sgl_circle_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief construct function of the circle object
 * @param surf pointer to the surface
 * @param obj pointer to the object
 * @param evt pointer to the event
 * @return none
 */
static void sgl_circle_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_circle_t *circle = (sgl_circle_t*)obj;
    
    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_circle(surf, &obj->area, &circle->desc);
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        if(circle->desc.cx == -1) {
            circle->desc.cx = (circle->obj.coords.x1 + circle->obj.coords.x2) / 2;
        }

        if(circle->desc.cy == -1) {
            circle->desc.cy = (circle->obj.coords.y1 + circle->obj.coords.y2) / 2;
        }

        if(circle->desc.radius == -1) {
            circle->desc.radius = (circle->obj.coords.y2 - circle->obj.coords.y1) / 2;
        }
    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief create a circle object
 * @param parent parent of the object
 * @return pointer to the object
 */
sgl_obj_t* sgl_circle_create(sgl_obj_t* parent)
{
    sgl_circle_t *circle = sgl_malloc(sizeof(sgl_circle_t));
    if(circle == NULL) {
        SGL_LOG_ERROR("sgl_circle_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(circle, 0, sizeof(sgl_circle_t));

    sgl_obj_t *obj = &circle->obj;
    sgl_obj_init(&circle->obj, parent);
    obj->construct_fn = sgl_circle_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_circle_set_style;
    obj->get_style = sgl_circle_get_style;
#endif
    obj->needinit = 1;

    circle->desc.alpha = SGL_ALPHA_MAX;
    circle->desc.color = SGL_THEME_COLOR;
    circle->desc.pixmap = NULL;
    circle->desc.border = SGL_THEME_BORDER_WIDTH;
    circle->desc.border_color = SGL_THEME_BORDER_COLOR;
    circle->desc.cx = -1;
    circle->desc.cy = -1;
    circle->desc.radius = -1;

    return obj;
}
