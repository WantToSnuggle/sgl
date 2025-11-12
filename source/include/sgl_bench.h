/* source/include/sgl_bench.h
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

#ifndef __SGL_BENCH_H__
#define __SGL_BENCH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sgl.h>

#define CONFIG_SGL_BENCH_PER_TIMES   5
#define CONFIG_SGL_BENCH 1
#if (CONFIG_SGL_BENCH)

typedef struct sgl_bench {
    struct sgl_bench *next;
    const char       *name;
    void             (*init)(void);
    void             (*func)(void);
    void             (*deinit)(void);
    uint32_t          frame;
} sgl_bench_t;


extern sgl_bench_t sgl_bench_ctx;


#endif // ! CONFIG_SGL_BENCH

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // ! __SGL_BENCH_H__
