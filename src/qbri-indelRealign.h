

#ifndef __INDELREALIGN__
#define __INDELREALIGN__

#include <cstring>
#include <vector>
#include "rundata.h"

std::string IndelRealigner (std::string sample, std::string& outputDirectory,std::string& reference, std::string& mills, std::string& dbsnp, char type, bool fixMisecodedQ);
void RealignerTargetCreator(std::string sample, std::string& outputDirectory, std::string& reference, std::string& mills, std::string& dbsnp, char type , bool fixMisecodedQ);
void runIndelRealign(RunData * run, RefData * ref, bool fixMisecodedQ);

#endif
