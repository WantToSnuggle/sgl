/* source/core/sgl_core.c
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

#include <sgl_core.h>
#include <sgl_anim.h>
#include <sgl_math.h>
#include <sgl_mm.h>
#include <sgl_log.h>
#include <string.h>
#include <sgl_draw.h>
#include <sgl_font.h>
#include <sgl_theme.h>
#include <sgl_draw_cg.h>


#if CONFIG_SGL_USE_OBJ_ID
static size_t obj_global_id = 0;
#endif


/* the maximum depth of object*/
#define  SGL_OBJ_DEPTH_MAX     64


/* current context, page pointer, and dirty area and started flag */
current_ctx_t current_ctx;


/* the frame buffer device, its necessary */
sgl_device_fb_info_t sgl_device_fb = {
    .xres = 0,
    .yres = 0,
    .xres_virtual = 0,
    .yres_virtual = 0,
    .framebuffer_size = 0,
};


/* the log output device for debug, its optional */
sgl_device_log_t sgl_device_log = {
    .log_puts = NULL,
};


/**
 * the memory pool, it will be used to allocate memory for the page pool
*/
static uint8_t sgl_mem_pool[CONFIG_SGL_HEAP_MEMORY_SIZE];


/**
 * @brief register the frame buffer device
 * @param fb_dev the frame buffer device
 * @return int, 0 if success, -1 if failed
 */
int sgl_device_fb_register(sgl_device_fb_t *fb_dev)
{
    sgl_check_ptr_return(fb_dev, -1);

    if(fb_dev->framebuffer == NULL || fb_dev->flush_area == NULL) {
        SGL_LOG_ERROR("framebuffer or flush_area is null");
        return -1;
    }

    sgl_device_fb.framebuffer[0]   = fb_dev->framebuffer;

    /* double buffer for dma mode */
    #if (CONFIG_SGL_DRAW_USE_DMA)
    current_ctx.fb_swap = 0;
    sgl_device_fb.framebuffer_size = fb_dev->framebuffer_size / 2;
    sgl_device_fb.framebuffer[1]   = ((sgl_color_t*)fb_dev->framebuffer) + sgl_device_fb.framebuffer_size;

    if(((int16_t)sgl_device_fb.framebuffer_size) < fb_dev->xres) {
        SGL_LOG_ERROR("framebuffer size is too small");
        return -1;
    }

    #else
    sgl_device_fb.framebuffer_size = fb_dev->framebuffer_size;
    #endif
    sgl_device_fb.xres             = fb_dev->xres;
    sgl_device_fb.yres             = fb_dev->yres;
    sgl_device_fb.xres_virtual     = fb_dev->xres_virtual;
    sgl_device_fb.yres_virtual     = fb_dev->yres_virtual;
    sgl_device_fb.flush_area       = fb_dev->flush_area;

    return 0;
}


/**
 * @brief add object to parent
 * @param parent: pointer of parent object
 * @param obj: pointer of object
 * @return none
 */
void sgl_obj_add_child(sgl_obj_t *parent, sgl_obj_t *obj)
{
    SGL_ASSERT(parent != NULL && obj != NULL);

    sgl_obj_t *tail = NULL;

    if(parent->child) {
        tail = parent->child;
        while(tail->sibling) {
            tail = tail->sibling;
        }
        tail->sibling = obj;
    }
    else {
        parent->child = obj;
    }

    obj->parent = parent;
    obj->sibling = NULL;
}


#if (CONFIG_SGL_OBJ_SLOT_DYNAMIC)

/**
 * @brief add object to task list
 * @param page: pointer of page
 * @param obj: pointer of object
 * @return none
 */
static inline void sgl_obj_add_to_task(sgl_page_t *page, sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL && obj != NULL);
    sgl_list_add_node_at_tail(&page->head, &obj->slot);
}


/**
 * @brief init page slot
 * @param page: pointer of page
 * @return none
 * @note this function only initialize the count to 0 and add obj of page to page slot
 */
static inline void sgl_page_slot_init(sgl_page_t *page)
{
    SGL_ASSERT(page != NULL);
    /* init draw task list  */
    sgl_list_init(&page->head);
    /* add obj of page to page slot */
    sgl_obj_add_to_task(page, &page->obj);
}


/**
 * @brief Add an object to the event task
 * @param head The head of the object list
 * @return none
 */
static void add_obj_to_page_slot(sgl_obj_t *head)
{
    SGL_ASSERT(head != NULL);
    sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    sgl_obj_t *child = NULL;
    sgl_obj_t *current = NULL;
    stack[top++] = head;

    while (top > 0) {
        current = stack[--top];

        sgl_obj_for_each_child(child, current) {
            /* update child's area */
            if(!sgl_area_clip(&current->parent->area, &current->coords, &current->area)) {
                sgl_obj_set_invalid(current);
            }
            sgl_obj_add_to_task(current_ctx.page, child);

            stack[top++] = child;
        }
    }
}


