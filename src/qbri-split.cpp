#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include "config.h"
#include "getFileList.h"
#include "qbri-split.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

void splitCigars(RunData * run, RefData * ref)
{

  set<string> samples;
  // it;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    string input = run->outputDirectory + "/" + sample->sampleID + ".md.bam";
    string output = run->outputDirectory + "/" + sample->sampleID + ".sc.bam";
    //string output = "O=" + output_file;
    string cmd = string(JAVA) + " " + string(GATKMEM) +" -jar "+ string(GATK) +" -T SplitNCigarReads -I "+ input +" -o "+ output +" -R "+ ref->reference +" -rf ReassignOneMappingQuality -RMQF 255 -RMQT 60 -U ALLOW_N_CIGAR_READS";
    execCmd(cmd);
    runBamQC(sample->sampleID, output);
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
