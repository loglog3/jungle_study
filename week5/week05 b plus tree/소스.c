#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<stdlib.h>
#include<time.h>

int order, t;
// Struct Definition : Node, Tree, Search_Result
typedef struct btNode {
	int key_num;
	int* keys;
	struct btNode** child_pointer;
	struct btNode* link;
	bool isLeaf;
}btNode;

typedef struct searchResult {
	btNode* node;
	int index;
	bool result;
}sr;

typedef struct bTree {
	struct btNode* root;
}bTree;

typedef struct find_sibs {
	int direction;
	bool result;
}fs;

// Initialization
btNode* create_node(bool isLeaf_) {
	btNode* node;
	node = (struct btNode*)malloc(sizeof(struct btNode));
	node->child_pointer = (struct btNode**)malloc((order) * sizeof(struct btNode*));
	node->link = NULL;
	node->isLeaf = isLeaf_;
	node->key_num = 0;
	node->keys = (int*)malloc((order - 1) * sizeof(int));
	for (int i = 0; i < order - 1; i++) {
		(node->keys)[i] = -1;
	}
	return node;
}

bTree* create_tree(int order) {
	bTree* tree;
	tree = (struct bTree*)malloc(sizeof(struct bTree));
	//tree.order = order;
	tree->root = create_node(order, true);
	//tree->node_count = 1;
	return tree;
}

// Methods for Printing Tree
/*
void print_all(btNode* x, int depth) {
	int i;
	for (i = 0; i < depth; i++) {
		printf("                    ");
	}
	for (i = 0; i < x->key_num; i++) {
		printf("%d ", x->keys[i]);
	}
	printf("\n");
	if (x->isLeaf == false) {
		for (i = 0; i < x->key_num + 1; i++) {
			print_all(x->child_pointer[i], depth + 1);
		}
	}
}
*/
//version 1  simple way

void print_all(btNode* x, int depth) {
	for (int i = 0; i <= x->key_num; i++) {
		if (x->isLeaf != true) {
			print_all(x->child_pointer[i], depth + 1);
		}
		if (i == 0) {
			for (int j = 0; j < depth; j++) {
				printf("------------------");
			}
			for (int k = 0; k < x->key_num; k++) {
				printf("%4d", x->keys[k]);
			}
			puts("");
		}

	}
}

// Methods for Insert Operation
void split(btNode* x, int i) {
	printf("starting split %dth node\n", i);
	btNode* y;
	btNode* z;
	y = x->child_pointer[i];
	z = create_node(y->isLeaf);

	if (y->isLeaf == true) {
		y->link = z;
		z->key_num = t - 1;

		for (int j = t; j < y->key_num; j++) {
			z->keys[j - t] = y->keys[j];
		}
		y->key_num = t;

		for (int j = x->key_num; j >= i; j--) {
			x->child_pointer[j + 1] = x->child_pointer[j];
		}
		x->child_pointer[i + 1] = z;

		for (int j = x->key_num - 1; j >= i; j--) {
			x->keys[j + 1] = x->keys[j];
		}
		x->keys[i] = z->keys[0];
		x->key_num = x->key_num + 1;
	}
	else {
		z->key_num = t - 1;
		for (int j = 0; j < t - 1; j++) {
			z->keys[j] = y->keys[j + t];
		}

		if (!y->isLeaf) {
			for (int j = 0; j < t; j++) {
				z->child_pointer[j] = y->child_pointer[j + t];
			}
		}
		y->key_num = t - 1;

		for (int j = x->key_num; j >= i; j--) {
			x->child_pointer[j + 1] = x->child_pointer[j];
		}
		x->child_pointer[i + 1] = z;

		for (int j = x->key_num - 1; j >= i; j--) {
			x->keys[j + 1] = x->keys[j];
		}
		x->keys[i] = y->keys[t - 1];
		x->key_num = x->key_num + 1;
	}
}

void insertNonFull(btNode* x, int k) {
	int i = x->key_num;
	if (x->isLeaf) {
		//look for index to input k
		while (i >= 0 && k < x->keys[i - 1]) {
			x->keys[i] = x->keys[i - 1];
			i -= 1;
		}
		x->keys[i] = k;
		x->key_num += 1;
	}
	else {
		while (i >= 0 && k < x->keys[i - 1]) {
			i = i - 1;
		}
		//i = i + 1;
		if (x->child_pointer[i]->key_num == 2 * t - 1) {
			split(x, i);
			if (k > x->keys[i]) {
				i += 1;
			}
		}
		insertNonFull(x->child_pointer[i], k); \
	}
}