/**
 * @brief free object slot
 * @param obj: pointer of object
 * @return none
 */
static inline void obj_free_slot(sgl_obj_t *obj)
{
    sgl_list_del_node(&obj->slot);
    sgl_free(obj);
}


/**
 * @brief get last child of an object
 * @param obj object
 * @return last child
 */
static inline sgl_obj_t* sgl_obj_get_last_child(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *child = NULL;

    sgl_obj_for_each_child(child, obj) {
        if(child->sibling == NULL) {
            return child;
        }
    }

    return NULL;
}


/**
 * @brief init object slot
 * @param parent: pointer of parent object
 * @param obj: pointer of object
 * @return none
 */
static inline void obj_slot_init(sgl_obj_t *parent, sgl_obj_t *obj)
{
    /* construct object tree */
    sgl_obj_node_init(obj);

    /* if first add object, find the last task and add object to the end of task list */
    if(current_ctx.started) {
        sgl_obj_t *sibling = sgl_obj_get_last_child(parent);
        if(sibling != NULL) {
            sgl_list_add_node_at_after(&sibling->slot, &obj->slot);
        }
        else {
            sgl_list_add_node_at_after(&parent->slot, &obj->slot);
        }
    }

    /* add object to parent's child list */
    sgl_obj_add_child(parent, obj);
}

#else // !CONFIG_SGL_OBJ_SLOT_DYNAMIC
/**
 * @brief add object to page slot
 * @param page: pointer of page
 * @param obj: pointer of object
 * @return none
 */
static inline void sgl_obj_add_to_slot(sgl_page_t *page, sgl_obj_t *obj)
{
    SGL_ASSERT(page != NULL && obj != NULL);
    page->slot[page->slot_count] = obj;
    page->slot_count ++;
}


/**
 * @brief init page slot
 * @param page: pointer of page
 * @return none
 * @note this function only initialize the count to 0 and add obj of page to page slot
 */
static inline void sgl_page_slot_init(sgl_page_t *page)
{
    SGL_ASSERT(page != NULL);
    page->slot_count = 0;
    /* init draw task list  */
    sgl_obj_add_to_slot(page, &page->obj);
}


/**
 * @brief get page slot count
 * @param page: page
 * @return slot count
 */
static inline uint32_t sgl_page_get_slot_count(sgl_page_t *page)
{
    SGL_ASSERT(page != NULL);
    return page->slot_count;
}


/**
 * @brief Add all objects to the page slot
 * @param head The head of the object list
 * @return none
 */
static void add_obj_to_page_slot(sgl_obj_t *head)
{
    SGL_ASSERT(head != NULL);
    sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    sgl_obj_t *child = NULL;
    sgl_obj_t *current = NULL;
    stack[top++] = head;

    while (top > 0) {
        current = stack[--top];

        sgl_obj_for_each_child(child, current) {
            /* update child's area */
            if(!sgl_area_clip(&current->parent->area, &current->coords, &current->area)) {
                sgl_obj_set_invalid(current);
            }

            sgl_obj_add_to_slot(current_ctx.page, child);

            if(current_ctx.page->slot_count > SGL_OBJ_SLOT_SIZE) {
                SGL_LOG_ERROR("too many objects in one page, max is %d", sgl_page_get_slot_count(current_ctx.page));
                return;
            }

            stack[top++] = child;
        }
    }
}


/**
 * @brief free object slot
 * @param obj: pointer of object
 * @return none
 */
static inline void obj_free_slot(sgl_obj_t *obj)
{
    /* free object */
    sgl_free(obj);

    /* clear page slot */
    sgl_page_slot_init(current_ctx.page);
    /* add all object to page slot */
    add_obj_to_page_slot(&current_ctx.page->obj);
}


/**
 * @brief to init object slot
 * @param parent: parent object
 * @param obj: object
 * @return none
 */
static inline void obj_slot_init(sgl_obj_t *parent, sgl_obj_t *obj)
{
    /* add object to parent's child list */
    sgl_obj_add_child(parent, obj);

    /* if first add object, find the last task and add object to the end of task list */
    if(current_ctx.started) {
        current_ctx.page->slot_count = 1;
        add_obj_to_page_slot(&current_ctx.page->obj);
    }
}
#endif // !CONFIG_SGL_OBJ_SLOT_DYNAMIC


/**
 * @brief remove an object from its parent
 * @param obj object to remove
 * @return none
 */
void sgl_obj_remove(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);

    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *pos = NULL;

    /* if the object is active, do not remove it */
    if(obj == sgl_screen_act()) {
        return;
    }

    if(parent->child != obj) {
        pos = parent->child;
        while(pos->sibling != obj) {
            pos = pos->sibling;
        }
        pos->sibling = obj->sibling;
    }
    else {
        parent->child = obj->sibling;
    }

    obj->sibling = NULL;
}


