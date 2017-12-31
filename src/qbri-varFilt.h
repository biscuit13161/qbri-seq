

#ifndef __VARIENTFILTRATION__
#define __VARIENTFILTRATION__

#include <cstring>
#include <vector>
#include "rundata.h"

void VariantFilter(std::string sample, std::string& outputDirectory, std::string& reference);
void runVarientFiltration(RunData * run, RefData * ref);

#endif
