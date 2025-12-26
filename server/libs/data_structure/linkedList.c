#include "linkedList.h"

int linkedListCreate(LinkedList* list, int maxSize) {
    list->head = NULL;
    list->tail = NULL;
    list->maxSize = maxSize;
    list->listSize = 0;
}

int inputNode(LinkedList* list, void* data) {
    Node* tmp = (struct Node*)malloc(sizeof(Node));
    if (tmp == NULL) {
        perror("malloc");
        return 0;
    }
    tmp->next = NULL;
    tmp->data = data;
    if (list->head == NULL) {
        list->head = list->tail = tmp;
    } else {
        list->tail->next = tmp;
        list->tail = tmp;
    }
    list->listSize++;
} 

int searchNode(LinkedList* list, int (*searchLogic)(void*, void*)
    , void* findData, void** result) {
    Node* tmp = list->head;

    // searchLogic 을 통해 받은 함수로 탐색을 수행
    while (tmp) {
        if (searchLogic(findData, tmp->data)) { 
            *result = tmp->data;
            return 1;
        }
        tmp = tmp->next;
    }

    // 찾고자 하는 원소가 존재하지 않음
    return 0;
}

int deleteNode(LinkedList* list, int* (*searchLogic)(void*, void*), void* removeData) {
    Node* temp = list->head;
    Node* btemp = list->head;
    Node* ctemp;

    int flag = 0;	// 삭제 안되면 0 삭제 된적이 있으면 1

    // 삭제할 노드를 리스트를 순회 하며 탐색
    while (temp) {
        if (searchLogic(removeData, temp->data)) {
            if (temp == list->head) {           // 삭제 노드가 첫번째 노드인 경우
                list->head = list->head->next;
            } else if (temp == list->tail) {    // 마지막 노드 삭제
                btemp->next = NULL;
                list->tail = btemp;
            } else {                            // 중간 노드 삭제
                btemp->next = temp->next;
            }

            // temp는 다음 노드로 이동 btemp는 이동 할 필요 x
            ctemp = temp;
            temp = temp->next;

            free(ctemp);
            list->listSize--;
        } else {
            // 노드 이동 : btemp - 현재 노드, temp - 다음 노드로 이동
            btemp = temp;
            temp = temp->next;
        }
    }

    if (!flag) { return 0; }

    return 1;
}