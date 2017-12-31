
#ifndef __GETHELP__
#define __GETHELP__

#include <cstring>
#include <sys/wait.h>
#include <unistd.h>  // provides execlp()
#include "rundata.h"

using namespace std;

void printVersion();
void getReference(RefData * ref);
void sortReads(string sample, string& outputDirectory, char type);
string ReadGroups(string sample, string& outputDirectory);

#endif
