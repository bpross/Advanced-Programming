// $Id: inode.cc,v 1.53 2011-01-11 13:22:56-08 - - $

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
  //Iterator used to search the map 
  map<string, inode *>::iterator search;
  map<string, inode *> remove_node;
  directory dirents = *contents.dirents;
  
  //Searches the directory for the filename
  search = dirents.find(filename);
  int size = 0;
  //Checks to see if the file was found. Throws error if not found
  assert (search != dirents.end());
  switch(type){
    case DIR_INODE:
      remove_node = dirents;
//      remove_node = dirents(search);
      size = remove_node.size();
      //Checks the size of the directory. If it is NOT 2,
      //the directory is not empty
      assert (size == 2);
      dirents.erase(search);
      break;
    case FILE_INODE:
      dirents.erase(search);
      break;
  }
  TRACE ('i', filename);
}

inode inode::mkdir (const string &filename) {
  inode new_dir (DIR_INODE);
  contents.dirents->insert( pair<string, inode *>(filename,&new_dir) );
  inode dot (DIR_INODE);
  inode dot_dot (DIR_INODE);
  new_dir.contents.dirents->insert( pair<string, inode *>(".",&dot));
  new_dir.contents.dirents->insert(pair<string,inode *>("..",&dot_dot));
  cout << new_dir.contents.dirents->size() << "\n";
  return new_dir;
}

inode inode::mkfile (const string &filename) {
  map<string, inode *>::iterator search;
  search = contents.dirents->find(filename);
  assert (search == contents.dirents->end() );
  inode new_file (FILE_INODE);
  contents.dirents->insert(pair<string, inode *>(filename, &new_file));
  return new_file;
}


inode_state::inode_state(): root (NULL), cwd (NULL), prompt ("%") {
   TRACE ('i', "root = " << (void*) root << ", cwd = " << (void*) cwd
          << ", prompt = " << prompt);
}

ostream &operator<< (ostream &out, const inode_state &state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

