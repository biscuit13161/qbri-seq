#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "getFileSize.h"

using namespace std;

int main (int argc, char *argv[] )
{
  long file = getFileSize(string(argv[1]));
  cout << "File is " << file << endl;
  cout << file/1024 << "KB" << endl;
  cout << file/(1024*1024) << "MB" << endl;
  cout << file/(1024*1024*1024) << "GB" << endl;
  if ( file > 1050000 )
    cout << " File is bigger than 1MB" <<endl;
  return 0;
}
