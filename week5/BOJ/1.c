
#include <stdlib.h>
#include <stdio.h>

int* min_degree;

struct TREE
{
	struct NODE* root; // root�� ���� �ּҸ� �������ش�
};

typedef struct NODE
{
	struct NODE* c;
	int* key; // Ű
	int N;    // ����
	int leaf; // ����
} Node;

struct NODE* createNode(int leaf)
{
	Node* newNode;
	Node* c = calloc(3 * 2 - 1, sizeof(int)); // �迭�����
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