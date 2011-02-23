// $Id: yshell.cc,v 1.18 2011-02-10 16:31:30-08 - - $
// Authors: Ben Ross, Erik Steggall
// Usernames: bpross@ucsc.edu, esteggal@ucsc.edu

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <unistd.h>

using namespace std;

#include "commands.h"
#include "trace.h"
#include "inode.h"
#include "util.h"

//
// scan_options
//    Options analysis:  The only option is -Dflags. 
//

void scan_options (int argc, char **argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) option << ": invalid option"
                       << endl;
            break;
      }
   }
   if (optind < argc) {
      complain() << "operands not permitted" << endl;
   }
}

//
// main -
//    Main program which loops reading commands until end of file.
//

int main (int argc, char **argv) {
   setexecname (argv[0]);
   cout << boolalpha; // Print false or true instead of 0 or 1.
   cerr << boolalpha;
   scan_options (argc, argv);
   bool need_echo = want_echo();
   commands cmdmap;
   inode_state state;
   string prompt = state.get_prompt();
   inode start_root (DIR_INODE);
   start_root.mkroot(start_root);
   state.change_root(start_root);
   state.change_cwd(start_root);
   try {
      for (;;) {
         try {
   
            // Read a line, break at EOF, and echo print the prompt
            // if one is needed.
            cout << prompt << " ";
            string line;
            getline (cin, line);
            if (cin.eof()) {
               if (need_echo) cout << "^D";
               cout << endl;
               TRACE ('y', "EOF");
               break;
            }
            if (need_echo) cout << line << endl;
   
            // Split the line into words and lookup the appropriate
            // function.  Complain or call it.
            wordvec words = split (line, " \t");
            TRACE ('y', "words = " << words);
            string comment = words[0];
            if (!(comment == "#")){
              function fn = cmdmap[words[0]];
              if (fn == NULL) {
                throw yshell_exn (words[0] + ": no such function");
              }
              fn (state, words);
            }
            prompt = state.get_prompt();
         }catch (yshell_exn exn) {
            // If there is a problem discovered in any function, an
            // exn is thrown and printed here.
            complain() << exn.what() << endl;
         }
      }
   } catch (ysh_exit_exn) {
   }

   return exit_status_message();
}

