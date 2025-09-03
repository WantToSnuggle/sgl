/* source/lm_array.h
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

#ifndef __LM_ARRAY_H__
#define __LM_ARRAY_H__

#include "lm_list.h"
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct lm_array_node {
    lm_list_node_t node;
    char *string;

}lm_array_node_t;


typedef struct lm_array {
    lm_list_node_t head;
    int count;

}lm_array_t;


#ifdef __cplusplus
extern "C" {
#endif

int lm_array_add(lm_array_t *array, char *str);
int lm_array_delete(lm_array_t *array);
void lm_array_print(FILE* output, lm_array_t *array);
void lm_array_print_with_max_len(FILE* output, lm_array_t *array, int max_len);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!__LM_ARRAY_H__

