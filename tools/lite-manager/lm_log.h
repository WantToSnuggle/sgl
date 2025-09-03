/* source/lm_log.h
 *
 * MIT License
 *
 * Copyright(c) 2023-present @ li shanwen
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

#ifndef __LM_LOG_H__
#define __LM_LOG_H__


#include "config.h"


#ifdef __cplusplus
extern "C" {
#endif


#define LM_LOG_LEVEL_TRACE 0 /**< A lot of logs to give detailed information*/
#define LM_LOG_LEVEL_INFO  1 /**< Log important events*/
#define LM_LOG_LEVEL_WARN  2 /**< Log if something unwanted happened but didn't caused problem*/
#define LM_LOG_LEVEL_ERROR 3 /**< Only critical issue, when the system may fail*/
#define LM_LOG_LEVEL_USER  4 /**< Custom logs from the user*/
#define LM_LOG_LEVEL_NONE  5 /**< Do not log anything*/
#define _LM_LOG_LEVEL_NUM  6 /**< Number of log levels*/


#if CONFIG_DEBUG

/**
 * @brief LM log printing function, used to print debugging information. Note that this function 
 *        should only be called in debugging mode, otherwise it may affect system real-time 
 *        performance due to long execution time
 * 
 * @param level:  log level, such as, INFO, USER...
 * @param format:  log content
 * 
 * @return none
*/
void lm_log(const char *level, const char * format, ...);


#define LM_LOG_TRACE_FLAG              "\x1b[34m[TRACE]\x1b[0m "
#define LM_LOG_INFO_FLAG               "\x1b[32m[INFO]\x1b[0m "
#define LM_LOG_WARN_FLAG               "\x1b[33m[WARN]\x1b[0m "
#define LM_LOG_ERROR_FLAG              "\x1b[31m[ERROR]\x1b[0m "
#define LM_LOG_USER_FLAG               "\x1b[36m[USER]\x1b[0m "
#define LM_ASSERT_FLAG                 "\x1b[31m[ASSERT]\x1b[0m "


#ifndef LM_LOG_TRACE
#    if CONFIG_LOG_LEVEL <= LM_LOG_LEVEL_TRACE
#        define LM_LOG_TRACE(...) lm_log(LM_LOG_TRACE_FLAG, __VA_ARGS__)
#    else
#        define LM_LOG_TRACE(...) do {}while(0)
#    endif
#endif

#ifndef LM_LOG_INFO
#    if CONFIG_LOG_LEVEL <= LM_LOG_LEVEL_INFO
#        define LM_LOG_INFO(...) lm_log(LM_LOG_INFO_FLAG,  __VA_ARGS__)
#    else
#        define LM_LOG_INFO(...) do {}while(0)
#    endif
#endif

#ifndef LM_LOG_WARN
#    if CONFIG_LOG_LEVEL <= LM_LOG_LEVEL_WARN
#        define LM_LOG_WARN(...) lm_log(LM_LOG_WARN_FLAG, __VA_ARGS__)
#    else
#        define LM_LOG_WARN(...) do {}while(0)
#    endif
#endif

#ifndef LM_LOG_ERROR
#    if CONFIG_LOG_LEVEL <= LM_LOG_LEVEL_ERROR
#        define LM_LOG_ERROR(...) lm_log(LM_LOG_ERROR_FLAG, __VA_ARGS__)
#    else
#        define LM_LOG_ERROR(...) do {}while(0)
#    endif
#endif

#ifndef LM_LOG_USER
#    if CONFIG_LOG_LEVEL <= LM_LOG_LEVEL_USER
#        define LM_LOG_USER(...) lm_log(LM_LOG_USER_FLAG,  __VA_ARGS__)
#    else
#        define LM_LOG_USER(...) do {}while(0)
#    endif
#endif

#if CONFIG_LOG_LEVEL < LM_LOG_LEVEL_NONE
#    define LM_LOG(...) lm_log(__VA_ARGS__)
#else
#    define LM_LOG(...) do {} while(0)
#endif


/**
 * @brief LM assert handler, used to handle assertions
 * 
 * @param file:  file name
 * @param func:  function name
 * @param line:  line number
 * 
 * @return none
*/
void lm_assert_handler(const char *file, const char *func, int line);

#define LM_ASSERT(condition)        \
        do {                         \
            if(!(condition)) {       \
                lm_assert_handler(__FILE__, __FUNCTION__, __LINE__); \
            }                        \
        } while(0)


#else // no define debug mode macro

#define LM_LOG_TRACE(...) do {}while(0)
#define LM_LOG_INFO(...) do {}while(0)
#define LM_LOG_WARN(...) do {}while(0)
#define LM_LOG_ERROR(...) do {}while(0)
#define LM_LOG_USER(...) do {}while(0)
#define LM_LOG(...) do {}while(0)

#define LM_ASSERT(condition) do {}while(0)

#endif //!CONFIG_DEBUG


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //__LM_LOG_H__
