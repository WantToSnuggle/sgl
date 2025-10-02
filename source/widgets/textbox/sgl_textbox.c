/* source/widgets/sgl_textbox.c
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
#include <string.h>
#include "sgl_textbox.h"


static void textbox_scroll_set_pos(sgl_textbox_t *textbox, int16_t pos)
{
    int16_t bg_h = textbox->scroll_bg_coords.y2 - textbox->scroll_bg_coords.y1 + 1;
    int16_t scroll_h = bg_h / 8;
    int16_t scroll_y = (bg_h - scroll_h) * pos / 100;

    textbox->scroll_fg_coords.x1 = textbox->scroll_bg_coords.x1;
    textbox->scroll_fg_coords.y1 = textbox->scroll_bg_coords.y1 + scroll_y;
    textbox->scroll_fg_coords.x2 = textbox->scroll_bg_coords.x2;
    textbox->scroll_fg_coords.y2 = textbox->scroll_fg_coords.y1 + scroll_h;
}


/**
 * @brief set the style of the textbox object
 * @param obj pointer to the textbox object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_textbox_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;

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
        textbox->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_BG_COLOR:
        textbox->desc.bg_flag = 1;
        textbox->desc.bg_color = sgl_int2color(value);
        break;

    case SGL_STYLE_PIXMAP:
        textbox->bg.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_ALPHA:
        textbox->desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_RADIUS:
        textbox->desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_TEXT:
        textbox->desc.text = (char*)value;
        break;

    case SGL_STYLE_ALIGN:
        textbox->desc.align = (uint8_t)value;
        break;

    case SGL_STYLE_TEXT_COLOR:
        textbox->desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_FONT:
        textbox->desc.font = (sgl_font_t*)value;
        break;
    
    case SGL_STYLE_ICON:
        textbox->desc.icon = (sgl_icon_pixmap_t*)value;
        break;

    case SGL_STYLE_TEXT_MARGIN:
        textbox->desc.margin = (int16_t)value;
        break;

    case SGL_STYLE_TEXT_X_OFFSET:
        textbox->desc.x_offset = (int16_t)value;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        textbox->desc.y_offset = (int16_t)value;
        break;

    case SGL_STYLE_LINE_SPACE:
        textbox->desc.line_space = (int16_t)value;
        break;

    case SGL_STYLE_BG_TRANSPARENT:
        textbox->desc.bg_flag = (value == 1 ? 0 : 1);
        break;

    default:
        SGL_LOG_WARN("sgl_textbox_set_style: unsupported style type %d", type);
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get the style of the textbox object
 * @param obj pointer to the textbox object
 * @param type style type
 * @return style value
 */
