using namespace std;

#include "commands.h"
#include "trace.h"
#include "inode.h"
#include "util.h"


int main(){
   inode new_dir (DIR_INODE);
   int size = new_dir.size();
   cout << size;
   inode new_file (FILE_INODE);
   new_file.writefile(work);
   
   
}
