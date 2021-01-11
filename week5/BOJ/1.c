
#include <stdlib.h>
#include <stdio.h>

int* min_degree;

struct TREE
{
	struct NODE* root; // root로 가는 주소를 지정해준다
};

typedef struct NODE
{
	struct NODE* c;
	int* key; // 키
	int N;    // 갯수
	int leaf; // 리프
} Node;

struct NODE* createNode(int leaf)
{
	Node* newNode;
	Node* c = calloc(3 * 2 - 1, sizeof(int)); // 배열만들기
	newNode->key = (*int)calloc(3 * 2 - 1, sizeof(int));
	newNode->c = c;
	newNode->leaf = leaf;
};

int main()
{
	scanf_s("%d", &min_degree);
	struct TREE* a = malloc(sizeof(struct TREE));
	createTree(a);
	createNode(0);

	return 0;
}