/* source/lm_log.c
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

#include "stdarg.h"
#include "lm_log.h"
#include <stdio.h>
#include <string.h>


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
void lm_log(const char *level, const char * format, ...)
{
    printf("%s", level);
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    printf("\n");
    va_end(args);
}


/**
 * @brief LM assert handler, used to handle assertions
 * 
 * @param file:  file name
 * @param func:  function name
 * @param line:  line number
 * 
 * @return none
*/
void lm_assert_handler(const char *file, const char *func, int line)
{
    lm_log(LM_ASSERT_FLAG, "file: %s, function: %s, line: %d", file, func, line);
    while(1) {

    };
}

#endif //CONFIG_DEBUG
