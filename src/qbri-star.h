
#ifndef __STAR__
#define __STAR__

#include <cstring>
#include <vector>
#include "rundata.h"

using namespace std;

void alignReads (string sample, string& outputDirectory,vector<string> files, string& ref);
void runSTAR(RunData * run,RefData * ref);

#endif
