/* source/widgets/sgl_keyboard.c
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
#include "sgl_keyboard.h"


#define  KEYBOARD_ICON_ENTER              (0)
#define  KEYBOARD_ICON_NEWLINE            (1)
#define  KEYBOARD_ICON_BACKSPACE          (2)
#define  KEYBOARD_ICON_KEYBD              (3)
#define  KEYBOARD_ICON_LEFT               (4)
#define  KEYBOARD_ICON_RIGHT              (5)
#define  KEYBOARD_ICON_INVALID            (-1)


static const uint8_t btn_ok_bitmap[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x63,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5e,0xfe,0x30,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0xef,0xff,0xe5,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5e,0xff,0xff,0xfc,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0xef,0xff,0xff,0xe5,0x00,
0x00,0x0a,0xca,0x00,0x00,0x00,0x5e,0xff,0xff,0xfe,0x50,0x00,
0x00,0xae,0xff,0xa0,0x00,0x05,0xef,0xff,0xff,0xe5,0x00,0x00,
0x08,0xef,0xff,0xfa,0x00,0x5e,0xff,0xff,0xfe,0x50,0x00,0x00,
0x0c,0xff,0xff,0xff,0xa5,0xef,0xff,0xff,0xe5,0x00,0x00,0x00,
0x00,0xef,0xff,0xff,0xff,0xff,0xff,0xfe,0x50,0x00,0x00,0x00,
0x00,0x0e,0xff,0xff,0xff,0xff,0xff,0xe5,0x00,0x00,0x00,0x00,
0x00,0x00,0xef,0xff,0xff,0xff,0xfe,0x50,0x00,0x00,0x00,0x00,
0x00,0x00,0x0e,0xff,0xff,0xff,0xe5,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xef,0xff,0xfe,0x50,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0e,0xff,0xe5,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xcc,0x50,0x00,0x00,0x00,0x00,0x00,0x00,
};


static const uint8_t btn_newline_bitmap[] = {
0xcc,0xcc,0xcc,0xcc,0x00,
0x8e,0xff,0xff,0xfe,0x00,
0x0a,0xcc,0xcf,0xfe,0x00,
0x00,0x00,0x3e,0xfe,0x00,
0x00,0x00,0x3e,0xfe,0x00,
0x00,0x00,0x3e,0xfe,0x00,
0x00,0x00,0x3e,0xfe,0x00,
0x00,0x00,0x3e,0xfe,0x00,
0x00,0x00,0x3e,0xfe,0x00,
0x00,0xac,0xcf,0xff,0xcc,
0x00,0x5e,0xff,0xff,0xfe,
0x00,0x08,0xef,0xff,0xe5,
0x00,0x00,0xaf,0xfe,0x80,
0x00,0x00,0x0c,0xfa,0x00,
};


static const uint8_t btn_backspace_bitmap[] = {
0x00,0x08,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x8e,0xe8,0x00,0x00,0x00,0x00,0x00,0x00,
0x0a,0xef,0xfe,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x8e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,
0x0c,0xff,0xfe,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x00,0xcf,0xe8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0a,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,
};


static const uint8_t btn_keybd_bitmap[] = {
0x05,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x65,0x00,
0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,
0xee,0xca,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xce,0xe0,
0xee,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5e,0xe0,
0xee,0x53,0xaa,0x05,0xa8,0x08,0xa8,0x08,0xa5,0x0a,0xa5,0x0a,0xa3,0x5e,0xe0,
0xee,0x58,0xee,0x3a,0xfe,0x0c,0xfc,0x0c,0xfa,0x0e,0xe8,0x3e,0xe5,0x5e,0xe0,
0xee,0x53,0x65,0x03,0x65,0x05,0x65,0x05,0x63,0x05,0x63,0x3e,0xe5,0x5e,0xe0,
0xee,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0xe5,0x5e,0xe0,
0xee,0x55,0xaa,0xaa,0x08,0xa8,0x08,0xa8,0x0a,0xa8,0x3a,0xae,0xe5,0x5e,0xe0,
0xee,0x58,0xef,0xfe,0x0a,0xfc,0x0c,0xfc,0x0e,0xfa,0x3e,0xff,0xe5,0x5e,0xe0,
0xee,0x50,0x33,0x33,0x00,0x33,0x03,0x33,0x03,0x30,0x03,0x33,0x30,0x5e,0xe0,
0xee,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5e,0xe0,
0xee,0x58,0xcc,0x38,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xc8,0x3c,0xc5,0x5e,0xe0,
0xee,0x58,0xee,0x3a,0xff,0xff,0xff,0xff,0xff,0xff,0xe8,0x3e,0xe5,0x5e,0xe0,
0xee,0x50,0x33,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x30,0x03,0x30,0x5e,0xe0,
0xee,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5e,0xe0,
0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0,
0xae,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xa0,
};


static const uint8_t btn_left_bitmap[] = {
0x00,0x00,0x00,0x00,0xae,0xc0,0x00,
0x00,0x00,0x00,0x0a,0xef,0xfc,0x00,
0x00,0x00,0x00,0xae,0xff,0xfe,0x30,
0x00,0x00,0x0a,0xef,0xff,0xe3,0x00,
0x00,0x00,0xae,0xff,0xfe,0x30,0x00,
0x00,0x0a,0xef,0xff,0xe3,0x00,0x00,
0x00,0xae,0xff,0xfe,0x30,0x00,0x00,
0x0a,0xef,0xff,0xe3,0x00,0x00,0x00,
0x08,0xef,0xff,0xe5,0x00,0x00,0x00,
0x00,0x8e,0xff,0xfe,0x50,0x00,0x00,
0x00,0x08,0xef,0xff,0xe5,0x00,0x00,
0x00,0x00,0x8e,0xff,0xfe,0x50,0x00,
0x00,0x00,0x08,0xef,0xff,0xe5,0x00,
0x00,0x00,0x00,0x8e,0xff,0xfe,0x50,
0x00,0x00,0x00,0x08,0xef,0xfc,0x00,
0x00,0x00,0x00,0x00,0x8e,0xc0,0x00,
};


static const uint8_t btn_right_bitmap[] = {
0x03,0xee,0x80,0x00,0x00,0x00,0x00,
0x3e,0xff,0xe8,0x00,0x00,0x00,0x00,
0x5e,0xff,0xfe,0x80,0x00,0x00,0x00,
0x05,0xef,0xff,0xe8,0x00,0x00,0x00,
0x00,0x5e,0xff,0xfe,0x80,0x00,0x00,
0x00,0x05,0xef,0xff,0xe8,0x00,0x00,
0x00,0x00,0x5e,0xff,0xfe,0x80,0x00,
0x00,0x00,0x05,0xef,0xff,0xe8,0x00,
0x00,0x00,0x08,0xef,0xff,0xe5,0x00,
0x00,0x00,0x8e,0xff,0xfe,0x50,0x00,
0x00,0x08,0xef,0xff,0xe5,0x00,0x00,
0x00,0x8e,0xff,0xfe,0x50,0x00,0x00,
0x08,0xef,0xff,0xe5,0x00,0x00,0x00,
0x5e,0xff,0xfe,0x50,0x00,0x00,0x00,
0x0c,0xff,0xe5,0x00,0x00,0x00,0x00,
0x00,0xce,0x50,0x00,0x00,0x00,0x00,
};


static const sgl_icon_pixmap_t keyboard_icon[] = { 
    {
        .bitmap = btn_ok_bitmap,
        .bpp = 4,
        .height = 16,
        .width = 24,
    },
    {
        .bitmap = btn_newline_bitmap,
        .bpp = 4,
        .height = 14,
        .width = 10,
    },
    {
        .bitmap = btn_backspace_bitmap,
        .bpp = 4,
        .height = 8,
        .width = 18,
    },
    {
        .bitmap = btn_keybd_bitmap,
        .bpp = 4,
        .height = 18,
        .width = 30,
    },
    {
        .bitmap = btn_left_bitmap,
        .bpp = 4,
        .height = 16,
        .width = 14,
    },
    {
        .bitmap = btn_right_bitmap,
        .bpp = 4,
        .height = 16,
        .width = 14,
    }
};


#define  KEYBOARD_KEYMODE_UPPER                   (0)
#define  KEYBOARD_KEYMODE_LOWER                   (1)
#define  KEYBOARD_KEYMODE_SPEC                    (2)
#define  KEYBOARD_KEYMODE_MAX                     (3)

#define  KEYBOARD_BTN_LINES                       (4)
#define  KEYBOARD_BTN_COLUMNS                     (12)
#define  KEYBOARD_BTN_NUM                         (40)


#define  KEYBOARD_KEY_INVALID                     (-1)
#define  KEYBOARD_KEY_TO_UPPER                    (255)
#define  KEYBOARD_KEY_TO_LOWER                    (254)
#define  KEYBOARD_KEY_TO_SPEC                     (253)
#define  KEYBOARD_KEY_TO_CLOSE                    (252)

#define  KEYBOARD_KEY_MODE(x)                     ((x) >> 1)

#define  KEYBOARD_KEY_BACKSPACE                   (11)
#define  KEYBOARD_KEY_NUMBER_BACKSPACE            (10)
#define  KEYBOARD_KEY_SPACE                       (37)
#define  KEYBOARD_KEY_NEWLINE                     (22)
#define  KEYBOARD_KEY_ENTER                       (39)
#define  KEYBOARD_KEY_CLOSE                       (35)
#define  KEYBOARD_KEY_LEFT                        (36)
#define  KEYBOARD_KEY_RIGHT                       (38)



static const char keyboard_key_ascii[KEYBOARD_KEYMODE_MAX][KEYBOARD_BTN_NUM] = {
    {
        KEYBOARD_KEY_TO_SPEC, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '\b',
        KEYBOARD_KEY_TO_LOWER, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '\n',
        '_', '-', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '.', ',', ':',
        KEYBOARD_KEY_TO_CLOSE, '<', ' ', '>', '\r'
    },
    {
        KEYBOARD_KEY_TO_SPEC, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '\b',
        KEYBOARD_KEY_TO_UPPER, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\n',
        '_', '-', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.', ',', ':',
        KEYBOARD_KEY_TO_CLOSE, '<', ' ', '>', '\r'
    },
    {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\b',
        KEYBOARD_KEY_TO_LOWER, '+', '-', '/', '*', '=', '%', '!', '?', '#', '<', '>',
        '\\', '@', '$', '(', ')', '{', '}', '[', ']', ';', '\"', '\'',
        KEYBOARD_KEY_TO_CLOSE, '<', ' ', '>', '\r'
    },
};


static const char keybd_btn_map[KEYBOARD_KEYMODE_MAX][KEYBOARD_BTN_NUM][4] = {
    { 
        "1#", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "<<",
        "abc", "A", "S", "D", "F", "G", "H", "J", "K", "L", "nl",
        "_", "-", "Z", "X", "C", "V", "B", "N", "M", ".", ",", ":",
        "kbd", "<", " ", ">", "OK"
    },
    { 
        "1#", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "<<",
        "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", "nl",
        "_", "-", "z", "x", "c", "v", "b", "n", "m", ".", ",", ":",
        "kbd", "<", " ", ">", "OK"
    },
    {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<<",
        "abc", "+", "-", "/", "*", "=", "%", "!", "?", "#", "<", ">",
        "\\", "@", "$", "(", ")", "{", "}", "[", "]", ";", "\"", "'",
        "kbd", "<", " ", ">", "OK"
    },
};


static const uint8_t keybd_btn_width[2][KEYBOARD_BTN_LINES][KEYBOARD_BTN_COLUMNS] = {
    /* upper and lower mode */
    {
        {5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7},
        {6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {2, 2, 6, 2, 2},
    },
    /* number mode and spec mode */
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {2, 2, 6, 2, 2},
    },
};


