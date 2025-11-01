/* source/widgets/sgl_dropdown.c
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
#include "sgl_dropdown.h"


static const uint8_t dropdown_bitmap[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,
    0x0c,0xfa,0x00,0x00,0x00,0x00,0x05,0xee,0x50,
    0x0c,0xff,0xa0,0x00,0x00,0x00,0x5e,0xfe,0x30,
    0x00,0xcf,0xfa,0x00,0x00,0x05,0xef,0xe3,0x00,
    0x00,0x0c,0xff,0xa0,0x00,0x5e,0xfe,0x30,0x00,
    0x00,0x00,0xcf,0xfa,0x05,0xef,0xe3,0x00,0x00,
    0x00,0x00,0x0c,0xff,0xae,0xfe,0x30,0x00,0x00,
    0x00,0x00,0x00,0xcf,0xff,0xe3,0x00,0x00,0x00,
    0x00,0x00,0x00,0x0c,0xfe,0x30,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x53,0x00,0x00,0x00,0x00,
};


static const sgl_icon_pixmap_t dropdown_icon = { 
    .bitmap = dropdown_bitmap,
    .bpp = 4,
    .height = 10,
    .width = 18,
};


/**
 * @brief set the style of the dropdown object
 * @param obj pointer to the dropdown object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_dropdown_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;

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
        dropdown->body_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_ALPHA:
        dropdown->body_desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_RADIUS:
        dropdown->body_desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_PIXMAP:
        dropdown->body_desc.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_FONT:
        dropdown->font = (sgl_font_t*)value;
        break;

    case SGL_STYLE_TEXT_COLOR:
        dropdown->text_color = sgl_int2color(value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        dropdown->body_desc.border = (uint8_t)value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        dropdown->body_desc.color = sgl_int2color(value);
        break;

    default:
        SGL_LOG_WARN("sgl_dropdown_set_style: unsupported style type %d", type);
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get the style of the dropdown object
 * @param obj pointer to the dropdown object
 * @param type style type
 * @return style value
 */