/**
 * @brief  Set the object to be destroyed 
 * @param  obj: the object to set
 * @retval None
 * @note this function is used to set the destroyed flag of the object, then next draw cycle, the object will be removed
 */
void sgl_obj_set_destroyed(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    sgl_obj_t *child = NULL;
    sgl_obj_t *current = NULL;
    stack[top++] = obj;

    while (top > 0) {
        current = stack[--top];
        current->destroyed = 1;

        sgl_obj_for_each_child(child, current) {
            stack[top++] = child;
        }
    }
}


/**
 * @brief Set object to dirty
 * @param obj point to object
 * @return none
 * @note this function will set object to dirty, include its children
 */
void sgl_obj_set_dirty(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    sgl_obj_t *child = NULL;
    sgl_obj_t *current = NULL;
    stack[top++] = obj;

    while (top > 0) {
        current = stack[--top];
        current->dirty = 1;

        sgl_obj_for_each_child(child, current) {
            stack[top++] = child;
        }
    }
}


/**
 * @brief move object position
 * @param obj point to object
 * @param x: x position
 * @param y: y position
 * @return none
 */
static void sgl_obj_move_pos(sgl_obj_t *obj, int16_t x, int16_t y)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    sgl_obj_t *child = NULL;
    sgl_obj_t *current = NULL;
    stack[top++] = obj;

    while (top > 0) {
        current = stack[--top];
        current->dirty = 1;
        current->needinit = 1;
        current->coords.x1 += x;
        current->coords.x2 += x;
        current->coords.y1 += y;
        current->coords.y2 += y;

        sgl_obj_for_each_child(child, current) {
            stack[top++] = child;
        }
    }
}


/**
 * @brief Set object position
 * @param obj point to object
 * @param x: x position
 * @param y: y position
 * @return none
 */
void sgl_obj_set_pos(sgl_obj_t *obj, int16_t x, int16_t y)
{
    SGL_ASSERT(obj != NULL);
    obj->needinit = 1;
    obj->dirty = 1;

    int16_t x_inc = x - obj->coords.x1;
    int16_t y_inc = y - obj->coords.y1;

    sgl_obj_dirty_merge(obj);

    if(obj->parent != NULL) {
        obj->coords.x1 = obj->parent->coords.x1 + x;
        obj->coords.y1 = obj->parent->coords.y1 + y;
    }
    else {
        obj->coords.x1 = x;
        obj->coords.y1 = y;
    }

    obj->coords.x2 = obj->coords.x2 + x_inc;
    obj->coords.y2 = obj->coords.y2 + y_inc;

    sgl_obj_t *child = NULL;
    sgl_obj_for_each_child(child, obj) {
        sgl_obj_move_pos(child, x_inc, y_inc);
    }
}


/**
 * @brief get fix radius of object
 * @param obj object
 * @return fix radius
 * @note if radius is larger than object's width or height, fix radius will be returned
 */
int16_t sgl_obj_fix_radius(sgl_obj_t *obj, size_t radius)
{
    int16_t w = (obj->coords.x2 - obj->coords.x1 + 1) / 2;
    int16_t h = (obj->coords.y2 - obj->coords.y1 + 1) / 2;
    int16_t r_min = w > h ? h : w;

    if((int16_t)radius > r_min) {
        radius = r_min;
    }

    obj->radius = radius & 0xFFF;
    return radius;
}


/**
 * @brief page construct callback function
 * @param surf surface pointer
 * @param obj page object
 * @param evt event
 * @return none
 * @note evt not used
 */
static void sgl_page_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_page_t *page = (sgl_page_t *)obj;
    sgl_pixmap_t *pixmap = page->bg_img;

    if(evt->type == SGL_EVENT_DRAW_MAIN) {
        if(pixmap == NULL) {
            sgl_draw_fill_rect(surf, &obj->area, &obj->coords, page->color);
        }
        else {
            sgl_draw_fill_rect_pixmap(surf, &obj->area, &obj->coords, page->bg_img);
        }
    }
}


/**
 * @brief set page style callback function
 * @param[in] obj pointer to the object
 * @param[in] style pointer to the style
 * @param[in] value value of the style
 * @return none
 */
void sgl_page_set_style(sgl_obj_t* obj, sgl_style_type_t type, size_t value)
{
    sgl_page_t* page = (sgl_page_t*)obj;

    switch (type)
    {
    case SGL_STYLE_COLOR:
        page->color = sgl_int2color(value);
        break;
    
    case SGL_STYLE_PIXMAP:
        page->bg_img = (sgl_pixmap_t*)value;
        break;

    default:
        SGL_LOG_WARN("page: style type not supported");
        break;
    }
}


/**
 * @brief set page style callback function
 * @param[in] obj pointer to the object
 * @param[in] type style type
 * @return size_t, value of the style
 */