void insert(bTree* tree, int k) {
	btNode* r = tree->root;
	printf("starting insert %d to tree\n", k);
	if (r->key_num == order - 1) {
		btNode* s;
		s = create_node(false);
		tree->root = s;
		s->key_num = 0;
		s->isLeaf = false;
		(s->child_pointer)[0] = r;
		split(s, 0);
		insertNonFull(s, k);
	}
	else {
		insertNonFull(r, k);
	}
}

// Methods for Search Operation
sr* searchNode(btNode* x, int k) {
	int i = 0;
	while (i<x->key_num && k > x->keys[i]) {
		i += 1;
	}
	if (i < x->key_num && k == x->keys[i]) {
		sr* search_result = (sr*)malloc(sizeof(sr));
		search_result->node = x;
		search_result->index = i;
		search_result->result = true;
		return search_result;
	}
	else if (x->isLeaf == true) {
		sr* search_result = (sr*)malloc(sizeof(sr));
		search_result->result = false;
		return search_result;
	}
	else {
		return searchNode(x->child_pointer[i], k);
	}
}

int find_succ(btNode* x) {//find successor from the sub-tree starting with the left node
	printf("Finding successor\n");
	int succ;
	// If x is Leaf node, return the key on the right end. else, recursively find in the childs.
	if (x->isLeaf == true) {
		succ = x->keys[0];
		return succ;
	}
	else {
		find_succ(x->child_pointer[0]);
	}
}

int find_pred(btNode* x) {//find predecessor from the sub-tree starting with the right node
	printf("Finding predecessor\n");
	int pred;
	// If x is Leaf node, return the key on the left end. else, recursively find in the childs.
	if (x->isLeaf == true) {
		pred = x->keys[x->key_num - 1];
		return pred;
	}
	else {
		find_pred(x->child_pointer[x->key_num]);
	}
}

fs* find_siblings(btNode* x, int i) {
	//Given node x.ci, look around and see if borrow/merge is possible. 
	fs* find_result = (fs*)malloc(sizeof(fs));
	if (i == 0) {
		//if i is at the left end, see only to the right side.
		find_result->direction = 1;
		if (x->child_pointer[i + 1]->key_num > t - 1) {
			//if sibling on the right has key_num larger than t-1 ==> RIGHT BORROW
			find_result->result = true;
			return find_result;
		}
		else {
			//if sibling on the right has key_num equal to t-1 ==> RIGHT MERGE
			find_result->result = false;
			return find_result;
		}
	}
	else if (i < x->key_num) {
		//if i is somewhere in the middle of the node, see both ways. primary direction is right.
		if (x->child_pointer[i + 1]->key_num > t - 1) {
			//if sibling on the right has key_num larger than t-1 ==> RIGHT BORROW
			find_result->direction = 1;
			find_result->result = true;
			return find_result;
		}
		else if (x->child_pointer[i - 1]->key_num > t - 1) {
			//if sibling on the left has key_num larger than t-1 ==> LEFT BORROW
			find_result->direction = -1;
			find_result->result = true;
			return find_result;
		}
		else {
			//if siblings on the both side has key_num equal to t-1 ==> RIGHT MERGE
			find_result->direction = 1;
			find_result->result = false;
			return find_result;
		}
	}
	else if (i == x->key_num) {
		//if i is on the right end, look sibling on the left side.
		find_result->direction = -1;
		if (x->child_pointer[i - 1]->key_num > t - 1) {
			//if sibling on the left has key_num larger than t-1 ==> LEFT BORROW
			find_result->result = true;
			return find_result;
		}
		else {
			//if sibling on the left has key_num equal to t-1 ==> LEFT MERGE
			find_result->result = false;
			return find_result;
		}
	}
	else {
		printf("finding siblings failed. please fix me");
		find_result->direction = 0;
		find_result->result = false;
		return find_result;
	}
}

