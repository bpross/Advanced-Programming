// Authors: Ben Ross, Erik Steggall
// emails: bpross@ucsc.edu, esteggal@ucsc.edu
// Assignment 4
// CMPS109
// 02.28.11

#include "List.h"
/*

List::List():head( NULL), tail( NULL), size(0){
}

List::~List(){
  node* temp;
  while !this.isEmpty(){
    temp = this.pop();
    temp->List::~node();
  }
}

node List::node(){
  node new_node = new node();
  new_node.item = NULL;
  new_node.next = NULL;
  new_node.previous = NULL:
  return new_node;
}
  
void typename List<T>::~node(){
  delete [] item;
  delete [] next;
}

bool typename List<T>::isEmpty(){
  if this.size == 0
    return true;
  else
    return false;
}

node* typename List<T>::remove(node* temp){
  node* temp_prev = temp->previous;
  node* temp_next = temp->next;
  temp_prev->next = temp_next;
  temp_next->previous = temp_prev;
  return temp;
}

void typename List<T>::push(T item){
  node temp = new node();
  if this.head == NULL{
    this.head = &temp;
    this.tail = &temp;
    temp.item = item;
    this.size ++;
  }else{
    this.tail.next = &temp;
    this.tail.previous = this.tail;
    this.tail = this.tail.next;
    temp.item = item;
    this.size ++;
  }
}

T typename List<T>::pop(){
  node* temp = this.tail;
  this.tail = this.tail.previous;
  return temp->item;
}

void typename List<T>::sort(){
  int i, j, k;
  node* temp,hold;
  
  for(i = 1; i < this.size; ++i){
    j = i;
    temp = this.head;
    for(k = 1; k < j; ++k)
      temp = temp->next;
    
    while(j > 0 && (temp.previous.item > temp.item)){
      hold = temp->previous;
      temp = this.remove(temp);
      temp->next = hold;
      temp.previous = hold->previous;
      hold->previous = temp;
      temp->previous->next = temp;
      j--;
    }
  }
}

ostream &operator<< (ostream &out, const List<class T> &that){
  if(that.isEmpty() ) return out << "List empty" << endl;
  int i = 0;
  node* temp = that.front;
  for(i = 1; i <= that.size(); ++i){
    out << curr->item << endl;
    temp = temp.next;
  }
}

*/