static const uint8_t keybd_btn_height[KEYBOARD_BTN_LINES] = {
    1, 1, 1, 1
};


static const uint8_t keyboard_btn_count[2][KEYBOARD_BTN_LINES] = {
    /* upper and lower mode */
    {
        12, 11, 12, 5
    },
    /* number mode and spec mode */
    {
        11, 12, 12, 5
    }
};


static void keyboard_btn_handler(sgl_keyboard_t *keyboard)
{
    char *edit = keyboard->edit;
    int index = keyboard->edit_max_len;
    while (*edit && index) {
        edit ++;
        index --;
    }

    if (index == 0) {
        SGL_LOG_ERROR("keyboard edit buffer is full\n");
        return;
    }

    switch (keyboard->opcode) {
    case '\b': 
        *(--edit) = '\0';
        break;
    case '\n':
    case '\r':
        *(edit++) = '\n';
        *(edit++) = '\0';
        break;
    default :
        *(edit++) = keyboard->opcode;
        *(edit++) = '\0';
        break;
    }
}


static int8_t keyindex_is_icon(uint8_t mode, uint8_t index)
{
    if(index == KEYBOARD_KEY_NUMBER_BACKSPACE && (mode == KEYBOARD_KEYMODE_SPEC)) {
        return KEYBOARD_ICON_BACKSPACE;
    }
    else if(index == KEYBOARD_KEY_BACKSPACE && (mode < KEYBOARD_KEYMODE_SPEC)) {
        return KEYBOARD_ICON_BACKSPACE;
    }
    else if(index == KEYBOARD_KEY_ENTER) {
        return KEYBOARD_ICON_ENTER;
    }
    else if(index == KEYBOARD_KEY_NEWLINE && (mode != KEYBOARD_KEYMODE_SPEC)) {
        return KEYBOARD_ICON_NEWLINE;
    }
    else if(index == KEYBOARD_KEY_CLOSE) {
        return KEYBOARD_ICON_KEYBD;
    }
    else if(index == KEYBOARD_KEY_LEFT) {
        return KEYBOARD_ICON_LEFT;
    }
    else if(index == KEYBOARD_KEY_RIGHT) {
        return KEYBOARD_ICON_RIGHT;
    }

    return KEYBOARD_ICON_INVALID;
}


