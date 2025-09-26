/* source/lm_parser.c
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
#include "lm_string.h"
#include "lm_error.h"
#include "lm_mem.h"
#include "lm_macro.h"
#include "lm_parser.h"
#include "lm_log.h"
#include <dirent.h>
#include <ctype.h>


#define MAX_PER_LINE_LENGTH          4096
#define MAX_FILE_PATH                1024
#define MAX_MACRO_NAME               1024


static char lm_parser_list_name[][20] = {
    {VAR_C_SOURCE},
    {VAR_C_OBJECT},
    {VAR_C_PATH},
    {VAR_C_DEFINE},
    {VAR_ASM_SOURCE},
    {VAR_LDS_SOURCE},
    {VAR_MC_FLAG},
    {VAR_AS_FLAG},
    {VAR_C_FLAG},
    {VAR_CPP_FLAG},
    {VAR_LD_FLAG},
    {VAR_LIB_NAME},
    {VAR_LIB_PATH},
};


static struct lm_parser_list {
    lm_array_t src_list;
    lm_array_t obj_list;
    lm_array_t path_list;
    lm_array_t define_list;
    lm_array_t asm_list;
    lm_array_t lds_list;
    lm_array_t mcflag_list;
    lm_array_t asflag_list;
    lm_array_t cflag_list;
    lm_array_t cppflag_list;
    lm_array_t ldflag_list;
    lm_array_t lib_list;
    lm_array_t libpath_list;

}lm_parser_list;


static lm_macro_head_t config_head;
static lm_macro_head_t macro_head;
static const char *config_file = NULL;
static char error_msg[MAX_PER_LINE_LENGTH] = {0};


static char *lm_parser_alloc(void)
{
    char *mem = (char*)lm_malloc(MAX_PER_LINE_LENGTH);
    if(mem == NULL) {
        LM_LOG_ERROR("out of memory");
        lm_mem_destroy();
        exit(1);
    }

    memset(mem, 0, MAX_PER_LINE_LENGTH);
    return mem;
}


static void lm_parser_skip_space(char **p)
{
    while(**p) {
        if(**p != ' ') {
            return;
        }

        (*p) ++;
    }
}


void lm_parser_init(void)
{
    int len = sizeof(lm_parser_list) / sizeof(lm_array_t);
    lm_array_t *list = (lm_array_t*)&lm_parser_list;

    for(int i = 0; i < len; i++) {
        lm_list_init(&list->head);
        list ++;
    }

    lm_list_init(&config_head.node);
    lm_macro_list_cache_init(&config_head);

    lm_list_init(&macro_head.node);
    lm_macro_list_cache_init(&macro_head);
}


static char* lm_parser_get_macro_value(lm_macro_head_t *head, lm_macro_t *macro)
{
    lm_macro_t *search = lm_macro_search_by_name(head, macro->name);
    if (search != NULL) { // found macro name
        if (search->value == NULL) {
            return NULL;
        }
        else {
            return search->value;
        }
    }
    else {
        return NULL;
    }
}


static bool lm_parser_is_skip_line(char *read_line)
{
    if (lm_str_find_char(read_line, '#') == 0) { // checked annotate
        return true;
    }
    else {
        if (lm_str_is_all_space(read_line)) {
            return true;
        }
    }
    return false;
}


static char *lm_parser_config_get_macro_name(char *str)
{
    char *ret = str;
    char *p = str;
    while (*p) {
        if (*p == '=') {
            *p = '\0';
            return ret;
        }
        p++;
    }

    return NULL;
}


static char *lm_parser_config_get_macro_value(char *str)
{
    while (*str) {
        if (*str == '=') {
            return str + 1;
        }
        str++;
    }

    return NULL;
}


static int lm_parser_config_file_key_string(char *readline)
{
    char *tmp = lm_parser_alloc();

    strcpy(tmp, readline);

    int len = sizeof(lm_parser_list_name) / sizeof(lm_parser_list_name[0]);
    char *name = NULL;

    for(int i = 0; i < len; i++) {
        name = lm_parser_list_name[0];
        strcpy(tmp, readline);

        int key_len = strlen(name);
        tmp[key_len] = '\0';
        if(strcmp(tmp, name) == 0) {
            lm_free(tmp);
            return LM_OK;
        }
    }

    lm_free(tmp);
    return LM_ERR;
}


int lm_parser_config_file(const char *path)
{
    if(path == NULL) {
        return 0; //no defconfig file
    }

    config_file = path;

    lm_macro_t *macro = NULL;
    char *read_line = lm_parser_alloc();

    char *read_scan = NULL;
    FILE *file_p = NULL;
    int line_count = 0;

    file_p = fopen(path, "r"); // read only
    if (file_p == NULL) {
        LM_LOG_ERROR("file: %s, lines: %d, No such file", path, line_count);
        return LM_ERR;
    }

    while (fgets(read_line, MAX_PER_LINE_LENGTH, file_p)) {

        line_count++;

        if (read_line[strlen(read_line) - 1] == '\n' 
           || read_line[strlen(read_line) - 1] == '\r') {
            read_line[strlen(read_line) - 1] = '\0';
        }

        if (lm_parser_is_skip_line(read_line)) {
            continue;
        }

        if(lm_parser_config_file_key_string(read_line) == LM_OK){
            continue;
        }

        char *macro_name = lm_str_delete_space(read_line);
        if (macro_name == NULL) {
            LM_LOG_ERROR("file: %s, lines: %d, syntax error", path, line_count);
            return LM_ERR;
        }

        macro_name = lm_parser_config_get_macro_name(macro_name);
        if (macro_name == NULL) {
            LM_LOG_ERROR("file: %s, lines: %d, syntax error", path, line_count);
            return LM_ERR;
        }

        macro = lm_macro_new_and_add(&config_head, macro_name);
        if (macro == NULL) {
            LM_LOG_ERROR("macro new error\n");
            return LM_ERR;
        }

        lm_free(macro_name);

        read_scan = lm_parser_config_get_macro_value(read_line);
        if (read_scan == NULL) {
            LM_LOG_ERROR("file: %s, lines: %d, syntax error", path, line_count);
            return LM_ERR;
        }

        if(lm_str_is_all_space(read_scan)) {
            lm_macro_value_set(macro, " ");
        }
        else {
            char *macro_value = lm_str_delete_head_tail_space(read_scan);
            lm_macro_value_set(macro, macro_value);
            lm_free(macro_value);
        }
    }

    fclose(file_p); // close file
    return LM_OK;
}


static int lm_parser_macro_express_value(const char *expression, int *index);

// 解析因子
static int evaluate_factor(const char *expression, int *index)
{
    int result;

    if (expression[*index] == '!') {
        (*index)++;
        result = evaluate_factor(expression, index);
        if (result == -1) 
            return -1;
        
        result = !result;
    }
    else if (expression[*index] == '(') {
        (*index)++;
        result = lm_parser_macro_express_value(expression, index);
        if (result == -1)
            return -1;

        if (expression[*index] == ')') {
            (*index)++;
        }
        else {
            return -1; // 缺少右括号
        }
    }
    else if (isdigit(expression[*index])) {
        result = expression[*index] - '0';
        (*index)++;
    }
    else {
        return -1; // 无效字符
    }

    return result;
}


// 解析项
static int evaluate_term(const char *expression, int *index)
{
    int result = evaluate_factor(expression, index);
    if (result == -1) return -1;

    while (expression[*index] == '&') {
        (*index)++;
        int factor_result = evaluate_factor(expression, index);
        if (factor_result == -1)
            return -1;
        
        result &= factor_result;
    }

    return result;
}


// 解析表达式, 如果表达式未完全解析，则表示语法错误
static int lm_parser_macro_express_value(const char *expression, int *index)
{
    int result = evaluate_term(expression, index);
    if (result == -1) 
        return -1;

    while (expression[*index] == '|') {
        (*index)++;
        int term_result = evaluate_term(expression, index);
        if (term_result == -1)
            return -1;
        
        result |= term_result;
    }

    return result;
}


static char *lm_parser_macro_depend_preprocess(char *depend)
{
    if(depend == NULL) {
        return NULL;
    }
    
    int dep_len = strlen(depend) + 1;

    char *output = lm_parser_alloc();
    char *p = output;
    char macro_name[MAX_MACRO_NAME];
    char *name_p = macro_name;

    for (int i = 0; i < dep_len; i++) {
        if (*depend == ' ') {
            depend++;
            continue;
        }
        else if (*depend != '&' && *depend != '|' && *depend != '!' 
                 && *depend != '(' && *depend != ')' && *depend != '\0') {
            *name_p++ = *depend++;
        }
        else {
            *name_p++ = '\0';

            if(macro_name[0] != 0) {

                lm_macro_t *macro = lm_macro_search_by_name(&macro_head, macro_name);
                if (macro != NULL) { // found macro name
                    if (macro->value == NULL || strcmp(macro->value, "n") == 0 || strcmp(macro->value, " ") == 0) {
                        *p++ = '0';
                    }
                    else {
                        *p++ = '1';
                    }
                }
                else {
                    strcpy(error_msg, macro_name);
                    return "error";
                }
            }

            if(*depend)
                *p++ = *depend++;

            name_p = macro_name;
        }
    }
    *p = '\0';

    return output;
}


static int lm_parser_get_macro_depend_value(lm_macro_t *macro)
{
    int ret = 0, index = 0;

    if (macro == NULL) {
        return -1;
    }

    char *dep = lm_parser_macro_depend_preprocess(macro->depend);
    if(dep == NULL) { //no dependence
        return 1;
    }
    else if(strcmp(dep, "error") == 0) {
        return 2; //undefine macro error
    }

    ret = lm_parser_macro_express_value(dep, &index);
    if (dep[index] != '\0') {
        return -1;
    }

    return ret;
}


// return 0: disable  1: enable
static int lm_parser_get_macro_status_by_name(char *macro_name, char *macro_val)
{
    lm_macro_head_t *head = &macro_head;
    lm_macro_t * macro = NULL;

    while(1) {
        macro = lm_macro_search_by_name(head, macro_name);
        if(macro != NULL) {
            if(strcmp(macro->value, "n") == 0) {
                if(macro_val[0] == 0 || strcmp(macro_val, "n") != 0) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            else {
                if(macro_val[0] == 0 || strcmp(macro->value, macro_val) == 0) {
                    return 1;
                }
                else {
                    return 0;
                }
            }
        }
        else {
            if(head == &config_head)
                return 0;

            head = &config_head;
        }
    }

    return 0;
}


// return 0: disable  1: enable  -1: syntax error
static int lm_parser_get_keystring_depend_value(char* keystring)
{
    char macro_name[512] = {0};
    char str_value[512] = {0};
    char *p = keystring;
    int i = 0;
    int status = 0;
    int flag = 0;

    while(*p) {
        switch(status) {
            case 0:
                if(*p == ' ') {
                    break;
                }
                else {
                    status = 1;
                }
                // fall through

            // match macro name
            case 1:
                if(*p != ' ' && *p != '=') {
                    macro_name[i++] = *p;
                    flag = 1;
                    break;
                }
                else {
                    status = 2;
                    macro_name[i] = '\0';
                    i = 0;
                }
                // fall through
            
            // filter space
            case 2:
                if(*p == ' ') {
                    break;
                }
                else {
                    status = 3;
                }
                // fall through
            // match '='
            case 3:
                if(*p == '=' && *(p + 1) == '=') {
                    status = 4;
                    flag = 0;
                    p ++;
                    break;
                }
                else {
                    return -1; //error
                }
                // fall through

            // filter space
            case 4:
                if(*p == ' ') {
                    break;
                }
                else {
                    status = 5;
                }
                // fall through

            // match value
            case 5:
                if(*p != ' ') {
                    str_value[i++] = *p;
                    str_value[i] = '\0';
                    flag = 1;
                    break;
                }
                else {
                    status = 6;
                }
                // fall through

            case 6:
                if(*p != ' ') {
                    return -1;
                }
                else {
                    break;
                }
        }
        p++;
    }

    if(flag == 0 || macro_name[0] == 0) {
        return -1; //syntax error
    }

    return lm_parser_get_macro_status_by_name(macro_name, str_value);
}


static void lm_parser_src_add_wildcard(const char *wildcard, const char *path, lm_array_t *array)
{
    DIR *dir;
    struct dirent *entry;
    char file_name[MAX_FILE_PATH] = {0};
    char wildcard_path[MAX_FILE_PATH] = {0};
    char abs_path[MAX_FILE_PATH] = {0};

    char *wildcard_dir = strrchr(wildcard, '/');
    if(wildcard_dir) {
        strncpy(wildcard_path, wildcard, strlen(wildcard) - strlen(wildcard_dir));

        if(strcmp(path, ".") == 0) {
            sprintf(abs_path, "%s", wildcard_path);
        }
        else {
            sprintf(abs_path, "%s/%s", path, wildcard_path);
        }
    }
    else {
        strcpy(abs_path, path);
    }

    dir = opendir(abs_path);
    if (dir == NULL) {
        LM_LOG_ERROR("%s: No such directory", abs_path);
        lm_mem_destroy();
        LM_LOG_ERROR("parser failed, exiting");
    }

    while ((entry = readdir(dir)) != NULL) {
        size_t len = strlen(entry->d_name);

        if (len >= 2 && strcmp(entry->d_name + len - 2, ".c") == 0) {
            if(strcmp(abs_path, ".") == 0) {
                sprintf(file_name, "%s", entry->d_name);
            }
            else {
                sprintf(file_name, "%s/%s", abs_path, entry->d_name);
            }

            lm_array_add(array, file_name);
        }
    }

    closedir(dir);
}


static lm_parser_err_e lm_parser_prompt_src_add_list(const char *path, char *read_line)
{
    char macro_name[MAX_MACRO_NAME];
    char file_name[MAX_FILE_PATH];
    lm_array_t *array = NULL;
    char *p = read_line;
    int status = 0, i = 0;
    bool flag = false;
    bool enable = true;

    while(*p) {
        switch(status) {
        // match if src or obj or others
        case 0:
            if(p[0] == 'S' && p[1] == 'R' && p[2] == 'C') {
                array = &lm_parser_list.src_list;
            }
            else {
                return LM_PARSER_NOT_MATCH;
            }

            p += 3;

            if(*p == '-') {
                status = 1;
                break;
            }
            else if(*p == ' ') {
                status = 2;
                break;
            }
            else {
                return LM_PARSER_NOT_MATCH;
            }

        // check src or obj is enable ?
        case 1:
            if(*p == '$' && *(p+1) == '(') {
                p += 1;
                while(*p++) {
                    if(*p == ')') {
                        macro_name[i++] = '\0';
                        flag = true;
                        break;
                    }
                    macro_name[i++] = *p;
                }
            }
            else {
                return LM_PARSER_SYNTAX;
            }

            if(!flag) {
                return LM_PARSER_SYNTAX;
            }

            int depend_val = lm_parser_get_keystring_depend_value(macro_name);
            if(depend_val < 0) {
                return LM_PARSER_SYNTAX;
            }
            else {
                status = 2;
                enable = depend_val == 1 ? true : false;
                break;
            }

        case 2:
            while(*p) {
                if(*p == '+' && *(p+1) == '=') {
                    p += 2;
                    break;
                }
                else if(*p == ' ') {
                    p ++;
                }
                else {
                    return LM_PARSER_SYNTAX; 
                }
            }

            if(!enable) {
                return LM_PARSER_OK;
            }

            int num = lm_str_num_str_space(p);
            char *pick = NULL;

            for(int i = 0; i < num; i++) {
                pick = lm_str_pick_str(p, i);
                lm_str_delete_tail_space(pick);

                if(pick && array) {
                    if((strcmp(pick, "*.c") == 0)|| strcmp(pick + strlen(pick) - 3, "*.c") == 0) {
                        lm_parser_src_add_wildcard(pick, path, array);
                        lm_free(pick);
                    }
                    else {
                        if(strcmp(path, ".") == 0)
                            sprintf(file_name, "%s", pick);
                        else
                            sprintf(file_name, "%s/%s", path, pick);

                        lm_array_add(array, file_name);
                        lm_free(pick);
                    }
                }
            }

            return LM_PARSER_OK;
        }
        p++;
    }

    return LM_PARSER_NOT_MATCH;
}


static void lm_parser_add_list_raw(lm_array_t *list, char *flag)
{
    char str[MAX_FILE_PATH];

    if(list) {
        sprintf(str, "%s", flag);
        lm_array_add(list, str);
    }
}


static void lm_parser_add_list_path_and_prefix(lm_array_t *list, const char *path, char *prefix, char *flag)
{
    char str[MAX_FILE_PATH];

    if(path && prefix) {
        if(strcmp(flag, ".") == 0 || strcmp(flag, "./") == 0) {
            if(strcmp(path, ".") != 0 && strcmp(path, "./") != 0)
                sprintf(str, "%s%s", prefix, path);
            else
                sprintf(str, "%s.", prefix);
        }
        else {
            if(strcmp(path, ".") == 0)
                sprintf(str, "%s%s", prefix, flag);
            else
                sprintf(str, "%s%s/%s", prefix, path, flag);
        }
        
        lm_array_add(list, str);
    }
    else if(path) {
        if(strcmp(flag, ".") == 0 || strcmp(flag, "./") == 0) {
            if(strcmp(path, ".") != 0 && strcmp(path, "./") == 0) {
                sprintf(str, "%s/%s", path, flag);
                lm_array_add(list, str);
            }
        }
        else {
            if(strcmp(path, ".") == 0)
                sprintf(str, "%s", flag);
            else
                sprintf(str, "%s/%s", path, flag);

            lm_array_add(list, str);
        }
    }
    else if(prefix) {
        sprintf(str, "%s%s", prefix, flag);
        lm_array_add(list, str);
    }
    else {
        sprintf(str, "%s", flag);
        lm_array_add(list, str);
    }
}


static lm_parser_err_e lm_parser_key_string_add_list(lm_array_t *list, const char *path, char *prefix, bool rawflag, char *key_str, char *readline)
{
    char macro_name[MAX_MACRO_NAME];
    char *p = readline;
    int index = 0;
    int status = 0;
    int key_len = strlen(key_str);
    bool flag = false;
    bool enable = true;

    while(*p) {
        switch(status) {
        //match key string
        case 0:
            for(int i = 0; i < key_len; i ++) {
                if(p[i] == key_str[i]) {
                    continue;
                }
                else {
                    return LM_PARSER_NOT_MATCH;
                }
            }

            p += key_len;

            if(*p == '-') {
                status = 1;
                break;
            }
            else if(*p == ' ') {
                status = 2;
                break;
            }
            else {
                return LM_PARSER_NOT_MATCH;
            }

        //check key is enbale
        case 1:
            if(*p == '$' && *(p+1) == '(') {
                p += 1;
                while(*p++) {
                    if(*p == ')') {
                        macro_name[index++] = '\0';
                        flag = true;
                        break;
                    }
                    macro_name[index++] = *p;
                }

                if(!flag) {
                    return LM_PARSER_SYNTAX;
                }

                int depend_val = lm_parser_get_keystring_depend_value(macro_name);
                if(depend_val < 0) {
                    return LM_PARSER_SYNTAX;
                }
                else {
                    status = 2;
                    enable = depend_val == 1 ? true : false;
                    break;
                }
            }
            else {
                return LM_PARSER_SYNTAX;
            }
        
        //add key value into list
        case 2:
            while(*p) {
                if(*p == '+' && *(p+1) == '=') {
                    p += 2;
                    break;
                }
                else if(*p == ' ') {
                    p ++;
                }
                else {
                    return LM_PARSER_SYNTAX; 
                }
            }

            if(!enable) {
                return LM_PARSER_OK;
            }

            if(rawflag) {
                lm_parser_add_list_raw(list, p);
                return LM_PARSER_OK;
            }

            int num = lm_str_num_str_space(p);
            char *pick = NULL;

            for(int i = 0; i < num; i++) {
                pick = lm_str_pick_str(p, i);
                if(pick && list) {
                    lm_parser_add_list_path_and_prefix(list, path, prefix, pick);

                    lm_free(pick);
                }
            }
            return LM_PARSER_OK;
        }
        p ++;
    }

    return LM_PARSER_SYNTAX;
}


static lm_macro_t *lm_parser_prompt_is_macro(lm_macro_head_t *head, char *read_line)
{
    lm_macro_t *ret;
    if (read_line[0] != ' ') {
        if (lm_str_num_str_space(read_line) == 1) {
            char *name = lm_str_delete_space(read_line);
            ret = lm_macro_new_and_add(head, name);

            lm_free(name);
            return ret;
        }
    }
    return NULL;
}


static lm_parser_err_e lm_parser_prompt_is_default(lm_macro_t *macro, char *read_line)
{
    char *p = read_line;

    if (!lm_str_head_is_four_space(read_line)) {
        strcpy(error_msg, "syntax error: must start with four spaces");
        return LM_PARSER_SYNTAX;
    }

    if (lm_str_find_str_space(read_line, "default") == 0 
        && lm_str_find_str_space(read_line, "=") == 1) {

        while (*p) {
            if (*p != '=')
                p++;
            else
                break;
        }

        char *def_str = lm_str_delete_space(p + 1);
        if(def_str == NULL) {
            strcpy(error_msg, "invalid default value");
            return LM_PARSER_SYNTAX;
        }

        /* check value is valid */
        if(lm_macro_value_is_valid(macro, def_str) == false) {
            strcpy(error_msg, "invalid default value");
            return LM_PARSER_SYNTAX;
        }

        if(macro->type == LM_MACRO_NUMBER) {
            char *endptr;
            float number = strtof(def_str, &endptr);
            if (*endptr != 0) {
                strcpy(error_msg, "invalid default value");
                return LM_PARSER_SYNTAX; // 第一个数字无效
            }

            macro->def_num = number;
            macro->def_flag = 1;
        }
        else if(macro->type == LM_MACRO_STRING) {
            lm_macro_default_set(macro, def_str);
            macro->def_flag = 1;
            lm_free(def_str);
        }

        return LM_PARSER_OK;
    }

    return LM_PARSER_NOT_MATCH;
}


