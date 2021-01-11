// 1. struct 만들기(노드 struct, 트리 struct) t=2, maxkey=5, maxchild=6, minkey= 2, minchild=3,
// head노드 만들어놓기
// 1.1 함수 createNode
// 2. 함수 search 만들기
// 3. 함수 treeCreate
// 4. 함수 childSplit
// 4.1 함수 insert
// 4.2 함수 insertNonfull
//

#include <stdio.h>
#include <stdlib.h>

struct TREE {
    struct NODE *root;  // root로 가는 주소를 지정해준다
};

typedef struct NODE {
    // 포인터 배열 -> 배열로
    // struct NODE *c0;
    struct NODE c[6];
    int key[5];
    int N;     // 노드의 key의 갯수
    int Leaf;  // 리프노드 여부 0(내부노드)/1(리프노드)
} Node;

int createTree(struct TREE *T) {
    Node *x = (Node *)malloc(sizeof(Node));  // x = ALLOCATE-NODE()
    x->Leaf = 1;                             // x.leaf = TRUE
    x->N = 0;                                // x.n = 0
    T->root = x;
}

int searchKey(struct NODE *x, int k)  // 이렇게 x의 주소를불러올수있나?...
{
    int i = 0;
    Node *horse = x;

    while (horse != NULL) {
        if (horse->key[1] == k)
            break;
        h
    }

    whlie(i <= (x->n) && k > x->key[i])  //  x->n 이렇게 값 불러오는게 맞나?
    {
        i++;
    }

    if (i <= x->n and key == x->key[i]) {
        // 순서쌍 어떻게하지?
        return (x, i)
    } else if (x->leaf) {  // x->leaf로 이렇게 불러오는거 맞나?
        return NULL
    }
    elseif(x->leaf) { return NULL }
    else {
        return searchKey(x->c[i], k)} {
        return search(x, key)
    }
}

int main() {
    struct TREE *a = malloc(sizeof(struct TREE));
    createTree(a);
    return 0;
}