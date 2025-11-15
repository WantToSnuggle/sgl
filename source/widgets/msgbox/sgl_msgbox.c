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


static void msgbox_draw_text(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *coords, const sgl_icon_pixmap_t *icon, const char *text, 
                             const sgl_font_t *font, sgl_color_t color, uint8_t alpha, uint8_t y_offset)
{
    sgl_pos_t align_pos;
    int32_t text_x = 0, icon_y = 0;

    if (icon) {
        text_x = icon->width + 2;
    }

    align_pos = sgl_get_text_pos(coords, font, text, text_x, SGL_ALIGN_CENTER);

    if (icon) {
        icon_y = ((coords->y2 - coords->y1) - (icon->height)) / 2 + 1;
        sgl_draw_icon(surf, area, align_pos.x, coords->y1 + icon_y + y_offset, color, alpha, icon);
    }
    sgl_draw_string(surf, area, align_pos.x + text_x, align_pos.y + y_offset, text, color, alpha, font);
}


static void sgl_msgbox_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_msgbox_t *msgbox = (sgl_msgbox_t *)obj;
    const sgl_font_t *font = msgbox->font;
    int32_t font_height = sgl_font_get_height(font) + 8;
    int16_t border = msgbox->body_desc.border;

    sgl_color_t tmp_color;
    sgl_area_t  button_coords = {
        .x1 = obj->coords.x1,
        .x2 = obj->coords.x2,
        .y1 = obj->coords.y2 - font_height,
        .y2 = obj->coords.y2
    };

    sgl_area_t apply_coords = {
        .x1 =  obj->coords.x1 + border,
        .x2 = (obj->coords.x1 + obj->coords.x2) / 2 - border / 2,
        .y1 = obj->coords.y2 - 2 * font_height,
        .y2 = obj->coords.y2 - border,
    };

    sgl_area_t close_coords = {
        .x1 = (obj->coords.x1 + obj->coords.x2) / 2 + border / 2,
        .x2 = obj->coords.x2 - border,
        .y1 = obj->coords.y2 - 2 * font_height,
        .y2 = obj->coords.y2 - border,
    };

    sgl_area_t title_coords = {
        .x1 = obj->coords.x1 + border + 2,
        .x2 = obj->coords.x2 - border + 2,
        .y1 = obj->coords.y1 + 1,
        .y2 = obj->coords.y1 + font_height + border,
    };

    sgl_area_t text_coords = {
        .x1 = obj->coords.x1 + border,
        .x2 = obj->coords.x2 - border,
        .y1 = obj->coords.y1 + font_height + border,
        .y2 = obj->coords.y2 - (font_height + border),
    };

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        if(msgbox->status & SGL_MSGBOX_STATUS_EXIT) {
            sgl_obj_set_destroyed(&msgbox->obj);
        }

        sgl_draw_rect(surf, &obj->area, &obj->coords, &msgbox->body_desc);
        
        msgbox_draw_text(surf, &obj->area, &title_coords, msgbox->title_icon, msgbox->title_text, font, msgbox->title_color, msgbox->body_desc.alpha, 0);

        sgl_draw_fill_hline(surf, &obj->area,
                                       obj->coords.y1 + font_height + 4,
                                       obj->coords.x1 + msgbox->body_desc.border,
                                       obj->coords.x2 - msgbox->body_desc.border,
                                       msgbox->body_desc.border,
                                       msgbox->body_desc.border_color,
                                       msgbox->body_desc.alpha
                                      );

        sgl_draw_string_mult_line(surf, &text_coords, text_coords.x1, text_coords.y1, msgbox->msg_text, msgbox->msg_color, msgbox->body_desc.alpha, font, 2, msgbox->msg_line_margin);

        if(msgbox->status & SGL_MSGBOX_STATUS_APPLY) {
            tmp_color = msgbox->apply_color;
            msgbox->apply_color = sgl_color_mixer(msgbox->btn_text_color, msgbox->body_desc.color, 128);
        }
        else if(msgbox->status & SGL_MSGBOX_STATUS_CLOSE) {
            tmp_color = msgbox->close_color;
            msgbox->close_color = sgl_color_mixer(msgbox->btn_text_color, msgbox->body_desc.color, 128);
        }

        sgl_draw_fill_round_rect(surf, &button_coords, &apply_coords, obj->radius, msgbox->apply_color, msgbox->body_desc.alpha);
        sgl_draw_fill_round_rect(surf, &button_coords, &close_coords, obj->radius, msgbox->close_color, msgbox->body_desc.alpha);
        msgbox_draw_text(surf, &obj->area, &apply_coords, msgbox->apply_icon, msgbox->apply_text, font, msgbox->btn_text_color, msgbox->body_desc.alpha, font_height / 2);
        msgbox_draw_text(surf, &obj->area, &close_coords, msgbox->close_icon, msgbox->close_text, font, msgbox->btn_text_color, msgbox->body_desc.alpha, font_height / 2);
    
        if(msgbox->status & SGL_MSGBOX_STATUS_APPLY) {
            msgbox->apply_color = tmp_color;
        }
        else if(msgbox->status & SGL_MSGBOX_STATUS_CLOSE) {
            msgbox->close_color = tmp_color;
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
    obj->construct_fn = sgl_msgbox_construct_cb;

    msgbox->body_desc.alpha = SGL_THEME_ALPHA;
    msgbox->body_desc.color = SGL_THEME_COLOR;
    msgbox->body_desc.radius = SGL_THEME_RADIUS;
    msgbox->body_desc.border = SGL_THEME_BORDER_WIDTH;
    msgbox->body_desc.border_color = SGL_THEME_BORDER_COLOR;
    msgbox->body_desc.pixmap = NULL;

    msgbox->font = NULL;
    msgbox->msg_color = SGL_THEME_TEXT_COLOR;
    msgbox->msg_line_margin = 1;
    msgbox->msg_text = "Message Box";

    msgbox->apply_color = sgl_color_mixer(SGL_THEME_COLOR, SGL_THEME_TEXT_COLOR, 200);
    msgbox->close_color = sgl_color_mixer(SGL_THEME_COLOR, SGL_THEME_TEXT_COLOR, 200);
    msgbox->apply_text = "OK";
    msgbox->close_text = "NO";
    msgbox->btn_text_color = SGL_THEME_TEXT_COLOR;

    msgbox->status = SGL_MSGBOX_STATUS_NORMAL;

    sgl_obj_set_clickable(obj);

    return obj;
}