// Methods for Delete Operation
void mergeRightNode(btNode* x, int i) {
	//merge x's i-th node with i+1-th node
	btNode* left_node = x->child_pointer[i];
	btNode* right_node = x->child_pointer[i + 1];
	int left_keynum = left_node->key_num;
	int right_keynum = right_node->key_num;

	if (left_node->isLeaf == true) {
		//move keys from right node to left node
		for (int j = 0; j < right_keynum; j++) {
			left_node->keys[left_keynum + j] = right_node->keys[j];
			printf("%d\n", right_node->keys[j]);
		}
		left_node->key_num = left_keynum + right_keynum;

		//As key 'K' of node 'X' moved to child node, change the key, child index of node 'x' 
		for (int j = i + 1; j < x->key_num; j++) {
			x->child_pointer[j] = x->child_pointer[j + 1];
			x->keys[j - 1] = x->keys[j];
		}
		x->key_num -= 1;
		left_node->link = right_node->link;
		free(right_node);
	}
	else {
		left_node->keys[left_keynum] = x->keys[i];//add k to left node(x->keys[i] == k)
		//move keys from right node to left node
		for (int j = 0; j < right_keynum; j++) {
			left_node->keys[left_keynum + j + 1] = right_node->keys[j];
		}
		//if right node is not leaf, move childs from right node to left node
		if (x->child_pointer[i]->isLeaf == false) {
			for (int j = 0; j <= right_keynum; j++) {
				left_node->child_pointer[left_keynum + j + 1] = right_node->child_pointer[j];
			}
		}
		left_node->key_num = left_keynum + right_keynum + 1;
		//As key 'K' of node 'X' moved to child node, change the key, child index of node 'x' 
		for (int j = i + 1; j < x->key_num; j++) {
			x->child_pointer[j] = x->child_pointer[j + 1];
			x->keys[j - 1] = x->keys[j];
		}
		x->key_num -= 1;
		free(right_node);
	}
}

