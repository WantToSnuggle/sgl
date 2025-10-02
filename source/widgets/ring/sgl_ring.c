/* source/widgets/sgl_ring.c
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
#include "sgl_ring.h"


/**
 * @brief Set the style of the ring
 * @param obj The ring object
 * @param type The style type
 * @param value The value of the style
 * @return None
 */
void sgl_ring_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;

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
        ring->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_RADIUS:
        ring->desc.radius_out = value;
        break;

    case SGL_STYLE_RING_WIDTH:
        ring->desc.radius_in = ring->desc.radius_out - value;
        break;

    case SGL_STYLE_ALPHA:
        ring->desc.alpha = value;
        break;

    case SGL_STYLE_CENTER_X_OFFSET:
        ring->desc.cx = obj->coords.x1 + value;
        break;

    case SGL_STYLE_CENTER_Y_OFFSET:
        ring->desc.cy = obj->coords.y1 + value;
        break;

    default:
        SGL_LOG_WARN("sgl_ring_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief Get the style of the ring
 * @param obj The ring object
 * @param type The style type
 * @return The value of the style
 */
size_t sgl_ring_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;

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
        return sgl_color2int(ring->desc.color);

    case SGL_STYLE_RADIUS:
        return ring->desc.radius_out;

    case SGL_STYLE_RING_WIDTH:
        return ring->desc.radius_out - ring->desc.radius_in;

    case SGL_STYLE_ALPHA:
        return ring->desc.alpha;

    case SGL_STYLE_CENTER_X_OFFSET:
        return ring->desc.cx;

    case SGL_STYLE_CENTER_Y_OFFSET:
        return ring->desc.cy;

    default:
        SGL_LOG_WARN("sgl_ring_set_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


static void sgl_ring_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_ring(surf, &obj->area, &ring->desc);
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {

        if(ring->desc.cx == -1) {
            ring->desc.cx = (obj->coords.x2 + obj->coords.x1) / 2;
        }

        if(ring->desc.cy == -1) {
            ring->desc.cy = (obj->coords.y2 + obj->coords.y1) / 2;
        }

        if(ring->desc.radius_out == -1) {
            ring->desc.radius_out = (obj->coords.x2 - obj->coords.x1) / 2;
        }

        if(ring->desc.radius_in == -1) {
            ring->desc.radius_in = ring->desc.radius_out - 2;
        }
    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief Create a ring object
 * @param parent The parent object of the ring
 * @return The ring object
 */
sgl_obj_t* sgl_ring_create(sgl_obj_t* parent)
{
    sgl_ring_t *ring = sgl_malloc(sizeof(sgl_ring_t));
    if(ring == NULL) {
        SGL_LOG_ERROR("sgl_ring_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(ring, 0, sizeof(sgl_ring_t));

    sgl_obj_t *obj = &ring->obj;
    sgl_obj_init(&ring->obj, parent);
    obj->construct_fn = sgl_ring_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_ring_set_style;
    obj->get_style = sgl_ring_get_style;
#endif
    obj->needinit = 1;

    ring->desc.radius_in = -1;
    ring->desc.radius_out = -1;
    ring->desc.cx = -1;
    ring->desc.cy = -1;
    ring->desc.alpha = SGL_THEME_ALPHA;
    ring->desc.color = SGL_THEME_COLOR;

    return obj;
}
