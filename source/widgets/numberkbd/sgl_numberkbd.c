/* source/widgets/sgl_numberkbd.c
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
#include <sgl_draw.h>
#include <string.h>
#include "sgl_numberkbd.h"


#define NUMBERKBD_BTN_ROW        5
#define NUMBERKBD_BTN_COL        4

#define NUMBERKBD_BTN_OK_ASCII   13

static const char kbd_digits[NUMBERKBD_BTN_ROW][NUMBERKBD_BTN_COL] = {
    {'+', '-', '*', '/'                    }, 
    {'7', '8', '9', '='                    },
    {'4', '5', '6', '\b'                   },
    {'1', '2', '3', NUMBERKBD_BTN_OK_ASCII },
    {'.', '0', '%', NUMBERKBD_BTN_OK_ASCII }
};


static const uint8_t btn_enter_bitmap[] = {
    /*  */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8e,0xfa,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xef,0xff,0xa0,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8e,0xff,0xff,0xfa,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xef,0xff,0xff,0xfe,0x50,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8e,0xff,0xff,0xff,0xfe,0x30,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xef,0xff,0xff,0xff,0xe3,0x00,
    0x00,0x00,0xae,0xe8,0x00,0x00,0x00,0x00,0x8e,0xff,0xff,0xff,0xfe,0x30,0x00,
    0x00,0x0a,0xff,0xfe,0x80,0x00,0x00,0x08,0xef,0xff,0xff,0xff,0xe3,0x00,0x00,
    0x00,0xae,0xff,0xff,0xe8,0x00,0x00,0x8e,0xff,0xff,0xff,0xfe,0x30,0x00,0x00,
    0x05,0xef,0xff,0xff,0xfe,0x80,0x08,0xef,0xff,0xff,0xff,0xe3,0x00,0x00,0x00,
    0x03,0xef,0xff,0xff,0xff,0xe8,0x8e,0xff,0xff,0xff,0xfe,0x30,0x00,0x00,0x00,
    0x00,0x5e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0x00,0x00,0x00,0x00,
    0x00,0x05,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x5e,0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x05,0xef,0xff,0xff,0xff,0xff,0xfe,0x30,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x5e,0xff,0xff,0xff,0xff,0xe3,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x05,0xef,0xff,0xff,0xfe,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x5e,0xff,0xff,0xe3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x05,0xef,0xfe,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x5c,0xc3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


static sgl_icon_pixmap_t enter_icon = {
    .bitmap = btn_enter_bitmap,
    .bpp = 4,
    .height = 20,
    .width = 30,
};