int deleteNode(btNode* x, int k, bTree* tree) {
	if (k == 19912) {
		print_all(tree->root, 0);
	}
	int i;

	//if x is a leaf node, find k from x and return 1
	if (x->isLeaf == true) {
		for (i = 0; i < x->key_num; i++) {
			if (x->keys[i] == k) {
				break;
			}
		}
		printf("%dth key was %d. Delete Success\n", i, k);
		for (; i < x->key_num - 1; i++) {
			x->keys[i] = x->keys[i + 1];
		}
		x->key_num -= 1;
		return 1;
	}
	i = 0;
	//Find index of k
	// range to key_num - 1 prevents i going over than the key_value
	while (x->keys[i] < k && i < x->key_num - 1) {
		i += 1;
	}
	printf("i = %d\n", i);
	// Case 2: K is in an internal node x
	if (x->keys[i] == k) {
		// Case 2-a : (i)th child node has key_num larger than t-1
		if (x->child_pointer[i + 1]->key_num > t - 1) {
			if (x->child_pointer[i + 1]->isLeaf == true) {
				if (x->keys[i] != x->child_pointer[i + 1]->keys[0]) {
					x->keys[i] = x->child_pointer[i + 1]->keys[0];
				}
				else {
					int replace_num = find_succ(x->child_pointer[i + 1]);
					printf("%d\n", replace_num);
					x->keys[i] = replace_num;//x->keys[i] = K
					deleteNode(x->child_pointer[i + 1], replace_num, tree);
				}
			}
			else {
				int replace_num = find_succ(x->child_pointer[i + 1]);
				x->keys[i] = replace_num;//x->keys[i] = K
				printf("%d\n", replace_num);
				deleteNode(x->child_pointer[i + 1], replace_num, tree);
			}
		}
		// Case 2-b : (i+1)th child node has ken_num larger than t-1
		else if (x->child_pointer[i]->key_num > t - 1) {
			if (x->child_pointer[i]->isLeaf == true) {
				int pred = x->child_pointer[i]->keys[x->child_pointer[i]->key_num - 1];
				printf("%d\n", pred);
				print_all(tree->root, 0);
				x->keys[i] = pred;
				x->child_pointer[i + 1]->keys[0] = pred;
				x->child_pointer[i]->key_num -= 1;
			}
			else {
				int replace_num = find_pred(x->child_pointer[i]);
				printf("%d\n", replace_num);
				x->keys[i] = replace_num;//x->keys[i] = K
				print_all(tree->root, 0);
				deleteNode(x->child_pointer[i], replace_num, tree);
			}
		}
		// Case 2-c : Merge (i)th child node with (i+1)th child node if both node's key_num is equal to t-1
		else {
			mergeRightNode(x, i);
			if (tree->root->key_num == 0) {
				tree->root = x->child_pointer[i];
				if (!(x->child_pointer[i]->isLeaf)) {
					deleteNode(x->child_pointer[i], k, tree);
				}
				free(x->child_pointer);
				free(x->keys);
				free(x);
			}
			else {
				deleteNode(x->child_pointer[i], k, tree);
			}
		}
	}
	else {
		// Case 3 : K is not in node X
		// increase i if i should visit the last child node
		if (k > x->keys[i]) {
			i += 1;
		}
		// Next node to visit is (i)th child node.
		// If (i)th child node has key_num larger than t-1, visit (i)th node.
		if (x->child_pointer[i]->key_num > t - 1) {
			deleteNode(x->child_pointer[i], k, tree);
		}
		else {
			// Case 3-a
			fs* find_result = find_siblings(x, i);
			if (find_result->direction == 1 && find_result->result == true) {
				if (x->child_pointer[i]->isLeaf == true) {
					// Borrow from right node
					// move (i)th key of node X to (i)th child node
					x->child_pointer[i]->keys[t - 1] = x->keys[i];
					x->child_pointer[i]->key_num += 1;
					//move (i+1)th child node's keys to the left
					for (int j = 0; j < x->child_pointer[i + 1]->key_num - 1; j++) {
						x->child_pointer[i + 1]->keys[j] = x->child_pointer[i + 1]->keys[j + 1];
					}
					x->keys[i] = x->child_pointer[i + 1]->keys[0];
					x->child_pointer[i + 1]->key_num -= 1;
					deleteNode(x->child_pointer[i], k, tree);
				}
				else {
					// Borrow from right node
					// move (i)th key of node X to (i)th child node
					x->child_pointer[i]->keys[t - 1] = x->keys[i];
					x->child_pointer[i]->key_num += 1;
					x->keys[i] = x->child_pointer[i + 1]->keys[0];
					x->child_pointer[i]->child_pointer[t] = x->child_pointer[i + 1]->child_pointer[0];

					//move (i+1)th child node's keys to the left
					for (int j = 0; j < x->child_pointer[i + 1]->key_num - 1; j++) {
						x->child_pointer[i + 1]->keys[j] = x->child_pointer[i + 1]->keys[j + 1];
					}
					//move (i+1)th child node's child pointers to the left
					if (x->child_pointer[i + 1]->isLeaf == false) {
						for (int j = 0; j < x->child_pointer[i + 1]->key_num; j++) {
							x->child_pointer[i + 1]->child_pointer[j] = x->child_pointer[i + 1]->child_pointer[j + 1];
						}
					}
					x->child_pointer[i + 1]->key_num -= 1;
					deleteNode(x->child_pointer[i], k, tree);
				}
			}
			else if (find_result->direction == 1 && find_result->result == false) {
				//Merge with right node
				mergeRightNode(x, i);
				if (tree->root->key_num == 0) {
					tree->root = x->child_pointer[i];
					deleteNode(x->child_pointer[i], k, tree);
					free(x->child_pointer);
					free(x->keys);
					free(x);
				}
				else {
					deleteNode(x->child_pointer[i], k, tree);
				}
			}
			else if (find_result->direction == -1 && find_result->result == false) {
				//Merge with left node
				mergeRightNode(x, i - 1);
				if (tree->root->key_num == 0) {
					tree->root = x->child_pointer[i - 1];
					deleteNode(x->child_pointer[i - 1], k, tree);
					free(x->child_pointer);
					free(x->keys);
					free(x);
				}
				else {
					deleteNode(x->child_pointer[i - 1], k, tree);
				}
			}
			else if (find_result->direction == -1 && find_result->result == true) {
				//borrow from left node
				//to borrow one key from left node, move existing keys to the right
				if (x->child_pointer[i]->isLeaf == true) {
					int replace_num = x->child_pointer[i - 1]->keys[x->child_pointer[i - 1]->key_num - 1];
					x->keys[i - 1] = replace_num;
					//move existing child pointers to the right
					for (int j = x->child_pointer[i]->key_num - 1; j >= 0; j--) {
						x->child_pointer[i]->keys[j + 1] = x->child_pointer[i]->keys[j];
					}
					x->child_pointer[i]->keys[0] = replace_num;
					x->child_pointer[i]->key_num += 1;
					x->child_pointer[i - 1]->key_num -= 1;
					deleteNode(x->child_pointer[i], k, tree);
				}
				else {
					for (int j = x->child_pointer[i]->key_num - 1; j >= 0; j--) {
						x->child_pointer[i]->keys[j + 1] = x->child_pointer[i]->keys[j];
					}
					x->child_pointer[i]->keys[0] = x->keys[i - 1];
					x->keys[i - 1] = x->child_pointer[i - 1]->keys[x->child_pointer[i - 1]->key_num - 1];
					//move existing child pointers to the right
					for (int j = x->child_pointer[i]->key_num; j >= 0; j--) {
						x->child_pointer[i]->child_pointer[j + 1] = x->child_pointer[i]->child_pointer[j];
					}
					x->child_pointer[i]->child_pointer[0] = x->child_pointer[i - 1]->child_pointer[x->child_pointer[i - 1]->key_num];
					x->child_pointer[i]->key_num += 1;
					x->child_pointer[i - 1]->key_num -= 1;
					deleteNode(x->child_pointer[i], k, tree);
				}
			}
			else {
				printf("Something wrong while operating Delete");
			}
		}
	}

}

