/* source/widgets/sgl_switch.c
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
#include "sgl_switch.h"


/**
 * @brief set switch object style
 * @param obj switch object
 * @param type style type
 * @param value style value
 */
void sgl_switch_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_switch_t *p_switch = (sgl_switch_t*)(obj);

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
        p_switch->bg_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_BG_COLOR:
        p_switch->bg_color = sgl_int2color(value);
        break;

    case SGL_STYLE_KNOB_COLOR:
        p_switch->knob_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_RADIUS:
        p_switch->bg_desc.radius = sgl_obj_fix_radius(obj, value);
        p_switch->knob_desc.radius = p_switch->bg_desc.radius - p_switch->bg_desc.border - 1;
        break;

    case SGL_STYLE_ALPHA:
        p_switch->bg_desc.alpha = value;
        p_switch->knob_desc.alpha = value;
        break;

    case SGL_STYLE_BORDER_WIDTH:
        p_switch->bg_desc.border = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        p_switch->bg_desc.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_STATUS:
        p_switch->status = (bool)value;
        break;

    default:
        SGL_LOG_WARN("sgl_switch_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get switch object style
 * @param obj switch object
 * @param type style type
 * @return style value
 */
size_t sgl_switch_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_switch_t *p_switch = (sgl_switch_t*)(obj);

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
        return sgl_color2int(p_switch->bg_desc.color);
    
    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(p_switch->bg_color);

    case SGL_STYLE_KNOB_COLOR:
        return sgl_color2int(p_switch->knob_desc.color);

    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_ALPHA:
        return p_switch->bg_desc.alpha;

    case SGL_STYLE_BORDER_WIDTH:
        return p_switch->bg_desc.border;

    case SGL_STYLE_BORDER_COLOR:
        return sgl_color2int(p_switch->bg_desc.border_color);

    case SGL_STYLE_STATUS:
        return p_switch->status;

    default:
        SGL_LOG_WARN("sgl_switch_set_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


static void sgl_switch_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_switch_t *p_switch = (sgl_switch_t*)(obj);
    sgl_draw_rect_t *bg_desc = &p_switch->bg_desc;
    sgl_draw_rect_t *knob_desc = &p_switch->knob_desc;
    int16_t width = obj->coords.y2 - obj->coords.y1 - 2 * bg_desc->border;
    sgl_rect_t *knob_rect = &p_switch->knob_rect;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        if(p_switch->status) {
            bg_desc->color = p_switch->color;
            knob_rect->x2 = obj->coords.x2 - bg_desc->border;
            knob_rect->x1 = knob_rect->x2 - width;
        }
        else {
            bg_desc->color = p_switch->bg_color;
            knob_rect->x1 = obj->coords.x1 + bg_desc->border;
            knob_rect->x2 = knob_rect->x1 + width;
        }

        sgl_draw_rect(surf, &obj->area, &obj->coords, &p_switch->bg_desc);
        sgl_draw_rect(surf, &obj->area, knob_rect, knob_desc);
    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        p_switch->status = !p_switch->status;

        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        p_switch->color = bg_desc->color;
        knob_rect->y1 = obj->coords.y1 + bg_desc->border;
        knob_rect->y2 = obj->coords.y2 - bg_desc->border;

        if(p_switch->bg_desc.radius == -1) {
            p_switch->bg_desc.radius = sgl_obj_fix_radius(obj, (obj->coords.y2 - obj->coords.y1) / 2);
            knob_desc->radius = bg_desc->radius - bg_desc->border - 1;
        }
    }
}


/**
 * @brief create a switch object
 * @param parent parent of the switch
 * @return switch object
 */
sgl_obj_t* sgl_switch_create(sgl_obj_t* parent)
{
    sgl_switch_t *p_switch = sgl_malloc(sizeof(sgl_switch_t));
    if(p_switch == NULL) {
        SGL_LOG_ERROR("sgl_switch_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(p_switch, 0, sizeof(sgl_switch_t));

    sgl_obj_t *obj = &p_switch->obj;
    sgl_obj_init(&p_switch->obj, parent);
    obj->needinit = 1;
    obj->construct_fn = sgl_switch_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_switch_set_style;
    obj->get_style = sgl_switch_get_style;
#endif
    p_switch->bg_desc.alpha = SGL_THEME_ALPHA;
    p_switch->bg_desc.color = SGL_THEME_COLOR;
    p_switch->bg_desc.border_color = SGL_THEME_BORDER_COLOR;
    p_switch->bg_desc.border = SGL_THEME_BORDER_WIDTH;
    p_switch->bg_desc.radius = -1;

    p_switch->knob_desc.alpha = SGL_THEME_ALPHA;
    p_switch->knob_desc.color = sgl_color_mixer(SGL_THEME_COLOR, SGL_THEME_BG_COLOR, 128);
    p_switch->knob_desc.border = 0;

    p_switch->status = false;
    p_switch->bg_color = SGL_THEME_BG_COLOR;

    sgl_obj_set_clickable(obj);

    return obj;
}


/**
 * @brief set switch status
 * @param obj switch object
 * @param status switch status, true means on, false means off
 * @return none
 */
void sgl_switch_set_status(sgl_obj_t *obj, bool status)
{
    sgl_check_ptr_break(obj);
    sgl_switch_t *p_switch = (sgl_switch_t*)obj;
    if(status == p_switch->status) {
        return;
    }

    p_switch->status = status;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief  Get switch status
 * @param  obj: switch object
 * @return switch status, true means on, false means off
 */
bool sgl_switch_get_status(sgl_obj_t *obj)
{
    sgl_check_ptr_return(obj, false);
    return ((sgl_switch_t*)obj)->status;
}
