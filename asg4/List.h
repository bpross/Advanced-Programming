// Authors: Ben Ross, Erik Steggall
// emails: bpross@ucsc.edu, esteggal@ucsc.edu
// Assignment 4
// CMPS109
// 02.28.11

#include <iostream>
using namespace std;

template <typename T>
class List{
  private:
    struct node{
      T item;
      node *next;
      node *previous;
      friend ostream &operator << <>(ostream &, const List<T> &);
    }; 
    node* head;
    node* tail;
    int size;
    bool isEmpty();
    node* remove();
  public:
    List();
    ~List();
    void push(T item);
    T pop();
    void sort();
}