int deleteCheck(bTree* x, int k) {
	//Delete k from tree x, if k is not in tree x, 
	sr* search_result;
	btNode* r = x->root;
	search_result = searchNode(r, k);
	if (search_result->result == false) {
		printf("%d is not in the tree.\n", k);
		return 0;
	}
	else {
		printf("%d is in the tree.\n", k);
		deleteNode(r, k, x);
	}
}

void main() {
	while (1) {
		printf("Input Max Degree : \n");
		scanf_s("%d", &order);
		if (order >= 4 && order % 2 == 0) {
			break;
		}
		else {
			puts("Max Degree should be even and over 4. Please Input appropriate number.");
		}
	}
	
	int insert_num = 0;
	char type;
	int k;
	t = order / 2;
	bTree* tr = create_tree(order);

	/*
	for (int j = 1; j < 1000; j++) {
		insert(tr, j);
	}
	*/

	while (true) {
		printf("Input Operation : (Input : i, Delete : d, Search : s, Print : p, Quit: q)\n");
		scanf_s("\n%c", &type, 1);

		if (type == 'i') {
			printf("Write the number to insert and press Enter\n");
			while (true) {
				scanf_s("%d", &insert_num);
				if (insert_num == -1) {
					break;
				}
				sr* result = searchNode(tr->root, insert_num);
				if (result->result == true) {
					puts("\nthe number is already in the tree.");
					//print_all(tr->root, 0);
				}
				else {
					insert(tr, insert_num);
				}
			}
		}
		else if (type == 'p') {
			print_all(tr->root, 0);
		}
		else if (type == 's') {
			printf("Wirte the number to search and press Enter\n");
			scanf_s("%d", &insert_num);
			sr* result = searchNode(tr->root, insert_num);
			if (result->result == true) {
				printf("the number is %dth key of a node.\n", result->index);
				print_all(tr->root, 0);
			}
			else {
				printf("%d doesn't exist in the tree\n", insert_num);
			}
		}
		else if (type == 'q') {
			exit(0); // 프로그램 종료
 		}
		else if (type == 'd') {
			scanf_s("%d", &insert_num);
			int k = deleteCheck(tr, insert_num);
			int l = deleteCheck(tr, insert_num);
		}
		else if (type == 'e') {
			int k = find_pred(tr->root);
			printf("predecessor of the root is %d\n", k);
		}
		else if (type == 'c') {
			int k = find_succ(tr->root);
			printf("successor of the root is %d\n", k);
		}
		else if (type == 'm') {
			mergeRightNode(tr->root, 1);
			print_all(tr->root, 0);
		}
		else if (type == 'x') {
			break;
		}

	}

}