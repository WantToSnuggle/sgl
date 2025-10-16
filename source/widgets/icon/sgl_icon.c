/* source/widgets/sgl_icon.c
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
#include "sgl_icon.h"


/**
 * @brief set icon style
 * @param obj pointer to object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_icon_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_icon_t *icon = (sgl_icon_t*)obj;

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
        icon->desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_ALPHA:
        icon->desc.alpha = value;
        break;
    
    case SGL_STYLE_ICON:
        icon->desc.icon = (sgl_icon_pixmap_t*)value;
        break;

    default:
        SGL_LOG_WARN("sgl_icon_set_style: unsupported style type %d", type);
        break;
    }

    /* set style */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get icon style
 * @param obj pointer to object
 * @param type style type
 * @return style value
 */
size_t sgl_icon_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_icon_t *icon = (sgl_icon_t*)obj;

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
        return sgl_color2int(icon->desc.color);

    case SGL_STYLE_ALPHA:
        return icon->desc.alpha;

    case SGL_STYLE_ICON:
        return (size_t)icon->desc.icon;

    default:
        SGL_LOG_WARN("sgl_icon_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief icon construct callback
 * @param surf pointer to surface
 * @param obj pointer to object
 * @param evt pointer to event
 * @return none
 */
static void sgl_icon_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_icon_t *icon = (sgl_icon_t*)obj;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_icon(surf, &obj->area, &obj->coords, &icon->desc);
    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_RELEASED) {
        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
}


/**
 * @brief create icon object
 * @param parent pointer to parent object
 * @return pointer to icon object
 */
sgl_obj_t* sgl_icon_create(sgl_obj_t* parent)
{
    sgl_icon_t *icon = sgl_malloc(sizeof(sgl_icon_t));
    if(icon == NULL) {
        SGL_LOG_ERROR("sgl_icon_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(icon, 0, sizeof(sgl_icon_t));

    sgl_obj_t *obj = &icon->obj;
    sgl_obj_init(&icon->obj, parent);
    obj->construct_fn = sgl_icon_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_icon_set_style;
    obj->get_style = sgl_icon_get_style;
#endif

    icon->desc.alpha = SGL_THEME_ALPHA;
    icon->desc.icon = NULL;
    icon->desc.color = SGL_THEME_TEXT_COLOR;

    return obj;
}
