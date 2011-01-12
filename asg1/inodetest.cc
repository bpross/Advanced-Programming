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
  cout << "contents of new_file: " << test << "\n";
  size = new_file.size();
  cout << "file size of test: " << size << "\n";

  inode test_dir = new_dir.mkdir("first_dir");
  size = new_dir.size();
  cout << "Size of new_dir after calling mkdir: " << size << "\n";
  inode test_file = new_dir.mkfile("first_file");
  size = new_dir.size();
  cout << "Size of new_dir after calling mkfile: " << size << endl;

  wordvec shit;
  shit.assign(4, "test");
  test_file.writefile(shit);
  test = test_file.readfile();
  cout << "contents of test_file: " << test << "\n";
  new_dir.remove("first_file");
  size = new_dir.size();
  cout << "size of new_dir after calling remove(first_file): " << size << endl;
  new_dir.remove("first_dir");

  


  size = new_dir.size();
  cout << "size of new_dir after calling remove(first_dir): " << size << endl;
  test = test_file.readfile();
  cout << "contents of test_file: " << test << "\n";
   

   
}
