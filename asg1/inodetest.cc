using namespace std;

#include "commands.h"
#include "trace.h"
#include "inode.h"
#include "util.h"


int main(){
   inode new_dir (DIR_INODE);
   int size = new_dir.size();
   cout << size << "\n";
   inode new_file (FILE_INODE);
   wordvec work;
   work.assign(7,"test");
   cout << "Size of work: " << int (work.size()) << "\n";
   new_file.writefile(work);
   wordvec test;
   test = new_file.readfile();
   cout << test << "\n";
   size = new_file.size();
   cout << size << "\n";

   inode test_dir = new_dir.mkdir("first_dir");
   size = new_dir.size();
   cout << size << "\n";
   inode test_file = new_dir.mkfile("first_file");
   size = new_dir.size();
   cout << size << endl;

   test_file.writefile(work);
   test = new_file.readfile();
   cout << test << "\n";
}
