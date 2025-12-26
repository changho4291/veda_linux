#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int maxSize;
    int listSize;
} LinkedList;

int linkedListCreate(LinkedList* list, int maxSize);

int inputNode(LinkedList* list, void* data);

int searchNode(LinkedList* list, int (*searchLogic)(void*, void*)
    , void* findData, void** result);

int deleteNode(LinkedList* list, int* (*searchLogic)(void*, void*), void* removeData);

#endif // __LINKEDLIST_H__