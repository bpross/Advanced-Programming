#include <iostream>
using namespace std;

//List prototype

template<typename T> class List;
template<typename T> ostream &operator <<(ostream &, const List<T> &);

// Node
template<typename T>
class Node{
  friend class List<T>;

  public:
    Node(T item) { data = item; next = 0;};
    ~Node(){};

  friend ostream &operator << <T>(ostream &, const List<T> &);
  private:
    T data;
    Node<T>* next;
    Node<T>* prev;

};

// List
template<typename T>
class List{
  public:
    List(){ length = 0; head = 0, tail = 0;};
    ~List() {while(length>0) pop();};

    //operands 
    void push(T item,bool unique);
    T pop();
    void print();
    void swap(Node<T>* m, Node<T>* n);
    void sort();

    friend ostream &operator << <T>(ostream &, const List<T> &);
    private:

      int length;
      Node<T>* head;
      Node<T>* curr;
      Node<T>* tail;
      bool isEmpty();
};

// List::pop()
// deletes head of List
// Precondition: Length > 0

template<typename T>
T List<T>::pop(){
  if(length == 0){
    cout << "Error: calling pop() on empty list" << endl;
    exit(1);
  }
  T temp = head->data;
  if(length == 1){
    delete head;
    head = tail = 0;
  }else{
    Node<T>* N = head;
    head = head->next;
    delete N;
  }
  length--;
  return temp;
}

// List::push()
// pushes a new node to the head of the list

template<typename T>
void List<T>::push(T item, bool unique){
  bool found = false;
  if (unique == true){
    int iter;
    Node<T>* temp = head;
    for(iter = 0; iter < length; ++iter){
      if (temp->data == item){
        found = true;
        break;
      }
      else
        temp = temp->next;
    }
  }
  if (found != true){
    Node<T>* N = new Node<T>(item);
    if(length == 0){
      head=tail=N;
    }else{
      head->prev = N;
      N->next = head;
      N->prev = NULL;
      head = N;
    }
    length++;
  }
}

// List::print()
// prints data from each node

template<typename T>
void List<T>::print(){
  Node<T>* temp = head;
  while(temp != 0){
    cout << temp->data << " ";
    temp = temp->next;
  }
  cout << endl;
}

// List::swap()
// swaps the current marker in the list with the previous

template<typename T>
void List<T>::swap(Node<T>* m, Node<T>* n){
  n->prev = m->prev;
  m->prev = n;
  m->next = n->next;
  if (m->next != NULL)
    m->next->prev = m;
  n->next = m;
  if (n->prev != NULL)
    n->prev->next = n;
  int i;
  Node<T>* temp = this->tail;
  for(i = length; i > 0; --i){
    if (temp->prev != NULL)
      temp = temp->prev;
  }
  this->head = temp;
}


// List::sort()
template<typename T>
void List<T>::sort(){
  int i, j, k;
  for(i = 1; i < this->length; ++i){
    j = i;
    this->curr = this->head;
    for(k = 0; k < j; ++k){
      this->curr = this->curr->next;
    }
    while(j > 0 && (this->curr->prev->data > this->curr->data) ) {
      swap(this->curr->prev, this->curr);
      --j;
    }
  }
}

template<typename T>
bool List<T>::isEmpty(){
  if (this->length == 0)
    return true;
  else
    return false;
}          

// ostream operator
template<typename T>
ostream &operator<< (ostream &out, const List<T> &that){
  if(that.length == 0 ){
    out << "List empty" << endl;
  }else{
    int i = 0;
    Node<T>* temp = that.head;
    for(i = 1; i <= that.length; ++i){
      out << temp->data << " ";
      temp = temp->next;
    }
  }
  return out;
}
    
  
