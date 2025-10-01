/* source/draw/sgl_draw.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Li, Shanwen  (1477153217@qq.com)
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

#include <sgl_draw.h>
#include <sgl_math.h>


/**
 * @brief draw widget slice completely
 * @param obj it should point to active root object
 * @param surf surface that draw to
 * @param dirty_h dirty height
 * @return none
 */
static inline void draw_widget_slice(sgl_obj_t *obj, sgl_surf_t *surf, int16_t dirty_h)
{
    sgl_event_t evt;
    sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX], *child = NULL;
    int top = 0;

    stack[top++] = obj;

    while (top > 0) {
        obj = stack[--top];

        /* if obj is hide or invalid, skip it, of course it' obj also hide */
        if(unlikely(sgl_obj_is_invalid(obj))) {
            continue;
        }

        /* dispatch the draw event */
        evt.type = SGL_EVENT_DRAW_MAIN;

        /* if surface is overlap with obj ares, call construct function to execute draw event */
        if(sgl_surf_area_is_overlap(surf, &obj->area)) {
            SGL_ASSERT(obj->construct_fn != NULL);
            obj->construct_fn(surf, obj, &evt);

            sgl_obj_for_each_child(child, obj) {
                stack[top++] = child;
            }
        }
    }

    /* flush dirty area into screen */
    sgl_panel_flush_area(surf->x, surf->y, surf->w, dirty_h, surf->buffer);
}


/**
 * @brief calculate dirty area by for each all object that is dirty and visible
 * @param obj it should point to active root object
 * @param dirty the dirty area that need to upate
 * @return true if there is dirty area, otherwise false
 * @note if there is no dirty area, the dirty area will remain unchanged
 */
static bool draw_calculate_dirty_area(sgl_obj_t *obj, sgl_area_t *dirty)
{
    bool need_draw = false;
    sgl_obj_t *child = NULL, *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    /* for each all object from the first task of page */
    while (top > 0) {
        obj = stack[--top];

        if(unlikely(sgl_obj_is_hidden(obj))) {
            continue;
        }

        /* check if obj is destroyed */
        if(unlikely(sgl_obj_is_destroyed(obj))) {
            /* merge destroy area */
            sgl_area_selfmerge(dirty, &obj->area);

            /* free obj resource */
            sgl_obj_free(obj);

            /* remove obj from parent */
            sgl_obj_remove(obj);

            /* update parent layout */
            sgl_obj_set_layout(obj->parent, (sgl_layout_type_t)obj->parent->layout);

            need_draw = true;
            /* obj is destroyed, skip */
            continue;
        }

        /* check child need init coords */
        if(unlikely(sgl_obj_is_needinit(obj))) {
            sgl_event_t evt = {
                .type = SGL_EVENT_DRAW_INIT,
            };

            /* check construct function */
            SGL_ASSERT(obj->construct_fn != NULL);
            obj->construct_fn(NULL, obj, &evt);
            /* maybe no need to clear flag */
            sgl_obj_clear_needinit(obj);
        }

        /* check child dirty and merge all dirty area */
        if(sgl_obj_is_dirty(obj)){

            /* update obj area */
            if(unlikely(!sgl_area_clip(&obj->parent->area, &obj->coords, &obj->area))) {
                sgl_obj_set_invalid(obj);
                continue;
            }
            else {
                sgl_obj_set_valid(obj);
            }

            /* merge dirty area */
            sgl_area_selfmerge(dirty, &obj->area);

            need_draw = true;

            /* clear dirty flag */
            sgl_obj_clear_dirty(obj);
        }

        sgl_obj_for_each_child(child, obj) {
            stack[top++] = child;
        }
    }

    return need_draw;
}


/**
 * @brief sgl to draw complete frame
 * @param page current page
 * @param dirty the dirty area that need to upate
 * @return none
 * @note this function should be called in deamon thread or cyclic thread
 */
void sgl_draw_frame(sgl_page_t *page, sgl_area_t *dirty)
{
    sgl_surf_t *surf = &page->surf;
    sgl_obj_t *head = &page->obj;

    /* calculate dirty area, if no dirty area, return directly */
    if(! draw_calculate_dirty_area(head, dirty)) {
        return;
    }

    /* fix button press increase area */
    dirty->y1 = sgl_max(dirty->y1 - 2, 0);
    dirty->y2 = sgl_min(dirty->y2 + 3, head->area.y2);

    /* to set start x and y position for dirty area */
    surf->y = dirty->y1;
    surf->x = sgl_max(dirty->x1 - 2, 0);
    surf->w = sgl_min(dirty->x2 - dirty->x1 + 5, head->area.x2 - surf->x);
    surf->h = surf->size / surf->w;

    SGL_LOG_TRACE("start draw dirty area: x = %d, y = %d, w = %d, h = %d, dirty_h = %d", surf->x, surf->y, surf->w, surf->h, dirty->y2 - surf->y);

    while(surf->y < dirty->y2) {
        /* cycle draw widget slice until the end of dirty area */
        draw_widget_slice(head, surf, sgl_min(dirty->y2 - surf->y, surf->h));
        surf->y += surf->h;

        /* swap buffer for dma operation, but it depends on double buffer */
        #if (CONFIG_SGL_DRAW_USE_DMA)
        sgl_surf_buffer_swap(surf);
        #endif
    }
}
