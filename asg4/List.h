// Authors: Ben Ross, Erik Steggall
// emails: bpross@ucsc.edu, esteggal@ucsc.edu
// Assignment 4
// CMPS109
// 02.28.11



typedef struct Node{
  int data;
  struct Node *next;
  struct Node *prev;
}Node;

typedef Node *Node_ref;

typedef struct List{
  Node_ref head;
  Node_ref curr;
  Node_ref tail;
  int length;
}List;

typedef List *List_ref;


/*              Constructors                 */

Node_ref newNode(int data);

List_ref newList();

void freeNode(Node_ref *fN);

void freeList(List_ref* L);
