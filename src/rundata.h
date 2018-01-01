#ifndef __RUNSAMPLE__
#define __RUNSAMPLE__

#include <cstring>
#include <string>
#include <vector>
#include <set>

struct RunSample
{
  std::string Lane;
  std::string sampleID;
  std::string sampleName;
  std::string i7IndexID;
  std::string i7index;
  std::string i5IndexID;
  std::string i5index;
  std::string projectName;
};

struct RunData
{
  char workingDirectory[256];
  std::string sampleSheet;
  std::string outputDirectory;
  std::string inputDirectory;
  bool pairedEnd;
  bool debug;
  char projectName[256];
  std::set<std::string> projectNames;
  std::vector<RunSample *> Samples;
} ;

struct RefData
{
  std::string reference;
  std::string javamem; 
  std::string mills;
  std::string dbsnp;
  std::string g1000;
  std::string hapmap;
  std::string genomeDir;
  char type;
  int call; 
};
#endif
