
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp 함수가 선언된 헤더 파일
// Node *c = (Node *)calloc(3 * 2 - 1, sizeof(Node *)); // 배열만들기
// Node *c = calloc(3 * 2 - 1, sizeof(Node)); // 위에꺼랑 이거랑 차이가 뭘까.
// newNode->key = (int)calloc(3 * 2 - 1, sizeof(int));

const int min_degree;

// 구조체
// root로 가는 주소를 지정해준다
typedef struct TREE {
    struct NODE *root;
} Tree;

typedef struct NODE {
    struct NODE **c;
    int *key;  // 키
    int N;     // 갯수
    int leaf;  // 리프
} Node;

//삽입 삭제 공통함수
Node *createNode(int leaf) {
    Node *newNode;
    int max = ((int)min_degree) * 2 - 1;
    newNode->key = malloc(max * sizeof(int));
    Node **c = (Node **)malloc(max * sizeof(Node *));  // 배열만들기
    newNode->c = c;
    newNode->leaf = leaf;
    newNode->N = 0;  // 생성노드 초기크기 0

    return newNode;
};

//삽입 함수
//삽입-나누기
void split_Child(Node *x, int i) {
    Node *z = createNode(0);
    Node *y = x->c[i];
    z->leaf = y->leaf;
    z->N = min_degree - 1;
    for (int j = 0; j < min_degree - 1; j++) {  // y의 기존키 새로운 z로 옮겨주기(1개 적게)
        z->key[j] = y->key[j + min_degree];
    };
    if (y->leaf == 0) {  // y의 기존자식 새로운 z로 옮겨주기(1개 적게)
        for (int j = 0; j < min_degree - 1; j++) {
            z->c[j] = y->c[j + min_degree];
        }
    }
    y->N = min_degree - 1;
    for (int j = x->N; j >= i; j--) {
        x->c[j + 1] = x->c[j];
    }
    x->c[i + 1] = z;
    for (int j = x->N - 1; j >= i - 1; j--) {
        x->key[j + 1] = x->key[j];
    }
    x->key[i] = y->key[min_degree];
    x->N = x->N + 1;
}
//삽입-재귀
void intert_Null(Node *x, int k) {
    int i = x->N;
    if (x->leaf) {
        while (i >= 1 && k < x->key[i]) {
            x->key[i] = x->key[i - 1];
            i = i - 1;
                }
    }
};
//삽입-초기
void insertKey(Tree *Tree, int k) {
    Node *r = Tree->root;
    if (r->N == min_degree * 2 - 1) {
        Node *s = createNode(0);
        Tree->root = s;
        s->N = 0;
        s->leaf = 0;
        s->c[0] = r;
        split_Child(s, 0);
        inert_Nonfull(s, k);
    } else {
        insert_Nonfull(r, k);
    }
}

int main() {
    //입력받기 시작
    puts("트리의 minimum degree를 입력해주세요\n");
    scanf("%d", &min_degree);
    //tree 초기화 (리프노드 = TRUE)
    Node *firstNode = createNode(1);
    struct TREE *tree;
    tree->root = firstNode;

    puts("insert 또는 delete를 입력해주세요\n");
    char command[10];
    char instert[10] = "insert";
    char delete[10] = "delete";
    scanf("%s", &command);
    int isInsert = strcmp(command, instert);
    int isDelete = strcmp(command, delete);

    if (isInsert == 0) {
        puts("넣을 key를 입력해주세요\n");
        int newkey;
        scanf("%d", &newkey);
        insertKey(tree, newkey);
    } else if (isDelete == 0) {
        puts("not yet");
    }

    return 0;
}