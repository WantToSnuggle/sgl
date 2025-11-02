/* source/widgets/sgl_label.c
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
#include "sgl_label.h"


/**
 * @brief set the style of the label object
 * @param obj pointer to the label object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_label_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_label_t *label = (sgl_label_t*)obj;
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
        label->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_BG_COLOR:
        label->desc.bg_flag = 1;
        label->desc.bg_color = sgl_int2color(value);
        break;

    case SGL_STYLE_ALPHA:
        label->desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_RADIUS:
        label->desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_TEXT:
        label->desc.text = (char*)value;
        break;

    case SGL_STYLE_ALIGN:
        label->desc.align = value & 0x1F;
        break;

    case SGL_STYLE_TEXT_COLOR:
        label->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_FONT:
        label->desc.font = (sgl_font_t*)value;
        break;
    
    case SGL_STYLE_ICON:
        label->desc.icon = (sgl_icon_pixmap_t*)value;
        break;

    case SGL_STYLE_TEXT_MARGIN:
        label->desc.margin = (int16_t)value;
        break;

    case SGL_STYLE_TEXT_X_OFFSET:
        label->desc.x_offset = (int16_t)value;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        label->desc.y_offset = (int16_t)value;
        break;

    case SGL_STYLE_LINE_SPACE:
        label->desc.line_space = (int16_t)value;
        break;

    case SGL_STYLE_BG_TRANSPARENT:
        label->desc.bg_flag = (value == 1) ? 0 : 1;
        break;

    default:
        SGL_LOG_WARN("sgl_label_set_style: unsupported style type %d", type);
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get the style of the label object
 * @param obj pointer to the label object
 * @param type style type
 * @return style value
 */
size_t sgl_label_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_label_t *label = (sgl_label_t*)obj;
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
        return sgl_color2int(label->desc.color);

    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(label->desc.bg_color);

    case SGL_STYLE_ALPHA:
        return label->desc.alpha;
    
    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_TEXT:
        return (size_t)label->desc.text;

    case SGL_STYLE_ALIGN:
        return label->desc.align;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(label->desc.color);

    case SGL_STYLE_FONT:
        return (size_t)label->desc.font;
    
    case SGL_STYLE_ICON:
        return (size_t)label->desc.icon;

    case SGL_STYLE_TEXT_MARGIN:
        return label->desc.margin;

    case SGL_STYLE_TEXT_X_OFFSET:
        return label->desc.x_offset;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        return label->desc.y_offset;

    case SGL_STYLE_LINE_SPACE:
        return label->desc.line_space;

    case SGL_STYLE_BG_TRANSPARENT:
        return label->desc.bg_flag;

    default:
        SGL_LOG_WARN("sgl_label_set_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief construct the label object
 * @param surf pointer to the surface
 * @param obj pointer to the label object
 * @param evt pointer to the event
 * @return none
 */
static void sgl_label_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    if (evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_label_t *label = (sgl_label_t*)obj;
        sgl_draw_text(surf, &obj->area, &obj->coords, &label->desc);
    }
    else if (evt->type == SGL_EVENT_PRESSED || evt->type == SGL_EVENT_RELEASED) {
        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
}


/**
 * @brief create a label object
 * @param parent parent of the label
 * @return pointer to the label object
 */
sgl_obj_t* sgl_label_create(sgl_obj_t* parent)
{
    sgl_label_t *label = sgl_malloc(sizeof(sgl_label_t));
    if(label == NULL) {
        SGL_LOG_ERROR("sgl_label_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(label, 0, sizeof(sgl_label_t));

    sgl_obj_t *obj = &label->obj;
    sgl_obj_init(&label->obj, parent);
    obj->construct_fn = sgl_label_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_label_set_style;
    obj->get_style = sgl_label_get_style;
#endif
    label->desc.alpha = SGL_ALPHA_MAX;
    label->desc.bg_flag = 0;
    label->desc.margin = 0;
    label->desc.mode = SGL_DRAW_TEXT_NORMAL;
    label->desc.color = SGL_THEME_TEXT_COLOR;
    label->desc.text = "";

    return obj;
}
