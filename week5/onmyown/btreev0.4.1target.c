
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int min_degree;

typedef struct NODE {
    int leaf;  // 리프
    int N;     // 갯수
    int *key;  // 키
    struct NODE **c;
} Node;

// 프린트
void printAll(Node *root, int depth) {
    printf("\n");
    Node *node = root;
    if (node->leaf == 1) {
        for (int i = 1; i <= depth; i++) {
            printf("                   ");
        }
        for (int vIdx = 1; vIdx <= node->N; vIdx++) {
            printf("%6d", node->key[vIdx]);
        }
        return;
    }
    if (node->leaf == 0) {
        for (int i = 1; i <= depth; i++) {
            printf("                   ");
        }
        for (int vIdx = 1; vIdx <= node->N; vIdx++) {
            printf("%4d", node->key[vIdx]);
        }
        for (int vIdx = 1; vIdx <= node->N + 1; vIdx++) {
            printAll(node->c[vIdx], depth + 1);
        }
    }
}

//삽입 삭제 공통함수
Node *createNode(int leaf) {
    // Node *newNode; // 아래꺼와의 차이가 뭔지 물어보기
    Node *newNode = (Node *)malloc(sizeof(Node));  // 한번 newNode에 크기를 지정해보자.
    int max = ((int)min_degree) * 2;
    newNode->key = (int *)malloc((max) * sizeof(int));       // 두 번째 node를 만들 때 segmentation fault 나옴
    Node **c = (Node **)malloc((max + 1) * sizeof(Node *));  // 배열만들기
    newNode->c = c;
    newNode->leaf = (int)leaf;
    newNode->N = 0;  // 생성노드 초기크기 0

    return newNode;
};

//삽입 함수
//삽입-나누기
void split_Child(Node **root_address, Node *x, int i) {
    Node *z = createNode(0);  //일단 리프노드는 아니고
    Node *y = x->c[i];
    z->leaf = y->leaf;  // y가 리프노드이면 z도 리프노드다
    z->N = min_degree - 1;
    for (int j = 1; j <= min_degree - 1; j++) {  // y의 기존키 새로운 z로 옮겨주기(1개 적게)
        z->key[j] = y->key[j + min_degree];
    };
    if (y->leaf == 0) {  // y의 기존자식 새로운 z로 옮겨주기(1개 적게)
        for (int j = 1; j <= min_degree; j++) {
            z->c[j] = y->c[j + min_degree];
        }
    }
    y->N = min_degree - 1;
    for (int j = x->N + 1; j >= i + 1; j--) {
        x->c[j + 1] = x->c[j];
    }
    x->c[i + 1] = z;
    for (int j = x->N; j >= i - 1; j--) {
        x->key[j + 1] = x->key[j];
    }
    x->key[i] = y->key[min_degree];
    x->N = x->N + 1;
}
//삽입-재귀
void insert_Nonfull(Node **root_address, Node *x, int k) {
    int i = x->N;
    if (x->leaf) {
        while (i >= 1 && k < x->key[i]) {
            x->key[i + 1] = x->key[i];
            i = i - 1;
        }
        x->key[i + 1] = k;
        x->N = x->N + 1;

    } else {
        while (i >= 1 && k < x->key[i]) {
            i = i - 1;
        }
        i = i + 1;
        if (x->c[i]->N == 2 * min_degree - 1) {
            split_Child(root_address, x, i);
            if (k > x->key[i]) i = i + 1;
        }
        insert_Nonfull(root_address, x->c[i], k);
    }
};
//삽입-main
void insertKey(Node **root_address, Node *root, int k) {
    Node *r = (Node *)root;
    if (r->N == min_degree * 2 - 1) {
        Node *s = createNode(0);  //새로운 노드는 루트가 되고 리프가 아니다! 고로 0
        *root_address = s;
        s->N = 0;
        s->leaf = 0;
        s->c[1] = r;
        // s->c[1]->leaf = 1;  // c[1]에게 leaf=1을 해줘야하나? (확인필요)
        split_Child(&s, s, 1);  //쪼개지는 아이 (split_child(부모, i 번째 자식이 왼쪽자식이 된다.))
        insert_Nonfull(&s, s, k);
    } else {
        insert_Nonfull(&r, r, k);
    }
}

