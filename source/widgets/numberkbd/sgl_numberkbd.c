/* source/widgets/sgl_numberkbd.c
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
#include <sgl_theme.h>
#include <sgl_cfgfix.h>
#include <sgl_draw_cg.h>
#include <string.h>
#include "sgl_numberkbd.h"



/**
 * @brief Set style of numberkbd
 * @param obj numberkbd object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_numberkbd_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_numberkbd_t *numberkbd = (sgl_numberkbd_t*)obj;
    int16_t w = (obj->coords.x2 - obj->coords.x1 + 1) / 2;
    int16_t h = (obj->coords.y2 - obj->coords.y1 + 1) / 2;
    int16_t r_min = w > h ? h : w;

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

    case SGL_STYLE_NUMBERKBD_BG_COLOR:
        numberkbd->body_desc.color = sgl_int2color(value);
        break;    

    case SGL_STYLE_COLOR:
        numberkbd->body_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_ALPHA:
        numberkbd->body_desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_RADIUS:
        numberkbd->body_desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_RADIUS:
        numberkbd->btn_desc.radius = sgl_min((int16_t)value, r_min);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_COLOR:
        numberkbd->btn_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_PIXMAP:
        numberkbd->btn_desc.pixmap = (sgl_pixmap_t*)(value);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_ALPHA:
        numberkbd->btn_desc.alpha = (uint8_t)value;
        break;

    case SGL_STYLE_NUMBERKBD_BTN_MARGIN:
        numberkbd->margin = (uint8_t)value;
        break;

    case SGL_STYLE_TEXT_COLOR:
        numberkbd->text_color = sgl_int2color(value);
        break;

    case SGL_STYLE_NUMBERKBD_TEXT_COLOR:
        numberkbd->text_color = sgl_int2color(value);
        break;

    case SGL_STYLE_FONT:
        numberkbd->font = (sgl_font_t*)value;
        break;

    case SGL_STYLE_PIXMAP:
        numberkbd->body_desc.pixmap = (sgl_pixmap_t*)value;
        break;

    default:
        SGL_LOG_WARN("sgl_numberkbd_set_style: unsupported style type %d", type);
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief Get style of numberkbd
 * @param obj numberkbd object
 * @param type style type
 * @return style value
 */
size_t sgl_numberkbd_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_numberkbd_t *numberkbd = (sgl_numberkbd_t*)obj;
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
        return sgl_color2int(numberkbd->body_desc.color);

    case SGL_STYLE_ALPHA:
        return numberkbd->body_desc.alpha;
    
    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(numberkbd->text_color);

    case SGL_STYLE_FONT:
        return (size_t)numberkbd->font;

    default:
        SGL_LOG_WARN("sgl_numberkbd_set_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief numberkbd constructor function
 * @param surf: pointer to surface
 * @param obj: pointer to numberkbd object
 * @param evt: pointer to event 
 * @return none
 */
static void sgl_numberkbd_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    static const char digits[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', 'b'};

    sgl_numberkbd_t *numberkbd = (sgl_numberkbd_t*)obj;
    int16_t body_w = obj->coords.x2 - obj->coords.x1 + 1;
    int16_t body_h = obj->coords.y2 - obj->coords.y1 + 1;

    int16_t box_w = (body_w - 4 * numberkbd->margin) / 3;
    int16_t box_h = (body_h - 5 * numberkbd->margin) / 4;
    int16_t text_x = 0, text_y = 0, index = 0;

    sgl_rect_t btn = {
        .y1 = obj->coords.y1 + numberkbd->margin,
        .x2 = obj->coords.x1 + numberkbd->margin + box_w,
        .y2 = obj->coords.y1 + numberkbd->margin + box_h,
    };

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &obj->coords, &numberkbd->body_desc);

        for(; btn.y1 < (obj->coords.y2 - numberkbd->margin); btn.y1 += (box_h + numberkbd->margin), btn.y2 = (btn.y1 + box_h)) {
            btn.x1 = obj->coords.x1 + numberkbd->margin;
            text_y = btn.y1 + ((box_h - sgl_font_get_height(numberkbd->font)) / 2);

            for(btn.x2 = (btn.x1 + box_w); btn.x1 < (obj->coords.x2 - numberkbd->margin) ; btn.x1 += (box_w + numberkbd->margin), btn.x2 = (btn.x1 + box_w)) {
                sgl_draw_rect(surf, &obj->area, &btn, &numberkbd->btn_desc);
                text_x = btn.x1 + ((box_w -  sgl_font_get_string_width("0", numberkbd->font)) / 2);
                sgl_draw_character_with_alpha(surf, &obj->area, text_x, text_y, digits[index] - 32, numberkbd->text_color, numberkbd->btn_desc.alpha, numberkbd->font);
                index ++;
            }
        }
    }
    else if(evt->type == SGL_EVENT_PRESSED) {

    }
    else if(evt->type == SGL_EVENT_RELEASED) {

    }

    if(obj->event_fn) {
        obj->event_fn(evt);
    }
}


/**
 * @brief create numberkbd object
 * @param parent parent of object
 * @return numberkbd object
 */
sgl_obj_t* sgl_numberkbd_create(sgl_obj_t* parent)
{
    sgl_numberkbd_t *numberkbd = sgl_malloc(sizeof(sgl_numberkbd_t));
    if(numberkbd == NULL) {
        SGL_LOG_ERROR("sgl_numberkbd_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(numberkbd, 0, sizeof(sgl_numberkbd_t));

    sgl_obj_t *obj = &numberkbd->obj;
    sgl_obj_init(&numberkbd->obj, parent);
    obj->construct_fn = sgl_numberkbd_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_numberkbd_set_style;
    obj->get_style = sgl_numberkbd_get_style;
#endif
    obj->clickable = 1;
    
    numberkbd->body_desc.alpha = SGL_THEME_ALPHA;
    numberkbd->body_desc.color = SGL_THEME_BG_COLOR;
    numberkbd->body_desc.radius = SGL_THEME_RADIUS;
    numberkbd->body_desc.border = SGL_THEME_BORDER_WIDTH;
    numberkbd->body_desc.border_color = SGL_THEME_BORDER_COLOR;
    numberkbd->body_desc.pixmap = NULL;
    numberkbd->margin = 5;
    numberkbd->text_color = SGL_THEME_TEXT_COLOR;

    numberkbd->btn_desc.alpha = SGL_THEME_ALPHA;
    numberkbd->btn_desc.color = SGL_THEME_COLOR;
    numberkbd->btn_desc.radius = SGL_THEME_RADIUS;
    numberkbd->btn_desc.border = 0;
    numberkbd->btn_desc.border_color = SGL_THEME_BORDER_COLOR;
    numberkbd->btn_desc.pixmap = NULL;

    numberkbd->opcode = 0;

    return obj;
}
