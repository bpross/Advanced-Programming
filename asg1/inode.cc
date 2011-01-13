// $Id: inode.cc,v 1.133 2011-01-13 02:17:26-08 - - $

#include <cassert>
#include <iostream>
#include <map>

using namespace std;

#include "trace.h"
#include "inode.h"


int inode::next_inode_nr = 1;

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case DIR_INODE:
           contents.dirents = new directory();
           break;
      case FILE_INODE:
           contents.data = new wordvec();
           break;
   }
   TRACE ('i', "inode " << inode_nr << ", type = " << type);
}


//
// copy ctor -
//    Make a copy of a given inode.  This should not be used in
//    your program if you can avoid it, since it is expensive.
//    Here, we can leverage operator=.
//
inode::inode (const inode &that) {
   *this = that;
}

//
// operator= -
//    Assignment operator.  Copy an inode.  Make a copy of a
//    given inode.  This should not be used in your program if
//    you can avoid it, since it is expensive.
//
inode &inode::operator= (const inode &that) {
   if (this != &that) {
      inode_nr = that.inode_nr;
      type = that.type;
      contents = that.contents;
   }
   TRACE ('i', "inode " << inode_nr << ", type = " << type);
   return *this;
}

int inode::get_inode_nr() {
   TRACE ('i', "inode = " << inode_nr);
   return inode_nr;
}

int inode::size() {
   int size = 1;
   //Checks for INODE Type
   switch (type) {
     case DIR_INODE:
       size = contents.dirents->size();
       break;
     case FILE_INODE:
       wordvec data = *contents.data;
       //Adds the word count to the character count of each word
       for(unsigned vec_itor = 0; vec_itor < data.size(); vec_itor++){
         size += data[vec_itor].size();
       }
       break;
   }
   TRACE ('i', "size = " << size);
   return size;
}

const wordvec &inode::readfile() const {
   TRACE ('i', *contents.data);
   assert (type == FILE_INODE);
   return *contents.data;
}

void inode::writefile(const wordvec &words) {
  TRACE('i', words);
  assert (type == FILE_INODE);
  *contents.data = words;
}


void inode::remove (const string &filename) {
  map<string, inode*>::iterator search;
  search = contents.dirents->find(filename);
  assert (search != contents.dirents->end());
  inode *remove_node = search->second;
  cout << "in remove1" << endl;
  switch(remove_node->type){
    case DIR_INODE:
      cout << "In dir" << endl;
      assert (remove_node->size() <= 2);
      contents.dirents->erase(search);
      break;
    case FILE_INODE:
      cout << "In file" << endl;
      contents.dirents->erase(search);
      break;
  }
  cout << "in remove2" << endl;
  TRACE ('i', filename);
}

int inode::get_type(){
  int type_int = 2;
  switch(type){
    case DIR_INODE:
      type_int = 0;
      break;
    case FILE_INODE:
      type_int = 1;
      break;
  }
  return type_int;
}

inode &inode::locate (const string &filename) {
  map<string, inode*>::iterator search;
  search = contents.dirents->find(filename);
  assert (search != contents.dirents->end());
  inode *found_node = search->second;
  cout << found_node->get_type() << endl;
  return *found_node;
}

directory &inode::get_directory(){
  return *contents.dirents;
}

inode inode::mkdir (const string &filename) {
  inode *new_dir = new inode (DIR_INODE);
  contents.dirents->insert( pair<string, inode *>(filename,new_dir) );
  inode *dot = new inode (DIR_INODE);
  inode *dot_dot = new inode (DIR_INODE);
  new_dir->contents.dirents->insert( pair<string, inode *>(".",dot));
  new_dir->contents.dirents->insert(pair<string,inode *>("..",dot_dot));
  dot_dot->contents.dirents = contents.dirents;
  return *new_dir;
}

void inode::list (){
  map<string, inode*>::iterator it;
  for( it = contents.dirents->begin(); it != contents.dirents->end(); it++){
    cout <<  (*it).first << endl;
  }
}

void inode::mkroot (const inode &start_root){
//  start_root.contents.dirents->insert( pair<string, inode *>("/", start_root);
  inode *dot = new inode (DIR_INODE);
  inode *dot_dot = new inode (DIR_INODE);
  start_root.contents.dirents->insert( pair<string, inode *>(".", dot));
  start_root.contents.dirents->insert( pair<string, inode *>("..", dot_dot));
  dot_dot->contents.dirents = contents.dirents;
}

inode inode::mkfile (const string &filename) {
  map<string, inode *>::iterator search;
  search = contents.dirents->find(filename);
  assert (search == contents.dirents->end() );
  inode *new_file = new inode (FILE_INODE);
  contents.dirents->insert(pair<string, inode *>(filename, new_file));
  return *new_file;
}

void inode_state::append_cwd_string(string addition){
  string append_str;
  if(cwd_string.size() != 1)
    append_str.append("/");
  append_str.append(addition);
  cwd_string.append(append_str);
}

void inode_state::remove_dir_string(){
  unsigned int str_itor = 0;
  string temp;
  for(str_itor = cwd_string.size();;str_itor--){
    temp = cwd_string[str_itor];
    if(temp == "/")
      break;
    else
      cwd_string.erase(str_itor);
  }
  if(cwd_string.size() != 1)
    cwd_string.erase(str_itor);
}

void inode_state::to_root(){
  cwd_string = "/";
}

string inode_state::get_cwd_string(){
  return cwd_string;
}

inode_state::inode_state(): root (NULL), cwd (NULL), prompt ("%"), cwd_string("/") {
   TRACE ('i', "root = " << (void*) root << ", cwd = " << (void*) cwd
          << ", prompt = " << prompt);
}

inode *inode_state::locateinode(const string &filename){

  map<string, inode *>::iterator search;
  inode cwd = get_cwd();
  directory cwd_dirents = cwd.get_directory();
  for( search = cwd_dirents.begin(); search != cwd_dirents.end(); search++){
    if(search->first == filename) return search->second;
  }
  return NULL;
}

void inode_state::change_root(inode &new_root){
  root = &new_root;
}

void inode_state::change_cwd(inode &new_cwd){
  cwd = &new_cwd;
}

void inode_state::change_prompt(string &prompt_string){
  prompt = prompt_string;
}

inode *inode_state::get_root(){
  return root;
}


inode inode_state::get_cwd(){
  return *cwd;
}

string inode_state::get_prompt(){
  return prompt;
}

ostream &operator<< (ostream &out, const inode_state &state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

