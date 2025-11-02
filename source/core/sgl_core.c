/* source/core/sgl_core.c
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
#include <sgl_anim.h>
#include <sgl_math.h>
#include <sgl_mm.h>
#include <sgl_log.h>
#include <string.h>
#include <sgl_draw.h>
#include <sgl_font.h>
#include <sgl_theme.h>


/* current context, page pointer, and dirty area and started flag */
sgl_context_t sgl_ctx = {
    .fb_dev = {
        .xres = 0,
        .yres = 0,
        .xres_virtual = 0,
        .yres_virtual = 0,
        .framebuffer_size = 0,
    },
    .log_dev = {
        .log_puts = NULL,
    },
    .page = NULL,
    .started = false,
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

    if (fb_dev->framebuffer == NULL || fb_dev->flush_area == NULL) {
        SGL_LOG_ERROR("framebuffer pointer or flush_area function pointer is null");
        SGL_ASSERT(0);
        return -1;
    }

    sgl_ctx.fb_dev.framebuffer[0]   = fb_dev->framebuffer;

    /* double buffer for dma mode */
#if (CONFIG_SGL_DRAW_USE_DMA)
    sgl_ctx.fb_swap = 0;
    sgl_ctx.fb_dev.framebuffer_size = fb_dev->framebuffer_size / 2;
    sgl_ctx.fb_dev.framebuffer[1]   = ((sgl_color_t*)fb_dev->framebuffer) + sgl_ctx.fb_dev.framebuffer_size;

    if ((sgl_ctx.fb_dev.framebuffer_size) < (size_t)fb_dev->xres) {
        SGL_LOG_ERROR("framebuffer size is too small");
        SGL_ASSERT(0);
        return -1;
    }

#else
    sgl_ctx.fb_dev.framebuffer_size = fb_dev->framebuffer_size;
#endif
    sgl_ctx.fb_dev.xres             = fb_dev->xres;
    sgl_ctx.fb_dev.yres             = fb_dev->yres;
    sgl_ctx.fb_dev.xres_virtual     = fb_dev->xres_virtual;
    sgl_ctx.fb_dev.yres_virtual     = fb_dev->yres_virtual;
    sgl_ctx.fb_dev.flush_area       = fb_dev->flush_area;

    return 0;
}


/**
 * @brief initialize dirty area
 * @param none
 * @return none
 */
static inline void sgl_dirty_area_init(void)
{
#if CONFIG_SGL_DIRTY_AREA_THRESHOLD
    sgl_ctx.dirty_num = 0;
#else
    sgl_area_init(&sgl_ctx.dirty);
#endif
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
    sgl_obj_t *tail = parent->child;

    if (parent->child) {
        while (tail->sibling != NULL) {
            tail = tail->sibling;
        };
        tail->sibling = obj;
    }
    else {
        parent->child = obj;
    }

    obj->parent = parent;
}


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

    if (parent->child != obj) {
        pos = parent->child;
        while (pos->sibling != obj) {
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
 * @brief Set object position
 * @param obj point to object
 * @param x: x position
 * @param y: y position
 * @return none
 */
void sgl_obj_set_pos(sgl_obj_t *obj, int16_t x, int16_t y)
{
    SGL_ASSERT(obj != NULL);
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    int16_t x_inc = x - obj->coords.x1;
    int16_t y_inc = y - obj->coords.y1;

    obj->dirty = 1;
    obj->needinit = 1;
    obj->coords.x1 = x;
    obj->coords.x2 += x_inc;
    obj->coords.y1 = y;
    obj->coords.y2 += y_inc;

    sgl_obj_dirty_merge(obj);

    if (obj->child == NULL) {
        return;
    }
    stack[top++] = obj->child;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        obj->dirty = 1;
        obj->needinit = 1;
        obj->coords.x1 += x_inc;
        obj->coords.x2 += x_inc;
        obj->coords.y1 += y_inc;
        obj->coords.y2 += y_inc;

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}


/**
 * @brief move object up a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_up(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;
    sgl_obj_t *next = NULL;

    /* if the object is the last child, do not move it */
    if (obj->sibling == NULL) {
        return;
    }
    else if (parent->child == obj) {
        parent->child = obj->sibling;
        obj->sibling = obj->sibling->sibling;
        /* mark object as dirty */
        sgl_obj_set_dirty(obj);
        return;
    }

    /* move the object to its next sibling */
    sgl_obj_for_each_child(prev, parent) {
        if (prev->sibling == obj) {
            next = obj->sibling;
            obj->sibling = next->sibling;
            prev->sibling = next;
            next->sibling = obj;
            /* mark object as dirty */
            sgl_obj_set_dirty(obj);
            return;
        }
    }
}


/**
 * @brief move object down a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_down(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;
    sgl_obj_t *gprev = NULL;

    /* if the object is the first child, do not move it */
    if (parent->child == obj || obj->sibling == NULL) {
        return;
    }
    else if (parent->child->sibling == obj) {
        parent->child->sibling = obj->sibling;
        obj->sibling = parent->child;
        parent->child = obj;
        /* mark object as dirty */
        sgl_obj_set_dirty(obj);
        return;
    }

    /* move the object to its prev sibling */
    sgl_obj_for_each_child(gprev, parent) {
        prev = gprev->sibling;

        if (prev->sibling == obj) {
            prev->sibling = obj->sibling;
            gprev->sibling = obj;
            obj->sibling = prev;
            /* mark object as dirty */
            sgl_obj_set_dirty(obj);
            return;
        }
    }
}


