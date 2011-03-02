// Authors: Ben Ross, Erik Steggall
// emails: bpross@ucsc.edu, esteggal@ucsc.edu
// Assignment 4
// CMPS109
// 02.28.11

#include <iostream>
using namespace std;

template <typename T>
class List;
template <typename T>
ostream &operator <<(ostream &, const List<T> &);
template <typename T>
class List{
  private:
    struct node{
      T item;
      node *next;
      node *previous;
      friend ostream &operator << <T>(ostream &, const List<T> &);
    }; 
    node* head;
    node* tail;
    int size;
    bool isEmpty();
    node *remove(node* temp);
  public:
    List();
    ~List();
    void push(T item);
    T pop();
    void sort();
};

template <typename T>
List<T>::List():head(NULL), tail(NULL), size(0){}

template <typename T>
List<T>::~List(){
  node* temp;
  while( !this->isEmpty() ){
    temp = this->pop();
    temp->List::~node();
  }
}
  
template <typename T>
bool List<T>::isEmpty(){
  if(this->size == 0)
    return true;
  else
    return false;
}

template <typename T>
typename List<T>::node *List<T>::remove(node* temp){
  node* temp_prev = temp->previous;
  node* temp_next = temp->next;
  temp_prev->next = temp_next;
  temp_next->previous = temp_prev;
  return temp;
}

template <typename T>
void List<T>::push(T item){
  node temp = new node();
  if(this->head == NULL){
    this->head = &temp;
    this->tail= &temp;
    temp.item = item;
    this->size ++;
  }else{
    this->tail->next = &temp;
    this->tail->previous = this->tail;
    this->tail = this->tail->next;
    temp.item = item;
    this->size ++;
  }
}

template <typename T>
T List<T>::pop(){
  node* temp = this->tail;
  this->tail = this->tail->previous;
  return temp->item;
}
 
template <typename T>
void List<T>::sort(){
  int i, j, k;
  node* temp,hold;
  for(i = 1; i < this->size; ++i){
    j = i;
    temp = this->head;
    for(k = 1; k < j; ++k)
      temp = temp->next;
    while(j > 0 && (temp.previous.item > temp.item)){
      hold = temp->previous;
      temp = this->remove(temp);
      temp->next = hold;
      temp.previous = hold->previous;
      hold->previous = temp;
      temp->previous->next = temp;
      j--;
    }
  }
}
template <typename T>
ostream &operator<< (ostream &out, const List<T> &that){
  if(that.isEmpty() ){
    out << "List empty" << endl;
  }else{
    int i = 0;
    typename List<T>::node* temp = that.front;
    for(i = 1; i <= that.size(); ++i){
      out << temp->item << endl;
      temp = temp.next;
    }
  }
  return out;
}