size_t sgl_page_get_style(sgl_obj_t* obj, sgl_style_type_t type)
{
    sgl_page_t* page = (sgl_page_t*)obj;

    switch (type)
    {
    case SGL_STYLE_COLOR:
        return sgl_color2int(page->color);

    case SGL_STYLE_PIXMAP:
        return (size_t)page->bg_img;

    default:
        SGL_LOG_WARN("page: style type not supported");
        break;
    }

    return SGL_STYLE_FAILED;
}


/**
 * @brief create a page
 * @param none
 * @return sgl_page_t* the page pointer
 */
static sgl_page_t* sgl_page_create(void)
{
    sgl_page_t *page = sgl_malloc(sizeof(sgl_page_t));
    if(page == NULL) {
        SGL_LOG_ERROR("sgl_page_create: malloc failed");
        return NULL;
    }

    /* clear the page all fields */
    memset(page, 0, sizeof(sgl_page_t));

    sgl_obj_t *obj = &page->obj;

    if(sgl_device_fb.framebuffer[0] == NULL)  {
        SGL_LOG_ERROR("sgl_page_create: framebuffer is NULL");
        sgl_free(page);
        return NULL;
    }

    page->surf.buffer = (sgl_color_t*)sgl_device_fb.framebuffer[0];
    page->surf.x = 0;
    page->surf.y = 0;
    page->surf.w = sgl_device_fb.xres;
    page->surf.h = sgl_device_fb.framebuffer_size / sgl_device_fb.xres;
    page->surf.size = sgl_device_fb.framebuffer_size;
    page->color = SGL_THEME_DESKTOP;

    obj->parent = obj;
    obj->clickable = 1;
    obj->construct_fn = sgl_page_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_page_set_style;
    obj->get_style = sgl_page_get_style;
#endif
    obj->dirty = 1;
    obj->coords = (sgl_area_t){
        .x1 = 0,
        .y1 = 0,
        .x2 = sgl_device_fb.xres,
        .y2 = sgl_device_fb.yres,
    };

    obj->area = obj->coords;

#if CONFIG_SGL_USE_OBJ_ID
    obj->id = obj_global_id;
    obj_global_id ++;
#endif

    /* init child list */
    sgl_obj_node_init(&page->obj);

    /* init slot list */
    sgl_page_slot_init(page);

    if(current_ctx.page == NULL) {
        current_ctx.page = page;
    }

    return page;
}


/**
 * @brief Create an object
 * @param parent parent object
 * @return sgl_obj_t
 * @note if parent is NULL, the object will be as an new page
 */
sgl_obj_t* sgl_obj_create(sgl_obj_t *parent)
{
    sgl_obj_t *obj;

    /* create page object */
    if(parent == NULL) {
        sgl_page_t *page = sgl_page_create();
        if(page == NULL) {
            SGL_LOG_ERROR("sgl_obj_create: create page failed");
            return NULL;
        }
        obj = &page->obj;
        return obj;
    }
    else {
        obj = (sgl_obj_t*)sgl_malloc(sizeof(sgl_obj_t));
        if(obj == NULL) {
            SGL_LOG_ERROR("malloc failed");
            return NULL;
        }

        obj->coords = parent->coords;
        obj->parent = parent;
        obj->event_fn = NULL;
        obj->event_data = 0;
        obj->construct_fn = NULL;
        obj->dirty = 1;

        /* init node */
        sgl_obj_node_init(obj);
        /* add the child into parent's child list */
        sgl_obj_add_child(parent, obj);

        return obj;
    }
}


/**
 * @brief set current object as screen object
 * @param obj object, that you want to set an object as active page
 * @return none
 */
void sgl_screen_load(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    current_ctx.page = (sgl_page_t*)obj;
    current_ctx.started = false;

    /* initilize framebuffer swap */
    #if (CONFIG_SGL_DRAW_USE_DMA)
    current_ctx.fb_swap = 0;
    #endif

    /* initialize dirty area */
    sgl_area_init(&current_ctx.dirty);
}


/**
 * @brief color mixer
 * @param fg_color : foreground color
 * @param bg_color : background color
 * @param factor   : color mixer factor
 * @return sgl_color_t: mixed color
 */
