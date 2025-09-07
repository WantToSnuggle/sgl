#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <sgl.h>
#include <sgl_font.h>


typedef struct sgl_port_sdl2 sgl_port_sdl2_t;


sgl_port_sdl2_t* sgl_port_sdl2_init(void);
size_t sgl_port_sdl2_get_frame_count(sgl_port_sdl2_t* sdl2_dev);
void sgl_port_sdl2_increase_frame_count(sgl_port_sdl2_t* sdl2_dev);
void sgl_port_sdl2_deinit(sgl_port_sdl2_t* sdl2_dev);
void flush_window_callback(void *param);


extern const unsigned char gImage_bg[272640];
sgl_pixmap_t bg_pixmap = {
    .width = 213,
    .height = 320,
    .bitmap = gImage_bg,
};

extern const unsigned char gImage_test[1440000];
sgl_pixmap_t test_pixmap = {
    .width = 800,
    .height = 480,
    .bitmap = gImage_test,
};

sgl_obj_t *rect43 = NULL;

char buf[16] = {0};

void slider_callback(sgl_event_t *event)
{
    sgl_obj_t *label = (sgl_obj_t *)event->param;
    sprintf(buf, "H:%lld%%", sgl_obj_get_style(event->obj, SGL_STYLE_VALUE));
    sgl_obj_set_style(label, SGL_STYLE_TEXT, SGL_TEXT(buf));
}

char arcbuf[10];

