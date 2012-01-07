#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef struct lnode
{
  struct lnode* prev; /* Pointer to previous node */
  struct lnode* next; /* Pointer to next node */
  void* data; /* User data */
} node;

static node* create_node(void* data);
static void remove_node(node* n, list_op free_func);

/** create_list
  *
  * Creates a list by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head to NULL.
  *
  * @return an empty linked list
  */
list* create_list(void)
{
    list *ret = (list*)malloc(sizeof(list));
    ret->size = 0;
    ret->head = NULL;
    return ret;
}

/** create_node
  *
  * Helper function that creates a node by allocating memory for it on the heap.
  * Be sure to set its pointers to NULL.
  *
  * @param data a void pointer to data the user wants to store in the list
  * @return a node
  */
static node* create_node(void* data)
{
    node *ret = (node*)malloc(sizeof(node));
    ret->data = data;
    ret->next = NULL;
    ret->prev = NULL;
    return ret;
}

/** push_front
  *
  * Adds the data to the front of the linked list
  *
  * @param llist a pointer to the list.
  * @param data pointer to data the user wants to store in the list.
  */
void push_front(list* llist, void* data)
{
    node *new_front = create_node(data);
    if(llist->size > 0) {
        node *old_head = llist->head;
        node *tail = old_head->prev;
        new_front->prev = tail;
        new_front->next = old_head;
        old_head->prev = new_front;
        tail->next = new_front;
    }
    else {
        new_front->next = new_front;
        new_front->prev = new_front;
    }
    llist->head = new_front;
    llist->size++;
}

/** push_back
  *
  * Adds the data to the back/end of the linked list
  *
  * @param llist a pointer to the list.
  * @param data pointer to data the user wants to store in the list.
  */
void push_back(list* llist, void* data)
{
    node *new_back = create_node(data);
    if(llist->size == 0) {
        llist->head = new_back;
        new_back->next = new_back;
        new_back->prev = new_back;
    }
    else {
        node *old_back = llist->head->prev;
        new_back->prev = old_back;
        new_back->next = llist->head;
        llist->head->prev = new_back;
        old_back->next = new_back;
    }
    llist->size++;
}

/** remove_node
  *
  * Removes the given node by fixing the pointers on either side of it and then freeing it with the given free_func
  *
  * @param n the node to remove.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  */
static void remove_node(node* n, list_op free_func) {
    n->prev->next = n->next;
    n->next->prev = n->prev;
    free_func(n->data);
    free(n);
}

/** remove_front
  *
  * Removes the node at the front of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed (which is only there are no elements) 0 if the remove succeeded.
  */
int remove_front(list* llist, list_op free_func)
{
    if(llist->size == 0)
        return -1;
    node *n = llist->head;
    if(llist->size == 1) {
        free_func(n->data);
        free(n);
        llist->head = NULL;
    }
    else {
        llist->head = n->next;
        remove_node(n, free_func);
    }
    llist->size--;
    return 0;
}

/** remove_index
  *
  * Removes the indexth node of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param index index of the node to remove.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed 0 if the remove succeeded.
  */
int remove_index(list* llist, int index, list_op free_func)
{
    if(llist->size == 0 || index >= llist->size)
        return -1;
    if(llist->size == 1 || index == 0)
        return remove_front(llist, free_func);
    node *n = llist->head;
    for(;index > 0; index--)
        n = n->next;
    remove_node(n, free_func);
    llist->size--;
    return 0;
}

/** remove_back
  *
  * Removes the node at the back of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed 0 if the remove succeeded.
  */
int remove_back(list* llist, list_op free_func)
{
    if(llist->size == 0)
        return -1;
    node *n = llist->head->prev;
    if(llist->size == 1) {
        free_func(n->data);
        free(n);
        llist->head = NULL;
    }
    else {
        remove_node(n, free_func);
    }
    llist->size--;
    return 0;
}

/** remove_data
  *
  * Removes ALL nodes whose data is EQUAL to the data you passed in or rather when the comparison function returns true (!0)
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list
  * @param data data to compare to.
  * @param compare_func a pointer to a function that when it returns true it will remove the element from the list and do nothing otherwise @see equal_op.
  * @param free_func a pointer to a function that is responsible for freeing the node's data
  * @return the number of nodes that were removed.
  */
