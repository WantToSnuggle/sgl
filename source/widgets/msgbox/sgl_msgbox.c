/* source/widgets/sgl_msgbox.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Document reference: docs directory
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

#include <sgl.h>
#include <sgl_theme.h>


#define  SGL_MSGBOX_STATUS_NORMAL               (1 << 7)
#define  SGL_MSGBOX_STATUS_APPLY                (1 << 0)
#define  SGL_MSGBOX_STATUS_CLOSE                (1 << 1)
#define  SGL_MSGBOX_STATUS_EXIT                 (1 << 2)


/**
 * @brief Set style of message box
 * @param obj pointer to message box object
 * @param type style type
 * @param value value of style
 * @return none
 */
void sgl_msgbox_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;

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

    case SGL_STYLE_BG_COLOR:
        msgbox->body_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_RADIUS:
        msgbox->body_desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_PIXMAP:
        msgbox->body_desc.pixmap = (sgl_pixmap_t *)value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        msgbox->body_desc.border_color = sgl_int2color(value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        msgbox->body_desc.border = value;
        break;
    
    case SGL_STYLE_ALPHA:
        msgbox->body_desc.alpha = value;
        msgbox->text_desc.alpha = value;
        msgbox->title_desc.alpha = value;
        msgbox->title_line_desc.alpha = value;
        msgbox->apply_text.alpha = value;
        msgbox->close_text.alpha = value;
        break;;
    
    case SGL_STYLE_TEXT:
        msgbox->text_desc.text = (char *)value;
        break;
    
    case SGL_STYLE_MSGBOX_TEXT:
        msgbox->text_desc.text = (char *)value;
        break;
    
    case SGL_STYLE_TEXT_COLOR:
        msgbox->text_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_MSGBOX_TEXT_COLOR:
        msgbox->text_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_MSGBOX_TITLE:
        msgbox->title_desc.text = (char *)value;
        break;

    case SGL_STYLE_MSGBOX_TITLE_COLOR:
        msgbox->title_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_MSGBOX_TITLE_ICON:
        msgbox->title_desc.icon = (sgl_icon_pixmap_t*)value;
        break;

    case SGL_STYLE_MSGBOX_BUTTON_COLOR:
        msgbox->apply_text.bg_color = sgl_int2color(value);
        msgbox->close_text.bg_color = sgl_int2color(value);
        msgbox->apply_text.bg_flag = 1;
        msgbox->close_text.bg_flag = 1;
        break;

    case SGL_STYLE_MSGBOX_CLOSE_TEXT:
        msgbox->close_text.text = (char *)value;
        break;
    
    case SGL_STYLE_MSGBOX_CLOSE_TEXT_COLOR:
        msgbox->close_text.color = sgl_int2color(value);
        break;

    case SGL_STYLE_MSGBOX_CLOSE_ICON:
        msgbox->close_text.icon = (sgl_icon_pixmap_t*)value;
        break;
    
    case SGL_STYLE_MSGBOX_APPLY_TEXT:
        msgbox->apply_text.text = (char *)value;
        break;
    
    case SGL_STYLE_MSGBOX_APPLY_TEXT_COLOR:
        msgbox->apply_text.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_MSGBOX_APPLY_ICON:
        msgbox->apply_text.icon = (sgl_icon_pixmap_t*)value;
        break;

    case SGL_STYLE_LINE_SPACE:
        msgbox->text_desc.line_space = value;
        break;
    
    case SGL_STYLE_FONT:
        msgbox->title_desc.font = (sgl_font_t *)value;
        msgbox->text_desc.font = (sgl_font_t *)value;
        msgbox->close_text.font = (sgl_font_t *)value;
        msgbox->apply_text.font = (sgl_font_t *)value;
        break;

    default:
        SGL_LOG_WARN("sgl_msgbox_set_style: unsupported style type %d", type);
        break;
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief Get style of message box
 * @param obj pointer to message box object
 * @param type style type
 * @return value of style
 */
size_t sgl_msgbox_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;

    switch ((int)type) {
    case SGL_STYLE_POS_X:
        return sgl_obj_get_pos_x(obj);

    case SGL_STYLE_POS_Y:
        return sgl_obj_get_pos_y(obj);
    
    case SGL_STYLE_SIZE_W:
        return sgl_obj_get_width(obj);
    
    case SGL_STYLE_SIZE_H:
        return sgl_obj_get_height(obj);

    case SGL_STYLE_FONT:
        return (size_t)msgbox->title_desc.font;

    case SGL_STYLE_STATUS:
        return (msgbox->status & SGL_MSGBOX_STATUS_APPLY);

    case SGL_STYLE_MSGBOX_ANSWER:
        return (msgbox->status & SGL_MSGBOX_STATUS_APPLY);

    default:
        SGL_LOG_WARN("sgl_msgbox_get_style: unsupported style type %d", type);
        break;
    }

    return SGL_STYLE_FAILED;
}


