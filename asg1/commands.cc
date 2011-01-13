// $Id: commands.cc,v 1.52 2011-01-13 05:28:52-08 - - $

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
  inode *found_node = state.locateinode(cat_vec.front());
  if (found_node == NULL)
    throw yshell_exn ("File does not exist");
  else{
    wordvec file_contents = found_node->readfile();
    for(unsigned int vec_itor = 0; vec_itor < file_contents.size();
	vec_itor++)
      cout << file_contents[vec_itor] << " ";
    cout << "\n";
  }
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_cd (inode_state &state, const wordvec &words){
  const string root = "/";
  const string old_cwd = state.get_cwd_string();
  wordvec cd_vec = words;
  //Removes command from vector, size is decremented
  cd_vec.erase (cd_vec.begin());
  //Store the Name of the file
  string filename = cd_vec.front();
  if(filename[0] == '/'){
    inode *to_root = state.get_root();
 //   inode cdir (DIR_INODE);
    state.change_cwd(*to_root);
    state.to_root();
    wordvec dir_change = split(cd_vec.front(), root);
    for(unsigned int vec_itor = 0; vec_itor < dir_change.size(); vec_itor++){
      state.append_cwd_string(dir_change[vec_itor]);
      inode *cdir = state.locateinode( dir_change[vec_itor] );
      if (cdir == NULL){
	state.set_cwd_string(old_cwd);
	throw yshell_exn ("Directory does not exist");
      }
      else
	state.change_cwd(*cdir);
    }
  }
  else if(filename[0] == '.' && filename[1] == '.'){
    string dot_dot = "..";
    state.remove_dir_string();
    inode *up_dir = state.locateinode(dot_dot);
    state.change_cwd(*up_dir);
    wordvec dir_change = split(cd_vec.front(), root);
    dir_change.erase(dir_change.begin());
    for(unsigned int vec_itor = 0; vec_itor < dir_change.size(); vec_itor++){
      if(dir_change[vec_itor] == "..")
        state.remove_dir_string();
      else
        state.append_cwd_string(dir_change[vec_itor]);
      inode *cdir = state.locateinode( dir_change[vec_itor] );
      if (cdir == NULL){
	state.set_cwd_string(old_cwd);
	throw yshell_exn ("Directory does not exist");
      }
      else
	state.change_cwd(*cdir);
    }
  }
  else{
    state.append_cwd_string(filename);
    inode *found_node = state.locateinode(filename);
    if (found_node == NULL){
      state.set_cwd_string(old_cwd);
      throw yshell_exn ("Directory does not exist");
    }
    else
      state.change_cwd(*found_node);
  }
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
  const string root = "/";
  const string old_cwd = state.get_cwd_string();
  wordvec ls_vec = words;
  //Removes command from vector, size is decremented
  ls_vec.erase (ls_vec.begin());
  string filename = ls_vec.front();
  //inode tmp_cwd = state.get_cwd(); 
  //state.change_tmp(tmp_cwd);
  if (filename[0] == '/' || (filename[0] == '.' && filename[1] == '.')){
    inode *fake_cwd = state.get_root();
    if(filename[0] == '/'){
      //inode *to_root = state.get_root();
      //   inode cdir (DIR_INODE);
      //state.change_tmp(*to_root);
      wordvec dir_change = split(ls_vec.front(), root);
      for(unsigned int vec_itor = 0; vec_itor < dir_change.size(); vec_itor++){
	inode *cdir = state.locateinode( dir_change[vec_itor] );
	if (cdir == NULL){
	  state.set_cwd_string(old_cwd);
	  throw yshell_exn ("Directory does not exist");
	}
	else
	  fake_cwd = cdir;
      }
    }
    else if(filename[0] == '.' && filename[1] == '.'){
      string dot_dot = "..";
      state.remove_dir_string();
      inode *up_dir = state.locateinode(dot_dot);
      //state.change_cwd(*up_dir);
      fake_cwd = up_dir;
      wordvec dir_change = split(ls_vec.front(), root);
      dir_change.erase(dir_change.begin());
      for(unsigned int vec_itor = 0; vec_itor < dir_change.size(); vec_itor++){
	//if(dir_change[vec_itor] == "..")
	//  state.remove_dir_string();
	//else
        //state.append_cwd_string(dir_change[vec_itor]);
	inode *cdir = state.locateinode( dir_change[vec_itor] );
	if (cdir == NULL){
	  state.set_cwd_string(old_cwd);
	  throw yshell_exn ("Directory does not exist");
	}
	else
	  fake_cwd = cdir;
      }
    }
    //inode cwd = state.get_cwd();
    //cwd.list();
    //state.change_cwd(tmp_cwd);
    fake_cwd->list();
  }
  else{
    inode cwd = state.get_cwd();
    cwd.list();
   }
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
  inode newfile = cwd.mkfile(filename);
  //Now add the contents of words into newfile
  newfile.writefile(make_vec);
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_mkdir (inode_state &state, const wordvec &words){
  wordvec mkdir_vec = words;
  //Removes command from vector, size is decremented
  mkdir_vec.erase (mkdir_vec.begin());
  //Store the Name of the file
  string dirname = mkdir_vec.front();
  //Now create a directory
  inode cwd = state.get_cwd();
  cwd.mkdir(dirname);
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
  string pwd_string = state.get_cwd_string();
  cout << pwd_string << endl;
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rm (inode_state &state, const wordvec &words){
  wordvec rm_vec = words;
  //Removes command from vector, size is decremented
  rm_vec.erase (rm_vec.begin());
  //Store the Name of the file
  string filename = rm_vec.front();
  inode *file_test = state.locateinode( filename );
  if (file_test == NULL){
    throw yshell_exn ("File does not exist");
  }
  else{
    inode cwd = state.get_cwd();
    cwd.remove(filename);
  }
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