static const uint8_t btn_backspace_bitmap[] = {
    /*  */
    0x00,0x00,0x00,0x3e,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x05,0xef,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x5e,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x08,0xef,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x8e,0xff,0xff,0xec,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
    0x0a,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,
    0x3e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,
    0x03,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,
    0x00,0x3e,0xff,0xff,0xc6,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x65,
    0x00,0x03,0xef,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x0c,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0xcf,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x0a,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


static sgl_icon_pixmap_t backspace_icon = {
    .bitmap = btn_backspace_bitmap,
    .bpp = 4,
    .height = 13,
    .width = 30,
};


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

    case SGL_STYLE_COLOR:
        numberkbd->body_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_ALPHA:
        numberkbd->body_desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_RADIUS:
        numberkbd->body_desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_PIXMAP:
        numberkbd->body_desc.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_FONT:
        numberkbd->font = (sgl_font_t*)value;
        break;

    case SGL_STYLE_TEXT_COLOR:
        numberkbd->text_color = sgl_int2color(value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        numberkbd->body_desc.border = (uint8_t)value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        numberkbd->body_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_NUMBERKBD_TEXT_COLOR:
        numberkbd->text_color = sgl_int2color(value);
        break;

    case SGL_STYLE_NUMBERKBD_BG_COLOR:
        numberkbd->body_desc.color = sgl_int2color(value);
        break;    

    case SGL_STYLE_NUMBERKBD_BTN_MARGIN:
        numberkbd->margin = (uint8_t)value;
        break;

    case SGL_STYLE_NUMBERKBD_BTN_COLOR:
        numberkbd->btn_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_RADIUS:
        numberkbd->btn_desc.radius = sgl_min((int16_t)value, r_min);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_ALPHA:
        numberkbd->btn_desc.alpha = (uint8_t)value;
        break;

    case SGL_STYLE_NUMBERKBD_BTN_PIXMAP:
        numberkbd->btn_desc.pixmap = (sgl_pixmap_t*)(value);
        break;

    case SGL_STYLE_NUMBERKBD_BTN_BORDER_WIDTH:
        numberkbd->btn_desc.border = (uint8_t)value;
        break;
    
    case SGL_STYLE_NUMBERKBD_BTN_BORDER_COLOR:
        numberkbd->btn_desc.border_color = sgl_int2color(value);
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

    case SGL_STYLE_NUMBERKBD_OPCODE:
        return numberkbd->opcode;

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
    sgl_numberkbd_t *numberkbd = (sgl_numberkbd_t*)obj;
    int16_t body_w = obj->coords.x2 - obj->coords.x1 + 1;
    int16_t body_h = obj->coords.y2 - obj->coords.y1 + 1;
    sgl_color_t btn_color = numberkbd->btn_desc.color;

    int16_t box_w = (body_w - (NUMBERKBD_BTN_COL + 1) * numberkbd->margin) / NUMBERKBD_BTN_COL;
    int16_t box_h = (body_h - (NUMBERKBD_BTN_ROW + 1) * numberkbd->margin) / NUMBERKBD_BTN_ROW;
    int16_t text_x = 0, text_y = 0, btn_row = 0, btn_col = 0;

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
            btn_col = 0;

            for(btn.x2 = (btn.x1 + box_w); btn.x1 < (obj->coords.x2 - numberkbd->margin) ; btn.x1 += (box_w + numberkbd->margin), btn.x2 = (btn.x1 + box_w)) {
                if(numberkbd->opcode != kbd_digits[btn_row][btn_col]) {
                    numberkbd->btn_desc.color = btn_color;
                }
                else {
                    numberkbd->btn_desc.color = sgl_color_mixer(btn_color, numberkbd->text_color, 128);
                }

                if(btn_col == 3 && btn_row > 1) {
                    if(btn_row == 2) {
                        sgl_draw_rect(surf, &btn, &btn, &numberkbd->btn_desc);
                        text_x = btn.x1 + ((box_w -  backspace_icon.width) / 2);
                        text_y = btn.y1 + ((box_h - backspace_icon.height + 1) / 2);
                        sgl_draw_icon_on_bg(surf, &btn, text_x, text_y, numberkbd->text_color, &backspace_icon);
                    }
                    else if (btn_row == 3) {
                        btn.y2 += (numberkbd->margin + box_h);
                        sgl_draw_rect(surf, &btn, &btn, &numberkbd->btn_desc);
                        text_x = btn.x1 + ((box_w -  enter_icon.width) / 2);
                        text_y = btn.y1 + ((2 * box_h - enter_icon.height) / 2);
                        sgl_draw_icon_on_bg(surf, &btn, text_x, text_y, numberkbd->text_color, &enter_icon);
                    }
                }
                else {
                    sgl_draw_rect(surf, &btn, &btn, &numberkbd->btn_desc);
                    text_x = btn.x1 + ((box_w -  sgl_font_get_string_width("0", numberkbd->font)) / 2);
                    sgl_draw_character_on_bg(surf, &obj->area, text_x, text_y, kbd_digits[btn_row][btn_col] - 32, numberkbd->text_color, numberkbd->font);
                }
                btn_col ++;
            }

            numberkbd->btn_desc.color = btn_color;
            btn_row ++;
        }
    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        int16_t x_rel = evt->pos.x - obj->coords.x1;
        int16_t y_rel = evt->pos.y - obj->coords.y1;

        btn_col = -1;
        int16_t cur_x = numberkbd->margin;
        for (int c = 0; c < NUMBERKBD_BTN_COL; c++) {
            if (x_rel >= cur_x && x_rel < cur_x + box_w) {
                btn_col = c;
                break;
            }
            cur_x += box_w + numberkbd->margin;
        }

        btn_row = -1;
        int16_t cur_y = numberkbd->margin;

        for (int r = 0; r < NUMBERKBD_BTN_ROW; r++) {
            if (y_rel >= cur_y && y_rel < cur_y + box_h) {
                btn_row = r;
                break;
            }
            cur_y += box_h + numberkbd->margin;
        }

        if(btn_col == 3 && btn_row >= 3) {
            numberkbd->opcode = NUMBERKBD_BTN_OK_ASCII;
        }
        else if (btn_col >= 0 && btn_row >= 0) {
            numberkbd->opcode = (uint8_t)kbd_digits[btn_row][btn_col];
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
        if(numberkbd->opcode == 0) {
            sgl_obj_clear_dirty(obj);
            return;
        }
        numberkbd->opcode = 0;
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        int16_t new_width = box_w * NUMBERKBD_BTN_COL + (NUMBERKBD_BTN_COL + 1) * numberkbd->margin;
        int16_t new_height = box_h * NUMBERKBD_BTN_ROW + (NUMBERKBD_BTN_ROW + 1) * numberkbd->margin;

        obj->coords.x2 = obj->coords.x1 + new_width;
        obj->coords.y2 = obj->coords.y1 + new_height;

        SGL_ASSERT(numberkbd->font != NULL);
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
    obj->needinit  = 1;

    numberkbd->body_desc.alpha = SGL_THEME_ALPHA;
    numberkbd->body_desc.color = SGL_THEME_COLOR;
    numberkbd->body_desc.radius = SGL_THEME_RADIUS;
    numberkbd->body_desc.border = SGL_THEME_BORDER_WIDTH;
    numberkbd->body_desc.border_color = SGL_THEME_BORDER_COLOR;
    numberkbd->body_desc.pixmap = NULL;
    numberkbd->margin = 5;
    numberkbd->text_color = SGL_THEME_TEXT_COLOR;

    numberkbd->btn_desc.alpha = SGL_THEME_ALPHA;
    numberkbd->btn_desc.color = SGL_THEME_COLOR;
    numberkbd->btn_desc.radius = SGL_THEME_RADIUS;
    numberkbd->btn_desc.border = 1;
    numberkbd->btn_desc.border_color = SGL_THEME_BORDER_COLOR;
    numberkbd->btn_desc.pixmap = NULL;

    return obj;
}


/**
 * @brief get numberkbd opcode
 * @param obj numberkbd object
 * @return opcode [0 ~ 255]
 */
uint8_t sgl_numberkbd_get_opcode(sgl_obj_t *obj)
{
    return ((sgl_numberkbd_t*)obj)->opcode;
}
