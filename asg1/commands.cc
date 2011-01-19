// $Id: commands.cc,v 1.45 2011-01-18 23:02:42-08 - - $
// Authors: Ben Ross, Erik Steggall
// Usernames: bpross@ucsc.edu, esteggal@ucsc.edu

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
  string pathname;
  size_t found;
  string slash = "/";
  for(unsigned int vec_itor = 0; vec_itor < cat_vec.size(); vec_itor++){
    string old_cwd = state.get_cwd_string();
    pathname = cat_vec[vec_itor];
    found = pathname.find(slash);
    if (found != string::npos)
      pathname.insert(0,slash);
    inode *cwd = state.get_cwd();
    string cat_file;
    if(pathname[0] == '/'){
      inode *to_root = state.get_root();
      //   inode cdir (DIR_INODE);
      state.change_cwd(*to_root);
      state.to_root();
      wordvec dir_change = split(cat_vec[vec_itor], slash);
      cat_file = dir_change.back();
      dir_change.erase(dir_change.end());

      for(unsigned int vec_iter = 0; vec_iter< dir_change.size(); vec_iter++){
	state.append_cwd_string(dir_change[vec_iter]);
	inode *cdir = state.locateinode( dir_change[vec_iter] );
	if (cdir == NULL){
	  state.set_cwd_string(old_cwd);
	  throw yshell_exn("File does not exist");
	}
	else
	  state.change_cwd(*cdir);
      }
    }
    inode *found_node = state.locateinode(cat_file);
    if (found_node == NULL)
      throw yshell_exn ("File does not exist");
    else{
      wordvec file_contents = found_node->readfile();
      for(unsigned int vec_itor = 0; vec_itor < file_contents.size();
	  vec_itor++)
	cout << file_contents[vec_itor] << " ";
      cout << "\n";
    }
    state.change_cwd(*cwd);
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
  // Need to store the current cwd, recursion makes this
  // tricky, hence global variable hack.
  inode *cwd = state.get_cwd();
  if(hack == 0) state.change_tmp(*cwd);
  hack++;

  //initalize variables, store the words after lsr command
  map<string, inode *>::iterator it;
  wordvec lsr_vec = words;
  lsr_vec.erase(lsr_vec.begin());
  string pathname;
  size_t found;
  for(unsigned int vec_iter=0;vec_iter<lsr_vec.size();vec_iter++){
    cout << "called" << endl;
    string old_cwd = state.get_cwd_string();
    pathname = lsr_vec[vec_iter];
    found = pathname.find(slash);
    if (found != string::npos)
      pathname.insert(0,"/");
    if (pathname[0] == '/'){
      inode *to_root = state.get_root();
      state.change_cwd(*to_root);
      state.to_root();
      wordvec dir_change = split(lsr_vec[vec_iter],"/");
      
      for(unsigned int vec_iter = 0; vec_iter< dir_change.size(); vec_iter++){
	state.append_cwd_string(dir_change[vec_iter]);
	inode *cdir = state.locateinode( dir_change[vec_iter] );
	if (cdir == NULL){
	  state.set_cwd_string(old_cwd);
	  throw yshell_exn("File does not exist");
	}
	else
	  state.change_cwd(*cdir);
      }
    }/*
    // Call ls preorder
    fn_ls(state, words);
    // Get the dirents of cwd
    directory cwd_dirents = cwd->get_directory();
    // For all dirents in the directory:
    for( it = cwd_dirents.begin(); it != cwd_dirents.end(); it++){
      
      // Get the filename
      string current_file = it->first;
      // Skip through if it's . or ..
      if(current_file == "." || current_file == "..") continue;
      
      // Makes sure that cwd_string is corrent 
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
      
      // Create a wordvec to pass back into lsr, needs an 
      // lsr at the front so it doesn't delete the filename
      wordvec curr_file;
      curr_file.push_back("lsr");
      curr_file.push_back(current_file);
      // Find the node
      inode *change_node = state.locateinode(current_file);
      // check to make sure it's a directory
      int type = change_node->get_type();
      if (type != 1){
	state.change_cwd(*change_node);
	// call lsr recurvisly
	fn_lsr(state, curr_file);
      }
      }*/
    wordvec curr_file;
    curr_file.push_back("lsr");
    print_lsr(state,curr_file);
  }
  if (lsr_vec.size() == 0){
    wordvec curr_file;
    curr_file.push_back("lsr");
    print_lsr(state,curr_file);
  }

  // set hack back to 0
  hack = 0;
  // change cwd back
  inode *change_node_back = state.get_tmp();
  state.change_cwd(*change_node_back);

  TRACE ('c', state);
  TRACE ('c', words);
}