int remove_data(list* llist, const void* data, equal_op compare_func, list_op free_func)
{
    if(llist->size == 0)
        return 0;
    int i, count, size;
    node *cur = llist->head;
    count = 0;
    size = llist->size; //the size at the start
    for(i = 0; i < size; i++) {
        if(compare_func(data, cur->data)) {
            node *rm = cur;
            cur = cur->next;
            count++;
            if(rm == llist->head) {
                remove_front(llist, free_func);
            }
            else {
                remove_node(rm, free_func);
                llist->size--;
            }
        }
        else
            cur = cur->next;
    }
    return count;
}

/** remove_if
  *
  * Removes all nodes whose data when passed into the predicate function returns true
  *
  * @param llist a pointer to the list
  * @param pred_func a pointer to a function that when it returns true it will remove the element from the list and do nothing otherwise @see list_pred.
  * @param free_func a pointer to a function that is responsible for freeing the node's data
  * @return the number of nodes that were removed.
  */
int remove_if(list* llist, list_pred pred_func, list_op free_func)
{
    if(llist->size == 0)
        return 0;
    int i, count, size;
    node *cur = llist->head;
    count = 0;
    size = llist->size; //the size at the start
    for(i = 0; i < size; i++) {
        if(pred_func(cur->data)) {
            node *rm = cur;
            cur = cur->next;
            count++;
            if(rm == llist->head) {
                remove_front(llist, free_func);
            }
            else {
                remove_node(rm, free_func);
                llist->size--;
            }
        }
        else
            cur = cur->next;
    }
    return count;
}

/** front
  *
  * Gets the data at the front of the linked list
  * If the list is empty return NULL.
  *
  * @param llist a pointer to the list
  * @return The data at the first node in the linked list or NULL.
  */
void* front(list* llist)
{
    return llist->head->data;
}

/** get_index
  *
  * Gets the data at the indexth node of the linked list
  * If the list is empty or if the index is invalid return NULL.
  *
  * @param llist a pointer to the list
  * @return The data at the indexth node in the linked list or NULL.
  */
void* get_index(list* llist, int index)
{
    if(llist->size == 0 || index >= llist->size)
        return NULL;
    node *ret = llist->head;
    for(;index > 0; index--)
        ret = ret->next;
    return ret->data;
}

/** back
  *
  * Gets the data at the "end" of the linked list
  * If the list is empty return NULL.
  *
  * @param llist a pointer to the list
  * @return The data at the last node in the linked list or NULL.
  */
void* back(list* llist)
{
    if(llist->size == 0)
        return NULL;
    return llist->head->prev->data;
}

/** is_empty
  *
  * Checks to see if the list is empty.
  *
  * @param llist a pointer to the list
  * @return 1 if the list is indeed empty 0 otherwise.
  */
int is_empty(list* llist)
{
    return (llist->size == 0);
}

/** size
  *
  * Gets the size of the linked list
  *
  * @param llist a pointer to the list
  * @return The size of the linked list
  */
int size(list* llist)
{
    return llist->size;
}

/** find_occurence
  *
  * Tests if the search data passed in is in the linked list.
  *
  * @param llist a pointer to a linked list.
  * @param search data to search for the occurence.
  * @param compare_func a pointer to a function that returns true if two data items are equal @see equal_op.
  * @return 1 if the data is indeed in the linked list 0 otherwise.
  */
int find_occurrence(list* llist, const void* search, equal_op compare_func)
{
    if(llist->size == 0)
        return 0;
    node *cur = llist->head;
    do {
        if(compare_func(search,cur->data))
            return 1;
        cur = cur->next;
    } while(cur != llist->head);
    return 0;
}

/** empty_list
  *
  * Empties the list after this is called the list should be empty.
  *
  * @param llist a pointer to a linked list.
  * @param free_func function used to free the node's data.
  *
  */
void empty_list(list* llist, list_op free_func)
{
    /// @todo Implement
    /// @note Free all of the nodes not the linked list itself.
    /// @note do not free llist.
    node *cur = llist->head;
    while(llist->size > 0) {
        node *rm = cur;
        cur = cur->next;
        free_func(rm->data);
        free(rm);
        llist->size--;
    }
    llist->head = NULL;
}

/** traverse
  *
  * Traverses the linked list calling a function on each node's data.
  *
  * @param llist a pointer to a linked list.
  * @param do_func a function that does something to each node's data.
  */
void traverse(list* llist, list_op do_func)
{
    if(llist->size > 0) {
        node *cur = llist->head;
        do {
            do_func(cur->data);
            cur = cur->next;
        } while(cur != llist->head);
    }
}
