/* source/lm_macro.c
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

#include <string.h>
#include <stdlib.h>
#include "lm_macro.h"
#include "lm_string.h"
#include "lm_error.h"
#include "lm_mem.h"
#include "lm_log.h"


#if (_WIN32)
#include <windows.h>
#elif ( __linux__)
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#define ESC "\x1b"
#define CSI "\x1b["


void lm_macro_list_cache_init(lm_macro_head_t *head)
{
    for(int i = 0; i < CONFIG_MACRO_CACHE_SIZE; i ++) {
        head->cache[i].macro = NULL;
        head->cache[i].count = 0;
    }
}


void lm_macro_list_add(lm_macro_head_t *head, lm_macro_t *macro)
{
    head->count ++;
    lm_list_add_node_at_tail(&head->node, &macro->node);
}


int lm_macro_list_delete(lm_macro_head_t *head, lm_macro_t *macro)
{
    lm_list_node_t *find = lm_list_next_node(&head->node);

    for(int i = 0; i < head->count; i++) {
        
        lm_macro_t *node = container_of(find, lm_macro_t, node);
        if(node == macro) {
            head->count --;
            lm_list_del_node(find);

            return LM_OK;
        }
        find = lm_list_next_node(find);
    }

    return LM_ERR;
}


lm_macro_t* lm_macro_new(char* name)
{
    lm_macro_t* macro = lm_malloc(sizeof(lm_macro_t));
    if(macro == NULL) {
        return NULL;
    }

    macro->name = NULL;
    macro->choice.count = 0;
    lm_list_init(&macro->choice.head);
    macro->depend = NULL;
    macro->value = NULL;
    macro->type = LM_MACRO_STRING;
    macro->def_str = NULL;
    macro->def_flag = 0;

    int ret = lm_str_dupli_string(&macro->name, name);
    if(ret) {
        return NULL;
    }

    return macro;
}


lm_macro_t* lm_macro_new_and_add(lm_macro_head_t *head, char* name)
{
    lm_macro_t* macro = lm_macro_new(name);
    if(macro == NULL) {
        return NULL;
    }

    lm_macro_list_add(head, macro);

    return macro;
}


int lm_macro_value_set(lm_macro_t* macro, char *value)
{
    if(value == NULL) {
        macro->value = NULL;
        return LM_OK;
    }
    return lm_str_dupli_string(&macro->value, value);
}


void lm_macro_type_set(lm_macro_t* macro, enum lm_macro_type type)
{
    macro->type = type;
}


enum lm_macro_type lm_macro_type_get(lm_macro_t* macro)
{
    return macro->type;
}


void lm_macro_range_set(lm_macro_t* macro, double min, double max)
{
    macro->range.min = min;
    macro->range.max = max;
}


void lm_macro_choice_set_count(lm_macro_t* macro, int count)
{
    macro->choice.count = count;
}


int lm_macro_choice_append(lm_macro_t* macro, char* str)
{
    return lm_array_add(&macro->choice, str);
}


char *lm_macro_choice_get_first(lm_macro_t* macro)
{
    lm_list_node_t *node = lm_list_next_node(&macro->choice.head);
    lm_array_node_t *array_node = NULL;

    array_node = container_of(node, lm_array_node_t, node);

    return array_node->string;
}


int lm_macro_depend_set(lm_macro_t* macro, char* str)
{
    return lm_str_dupli_string(&macro->depend, str);
}


int lm_macro_default_set(lm_macro_t* macro, char* str)
{
    return lm_str_dupli_string(&macro->def_str, str);
}


static int __lm_macro_delete(lm_macro_t* macro)
{
    int ret = lm_array_delete(&macro->choice);
    if(ret != LM_OK) {
        return LM_ERR;
    }

    lm_free(macro->depend);
    lm_free(macro->name);
    lm_free(macro->value);
    lm_free(macro);

    return LM_OK;
}


int lm_macro_delete(lm_macro_head_t *head, lm_macro_t *macro)
{
    lm_list_node_t *node_head = &head->node;
    lm_list_node_t *node;
    lm_macro_t *find_macro = NULL;

    lm_list_for_each(node, node_head) {

        find_macro = container_of(node, lm_macro_t, node);
        if(find_macro == NULL) {
            return LM_ERR;
        }
        else if(find_macro == macro) {
            __lm_macro_delete(macro);
            head->count -- ;
            return LM_OK;
        }
    }

    return LM_ERR;
}


static void lm_macro_update_cache(lm_macro_head_t *head, lm_macro_t *macro)
{
    int min_index = 0;
    int min_count = 0;

    for(int i = 0; i < CONFIG_MACRO_CACHE_SIZE; i++) {
        if(head->cache[i].macro) {
            if(head->cache[i].count < min_count) {
                min_index = i;
            }
        }
        else {
            head->cache[i].macro = macro;
            head->cache[i].count ++;
            return;
        }
    }

    head->cache[min_index].macro = macro;
    head->cache[min_index].count ++;
}


lm_macro_t *lm_macro_search_by_name(lm_macro_head_t *head, char *name)
{
    if(head == NULL) {
        return NULL;
    }
    
    for(int i = 0; i < CONFIG_MACRO_CACHE_SIZE; i++) {
        if(head->cache[i].macro) {
            if(strcmp(head->cache[i].macro->name, name) == 0) {
                head->cache[i].count ++;
                return head->cache[i].macro;
            }
        }
    }

    lm_list_node_t *node_head = &head->node;
    lm_list_node_t *node;
    lm_macro_t *macro = NULL;

    lm_list_for_each(node, node_head) {

        macro = container_of(node, lm_macro_t, node);
        if(macro == NULL) {
            return NULL;
        }

        if(strcmp(macro->name, name) == 0) {
            lm_macro_update_cache(head, macro);
            return macro;
        }
    }

    return NULL;
}


bool lm_macro_value_is_valid(lm_macro_t *macro, char *value)
{
    lm_array_t *array = &macro->choice;
    lm_list_node_t *node = lm_list_next_node(&array->head);
    lm_array_node_t *array_node = NULL;

    if(macro->type == LM_MACRO_NUMBER) {
        double num_value = strtod(value, NULL);
        if(num_value >= macro->range.min && num_value <= macro->range.max) {
            return true;
        }
        else {
            return false;
        }
    }

    for(int i = 0; i < array->count; i++) {
        
        array_node = container_of(node, lm_array_node_t, node);

        if(array_node != LM_ERR_PTR) {
            if(strcmp(array_node->string, value) == 0) {
                return true;
            }
        }

        node = lm_list_next_node(node);
    }

    return false;
}


void lm_macro_print_all(FILE* output, lm_macro_head_t *head)
{
    if(head == NULL) {
        return;
    }
    
    lm_list_node_t *node_head = &head->node;
    lm_list_node_t *node;
    lm_macro_t *macro = NULL;

    lm_list_for_each(node, node_head) {

        macro = container_of(node, lm_macro_t, node);
        if(macro == NULL) {
            continue;
        }

        fprintf(output, "%s\n", macro->name);
        fprintf(output, "    choice: "); lm_array_print(output, &macro->choice);
        fprintf(output, "    value: %s\n", macro->value);
        fprintf(output, "    depend: %s\n", macro->depend);
    }
}


#if (_WIN32)

void lm_macro_print_all_value(lm_macro_head_t *head)
{
    printf("\x1b[32m"); //green color
    printf(ESC"(0""┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"ESC"(B" "\n");
    printf(ESC"(0""┃"ESC"(B""                               Lite-manager Configuration Information                           "ESC"(0""┃"ESC"(B" "\n");
    printf(ESC"(0""┣"ESC"(B━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫"ESC"(B" "\n");
    printf("\x1b[0m"); //clear color

    // print all macro value
    lm_list_node_t *node_head = &head->node;
    lm_list_node_t *node;
    lm_macro_t *macro = NULL;

    if(head->count) {
        lm_list_for_each(node, node_head) {

            macro = container_of(node, lm_macro_t, node);
            if(macro == NULL) {
                continue;
            }

            printf("\x1b[32m┃\x1b[0m");
            if(strcmp(macro->value, "n") == 0) {
                printf("⛔ %-50s%-43s", macro->name, " ");
            }
            else if(strcmp(macro->value, "'n'") == 0) {
                printf("✅ %-50s%-43s", macro->name, "n");
            }
            else {
                printf("✅ %-50s%-43s", macro->name, macro->value);
            }
            printf("\x1b[32m┃\x1b[0m\n");
        }
    }
    else {
        printf(ESC"(0""\x1b[32m┃\x1b[0m"ESC"(B");
        printf("                                       No Macro Configration!                                   ");
        printf(ESC"(0""\x1b[32m┃\x1b[0m"ESC"(B" "\n");
    }

    printf("\x1b[32m"); //green color
    printf(ESC"(0""┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"ESC"(B" "\n");
    printf("\x1b[0m"); //clear color
}

#elif ( __linux__)

void lm_macro_print_all_value(lm_macro_head_t *head)
{
    printf("\x1b[32m"); //green color
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃                               Lite-manager Configuration Information                           ┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
    printf("\x1b[0m"); //clear color

    // print all macro value
    lm_list_node_t *node_head = &head->node;
    lm_list_node_t *node;
    lm_macro_t *macro = NULL;

    if(head->count) {
        lm_list_for_each(node, node_head) {

            macro = container_of(node, lm_macro_t, node);
            if(macro == NULL) {
                continue;
            }

            printf("\x1b[32m┃\x1b[0m");
            if(strcmp(macro->value, "n") == 0) {
                printf("⛔ %-50s%-43s", macro->name, " ");
            }
            else if(strcmp(macro->value, "'n'") == 0) {
                printf("✅ %-50s%-43s", macro->name, "n");
            }
            else {
                printf("✅ %-50s%-43s", macro->name, macro->value);
            }
            printf("\x1b[32m┃\x1b[0m\n");
        }
    }
    else {
        printf("\x1b[32m┃\x1b[0m");
        printf("                                       No Macro Configration!                                   ");
        printf("\x1b[32m┃\x1b[0m\n");
    }

    printf("\x1b[32m"); //green color
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
    printf("\x1b[0m"); //clear color
}

#endif