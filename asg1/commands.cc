// $Id: commands.cc,v 1.145 2011-01-18 17:07:56-08 - - $

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
   map["rmr"    ] = fn_rmr    ;
}

function commands::operator[] (const string& cmd) {
   return map[cmd];
}

void fn_cat (inode_state &state, const wordvec &words){
  wordvec cat_vec = words;
  //Erases the command from the vector, size is decremented
  cat_vec.erase (cat_vec.begin());
  for(unsigned int vec_itor = 0; vec_itor < cat_vec.size(); vec_itor++){
    inode *found_node = state.locateinode(cat_vec[vec_itor]);
    if (found_node == NULL)
      throw yshell_exn ("File does not exist");
    else{
      wordvec file_contents = found_node->readfile();
      for(unsigned int vec_itor = 0; vec_itor < file_contents.size();
	  vec_itor++)
	cout << file_contents[vec_itor] << " ";
      cout << "\n";
    }
  }
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_cd (inode_state &state, const wordvec &words){
  const string root = "/";
  wordvec cd_vec = words;
  string old_cwd = state.get_cwd_string();
  //Removes command from vector, size is decremented
  cd_vec.erase (cd_vec.begin());
  //Store the Name of the file
  size_t found;
  string filename = cd_vec.front();
  found = filename.find("/");
  if (found != string::npos)
    filename.insert(0,"/");
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
	throw yshell_exn("Directory does not exist");

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
  wordvec ls_vec = words;
  
  //Removes command from vector, size is decremented
  ls_vec.erase (ls_vec.begin());
  size_t found;
  string filename = ls_vec.front();
  string fake_cwd_string = "/";
  found = filename.find("/");
  if (found != string::npos)
    filename.insert(0,"/");
  if (filename[0] == '/' || (filename[0] == '.' && filename[1] == '.')){
    inode *fake_cwd = state.get_root();
    if(filename[0] == '/'){  
      wordvec dir_change = split(ls_vec.front(), root);
      for(unsigned int vec_itor = 0; vec_itor < dir_change.size(); vec_itor++){
	inode *cdir = state.locateinode( dir_change[vec_itor] );
	if (cdir == NULL){
	  throw yshell_exn ("Directory does not exist");
	}
	else{
	  fake_cwd_string.append(dir_change[vec_itor]);
	  fake_cwd = cdir;
	}
      }
    }
    else if(filename[0] == '.' && filename[1] == '.'){
      string dot_dot = "..";
      state.remove_dir_string();
      inode *up_dir = state.locateinode(dot_dot);
      
      fake_cwd = up_dir;
      wordvec dir_change = split(ls_vec.front(), root);
      dir_change.erase(dir_change.begin());
      for(unsigned int vec_itor = 0; vec_itor < dir_change.size(); vec_itor++){
	
	inode *cdir = state.locateinode( dir_change[vec_itor] );
	if (cdir == NULL)
	  throw yshell_exn ("Directory does not exist");
	else{
	  fake_cwd_string.append(dir_change[vec_itor]);
	  fake_cwd = cdir;
	}
      }
    }
    cout << fake_cwd_string << endl;
    fake_cwd->list();
  }
  else{
    fn_pwd(state, ls_vec);
    inode *cwd = state.get_cwd();
    cwd->list();
   }
  TRACE ('c', state);
  TRACE ('c', words);
}

int hack = 0;

void fn_lsr (inode_state &state, const wordvec &words){
  const string slash = "/";
  inode *cwd = state.get_cwd();
  if(hack == 0) state.change_tmp(*cwd);
  hack++;
  map<string, inode *>::iterator it;
  wordvec lsr_vec = words;
  lsr_vec.erase (lsr_vec.begin() );
  string filename = lsr_vec.front();
  fn_ls(state, words);
  directory cwd_dirents = cwd->get_directory();
  for( it = cwd_dirents.begin(); it != cwd_dirents.end(); it++){

    string current_file = it->first;
    if(current_file == "." || current_file == "..") continue;

    string pwd_string = state.get_cwd_string();
    if(pwd_string.compare(slash) != 0){
      wordvec pathname = split(pwd_string, slash);
      string end = pathname.back();
      if(current_file.compare(end) != 0 ){
        state.append_cwd_string(current_file);
      }
    }else{
      state.append_cwd_string(current_file);
    }

    wordvec curr_file;
    curr_file.push_back("crap");
    curr_file.push_back(current_file);
    inode *change_node = state.locateinode(current_file);
    int type = change_node->get_type();
    if (type != 1){
      state.change_cwd(*change_node);
      fn_lsr(state, curr_file);
    }
  } 
  hack = 0;
  inode *change_node_back = state.get_tmp();
  state.change_cwd(*change_node_back);

  TRACE ('c', state);
//  TRACE ('c', words);
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
  inode *cwd = state.get_cwd();
  inode *check = state.locateinode(filename);
  if (check != NULL){
    int type = check->get_type();
    if (type == 0)
      throw yshell_exn ("Cannot make file name same as a directory");
    else
      throw yshell_exn ("File already exists");
  }
  else{
    inode newfile = cwd->mkfile(filename);
    //Now add the contents of words into newfile
    newfile.writefile(make_vec);
  }
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_mkdir (inode_state &state, const wordvec &words){
  string dirname = words[1];
  //Now create a directory
  inode *cwd = state.get_cwd();
  cwd->mkdir(dirname);
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
  vector<string> pwd_string;
  string tmp_cwd;
  directory cwd_dirents;
  inode *cwd = state.get_cwd();
//  pwd_string.push_back("/");
//  pwd_string = cwd->get_inode_name();
  int inode_nr = 0;
  inode_nr = cwd->get_inode_nr();
  while(inode_nr != 1){
    tmp_cwd = cwd->get_inode_name();
    pwd_string.push_back(tmp_cwd);
    inode_nr = cwd->get_inode_nr();
    cwd_dirents = cwd->get_directory();
    cwd = (cwd_dirents)[".."];
    inode_nr = cwd->get_inode_nr();
    
  } 
  if(pwd_string.empty()){
    cout << "/";
  }
  else{
    vector<string>::reverse_iterator rit;
    for(rit = pwd_string.rbegin(); rit < pwd_string.rend(); ++rit){
      cout << "/" << *rit;
    }
  }
  cout << endl;
/*
  string pwd_string = state.get_cwd_string();
  cout << pwd_string << endl;
*/
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rm (inode_state &state, const wordvec &words){
  wordvec rm_vec = words;
  //Removes command from vector, size is decremented
  rm_vec.erase (rm_vec.begin());
  //Store the Name of the file
  string filename = rm_vec.front();
  inode *cwd = state.get_cwd();
  cwd->remove(filename);
  
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rmr (inode_state &state, const wordvec &words){
  
  map<string, inode*>::iterator it;
  wordvec rmr_vec = words;
  rmr_vec.erase (rmr_vec.begin() );
  string filename = rmr_vec.front();

  inode *change_node = state.locateinode(filename);
  state.change_cwd(*change_node);


  directory *curr_dirents = &(change_node->get_directory() );

  for(it = curr_dirents->begin(); it != curr_dirents->end(); it++){
    const string current_file = it->first;
    
    if(current_file == "." || current_file == "..") continue;

    wordvec curr_file;
    change_node = state.locateinode(it->first);
    int type = change_node->get_type();
    curr_file.push_back("rmr");
    curr_file.push_back(current_file);
    if(type == 1)
      fn_rm(state, curr_file);
    else
      fn_rmr(state, curr_file);
  }

  change_node = (*curr_dirents)[".."];
  curr_dirents = &(change_node->get_directory() );

 
  state.change_cwd(*change_node);
  fn_ls(state,words);
  fn_rm(state, words);

  TRACE ('c', state);
  TRACE ('c', words);
}
/*
    change_cwd = state.locateinode(it->first);
    state.change_cwd(*change_node);
    cwd_dirents = cwd->get_directory();
*/

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

