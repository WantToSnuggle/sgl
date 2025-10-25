/* source/include/sgl_misc.c
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

#include <sgl.h>

#if (CONFIG_SGL_BOOT_LOGO)

void sgl_boot_logo(void)
{

}


#endif // !CONFIG_SGL_BOOT_LOGO


#if (CONFIG_SGL_BOOT_ANIMATION)

typedef struct {
    sgl_obj_t *background;
    sgl_obj_t *text;
} logo_t;

/**
 * @brief logo animation path
 * @param anim animation object, it will be logo and background anim
 * @param value animation value
 * @return none
 */
static void logo_anim_path(struct sgl_anim *anim, int32_t value)
{
    sgl_obj_t *obj = (sgl_obj_t*)anim->data;
    sgl_obj_set_pos_x(obj, value);
}


/**
 * @brief logo finished status animation path
 * @param anim animation object
 * @param value animation value
 * @return none
 */
static void finished_anim_path(struct sgl_anim *anim, int32_t value)
{
    logo_t *logo = (logo_t*)anim->data;
    sgl_obj_set_alpha(logo->text, value);
    sgl_obj_set_alpha(logo->background, value);
}


/**
 * @brief sgl boot animation
 * @return none
 * @note this function will block the task, it should be called at before of sgl_task_handle and at after of sgl_init()
 */
void sgl_boot_animation(void)
{
    sgl_anim_t *logo_anim = sgl_anim_create();
    if (logo_anim == NULL) {
        SGL_LOG_ERROR("sgl boot logo anin create failed");
        return;
    }

    sgl_anim_t *bg_anim = sgl_anim_create();
    if (bg_anim == NULL) {
        SGL_LOG_ERROR("sgl boot bg anin create failed");
        return;
    }

    sgl_anim_t *finish_ainm = sgl_anim_create();
    if (finish_ainm == NULL) {
        SGL_LOG_ERROR("sgl boot finish anin create failed");
        return;
    }

    logo_t logo;

    logo.background = sgl_rect_create(NULL);
    if (logo.background == NULL) {
        SGL_LOG_ERROR("sgl boot background create failed");
        return;
    }

    logo.text = sgl_label_create(NULL);
    if (logo.text == NULL) {
        SGL_LOG_ERROR("sgl boot text create failed");
        return;
    }

    sgl_obj_set_size(logo.text, 40, 30);
    sgl_obj_set_color(logo.text, SGL_COLOR_ROYAL_BLUE);
    sgl_obj_set_font(logo.text, &song23);
    sgl_obj_set_pos_align(logo.text, SGL_ALIGN_LEFT_MID);
    sgl_obj_set_text(logo.text, "SGL");

    sgl_obj_set_size(logo.background, 60, 30);
    sgl_obj_set_color(logo.background, SGL_COLOR_LIGHT_GRAY);
    sgl_obj_set_border_width(logo.background, 0);
    sgl_obj_set_radius(logo.background, 8);
    sgl_obj_set_pos_align(logo.background, SGL_ALIGN_RIGHT_MID);

    sgl_anim_set_data(logo_anim, logo.text);
    sgl_anim_set_act_duration(logo_anim, 500);
    sgl_anim_set_start_value(logo_anim, 0);
    sgl_anim_set_end_value(logo_anim, sgl_panel_resolution_width() / 2 - 20);
    sgl_anim_set_path(logo_anim, logo_anim_path, SGL_ANIM_PATH_EASE_IN);
    sgl_anim_set_repeat_cnt(logo_anim, 1);
    sgl_anim_start(logo_anim);

    sgl_anim_set_data(bg_anim, logo.background);
    sgl_anim_set_act_duration(bg_anim, 500);
    sgl_anim_set_start_value(bg_anim, sgl_panel_resolution_width());
    sgl_anim_set_end_value(bg_anim,  sgl_panel_resolution_width() / 2 - 30);
    sgl_anim_set_path(bg_anim, logo_anim_path, SGL_ANIM_PATH_EASE_IN);
    sgl_anim_set_repeat_cnt(bg_anim, 1);
    sgl_anim_start(bg_anim);

    while (!sgl_anim_is_finished(logo_anim) && !sgl_anim_is_finished(bg_anim)) {
        sgl_task_handle();
    }
    sgl_anim_free(logo_anim);
    sgl_anim_free(bg_anim);

    sgl_anim_set_data(finish_ainm, &logo);
    sgl_anim_set_act_duration(finish_ainm, 500);
    sgl_anim_set_start_value(finish_ainm, 255);
    sgl_anim_set_end_value(finish_ainm,  0);
    sgl_anim_set_path(finish_ainm, finished_anim_path, SGL_ANIM_PATH_LINEAR);
    sgl_anim_set_repeat_cnt(finish_ainm, 1);
    sgl_anim_start(finish_ainm);

    while (!sgl_anim_is_finished(finish_ainm)) {
        sgl_task_handle();
    }

    sgl_obj_delete(logo.text);
    sgl_obj_delete(logo.background);

    sgl_anim_free(finish_ainm);
    sgl_obj_set_dirty(sgl_screen_act());
}


#endif // !CONFIG_SGL_BOOT_ANIMATION
