/* source/lm_string.h
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

#ifndef __LM_STRING_H__
#define __LM_STRING_H__

#include "lm_list.h"


#ifdef __cplusplus
extern "C" {
#endif


int lm_str_find_char(char* str, char ch);
bool lm_str_is_all_space(char* str);
int lm_str_find_str(char* str, char* substr);
int lm_str_find_str_space(char* str, char* substr);
int lm_str_num_str_space(char* str);
char* lm_str_get_quote(char* str);
char* lm_str_pick_str(char* str, int index);
char* lm_str_delete_space(char* str);
void lm_str_delete_tail_space(char* str);
char* lm_str_delete_head_tail_space(char* str);
bool lm_str_head_is_four_space(char* str);
bool lm_str_head_is_eight_space(char* str);
int lm_str_dupli_string(char **lm_str, char *str);
long long lm_str_to_int(char *str);
int lm_str_num_of_substr_split(char *str);


#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif //! __LM_STRING_H__