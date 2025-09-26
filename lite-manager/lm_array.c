/* source/lm_array.c
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

#include "lm_array.h"
#include "lm_error.h"
#include "lm_string.h"
#include "lm_mem.h"


int lm_array_add(lm_array_t *array, char *str)
{
    lm_array_node_t *node = lm_malloc(sizeof(lm_array_node_t));
    if(node == NULL) {
        return LM_ERR;
    }

    node->string = NULL;

    if(str == NULL) {
        return 0;
    }

    int ret = lm_str_dupli_string(&node->string, str);
    if(ret) {
        lm_free(node);
        return LM_ERR;
    }

    lm_list_add_node_at_tail(&array->head, &node->node);
    array->count ++;

    return 0;
}


int lm_array_delete(lm_array_t *array)
{
    if(array->count == 0) {
        return LM_OK;
    }

    lm_list_node_t *node = lm_list_next_node(&array->head);

    for(int i = 0; i < array->count; i++) {
        
        lm_array_node_t *array_node = container_of(node, lm_array_node_t, node);
        lm_free(array_node->string);
        lm_free(array_node);

        node = lm_list_next_node(node);
    }
    return LM_OK;
}


void lm_array_print(FILE* output, lm_array_t *array)
{
    lm_list_node_t *node = lm_list_next_node(&array->head);
    lm_array_node_t *array_node = NULL;

    for(int i = 0; i < array->count - 1; i++) {
        
        array_node = container_of(node, lm_array_node_t, node);

        if(array_node != LM_ERR_PTR) {
            fprintf(output, "[%s], ", array_node->string);
        }

        node = lm_list_next_node(node);
    }
    
    array_node = container_of(node, lm_array_node_t, node);
    if(array_node != LM_ERR_PTR) {
        fprintf(output, "[%s]", array_node->string);
    }
}



void lm_array_print_with_max_len(FILE* output, lm_array_t *array, int max_len)
{
    lm_list_node_t *node = lm_list_next_node(&array->head);
    lm_array_node_t *array_node = NULL;
    int line_count = 0;

    for(int i = 0; i < array->count - 1; i++) {
        
        array_node = container_of(node, lm_array_node_t, node);

        if(array_node != LM_ERR_PTR) {
            fprintf(output, "%s ", array_node->string);
        }

        line_count ++;

        if(line_count == max_len) {
            fprintf(output, "\\\n            ");
        }

        node = lm_list_next_node(node);
    }
    
    array_node = container_of(node, lm_array_node_t, node);
    if(array_node != LM_ERR_PTR) {
        fprintf(output, "%s", array_node->string);
    }

    fprintf(output, "\n");
}