static inline uint8_t keyboard_index_to_ascii(uint8_t keymode, uint8_t index)
{
    return keyboard_key_ascii[keymode][index];
}


static int8_t keyboard_pos_to_index(int16_t x, int16_t y, sgl_keyboard_t *keyboard, int16_t width, int16_t height)
{
    sgl_obj_t *obj = &keyboard->obj;
    int16_t index = 0, x_index = -1, y_index = -1, key_mode = KEYBOARD_KEY_MODE(keyboard->key_mode);
    int16_t btn_width[KEYBOARD_BTN_COLUMNS] = {0};
    int16_t btn_height[KEYBOARD_BTN_LINES] = {0};
    sgl_rect_t btn = {0};

    /* calculate button height */
    sgl_split_len(keybd_btn_height, KEYBOARD_BTN_LINES, height, keyboard->key_margin, btn_height);
    btn.y1 = obj->coords.y1;
    for(int i = 0; i < KEYBOARD_BTN_LINES; i++) {
        btn.y1 += keyboard->key_margin;
        btn.y2 = btn.y1 + btn_height[i] - 1;

        if(y >= btn.y1 && y <= btn.y2) {
            y_index = i;
            break;
        }
        btn.y1 += btn_height[i];
    }

    if(y_index < 0 || y_index >= KEYBOARD_BTN_LINES) {
        return -1;
    }

    sgl_split_len(keybd_btn_width[key_mode][y_index], keyboard_btn_count[key_mode][y_index], width, keyboard->key_margin, btn_width);

    btn.x1 = obj->coords.x1;
    for(int i = 0; i < keyboard_btn_count[key_mode][y_index]; i++) {
        btn.x1 += keyboard->key_margin;
        btn.x2 = btn.x1 + btn_width[i] - 1;
        if(x >= btn.x1 && x <= btn.x2) {
            x_index = i;
            break;
        }
        btn.x1 += btn_width[i];
    }

    if(x_index < 0 || x_index >= keyboard_btn_count[key_mode][y_index]) {
        return -1;
    }

    for(int i = 0; i < y_index; i ++) {
        index += keyboard_btn_count[key_mode][i];
    }

    return (index + x_index);
}


