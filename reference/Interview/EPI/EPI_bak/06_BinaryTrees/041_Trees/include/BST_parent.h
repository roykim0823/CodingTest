#include <iostream>
#include <cstring>

#ifndef BST_P_H
#define BST_P_H
using namespace std;

const int maxn = 100;
struct Node{
    int key;
    Node *lchild, *rchild, *parent;
};
Node *p, node[maxn];
int cnt;

void init(){
    p = NULL;
    memset(node, '\0', sizeof(node));
    cnt = 0;
}

void create_minimal_tree(Node* &head, Node *parent, int a[], int start, int end){
    if(start <= end){
        int mid = (start + end)>>1;
        //int mid = start + (end-start)>>1;
        node[cnt].key = a[mid];
        node[cnt].parent = parent;
        head = &node[cnt++];
        create_minimal_tree(head->lchild, head, a, start, mid-1);
        create_minimal_tree(head->rchild, head, a, mid+1, end);
    }
}

Node* search(Node* head, int x){
    if(head == NULL) return NULL;
    else if(x == head->key) return head;
    else if(x <= head->key) search(head->lchild, x);
    else search(head->rchild, x);
}
#endif
