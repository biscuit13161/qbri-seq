#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include "config.h"
#include "getFileList.h"
#include "qbri-varFilt.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

void VariantFilter(string sample, string& outputDirectory, string& reference)
{
  string inputFile = outputDirectory;
  inputFile += "/" + sample + ".hc.vcf";
  string outputFile = outputDirectory + "/" + sample + ".vcf";
  cout << endl << "\tFiltering "<< sample << endl << endl;

  string cmd = string(JAVA) + " "+ string(GATKMEM) + " -jar "+ string(GATK) +" -T VariantFiltration -V "+  inputFile +" -o "+  outputFile
        + " -R " +  reference 
        + " -window 35 -cluster 3 -filterName FS -filter \"FS > 30.0\" -filterName QD -filter \"QD < 2.0\" ";
    
  execCmd(cmd);
}

void runVarientFiltration(RunData * run, RefData * ref)
{
  set<string> samples;
  // it;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    samples.insert(sample->sampleID);
    VariantFilter(sample->sampleID, run->outputDirectory, ref->reference);
  }
}