static lm_parser_err_e lm_parser_prompt_is_depend(lm_macro_t *macro, char *read_line)
{
    char *p = read_line;

    if (!lm_str_head_is_four_space(read_line)) {
        strcpy(error_msg, "syntax error: must start with four spaces");
        return LM_PARSER_SYNTAX;
    }

    if (lm_str_find_str_space(read_line, "depends") == 0 
        && lm_str_find_str_space(read_line, "=") == 1) {

        while (*p) {
            if (*p != '=')
                p++;
            else
                break;
        }

        lm_macro_depend_set(macro, p+1);

        return LM_PARSER_OK;
    }

    return LM_PARSER_NOT_MATCH;
}


static char *lm_parser_prompt_process_var(char *read_line)
{
    char *rel_val = lm_parser_alloc();
    char *rel_p = rel_val;

    char *p = read_line;
    char var_name[MAX_MACRO_NAME];
    int i = 0;

    while(*p) {
        if(*p == '$' && *(p+1) == '(') {
            p+=2;
            while(*p) {
                if(*p != ')' && *p != ' ') {
                   var_name[i++] = *p;
                }
                else if(*p != ' ') {
                    var_name[i] = '\0';

                    lm_macro_t * macro = lm_macro_search_by_name(&macro_head, var_name);
                    if(macro != NULL) {
                        if(macro->value != NULL) {
                            strcat(rel_val, macro->value);
                            rel_p += strlen(macro->value);
                        }
                    }

                    i = 0;
                    break;
                }

                p++;
            }
        }
        else {
            *rel_p++ = *p;
        }

        p++;
    }

    if(i != 0) {
        return NULL;
    }
    
    *rel_p = '\0';
    return rel_val;
}