sgl_color_t sgl_color_mixer(sgl_color_t fg_color, sgl_color_t bg_color, uint8_t factor)
{
    sgl_color_t ret;
#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB233)

    ret.ch.red   = bg_color.ch.red + ((fg_color.ch.red - bg_color.ch.red) * (factor >> 5) >> 3);
    ret.ch.green = bg_color.ch.green + ((fg_color.ch.green - bg_color.ch.green) * (factor >> 5) >> 3);
    ret.ch.blue  = bg_color.ch.blue + ((fg_color.ch.blue - bg_color.ch.blue) * (factor >>6) >> 2);

#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB565)

    uint32_t rxb = bg_color.full & 0xF81F;
    rxb += ((fg_color.full & 0xF81F) - rxb) * (factor >> 2) >> 6;
    uint32_t xgx = bg_color.full & 0x07E0;
    xgx += ((fg_color.full & 0x07E0) - xgx) * factor >> 8;
    ret.full = (rxb & 0xF81F) | (xgx & 0x07E0);

#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB888)

    ret.ch.red   = bg_color.ch.red + ((fg_color.ch.red - bg_color.ch.red) * factor >> 8);
    ret.ch.green = bg_color.ch.green + ((fg_color.ch.green - bg_color.ch.green) * factor >> 8);
    ret.ch.blue  = bg_color.ch.blue + ((fg_color.ch.blue - bg_color.ch.blue) * factor >> 8);

#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_ARGB8888)

    ret.ch.alpha = bg_color.ch.alpha + ((fg_color.ch.alpha - bg_color.ch.alpha) * factor >> 8);
    ret.ch.red   = bg_color.ch.red + ((fg_color.ch.red - bg_color.ch.red) * factor >> 8);
    ret.ch.green = bg_color.ch.green + ((fg_color.ch.green - bg_color.ch.green) * factor >> 8);
    ret.ch.blue  = bg_color.ch.blue + ((fg_color.ch.blue - bg_color.ch.blue) * factor >> 8);

#endif
    return ret;
}


/**
 * @brief check surf and other area is overlap
 * @param surf surfcare
 * @param area area b
 * @return true or false, true means overlap, false means not overlap
 * @note: this function is unsafe, you should check the surfcare and area is not NULL by yourself
 */
bool sgl_surf_area_is_overlap(sgl_surf_t *surf, sgl_area_t *area)
{
    SGL_ASSERT(surf != NULL && area != NULL);
    int16_t h_pos = surf->y + surf->h - 1;
    int16_t w_pos = surf->x + surf->w - 1;

    if(area->y1 > h_pos || area->y2 < surf->y || area->x1 > w_pos || area->x2 < surf->x) {
        return false;
    }

    return true;
}


/**
 * @brief check two area is overlap
 * @param area_a area a
 * @param area_b area b
 * @return true or false, true means overlap, false means not overlap
 * @note: this function is unsafe, you should check the area_a and area_b is not NULL by yourself
 */
bool sgl_area_is_overlap(sgl_area_t *area_a, sgl_area_t *area_b)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL);
    if(area_b->y1 > area_a->y2 || area_b->y2 < area_a->y1 || area_b->x1 > area_a->x2 || area_b->x2 < area_a->x1) {
        return false;
    }

    return true;
}


/**
 * @brief  Get area intersection between surface and area
 * @param surf: surface
 * @param area: area
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the surf and area is not NULL by yourself
 */
bool sgl_surf_clip(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *clip)
{
    SGL_ASSERT(surf != NULL && area != NULL && clip != NULL);
    int16_t h_pos = surf->y + surf->h - 1;
    int16_t w_pos = surf->x + surf->w - 1;

    if(area->y1 > h_pos || area->y2 < surf->y || area->x1 > w_pos || area->x2 < surf->x) {
        return false;
    }

    clip->x1 = sgl_max(surf->x, area->x1);
    clip->x2 = sgl_min(w_pos, area->x2);
    clip->y1 = sgl_max(surf->y, area->y1);
    clip->y2 = sgl_min(h_pos, area->y2);

    return true;
}


/**
 * @brief  Get area intersection between two areas
 * @param area_a: area a
 * @param area_b: area b
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the area_a and area_b and clip is not NULL by yourself
 */
bool sgl_area_clip(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *clip)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL && clip != NULL);
    if(area_b->y1 > area_a->y2 || area_b->y2 < area_a->y1 || area_b->x1 > area_a->x2 || area_b->x2 < area_a->x1) {
        return false;
    }

    clip->x1 = sgl_max(area_a->x1, area_b->x1);
    clip->x2 = sgl_min(area_a->x2, area_b->x2);
    clip->y1 = sgl_max(area_a->y1, area_b->y1);
    clip->y2 = sgl_min(area_a->y2, area_b->y2);

    return true;
}


/**
 * @brief clip area with another area
 * @param clip [in][out] clip area
 * @param area [in] area
 * @return true if clip area is valid, otherwise two area is not overlapped
 * @note: this function is unsafe, you should check the clip and area is not NULL by yourself
 */
bool sgl_area_selfclip(sgl_area_t *clip, sgl_area_t *area)
{
    SGL_ASSERT(clip != NULL && area != NULL);
    if(area->y1 > clip->y2 || area->y2 < clip->y1 || area->x1 > clip->x2 || area->x2 < clip->x1) {
        return false;
    }

    clip->x1 = sgl_max(clip->x1, area->x1);
    clip->x2 = sgl_min(clip->x2, area->x2);
    clip->y1 = sgl_max(clip->y1, area->y1);
    clip->y2 = sgl_min(clip->y2, area->y2);

    return true;
}


