/* source/lm_mem.c
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

#include "lm_mem.h"
#include <stdio.h>
#include <stdlib.h>


static void* lm_mem_pool = NULL;
static tlsf_t tlsf_pool;


int lm_mem_init(int size_mb)
{
    lm_mem_pool = malloc(size_mb*1024*1024);
    if(lm_mem_pool == NULL) {
        return -1;
    }

    tlsf_pool = tlsf_create_with_pool(lm_mem_pool, size_mb*1024*1024);

    return 0;
}


void* lm_malloc(size_t size)
{
    return tlsf_malloc(lm_mem_pool, size);
}


void lm_free(void *p)
{
    tlsf_free(lm_mem_pool, p);
}


void lm_mem_destroy(void)
{
    tlsf_destroy(tlsf_pool);
    free(lm_mem_pool);
}