size_t sgl_dropdown_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    //sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;

    switch((int)type) {
    case SGL_STYLE_POS_X:
        return sgl_obj_get_pos_x(obj);

    case SGL_STYLE_POS_Y:
        return sgl_obj_get_pos_y(obj);
    
    case SGL_STYLE_SIZE_W:
        return sgl_obj_get_width(obj);
    
    case SGL_STYLE_SIZE_H:
        return sgl_obj_get_height(obj);

    default:
        SGL_LOG_WARN("sgl_dropdown_get_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


// static void dropdown_open(sgl_dropdown_t *dropdown)
// {

// }


// static void dropdown_close(sgl_dropdown_t *dropdown)
// {

// }


static void sgl_dropdown_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    int16_t icon_h = dropdown->option_h;
    int16_t pos_x = 0, pos_y = 0, icon_y = 0;
    sgl_dropdown_option_t *option = dropdown->head;

    sgl_rect_t body_area = {
        .x1 = obj->coords.x1,
        .x2 = obj->coords.x2,
        .y1 = obj->coords.y1,
        .y2 = obj->coords.y1 + dropdown->option_h,
    };

    sgl_rect_t icon_area = {
        .x1 = obj->coords.x2 - dropdown_icon.width - 2,
        .x2 = obj->coords.x2 - 2,
        .y1 = obj->coords.y1,
        .y2 = obj->coords.y2,
    };

    sgl_rect_t text_area = {
        .x1 = obj->coords.x1 + 2,
        .x2 = obj->coords.x2 - dropdown_icon.width - 5,
        .y1 = obj->coords.y1,
        .y2 = obj->coords.y2,
    };

    icon_y = obj->coords.y1 + (icon_h - dropdown_icon.height) / 2;

    if (evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &body_area, &body_area, &dropdown->body_desc);
        sgl_draw_icon_with_alpha(surf, &icon_area, icon_area.x1, icon_y, dropdown->text_color, dropdown->body_desc.alpha, &dropdown_icon);

        for (int i = 0; option != NULL; i++) {
            if (i == dropdown->selected) {
                break;
            }
            option = option->next;
        }

        if (option != NULL) {
            pos_x = text_area.x1;
            pos_y = obj->coords.y1 + (icon_h - dropdown->font->font_height) / 2;
            if (option->icon != NULL) {
                icon_y = obj->coords.y1 + (icon_h - option->icon->height) / 2;
                sgl_draw_icon_with_alpha(surf, &text_area, pos_x, icon_y, dropdown->text_color, dropdown->body_desc.alpha, option->icon);
                pos_x += option->icon->width + 2;
            }

            sgl_draw_string_with_alpha(surf, &text_area, pos_x, pos_y, option->text, dropdown->text_color, dropdown->body_desc.alpha, dropdown->font, 1);
        }

        if (dropdown->is_open) {
            option = dropdown->expend_start;
            body_area.y1 = obj->coords.y1 + dropdown->option_h;
            body_area.y2 = body_area.y1 + dropdown->expand_h;
            sgl_draw_rect(surf, &body_area, &body_area, &dropdown->body_desc);
            pos_y += dropdown->option_h;

            for (int i = 0; option != NULL; i++) {
                pos_x = text_area.x1;
                if (option->icon != NULL) {
                    icon_y = pos_y + (dropdown->font->font_height - option->icon->height) / 2;
                    sgl_draw_icon_with_alpha(surf, &obj->area, pos_x, icon_y, dropdown->text_color, dropdown->body_desc.alpha, option->icon);
                    pos_x += option->icon->width + 2;
                }
                sgl_draw_string_with_alpha(surf, &obj->area, pos_x, pos_y, option->text, dropdown->text_color, dropdown->body_desc.alpha, dropdown->font, 1);
                option = option->next;
                pos_y += dropdown->font->font_height;
            }
        }
    }
    else if (evt->type == SGL_EVENT_MOVE_UP) {
        //dropdown->expend_start = dropdown->expend_start->next;
    }
    else if (evt->type == SGL_EVENT_MOVE_DOWN) {

    }
    else if (evt->type == SGL_EVENT_PRESSED) {
        dropdown->expand_h = dropdown->font->font_height * sgl_min(10, dropdown->option_num) + 5;

        if (dropdown->is_open) {
            dropdown->is_open = false;
            obj->coords.y2 -= dropdown->expand_h;
                SGL_LOG_INFO("sgl_dropdown_construct_cb: selected %d  %d  %d", evt->pos.y, obj->coords.y2, dropdown->font->font_height);
            if (evt->pos.y > obj->coords.y2) {
                dropdown->selected = (evt->pos.y - obj->coords.y2) / dropdown->font->font_height;
            }
        }
        else {
            dropdown->is_open = true;
            obj->coords.y2 += dropdown->expand_h;
        }

        sgl_obj_update_area(obj);
        dropdown->expend_start = dropdown->head;
    }
    else if (evt->type == SGL_EVENT_RELEASED) {

    }
    else if (evt->type == SGL_EVENT_DRAW_INIT) {
        dropdown->option_h = obj->coords.y2 - obj->coords.y1 + 1;
    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief create a dropdown object
 * @param parent parent of the dropdown
 * @return pointer to the dropdown object
 */
sgl_obj_t* sgl_dropdown_create(sgl_obj_t* parent)
{
    sgl_dropdown_t *dropdown = sgl_malloc(sizeof(sgl_dropdown_t));
    if(dropdown == NULL) {
        SGL_LOG_ERROR("sgl_dropdown_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(dropdown, 0, sizeof(sgl_dropdown_t));

    sgl_obj_t *obj = &dropdown->obj;
    sgl_obj_init(&dropdown->obj, parent);
    obj->construct_fn = sgl_dropdown_construct_cb;

#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_dropdown_set_style;
    obj->get_style = sgl_dropdown_get_style;
#endif
    sgl_obj_set_clickable(obj);
    sgl_obj_set_movable(obj);

    dropdown->body_desc.alpha = SGL_THEME_ALPHA;
    dropdown->body_desc.color = SGL_THEME_COLOR;
    dropdown->body_desc.radius = 5;
    dropdown->body_desc.border = 1;
    dropdown->body_desc.border_color = SGL_THEME_BORDER_COLOR;

    dropdown->head = NULL;
    dropdown->is_open = false;

    dropdown->selected = 0;

    return obj;
}


void sgl_dropdown_add_option(sgl_obj_t *obj, const char *text, const sgl_icon_pixmap_t *icon)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    sgl_dropdown_option_t *tail = dropdown->head;
    sgl_dropdown_option_t *add = sgl_malloc(sizeof(sgl_dropdown_option_t));

    if (add == NULL) {
        SGL_LOG_ERROR("sgl_dropdown_add_option: malloc failed");
        return;
    }

    if (tail == NULL) {
        dropdown->head = add;
    }
    else {
        while (tail != NULL) {
            if (tail->next == NULL) {
                tail->next = add;
                break;
            }
            tail = (tail)->next;
        }
    }

    dropdown->option_num ++;
    add->text = text;
    add->icon = icon;
    add->next = NULL;
}
