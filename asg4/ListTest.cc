#include "List.h"

template<typename T>
void helper_func(bool unique){
  List<T> List;
  T push_item;
  while(!cin.eof()){
    cin >> push_item;
    cout << push_item;
    List.push(push_item,unique);
  }
  List.sort();
  cout << List;
}

int main(int argc, char** argv){
  int itor;
  bool unique = false;
  for(itor = 0; itor < argc; ++itor){
    if(itor > 0){
      cout << argv[itor] << endl;
      if(argv[itor] == "-u"){
        cout << "Unique" << endl;
        unique = true;
      }else if(argv[itor] == "int"){
        cout << "Made it" << endl;
        helper_func<int>(unique);
      }else if(argv[itor] == "double"){
        helper_func<double>(unique);
      }else if(argv[itor] == "string"){
        helper_func<string>(unique);
      }else
       cout << "GAY" << endl;
    }
  }
}
    