void print_lsr(inode_state &state, const wordvec &words){
  map<string, inode *>::iterator it;
  // Call ls preorder
  if (!words.empty()){
    fn_ls(state, words);
  }
  // Get the dirents of cwd
  inode *cwd = state.get_cwd();
  directory cwd_dirents = cwd->get_directory();
  
  // For all dirents in the directory:
  for( it = cwd_dirents.begin(); it != cwd_dirents.end(); it++){
  
    // Get the filename
    string current_file = it->first;
    // Skip through if it's . or ..
    if(current_file == "." || current_file == "..") continue;
    
    // Makes sure that cwd_string is corrent 
    string pwd_string = state.get_cwd_string();
    if(pwd_string.compare("/") != 0){
      wordvec pathname = split(pwd_string, "/");
      string end = pathname.back();
      if(current_file.compare(end) != 0 ){
	state.append_cwd_string(current_file);
      }
    }else{
      state.append_cwd_string(current_file);
    }
    
    // Create a wordvec to pass back into lsr, needs an 
    // lsr at the front so it doesn't delete the filename
    wordvec curr_file = words;
    curr_file.push_back("lsr");
    curr_file.push_back(current_file);
    // Find the node
    inode *change_node = state.locateinode(current_file);
    // check to make sure it's a directory
    int type = change_node->get_type();
    if (type != 1){
      state.change_cwd(*change_node);
      // call lsr recurvisly
      print_lsr(state, curr_file);
    }
  }
}

