#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 링크드 리스트 노드
 * 
 */
typedef struct Node {
    void* data;         // 유연하게 데이터를 받기위해 void 타입 사용
    struct Node* next; 
} Node;

/**
 * @brief 링크드 리스트 객체
 * 
 */
typedef struct {
    Node* head;
    Node* tail;
    int maxSize;
    int listSize;
} LinkedList;

/**
 * @brief 링크드 리스트 생성
 * 
 * @param list      링크드 리스트 구조체
 * @param maxSize   링크드 리스트 최대 값
 * @return int      0 실패 1 성공
 */
int linkedListCreate(LinkedList* list, int maxSize);

/**
 * @brief 링크드 리스트에 값 삽입
 * 
 * @param list  링크드 리스트 구조체
 * @param data  삽입할 데이터
 * @return int  0 실패 1 성공
 */
int inputNode(LinkedList* list, void* data);

/**
 * @brief 링크드 리스트 노드 탐색
 * 
 * @param list          링크드 리스트 구조체
 * @param searchLogic   탐색 로직 콜백 (사용자가 구현해야 함)
 * @param findData      찾을 데이터
 * @param result        찾은 결과
 * @return int          0 결과 없음 1 결과 있음
 */
int searchNode(LinkedList* list, int (*searchLogic)(void*, void*)
    , void* findData, void** result);

/**
 * @brief 링크드 리스트 노드 삭제
 * 
 * @param list          링크드 리스트 구조체
 * @param searchLogic   탐색 로직 콜백 (사용자가 구현해야 함)
 * @param removeData    찾을 데이터
 * @return int          0 실패 1 성공
 */
int deleteNode(LinkedList* list, int* (*searchLogic)(void*, void*), void* removeData);

#endif // __LINKEDLIST_H__