/* source/lm_list.h
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

#ifndef __LM_LIST_H__
#define __LM_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stddef.h>


/**
 * @brief Get the offset of internal members of the structure
 * 
 * @struct_t: structure typedef 
 * @member: member in structure
 * 
 * @return offset of member in the structure
*/
#define   lm_offsetof(struct_t, member)      \
               ((size_t)&((struct_t*)0)->member)


/**
 * @brief Get the address of the structure instance.
 *
 * @ptr: address of the structure member.
 * @type: type of the structure.
 * @member: member name of the ptr in structure.
 *   
 * @return pointer to address of structure 
 */
#define container_of(ptr, type, member)      ({ \
               (type *)((char *)ptr - lm_offsetof(type, member)); })





/**
 * @prev: previous node of the list.
 * @next: next node of the list.
 */
typedef struct lm_list_node {
	struct lm_list_node *prev;
	struct lm_list_node *next;
}lm_list_node_t;

#define LM_LIST_HEAD(name) \
	lm_list_node_t name = {.prev = &(name), .next = &(name)}


/**
 * @brief foreach the list.
 *
 * @pos: the &struct list_head to use as a loop cursor.
 * @list_head: the head for your list.
 * 
 * @return none
 */
#define lm_list_for_each(pos, list_head) \
	for (pos = (list_head)->next; pos != (list_head); pos = pos->next)


/**
 * @brief Return the next entry of specific node.
 *
 * @entry: specific entry.
 *
 * @return entry_type: next entry of specific entry.
 */
#define lm_list_next_entry(entry, entry_type, list_node_member) \
	container_of(entry->list_node_member.next, entry_type, list_node_member)


/**
 * @brief Return the previous entry of specific node.
 *
 * @entry: specific entry.
 *
 * @return entry_type: previous entry of specific entry.
 */
#define lm_list_prev_entry(entry, entry_type, list_node_member) \
	container_of(entry->list_node_member.prev, entry_type, list_node_member)


/**
 * @brief foreach the list inserted in a structure.
 *
 * @pos: the &struct list_head to use as a loop cursor.
 * @list_head: the head for your list.
 * @entry_type: type of the struct.
 * @list_node_member: member name of the list_node in structure.
 *
 * @return none
 */
#define lm_list_for_each_entry(pos, list_head, entry_type, list_node_member) \
	for (pos = container_of((list_head)->next, entry_type, list_node_member); \
	     &pos->list_node_member != (list_head); \
	     pos = container_of(pos->list_node_member.next, entry_type, list_node_member))


/**
 * @brief inititialize a list.
 *
 * @node: inserted node.
 *
 * @return none
 */
static inline void lm_list_init(lm_list_node_t *node) 
{
	node->next = node;
	node->prev = node;
}


/**
 * @brief Add a node to the list tail.
 *
 * @head: head node of the list.
 * @node: inserted node.
 * 
 * @return none
 */
static inline void lm_list_add_node_at_tail(lm_list_node_t *head, lm_list_node_t *node)
{
	lm_list_node_t *tail = head->prev;

	node->prev = tail;
	node->next = head;
	tail->next = node;
	head->prev = node;
}


/**
 * @brief Add a node to the list front.
 *
 * @head: head node of the list.
 * @node: inserted node.
 * 
 * @return none
 */
static inline void lm_list_add_node_at_front(lm_list_node_t *head, lm_list_node_t *node)
{
	lm_list_node_t *front = head->next;

	node->prev = head;
	node->next = front;
	front->prev = node;
	head->next = node;
}


/**
 * @brief Delete a tail node from the list.
 *
 * @head: head node of the list.
 * 
 * @return none
 */
static inline void lm_list_del_tail_node(lm_list_node_t *head)
{
	lm_list_node_t *tail_prev = head->prev->prev;

	tail_prev->next = head;
	head->prev = tail_prev;
}


/**
 * @brief  Delete a front node from the list.
 *
 * @head: head node of the list.
 * 
 * @return none
 */
static inline void lm_list_del_front_node(lm_list_node_t *head)
{
	lm_list_node_t *front_next = head->next->next;

	front_next->prev = head;
	head->next = front_next;
}


/**
 * @brief  Delete a node from the list.
 *
 * @node: the node of the list.
 * 
 * @return none
 */
static inline void lm_list_del_node(lm_list_node_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}


/**
 * @brief Determine whether the list is empty.
 *
 * @head: head node of the list.
 * 
 * @return  bool: is empty.
 */
static inline bool lm_list_is_empty(lm_list_node_t *head)
{
	return head->next == head;
}


/**
 * @brief Return the next node of specific node.
 *
 * @node: specific node.
 *
 * @return list_node*: next node of specific node.
 */
static inline lm_list_node_t* lm_list_next_node(lm_list_node_t *node)
{
	return node->next;
}


/**
 * @brief Return the previous node of specific node.
 *
 * @node: specific node.
 * 
 * @return list_node*: previous node of specific node.
 */
static inline lm_list_node_t* lm_list_prev_node(lm_list_node_t *node)
{
	return node->prev;
}


#ifdef __cplusplus
}
#endif

#endif /* __LM_LIST_H__ */