/**
 * @brief move object top level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_foreground(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL && obj->parent != NULL);

    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *last = NULL;
    sgl_obj_t *prev = NULL;

    /* if the object is the last child, do not move it */
    if (obj->sibling == NULL) {
        return;
    }

    sgl_obj_for_each_child(last, parent) {
        if (last->sibling == obj) {
            prev = last;
        }
    }

    prev->sibling = obj->sibling;
    obj->sibling  = last->sibling;
    last->sibling = obj;
    /* mark object as dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief move object bottom level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_background(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;

    /* if the object is the first child, do not move it */
    if (parent->child == obj) {
        return;
    }

    sgl_obj_for_each_child(prev, parent) {
        if (prev->sibling == obj) {
            break;
        }
    }

    prev->sibling = obj->sibling;
    obj->sibling = parent->child;
    parent->child = obj;
    /* mark object as dirty */
    sgl_obj_set_dirty(obj);
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

    if ((int16_t)radius > r_min) {
        radius = r_min;
    }

    obj->radius = radius & 0xFFF;
    return radius;
}


#if (CONFIG_SGL_OBJ_USE_NAME && CONFIG_SGL_DEBUG)
/**
 * @brief print object name that include this all child
 * @param obj point to object
 * @return none
 */
void sgl_obj_print_name(sgl_obj_t *obj)
{
    int top = 0;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    stack[top++] = obj;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        if (obj->name == NULL) {
            SGL_LOG_INFO("[OBJ NAME]: %s", "NULL");
        }
        else {
            SGL_LOG_INFO("[OBJ NAME]: %s", obj->name);
        }

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}

