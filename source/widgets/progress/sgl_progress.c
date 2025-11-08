/* source/widgets/sgl_progress.c
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
#include "sgl_progress.h"


/**
 * @brief set progress object style
 * @param obj progress object
 * @param type style type
 * @param value style value
 */
void sgl_progress_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_progress_t *progress = (sgl_progress_t*)obj;

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
        progress->color = sgl_int2color(value);
        break;

    case SGL_STYLE_BG_COLOR:
        progress->body.color = sgl_int2color(value);
        break;

    case SGL_STYLE_ALPHA:
        progress->body.alpha = value;
        break;

    case SGL_STYLE_RADIUS:
        progress->body.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_PIXMAP:
        progress->body.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_BORDER_WIDTH:
        progress->body.border = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        progress->body.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_PROGRESS_FILL_ALPHA:
        progress->alpha = value;
        break;
    
    case SGL_STYLE_PROGRESS_FILL_RADIUS:
        progress->knob_radius = sgl_min(value, obj->radius);
        break;

    case SGL_STYLE_PROGRESS_FILL_WIDTH:
        progress->knob_width = value;
        break;

    case SGL_STYLE_VALUE:
        progress->value = value;
        break;

    default:
        SGL_LOG_WARN("sgl_progress_set_style: unsupported style type %d", type);
    }

    /* set object dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get progress object style
 * @param obj progress object
 * @param type style type
 * @return style value
 */
size_t sgl_progress_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_progress_t *progress = (sgl_progress_t*)obj;

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
        return sgl_color2int(progress->color);

    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(progress->body.color);

    case SGL_STYLE_ALPHA:
        return progress->body.alpha;

    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_PIXMAP:
        return (size_t)progress->body.pixmap;

    case SGL_STYLE_BORDER_WIDTH:
        return progress->body.border;

    case SGL_STYLE_BORDER_COLOR:
        return sgl_color2int(progress->body.border_color);

    case SGL_STYLE_SLIDER_KNOB_ALPHA:
        return progress->alpha;

    case SGL_STYLE_VALUE:
        return progress->value;

    default:
        SGL_LOG_WARN("sgl_progress_set_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


static void sgl_progress_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_progress_t *progress = (sgl_progress_t*)obj;
    sgl_area_t knob = obj->coords;
    knob.x1 = obj->coords.x1 + obj->radius / 2 + 2;
    sgl_area_t rect = {
        .x1 = obj->coords.x1 - progress->interval * 2 + progress->shift,
        .y1 = obj->coords.y1 + 2,
        .x2 = 0,
        .y2 = obj->coords.y2 - 2,
    };

    if (progress->shift > progress->interval * 2) {
        progress->shift = 0;
    }
    progress->shift ++;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        knob.x2 = obj->coords.x1 + (obj->coords.x2 - obj->coords.x1) * progress->value / 100;

        sgl_draw_rect(surf, &obj->area, &obj->coords, &progress->body);

        while (rect.x2 <= knob.x2) {
            rect.x2 = rect.x1 + progress->knob_width;
            sgl_draw_fill_round_rect_with_alpha(surf, &knob, &rect, progress->knob_radius, progress->color, progress->alpha);
            rect.x1 = rect.x2 + progress->interval;
        }
    }
}


/**
 * @brief create a progress object
 * @param parent parent object of the progress
 * @return progress object
 */
sgl_obj_t* sgl_progress_create(sgl_obj_t* parent)
{
    sgl_progress_t *progress = sgl_malloc(sizeof(sgl_progress_t));
    if(progress == NULL) {
        SGL_LOG_ERROR("sgl_progress_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(progress, 0, sizeof(sgl_progress_t));

    sgl_obj_t *obj = &progress->obj;
    sgl_obj_init(&progress->obj, parent);
    sgl_obj_set_clickable(obj);
    sgl_obj_set_movable(obj);
    obj->construct_fn = sgl_progress_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_progress_set_style;
    obj->get_style = sgl_progress_get_style;
#endif
    progress->body.alpha = SGL_THEME_ALPHA;
    progress->body.color = SGL_THEME_BG_COLOR;
    progress->body.border = SGL_THEME_BORDER_WIDTH;
    progress->body.border_color = SGL_THEME_BORDER_COLOR;

    progress->color = SGL_THEME_COLOR;
    progress->alpha = SGL_THEME_ALPHA;
    progress->knob_width = 4;
    progress->interval = 4;
    progress->value = 50;
    progress->knob_radius = 2;

    return obj;
}
