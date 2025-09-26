/* source/lm_string.c
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

#include "lm_string.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lm_error.h"
#include "lm_mem.h"
#include "lm_log.h"

int lm_str_find_char(char* str, char ch)
{
    int i = 0;
    while (*str) {
        if (*str == ch) {
            return i;
        }
        else {
            i++;
            str++;
        }
    }
    return LM_ERR;
}


bool lm_str_is_all_space(char* str)
{
    while (*str) {
        if (*str != ' ' && *str != 9) {
            return false;
        }
        else {
            str++;
        }
    }
    return true;
}


int lm_str_find_str(char* str, char* substr)
{
	int len1 = strlen(str);
	int len2 = strlen(substr);

	for (int i = 0; i < len1; i++) {
		int j,k = i;
		for (j = 0; j < len2; j++) {
			if (str[k++] == substr[j]) {
				continue;
			}
            else {
				break;
			}
		}
		if (j == len2) 
            return i;
	}
	return LM_ERR; 
}


int lm_str_find_str_space(char* str, char* substr)
{
    char* tmp = (char*)lm_malloc(strlen(str) + 2);
    char* tmp_free = tmp;
    strcpy(tmp, str);
    *(tmp + strlen(str)) = ' ';
    char* pos = tmp;
    bool flag = false;
    int count = 0;

    while (*tmp) {
        if (*tmp == ' ' || *tmp == 9 || *tmp == '\0') { // space or TAB key
            *tmp = '\0';
            if (*(tmp + 1) != ' ' && *(tmp + 1) != 9) {
                if (strcmp(pos, substr) == 0) {
                    lm_free(tmp_free);
                    return count;
                }
                else {
                    pos = tmp + 1;
                    if (flag) {
                        count++;
                    }
                }
            }
            else if (*(tmp + 1) == '\0') {
                if (strcmp(pos, substr) == 0) {
                    lm_free(tmp_free);
                    return count;
                }
            }
        }
        else {
            flag = true;
        }
        tmp++;
    }
    lm_free(tmp_free);
    return LM_ERR;
}


int lm_str_num_str_space(char* str)
{
    int count = 0;
    bool flag = false;
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        if(*str == '\'' || *str == '"') {
            count ++;
            str ++;

            while(*str) {
                if(*str == '\'' || *str == '"') {
                    str ++;
                    break;
                }
                str ++;
            }
        }

        if(*str == 0) {
            return count;
        }
        else if(*str != ' ' && *str != 9 ) {
            if(!flag) {
                count ++ ;
                flag = true;
            }
        }
        else {
            flag = false;
        }
        str ++;
    }
    return count;
}


char* lm_str_get_quote(char* str)
{
    char* pos, * tmp;
    int flag = 0;
    char cache[512];

    strcpy(cache, str);
    pos = cache;
    tmp = cache;
    
    while (*tmp) {
        if (*tmp == '\"') {
            if (flag) {
                *tmp = '\0';
                tmp = (char*)lm_malloc(strlen(pos) + 1);
                strcpy(tmp, pos);
                flag = 2;
                break;
            }
            else {
                flag = 1;
                pos = tmp + 1;
            }
        }
        tmp++;
    }
    if (flag != 2)
        return NULL;
    else
        return tmp;
}


char* lm_str_pick_str(char* str, int index)
{
    char *ret_str = lm_malloc(256);
    memset(ret_str, 0, 256);
    char *p = ret_str;
    int count = 0;
    bool flag = false;
    index += 1;

    int num = lm_str_num_str_space(str);

    if(num == 1 && index == 1) {
        while(*str) {
            if(*str != ' ') {
                break;
            }
            str ++;
        }

        strcpy(ret_str, str);
        return ret_str;
    }

    if(index > num) {
        return NULL;
    }

    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        if(*str == '\'' || *str == '\"') {
            str ++;
            while(*str) {
                if(*str == '\'' || *str == '\"') {
                    count ++;
                    *p = '\0';
                    p = ret_str;

                    if(index == count) {
                        return ret_str;
                    }

                    str ++;
                    break;
                }

                i++;
                *p++ = *str++;
            }
        }
    
        if(*str == 0) {
            break;
        }
        else if(*str != ' ' && *str != 9 ) {
            if(!flag) {
                flag = true;
            }

            *p++ = *str++;
        }
        else {
            if(flag) {
                flag = false;
                *p = '\0';
                p = ret_str;
                count ++ ;

                if(index == count) {
                    return ret_str;
                }
            }
            str ++;
        }
    }

    *p = '\0';
    return ret_str;
}


char* lm_str_delete_space(char* str)
{
    char* tmp = (char*)lm_malloc(strlen(str) + 1);
    char* pos = tmp;
    int size = 0;
    tmp[strlen(str)] = '\0';
    bool flag = false;

    while (*str) {
        if(*str == '\'' || *str == '\"') {
            flag = !flag;
        }

        if(!flag) {
            if (*str != ' ' && *str != 9) {
                *(pos++) = *str;
                size++;
            }
        }
        else {
            *(pos++) = *str;
            size++;
        }

        str++;
    }
    tmp[size] = '\0';
    return tmp;
}


void lm_str_delete_tail_space(char* str)
{
    char *p = &str[strlen(str) - 1];
    while(*p == ' ' && p >= str) {
        *p = '\0';
        p--;
    };
}


char* lm_str_delete_head_tail_space(char* str)
{
    char* tmp = (char*)lm_malloc(strlen(str) + 1);
    char* pos = tmp;
    tmp[strlen(str)] = '\0';

    //delete head space
    while(*str == ' ') {
        str++;
    }

    strcpy(pos, str);
    pos += (strlen(tmp) - 1);

    while (*pos == ' ') {
        *pos = 0;
        pos --;
    }

    return tmp;
}


bool lm_str_head_is_four_space(char* str)
{
    int i = 4;
    while(i) {
        if(*str != ' ')
            return false;
        i--;
        str++;
    }
    if(*str != ' ' && *str != '\0')
        return true;
    else
        return false;
}


bool lm_str_head_is_eight_space(char* str)
{
    int i = 8;
    while(i) {
        if(*str != ' ')
            return false;
        i--;
        str++;
    }
    if(*str != ' ' && *str != '\0')
        return true;
    else
        return false;
}


int lm_str_dupli_string(char **lm_str, char *str)
{
    *lm_str = lm_malloc(strlen(str) + 1);
    if(*lm_str == LM_ERR_PTR) {
        LM_LOG_ERROR("out of memory");
        lm_mem_destroy();
        exit(1);
    }

    strcpy(*lm_str, str);

    return LM_OK;
}


long long lm_str_to_int(char *str)
{
    char *end;
    long num;
    num = strtol(str, &end, 10);
	if(*end == '\0') {
        return num;
    }
    else {
        return LM_INVALID_NUM;
    }
}


int lm_str_num_of_substr_split(char *str)
{
    int count = 0;
    char *p = str;
    int len = strlen(str);

    for(int i = 0; i < len; i ++) {
        if(*p == ' ') {
            p ++;
        }
        else if(*p == '\"') {
            p ++;

            while(*p) {
                if(*p == '\"') {
                    break;
                }
                i ++;
                p++;
            }

            if(*p == '\0') {
                goto err;
            }

            count ++;
        }
        else {
            while(*p) {
                if(*p == '(') {
                    while(*p) {
                        if(*p == ')') {
                            break;
                        }
                        i ++;
                        p++;
                    }
                }

                if(*p == '\0') {
                    goto err;
                }

                if(*p == ' ') {
                    break;
                }
                i ++;

                p++;
            }

            count ++;
        }
    }

    return count;
err:
    return -1;
}
