/* source/widgets/sgl_slider.c
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
#include "sgl_slider.h"


/**
 * @brief set slider object style
 * @param obj slider object
 * @param type style type
 * @param value style value
 */
void sgl_slider_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_slider_t *slider = (sgl_slider_t*)obj;

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
        slider->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_BG_COLOR:
        slider->desc.bg_color = sgl_int2color(value);
        break;

    case SGL_STYLE_ALPHA:
        slider->desc.alpha = value;
        break;

    case SGL_STYLE_RADIUS:
        slider->desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        slider->desc.border = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        slider->desc.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_DIRECTION:
        slider->desc.direct = value;
        break;

    case SGL_STYLE_VALUE:
        slider->value = value;
        break;

    default:
        SGL_LOG_WARN("sgl_slider_set_style: unsupported style type %d", type);
    }

    /* set object dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get slider object style
 * @param obj slider object
 * @param type style type
 * @return style value
 */
size_t sgl_slider_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_slider_t *slider = (sgl_slider_t*)obj;

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
        return sgl_color2int(slider->desc.color);

    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(slider->desc.bg_color);

    case SGL_STYLE_ALPHA:
        return slider->desc.alpha;

    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_BORDER_WIDTH:
        return slider->desc.border;

    case SGL_STYLE_BORDER_COLOR:
        return sgl_color2int(slider->desc.border_color);

    case SGL_STYLE_VALUE:
        return slider->value;

    default:
        SGL_LOG_WARN("sgl_slider_set_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


static void sgl_slider_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_slider_t *slider = (sgl_slider_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        if(slider->desc.direct == SGL_DIRECT_HORIZONTAL) {
            slider->desc.knob_offset = (slider->obj.coords.x2 - slider->obj.coords.x1) * slider->value / 100;
        }
        else {
            slider->desc.knob_offset = (slider->obj.coords.y2 - slider->obj.coords.y1) * slider->value / 100;
        }
        sgl_draw_bar(surf, &obj->area, &obj->coords, &slider->desc);
    }
    else if(evt->type == SGL_EVENT_PRESSED ||
        evt->type == SGL_EVENT_MOVE_DOWN || evt->type == SGL_EVENT_MOVE_UP || evt->type == SGL_EVENT_MOVE_LEFT || evt->type == SGL_EVENT_MOVE_RIGHT
    ) {
        if(slider->desc.direct == SGL_DIRECT_HORIZONTAL) {
            slider->value = (evt->pos.x - obj->coords.x1) * 100 / (obj->coords.x2 - obj->coords.x1);
        }
        else {
            slider->value = (obj->coords.y2 - evt->pos.y) * 100 / (obj->coords.y2 - obj->coords.y1);
        }

        if(evt->type == SGL_EVENT_PRESSED) {
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
        obj->coords.x1 += 2;
        obj->coords.x2 -= 2;
        obj->coords.y1 += 2;
        obj->coords.y2 -= 2;

        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        int16_t h = obj->coords.y2 - obj->coords.y1 + 1;
        int16_t w = obj->coords.x2 - obj->coords.x1 + 1;

        slider->desc.radius = sgl_min(h, w) / 2;
    }
}


/**
 * @brief create a slider object
 * @param parent parent object of the slider
 * @return slider object
 */
sgl_obj_t* sgl_slider_create(sgl_obj_t* parent)
{
    sgl_slider_t *slider = sgl_malloc(sizeof(sgl_slider_t));
    if(slider == NULL) {
        SGL_LOG_ERROR("sgl_slider_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(slider, 0, sizeof(sgl_slider_t));

    sgl_obj_t *obj = &slider->obj;
    sgl_obj_init(&slider->obj, parent);
    sgl_obj_set_clickable(obj);
    sgl_obj_set_movable(obj);
    obj->construct_fn = sgl_slider_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_slider_set_style;
    obj->get_style = sgl_slider_get_style;
#endif
    obj->needinit = 1;

    slider->desc.direct = SGL_DIRECT_HORIZONTAL;
    slider->desc.alpha = SGL_THEME_ALPHA;
    slider->desc.color = sgl_color_mixer(SGL_THEME_BG_COLOR, SGL_THEME_COLOR, 128);
    slider->desc.bg_color = SGL_THEME_COLOR;
    slider->desc.border = SGL_THEME_BORDER_WIDTH;
    slider->desc.border_color = SGL_THEME_BORDER_COLOR;

    return obj;
}

