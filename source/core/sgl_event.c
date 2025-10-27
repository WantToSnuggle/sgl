/* source/core/sgl_event.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Email: 1477153217@qq.com
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

#include <sgl_event.h>
#include <sgl_math.h>
#include <sgl_cfgfix.h>
#include <sgl_log.h>
#include <string.h>
#include <sgl_mm.h>


/* define event queue size */
#define SGL_EVENT_QUEUE_SIZE  CONFIG_SGL_EVENT_QUEUE_SIZE


/**
 * @brief event lost object
 * @note this object is used to recode event lost object
 */
static struct sgl_obj *event_lost = NULL;

static sgl_event_pos_t touch_atc_pos[2] = {
    {-1, -1}, {-1, -1},
};


static struct event_queue {
    sgl_event_t* buffer;
    uint16_t capacity;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
} evtq;


/* define event queue buffer  */
static sgl_event_t evtq_buffer[SGL_EVENT_QUEUE_SIZE] = {0};


/**
 * @brief Initialize the event queue
 * @param none
 * @return 0 on success, -1 on failure
 * @note !!!!!! the SGL_EVENT_QUEUE_SIZE must be power of 2 !!!!!!
 */
int sgl_event_queue_init(void)
{
    if (!sgl_is_pow2(SGL_EVENT_QUEUE_SIZE)) {
        SGL_LOG_ERROR("The capacity must be power of 2");
        return -1;
    }

    evtq.buffer = evtq_buffer;
    evtq.capacity = SGL_EVENT_QUEUE_SIZE;
    evtq.head = evtq.tail = evtq.size = 0;

    return 0;
}


/**
 * @brief Check whether the event queue is empty
 * @param none
 * @return true if the event queue is empty, false otherwise
 */
static inline bool sgl_event_queue_is_empty(void)
{
    return evtq.size == 0;
}


/**
 * @brief Push an event into the event queue
 * @param event The event to be pushed
 * @return 0 on success, -1 on failure
 */
void sgl_event_queue_push(sgl_event_t event)
{
    if (unlikely( evtq.size == evtq.capacity )) {
        SGL_LOG_ERROR("Event queue is full, maybe system is too slow");
        evtq.head = evtq.tail = evtq.size = 0;
    }

    evtq.buffer[evtq.tail] = event;
    evtq.tail = ((evtq.tail + 1) & (evtq.capacity - 1));
    evtq.size++;
}


/**
 * @brief Pop an event from the event queue
 * @param out_event The event to be popped
 * @return 0 on success, -1 on failure
 */
static inline int sgl_event_queue_pop(sgl_event_t* out_event)
{
    if (sgl_event_queue_is_empty()) {
        return -1;
    }

    *out_event = evtq.buffer[evtq.head];
    evtq.head = ((evtq.head + 1) & (evtq.capacity - 1));
    evtq.size--;

    return 0;
}


/**
 * @brief Check whether the position is focus on the object
 * @param pos The position to be checked
 * @param rect The rectangle of the object
 * @param radius The radius of the object
 * @return true if the position is focus on the object, false otherwise
 */
static bool pos_is_focus_on_obj(sgl_event_pos_t *pos, sgl_rect_t *rect, int16_t radius)
{
    if (pos->x < rect->x1 || pos->x > rect->x2 || pos->y < rect->y1 || pos->y > rect->y2) {
        return false;
    }
    else if(radius == 0) {
        return true;
    }

    if ((pos->x >= rect->x1 + radius) && (pos->x <= rect->x2 - radius)) {
        return true;
    }
    else if ((pos->y >= rect->y1 + radius) && (pos->y <= rect->y2 - radius)) {
        return true;
    }

    if (pos->x <= rect->x1 + radius && pos->y <= rect->y1 + radius) {
        int16_t dx = pos->x - (rect->x1 + radius);
        int16_t dy = pos->y - (rect->y1 + radius);
        return sgl_pow2(dx) + sgl_pow2(dy) <= sgl_pow2(radius);
    }
    else if (pos->x >= rect->x2 - radius && pos->y <= rect->y1 + radius) {
        int16_t dx = pos->x - (rect->x2 - radius);
        int16_t dy = pos->y - (rect->y1 + radius);
        return sgl_pow2(dx) + sgl_pow2(dy) <= sgl_pow2(radius);
    }
    else if (pos->x <= rect->x1 + radius && pos->y >= rect->y2 - radius) {
        int16_t dx = pos->x - (rect->x1 + radius);
        int16_t dy = pos->y - (rect->y2 - radius);
        return sgl_pow2(dx) + sgl_pow2(dy) <= sgl_pow2(radius);
    }
    else if (pos->x >= rect->x2 - radius && pos->y >= rect->y2 - radius) {
        int16_t dx = pos->x - (rect->x2 - radius);
        int16_t dy = pos->y - (rect->y2 - radius);
        return sgl_pow2(dx) + sgl_pow2(dy) <= sgl_pow2(radius);
    }

    return false;
}


/**
 * @brief check whether the position is clicked on the object
 * @param pos The position to be clicked
 * @return The object that is clicked on, NULL if no object is clicked
 */
static struct sgl_obj* click_detect_object(sgl_event_pos_t *pos)
{
    struct sgl_obj *stack[SGL_OBJ_DEPTH_MAX], *obj = sgl_screen_act()->child, *find = NULL;
    int top = 0;

    if (unlikely(obj == NULL)) {
        return NULL;
    }
    stack[top++] = obj;

