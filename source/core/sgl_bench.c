/* source/include/sgl_bench.c
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

#include <sgl_bench.h>

#if (CONFIG_SGL_BENCH)

struct sgl_bench_ctx {
    sgl_bench_t *list_head;
    sgl_bench_t *list_tail;
    uint32_t     count;
    uint8_t      frame[64];
};


static struct sgl_bench_ctx sgl_bench_ctx = {
    .list_head = NULL,
    .list_tail = NULL,
    .count     = 0,
    .frame     = {0},
};


void sgl_bench_add(sgl_bench_t *bench)
{
    if (sgl_bench_ctx.list_tail != NULL) {
        sgl_bench_ctx.list_tail->next = bench;
        sgl_bench_ctx.list_tail = bench;
    }
    else {
        sgl_bench_ctx.list_head = bench;
        sgl_bench_ctx.list_tail = bench;
    }

    sgl_bench_ctx.count++;
}


void sgl_bench_remove(sgl_bench_t *bench)
{
    SGL_ASSERT(bench != NULL);
    sgl_bench_t *prev = NULL;

    if (sgl_bench_ctx.list_head == bench) {
        sgl_bench_ctx.list_head = bench->next;
        if (sgl_bench_ctx.list_head == NULL) {
            sgl_bench_ctx.list_tail = NULL;
        }
        sgl_bench_ctx.count--;
        return;
    }

    prev = sgl_bench_ctx.list_head;
    while (prev != NULL && prev->next != bench) {
        prev = prev->next;
    }

    if (prev == NULL) {
        return;
    }
    prev->next = bench->next;

    if (bench == sgl_bench_ctx.list_tail) {
        sgl_bench_ctx.list_tail = prev;
    }

    sgl_bench_ctx.count--;
}


void sgl_bench_run(void)
{
    sgl_bench_t *bench = sgl_bench_ctx.list_head;
    uint32_t loop = CONFIG_SGL_BENCH_PER_TIMES;
    sgl_obj_t *frame_label = 
    sgl_obj_t *memory_label = 

    while (bench != NULL) {
        bench->init();
    
        while (loop--) {
            bench->func();

            sgl_task_handle();
        }

        bench->deinit();
        bench = bench->next;
    }
}


#endif //!CONFIG_SGL_BENCH
