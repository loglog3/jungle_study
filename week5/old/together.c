
#include <stdlib.h>
#include <stdio.h>
// Node *c = (Node *)calloc(3 * 2 - 1, sizeof(Node *)); // 배열만들기
// Node *c = calloc(3 * 2 - 1, sizeof(Node)); // 위에꺼랑 이거랑 차이가 뭘까.
// newNode->key = (int)calloc(3 * 2 - 1, sizeof(int));

int min_degree;

// root로 가는 주소를 지정해준다
struct TREE
{
  struct NODE *root;
};

typedef struct NODE
{
  struct NODE *c;
  int *key; // 키
  int N;    // 갯수
  int leaf; // 리프
} Node;

Node *createNode(int leaf)
{
  Node *newNode;
  int max = ((int)min_degree) * 2 - 1;
  newNode->key = malloc(max * sizeof(int));
  // 할당 확인하기
  newNode->key[0] = 1;
  newNode->key[1] = 10;
  newNode->key[2] = 100;

  printf("%d", newNode->key[0]);
  printf("%d", newNode->key[1]);
  printf("%d", newNode->key[2]);

  Node *c = malloc(max * sizeof(Node)); // 배열만들기
  newNode->c = c;
  Node *c1;

  printf("%d", newNode->key[0]);
  printf("%d", newNode->key[1]);
  printf("%d", newNode->key[2]);

  newNode->leaf = leaf;
  return newNode;
};

int main()
{
  scanf("%d", &min_degree);
  struct TREE *a = malloc(sizeof(struct TREE));
  // createTree(a);
  createNode(0);

  return 0;
}