#endif


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
    const sgl_pixmap_t *pixmap = page->bg_img;

    if (evt->type == SGL_EVENT_DRAW_MAIN) {
        if (pixmap == NULL) {
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
    if (page == NULL) {
        SGL_LOG_ERROR("sgl_page_create: malloc failed");
        return NULL;
    }

    /* clear the page all fields */
    memset(page, 0, sizeof(sgl_page_t));

    sgl_obj_t *obj = &page->obj;

    if (sgl_ctx.fb_dev.framebuffer[0] == NULL) {
        SGL_LOG_ERROR("sgl_page_create: framebuffer is NULL");
        sgl_free(page);
        return NULL;
    }

    page->surf.buffer = (sgl_color_t*)sgl_ctx.fb_dev.framebuffer[0];
    page->surf.x = 0;
    page->surf.y = 0;
    page->surf.w = sgl_ctx.fb_dev.xres;
    page->surf.h = sgl_ctx.fb_dev.framebuffer_size / sgl_ctx.fb_dev.xres;
    page->surf.size = sgl_ctx.fb_dev.framebuffer_size;
    page->color = SGL_THEME_DESKTOP;

    obj->parent = obj;
    obj->clickable = 0;
    obj->construct_fn = sgl_page_construct_cb;
#if CONFIG_SGL_USE_STYLE_UNIFIED_API
    obj->set_style = sgl_page_set_style;
    obj->get_style = sgl_page_get_style;
#endif
    obj->dirty = 1;
    obj->coords = (sgl_area_t) {
        .x1 = 0,
        .y1 = 0,
        .x2 = sgl_ctx.fb_dev.xres - 1,
        .y2 = sgl_ctx.fb_dev.yres - 1,
    };

    obj->area = obj->coords;

    /* init child list */
    sgl_obj_node_init(&page->obj);

    if (sgl_ctx.page == NULL) {
        sgl_ctx.page = page;
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
    if (parent == NULL) {
        sgl_page_t *page = sgl_page_create();
        if (page == NULL) {
            SGL_LOG_ERROR("sgl_obj_create: create page failed");
            return NULL;
        }
        obj = &page->obj;
        return obj;
    }
    else {
        obj = (sgl_obj_t*)sgl_malloc(sizeof(sgl_obj_t));
        if (obj == NULL) {
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
 * @brief sgl global initialization
 * @param none
 * @return none
 * @note you should call this function before using sgl and you should call this function after register framebuffer device
 */
void sgl_init(void)
{
    /* init memory pool */
    sgl_mm_init(sgl_mem_pool, sizeof(sgl_mem_pool));

    /* initialize current context */
    sgl_ctx.page = NULL;
    sgl_ctx.started = false;

#if (CONFIG_SGL_DIRTY_AREA_THRESHOLD)
    /* alloc memory for dirty area */
    sgl_ctx.dirty_num = ((sgl_panel_resolution_height() + SGL_DIRTY_AREA_THRESHOLD - 1) / SGL_DIRTY_AREA_THRESHOLD) *
                        ((sgl_panel_resolution_width()  + SGL_DIRTY_AREA_THRESHOLD - 1) / SGL_DIRTY_AREA_THRESHOLD);

    sgl_ctx.dirty = sgl_malloc(sgl_ctx.dirty_num * sizeof(sgl_area_t));
    if (sgl_ctx.dirty == NULL) {
        SGL_LOG_ERROR("sgl dirty area memory alloc failed");
        SGL_ASSERT(0);
        return;
    }
#endif // !CONFIG_SGL_DIRTY_AREA_THRESHOLD

    /* initialize dirty area */
    sgl_dirty_area_init();

    /* create a screen object for drawing */
    sgl_obj_create(NULL);

    /* create event queue */
    sgl_event_queue_init();
}


/**
 * @brief set current object as screen object
 * @param obj object, that you want to set an object as active page
 * @return none
 */
void sgl_screen_load(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_ctx.page = (sgl_page_t*)obj;
    sgl_ctx.started = false;

    /* initilize framebuffer swap */
#if (CONFIG_SGL_DRAW_USE_DMA)
    sgl_ctx.fb_swap = 0;
#endif

    /* initialize dirty area */
    sgl_dirty_area_init();
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

    if (area->y1 > h_pos || area->y2 < surf->y || area->x1 > w_pos || area->x2 < surf->x) {
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
    if (area_b->y1 > area_a->y2 || area_b->y2 < area_a->y1 || area_b->x1 > area_a->x2 || area_b->x2 < area_a->x1) {
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
    if (area->y1 > clip->y2 || area->y2 < clip->y1 || area->x1 > clip->x2 || area->x2 < clip->x1) {
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
#if CONFIG_SGL_DIRTY_AREA_THRESHOLD
    int interval_x, interval_y;

    /* skip invalid area */
    if (obj->area.x1 >= obj->area.x2 || obj->area.y1 >= obj->area.y2) {
        return;
    }

    for (int i = 0; i < sgl_ctx.dirty_num; i++) {
        if (obj->area.x2 < sgl_ctx.dirty[i].x1) {
            interval_x = sgl_ctx.dirty[i].x1 - obj->area.x2;
        }
        else if (obj->area.x1 > sgl_ctx.dirty[i].x2) {
            interval_x = obj->area.x1 - sgl_ctx.dirty[i].x2;
        }
        else {
            interval_x = 0;
        }

        if (obj->area.y2 < sgl_ctx.dirty[i].y1) {
            interval_y = sgl_ctx.dirty[i].y1 - obj->area.y2;
        }
        else if (obj->area.y1 > sgl_ctx.dirty[i].y2) {
            interval_y = obj->area.y1 - sgl_ctx.dirty[i].y2;
        }
        else {
            interval_y = 0;
        }

        /* If the object's area is near the dirty rectangle, merge it.*/
        if (interval_x <= SGL_DIRTY_AREA_THRESHOLD && interval_y <= SGL_DIRTY_AREA_THRESHOLD) {
            /* merge object area with dirty area */
            sgl_ctx.dirty[i].x1 = sgl_min(sgl_ctx.dirty[i].x1, obj->area.x1);
            sgl_ctx.dirty[i].x2 = sgl_max(sgl_ctx.dirty[i].x2, obj->area.x2);
            sgl_ctx.dirty[i].y1 = sgl_min(sgl_ctx.dirty[i].y1, obj->area.y1);
            sgl_ctx.dirty[i].y2 = sgl_max(sgl_ctx.dirty[i].y2, obj->area.y2); 

            return;
        }
    }

    sgl_ctx.dirty[sgl_ctx.dirty_num] = obj->area;
    sgl_ctx.dirty_num ++;
#else
    /* direct to merge object area with dirty area  */
    sgl_ctx.dirty.x1 = sgl_min(sgl_ctx.dirty.x1, obj->area.x1);
    sgl_ctx.dirty.x2 = sgl_max(sgl_ctx.dirty.x2, obj->area.x2);
    sgl_ctx.dirty.y1 = sgl_min(sgl_ctx.dirty.y1, obj->area.y1);
    sgl_ctx.dirty.y2 = sgl_max(sgl_ctx.dirty.y2, obj->area.y2); 
#endif
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

    if ((!sgl_obj_has_child(obj)) || (type == SGL_LAYOUT_NONE)) {
        return;
    }

    sgl_obj_t *child = NULL;
    size_t child_num = sgl_obj_get_child_count(obj);
    int16_t child_span[128] = {0}, i = 0, child_pos = 0;

    /* set object to dirty flag for layout change */
    sgl_obj_set_dirty(obj);

    switch (obj->layout) {
    case SGL_LAYOUT_HORIZONTAL:
        sgl_split_len_avg((obj->coords.x2 - obj->coords.x1 + 1), child_num, obj->margin, child_span);
        child_pos = obj->coords.x1 + obj->margin;

        sgl_obj_for_each_child(child, obj) {
            child->coords.x1 = child_pos;
            child->coords.x2 = child_pos + child_span[i] - 1;
            child->coords.y1 = obj->coords.y1 + obj->margin;
            child->coords.y2 = obj->coords.y2 - obj->margin;
            child_pos += (child_span[i++] + obj->margin);
        }
        break;

    case SGL_LAYOUT_VERTICAL:
        sgl_split_len_avg((obj->coords.y2 - obj->coords.y1 + 1), child_num, obj->margin, child_span);
        child_pos = obj->coords.x1 + obj->margin;

        sgl_obj_for_each_child(child, obj) {
            child->coords.x1 = obj->coords.x1 + obj->margin;
            child->coords.x2 = obj->coords.x2 - obj->margin;
            child->coords.y1 = child_pos;
            child->coords.y2 = child_pos + child_span[i] - 1;
            child_pos += (child_span[i++] + obj->margin);
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

    if (parent == NULL) {
        parent = sgl_screen_act();
        if (parent == NULL) {
            SGL_LOG_ERROR("sgl_obj_init: have no active page");
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

    /* add the child into parent's child list */
    sgl_obj_add_child(parent, obj);

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
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}

        sgl_free(obj);
    }
}


#if (CONFIG_SGL_TEXT_UTF8)
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
uint32_t sgl_search_unicode_ch_index(const sgl_font_t *font, uint32_t unicode)
{
    uint32_t left = 0;
    uint32_t right = font->unicode_list_len - 1, mid = 0;

    while (left <= right) {
        mid = left + (right - left) / 2;

        if (font->unicode_list[mid] == unicode) {
            return mid;
        }
        else if (font->unicode_list[mid] < unicode) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
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
int32_t sgl_font_get_string_width(const char *str, const sgl_font_t *font)
{
    SGL_ASSERT(font != NULL);
    int32_t len = 0;
#if CONFIG_SGL_TEXT_UTF8
    uint32_t unicode = 0;
    uint32_t ch_index = 0;
    while (*str) {
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        len += font->table[ch_index].box_w;
    }
#else
    while (*str) {
        len += font->table[(uint8_t)(*str) - 32].box_w;
        str++;
    }
#endif
    return len;
}


/**
 * @brief get the height of a string, which is in a rect area
 * @param rect object rect, it is usually the parent of text 
 * @param str string
 * @param font sgl font of the string
 * @param line_space peer line space 
 * @param margin margin of left and right
 * @return height size of string
 */
int32_t sgl_font_get_string_height(sgl_area_t *rect, const char *str, const sgl_font_t *font, uint8_t line_space, int16_t margin)
{
    int16_t offset_x = margin;
    int16_t width = rect->x2 - rect->x1 + 1;
    int16_t ch_index;
    int16_t ch_width;
    int16_t lines = 1;

    #if CONFIG_SGL_TEXT_UTF8
    uint32_t unicode = 0;
    #endif

    while (*str) {
        #if CONFIG_SGL_TEXT_UTF8
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        #else
        ch_index = ((uint32_t)*str) - 32;
        str++;
        #endif

        ch_width = font->table[ch_index].box_w;

        if ((offset_x + ch_width + margin) >= width) {
            offset_x = margin;
            lines ++;
        }

        offset_x += ch_width;
    }

    return lines * (font->font_height + line_space);
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
    switch (type) {
        case SGL_ALIGN_CENTER:
            ret.x = (parent_size->w - size->w) / 2;
            ret.y = (parent_size->h - size->h) / 2;
        break;

        case SGL_ALIGN_TOP_MID:          
            ret.x = (parent_size->w - size->w) / 2;
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
            ret.x = (parent_size->w - size->w) / 2;
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
            ret.y = (parent_size->h - size->h) / 2;
        break;

        case SGL_ALIGN_RIGHT_MID:
            ret.x = parent_size->w - size->w;
            ret.y = (parent_size->h - size->h) / 2;
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
sgl_pos_t sgl_get_text_pos(sgl_area_t *area, const sgl_font_t *font, const char *text, int16_t offset, sgl_align_type_t type)
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
sgl_pos_t sgl_get_icon_pos(sgl_area_t *area, const sgl_icon_pixmap_t *icon, int16_t offset, sgl_align_type_t type)
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
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_CENTER    : Center the object in the parent object.
 *       - SGL_ALIGN_TOP_MID   : Align the object at the top middle of the parent object.
 *       - SGL_ALIGN_TOP_LEFT  : Align the object at the top left of the parent object.
 *       - SGL_ALIGN_TOP_RIGHT : Align the object at the top right of the parent object.
 *       - SGL_ALIGN_BOT_MID   : Align the object at the bottom middle of the parent object.
 *       - SGL_ALIGN_BOT_LEFT  : Align the object at the bottom left of the parent object.
 *       - SGL_ALIGN_BOT_RIGHT : Align the object at the bottom right of the parent object.
 *       - SGL_ALIGN_LEFT_MID  : Align the object at the left middle of the parent object.
 *       - SGL_ALIGN_RIGHT_MID : Align the object at the right middle of the parent object.
 */
void sgl_obj_set_pos_align(sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(obj != NULL);

    sgl_size_t p_size   = {0};
    sgl_pos_t  p_pos    = {0};
    sgl_pos_t  obj_pos  = {0};
    sgl_size_t obj_size = {
        .w = obj->coords.x2 - obj->coords.x1 + 1,
        .h = obj->coords.y2 - obj->coords.y1 + 1,
    };

    p_size = (sgl_size_t){
        .w = obj->parent->coords.x2 - obj->parent->coords.x1 + 1,
        .h = obj->parent->coords.y2 - obj->parent->coords.y1 + 1,
    };
    p_pos = (sgl_pos_t){
        .x = obj->parent->coords.x1,
        .y = obj->parent->coords.y1,
    };

    obj_pos = sgl_get_align_pos(&p_size, &obj_size, type);

    sgl_obj_set_pos(obj, p_pos.x + obj_pos.x,
                         p_pos.y + obj_pos.y
                    );
}


/**
 * @brief Set the alignment position of the object relative to sibling object.
 * @param ref The reference object, it should be the sibling object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_VERT_LEFT  : Align the object at the left side of the reference object.
 *       - SGL_ALIGN_VERT_RIGHT : Align the object at the right side of the reference object.
 *       - SGL_ALIGN_VERT_MID   : Align the object at the middle of the reference object.
 *       - SGL_ALIGN_HORIZ_TOP  : Align the object at the top side of the reference object.
 *       - SGL_ALIGN_HORIZ_BOT  : Align the object at the bottom side of the reference object.
 *       - SGL_ALIGN_HORIZ_MID  : Align the object at the middle of the reference object.
 */
void sgl_obj_set_pos_align_ref(sgl_obj_t *ref, sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(ref != NULL && obj != NULL);

    if (unlikely(ref == obj->parent)) {
        sgl_obj_set_pos_align(obj, type);
        return;
    }

    int16_t ref_w = ref->coords.x2 - ref->coords.x1 + 1;
    int16_t obj_w = obj->coords.x2 - obj->coords.x1 + 1;
    int16_t ref_h = ref->coords.y2 - ref->coords.y1 + 1;
    int16_t obj_h = obj->coords.y2 - obj->coords.y1 + 1;

    switch (type) {
    case SGL_ALIGN_VERT_MID:
        obj->coords.x1 = ref->coords.x1 + (ref_w - obj_w) / 2;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
        break;

    case SGL_ALIGN_VERT_LEFT:
        obj->coords.x1 = ref->coords.x1;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
        break;

    case SGL_ALIGN_VERT_RIGHT:
        obj->coords.x1 = ref->coords.x2 - obj_w;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
        break;
    
    case SGL_ALIGN_HORIZ_MID:
        obj->coords.y1 = ref->coords.y1 + (ref_h - obj_h) / 2;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
        break;

    case SGL_ALIGN_HORIZ_TOP:
        obj->coords.y1 = ref->coords.y1;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
        break;
    
    case SGL_ALIGN_HORIZ_BOT:
        obj->coords.y1 = ref->coords.y2 - obj_h;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
        break;

    default: 
        SGL_LOG_WARN("invalid align type");
        break;
    }
}


/**
 * @brief draw object slice completely
 * @param obj it should point to active root object
 * @param surf surface that draw to
 * @param dirty_h dirty height
 * @return none
 */
static inline void draw_obj_slice(sgl_obj_t *obj, sgl_surf_t *surf, int16_t dirty_h)
{
    int top = 0;
	sgl_event_t evt;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];

	SGL_ASSERT(obj != NULL);
	stack[top++] = obj;

	while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (sgl_surf_area_is_overlap(surf, &obj->area)) {
			evt.type = SGL_EVENT_DRAW_MAIN;
			SGL_ASSERT(obj->construct_fn != NULL);
			obj->construct_fn(surf, obj, &evt);

            if (obj->child != NULL) {
                stack[top++] = obj->child;
            }
		}
	}

    /* flush dirty area into screen */
    sgl_panel_flush_area(surf->x, surf->y, surf->w, dirty_h, surf->buffer);
}


/**
 * @brief calculate dirty area by for each all object that is dirty and visible
 * @param obj it should point to active root object
 * @return true if there is dirty area, otherwise false
 * @note if there is no dirty area, the dirty area will remain unchanged
 */
static inline bool sgl_dirty_area_calculate(sgl_obj_t *obj)
{
    bool need_draw = false;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    /* for each all object from the first task of page */
	while (top > 0) {
        SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        /* if sibling exists, push it to stack, it will be pop in next loop */
		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

        /* if object is hidden, skip it */
        if (unlikely(sgl_obj_is_hidden(obj))) {
            continue;
        }

        /* check if obj is destroyed */
        if (unlikely(sgl_obj_is_destroyed(obj))) {
            /* merge destroy area */
            sgl_obj_dirty_merge(obj);

            /* if the object is active, do not remove it */
            if (unlikely(obj == sgl_screen_act())) {
                obj->destroyed = 0;
                sgl_obj_node_init(obj);
                return false;
            }

            /* update parent layout */
            sgl_obj_set_layout(obj->parent, (sgl_layout_type_t)obj->parent->layout);

            /* remove obj from parent */
            sgl_obj_remove(obj);

            /* free obj resource */
            sgl_obj_free(obj);

            need_draw = true;
            /* object is destroyed, skip */
            continue;
        }

        /* check child need init coords */
        if (unlikely(sgl_obj_is_needinit(obj))) {
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
        if (sgl_obj_is_dirty(obj)) {
            /* update obj area */
            if (unlikely(!sgl_area_clip(&obj->parent->area, &obj->coords, &obj->area))) {
                sgl_obj_set_invalid(obj);
                continue;
            }
            else {
                sgl_obj_set_valid(obj);
            }

            /* merge dirty area */
            sgl_obj_dirty_merge(obj);

            need_draw = true;

            /* clear dirty flag */
            sgl_obj_clear_dirty(obj);
        }

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }

    return need_draw;
}


/**
 * @brief sgl to draw complete frame
 * @param dirty the dirty area that need to upate
 * @return none
 * @note this function should be called in deamon thread or cyclic thread
 */
static inline void sgl_draw_task(sgl_area_t *dirty)
{
    sgl_surf_t *surf = &sgl_ctx.page->surf;
    sgl_obj_t *head = &sgl_ctx.page->obj;

    /* fix button press increase area */
    dirty->y1 = sgl_max(dirty->y1 - 2, 0);
    dirty->y2 = sgl_min(dirty->y2 + 3, head->area.y2);

    /* to set start x and y position for dirty area */
    surf->y = dirty->y1;
    surf->x = sgl_max(dirty->x1 - 2, 0);
    surf->w = sgl_min(dirty->x2 - dirty->x1 + 5, head->area.x2 - surf->x + 1);
    surf->h = surf->size / surf->w;

    SGL_LOG_TRACE("sgl_draw_task: dirty area: x: %d, w: %d, h: %d, w: %d", dirty->x1, dirty->y1, surf->w, dirty->y2 - dirty->y1 + 1);

    while (surf->y <= dirty->y2) {
        /* cycle draw widget slice until the end of dirty area */
        draw_obj_slice(head, surf, sgl_min(dirty->y2 - surf->y + 1, surf->h));
        surf->y += surf->h;

        /* swap buffer for dma operation, but it depends on double buffer */
#if (CONFIG_SGL_DRAW_USE_DMA)
        sgl_surf_buffer_swap(surf);
#endif
    }
}


/**
 * @brief sgl task handle function
 * @param none
 * @return none
 * @note this function should be called in main loop or timer or thread
 */
void sgl_task_handle(void)
{
    /* event task */
    sgl_event_task();

#if (CONFIG_SGL_ANIMATION)
    sgl_anim_task();
#endif // !CONFIG_SGL_ANIMATION

    /* calculate dirty area, if no dirty area, return directly */
    if (! sgl_dirty_area_calculate(&sgl_ctx.page->obj)) {
        return;
    }

    /* draw task  */
#if (CONFIG_SGL_DIRTY_AREA_THRESHOLD)
    for (int i = 0; i < sgl_ctx.dirty_num; i++) {
        sgl_draw_task(&sgl_ctx.dirty[i]);
    }
#else
    sgl_draw_task(&sgl_ctx.dirty);
#endif
    sgl_dirty_area_init();
}