static char *lm_parser_prompt_is_include(char *read_line)
{
    char macro_depend[MAX_MACRO_NAME];
    char *p = read_line;
    int status = 0, i = 0;
    bool flag = false;
    int ret = -1;

    while(*p) {
        switch(status) {
        // match if is include
        case 0:
            if(p[0] == 'i' && p[1] == 'n' && p[2] == 'c' && p[3] == 'l' && p[4] == 'u' && p[5] == 'd' && p[6] == 'e' && 
               (p[7] == ' ' || (p[7] == '-' && p[8] == '$'))
              ) {

                ret = lm_str_num_of_substr_split(p);
                if( ret != 2 || ret < 0) {
                    return "syntax_error";
                }
                p += 7;

                if(*p != '-') {
                    status = 2;
                    break;
                }
                else {
                    status = 1;
                    break;
                }
            }
            else {
                return NULL;
            }

        // check include is enable
        case 1:
            if(*p == '$' && *(p+1) == '(') {
                p += 1;
                while(*p++) {
                    if(*p == ')') {
                        macro_depend[i++] = '\0';
                        flag = true;
                        break;
                    }
                    macro_depend[i++] = *p;
                }
            }
            else {
                return "syntax_error";
            }
            
            if(!flag) {
                return "syntax_error";
            }

            int depend_val = lm_parser_get_keystring_depend_value(macro_depend);
            if(depend_val == 0) {
                return "lm_ok";
            }
            else if(depend_val == 1) {
                status = 2;
                break;
            }
            else if(depend_val < 0) {
                return "syntax_error";
            }
            
            // fall through
        case 2:
            p = lm_str_get_quote(read_line);
            if(p == NULL) {
                return "syntax_error";
            }
            return lm_parser_prompt_process_var(p);
            // fall through
        }
        p++;
    }

    return NULL;
}


