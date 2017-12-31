

#ifndef __VARIENTRECALIBRATION__
#define __VARIENTRECALIBRATION__

#include <cstring>
#include <vector>
#include "rundata.h"

void VariantRecalibrator(std::string sample, std::string& outputDirectory, std::string mode, std::string& reference, std::string& mills, std::string& dbsnp, std::string& hapmap, std::string& g1000);
void runVarientRecalibration(RunData * run, std::string mode, RefData * ref);
void ApplyRecalibration(std::string sample, std::string& outputDirectory, std::string mode, std::string& reference);

#endif
