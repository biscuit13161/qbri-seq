#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include "config.h"
#include "qbri-hapCall.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

void runHapCaller(RunData * run, RefData * ref)
{
  set<string> samples;
  // it;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    haplotypeCaller(sample->sampleID,run->outputDirectory, ref->reference, ref->type);
  }
}

void haplotypeCaller(string sample, string& outputDirectory, string& reference, char type)
{
  string inputFile = outputDirectory;
  inputFile += "/" + sample + ".pr.bam";
  string outputFile = outputDirectory;
  outputFile += "/" + sample + ".hc.vcf";

  cout << endl << "\tCalling Variants for " << sample << endl << endl;

  string options;
  if (type == 'D') {
    options = " -stand_call_conf 30 --variant_index_type LINEAR -variant_index_parameter 128000 -maxAltAlleles 10 --generate_md5";
  } else {
    options = " -stand_call_conf 20.0 -dontUseSoftClippedBases";
  }


  string cmd = string(JAVA) + " "+ string(GATKMEM) + " -jar " + string(GATK) +" -T HaplotypeCaller -I " + inputFile +" -o "+ outputFile +" -R "+ reference + " -nct " + string(GATK_CORES) + " " + options;
  execCmd(cmd);
}

/*
java -Xmx16g -jar /apps/GATK/GenomeAnalysisTK.jar \
-T HaplotypeCaller -I <file>.pr.bam -o <file>.hc.vcf \
-R /hg38/Homo_sapiens_assembly38.fasta -stand_call_conf 30 -nct 4 \
--variant_index_type LINEAR -variant_index_parameter 128000

// */