static lm_parser_err_e lm_parser_prompt_is_choice_number(lm_macro_t *macro, char *str)
{
    char *p_sc = str;
    double range_min, range_max;

    lm_parser_skip_space(&p_sc);
    
    if(p_sc[0] != '[') {
        return LM_PARSER_NOT_MATCH;
    }

    if (strlen(p_sc) < 3 || p_sc[strlen(p_sc) - 1] != ']') {
        return LM_PARSER_SYNTAX;
    }

    char *token = strtok((char *)p_sc + 1, ",]");
    if (token == NULL) {
        return LM_PARSER_SYNTAX;
    }

    lm_macro_choice_append(macro, token);

    char *endptr;
    float num = strtof(token, &endptr);

    if (*endptr != 0) {
        return LM_PARSER_SYNTAX; // 第一个数字无效
    }
    range_min = num; // 第一个数字

    token = strtok(NULL, "]");
    if (token == NULL) {
        return LM_PARSER_SYNTAX; // 没有找到第二个数字
    }

    num = strtof(token, &endptr);
    if (*endptr != 0) {
        return LM_PARSER_SYNTAX; // 第二个数字无效
    }
    range_max = num; // 第二个数字

    lm_macro_type_set(macro, LM_MACRO_NUMBER);
    lm_macro_range_set(macro, range_min, range_max);

    return LM_PARSER_OK;
}


