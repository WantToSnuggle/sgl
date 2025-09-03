/* source/lm_macro.h
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

#ifndef __LM_MACRO_H__
#define __LM_MACRO_H__


#include <stdio.h>
#include "lm_array.h"
#include "lm_list.h"
#include "config.h"


enum lm_macro_type {
    LM_MACRO_STRING = 0,
    LM_MACRO_NUMBER = 1,
};


struct lm_macro_range {
    double min;
    double max;
};


typedef struct lm_macro lm_macro_t;

struct lm_macro_cache_node {
    lm_macro_t *macro;
    uint64_t   count;
};


typedef struct lm_macro_head {
    struct lm_macro_cache_node cache[CONFIG_MACRO_CACHE_SIZE];
    lm_list_node_t  node;
    int            count;
}lm_macro_head_t;


struct lm_macro {
    lm_list_node_t  node;
    char           *name;
    enum lm_macro_type type;
    lm_array_t   choice;
    struct lm_macro_range range;
    char           *value;
    char           *depend;

    union {
        char       *def_str;
        double     def_num;
    };
    int            def_flag;
};


#ifdef __cplusplus
extern "C" {
#endif


void lm_macro_list_cache_init(lm_macro_head_t *head);
void lm_macro_list_add(lm_macro_head_t *head, lm_macro_t *macro);
lm_macro_t* lm_macro_new(char* name);
lm_macro_t* lm_macro_new_and_add(lm_macro_head_t *head, char* name);
int lm_macro_value_set(lm_macro_t* macro, char *value);
void lm_macro_type_set(lm_macro_t* macro, enum lm_macro_type type);
enum lm_macro_type lm_macro_type_get(lm_macro_t* macro);
void lm_macro_range_set(lm_macro_t* macro, double min, double max);
void lm_macro_choice_set_count(lm_macro_t* macro, int count);
int lm_macro_choice_append(lm_macro_t* macro, char* str);
char *lm_macro_choice_get_first(lm_macro_t* macro);
int lm_macro_depend_set(lm_macro_t* macro, char* str);
int lm_macro_default_set(lm_macro_t* macro, char* str);
int lm_macro_delete(lm_macro_head_t *head, lm_macro_t *macro);
lm_macro_t *lm_macro_search_by_name(lm_macro_head_t *head, char *name);
bool lm_macro_value_is_valid(lm_macro_t *macro, char *value);
void lm_macro_print_all(FILE* output, lm_macro_head_t *head);

#if (_WIN32)
void lm_macro_print_all_value(lm_macro_head_t *head);
#elif __linux__
void lm_macro_print_all_value(lm_macro_head_t *head);
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif //! __LM_MACRO_H__