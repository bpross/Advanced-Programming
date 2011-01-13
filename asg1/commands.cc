// $Id: commands.cc,v 1.52 2011-01-12 19:06:58-08 - - $

#include <cstdlib>
#include <cassert>
#include "commands.h"
#include "inode.h"
#include "trace.h"

commands::commands(): map (commandmap()) {
   map["cat"    ] = fn_cat    ;
   map["cd"     ] = fn_cd     ;
   map["echo"   ] = fn_echo   ;
   map["exit"   ] = fn_exit   ;
   map["ls"     ] = fn_ls     ;
   map["lsr"    ] = fn_lsr    ;
   map["make"   ] = fn_make   ;
   map["mkdir"  ] = fn_mkdir  ;
   map["prompt" ] = fn_prompt ;
   map["pwd"    ] = fn_pwd    ;
   map["rm"     ] = fn_rm     ;
}

function commands::operator[] (const string& cmd) {
   return map[cmd];
}

void fn_cat (inode_state &state, const wordvec &words){
  wordvec cat_vec = words;
  //Erases the command from the vector, size is decremented
  cat_vec.erase (cat_vec.begin());
  //inode cwd = state.get_cwd();
  //inode read_node = cwd.locate(cat_vec.front());
  //cout << "read_node type " << read_node.get_type() << endl;
  //Read in file
  //wordvec file_contents = read_node.readfile();
  //cout << cat_vec << "contains" << file_contents << endl;
  
  inode cwd = state.get_cwd();
  directory current_dir = cwd.get_directory();
  map<string, inode*>::iterator search;
  search = current_dir.find(cat_vec.front());
  cout << "searching for " << cat_vec.front() << endl;
  assert (search != current_dir.end());
  inode *found_node = search->second;
  cout << "file type after find " << found_node->get_type() << endl;
  cout << "inode number of found: " << found_node->get_inode_nr() << endl;
  cout << "size of dir " << cwd.size() << endl;
  wordvec file_contents = found_node->readfile();
  for(unsigned int vec_itor = 0; vec_itor < cat_vec.size(); vec_itor++)
    cout << cat_vec[vec_itor] << " ";
  cout << "\n";

  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_cd (inode_state &state, const wordvec &words){
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_echo (inode_state &state, const wordvec &words){
  wordvec echo_vec = words;
  //Erases the command from the vector, size is decremented
  echo_vec.erase (echo_vec.begin());
  //prints out the message to the console
  for(unsigned int vec_itor = 0; vec_itor < echo_vec.size(); vec_itor++)
    cout << echo_vec[vec_itor] << " ";
  //makes sure prompt isn't printed at end of the message
  cout << "\n";

  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_exit (inode_state &state, const wordvec &words){
  wordvec exit_vec = words;
  //checks to see if a status is given
  if (exit_vec.size() > 1){
    exit_vec.erase (exit_vec.begin());
    //erases command from vector, size is decremented
    string status = exit_vec.front();
    bool is_digit;
    //checks to see if the status is a number
    for (unsigned string_itor=0;string_itor<status.length();
         string_itor++){
      if(!isdigit(status[string_itor]))
        is_digit = false;
      else
        is_digit = true;
    }
    //sets the status to the number
    if (is_digit){
      exit_status::set (atoi(status.c_str()));
    }
    //status was not a number. exit status is now 255
    else{
      exit_status::set (255);
    }
  }
  //status not given
  else
    exit_status::set (0);
  TRACE ('c', state);
  TRACE ('c', words);
  throw ysh_exit_exn ();
}

void fn_ls (inode_state &state, const wordvec &words){
  wordvec ls_vec = words;
  //Removes command from vector, size is decremented
  ls_vec.erase (ls_vec.begin());
  inode cwd = state.get_cwd();
  cwd.list();


  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_lsr (inode_state &state, const wordvec &words){
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_make (inode_state &state, const wordvec &words){
  wordvec make_vec = words;
  //Removes command from vector, size is decremented
  make_vec.erase (make_vec.begin());
  //Store the Name of the file
  string filename = make_vec.front();
  //Remove the filename from rest
  make_vec.erase (make_vec.begin());
  //Now create a new file
  inode cwd = state.get_cwd();
  cout << "cwd number " << cwd.get_inode_nr() << endl;
  inode newfile = cwd.mkfile(filename);
  //Now add the contents of words into newfile
  cout << "new file type: " << newfile.get_type() << endl;
  newfile.writefile(make_vec);
  cout << "after write " << newfile.get_type() << endl;
  cout << "number of inode added " << newfile.get_inode_nr() << endl;
  cout << "size of dir " << cwd.size() << endl;
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_mkdir (inode_state &state, const wordvec &words){
  
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_prompt (inode_state &state, const wordvec &words){
  wordvec prompt_vec = words;
  //Removes command from vector, size is decremented
  prompt_vec.erase (prompt_vec.begin());
  string prompt_string;
  // appends a string to change the prompt based on strings
  // in the vector
  for(unsigned int vec_itor=0;vec_itor<prompt_vec.size();vec_itor++){
    prompt_string.append(prompt_vec[vec_itor]);
    //used to fix the problem of adding an extra space to the end
    //of the prompt
    if (!(vec_itor+1 >= prompt_vec.size())){
      prompt_string.append(" ");
    }
  }
  state.change_prompt(prompt_string);
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_pwd (inode_state &state, const wordvec &words){
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rm (inode_state &state, const wordvec &words){
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rmr (inode_state &state, const wordvec &words){
  TRACE ('c', state);
  TRACE ('c', words);
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