static int lm_parser_prompt_choice_add_value(lm_macro_t *macro, char *value_str)
{
    char *p = value_str;
    int len = strlen(p) + 1;

    char stack[MAX_MACRO_NAME];
    char *stack_p = stack;
    memset(stack, 0, MAX_MACRO_NAME);
    bool flag = false;

    for (int i = 0; i < len; i++) {

        if(*p == '{' && *p != '\0') {
            flag = true;
            while(*p) {
                *stack_p++ = *p++;
                i ++;

                if(*p == '}' && *p != '\0') {
                    flag = false;
                    break;
                }
            }
        }

        if(*p == '\"' && *p != '\0') {
            flag = true;
            while(*p) {
                *stack_p++ = *p++;
                i ++;

                if(*p == '\"' && *p != '\0') {
                    flag = false;
                    break;
                }
            }
        }

        if(*p == '\'' && *p != '\0') {
            p ++;
            flag = true;
            while(*p) {
                *stack_p++ = *p++;
                i ++;

                if(*p == '\'' && *p != '\0') {
                    flag = false;
                    break;
                }
            }
        }

        if(*p != ' ' && *p != '\'') {
            if (*p != ',' && *p != '\0') {
                *stack_p++ = *p;
            }
            else {
                *stack_p = '\0';

                lm_macro_type_set(macro, LM_MACRO_STRING);
                lm_macro_choice_append(macro, stack);
                stack_p = stack;
            }
        }

        p++;
    }

    if(flag) {
        return -1;
    }
    return 0;
}


