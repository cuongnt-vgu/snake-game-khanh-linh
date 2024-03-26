#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delete_list(node_t* head_list) {
    node_t* curr = head_list;
    while (curr != NULL) {
        node_t* temp = curr;
        curr = curr->next;
        free(temp->data);
        free(temp);
    }
}

// int length_list(node_t* head_list) {
//     int count = 0;
//     node_t* curr = head_list;
//     while (curr != NULL) {
//         count++;
//         curr = curr->next;
//     }
//     return count;
// }

// void* get_first(node_t* head_list) {
//     if (head_list == NULL) {
//         return NULL;
//     }
//     return head_list->data;
// }

// void* get_last(node_t* head_list) {
//     if (head_list == NULL) {
//         return NULL;
//     }
//     node_t* curr = head_list;
//     while (curr->next != NULL) {
//         curr = curr->next;
//     }
//     return curr->data;
// }

// void insert_first(node_t** head_list, void* to_add, size_t size) {
//     node_t* new_node = malloc(sizeof(node_t));
//     new_node->data = malloc(size);
//     memcpy(new_node->data, to_add, size);
//     new_node->next = *head_list;
//     new_node->prev = NULL;
//     if (*head_list != NULL) {
//         (*head_list)->prev = new_node;
//     }
//     *head_list = new_node;
// }

// void insert_last(node_t** head_list, void* to_add, size_t size) {
//     node_t* new_node = malloc(sizeof(node_t));
//     new_node->data = malloc(size);
//     // memcpy(new_node->data, to_add, size);
//     new_node->next = NULL;
//     if (*head_list == NULL) {
//         new_node->prev = NULL;
//         *head_list = new_node;
//         return;
//     }
//     node_t* curr = *head_list;
//     while (curr->next != NULL) {
//         curr = curr->next;
//     }
//     curr->next = new_node;
//     new_node->prev = curr;
// }

// void* get(node_t* head_list, int index) {
//     if (index < 0 || index >= length_list(head_list)) {
//         return NULL;
//     }
//     node_t* curr = head_list;
//     for (int i = 0; i < index; i++) {
//         curr = curr->next;
//     }
//     return curr->data;
// }

// int remove_element(node_t** head_list, void* to_remove, size_t size) {
//     if (*head_list == NULL) {
//         return 0;
//     }
//     node_t* curr = *head_list;
//     while (curr != NULL) {
//         if (memcmp(curr->data, to_remove, size) == 0) {
//             if (curr->prev == NULL) {
//                 *head_list = curr->next;
//                 if (curr->next != NULL) {
//                     curr->next->prev = NULL;
//                 }
//             }
//             else {
//                 curr->prev->next = curr->next;
//                 if (curr->next != NULL) {
//                     curr->next->prev = curr->prev;
//                 }
//             }
//             free(curr->data);
//             free(curr);
//             return 1;
//         }
//         curr = curr->next;
//     }
//     return 0;
// }

// void reverse_helper(node_t** head_list) {
//     if (*head_list == NULL) {
//         return;
//     }
//     node_t* curr = *head_list;
//     node_t* temp = NULL;
//     while (curr != NULL) {
//         temp = curr->prev;
//         curr->prev = curr->next;
//         curr->next = temp;
//         curr = curr->prev;
//     }
//     if (temp != NULL) {
//         *head_list = temp->prev;
//     }
// }

// void reverse(node_t** head_list) {
//     if (*head_list == NULL) {
//         return;
//     }
//     node_t* curr = *head_list;
//     while (curr->next != NULL) {
//         curr = curr->next;
//     }
//     *head_list = curr;
//     reverse_helper(head_list);
// }

// void* remove_first(node_t** head_list) {
//     if (*head_list == NULL) {
//         return NULL;
//     }
//     node_t* curr = *head_list;
//     void* data = curr->data;
//     *head_list = curr->next;
//     if (curr->next != NULL) {
//         curr->next->prev = NULL;
//     }
//     free(curr);
//     return data;
// }

// void* remove_last(node_t** head_list) {
//     if (*head_list == NULL) {
//         return NULL;
//     }
//     node_t* curr = *head_list;
//     while (curr->next != NULL) {
//         curr = curr->next;
//     }
//     void* data = curr->data;
//     if (curr->prev == NULL) {
//         *head_list = NULL;
//     }
//     else {
//         curr->prev->next = NULL;
//     }
//     free(curr);
//     return data;
// }