static void sgl_msgbox_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    const sgl_font_t *font = msgbox->title_desc.font;
    int32_t font_height = sgl_font_get_height(font) + 8;
    sgl_color_t tmp_color;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        if(msgbox->status & SGL_MSGBOX_STATUS_EXIT) {
            sgl_obj_set_destroyed(&msgbox->obj);
        }

        sgl_draw_rect(surf, &obj->area, &obj->coords, &msgbox->body_desc);
        sgl_draw_text(surf, &obj->area, &msgbox->title_coords, &msgbox->title_desc);

        sgl_draw_fill_hline_with_alpha(surf, &obj->area,
                                       obj->coords.y1 + font_height + 4,
                                       obj->coords.x1 + msgbox->body_desc.border,
                                       obj->coords.x2 - msgbox->body_desc.border,
                                       msgbox->body_desc.border,
                                       msgbox->body_desc.border_color,
                                       msgbox->body_desc.alpha
                                      );

        sgl_draw_text(surf, &obj->area, &msgbox->text_coords, &msgbox->text_desc);

        if(msgbox->status & SGL_MSGBOX_STATUS_APPLY) {
            tmp_color = msgbox->apply_text.color;
            msgbox->apply_text.bg_color = sgl_color_mixer(msgbox->apply_text.color, msgbox->body_desc.color, 128);
        }
        else if(msgbox->status & SGL_MSGBOX_STATUS_CLOSE) {
            tmp_color = msgbox->close_text.color;
            msgbox->close_text.bg_color = sgl_color_mixer(msgbox->close_text.color, msgbox->body_desc.color, 128);
        }

        sgl_draw_text(surf, &msgbox->button_coords, &msgbox->apply_coords, &msgbox->apply_text);
        sgl_draw_text(surf, &msgbox->button_coords, &msgbox->close_coords, &msgbox->close_text);

        if(msgbox->status & SGL_MSGBOX_STATUS_APPLY) {
            msgbox->apply_text.color = tmp_color;
        }
        else if(msgbox->status & SGL_MSGBOX_STATUS_CLOSE) {
            msgbox->close_text.color = tmp_color;
        }
    }
    else if(evt->type == SGL_EVENT_PRESSED) {
        if(evt->pos.y > (obj->coords.y2 - font_height - 2) && evt->pos.x < ((obj->coords.x1 + obj->coords.x2) / 2)) {
            msgbox->status |= SGL_MSGBOX_STATUS_APPLY;
        }
        else if(evt->pos.y > (obj->coords.y2 - font_height - 2) && evt->pos.x > ((obj->coords.x1 + obj->coords.x2) / 2)) {
            msgbox->status |= SGL_MSGBOX_STATUS_CLOSE;
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
        if(evt->pos.y > (obj->coords.y2 - font_height - 2) && evt->pos.x < ((obj->coords.x1 + obj->coords.x2) / 2)) {
            msgbox->status |= SGL_MSGBOX_STATUS_EXIT;
        }
        else if(evt->pos.y > (obj->coords.y2 - font_height - 2) && evt->pos.x > ((obj->coords.x1 + obj->coords.x2) / 2)) {
            msgbox->status |= SGL_MSGBOX_STATUS_EXIT;
        }
        else {
            sgl_obj_clear_dirty(obj);
            return;
        }
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        msgbox->title_coords.x1 = obj->coords.x1 + 4;
        msgbox->title_coords.x2 = obj->coords.x2 - 4;
        msgbox->title_coords.y1 = obj->coords.y1 + 1;
        msgbox->title_coords.y2 = obj->coords.y1 + font_height + 2;

        msgbox->text_coords.x1 = obj->coords.x1 + 2;
        msgbox->text_coords.x2 = obj->coords.x2 - 2;
        msgbox->text_coords.y1 = obj->coords.y1 + font_height + 6;
        msgbox->text_coords.y2 = obj->coords.y2 - (font_height + 6);

        msgbox->apply_coords.x1 = obj->coords.x1 + 3;
        msgbox->apply_coords.x2 = (obj->coords.x1 + obj->coords.x2) / 2 - 2;
        msgbox->apply_coords.y1 = obj->coords.y2 - 2 * font_height;
        msgbox->apply_coords.y2 = obj->coords.y2 - 2;
        msgbox->apply_text.font = font;
        msgbox->apply_text.radius = msgbox->body_desc.radius;
        msgbox->apply_text.bg_flag = 1;
        msgbox->apply_text.y_offset = font_height / 2;
        msgbox->apply_text.align = SGL_ALIGN_CENTER;

        msgbox->close_coords.x1 = (obj->coords.x1 + obj->coords.x2) / 2 + 2;
        msgbox->close_coords.x2 = obj->coords.x2 - 3;
        msgbox->close_coords.y1 = obj->coords.y2 - 2 * font_height;
        msgbox->close_coords.y2 = obj->coords.y2 - 2;
        msgbox->close_text.font = font;
        msgbox->close_text.radius = msgbox->body_desc.radius;
        msgbox->close_text.bg_flag = 1;
        msgbox->close_text.y_offset = font_height / 2;
        msgbox->close_text.align = SGL_ALIGN_CENTER;

        msgbox->button_coords.x1 = obj->coords.x1;
        msgbox->button_coords.x2 = obj->coords.x2;
        msgbox->button_coords.y1 = obj->coords.y2 - font_height;
        msgbox->button_coords.y2 = obj->coords.y2;
    }
}


