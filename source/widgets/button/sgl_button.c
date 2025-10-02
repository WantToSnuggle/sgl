/* source/widgets/sgl_button.c
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
#include <sgl_theme.h>
#include <string.h>
#include "sgl_button.h"


/**
 * @brief  set button style
 * @param  obj: object pointer
 * @param  type: style type
 * @param  value: style value
 */
void sgl_button_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_button_t *button = (sgl_button_t*)obj;

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
        button->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_RADIUS:
        button->desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_ALPHA:
        button->text_desc.alpha = value;
        button->desc.alpha = value;
        break;

    case SGL_STYLE_PIXMAP:
        button->desc.pixmap = (sgl_pixmap_t*)value;
        break;
    
    case SGL_STYLE_TEXT:
        button->text_desc.text = (char*)value;
        break;
    
    case SGL_STYLE_ICON:
        button->text_desc.icon = (sgl_icon_pixmap_t*)value;
        break;

    case SGL_STYLE_ALIGN:
        button->text_desc.align = value;
        break;
    
    case SGL_STYLE_TEXT_COLOR:
        button->text_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_TEXT_ALPHA:
        button->text_desc.alpha = value;
        break;

    case SGL_STYLE_TEXT_MARGIN:
        button->text_desc.margin = value;
        break;

    case SGL_STYLE_TEXT_X_OFFSET:
        button->text_desc.x_offset = value;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        button->text_desc.y_offset = value;
        break;

    case SGL_STYLE_FONT:
        button->text_desc.font = (sgl_font_t*)value;
        break;

    case SGL_STYLE_BORDER_WIDTH:
        button->desc.border = value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        button->desc.border_color = sgl_int2color(value);
        break;

    default:
        SGL_LOG_WARN("sgl_button_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty flag */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief  get button style
 * @param  obj: object pointer
 * @param  type: style type
 * @return style value
 */
size_t sgl_button_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_button_t *button = (sgl_button_t*)obj;

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
        return sgl_color2int(button->desc.color);

    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_ALPHA:
        return button->desc.alpha;

    case SGL_STYLE_PIXMAP:
        return (size_t)button->desc.pixmap;

    case SGL_STYLE_TEXT:
        return (size_t)button->text_desc.text;

    case SGL_STYLE_ICON:
        return (size_t)button->text_desc.icon;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(button->text_desc.color);

    case SGL_STYLE_TEXT_ALPHA:
        return button->text_desc.alpha;

    case SGL_STYLE_TEXT_MARGIN:
        return button->text_desc.margin;

    case SGL_STYLE_TEXT_X_OFFSET:
        return button->text_desc.x_offset;

    case SGL_STYLE_TEXT_Y_OFFSET:
        return button->text_desc.y_offset;

    case SGL_STYLE_FONT:
        return (size_t)button->text_desc.font;

    case SGL_STYLE_BORDER_WIDTH:
        return button->desc.border;

    case SGL_STYLE_BORDER_COLOR:
        return sgl_color2int(button->desc.border_color);

    default:
        SGL_LOG_WARN("sgl_button_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief  button construct callback
 * @param  surf: surface pointer
 * @param  obj: object pointer
 * @param  evt: event pointer
 * @return none
 * @note   this function is called when the object is created or redraw
 */
static void sgl_button_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_button_t *button = (sgl_button_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &obj->coords, &button->desc);

        if(button->text_desc.text) {
            sgl_draw_text(surf, &obj->area, &obj->coords, &button->text_desc);
        }
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
 * @brief  create a button object
 * @param  parent: parent object
 * @return object pointer
 * @note  if parent is NULL, the button object will be created at the top level.
 */
sgl_obj_t* sgl_button_create(sgl_obj_t* parent)
{
    sgl_button_t *button = sgl_malloc(sizeof(sgl_button_t));
    if(button == NULL) {
        SGL_LOG_ERROR("sgl_button_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(button, 0, sizeof(sgl_button_t));

    sgl_obj_t *obj = &button->obj;
    sgl_obj_init(&button->obj, parent);
    sgl_obj_set_clickable(obj);
    sgl_obj_set_flexible(obj);

    obj->construct_fn = sgl_button_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_button_set_style;
    obj->get_style = sgl_button_get_style;
#endif
    button->desc.alpha = SGL_THEME_ALPHA;
    button->desc.color = SGL_THEME_COLOR;
    button->desc.border = SGL_THEME_BORDER_WIDTH;
    button->desc.border_color = SGL_THEME_BORDER_COLOR;
    button->desc.pixmap = NULL;
    button->desc.radius = 0;

    button->text_desc.alpha = SGL_THEME_ALPHA;
    button->text_desc.bg_flag = 0;
    button->text_desc.bg_color = SGL_THEME_BG_COLOR;
    button->text_desc.color = SGL_THEME_TEXT_COLOR;
    button->text_desc.text = NULL;
    button->text_desc.radius = 0;
    button->text_desc.font = NULL;
    button->text_desc.margin = 0;
    button->text_desc.icon = NULL;

    return obj;
}