static lm_parser_err_e lm_parser_prompt_is_choice(lm_macro_t *macro, char *read_line)
{
    char *p = read_line;

    if (!lm_str_head_is_four_space(read_line)) {
        strcpy(error_msg, "syntax error: must start with four spaces");
        return LM_PARSER_SYNTAX;
    }

    if (lm_str_find_str_space(read_line, "choices") == 0 
         && lm_str_find_str_space(read_line, "=") == 1) {

        while (*p) {
            if (*p != '=')
                p++;
            else
                break;
        }

        char *value_str = NULL;
        lm_str_dupli_string(&value_str, ++p);

        lm_parser_err_e err_ret = lm_parser_prompt_is_choice_number(macro, value_str);
        if(err_ret == LM_PARSER_OK) {
            return LM_PARSER_OK;
        }
        else if(err_ret == LM_PARSER_SYNTAX) {
            strcpy(error_msg, "invalid range format");
            return LM_PARSER_SYNTAX;
        }

        if (value_str[0] == ',' || value_str[strlen(value_str) - 1] == ',') {
            strcpy(error_msg, "expect a ']'");
            return LM_PARSER_SYNTAX;
        }

        if(lm_parser_prompt_choice_add_value(macro, value_str)) {
            strcpy(error_msg, "invalid choice value");
            return LM_PARSER_SYNTAX;
        }

        lm_free(value_str);

        return LM_PARSER_OK;
    }

    return LM_PARSER_NOT_MATCH;
}


static void trim_tail_zero(char *str)
{
    char *endp = str + strlen(str) - 1;
    while(endp > str && *endp == '0') {
        *endp = 0;
        endp --;
    }

    if(endp > str && *endp == '.') {
        *endp = 0;
    }
}


static lm_parser_err_e lm_parser_macro_set_value(lm_macro_t *macro)
{
    char value_str[1024] = {0};

    if(macro == NULL) {
        return LM_PARSER_OK;
    }

    int value = lm_parser_get_macro_depend_value(macro);
    if(value == 0) {
        lm_macro_value_set(macro, "n");
        return LM_PARSER_OK;
    }
    if(value == 1) {
        lm_macro_t *search = lm_macro_search_by_name(&config_head, macro->name);
        if(search == NULL) { //not found in defconfig list
            if(macro->def_flag == 1) {
                if(macro->type == LM_MACRO_NUMBER) {
                    snprintf(value_str, 1024, "%lf", macro->def_num);
                    trim_tail_zero(value_str);
                    lm_macro_value_set(macro, value_str);

                    if(lm_macro_value_is_valid(macro, value_str) == false) {
                        return LM_PARSER_INVALID_VALUE;
                    }
                }
                else if(macro->type == LM_MACRO_STRING) {
                    lm_macro_value_set(macro, macro->def_str);

                    if(lm_macro_value_is_valid(macro, macro->value) == false) {
                        return LM_PARSER_INVALID_VALUE;
                    }
                }
            }
            else {
                char *first_value = lm_macro_choice_get_first(macro);
                if(first_value)
                    lm_macro_value_set(macro, first_value);
                else
                    lm_macro_value_set(macro, "Unknow ERROR");
            }
        }
        else {
            if(strcmp(search->value, "n") == 0) {
                lm_macro_value_set(macro, "n");
            }
            else if(strcmp(search->value, "'n'") == 0) {
                if(lm_macro_value_is_valid(macro, "n")) {
                    lm_macro_value_set(macro, "'n'");
                }
                else {
                    return LM_PARSER_INVALID_VALUE;
                }
            }
            else if(lm_macro_value_is_valid(macro, search->value)) {
                lm_macro_value_set(macro, search->value);
            }
            else {
                return LM_PARSER_INVALID_VALUE;
            }
        }
    }
    else if(value < 0) {
        return LM_PARSER_INVALID_DEPEND;
    }
    else {
        return LM_PARSER_INVALID_MACRO;
    }

    return LM_PARSER_OK;
}