/**
 * @brief merge two area, the merge is result of the two area clip
 * @param area_a [in] area1
 * @param area_b [in] area2
 * @param merge  [out] merge result
 * @return none
 * @note: this function is unsafe, you should check the area_a and area_b and merge is not NULL by yourself
 */
void sgl_area_merge(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *merge)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL && merge != NULL);
    merge->x1 = sgl_min(area_a->x1, area_b->x1);
    merge->x2 = sgl_max(area_a->x2, area_b->x2);
    merge->y1 = sgl_min(area_a->y1, area_b->y1);
    merge->y2 = sgl_max(area_a->y2, area_b->y2);
}


/**
 * @brief merge two area, the merge is a new area
 * @param merge [in][out] merge area
 * @param area [in] area
 * @return none
 * @note: this function is unsafe, you should check the merge and area is not NULL by yourself
 */
void sgl_area_selfmerge(sgl_area_t *merge, sgl_area_t *area)
{
    SGL_ASSERT(merge != NULL && area != NULL);
    merge->x1 = sgl_min(merge->x1, area->x1);
    merge->x2 = sgl_max(merge->x2, area->x2);
    merge->y1 = sgl_min(merge->y1, area->y1);
    merge->y2 = sgl_max(merge->y2, area->y2); 
}


/**
 * @brief merge area with current dirty area
 * @param merge [in] merge area
 * @return none
 */
void sgl_obj_dirty_merge(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_area_selfmerge(&current_ctx.dirty, &obj->area);
}


/**
 * @brief sgl global initialization
 * @param none
 * @return none
 * @note you should call this function before using sgl
 */
void sgl_init(void)
{
    /* init memory pool */
    sgl_mm_init(sgl_mem_pool, sizeof(sgl_mem_pool));

    /* initialize current context */
    current_ctx.page = NULL;
    current_ctx.started = false;
    sgl_area_init(&current_ctx.dirty);

    /* create a screen object for drawing */
    sgl_obj_create(NULL);

    /* create event queue */
    sgl_event_queue_init();
}


/**
 * @brief sgl set object layout type
 * @param obj [in] object
 * @param type [in] layout type, SGL_LAYOUT_NONE, SGL_LAYOUT_HORIZONTAL, SGL_LAYOUT_VERTICAL, SGL_LAYOUT_GRID
 * @return none
 */
void sgl_obj_set_layout(sgl_obj_t *obj, sgl_layout_type_t type)
{
    SGL_ASSERT(obj != NULL);
    obj->layout = (((uint8_t)type) & 0x03);

    if((!sgl_obj_has_child(obj)) || (type == SGL_LAYOUT_NONE)) {
        return;
    }

    sgl_obj_t *child = NULL;
    size_t child_num = sgl_obj_get_child_count(obj);
    int16_t margin = obj->margin, child_w, child_xs, child_h, child_ys;

    /* set object to dirty flag for layout change */
    sgl_obj_set_dirty(obj);

    switch(obj->layout) {
    case SGL_LAYOUT_HORIZONTAL:
        child_w  = (obj->coords.x2 - obj->coords.x1 - margin * (child_num + 1)) / child_num;
        child_xs = obj->coords.x1 + margin;

        sgl_obj_for_each_child(child, obj) {
            child->coords.x1 = child_xs;
            child->coords.x2 = child->coords.x1 + child_w;
            child_xs += (child_w + margin);

            child->coords.y1 = obj->coords.y1 + margin;
            child->coords.y2 = obj->coords.y2 - margin;
        }
        break;

    case SGL_LAYOUT_VERTICAL:
        child_h  = (obj->coords.y2 - obj->coords.y1 - margin * (child_num + 1)) / child_num;
        child_ys = obj->coords.y1 + margin;

        sgl_obj_for_each_child(child, obj) {
            child->coords.y1 = child_ys;
            child->coords.y2 = child->coords.y1 + child_h;
            child_ys += (child_h + margin);

            child->coords.x1 = obj->coords.x1 + margin;
            child->coords.x2 = obj->coords.x2 - margin;
        }
        break;

    case SGL_LAYOUT_GRID:
        // TODO: set grid layout
        break;
    }
}


/**
 * @brief initialize object
 * @param obj object
 * @param parent parent object
 * @return int, 0 means successful, -1 means failed
 */
