
#ifndef __BCL2FASTQ__
#define __BCL2FASTQ__

#include <string>
#include <set>
#include <cstdlib>
#include <vector>
#include "rundata.h"

void bcl2fastq(RunData * run);
void runBcl2fastq(RunData * run);
void runFastqc(RunData * run);
bool checkForOutput(const RunData * run);
void populateRunData(RunData * run);
void getSampleSheet(RunData * run);

#endif
