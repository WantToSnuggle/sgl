/* source/widgets/sgl_textline.c
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
#include "sgl_textline.h"


/**
 * @brief set the style of the textline object
 * @param obj pointer to the textline object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_textline_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_textline_t *textline = (sgl_textline_t*)obj;

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
        textline->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_BG_COLOR:
        textline->desc.bg_flag = 1;
        textline->desc.bg_color = sgl_int2color(value);
        break;

    case SGL_STYLE_ALPHA:
        textline->desc.alpha = (uint8_t)value;
        break;

    case SGL_STYLE_RADIUS:
        textline->desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_TEXT:
        textline->desc.text = (char*)value;
        sgl_obj_set_height(obj, sgl_font_get_string_height(&obj->coords, textline->desc.text, textline->desc.font, textline->desc.line_space, textline->desc.margin));
        break;

    case SGL_STYLE_ALIGN:
        textline->desc.align = (uint8_t)value;
        break;

    case SGL_STYLE_TEXT_COLOR:
        textline->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_FONT:
        textline->desc.font = (sgl_font_t*)value;
        break;
    
    case SGL_STYLE_ICON:
        textline->desc.icon = (sgl_icon_pixmap_t*)value;
        break;

    case SGL_STYLE_TEXT_MARGIN:
        textline->desc.margin = (int16_t)value;
        break;

    case SGL_STYLE_TEXT_X_OFFSET:
        textline->desc.x_offset = (int16_t)value;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        textline->desc.y_offset = (int16_t)value;
        break;

    case SGL_STYLE_LINE_SPACE:
        textline->desc.line_space = (int16_t)value;
        break;

    case SGL_STYLE_BG_TRANSPARENT:
        textline->desc.bg_flag = (value == 1 ? 0 : 1);
        break;

    default:
        SGL_LOG_WARN("sgl_textline_set_style: unsupported style type %d", type);
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get the style of the textline object
 * @param obj pointer to the textline object
 * @param type style type
 * @return style value
 */
size_t sgl_textline_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_textline_t *textline = (sgl_textline_t*)obj;

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
        return sgl_color2int(textline->desc.color);

    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(textline->desc.bg_color);

    case SGL_STYLE_ALPHA:
        return textline->desc.alpha;
    
    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_TEXT:
        return (size_t)textline->desc.text;

    case SGL_STYLE_ALIGN:
        return textline->desc.align;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(textline->desc.color);

    case SGL_STYLE_FONT:
        return (size_t)textline->desc.font;
    
    case SGL_STYLE_ICON:
        return (size_t)textline->desc.icon;

    case SGL_STYLE_TEXT_MARGIN:
        return textline->desc.margin;

    case SGL_STYLE_TEXT_X_OFFSET:
        return textline->desc.x_offset;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        return textline->desc.y_offset;

    case SGL_STYLE_LINE_SPACE:
        return textline->desc.line_space;

    case SGL_STYLE_BG_TRANSPARENT:
        return textline->desc.bg_flag;

    default:
        SGL_LOG_WARN("sgl_textline_get_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


static void sgl_textline_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_textline_t *textline = (sgl_textline_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_text(surf, &obj->area, &obj->coords, &textline->desc);
    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief create a textline object
 * @param parent parent of the textline
 * @return pointer to the textline object
 */
sgl_obj_t* sgl_textline_create(sgl_obj_t* parent)
{
    sgl_textline_t *textline = sgl_malloc(sizeof(sgl_textline_t));
    if(textline == NULL) {
        SGL_LOG_ERROR("sgl_textline_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(textline, 0, sizeof(sgl_textline_t));

    sgl_obj_t *obj = &textline->obj;
    sgl_obj_init(&textline->obj, parent);
    obj->construct_fn = sgl_textline_construct_cb;

#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_textline_set_style;
    obj->get_style = sgl_textline_get_style;
#endif

    textline->desc.alpha = SGL_THEME_ALPHA;
    textline->desc.bg_flag = true;
    textline->desc.bg_color = SGL_THEME_COLOR;
    textline->desc.color = SGL_THEME_TEXT_COLOR;
    textline->desc.line_space = 1;
    textline->desc.mode = SGL_DRAW_TEXT_LINES;
    textline->desc.text = "textline";
    textline->desc.radius = 5;
    textline->desc.margin = 2;

    return obj;
}