//삭제 함수
//선행키찾기
int Pred(Node *x) {
    if (x->leaf == 1) {
        int newKey = x->key[x->N];
        return newKey;
    }

    int i = (x->N) + 1;
    x = x->c[i];
    Pred(x);
}
//후행키찾기
int Succ(Node *x) {
    if (x->leaf == 1) {
        int newKey = x->key[1];
        return newKey;
    }
    x = x->c[1];  // 한단계 더 깊이, 근데 1이 아니다.
    Succ(x);
}

void delete_Key(Node **root_address, Node *x, int target) {
    //case 1
    if (x->leaf == 1) {
        for (int i = 1; i <= x->N; i++) {
            if (x->key[i] == target) {
                for (int j = i; j < x->N; j++) {
                    x->key[j] = x->key[j + 1];  // 삭제단계1 값을 하나씩 끌어온다
                }
                x->N = (x->N) - 1;  // 삭제단계2 크기를 줄여준다
                return;
            }
        }
    }
    // case 2-3
    int target_leftIdx = 1;
    while (x->key[target_leftIdx] < target && target_leftIdx <= x->N) {  // x거 리프 아닐경우 Prom`sing 자식 찾기, x의 N보다 i가 작아야한다 (해결)
        target_leftIdx = target_leftIdx + 1;
    }

    // case 2 - x 내부에 k 존재하는 경우
    if (x->key[target_leftIdx] == target) {  // x 내부에 k가 있음
        // case 2-a (왼쪽자식 여유)
        if (x->c[target_leftIdx]->N >= min_degree) {
            int k_ = Pred(x->c[target_leftIdx]);
            x->key[target_leftIdx] = k_;
            delete_Key(root_address, x->c[target_leftIdx], k_);
            return;
        }
        // case 2-b (오른쪽자식 여유)
        if (x->c[target_leftIdx + 1]->N >= min_degree) {
            int k_ = Succ(x->c[target_leftIdx + 1]);  // 후행키찾기 잘못됨?... 여유있는 오른쪽자식으로 들어간다
            x->key[target_leftIdx] = k_;
            delete_Key(root_address, x->c[target_leftIdx + 1], k_);
            return;
        }
        // case 2-c (자식여유X, 병합하기) // (수정필요/ 왼쪽자식삭제오류 -> 해결 3-b 오른쪽자식문제였음)
        Node *y = x->c[target_leftIdx];
        Node *z = x->c[target_leftIdx + 1];
        int m = y->N;
        int p = z->N;
        y->key[m + 1] = target;
        for (int j = 1; j <= p; j++) {
            y->key[m + 1 + j] = z->key[j];
        }
        y->N = m + p + 1;
        for (int j = target_leftIdx; j <= (x->N); j++) {  //부모노드 k, c 조정 (수정해봄)
            x->c[j] = x->c[j + 1];                        // child ptr 옮겨주기
        }
        for (int j = target_leftIdx; j < x->N; j++) {  // 부모 key 옮겨주기
            x->key[j] = x->key[j + 1];                 // (중요/ 다시확인, 책이랑 다른 부분/ 확인필요) // 넣는게 맞다
        }

        x->N = x->N - 1;  // 부모노드 크기조정

        if (x->N == 0) {        // 만약 x의 크기가 0이면, 지워주고 루트노드 재설정해주기
            free(x);            // 루트 삭제
            *root_address = y;  // 루트 재설정
        }

        free(z);                              // z노드 메모리 제거
        delete_Key(root_address, y, target);  // y로 간 k를 재귀적으로 삭제하기(case1)
        return;
    }
    // case 3 - k가 x보다 아래노드에 존재하는 경우
    // case 3.0 : promising child's key is enough
    if (target_leftIdx <= x->N + 1 && x->c[target_leftIdx]->N >= min_degree) {  // promising자식 여유->재귀삭제 // (수정해봄) segfaut 보호장치 (i <= x->N + 1)
        delete_Key(root_address, x->c[target_leftIdx], target);
        return;
    }
    (여기)
        // case 3-a-1 - right sibling
        if (target_leftIdx <= x->N && x->c[target_leftIdx + 1]->N >= min_degree) {  // 가장 먼저 자식차수랑 i랑 비교, promising자식 부족, 오른쪽형제 충분 (중요 확인필요)

        x->c[target_leftIdx]->N = x->c[target_leftIdx]->N + 1;                     // promising 자식의 크기를 하나 키운다
        x->c[target_leftIdx]->key[min_degree] = x->key[target_leftIdx];            // 부모의 key를 준다. promising 자식의 커진공간에 -> 제대로 들어옴
        x->key[target_leftIdx] = x->c[target_leftIdx + 1]->key[1];                 // 부모의 key는 우측자식의 1번key가 된다
        x->c[target_leftIdx]->c[min_degree + 1] = x->c[target_leftIdx + 1]->c[1];  // 충분형제에서 부족으로 자식포인터 이동
        for (int j = 1; j < x->c[target_leftIdx + 1]->N; j++) {
            x->c[target_leftIdx + 1]->key[j] = x->c[target_leftIdx + 1]->key[j + 1];  //충분형제 key 당기기
        }
        for (int j = 1; j <= x->c[target_leftIdx + 1]->N; j++) {
            x->c[target_leftIdx + 1]->c[j] = x->c[target_leftIdx + 1]->c[j + 1];  // 충분형제 C 당기기
        }
        x->c[target_leftIdx + 1]->N = x->c[target_leftIdx + 1]->N - 1;  // 크기줄이기는 조심해서 해야한다
        delete_Key(root_address, x->c[target_leftIdx], target);
        return;  // 들어가는거 맞겠지? 이게 맞아?
        // case 3.a-2 - left sibling
        (여기)
    }
    else if (target_leftIdx > 1 && x->c[target_leftIdx - 1]->N >= min_degree) {  // promising자식 부족, 왼쪽형제 충분 (중요 확인필요) // 좌측sibling 찾고싶은데, 맨 왼쪽에있는애는 이걸 못한다. 그 조건이 i>=2
        x->c[target_leftIdx]->N = x->c[target_leftIdx]->N + 1;                   //

        for (int j = min_degree - 1; j >= 1; j--) {
            x->c[target_leftIdx]->key[j + 1] = x->c[target_leftIdx]->key[j];
        }
        for (int j = min_degree; j >= 1; j--) {
            x->c[target_leftIdx]->c[j + 1] = x->c[target_leftIdx]->c[j];
        }
        //키를 1에 넣으니까. 미리 키를 오른쪽으로 밀어줘야한다
        x->c[target_leftIdx]->key[1] = x->key[target_leftIdx - 1];                                  // 여기가 i-1인가?                        // 위에 값 i 로 내려주기 얘!
        x->key[target_leftIdx - 1] = x->c[target_leftIdx - 1]->key[x->c[target_leftIdx - 1]->N];    // 왼쪽 형제 가장 큰값을 부모로 올리기
        x->c[target_leftIdx]->c[1] = x->c[target_leftIdx - 1]->c[x->c[target_leftIdx - 1]->N + 1];  // 충분형제에서 부족으로 자식포인터 이동
        x->c[target_leftIdx]->N = min_degree;                                                       // promising 자식갯수 갱신
        x->c[target_leftIdx - 1]->N = x->c[target_leftIdx - 1]->N - 1;                              // 왼쪽의 충분형제는 크기가 하나 줄어든다 여기 오타...
        // right sibling 처럼 별도의 k나 c 이동작업 없이 N만 줄여주면 된다.
        delete_Key(root_address, x->c[target_leftIdx], target);
        return;  // 들어가는거 맞겠지? 이게 맞아?
    }
    // case 3.b 양쪽자식이 다 적다 병합해줘라!
    if (target_leftIdx == x->N + 1) {  // 내가 오른쪽 끝에있는 경우 -> i와 i-1을 봐줘야한다.
        Node *y = x->c[target_leftIdx - 1];
        Node *z = x->c[target_leftIdx];
        int m = y->N;
        int p = z->N;
        y->key[m + 1] = x->key[target_leftIdx - 1];  // x에서 y오른쪽끝으로 내리기 여기서는 i-1이 해당하는 녀석이다 (맞음)
        for (int j = 1; j <= p; j++) {               // z노드에서 y노드로 키 복사
            y->key[m + 1 + j] = z->key[j];
        }
        for (int j = 1; j <= p + 1; j++) {  //z노드에서 y노드로 child Ptr 복사
            y->c[m + 1 + j] = z->c[j];
        }
        y->N = m + p + 1;  // 크기 갱신

        x->N = x->N - 1;

        if (x->N == 0) {        // 만약 x의 크기가 0이면, 지워주고 루트노드 재설정해주기
            free(x);            // 루트 삭제
            *root_address = y;  // 루트 재설정
        }

        free(z);
        delete_Key(root_address, y, target);
        return;
    } else {  // 내가 왼쪽끝에있는경우
        Node *y = x->c[target_leftIdx];
        Node *z = x->c[target_leftIdx + 1];
        int m = y->N;
        int p = z->N;
        y->key[m + 1] = x->key[target_leftIdx];  // x에서 y오른쪽끝으로 내리기 여기서는 i-1이 해당하는 녀석이다 (맞음) (수정해봄)
        for (int j = 1; j <= p; j++) {           // z노드에서 y노드로 키 복사
            y->key[m + 1 + j] = z->key[j];
        }
        for (int j = 1; j <= p + 1; j++) {  //z노드에서 y노드로 child Ptr 복사
            y->c[m + 1 + j] = z->c[j];
        }

        y->N = m + p + 1;                                   // 크기 갱신
        for (int j = target_leftIdx + 1; j <= x->N; j++) {  // 부모노드의 자식포인터 갱신 (수정해봄)
            x->c[j] = x->c[j + 1];
        }
        for (int j = target_leftIdx; j < x->N; j++) {  // 부모노드의 키 갱신 (수정해봄)
            x->key[j] = x->key[j + 1];
        }
        x->N = x->N - 1;

        if (x->N == 0) {        // 만약 x의 크기가 0이면, 지워주고 루트노드 재설정해주기
            free(x);            // 루트 삭제
            *root_address = y;  // 루트 재설정
        }

        free(z);
        delete_Key(root_address, y, target);
        return;
    }
}

