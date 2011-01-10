using namespace std;

#include "commands.h"
#include "trace.h"
#include "inode.h"
#include "util.h"


int main(){
   inode newinode (DIR_INODE);
   int size = newinode.size();
   cout << size;
}