    while (top > 0) {
        SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        if (sgl_obj_has_sibling(obj)) {
            stack[top++] = obj->sibling;
        }

        if (pos_is_focus_on_obj(pos, &obj->coords, obj->radius)) {
            find = obj;
            if (sgl_obj_has_child(obj)) {
                stack[top++] = obj->child;
            }
        }
    }

    /**
     * if the object is clickable, return it, otherwise return its parent 
     * because the object may be a label attached to the object
    */
    if (find != NULL) {
        return sgl_obj_is_clickable(find) ? find : (sgl_obj_is_clickable(find->parent) ? find->parent : NULL);
    }

    return find;
}


/**
 * @brief check whether the position is motion on the object
 * @param pos The position to be motion
 * @return The object that is motion on, NULL if no object is motion
 */
static struct sgl_obj* motion_detect_object(sgl_event_pos_t *pos)
{
    struct sgl_obj *stack[SGL_OBJ_DEPTH_MAX], *obj = NULL;
    int top = 0;
    stack[top++] = sgl_screen_act();

    while (top > 0) {
        SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        if (sgl_obj_has_sibling(obj)) {
            stack[top++] = obj->sibling;
        }

        if (pos_is_focus_on_obj(pos, &obj->coords, obj->radius)) {
            if (sgl_obj_is_movable(obj)) {
                return obj;
            }
            else if (sgl_obj_has_child(obj)) {
                stack[top++] = obj->child;
            }
        }
    }

    return NULL;
}


/**
 * @brief Handle the position event
 * @param pos The position to be handled
 * @param type The type of the event
 * @return none
 */
void sgl_event_send_pos(sgl_event_pos_t pos, sgl_event_type_t type)
{
    sgl_event_t event = {
        .obj = NULL,
        .type = type,
        .pos = pos,
    };

    if (type == SGL_EVENT_PRESSED) {
        touch_atc_pos[1] = pos;
    }
    else if (type == SGL_EVENT_MOTION) {
        touch_atc_pos[0] = touch_atc_pos[1];
        touch_atc_pos[1] = pos;
    }

    sgl_event_queue_push(event);
}


/**
 * @brief get information of motion event type
 * @param evt [in][out] event to be handled
 * @return none
 */
static void sgl_get_move_info(sgl_event_t *evt)
{
    int16_t dx = touch_atc_pos[1].x - touch_atc_pos[0].x;
    int16_t dy = touch_atc_pos[1].y - touch_atc_pos[0].y;

    if (sgl_abs(dx) > sgl_abs(dy)) {
        if (dx > 0) {
            evt->type = SGL_EVENT_MOVE_RIGHT;
            evt->distance = dx;
        }
        else {
            evt->type = SGL_EVENT_MOVE_LEFT;
            evt->distance = -dx;
        }
    }
    else {
        if (dy > 0) {
            evt->type = SGL_EVENT_MOVE_DOWN;
            evt->distance = dy;
        }
        else {
            evt->type = SGL_EVENT_MOVE_UP;
            evt->distance = -dy;
        }
    }
}


/**
 * @brief All event task in SGL, this function will traverse all elements in the event queue, 
 *        respond to each element with an event, so that all events will trigger and point to the 
 *        corresponding callback function
 * @param none
 * @return none
*/
void sgl_event_task(void)
{
    sgl_event_t evt;
    struct sgl_obj *obj = NULL;

    /* get event from event queue */
    while (sgl_event_queue_pop(&evt) == 0) {

        if (evt.obj == NULL) {
            /* if event type is not motion, use pos to detect object */
            if (evt.type != SGL_EVENT_MOTION) {
                obj = click_detect_object(&evt.pos);
            }
            else {
                obj = motion_detect_object(&evt.pos);
                sgl_get_move_info(&evt);
            }
        }
        else {
            /* if obj is not NULL, means obj is set by user */
            obj = evt.obj;
        }

        if (obj) {
            /* set obj to event */
            evt.obj = obj;

            /* if type is PRESSED, means obj is clicked */
            if (evt.type == SGL_EVENT_PRESSED) {
                if (obj->pressed == false) {
                    /* set obj pressed to true */
                    obj->pressed = true;
                    /* update event lost object */
                    event_lost = obj;
                }
                else {
                    /* if obj is clicked, skip this event */
                    continue;
                }
            }
            else if (evt.type == SGL_EVENT_RELEASED) {
                /* if obj is pressed, set obj pressed to false */
                if (obj->pressed) {
                    obj->pressed = false;
                    /* clear event lost */
                    event_lost = NULL;
                }
                else {
                    /* if event lost is not current, means error occurred, push the event into queue again */
                    if (event_lost && event_lost != obj) {
                        evt.obj = event_lost;
                        sgl_event_queue_push(evt);
                    }
                    continue;
                }
            }
            else if (evt.type == SGL_EVENT_MOTION && (!sgl_obj_is_movable(obj))) {
                continue;
            }

            /* call the event function */
            if (obj->construct_fn) {
                sgl_obj_set_dirty(obj);
                evt.param = obj->event_data;
                obj->construct_fn(NULL, obj, &evt);
            }
        }
        else {
            SGL_LOG_TRACE("pos is out of object, skip event");
            /* if the event is released, check if the event is lost */
            if (evt.type == SGL_EVENT_RELEASED && event_lost != obj) {
                /* if the event is lost, set the event to the lost object and push it to the event queue again */
                evt.obj = event_lost;
                sgl_event_queue_push(evt);
            }
        }
    }
}
