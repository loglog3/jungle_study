#define _CRT_SECURE_NO_WARNINGS // sprintf 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>              // sprintf 함수가 선언된 헤더 파일
#include <stdlib.h>             // malloc, free 함수가 선언된 헤더 파일
typedef struct Node
{
  struct Node *next; // member로 Node를 가리키는 next라는 포인터를 가지고있음
  int data;
  int name;
} Node;

// 전체 다 출력
int printAll(Node *h)
{
  h = h->next;
  while (h->data)
  {
    printf("%s %d\n", "for문", h->name);
    if (h->next == NULL)
      break;
    h = h->next;
  }
  return 0;
}

int main(void)
{
  Node *head = malloc(sizeof(Node));
  Node *node1 = malloc(sizeof(Node));
  Node *node2 = malloc(sizeof(Node));

  // 연결해주기
  head->next = node1;
  node1->next = node2;
  node2->next = NULL;

  //데이터 넣기
  node1->data = 101;
  node1->name = 102;
  node2->data = 201;
  node2->name = 202;
  // 문자는 sprintf() 함수를 사용해야 한다.
  // node1->data = "157cm";
  // node1->name = "강도연";
  // node2->data = "182cm";
  // node2->name = "David";

  printf("%d\n", node1->name);
  printf("%d\n", node2->name);
  printf("%d\n", node1->data);
  printf("%d\n", node2->data);
  // printf("%s\n", node1->name);
  // printf("%s\n", node2->name);
  // printf("%s\n", node1->data);
  // printf("%s\n", node2->data);

  //전체 다 출력하기
  printAll(head);

  return 0;
}