int sgl_obj_init(sgl_obj_t *obj, sgl_obj_t *parent)
{
    SGL_ASSERT(obj != NULL);

    if(parent == NULL) {
        parent = sgl_screen_act();
        if(parent == NULL) {
            SGL_LOG_ERROR("sgl_button_create: have no active page");
            return -1;
        }
    }

    /* set essential member */
    obj->coords = parent->coords;
    obj->parent = parent;
    obj->event_fn = NULL;
    obj->event_data = 0;
    obj->construct_fn = NULL;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = NULL;
    obj->get_style = NULL;
#endif
    obj->dirty = 1;
    obj->clickable = 0;

    /* init object area to invalid */
    sgl_area_init(&obj->area);

#if CONFIG_SGL_USE_OBJ_ID
    obj->id = obj_global_id;
    obj_global_id ++;
#endif

    obj_slot_init(parent, obj);

    /* set layout to parent layout flag */
    sgl_obj_set_layout(parent, (sgl_layout_type_t)parent->layout);

    return 0;
}


/**
 * @brief  free an object
 * @param  obj: object to free
 * @retval none
 * @note this function will free all the children of the object
 */
void sgl_obj_free(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    /* if the object is active, do nothing */
    if(obj == sgl_screen_act()) {
        /* clear destroyed flag */
        obj->destroyed = 0;
        return;
    }
    
    obj_free_slot(obj);
}


#if (CONFIG_SGL_USE_OBJ_ID && CONFIG_SGL_DEBUG)
/**
 * @brief Print all task id of draw task list
 * @param  none
 * @return none
 */
static inline void sgl_obj_print_all_task_id(void)
{
    sgl_obj_t *pos = NULL;
    sgl_page_t *page = current_ctx.page;

    sgl_page_for_each_slot(pos, page) {
        SGL_LOG_INFO("DRAW TASK  ID:  %d", pos->id);
    }
}
#endif


#if(CONFIG_SGL_TEXT_UTF8 == 1)

/**
 * @brief Convert UTF-8 string to Unicode
 * @param utf8_str Pointer to the UTF-8 string to be converted
 * @param p_unicode_buffer Pointer to the buffer where the converted Unicode will be stored
 * @return The number of bytes in the UTF-8 string
 */
uint32_t sgl_utf8_to_unicode(const char *utf8_str, uint32_t *p_unicode_buffer)
{
    int bytes = 0;
    if (((uint8_t)(*utf8_str)) < 0x80) { // 1-byte/7-bit ASCII
        bytes = 1;
        *p_unicode_buffer = utf8_str[0];
    }
    else if ((((uint8_t)(*utf8_str)) & 0xE0) == 0xC0) { // 2-byte
        bytes = 2;
        *p_unicode_buffer = (utf8_str[0] & 0x1F) << 6;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F);
    }
    else if ((((uint8_t)(*utf8_str)) & 0xF0) == 0xE0) { // 3-byte
        bytes = 3;
        *p_unicode_buffer = (utf8_str[0] & 0x0F) << 12;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F) << 6;
        *p_unicode_buffer |= (utf8_str[2] & 0x3F);        
    }
    else if ((((uint8_t)(*utf8_str)) & 0xF8) == 0xF0) { // 4-byte
        bytes = 4;
        *p_unicode_buffer = (utf8_str[0] & 0x07) << 18;
        *p_unicode_buffer |= (utf8_str[2] & 0x3F) << 6;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F) << 12;
        *p_unicode_buffer |= (utf8_str[3] & 0x3F);        
    }
    return bytes;
}


/**
 * @brief Search for the index of a Unicode character in the font table
 * @param font Pointer to the font structure containing character data
 * @param unicode Unicode of the character to be searched
 * @return Index of the character in the font table
 */
uint32_t sgl_search_unicode_ch_index(sgl_font_t *font, uint32_t unicode)
{
    for (uint32_t i = 0; i < font->unicode_list_len; i++){
        if (font->unicode_list[i] == unicode) {
            return i;
        }
    }
    return 0;
}

#endif // !CONFIG_SGL_TEXT_UTF8


/**
 * @brief get the width of a string
 * @param str string
 * @param font sgl font
 * @return width of string
 */
int32_t sgl_font_get_string_width(const char *str, sgl_font_t *font)
{
    SGL_ASSERT(font != NULL);
    int32_t len = 0;
#if CONFIG_SGL_TEXT_UTF8
    uint32_t unicode = 0;
    uint32_t ch_index = 0;
    while(*str) {
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        len += font->table[ch_index].box_w;
    }
#else
    while(*str) {
        len += font->table[(uint8_t)(*str) - 32].box_w;
        str++;
    }
#endif
    return len;
}


/**
 * @brief get the alignment position
 * @param parent_size parent size
 * @param size object size
 * @param type alignment type
 * @return alignment position offset
 */
