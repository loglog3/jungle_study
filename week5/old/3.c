
while (1) {
    if (isInsert == 0) {
        puts("넣을 key를 입력해주세요\n");
        int newkey;
        scanf("%d", &newkey);
        insertKey(tree, newkey);
        break;
    } else if (isDelete == 0) {
        puts("삭제할 key를 입력해주세요\n");
        int newkey;
        scanf("%d", &newkey);
        delete_Key(tree, newkey);
        break;
    }
}
printAll(tree->root);