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


static void sgl_textline_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_textline_t *textline = (sgl_textline_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_obj_set_height(obj, sgl_font_get_string_height(&obj->coords, textline->text, textline->font, textline->line_margin, textline->edge_margin));
        obj->area = obj->coords;

        if (textline->bg_flag) {
            sgl_draw_fill_round_rect(surf, &obj->area, &obj->coords, obj->radius, textline->bg_color, textline->alpha);
        }

        sgl_draw_string_mult_line(surf, &obj->area, obj->coords.x1, obj->coords.y1, textline->text, textline->color, textline->alpha, textline->font, textline->edge_margin, textline->line_margin);
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

    textline->alpha = SGL_THEME_ALPHA;
    textline->bg_flag = true;
    textline->bg_color = SGL_THEME_COLOR;
    textline->color = SGL_THEME_TEXT_COLOR;
    textline->line_margin = 1;
    textline->edge_margin = 2;
    textline->text = "textline";

    return obj;
}