int search_Key(Node *x, int k) {
    if (x->leaf == 1) {  //x가 리프노드일경우
        for (int i = 0; i <= x->N; i++) {
            if (x->key[i] == k) {
                return 1;
            }
        }
        return 0;
    } else {
        int i = 1;
        while (x->key[i] < k && i <= x->N) {
            i++;
        }
        if (x->key[i] == k) {
            return 1;
        } else {
            search_Key(x->c[i], k);
        }
    }
}

void main() {
    //
    Node *root = (Node *)createNode(1);
    //입력받기 시작
    puts("B-트리의 최소차수(t: minimum degree)를 입력해주세요.");
    scanf("%d", &min_degree);
    if (min_degree < 2) {
        puts("min degree의 최솟값은 2입니다. 2로 설정되었습니다.");
        min_degree = 2;
    }

    for (int i = 1; i <= 20; i++) {
        insertKey(&root, root, i);
    }
    for (int i = 1; i <= 20; i = i + 3) {
        delete_Key(&root, root, i);
    }

    // 메인 실행부분
    while (1) {
        int totalbreak = 0;
        while (1) {
            puts("1: key 삽입, 2: key 삭제 3: key 조회 4: 트리 조회");
            int command;
            scanf("%d", &command);

            if (command == 1) {
                puts("넣을 key를 입력해주세요.");
                int inputKey;
                scanf("%d", &inputKey);
                int isExist = search_Key(root, inputKey);
                //
                if (isExist == 1) {
                    puts("이미 해당 key가 B-트리 안에 존재합니다");
                } else {
                    insertKey(&root, root, inputKey);
                }
                break;
            } else if (command == 2) {
                puts("삭제할 key를 입력해주세요.");
                int inputKey;
                scanf("%d", &inputKey);
                int isExist = search_Key(root, inputKey);
                if (isExist == 1) {
                    delete_Key(&root, root, inputKey);  // tree의 루트를 넘겨줌을 잊지 말자. delete는 계속 재귀로 해야하니깐. (확인필요) Node를 넘겨줘야한다
                } else {
                    puts("해당하는 Key가 B트리 안에 존재하지 않습니다.\n");
                }
                break;
            } else if (command == 3) {
                puts("조회할 key를 입력해주세요.");
                int inputKey;
                scanf("%d", &inputKey);
                int isExist = search_Key(root, inputKey);
                if (isExist == 1) {
                    printf("%d(이)가 B트리 안에 존재합니다.\n\n", inputKey);
                } else {
                    puts("해당하는 Key가 B트리 안에 존재하지 않습니다.\n");
                }
            } else if (command == 4) {
                puts("\n------------------");
                printAll(root, 0);
                printf("\n");
            } else {
                puts("1 또는 2를 입력해주세요.");
                break;
            }
        }

        printf("------------------");
        printAll(root, 0);  //루트에서부터 출력 시작, depth = 0부터 시작
        puts("\n------------------");

        puts("계속 하시곘습니까? (1: 예, 2: 아니오)");
        while (1) {
            int again_Check;
            scanf("%d", &again_Check);
            if (again_Check == 1) {
                break;
            } else if (again_Check == 2) {
                totalbreak = 1;
                return;
            }
        }
    }
}