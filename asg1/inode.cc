// $Id: inode.cc,v 1.21 2011-01-09 20:00:40-08 - - $

#include <cassert>
#include <iostream>

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
   directory dirents = *contents.dirents;
   switch (type) {
      case DIR_INODE:
         size = dirents.size();
         break;
      case FILE_INODE:
         wordvec data = *contents.data;
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

void inode::writefile (const wordvec &words) {
   TRACE ('i', words);
   assert (type == FILE_INODE);
}

void inode::remove (const string &filename) {
   TRACE ('i', filename);
   assert (type == DIR_INODE);
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

