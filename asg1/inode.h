// $Id: inode.h,v 1.3 2010-12-13 20:11:09-08 - - $

#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#include "trace.h"
#include "util.h"

//
// inode_t -
//    An inode is either a directory or a plain file.
//

enum inode_t {DIR_INODE, FILE_INODE};

//
// directory -
//    A directory is a list of paired strings (filenames) and inodes.
//    An inode in a directory may be a directory or a file.
//

class inode;
typedef map<string, inode *> directory;

//
// class inode -
//
// inode ctor -
//    Create a new inode of the given type, using a union.
//
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
//
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
//
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
//    Throws an yshell_exn for a directory.
//
// writefile -
//    Replaces the contents of a file with new contents.
//    Throws an yshell_exn for a directory.
//
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an yshell_exn if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
//
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
//
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.
//  
//    

class inode {
   private:
      int inode_nr;
      inode_t type;
      union {
         directory *dirents;
         wordvec *data;
      } contents;
      static int next_inode_nr;
   public:
      ~inode(){};
      inode (inode_t init_type);
      inode (const inode &source);
      inode &operator= (const inode &from);
      int get_inode_nr();
      int size();
      const wordvec &readfile() const;
      void writefile (const wordvec &newdata);
      void remove (const string &filename);
      inode &locate(const string &filename);
      directory &get_directory();
      int get_type();
      void list ();
      void mkdir (const string &filename);
      void mkroot (const inode &start_root);
      inode mkfile(const string &filename);
      inode &get_contents();
};

//
// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.
//
// change_root -
//    used to assign/change the root directory of the shell
//
// change_cwd -
//    used to assign/change the cwd of the shell
//
// change_prompt -
//    used to change the prompt string
//
// get_root-
//    returns pointer to the root node
//
// get_cwd -
//    returns pointer to the cwd node
//
// get_prompt -
//    returns the current prompt 

class inode_state {
   friend class inode;
   friend ostream &operator<< (ostream &out, const inode_state &);
   private:
      inode_state (const inode_state &); // disable copy ctor
      inode_state &operator= (const inode_state &); // disable op=
      inode *root;
      inode *cwd;
      inode *tmp;
      string prompt;
      string cwd_string;
   public:
      inode_state();
      inode *locateinode(const string &filename);
      void change_root(inode &new_root);
      void change_cwd(inode &new_cwd);
      void change_tmp(inode &new_tmp);
      void change_prompt(string &prompt_string);
      void append_cwd_string(string addition);
      void remove_dir_string();
      void to_root();
      string get_cwd_string();
      inode *get_root();
      inode *get_tmp();
      inode *get_cwd();
      string get_prompt();
      
};

ostream &operator<< (ostream &out, const inode_state &);

#endif

