// $Id: commands.cc,v 1.1 2011-01-11 10:39:39-08 - - $

#include "commands.h"
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
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_cd (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_echo (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_exit (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   throw ysh_exit_exn ();
}

void fn_ls (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_lsr (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_make (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_mkdir (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
}

void fn_prompt (inode_state &state, const wordvec &words){
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

