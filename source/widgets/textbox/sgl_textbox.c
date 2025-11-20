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
#include <sgl_theme.h>
#include "sgl_textbox.h"


#define  SGL_TEXTBOX_SCROLL_WIDTH                  (4)


static int16_t textbox_scroll_get_pos(sgl_obj_t* obj, int16_t scroll_h)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    int16_t height = obj->coords.y2 - obj->coords.y1 + 1;

    return (-textbox->y_offset) * (height - scroll_h) / (textbox->text_height - height);
}


static void sgl_textbox_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_textbox_t *textbox = (sgl_textbox_t*)obj;
    int16_t height = obj->coords.y2 - obj->coords.y1 + 1;
    int16_t scroll_height = height / 8;

    sgl_rect_t scroll_coords = {
        .x1 = obj->coords.x2 - SGL_TEXTBOX_SCROLL_WIDTH,
        .y1 = obj->coords.y1,
        .x2 = obj->coords.x2,
        .y2 = obj->coords.y2
    };

    SGL_ASSERT(textbox->font != NULL);

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &obj->coords, &textbox->bg);
        sgl_draw_string_mult_line(surf, &obj->area, obj->coords.x1, obj->coords.y1 + textbox->y_offset, textbox->text, textbox->text_color, textbox->bg.alpha, textbox->font, textbox->line_margin, textbox->edge_margin);

        if(textbox->scroll_enable) {
            sgl_draw_rect(surf, &obj->area, &scroll_coords, &textbox->scroll_bg);

            scroll_coords.y1 = textbox_scroll_get_pos(obj, scroll_height) + obj->coords.y1;
            scroll_coords.y2 = scroll_coords.y1 + scroll_height;

            sgl_draw_rect(surf, &obj->area, &scroll_coords, &textbox->scroll_fg);
        }
    }
    else if(evt->type == SGL_EVENT_MOVE_UP) {
        textbox->text_height = sgl_font_get_string_height(&obj->coords, textbox->text, textbox->font, textbox->line_margin, textbox->edge_margin);
        textbox->scroll_enable = 1;
        if((textbox->text_height + textbox->y_offset) > height ) {
           textbox->y_offset -= evt->distance;
        }
    }
    else if(evt->type == SGL_EVENT_MOVE_DOWN) {
        textbox->text_height = sgl_font_get_string_height(&obj->coords, textbox->text, textbox->font, textbox->line_margin, textbox->edge_margin);
        textbox->scroll_enable = 1;
        if(textbox->y_offset < 0) {
            textbox->y_offset += evt->distance;
        }
    }
    else if (evt->type == SGL_EVENT_PRESSED) {
        textbox->scroll_enable = 1;
    }
    else if (evt->type == SGL_EVENT_RELEASED) {
        textbox->scroll_enable = 0;
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

    sgl_obj_set_clickable(obj);
    sgl_obj_set_movable(obj);

    textbox->bg.alpha = SGL_THEME_ALPHA;
    textbox->bg.color = SGL_THEME_COLOR;
    textbox->bg.radius = SGL_THEME_RADIUS;
    textbox->bg.border = SGL_THEME_BORDER_WIDTH;
    textbox->bg.border_color = SGL_THEME_BORDER_COLOR;

    textbox->scroll_bg.alpha = SGL_THEME_ALPHA;
    textbox->scroll_bg.color = SGL_THEME_SCROLL_BG_COLOR;

    textbox->scroll_fg.alpha = SGL_THEME_ALPHA;
    textbox->scroll_fg.color = SGL_THEME_SCROLL_FG_COLOR;

    textbox->text_color = SGL_THEME_TEXT_COLOR;
    textbox->line_margin = 1;
    textbox->text = "textbox";
    textbox->edge_margin = 3;

    textbox->y_offset = 0;
    textbox->scroll_enable = 0;

    return obj;
}