static lm_parser_err_e lm_parser_lm_file_key_string(const char *base_path, char *read_line)
{
    lm_parser_err_e prompt_err = lm_parser_prompt_src_add_list(base_path, read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.path_list, base_path, "-I", false, "PATH", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.define_list, NULL, "-D", false, "DEFINE", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.asm_list, base_path, NULL, false, "ASM", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.lds_list, base_path, NULL, false, "LDS", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.mcflag_list, NULL, NULL, true, "MCFLAG", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.asflag_list, NULL, NULL, true, "ASFLAG", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.cflag_list, NULL, NULL, true, "CFLAG", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.cppflag_list, NULL, NULL, true, "CPPFLAG", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.ldflag_list, NULL, NULL, true, "LDFLAG", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.lib_list, NULL, "-l", false, "LIB", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    prompt_err = lm_parser_key_string_add_list(&lm_parser_list.libpath_list, base_path, "-L", false, "LIBPATH", read_line);
    if(prompt_err != LM_PARSER_NOT_MATCH) {
        return prompt_err;
    }

    return LM_PARSER_NOT_MATCH;
}


static int lm_parser_macro_set_prompt(lm_macro_t *macro, char *read_line)
{
    lm_parser_err_e prompt_ret = LM_PARSER_NOT_MATCH;

    prompt_ret = lm_parser_prompt_is_choice(macro, read_line);
    if(prompt_ret == LM_PARSER_SYNTAX) {
        return LM_ERR;
    }
    else if(prompt_ret == LM_PARSER_OK) {
        return LM_OK;
    }

    prompt_ret = lm_parser_prompt_is_default(macro, read_line);
    if(prompt_ret == LM_PARSER_SYNTAX) {
        return LM_ERR;
    }
    else if(prompt_ret == LM_PARSER_OK) {
        return LM_OK;
    }

    prompt_ret = lm_parser_prompt_is_depend(macro, read_line);
    if(prompt_ret == LM_PARSER_SYNTAX) {
        return LM_ERR;
    }
    else if(prompt_ret == LM_PARSER_OK) {
        return LM_OK;
    }

    strcpy(error_msg, "invalid prompt, only support: choices, default, depends");
    return LM_ERR;
}


static void lm_parser_macro_choice_helper(const char *file, int lines, lm_macro_t *macro)
{
    char *value_p = lm_parser_get_macro_value(&config_head, macro);
    if(value_p == NULL) {
        LM_LOG_ERROR("%s:%d %s = %s value is invalid", file, lines, macro->name, macro->value);
    }
    else {
        LM_LOG_ERROR("%s: %s = %s value is invalid", config_file, macro->name, value_p);
    }

    printf("\x1b[32m[INFO]💡You can choose: ");

    if(lm_macro_type_get(macro) == LM_MACRO_NUMBER) {
        printf("[%g ~ %g]", macro->range.min, macro->range.max);
    }
    else {
        lm_array_print(stdout, &macro->choice);
    }
    printf("\x1b[0m\n");
}


static int lm_parser_file_line_preprocess(FILE *file, char *read_line)
{
    int count = 0;

    if (read_line[strlen(read_line) - 1] == '\n' || read_line[strlen(read_line) - 1] == '\r') {
        read_line[strlen(read_line) - 1] = '\0';
    }

    if(read_line[strlen(read_line) - 1] == '\\') {
        read_line[strlen(read_line) - 1] = '\0';

        char *read_line_tmp = lm_parser_alloc();

        while(fgets(read_line_tmp, MAX_PER_LINE_LENGTH, file)) {
            if (read_line_tmp[strlen(read_line_tmp) - 1] == '\n' || read_line_tmp[strlen(read_line_tmp) - 1] == '\r') {
                read_line_tmp[strlen(read_line_tmp) - 1] = '\0';
            }

            count ++;
            char *pure = lm_str_delete_head_tail_space(read_line_tmp);
            
            read_line[strlen(read_line) - 1] = '\0';
            strcat(read_line, pure);
            
            lm_free(pure);
            if(read_line_tmp[strlen(read_line_tmp) - 1] != '\\') {
                break;
            }
        }

        lm_free(read_line_tmp);
    }

    return count;
}


int lm_parser_lm_file(const char *base_path, const char *path)
{
    char *read_line = lm_parser_alloc();
    int line_count = 0;
    lm_macro_t *macro = NULL;
    FILE *file_p = NULL;

    char *full_path = lm_parser_alloc();
    if(base_path == NULL || (strcmp(base_path, ".") == 0)) {
        strcpy(full_path, path);
    }
    else {
        sprintf(full_path, "%s/%s", base_path, path);
    }

    file_p = fopen(full_path, "r"); // read only
    if (file_p == NULL) {
        LM_LOG_ERROR("file: %s, No such file", full_path);
        return LM_ERR;
    }

    char *new_base_path = lm_parser_alloc();
    strcpy(new_base_path, full_path);

    char *last_dir = strrchr(new_base_path, '/');
    if(last_dir) {
        *last_dir = '\0';
    }
    else {
        strcpy(new_base_path, ".");
    }

    while (fgets(read_line, MAX_PER_LINE_LENGTH, file_p)) {

        line_count++;

        line_count += lm_parser_file_line_preprocess(file_p, read_line);

        if (lm_parser_is_skip_line(read_line)) {
            if(macro && macro->choice.count == 0) {
                goto macro_err;
            }
            macro = NULL;
            continue;
        }

        char *sub_file = lm_parser_prompt_is_include(read_line);
        if(sub_file != NULL && strcmp(sub_file, "lm_ok") == 0) {
            continue;
        }

        if(sub_file != NULL && strcmp(sub_file, "syntax_error") == 0) {
            goto syntax_err;
        }

        if(sub_file != NULL) {
            uint64_t file_pos = ftell(file_p);
            fclose(file_p);

            int sub_ret = lm_parser_lm_file(new_base_path, sub_file);
            if(sub_ret != LM_OK) {
                goto exit;
            }
            macro = NULL;

            file_p = fopen(full_path, "r");
            fseek(file_p, file_pos, SEEK_SET);

            continue;
        }

        if(macro != NULL) {
            if(lm_parser_macro_set_prompt(macro, read_line) == LM_ERR) {
                LM_LOG_ERROR("file: %s:%d, %s", full_path, line_count, error_msg);
                goto exit;
            }

            if(macro->choice.count == 0) {
                goto macro_err;
            }

            lm_parser_err_e val_ret = lm_parser_macro_set_value(macro);
            if(val_ret == LM_PARSER_INVALID_VALUE) {
                lm_parser_macro_choice_helper(full_path, line_count, macro);
                goto exit;
            }
            else if(val_ret == LM_PARSER_INVALID_DEPEND) {
                goto syntax_err;
            }
            else if(val_ret == LM_PARSER_INVALID_MACRO) {
                LM_LOG_ERROR("file: %s:%d, %s not found", full_path, line_count, error_msg);
                goto exit;
            }

            continue;
        }

        lm_parser_err_e key_ret = lm_parser_lm_file_key_string(new_base_path, read_line);
        if(key_ret == LM_PARSER_OK) {
            macro = NULL;
            continue;
        }
        else if(key_ret == LM_PARSER_SYNTAX) {
            goto syntax_err;
        }

        lm_macro_t *tmp_macro = lm_parser_prompt_is_macro(&macro_head, read_line);
        if(tmp_macro != NULL) {
            macro = tmp_macro;
        }
        else {
            goto syntax_err;
        }
    }

    if(lm_parser_macro_set_value(macro) == LM_PARSER_INVALID_VALUE) {
        char *value_p = lm_parser_get_macro_value(&config_head, macro);
        LM_LOG_ERROR("file: %s, %s: %s value is invalid", config_file, macro->name, value_p);
        goto exit;
    }

    fclose(file_p); // close file
    lm_free(read_line);
    lm_free(full_path);
    lm_free(new_base_path);
    return LM_OK;

syntax_err:
    LM_LOG_ERROR("file: %s:%d, invalid syntax", full_path, line_count);
    fclose(file_p); // close file
    lm_free(read_line);
    return LM_ERR;

macro_err:
    LM_LOG_ERROR("file: %s:%d, missing 'choice' attribute", full_path, line_count);
    fclose(file_p); // close file
    lm_free(read_line);
    return LM_ERR;

exit:
    fclose(file_p); // close file
    lm_free(read_line);
    return LM_ERR;
}


void lm_parser_print_macro_list(void)
{
    lm_macro_print_all(stdout, &macro_head);
}


void lm_parser_print_config_list(void)
{
    lm_macro_print_all(stdout, &config_head);
}


void lm_parser_print_path_list(void)
{
    lm_array_print(stdout, &lm_parser_list.path_list);
}


void lm_parser_print_define_list(void)
{
    lm_array_print(stdout, &lm_parser_list.define_list);
}


void lm_parser_print_option_list(void)
{
    lm_array_print(stdout, &lm_parser_list.ldflag_list);
}


void lm_parser_print_src_list(void)
{
    lm_array_print(stdout, &lm_parser_list.src_list);
}


bool lm_parser_lds_is_empty(void)
{
    return lm_parser_list.lds_list.count == 0;
}


lm_macro_head_t* lm_parser_get_macro_head(void)
{
    return &macro_head;
}


lm_macro_head_t* lm_parser_get_config_head(void)
{
    return &config_head;
}


int lm_parser_get_parser_list_count(void)
{
    return sizeof(lm_parser_list);
}


struct lm_parser_list* lm_parser_get_parser_list_head(void)
{
    return &lm_parser_list;
}


char* lm_parser_get_parser_list_name(int index)
{
    return lm_parser_list_name[index];
}
