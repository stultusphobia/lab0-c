#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));

    /* According to malloc(size_t size):
     * If the function failed to allocate the
     * requested block of memory, a null pointer is returned.
     */
    if (new == NULL)
        return NULL;

    /* Macro defined in list.h */
    INIT_LIST_HEAD(new);
    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    element_t *entry = NULL, *safe = NULL;

    /* Macro defined in list.h,
     * iterate over list entries and allow deletes
     * delete all elements
     */
    list_for_each_entry_safe (entry, safe, l, list) {
        list_del(&entry->list);
        free(entry);
    }

    free(l);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;

    element_t *new = malloc(sizeof(element_t));
    if (new == NULL)
        return false;

    new->value = strdup(s);
    if (new->value == NULL) {
        /* If space of string cannot be allocated, free the node. */
        free(new);
        return false;
    }

    /* Defined in list.h */
    list_add(&new->list, head);

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;

    element_t *new = malloc(sizeof(element_t));
    if (new == NULL)
        return false;

    new->value = strdup(s);
    if (new->value == NULL) {
        /* If space of string cannot be allocated, free the node. */
        free(new);
        return false;
    }

    /* Defined in list.h */
    list_add_tail(&new->list, head);

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;

    element_t *rm = list_first_entry(head, element_t, list);

    strncpy(sp, rm->value, bufsize - 1);

    *(sp + bufsize - 1) = '\0';

    list_del(&rm->list);
    return rm;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;

    element_t *rm = list_last_entry(head, element_t, list);

    strncpy(sp, rm->value, bufsize - 1);

    *(sp + bufsize - 1) = '\0';

    list_del(&rm->list);
    return rm;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    return -1;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    /* No effect if q is NULL or empty or only contains one element. */
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return false;

    /* Fast pointer moves double the speed of slow pointer,
     * when fast pointer reaches the end (head or head->next),
     * slow pointer will be at the middle of the list.
     * (Floyd’s Cycle detection)
     */
    struct list_head *slow = head->next, *fast = head->next->next;

    while (fast != head && fast != head->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_del(slow);
    /* free(list_entry(slow, element_t, list)); does not work!
     * Need to free value (*char) in element_t, too.
     */
    q_release_element(list_entry(slow, element_t, list));

    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    /* No effect if q is NULL or empty or only contains one element. */
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *node = head, *buff = NULL;
    do {
        buff = node->next;
        node->next = node->prev;
        node->prev = buff;
        node = buff;
    } while (node != head);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head) {}
