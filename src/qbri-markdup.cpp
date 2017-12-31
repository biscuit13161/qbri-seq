#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include "config.h"
#include "getFileList.h"
#include "qbri-markdup.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

void markDupes(RunData * run, RefData * ref)
{

  set<string> samples;
  // it;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    string input = "I=" + run->outputDirectory + "/" + sample->sampleID + ".rg.bam";
    string output_file = run->outputDirectory + "/" + sample->sampleID + ".md.bam";
    string output = "O=" + output_file;
    string metrics = "METRICS_FILE=" + run->outputDirectory + "/" + sample->sampleID + ".md.metrics";
    //string input = "I=" + inputfile;
    //string output = "O=" + outputfile;
    //string cmd = string(JAVA) + " " + string(PICARDMEM) +" -jar "+ string(PICARD) +" MarkDuplicates "+ input +" "+ output +" "+ metrics +" REMOVE_DUPLICATES=true CREATE_MD5_FILE=true CREATE_INDEX=true";
    string cmd = string(JAVA) + " " + string(PICARDMEM) + "  -jar "+ string(PICARD) +" MarkDuplicates "+ input +" "+ output +" "+ metrics +" REMOVE_DUPLICATES=true CREATE_MD5_FILE=true CREATE_INDEX=true";
    if (ref->type == 'R')
      cmd += " VALIDATION_STRINGENCY=SILENT";

    execCmd(cmd);
    runBamQC(sample->sampleID, output_file);
  }
}

/*
java –Xmx16g -jar /apps/string(PICARD)-tools/string(PICARD)-2.8.2.jar \
MarkDuplicates I=<file>.rg.bam \
OUTPUT=<file>.md.bam \
METRICS_FILE=<file>.md.metrics \
REMOVE_DUPLICATES=true CREATE_MD5_FILE=true \
CREATE_INDEX=true
//*/