size_t sgl_textbox_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;

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
        return sgl_color2int(textbox->desc.color);

    case SGL_STYLE_BG_COLOR:
        return sgl_color2int(textbox->desc.bg_color);

    case SGL_STYLE_ALPHA:
        return textbox->desc.alpha;
    
    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_TEXT:
        return (size_t)textbox->desc.text;

    case SGL_STYLE_ALIGN:
        return textbox->desc.align;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(textbox->desc.color);

    case SGL_STYLE_FONT:
        return (size_t)textbox->desc.font;
    
    case SGL_STYLE_ICON:
        return (size_t)textbox->desc.icon;

    case SGL_STYLE_TEXT_MARGIN:
        return textbox->desc.margin;

    case SGL_STYLE_TEXT_X_OFFSET:
        return textbox->desc.x_offset;
        break;

    case SGL_STYLE_TEXT_Y_OFFSET:
        return textbox->desc.y_offset;

    case SGL_STYLE_LINE_SPACE:
        return textbox->desc.line_space;

    case SGL_STYLE_BG_TRANSPARENT:
        return textbox->desc.bg_flag;

    default:
        SGL_LOG_WARN("sgl_textbox_get_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


static void sgl_textbox_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    //int16_t height = obj->coords.y2 - obj->coords.y1 + 1;
    int16_t width = obj->coords.x2 - obj->coords.x1 + 1;

    sgl_rect_t text_coords = {
        .x1 = obj->coords.x1,
        .y1 = obj->coords.y1,
        .x2 = obj->coords.x2 - 5,
        .y2 = obj->coords.y2,
    };

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &obj->coords, &textbox->bg);
        sgl_draw_text(surf, &obj->area, &text_coords, &textbox->desc);
        sgl_draw_rect(surf, &obj->area, &textbox->scroll_bg_coords, &textbox->scroll_bg);
        sgl_draw_rect(surf, &obj->area, &textbox->scroll_fg_coords, &textbox->scroll_fg);
    }
    else if(evt->move == SGL_EVENT_MOVE_UP) {
        //SGL_LOG_INFO("sgl_textbox_construct_cb: move up == =%d", textbox->text_height);
        // if(textbox->desc.y_offset < textbox->text_height) {
        //     uint8_t scroll_pos = textbox->desc.y_offset  / textbox->text_height;

        //     textbox->desc.y_offset -= evt->distance;
        //     SGL_LOG_INFO("textbox->desc.y_offset == =%d", scroll_pos);

        //     textbox_scroll_set_pos(textbox, 50);
        // }
    }
    else if(evt->move == SGL_EVENT_MOVE_DOWN) {
        if(textbox->desc.y_offset <= 0) {
            textbox->desc.y_offset += evt->distance;
        }
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        textbox->text_height = sgl_font_get_string_width(textbox->desc.text, textbox->desc.font);
        SGL_LOG_INFO("sgl_textbox_construct_cb: text_height == =%d", textbox->text_height);
        textbox->text_height = textbox->text_height / width * (sgl_font_get_height(textbox->desc.font) + textbox->desc.line_space);

        textbox->scroll_bg_coords.x1 = textbox->obj.coords.x2 - 5;
        textbox->scroll_bg_coords.y1 = textbox->obj.coords.y1;
        textbox->scroll_bg_coords.x2 = textbox->obj.coords.x2;
        textbox->scroll_bg_coords.y2 = textbox->obj.coords.y2;

        textbox_scroll_set_pos(textbox, 0);
    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief create a textbox object
 * @param parent parent of the textbox
 * @return pointer to the textbox object
 */
sgl_obj_t* sgl_textbox_create(sgl_obj_t* parent)
{
    sgl_textbox_t *textbox = sgl_malloc(sizeof(sgl_textbox_t));
    if(textbox == NULL) {
        SGL_LOG_ERROR("sgl_textbox_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(textbox, 0, sizeof(sgl_textbox_t));

    sgl_obj_t *obj = &textbox->obj;
    sgl_obj_init(&textbox->obj, parent);
    obj->construct_fn = sgl_textbox_construct_cb;

#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_textbox_set_style;
    obj->get_style = sgl_textbox_get_style;
#endif
    obj->needinit = 1;
    sgl_obj_set_clickable(obj);
    sgl_obj_set_movable(obj);

    textbox->bg.alpha = SGL_ALPHA_MAX;

    textbox->scroll_bg.alpha = SGL_ALPHA_MAX;
    textbox->scroll_bg.color = SGL_WHITE;
    // textbox->scroll_bg.radius = 2;

    textbox->scroll_fg.alpha = SGL_ALPHA_MAX;
    textbox->scroll_fg.color = SGL_GRAY;
    // textbox->scroll_fg.radius = 2;

    textbox->desc.alpha = SGL_ALPHA_MAX;
    textbox->desc.bg_flag = true;
    textbox->desc.bg_color = SGL_BLACK;
    textbox->desc.color = SGL_WHITE;
    textbox->desc.line_space = 1;
    textbox->desc.mode = SGL_DRAW_TEXT_LINES;
    textbox->desc.text = "textbox";
    textbox->desc.margin = 2;

    return obj;
}