/**
 * @brief Set style of keyboard
 * @param obj keyboard object
 * @param type style type
 * @param value style value
 * @return none
 */
void sgl_keyboard_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;

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
        keyboard->body_desc.color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_ALPHA:
        keyboard->body_desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_RADIUS:
        keyboard->body_desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_PIXMAP:
        keyboard->body_desc.pixmap = (sgl_pixmap_t*)value;
        break;

    case SGL_STYLE_FONT:
        keyboard->font = (sgl_font_t*)value;
        break;

    case SGL_STYLE_TEXT_COLOR:
        keyboard->text_color = sgl_int2color(value);
        break;

    case SGL_STYLE_BORDER_WIDTH:
        keyboard->body_desc.border = (uint8_t)value;
        break;

    case SGL_STYLE_BORDER_COLOR:
        keyboard->body_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_KEYBOARD_BTN_COLOR:
        keyboard->btn_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_KEYBOARD_BTN_RADIUS:
        keyboard->btn_desc.radius = sgl_obj_fix_radius(obj, value);
        break;

    case SGL_STYLE_KEYBOARD_BTN_ALPHA:
        keyboard->btn_desc.alpha = (uint8_t)value;
        break;
    
    case SGL_STYLE_KEYBOARD_BTN_BORDER_WIDTH:
        keyboard->btn_desc.border = (uint8_t)value;
        break;

    case SGL_STYLE_KEYBOARD_BTN_BORDER_COLOR:
        keyboard->btn_desc.color = sgl_int2color(value);
        break;

    case SGL_STYLE_KEYBOARD_BTN_PIXMAP:
        keyboard->btn_desc.pixmap = (sgl_pixmap_t*)value;
        break;

    default:
        SGL_LOG_WARN("sgl_keyboard_set_style: unsupported style type %d", type);
    }

    /* set dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief Get style of keyboard
 * @param obj keyboard object
 * @param type style type
 * @return style value
 */
