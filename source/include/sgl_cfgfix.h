/* source/include/sgl_cfgfix.h
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

#ifndef __SGL_CFGFIX_H__
#define __SGL_CFGFIX_H__

#include <sgl_config.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief sgl config fix file, it will fix the macro if not defined
 * @note please add the macro in this file if you want to add new macro into source code
 * 
 * @description:
 * 
 * CONFIG_SGL_PANEL_PIXEL_DEPTH:
 *      The pixel depth of panel, it will be used to define the color type
 *
 * CONFIG_SGL_COLOR16_SWAP:
 *      Its for 16 bit color, the color will be swapped
 * 
 * CONFIG_SGL_EVENT_QUEUE_SIZE:
 *      the size of event queue, default: 32
 * 
 * CONFIG_SGL_OBJ_SLOT_DYNAMIC
 *      If the object slot is dynamic, the object slot size will be dynamic allocated, otherwise, the object 
 *      slot size will be static allocated that you should define CONFIG_SGL_OBJ_NUM_MAX macro
 * 
 * CONFIG_SGL_OBJ_NUM_MAX:
 *      If CONFIG_SGL_OBJ_SLOT_DYNAMIC is 0 or not defined, you should define CONFIG_SGL_OBJ_NUM_MAX macro
 * 
 * CONFIG_SGL_DRAW_USE_DMA:
 *      If you want to use DMA to draw, please define this macro to 1
 * 
 * CONFIG_SGL_ANIMATION:
 *      If you want to use animation, please define this macro to 1
 * 
 * CONFIG_SGL_ANIMATION_TICK_MS:
 *      If the CONFIG_SGL_ANIMATION is 1, you should define CONFIG_SGL_ANIMATION_TICK_MS macro, and it means 
 *      the animation tick ms, default: 5
 * 
 * CONFIG_SGL_DEBUG:
 *      If you want to use debug, please define this macro to 1
 * 
 * CONFIG_SGL_TEXT_UTF8:
 *      If you want to use text utf8, please define this macro to 1
 * 
 * CONFIG_SGL_USE_OBJ_ID:
 *      If you want to use obj id, please define this macro to 1, at mostly, the CONFIG_SGL_USE_OBJ_ID should be 0
 * 
 * CONFIG_SGL_USE_STYLE_UNIFIED_API:
 *      If you want to use style unified api, please define this macro to 1, default: 1
 * 
 * CONFIG_SGL_HEAP_ALGO:
 *      The heap algorithm, default: lwmem
 * 
 * CONFIG_SGL_HEAP_MEMORY_SIZE:
 *      The heap memory size, default: 10240
 * 
 * CONFIG_SGL_FONT_SONG23:
 *      If you want to use font song23, please define this macro to 1
 * 
 * CONFIG_SGL_FONT_CONSOLAS23:
 *      If you want to use font consolas23, please define this macro to 1
 * 
 * CONFIG_SGL_FONT_KAI33:
 *      If you want to use font kai33, please define this macro to 1
 * 
 */


#ifndef CONFIG_SGL_PANEL_PIXEL_DEPTH
#define CONFIG_SGL_PANEL_PIXEL_DEPTH               16
#endif

#ifndef CONFIG_SGL_COLOR16_SWAP
#define CONFIG_SGL_COLOR16_SWAP                    0
#endif

#ifndef CONFIG_SGL_EVENT_QUEUE_SIZE
#define CONFIG_SGL_EVENT_QUEUE_SIZE                32
#endif

#ifndef CONFIG_SGL_OBJ_SLOT_DYNAMIC
    #ifndef CONFIG_SGL_OBJ_NUM_MAX
        #define CONFIG_SGL_OBJ_SLOT_DYNAMIC        1
    #else
        #define CONFIG_SGL_OBJ_SLOT_DYNAMIC        0
        #define CONFIG_SGL_OBJ_NUM_MAX             128
    #endif
#endif

#ifndef CONFIG_SGL_DRAW_USE_DMA
#define CONFIG_SGL_DRAW_USE_DMA                    0
#endif

#ifndef CONFIG_SGL_ANIMATION
#define CONFIG_SGL_ANIMATION                       0
#elif(CONFIG_SGL_ANIMATION == 1)
    #ifndef CONFIG_SGL_ANIMATION_TICK_MS
    #define CONFIG_SGL_ANIMATION_TICK_MS           5
    #endif
#endif

#ifndef CONFIG_SGL_DEBUG
    #define CONFIG_SGL_DEBUG                       0
#elif (CONFIG_SGL_DEBUG == 1)
    #ifndef CONFIG_SGL_LOG_COLOR
    #define CONFIG_SGL_LOG_COLOR                   0
    #endif
    #ifndef CONFIG_SGL_LOG_LEVEL
    #define CONFIG_SGL_LOG_LEVEL                   1
    #endif
#endif

#ifndef CONFIG_SGL_TEXT_UTF8
#define CONFIG_SGL_TEXT_UTF8                       0
#endif

#ifndef CONFIG_SGL_USE_OBJ_ID
#define CONFIG_SGL_USE_OBJ_ID                      0
#endif

#ifndef CONFIG_SGL_USE_STYLE_UNIFIED_API
#define CONFIG_SGL_USE_STYLE_UNIFIED_API           1
#endif

#ifndef CONFIG_SGL_HEAP_ALGO
#define CONFIG_SGL_HEAP_ALGO                       lwmem
#endif

#ifndef CONFIG_SGL_HEAP_MEMORY_SIZE
    #define CONFIG_SGL_HEAP_MEMORY_SIZE            10240
    #ifndef CONFIG_SGL_FL_INDEX_MAX
    #define CONFIG_SGL_FL_INDEX_MAX                20
    #endif
#endif

#ifndef CONFIG_SGL_FONT_SONG23
#define CONFIG_SGL_FONT_SONG23                     0
#endif

#ifndef CONFIG_SGL_FONT_CONSOLAS23
#define CONFIG_SGL_FONT_CONSOLAS23                 0
#endif

#ifndef CONFIG_SGL_FONT_KAI33
#define CONFIG_SGL_FONT_KAI33                      0
#endif


#ifdef __cplusplus
}
#endif

#endif // !__SGL_CFGFIX_H__
