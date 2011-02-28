// Authors: Ben Ross, Erik Steggall
// emails: bpross@ucsc.edu, esteggal@ucsc.edu
// Assignment 4
// CMPS109
// 02.28.11

#include <stdio.h>
#include <stdlib.h>
#include "List.h"


Node_ref newNode(int data){
  Node_ref newN = new Node;
  newN->data = data;
  return newN;
}

List_ref newList(){
  List_ref newL = new List_ref();
  newL->head = NULL;
  newL->curr = NULL;
  newL->tail = NULL;
  newL->length = 0;
  return newL;
}


  
