/* source/widgets/sgl_arc.c
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
#include "sgl_arc.h"


/**
 * @brief Set the style of the arc
 * @param obj The arc object
 * @param type The style type
 * @param value The value of the style
 * @return None
 */
void sgl_arc_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_arc_t *arc = (sgl_arc_t*)obj;

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
        arc->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_BG_COLOR:
        arc->desc.bg_color = sgl_int2color(value);
        break;

    case SGL_STYLE_RADIUS:
        arc->desc.radius_out = value;
        break;

    case SGL_STYLE_ARC_WIDTH:
        arc->desc.radius_in = arc->desc.radius_out - value;
        break;

    case SGL_STYLE_ALPHA:
        arc->desc.alpha = value;
        break;

    case SGL_STYLE_CENTER_X_OFFSET:
        arc->desc.cx = obj->coords.x1 + value;
        break;

    case SGL_STYLE_CENTER_Y_OFFSET:
        arc->desc.cy = obj->coords.y1 + value;
        break;

    case SGL_STYLE_ARC_MODE:
        arc->desc.mode = value;
        break;

    case SGL_STYLE_ARC_START_ANGLE:
        arc->desc.start_angle = value;
        break;

    case SGL_STYLE_ARC_END_ANGLE:
        arc->desc.end_angle = value;
        break;

    default:
        SGL_LOG_WARN("sgl_arc_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief Get the style of the arc
 * @param obj The arc object
 * @param type The style type
 * @return The value of the style
 */
size_t sgl_arc_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_arc_t *arc = (sgl_arc_t*)obj;

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
        return sgl_color2int(arc->desc.color);

    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(arc->desc.bg_color);

    case SGL_STYLE_RADIUS:
        return arc->desc.radius_out;

    case SGL_STYLE_ARC_WIDTH:
        return arc->desc.radius_out - arc->desc.radius_in;

    case SGL_STYLE_ALPHA:
        return arc->desc.alpha;

    case SGL_STYLE_CENTER_X_OFFSET:
        return arc->desc.cx;

    case SGL_STYLE_CENTER_Y_OFFSET:
        return arc->desc.cy;

    case SGL_STYLE_ARC_MODE:
        return arc->desc.mode;

    case SGL_STYLE_ARC_START_ANGLE:
        return arc->desc.start_angle;

    case SGL_STYLE_ARC_END_ANGLE:
        return arc->desc.end_angle;

    default:
        SGL_LOG_WARN("sgl_arc_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


static void sgl_arc_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_arc_t *arc = (sgl_arc_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        if(arc->desc.start_angle == 0 && arc->desc.end_angle == 360) {
            sgl_draw_ring(surf, &arc->obj.area, (sgl_draw_ring_t*)(&arc->desc));
        }
        else {
            sgl_draw_arc(surf, &arc->obj.area, &arc->desc);
        }
    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_RELEASED) {
        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(SGL_EVENT_DRAW_INIT)
    {
        if(arc->desc.cx == -1) {
            arc->desc.cx = (obj->coords.x2 + obj->coords.x1) / 2;
        }

        if(arc->desc.cy == -1) {
            arc->desc.cy = (obj->coords.y2 + obj->coords.y1) / 2;
        }

        if(arc->desc.radius_out < 0) {
            arc->desc.radius_out = (obj->coords.x2 - obj->coords.x1) / 2;
        }

        if(arc->desc.radius_in < 0) {
            arc->desc.radius_in = arc->desc.radius_out - 2;
        }
    }
}


/**
 * @brief create an arc object
 * @param parent parent object
 * @return arc object
 */
sgl_obj_t* sgl_arc_create(sgl_obj_t* parent)
{
    sgl_arc_t *arc = sgl_malloc(sizeof(sgl_arc_t));
    if(arc == NULL) {
        SGL_LOG_ERROR("sgl_arc_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(arc, 0, sizeof(sgl_arc_t));

    sgl_obj_t *obj = &arc->obj;
    sgl_obj_init(&arc->obj, parent);
    obj->needinit = 1;

    arc->desc.alpha = SGL_THEME_ALPHA;
    arc->desc.mode = SGL_DRAW_ARC_NORMAL;
    arc->desc.color = SGL_THEME_BG_COLOR;
    arc->desc.bg_color = SGL_THEME_COLOR;
    arc->desc.start_angle = 0;
    arc->desc.end_angle = 360;
    arc->desc.radius_out = -1;
    arc->desc.radius_in = -1;
    arc->desc.cx = -1;
    arc->desc.cy = -1;

    obj->construct_fn = sgl_arc_construct_cb;
    obj->set_style = sgl_arc_set_style;
    obj->get_style = sgl_arc_get_style;

    return obj;
}
