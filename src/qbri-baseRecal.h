

#ifndef __BASERECAL__
#define __BASERECAL__

#include <cstring>
#include <vector>
#include "rundata.h"

void baseRecal(std::string sample, std::string& outputDirectory, int count, std::string& reference, std::string& mills, std::string& dbsnp, std::string& hapmap);
void runBaseRecalibration(RunData * run, RefData * ref);
std::string printReads(std::string sample, std::string& outputDirectory, std::string& reference);

#endif
