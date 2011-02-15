// $Id: main.cc,v 1.1 2011-01-25 18:56:05-08 - - $
//bpross, esteggall

#include <fstream>
#include <iostream>

using namespace std;

#include "interp.h"
#include "trace.h"
#include "util.h"

//
// Parse a file.  Read lines from input file, parse each line,
// and interpret the command.
//

void parsefile (const string &infilename,
                     istream &infile, ostream &outfile) {
   objectmap objmap;
   interpreter interp (infilename, outfile, objmap);
   for (int linenr = 1;; ++linenr) {
      try {
         string line;
         getline (infile, line);
         if (infile.eof()) break;
         if (line.size() == 0) continue;
         for (;;) {
            TRACE ('m', line);
            int last = line.size() - 1;
            if (line[last] != '\\') break;
            line[last] = ' ';
            string contin;
            getline (infile, contin);
            if (infile.eof()) break;
            line += contin;
         }
         list<string> words = split (line, " \t");
         if (words.size() == 0 || words.front()[0] == '#') continue;
         TRACE ('m', words);
         interp.interpret (words);
      }catch (runtime_error error) {
         complain() << infilename << ":" << linenr << ": "
                    << error.what() << endl;
      }
   }
   TRACE ('m', infilename << " EOF");
}

//
// Strip off the dirname portion and the suffix and tack on .ps.
//
string get_outfilename (const string &infilename) {
      string suffix = ".dr";
      int slashpos = infilename.find_last_of ('/') + 1;
      string outname = infilename.substr (slashpos);
      int baselen = outname.size();
      int suffixlen = suffix.size();
      int difflen = baselen - suffixlen;
      if (baselen > suffixlen && outname.substr (difflen) == suffix) {
         outname = outname.substr (0, difflen);
      }
      return outname + ".ps";
}

//
// Scan the option -D and check for operands.
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
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

//
// Main function.  Iterate over files if given, use cin if not.
//
int main (int argc, char **argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   if (optind == argc) {
      parsefile ("-", cin, cout);
   }else {
      for (int argi = optind; argi < argc; ++argi) {
         const string infilename = argv[argi];
         ifstream infile (infilename.c_str());
         if (infile.fail()) {
            syscall_error (infilename);
            continue;
         }
         const string outfilename = get_outfilename (infilename);
         ofstream outfile (outfilename.c_str());
         if (outfile.fail()) {
            syscall_error (outfilename);
            infile.close();
            continue;
         }
         TRACE ('m', infilename << " => " << outfilename);
         parsefile (infilename, infile, outfile);
         infile.close ();
         outfile.close ();
      }
   }
   return sys_info::get_exit_status ();
}

