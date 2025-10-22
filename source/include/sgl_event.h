/* source/include/sgl_event.h
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

#ifndef __SGL_EVENT_H__
#define __SGL_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <sgl_cfgfix.h>
#include <stddef.h>
#include <sgl_list.h>
#include <sgl_types.h>


/* Forward declaration of sgl_obj and sgl_page*/
struct sgl_obj;
struct sgl_page;


/**
* @brief This enumeration type lists all the event types of sgl
*/
typedef enum sgl_event_type {
    SGL_EVENT_NULL = 0,
    SGL_EVENT_NORMAL,
    SGL_EVENT_PRESSED,
    SGL_EVENT_RELEASED,
    SGL_EVENT_CLICKED,
    SGL_EVENT_MOTION,
    SGL_EVENT_MOVE_UP,
    SGL_EVENT_MOVE_DOWN,
    SGL_EVENT_MOVE_LEFT,
    SGL_EVENT_MOVE_RIGHT,
    SGL_EVENT_LONG_PRESSED,
    SGL_EVENT_OPTION_WALK,
    SGL_EVENT_OPTION_TAP,
    SGL_EVENT_DRAW_INIT,
    SGL_EVENT_DRAW_MAIN,
    SGL_EVENT_ALL = 255,

} sgl_event_type_t;


/**
* @brief Event location structure, Used to represent the coordinates of an event, 
*        such as the click position of the mouse, the click position of the touch screen, 
*        and so on
*
* @x: x coordinate
* @y: y coordinate
* @sgl_event_type_t: event type
*/
typedef struct sgl_event_pos {
    int16_t x;
    int16_t y;
} sgl_event_pos_t;


typedef struct sgl_event {
    struct sgl_obj   *obj;
    size_t           param;
    sgl_event_pos_t  pos;
    uint16_t         type;
    uint16_t         distance;
} sgl_event_t;


/**
 * @brief Initialize the event queue
 * @param none
 * @return 0 on success, -1 on failure
 * @note !!!!!! the SGL_EVENT_QUEUE_SIZE must be power of 2 !!!!!!
 */
int sgl_event_queue_init(void);


/**
 * @brief Push an event into the event queue
 * @param event The event to be pushed
 * @return 0 on success, -1 on failure
 */
void sgl_event_queue_push(sgl_event_t event);


/**
 * @brief Handle the position event
 * @param pos The position to be handled
 * @param type The type of the event
 * @return none
 */
void sgl_event_send_pos(sgl_event_pos_t pos, sgl_event_type_t type);


/**
 * @brief Send an event to the specified object
 * @param event The event to be sent
 * @return none
 */
static inline void sgl_event_send(sgl_event_t event)
{
    sgl_event_queue_push(event);
}


/**
 * @brief All event task in SGL, this function will traverse all elements in the event queue, 
 *        respond to each element with an event, so that all events will trigger and point to the 
 *        corresponding callback function
 * @param none
 * @return none
*/
void sgl_event_task(void);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!__SGL_EVENT_H__
