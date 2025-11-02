/* source/include/sgl_style.h
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

#ifndef __SGL_STYLE_H__
#define __SGL_STYLE_H__


typedef enum sgl_style_type {
    SGL_STYLE_POS_X = 0,
    SGL_STYLE_POS_Y,
    SGL_STYLE_SIZE_W,
    SGL_STYLE_SIZE_H,
    SGL_STYLE_COLOR,
    SGL_STYLE_ALPHA,
    SGL_STYLE_RADIUS,
    SGL_STYLE_BG_COLOR,
    SGL_STYLE_ALIGN,
    SGL_STYLE_TEXT,
    SGL_STYLE_TEXT_ALPHA,
    SGL_STYLE_TEXT_COLOR,
    SGL_STYLE_TEXT_MARGIN,
    SGL_STYLE_TEXT_X_OFFSET,
    SGL_STYLE_TEXT_Y_OFFSET,
    SGL_STYLE_PIXMAP,
    SGL_STYLE_ICON,
    SGL_STYLE_FONT,
    SGL_STYLE_BORDER_WIDTH,
    SGL_STYLE_BORDER_COLOR,
    SGL_STYLE_KNOB_COLOR,
    SGL_STYLE_DIRECTION,
    SGL_STYLE_STATUS,
    SGL_STYLE_VALUE,
    /* circle or ring style */
    SGL_STYLE_CENTER_X_OFFSET,
    SGL_STYLE_CENTER_Y_OFFSET,
    /* label style */
    SGL_STYLE_BG_TRANSPARENT,
    /* line style */
    SGL_STYLE_LINE_WIDTH,
    SGL_STYLE_LINE_SPACE,
    /* ring style */
    SGL_STYLE_RING_WIDTH,
    /* arc style */
    SGL_STYLE_ARC_WIDTH,
    SGL_STYLE_ARC_MODE,
    SGL_STYLE_ARC_START_ANGLE,
    SGL_STYLE_ARC_END_ANGLE,
    /* message box style */
    SGL_STYLE_MSGBOX_TEXT,
    SGL_STYLE_MSGBOX_TEXT_COLOR,
    SGL_STYLE_MSGBOX_TITLE,
    SGL_STYLE_MSGBOX_TITLE_COLOR,
    SGL_STYLE_MSGBOX_TITLE_ICON,
    SGL_STYLE_MSGBOX_CLOSE_TEXT,
    SGL_STYLE_MSGBOX_CLOSE_TEXT_COLOR,
    SGL_STYLE_MSGBOX_CLOSE_ICON,
    SGL_STYLE_MSGBOX_APPLY_TEXT,
    SGL_STYLE_MSGBOX_APPLY_TEXT_COLOR,
    SGL_STYLE_MSGBOX_APPLY_ICON,
    SGL_STYLE_MSGBOX_BUTTON_COLOR,
    SGL_STYLE_MSGBOX_ANSWER,
    /* listview style */
    SGL_STYLE_LISTVIEW_ITEM_COLOR,
    SGL_STYLE_LISTVIEW_ITEM_RADIUS,
    SGL_STYLE_LISTVIEW_ITEM_TEXT_COLOR,

    /* checkbox style */
    SGL_STYLE_CHECKBOX_STATUS,

    /* number keyboard style */
    SGL_STYLE_NUMBERKBD_TEXT_COLOR,
    SGL_STYLE_NUMBERKBD_BG_COLOR,
    SGL_STYLE_NUMBERKBD_BTN_MARGIN,
    SGL_STYLE_NUMBERKBD_BTN_COLOR,
    SGL_STYLE_NUMBERKBD_BTN_RADIUS,
    SGL_STYLE_NUMBERKBD_BTN_ALPHA,
    SGL_STYLE_NUMBERKBD_BTN_PIXMAP,
    SGL_STYLE_NUMBERKBD_BTN_BORDER_WIDTH,
    SGL_STYLE_NUMBERKBD_BTN_BORDER_COLOR,
    SGL_STYLE_NUMBERKBD_OPCODE,

    /* textbox style */
    SGL_STYLE_TEXTBOX_SCROLL_BG_COLOR,
    SGL_STYLE_TEXTBOX_SCROLL_FG_COLOR,

    /* keyboard style */
    SGL_STYLE_KEYBOARD_OPCODE,
    SGL_STYLE_KEYBOARD_BTN_COLOR,
    SGL_STYLE_KEYBOARD_BTN_RADIUS,
    SGL_STYLE_KEYBOARD_BTN_ALPHA,
    SGL_STYLE_KEYBOARD_BTN_PIXMAP,
    SGL_STYLE_KEYBOARD_BTN_BORDER_WIDTH,
    SGL_STYLE_KEYBOARD_BTN_BORDER_COLOR,

    /* window style */
    SGL_STYLE_WINDOW_TITLE_TEXT,
    SGL_STYLE_WINDOW_TITLE_TEXT_COLOR,

    SGL_STYLE_MAX
} sgl_style_type_t;


#endif // !__SGL_STYLE_H__