sgl_pos_t sgl_get_align_pos(sgl_size_t *parent_size, sgl_size_t *size, sgl_align_type_t type)
{
    SGL_ASSERT(parent_size != NULL && size != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    switch(type) {
        case SGL_ALIGN_CENTER:
            ret.x = (parent_size->w - size->w)/2;
            ret.y = (parent_size->h - size->h)/2;
        break;

        case SGL_ALIGN_TOP_MID:          
            ret.x = (parent_size->w - size->w)/2;
            ret.y = 0;
        break;

        case SGL_ALIGN_TOP_LEFT:
            ret.x = 0;
            ret.y = 0;
        break; 
            
        case SGL_ALIGN_TOP_RIGHT:    
            ret.x = parent_size->w - size->w;
            ret.y = 0;
        break;

        case SGL_ALIGN_BOT_MID:
            ret.x = (parent_size->w - size->w)/2;
            ret.y = parent_size->h - size->h;
        break;

        case SGL_ALIGN_BOT_LEFT:
            ret.x = 0;
            ret.y = parent_size->h - size->h;
        break;

        case SGL_ALIGN_BOT_RIGHT:
            ret.x = parent_size->w - size->w;
            ret.y = parent_size->h - size->h;
        break;

        case SGL_ALIGN_LEFT_MID:
            ret.x = 0;
            ret.y = (parent_size->h - size->h)/2;
        break;

        case SGL_ALIGN_RIGHT_MID:
            ret.x = parent_size->w - size->w;
            ret.y = (parent_size->h - size->h)/2;
        break;

        default: break;
    }
    return ret;
}


/**
 * @brief get the text position in the area
 * @param area point to area
 * @param font point to font
 * @param text text string
 * @param offset text offset
 * @param type alignment type
 * @return sgl_pos_t position of text
 */
sgl_pos_t sgl_get_text_pos(sgl_area_t *area, sgl_font_t *font, const char *text, int16_t offset, sgl_align_type_t type)
{
    SGL_ASSERT(area != NULL && font != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    sgl_size_t parent_size = {
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };

    sgl_size_t text_size = {
        .w = sgl_font_get_string_width(text, font) + offset,
        .h = sgl_font_get_height(font),
    };

    ret = sgl_get_align_pos(&parent_size, &text_size, type);
    ret.x += area->x1;
    ret.y += area->y1;

    return ret;
}


/**
 * @brief get the icon position of area
 * @param area point to area
 * @param icon point to icon
 * @param offset offset
 * @param type align type
 */
sgl_pos_t sgl_get_icon_pos(sgl_area_t *area, sgl_icon_pixmap_t *icon, int16_t offset, sgl_align_type_t type)
{
    SGL_ASSERT(area != NULL && icon != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    sgl_size_t parent_size = {
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };

    sgl_size_t text_size = {
        .w = icon->width + offset,
        .h = icon->height,
    };

    ret = sgl_get_align_pos(&parent_size, &text_size, type);
    ret.x += area->x1;
    ret.y += area->y1;

    return ret;
}


/**
 * @brief Set the alignment position of the object relative to its parent object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 */
void sgl_obj_set_align(sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(obj != NULL);

    sgl_size_t p_size   = {0};
    sgl_pos_t  p_pos    = {0};
    sgl_pos_t  obj_pos  = {0};
    sgl_size_t obj_size = {
        .w = obj->coords.x2 - obj->coords.x1 + 1,
        .h = obj->coords.y2 - obj->coords.y1 + 1,
    };

    if(obj->parent == NULL) {
        p_size = (sgl_size_t){
            .w = sgl_device_fb.xres,
            .h = sgl_device_fb.yres,
        };
        p_pos = (sgl_pos_t){
            .x = 0,
            .y = 0,
        };
    }
    else {
        p_size = (sgl_size_t){
            .w = obj->parent->coords.x2 - obj->parent->coords.x1 + 1,
            .h = obj->parent->coords.y2 - obj->parent->coords.y1 + 1,
        };
        p_pos = (sgl_pos_t){
            .x = obj->parent->coords.x1,
            .y = obj->parent->coords.y1,
        };
    }

    obj_pos = sgl_get_align_pos(&p_size, &obj_size, type);

    sgl_obj_set_pos(obj, p_pos.x + obj_pos.x,
                         p_pos.y + obj_pos.y
                    );
}


/**
 * @brief sgl task handle function
 * @param none
 * @return none
 * @note this function should be called in main loop or timer or thread
 */
void sgl_task_handle(void)
{
    /* only run once that add all object to draw list */
    if(unlikely(current_ctx.started == false)){
        /* add all child object of current page to draw list */
        add_obj_to_page_slot(&current_ctx.page->obj);

        /* clear flag */
        current_ctx.started = true;

        #if (CONFIG_SGL_DEBUG && CONFIG_SGL_USE_OBJ_ID)
        sgl_obj_print_all_task_id();
        #endif
    };

    /* event task */
    sgl_event_task();

    #if (CONFIG_SGL_ANIMATION)
    sgl_anim_task();
    #endif // !CONFIG_SGL_ANIMATION

    /* draw task  */
    sgl_draw_frame(current_ctx.page, &current_ctx.dirty);

    /* clear dirty area */
    sgl_area_init(&current_ctx.dirty);
}