int main(int argc, char *argv[])
{
    SGL_UNUSED(argc);
    SGL_UNUSED(argv);

    int quit = 0;
    SDL_Event MouseEvent;
    sgl_port_sdl2_t* sdl2_dev = NULL;

    sdl2_dev = sgl_port_sdl2_init();
    if(sdl2_dev == NULL) {
        return -1;
    }

    sgl_obj_set_pixmap(sgl_screen_act(), &test_pixmap);

    // sgl_obj_t *msgbox = sgl_msgbox_create(NULL);
    // sgl_obj_set_pos(msgbox, 20, 20);
    // sgl_obj_set_size(msgbox, 300, 200);
    // sgl_obj_set_style(msgbox, SGL_STYLE_FONT, SGL_FONT(consolas23));
    // sgl_obj_set_style(msgbox, SGL_STYLE_MSGBOX_TITLE, SGL_TEXT("Message Box"));
    // sgl_obj_set_style(msgbox, SGL_STYLE_MSGBOX_TEXT, SGL_TEXT("SGL (Small Graphics Library) is a lightweight and fast graphics library"));
    // sgl_obj_set_style(msgbox, SGL_STYLE_RADIUS, 10);
    // sgl_obj_set_style(msgbox, SGL_STYLE_MSGBOX_APPLY_TEXT, SGL_TEXT("OK"));
    // sgl_obj_set_style(msgbox, SGL_STYLE_MSGBOX_CLOSE_TEXT, SGL_TEXT("NO"));
    // sgl_obj_set_style(msgbox, SGL_STYLE_BORDER_WIDTH, 2);
    // sgl_obj_set_style(msgbox, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_LIGHT_GRAY));

    sgl_obj_t *button = sgl_button_create(NULL);
    sgl_obj_set_pos(button, 20, 20);
    sgl_obj_set_size(button, 200, 100);
    sgl_obj_set_style(button, SGL_STYLE_RADIUS, 50);
    sgl_obj_set_style(button, SGL_STYLE_BORDER_WIDTH, 2);
    sgl_obj_set_style(button, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_BLACK));
    sgl_obj_set_font(button, &consolas23);
    sgl_obj_set_style(button, SGL_STYLE_TEXT ,SGL_TEXT("Button: 1"));
    sgl_obj_set_style(button, SGL_STYLE_PIXMAP, SGL_PIXMAP(bg_pixmap));

    // sgl_obj_t *circle = sgl_circle_create(NULL);
    // sgl_obj_set_pos(circle, 20, 150);
    // sgl_obj_set_size(circle, 100, 200);
    // sgl_obj_set_style(circle, SGL_STYLE_RADIUS, 100);
    // sgl_obj_set_style(circle, SGL_STYLE_CENTER_X_OFFSET, 50);
    // sgl_obj_set_style(circle, SGL_STYLE_CENTER_Y_OFFSET, 50);
    // sgl_obj_set_style(circle, SGL_STYLE_PIXMAP, SGL_PIXMAP(bg_pixmap));
    // sgl_obj_set_clickable(circle);

    // sgl_obj_t *slider = sgl_slider_create(NULL);
    // sgl_obj_set_pos(slider, 20, 20);
    // sgl_obj_set_size(slider, 200, 50);
    // sgl_obj_set_style(slider, SGL_STYLE_BG_COLOR, SGL_COLOR(SGL_GREEN));
    // sgl_obj_set_style(slider, SGL_STYLE_COLOR, SGL_COLOR(SGL_GRAY));

    // sgl_obj_t *label = sgl_label_create(slider);
    // sgl_obj_set_style(label, SGL_STYLE_FONT, SGL_FONT(song23));
    // sgl_obj_set_style(label, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_RED));
    // sgl_obj_set_style(label, SGL_STYLE_TEXT, SGL_TEXT("H:0%"));

    // sgl_obj_set_event_cb(slider, slider_callback, (size_t)label);

    sgl_obj_t *arc = sgl_arc_create(NULL);
    sgl_obj_set_pos(arc, 100, 100);
    sgl_obj_set_size(arc, 200, 200);
    sgl_obj_set_style(arc, SGL_STYLE_RADIUS, 100);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_START_ANGLE, 30);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_END_ANGLE, 330);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_WIDTH, 40);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_MODE, SGL_DRAW_ARC_RING_SMOOTH);
    sgl_obj_set_style(arc, SGL_STYLE_BG_COLOR, SGL_COLOR(SGL_BLUE));

    sgl_obj_set_alpha(arc, 128);
    sgl_obj_set_style(arc, SGL_STYLE_COLOR, SGL_COLOR(SGL_RED));
    sgl_obj_set_style(arc, SGL_STYLE_BG_COLOR, SGL_COLOR(SGL_GREEN));
    sgl_obj_set_style(arc, SGL_STYLE_RADIUS, 100);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_WIDTH, 40);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_START_ANGLE, 0);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_END_ANGLE, 0);
    sgl_obj_set_style(arc, SGL_STYLE_ARC_MODE, SGL_DRAW_ARC_RING_SMOOTH);

    sgl_obj_t *label2 = sgl_label_create(arc);
    // sgl_obj_set_pos(label2, 300, 200);
    // sgl_obj_set_size(label2, 200, 200);
    sgl_obj_set_style(label2, SGL_STYLE_FONT, SGL_FONT(kai33));
    sgl_obj_set_style(label2, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_BLUE));
    sgl_obj_set_style(label2, SGL_STYLE_TEXT, SGL_TEXT(arcbuf));

    // sgl_obj_t *textbox = sgl_textline_create(NULL);
    // sgl_obj_set_pos(textbox, 20, 100);
    // sgl_obj_set_size(textbox, 200, 300);
    // sgl_obj_set_style(textbox, SGL_STYLE_BG_COLOR, SGL_COLOR(SGL_GRAY));
    // sgl_obj_set_style(textbox, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_BLUE));
    // sgl_obj_set_style(textbox, SGL_STYLE_FONT, SGL_FONT(consolas23));
    // sgl_obj_set_style(textbox, SGL_STYLE_TEXT_MARGIN, 10);
    // sgl_obj_set_style(textbox, SGL_STYLE_RADIUS, 10);
    // sgl_obj_set_style(textbox, SGL_STYLE_TEXT, SGL_TEXT(""));

    // sgl_obj_t *textbox = sgl_textbox_create(NULL);
    // sgl_obj_set_pos(textbox, 20, 120);
    // sgl_obj_set_size(textbox, 200, 300);

    // sgl_obj_set_style(textbox, SGL_STYLE_FONT, SGL_FONT(song23));
    // sgl_obj_set_style(textbox, SGL_STYLE_TEXT, SGL_TEXT("SGL (Small Graphics Library) is a lightweight and a beautiful and lightweight GUI (Graphics User Interface) for MCU-level processors.SGL (Small Graphics Library) is a lightweight and a beautiful and lightweight GUI (Graphics User Interface) for MCU-level processors."));
    // sgl_obj_set_style(textbox, SGL_STYLE_BG_COLOR, SGL_COLOR(SGL_GREEN));
    // sgl_obj_set_style(textbox, SGL_STYLE_TEXT_COLOR, SGL_COLOR(SGL_RED));
    // sgl_obj_set_style(textbox, SGL_STYLE_RADIUS, 10);
    // sgl_obj_set_style(textbox, SGL_STYLE_BG_TRANSPARENT, 1);
    // sgl_obj_set_style(textbox, SGL_STYLE_PIXMAP, SGL_PIXMAP(bg_pixmap));

    uint8_t x = 0;
    while (!quit) {
        //SDL_Delay(10);
        SDL_PollEvent(&MouseEvent);
        switch (MouseEvent.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        }

        sgl_obj_set_style(arc, SGL_STYLE_ARC_END_ANGLE, x * 360 / 100 );
        sprintf(arcbuf, "H:%d%%", x);

        if(x == 100) {
            x = 0;
        }

        sgl_task_handle();

        //sgl_obj_set_dirty(sgl_screen_act());
        x +=1;

        sgl_port_sdl2_increase_frame_count(sdl2_dev);
    }

    sgl_port_sdl2_deinit(sdl2_dev);

    return 0;
}
