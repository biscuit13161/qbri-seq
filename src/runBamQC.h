
#ifndef __runBqmQC__
#define __runBqmQC__

#include <string>

void runBamQC(std::string sample, std::string file);
void flagstat(std::string& sample, std::string& file);
void bamqc(std::string& sample, std::string& file);
void execCmd (std::string& exec);

#endif