void fn_make (inode_state &state, const wordvec &words){
  //Following lines store the cwd because it is changed in function
  inode *cwd = state.get_cwd();
  state.change_tmp(*cwd);

  //Intialize variables
  string dir_path = words[1];
  string file_name;
  // used to store the char at the rit pointer 
  string tmp;
  string::reverse_iterator it;
  int path_size = dir_path.size();
  int path_end = path_size;
  int ool = 0;
  wordvec cd_words;
  // This loop runs backwards through the list and it deletes
  // everything up until the '/' it stores everything it deletes
  for(it = dir_path.rbegin(); it < dir_path.rend(); it++){
    if(*it == '/'){
       ool = 1;
       break;
    }
    tmp = *it;
    file_name.insert(0, tmp);
    path_size--;
    dir_path.erase(path_size, path_end);
  }
  // If the file being created had a path, do this:
  if(ool == 1){
    //This will change the directory to the path given
    cd_words.push_back("cd");
    cd_words.push_back(dir_path);
    fn_cd(state, cd_words);
    cwd = state.get_cwd();
  }

  wordvec make_vec = words;
  //Removes command from vector, size is decremented
  make_vec.erase (make_vec.begin());
  //Store the Name of the file
  string filename = make_vec.front();
  //Remove the filename from rest
  make_vec.erase (make_vec.begin());
  //Now create a new file
  inode *check = state.locateinode(filename);
  // check to make sure file isn't already created
  if (check != NULL){
    int type = check->get_type();
    if (type == 0)
      throw yshell_exn ("Cannot make file name same as a directory");
    else
      throw yshell_exn ("File already exists");
  }
  else{
    inode newfile = cwd->mkfile(file_name);
    //Now add the contents of words into newfile
    newfile.writefile(make_vec);
  }
  //Change cwd back to what it was before function was called
  inode *change_node_back = state.get_tmp();
  state.change_cwd(*change_node_back);
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_mkdir (inode_state &state, const wordvec &words){
  //Following lines store the cwd because it is changed in function
  inode *cwd = state.get_cwd();
  state.change_tmp(*cwd);

  //Intialize variables
  string dir_path = words[1];
  string dir_name;
  // used to store the char at the rit pointer 
  string tmp;
  string::reverse_iterator it;
  int path_size = dir_path.size();
  int path_end = path_size;
  int ool = 0;
  wordvec cd_words;
  // This loop runs backwards through the list and it deletes
  // everything up until the '/' it stores everything it deletes
  for(it = dir_path.rbegin(); it < dir_path.rend(); it++){
    if(*it == '/'){
       ool = 1;
       break;
    }
    tmp = *it;
    dir_name.insert(0, tmp);
    path_size--;
    dir_path.erase(path_size, path_end);
  }
  // If the file being created had a path, do this:
  if(ool == 1){
    //This will change the directory to the path given
    cd_words.push_back("cd");
    cd_words.push_back(dir_path);
    fn_cd(state, cd_words);
    cwd = state.get_cwd();
  }
  //Now create a directory
  cwd->mkdir(dir_name);
  //Change cwd back to what it was before function was called
  inode *change_node_back = state.get_tmp();
  state.change_cwd(*change_node_back);
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
  //string used to store pwd
  vector<string> pwd_string;
  //stores string returned from get_inode_name()
  string tmp_cwd;
  directory cwd_dirents;
  inode *cwd = state.get_cwd();
  // Used to check for root, root is == 1
  int inode_nr = 0;
  inode_nr = cwd->get_inode_nr();
  // While not root, get the inode name and go to parent
  while(inode_nr != 1){
    //stores the current inode
    tmp_cwd = cwd->get_inode_name();
    pwd_string.push_back(tmp_cwd);
    //The following lines change cwd to parent directory
    cwd_dirents = cwd->get_directory();
    cwd = (cwd_dirents)[".."];
    inode_nr = cwd->get_inode_nr();
    
  } 
  // If pwd is called from root:
  if(pwd_string.empty()){
    cout << "/";
  }
  else{
    // String was stored backwards, this prints it correctly
    vector<string>::reverse_iterator rit;
    for(rit = pwd_string.rbegin(); rit < pwd_string.rend(); ++rit){
      cout << "/" << *rit;
    }
  }
  cout << endl;
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rm (inode_state &state, const wordvec &words){
  wordvec rm_vec = words;
  //Removes command from vector, size is decremented
  rm_vec.erase (rm_vec.begin());
  
  //Store the Name of the file
  string pathname;
  size_t found;
  string slash = "/";
  for(unsigned int vec_itor = 0; vec_itor < rm_vec.size(); vec_itor++){
    string old_cwd = state.get_cwd_string();
    pathname = rm_vec[vec_itor];
    found = pathname.find(slash);
    if (found != string::npos)
      pathname.insert(0,slash);
    inode *cwd = state.get_cwd();
    string rm_file;
    if(pathname[0] == '/'){
      inode *to_root = state.get_root();
      //   inode cdir (DIR_INODE);
      state.change_cwd(*to_root);
      state.to_root();
      wordvec dir_change = split(rm_vec[vec_itor], slash);
      rm_file = dir_change.back();
      dir_change.erase(dir_change.end());

      for(unsigned int vec_iter = 0; vec_iter< dir_change.size(); vec_iter++){
	state.append_cwd_string(dir_change[vec_iter]);
	inode *cdir = state.locateinode( dir_change[vec_iter] );
	if (cdir == NULL){
	  state.set_cwd_string(old_cwd);
	  throw yshell_exn("File does not exist");
	}
	else
	  state.change_cwd(*cdir);
      }
    }
    inode *found_node = state.locateinode(rm_file);
    if (found_node == NULL)
      throw yshell_exn ("File does not exist");
    else{
      inode *cwd = state.get_cwd();
      cwd->remove(rm_file);
    }
    state.change_cwd(*cwd);
  }
  TRACE ('c', state);
  TRACE ('c', words);
}

void fn_rmr (inode_state &state, const wordvec &words){
  // Initialize iterator 
  map<string, inode*>::iterator it;
  // Store only the strings entered after rmr entered
  wordvec rmr_vec = words;
  rmr_vec.erase (rmr_vec.begin() );
  string filename = rmr_vec.front();

  // Change cwd to the directory specified
  inode *change_node = state.locateinode(filename);
  state.change_cwd(*change_node);


  // get the dirents of the new directory
  directory *curr_dirents = &(change_node->get_directory() );
  // Cycle through the dirents
  for(it = curr_dirents->begin(); it != curr_dirents->end(); it++){
    // store the name of the file
    const string current_file = it->first;
    
    // skip if the filename is . or ..
    if(current_file == "." || current_file == "..") continue;


    // store filename into a wordvec
    wordvec curr_file;
    curr_file.push_back("rmr");
    curr_file.push_back(current_file);

    // change node to the current filename
    change_node = state.locateinode(it->first);
    int type = change_node->get_type();
    // make sure that it's a directory
    if(type == 1)
      fn_rm(state, curr_file);
    else
      // if it's a directory pass it back into lsr recursivly
      fn_rmr(state, curr_file);
  }

  // Change node back to it's parent
  change_node = (*curr_dirents)[".."];
  // get the new dirents
  curr_dirents = &(change_node->get_directory() );
 
  // change current working directory back
  state.change_cwd(*change_node);
  // call rm post-order
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

