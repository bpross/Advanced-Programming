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
    void push(T item);
    T pop();
    void print();
    void swap();

    private:
      int length;
      Node<T>* head;
      Node<T>* curr;
      Node<T>* tail;
      friend ostream &operator << <T>(ostream &, const List<T> &);
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

  if(length == 1){
    delete head;
    head = tail = 0;
  }else{
    Node<T>* N = head;
    head = head->next;
    delete N;
  }
  length--;
}

// List::push()
// pushes a new node to the head of the list

template<typename T>
void List<T>::push(T item){
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
void List<T>::swap(){
/*Not working, trying for most simple case*/
  cout << "head = " << head->data << endl;
  cout << "tail = " << tail->data << endl;
  head->next = NULL;
  tail->prev = NULL;
  head->prev = tail;
  tail->next = head;
}

// ostream operator
template<typename T>
ostream &operator<< (ostream &out, const List<T> &that){
  if(that.isEmpty() ){
    out << "List empty" << endl;
  }else{
    int i = 0;
    Node<T> temp = that.front;
    for(i = 1; i <= that.length; ++i){
      out << temp->item << endl;
      temp = temp.next;
    }
  }
  return out;
}
    
  
