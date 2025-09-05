/* source/include/sgl_types.h
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

#ifndef __SGL_TYPES_H__
#define __SGL_TYPES_H__


#include <stdint.h>
#include <stdbool.h>
#include <sgl_config.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Get the offset of internal members of the structure
 * 
 * @struct_t: structure typedef 
 * @member: member in structure
 * 
 * @return offset of member in the structure
*/
#define sgl_offsetof(struct_t, member)          ((size_t)&((struct_t*)0)->member)


/**
 * @brief Get the address of the structure instance.
 *
 * @ptr: address of the structure member.
 * @type: type of the structure.
 * @member: member name of the ptr in structure.
 * @return pointer to address of structure 
 */
#define sgl_container_of(ptr, type, member)     ((type *)((char *)ptr - sgl_offsetof(type, member)))


#ifndef SGL_ARRAY_SIZE
#define SGL_ARRAY_SIZE(array)                   (sizeof(array) / sizeof(array[0]))
#endif


#define SGL_ALIGN_UP(x, align)                  (((x) + ((align) - 1)) & ~((align) - 1))
#define SGL_ROUND_UP(x, round)                  ((((x) + ((round) - 1)) / (round)) * (round))

#define SGL_UNUSED(x)                           (void)(x)


#define SGL_POS_INVALID                         (0xefff)
#define SGL_POS_MAX                             (8192)
#define SGL_POS_MIN                             (-8192)


#define SFL_WIDTH_INVALID                       (-1)
#define SGL_WIDTH_MAX                           (8192)
#define SGL_WIDTH_MIN                           (0)
#define SFL_HEIGHT_INVALID                      (-1)
#define SGL_HEIGHT_MAX                          (8192)
#define SGL_HEIGHT_MIN                          (0)


#define SGL_DIRECT_HORIZONTAL                   (0)
#define SGL_DIRECT_VERTICAL                     (1)

#define SGL_ALPHA_MAX                           (255)
#define SGL_ALPHA_MIN                           (0)


#define SGL_STYLE_OK                            (0)
#define SGL_STYLE_FAILED                        (UINT32_MAX)
#define SGL_STYLE_INVALID                       (UINT32_MAX)


#ifdef __GNUC__   /* gcc compiler      */
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#elif __clang__   /* clang compiler    */
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#elif __CC_ARM    /* RealView compiler */
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#elif __ICCARM__  /* IAR compiler      */
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#elif _MSC_VER    /* MSVC compiler     */
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#elif __MINGW32__ /* MinGW compiler    */
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#else             /* others compiler   */
#define likely(x)                               (x)
#define unlikely(x)                             (x)
#endif


#define  sgl_check_ptr_break(ptr)               if(unlikely((ptr) == NULL)) { SGL_LOG_ERROR("Function: %s, Line: %d, "#ptr" is NULL", __func__, __LINE__); return;}
#define  sgl_check_ptr_return(ptr, r)           if(unlikely((ptr) == NULL)) { SGL_LOG_ERROR("Function: %s, Line: %d, "#ptr" is NULL", __func__, __LINE__); return (r);}


/**
 * @brief  Run once function
 * @note you must sure the statement is thread safe
 */
#define  SGL_RUN_ONCE(statement)                do { \
                                                    static bool _run_once_flag = false;     \
                                                    if (unlikely(_run_once_flag == false)){ \
                                                        statement;                          \
                                                        _run_once_flag = true;              \
                                                    }                                       \
                                                } while(0)


// prototype: sgl_rgb(uint8_t r, uint8_t g, uint8_t b)
#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == 32 || CONFIG_SGL_PANEL_PIXEL_DEPTH == 24)
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.blue    = (b),              \
                                                               .ch.green   = (g),              \
                                                               .ch.red     = (r),}
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 16)
#if CONFIG_SGL_COLOR16_SWAP                   
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.green_h = (g >>2) >> 3,     \
                                                               .ch.red     = (r) >> 3,         \
                                                               .ch.blue    = (b) >> 3,         \
                                                               .ch.green_l = (g >>2) & 0x7,}
#else
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.blue    = (b) >> 3,         \
                                                               .ch.green   = (g) >> 2,         \
                                                               .ch.red     = (r) >> 3,}
#endif
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 8)
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.blue    = (b >> 6),         \
                                                               .ch.green   = (g >> 5),         \
                                                               .ch.red     = (r >> 5),}
#endif


/**
 * Define some basic colors
*/
#define SGL_RED                                 sgl_rgb(0xFF, 0, 0)
#define SGL_GREEN                               sgl_rgb(0, 0xFF, 0)
#define SGL_BLUE                                sgl_rgb(0, 0, 0xFF)
#define SGL_WHITE                               sgl_rgb(0xFF, 0xFF, 0xFF)
#define SGL_BLACK                               sgl_rgb(0, 0, 0)
#define SGL_CYAN                                sgl_rgb(0, 0xFF, 0xFF)
#define SGL_MAGENTA                             sgl_rgb(0xFF, 0, 0xFF)
#define SGL_YELLOW                              sgl_rgb(0xFF, 0xFF, 0)
#define SGL_GRAY                                sgl_rgb(0x80, 0x80, 0x80)
#define SGL_DARK_GRAY                           sgl_rgb(0x40, 0x40, 0x40)
#define SGL_LIGHT_GRAY                          sgl_rgb(0xC0, 0xC0, 0xC0)
#define SGL_BRIGHT_PURPLE                       sgl_rgb(0xFF, 0x00, 0xFF)
#define SGL_BRIGHT_BLUE                         sgl_rgb(0x00, 0xFF, 0xFF)


#ifdef __cplusplus
}
#endif

#endif //__SGL_TYPES_H__
