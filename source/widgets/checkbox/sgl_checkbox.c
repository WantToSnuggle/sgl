/* source/widgets/sgl_checkbox.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Li, Shanwen  (1477153217@qq.com)
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
#include <sgl_config.h>
#include <string.h>
#include "sgl_checkbox.h"


/**
 * @brief set style of checkbox
 * @param obj checkbox object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_checkbox_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_checkbox_t *checkbox = (sgl_checkbox_t*)(obj);

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
        checkbox->select_bg.border_color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_BG_COLOR:
        checkbox->select_bg.color = sgl_int2color(value);
        break;

    case SGL_STYLE_RADIUS:
        int r_min = sgl_obj_fix_radius(obj, value);
        checkbox->select_bg.radius = r_min;
        checkbox->select.radius = r_min / 2;
        break;

    case SGL_STYLE_TEXT_COLOR:
        checkbox->text_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_FONT:
        checkbox->text_desc.font = (sgl_font_t*)value;
        break;
    
    case SGL_STYLE_TEXT:
        checkbox->text_desc.text = (char*)value;
        break;

    case SGL_STYLE_TEXT_X_OFFSET:
        checkbox->text_desc.x_offset = value;    
        break;
    
    case SGL_STYLE_CHECKBOX_STATUS:
        if(((bool)value) != checkbox->status) {
            checkbox->status = (bool)value;
        }
        break;

    case SGL_STYLE_STATUS:
        checkbox->status = (bool)value;
        break;

    default:
        SGL_LOG_WARN("sgl_checkbox_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty flag */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get style of checkbox
 * @param obj checkbox object
 * @param type style type
 * @return style value
 */
size_t sgl_checkbox_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_checkbox_t *checkbox = (sgl_checkbox_t*)(obj);

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
        return sgl_color2int(checkbox->select_bg.border_color);
    
    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(checkbox->select_bg.color);

    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(checkbox->text_desc.color);

    case SGL_STYLE_FONT:
        return (size_t)checkbox->text_desc.font;

    case SGL_STYLE_TEXT:
        return (size_t)checkbox->text_desc.text;

    case SGL_STYLE_TEXT_X_OFFSET:
        return checkbox->text_desc.x_offset;

    case SGL_STYLE_CHECKBOX_STATUS:
        return (size_t)checkbox->status;

    case  SGL_STYLE_STATUS:
        return (size_t)checkbox->status;

    default:
        SGL_LOG_WARN("sgl_checkbox_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief checkbox construct callback
 * @param surf surface pointer
 * @param obj checkbox object
 * @param evt event parameter
 * @return none
 */
static void sgl_checkbox_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_checkbox_t *checkbox = (sgl_checkbox_t*)(obj);
    int16_t h = obj->coords.y2 - obj->coords.y1 + 1;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &checkbox->select_bg_coords, &checkbox->select_bg);
        sgl_draw_rect(surf, &obj->area, &checkbox->select_coords, &checkbox->select);
        sgl_draw_text(surf, &obj->area, &checkbox->text_coords, &checkbox->text_desc);

    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        if(evt->pos.x < obj->coords.x1 + h) {
            checkbox->status = !checkbox->status;
            if(checkbox->status) {
                checkbox->select.color = checkbox->select_bg.border_color;
            }
            else {
                checkbox->select.color = checkbox->select_bg.color;
            }
        }
        else {
            sgl_obj_clear_dirty(obj);
            return;
        }

        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_RELEASED) {
        sgl_obj_clear_dirty(obj);
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        int16_t margin = (obj->coords.y2 - obj->coords.y1) / 4;

        checkbox->select_bg_coords.x1 = obj->coords.x1;
        checkbox->select_bg_coords.y1 = obj->coords.y1;
        checkbox->select_bg_coords.x2 = obj->coords.x1 + obj->coords.y2 - obj->coords.y1;
        checkbox->select_bg_coords.y2 = obj->coords.y2;

        checkbox->select_coords.x1 = obj->coords.x1 + margin;
        checkbox->select_coords.y1 = obj->coords.y1 + margin;
        checkbox->select_coords.x2 = checkbox->select_bg_coords.x2 - margin;
        checkbox->select_coords.y2 = obj->coords.y2 - margin;

        checkbox->text_coords.x1 = checkbox->select_bg_coords.x2 + 2;
        checkbox->text_coords.y1 = obj->coords.y1;
        checkbox->text_coords.x2 = obj->coords.x2;
        checkbox->text_coords.y2 = obj->coords.y2;
    }
}


/**
 * @brief create a checkbox object
 * @param parent parent of the checkbox
 * @return checkbox object
 */
sgl_obj_t* sgl_checkbox_create(sgl_obj_t* parent)
{
    sgl_checkbox_t *checkbox = sgl_malloc(sizeof(sgl_checkbox_t));
    if(checkbox == NULL) {
        SGL_LOG_ERROR("sgl_checkbox_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(checkbox, 0, sizeof(sgl_checkbox_t));

    sgl_obj_t *obj = &checkbox->obj;
    sgl_obj_init(&checkbox->obj, parent);
    obj->construct_fn = sgl_checkbox_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_checkbox_set_style;
    obj->get_style = sgl_checkbox_get_style;
#endif
    obj->needinit = 1;

    checkbox->status = false;
    checkbox->text_desc.text = "checkbox";
    checkbox->text_desc.alpha = SGL_ALPHA_MAX;
    checkbox->text_desc.mode = SGL_DRAW_TEXT_NORMAL;
    checkbox->text_desc.align = SGL_ALIGN_LEFT_MID;
    checkbox->text_desc.bg_flag = false;

    checkbox->select.alpha = SGL_ALPHA_MAX;
    checkbox->select.color = SGL_WHITE;
    checkbox->select_bg.alpha = SGL_ALPHA_MAX;
    checkbox->select_bg.color = SGL_WHITE;
    checkbox->select_bg.border = 2;
    checkbox->select_bg.border_color = SGL_BLACK;

    sgl_obj_set_clickable(obj);

    return obj;
}
