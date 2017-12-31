

#ifndef __HAPCALLER__
#define __HAPCALLER__

#include <cstring>
#include <vector>
#include "rundata.h"

void runHapCaller(RunData * run, RefData * ref);
void haplotypeCaller(std::string sample, std::string& outputDirectory, std::string& reference, char type);

#endif
