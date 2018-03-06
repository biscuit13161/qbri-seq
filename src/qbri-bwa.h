
#ifndef __BWA__
#define __BWA__

#include <cstring>
#include <vector>
#include "rundata.h"

void alignReads (std::string sample, std::string& outputDirectory,std::vector<std::string> files, std::string& ref, bool verbose);
void runBWA(RunData * run,RefData * ref);

#endif