size_t sgl_keyboard_get_style(sgl_obj_t *obj, sgl_style_type_t type)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
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
        return sgl_color2int(keyboard->body_desc.color);

    case SGL_STYLE_ALPHA:
        return keyboard->body_desc.alpha;
    
    case SGL_STYLE_RADIUS:
        return obj->radius;

    case SGL_STYLE_TEXT_COLOR:
        return sgl_color2int(keyboard->text_color);

    case SGL_STYLE_FONT:
        return (size_t)keyboard->font;

    case SGL_STYLE_KEYBOARD_OPCODE:
        return keyboard->opcode;

    default:
        SGL_LOG_WARN("sgl_keyboard_set_style: unsupported style type %d", type);
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief keyboard constructor function
 * @param surf: pointer to surface
 * @param obj: pointer to keyboard object
 * @param evt: pointer to event 
 * @return none
 */
static void sgl_keyboard_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    int16_t body_w = obj->coords.x2 - obj->coords.x1 + 1;
    int16_t body_h = obj->coords.y2 - obj->coords.y1 + 1;

    int8_t index = 0, key_mode = KEYBOARD_KEY_MODE(keyboard->key_mode), icon_index = 0;
    sgl_color_t btn_color = keyboard->btn_desc.color;
    int16_t text_x = 0, text_y = 0;
    int16_t btn_width[KEYBOARD_BTN_COLUMNS] = {0};
    int16_t btn_height[KEYBOARD_BTN_LINES] = {0};
    sgl_rect_t btn = {0};

    /* calculate button height */
    sgl_split_len(keybd_btn_height, KEYBOARD_BTN_LINES, body_h, keyboard->key_margin, btn_height);

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        sgl_draw_rect(surf, &obj->area, &obj->coords, &keyboard->body_desc);

        btn.y1 = obj->coords.y1;

        for(int i = 0; i < KEYBOARD_BTN_LINES; i++) {
            sgl_split_len(keybd_btn_width[key_mode][i], keyboard_btn_count[key_mode][i], body_w, keyboard->key_margin, btn_width);

            btn.x1 = obj->coords.x1;
            btn.y1 += keyboard->key_margin;
            btn.y2 = btn.y1 + btn_height[i] - 1;

            for(int j = 0; j < keyboard_btn_count[key_mode][i]; j++) {
                btn.x1 += keyboard->key_margin;
                btn.x2 = btn.x1 + btn_width[j] - 1;

                if(index == keyboard->key_index) {
                    keyboard->btn_desc.color = sgl_color_mixer(btn_color, keyboard->text_color, 128);
                }
                else {
                    keyboard->btn_desc.color = btn_color;
                }
                sgl_draw_rect(surf, &btn, &btn, &keyboard->btn_desc);

                icon_index = keyindex_is_icon(keyboard->key_mode, index);
                if (icon_index != KEYBOARD_ICON_INVALID) {
                    text_x = btn.x1 + (btn_width[j] - keyboard_icon[icon_index].width) / 2;
                    text_y = btn.y1 + (btn_height[i] - keyboard_icon[icon_index].height) / 2;
                    sgl_draw_icon_on_bg(surf, &btn, text_x, text_y, keyboard->text_color, (sgl_icon_pixmap_t*)&keyboard_icon[icon_index]);
                }
                else {
                    text_x = btn.x1 + (btn_width[j] - sgl_font_get_string_width(keybd_btn_map[keyboard->key_mode][index], keyboard->font)) / 2;
                    text_y = btn.y1 + (btn_height[i] - keyboard->font->font_height) / 2;
                    sgl_draw_string_on_bg(surf, &btn, text_x, text_y, keybd_btn_map[keyboard->key_mode][index], keyboard->text_color, keyboard->font, 0);
                }

                btn.x1 += btn_width[j];
                index ++;
            }

            btn.y1 += btn_height[i];
        }

        keyboard->btn_desc.color = btn_color;
    }
    else if(evt->type == SGL_EVENT_PRESSED || evt->type == SGL_EVENT_OPTION_TAP) {
        if  (evt->type == SGL_EVENT_PRESSED) {
            index = keyboard_pos_to_index(evt->pos.x, evt->pos.y, keyboard, body_w, body_h);
            if(index < 0) {
                return;
            }
            keyboard->key_index = index;
        }

        uint8_t key_ascii = keyboard_index_to_ascii(keyboard->key_mode, index);
        if(key_ascii == KEYBOARD_KEY_TO_UPPER) {
            keyboard->key_mode = KEYBOARD_KEYMODE_UPPER;
            keyboard->key_index = KEYBOARD_KEY_INVALID;
        }
        else if(key_ascii == KEYBOARD_KEY_TO_LOWER) {
            keyboard->key_mode = KEYBOARD_KEYMODE_LOWER;
            keyboard->key_index = KEYBOARD_KEY_INVALID;
        }
        else if(key_ascii == KEYBOARD_KEY_TO_SPEC) {
            keyboard->key_mode = KEYBOARD_KEYMODE_SPEC;
            keyboard->key_index = KEYBOARD_KEY_INVALID;
        }
        else if(key_ascii == KEYBOARD_KEY_TO_CLOSE) {
            sgl_obj_set_destroyed(obj);
            return;
        }
        else {
            keyboard->opcode = key_ascii;
            if (keyboard->edit) {
                keyboard_btn_handler(keyboard);
            }
        }

        if(obj->event_fn) {
            obj->event_fn(evt);
        }
    }
    else if(evt->type == SGL_EVENT_RELEASED) {
        keyboard->key_index = KEYBOARD_KEY_INVALID;
    }
    else if(evt->type == SGL_EVENT_DRAW_INIT) {
        keyboard->opcode = 0;
        keyboard->key_index = KEYBOARD_KEY_INVALID;

        if (keyboard->key_margin == 0) {
            keyboard->key_margin = sgl_max(body_w / 128, 1);
        }

        if (keyboard->btn_desc.radius == 0) {
            keyboard->btn_desc.radius = sgl_max(keyboard->key_margin, 2);
        }

        SGL_ASSERT(keyboard->font != NULL);
    }
    else if (evt->type == SGL_EVENT_OPTION_WALK) {
        keyboard->key_index ++;
        
        if (keyboard->key_index >= KEYBOARD_BTN_NUM) {
            keyboard->key_index = 0;
        }
    }
}