/**
 * @brief create a message box object
 * @param parent parent of the message box
 * @return message box object
 */
sgl_obj_t* sgl_msgbox_create(sgl_obj_t* parent)
{
    sgl_msgbox_t *msgbox = sgl_malloc(sizeof(sgl_msgbox_t));
    if(msgbox == NULL) {
        SGL_LOG_ERROR("sgl_msgbox_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(msgbox, 0, sizeof(sgl_msgbox_t));

    sgl_obj_t *obj = &msgbox->obj;
    sgl_obj_init(&msgbox->obj, parent);
    obj->needinit = 1;
    obj->construct_fn = sgl_msgbox_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_msgbox_set_style;
    obj->get_style = sgl_msgbox_get_style;
#endif

    msgbox->body_desc.alpha = SGL_THEME_ALPHA;
    msgbox->body_desc.color = SGL_THEME_COLOR;
    msgbox->body_desc.radius = SGL_THEME_RADIUS;
    msgbox->body_desc.border = SGL_THEME_BORDER_WIDTH;
    msgbox->body_desc.border_color = SGL_THEME_BORDER_COLOR;
    msgbox->body_desc.pixmap = NULL;

    msgbox->title_desc.text = "Message Box";
    msgbox->title_desc.alpha = SGL_THEME_ALPHA;
    msgbox->title_desc.color = SGL_THEME_TEXT_COLOR;
    msgbox->title_desc.bg_flag = 0;

    msgbox->text_desc.text = "You have not set the text yet.";
    msgbox->text_desc.alpha = SGL_THEME_ALPHA;
    msgbox->text_desc.color = SGL_THEME_TEXT_COLOR;
    msgbox->text_desc.bg_flag = 0;
    msgbox->text_desc.mode = SGL_DRAW_TEXT_LINES;
    msgbox->text_desc.line_space = 2;
    msgbox->text_desc.margin = 3;
    msgbox->text_desc.radius = SGL_THEME_RADIUS;

    msgbox->apply_text.alpha = SGL_THEME_ALPHA;
    msgbox->apply_text.bg_flag = 1;
    msgbox->apply_text.text = "Apply";
    msgbox->apply_text.color = SGL_THEME_TEXT_COLOR;
    msgbox->close_text.alpha = SGL_THEME_ALPHA;
    msgbox->close_text.bg_flag = 1;
    msgbox->close_text.text = "Close";
    msgbox->close_text.color = SGL_THEME_TEXT_COLOR;

    msgbox->apply_text.bg_color = sgl_color_mixer(SGL_THEME_COLOR, SGL_THEME_TEXT_COLOR, 200);
    msgbox->close_text.bg_color = sgl_color_mixer(SGL_THEME_COLOR, SGL_THEME_TEXT_COLOR, 200);

    msgbox->status = SGL_MSGBOX_STATUS_NORMAL;

    sgl_obj_set_clickable(obj);

    return obj;
}
