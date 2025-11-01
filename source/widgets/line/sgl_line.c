/* source/widgets/sgl_line.c
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
#include "sgl_line.h"


/**
 * @brief set line object style
 * @param obj line object
 * @param type style type
 * @param value style value
 */
void sgl_line_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_line_t *line = (sgl_line_t*)obj;

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
        line->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_ALPHA:
        line->desc.alpha = value;
        break;

    case SGL_STYLE_LINE_WIDTH:
        line->desc.width = value;
        break;

    default:
        SGL_LOG_WARN("sgl_line_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get line object style
 * @param obj line object
 * @param type style type
 * @return style value
 */
size_t sgl_line_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_line_t *line = (sgl_line_t*)obj;

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
        return sgl_color2int(line->desc.color);
    
    case SGL_STYLE_ALPHA:
        return line->desc.alpha;

    case SGL_STYLE_LINE_WIDTH:
        return line->desc.width;

    default:
        SGL_LOG_WARN("sgl_line_set_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


static void sgl_line_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_line_t *line = (sgl_line_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_line(surf, &obj->area, &line->desc);
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        obj->coords.x1 = line->desc.start.x;
        obj->coords.y1 = line->desc.start.y;
        obj->coords.x2 = line->desc.end.x;
        obj->coords.y2 = line->desc.end.y;
    }
}


/**
 * @brief create a line object
 * @param parent parent of the line
 * @return line object
 */
sgl_obj_t* sgl_line_create(sgl_obj_t* parent)
{
    sgl_line_t *line = sgl_malloc(sizeof(sgl_line_t));
    if(line == NULL) {
        SGL_LOG_ERROR("sgl_line_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(line, 0, sizeof(sgl_line_t));

    sgl_obj_t *obj = &line->obj;
    sgl_obj_init(&line->obj, parent);
    obj->construct_fn = sgl_line_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_line_set_style;
    obj->get_style = sgl_line_get_style;
#endif
    obj->needinit = 1;

    line->desc.color = SGL_THEME_BG_COLOR;
    line->desc.alpha = SGL_ALPHA_MAX;
    line->desc.width = 1;

    return obj;
}


void sgl_line_set_start_point(sgl_obj_t *obj, int16_t x, int16_t y)
{
    sgl_line_t *line = (sgl_line_t*)obj;
    line->desc.start.x = x;
    line->desc.start.y = y;
}

void sgl_line_set_end_point(sgl_obj_t *obj, int16_t x, int16_t y)
{
    sgl_line_t *line = (sgl_line_t*)obj;
    line->desc.end.x = x;
    line->desc.end.y = y;
}


void sgl_line_set_width(sgl_obj_t *obj, uint16_t width)
{
    sgl_line_t *line = (sgl_line_t*)obj;
    line->desc.width = width;
}