/**
 * @brief create keyboard object
 * @param parent parent of object
 * @return keyboard object
 */
sgl_obj_t* sgl_keyboard_create(sgl_obj_t* parent)
{
    sgl_keyboard_t *keyboard = sgl_malloc(sizeof(sgl_keyboard_t));
    if(keyboard == NULL) {
        SGL_LOG_ERROR("sgl_keyboard_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(keyboard, 0, sizeof(sgl_keyboard_t));

    sgl_obj_t *obj = &keyboard->obj;
    sgl_obj_init(&keyboard->obj, parent);
    obj->construct_fn = sgl_keyboard_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_keyboard_set_style;
    obj->get_style = sgl_keyboard_get_style;
#endif
    obj->clickable = 1;
    obj->needinit  = 1;

    keyboard->body_desc.alpha = SGL_THEME_ALPHA;
    keyboard->body_desc.color = SGL_THEME_COLOR;
    keyboard->body_desc.radius = SGL_THEME_RADIUS;
    keyboard->body_desc.border = 1;
    keyboard->body_desc.border_color = SGL_THEME_BORDER_COLOR;
    keyboard->body_desc.pixmap = NULL;
    keyboard->text_color = SGL_THEME_TEXT_COLOR;

    keyboard->btn_desc.alpha = SGL_THEME_ALPHA;
    keyboard->btn_desc.color = SGL_THEME_COLOR;
    keyboard->btn_desc.radius = 0;
    keyboard->btn_desc.border = 1;
    keyboard->btn_desc.border_color = SGL_THEME_BORDER_COLOR;
    keyboard->btn_desc.pixmap = NULL;

    keyboard->key_mode = KEYBOARD_KEYMODE_LOWER;

    keyboard->key_margin = 0;

    return obj;
}


/**
 * @brief get keyboard opcode
 * @param obj keyboard object
 * @return opcode [0 ~ 255]
 */
uint8_t sgl_keyboard_get_opcode(sgl_obj_t *obj)
{
    return ((sgl_keyboard_t*)obj)->opcode;
}


/**
 * @brief set keyboard text buffer
 * @param obj keyboard object
 * @param buffer edit buffer
 * @param buf_max_len edit buffer max length
 */
void sgl_keyboard_set_textarea(sgl_obj_t *obj, char *buffer, int buf_max_len)
{
    sgl_keyboard_t *keyboard = (sgl_keyboard_t*)obj;
    keyboard->edit = buffer;
    keyboard->edit_max_len = buf_max_len;
}
