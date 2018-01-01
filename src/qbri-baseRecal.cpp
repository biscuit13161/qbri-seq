#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <regex>
#include "config.h"
#include "getFileList.h"
#include "qbri-baseRecal.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

void baseRecal(string sample, string& outputDirectory, int count, string& reference, string& mills, string& dbsnp, string& hapmap)
{
  string inputFile = outputDirectory;
  inputFile += "/" + sample + ".ir.bam";
  cout << endl << "\tRecalibrating Bases for " << sample << endl << endl;

  string cmd = "java " + string(GATKMEM) +" -jar "+ string(GATK) +" -T BaseRecalibrator -I "+ inputFile;
  if (count == 1)
  {
    string outputFile = outputDirectory + "/" + sample + ".br.grp";
    cmd +=  " -o "+ outputFile;
  }
  else
  {
    string outputFile = outputDirectory + "/" + sample + ".br2.grp";
    string bqsrFile = outputDirectory + "/" + sample + ".br.grp";
    cmd +=  " -o "+ outputFile +" -BQSR "+ bqsrFile;
  }


  //int numCPU = thread::hardware_concurrency();
  cmd += " --interval_padding 100 -R "+ reference +" --knownSites "+ mills +" --knownSites "+ dbsnp +" --knownSites "+ hapmap +" -sMode REMOVE_REF_BIAS -nct " + string(GATK_CORES);
  execCmd(cmd);

}


void runBaseRecalibration(RunData * run,RefData * ref)
{
  set<string> samples;
  // it;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    baseRecal(sample->sampleID, run->outputDirectory, 1, ref->reference, ref->mills, ref->dbsnp, ref->hapmap);
    baseRecal(sample->sampleID, run->outputDirectory, 2, ref->reference, ref->mills, ref->dbsnp, ref->hapmap);
    string output = printReads(sample->sampleID, run->outputDirectory,   ref->reference);
    runBamQC(sample->sampleID, output);
  }
}

string printReads(string sample, string& outputDirectory, string& reference)
{
  string inputFile = outputDirectory + "/" + sample + ".ir.bam";
  string outputFile = outputDirectory  + "/" + sample + ".pr.bam";
  string output = sample + ".pr.bam";
  string bqsrFile = outputDirectory  + "/" + sample + ".br2.grp";

  cout << endl << "\tPrinting Reads for " << sample << endl << endl;

  string cmd = "java "+ string(GATKMEM) +" -jar "+ string(GATK) + " -T PrintReads -I "+ inputFile +" -o "+ outputFile +" -BQSR "+ bqsrFile +" -R "+ reference +" --generate_md5";
  execCmd(cmd);

  return output;
}

/*
java -Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T BaseRecalibrator -I <file>.ir.bam -o <file>.br.grp \
-R /hg38/Homo_sapiens_assembly38.fasta  --interval_padding 100 \
--knownSites /hg38/Mills_and_1000G_gold_standard.indels.hg38.vcf.gz \
--knownSites /hg38/dbsnp_146.hg38.vcf.gz \
--knownSites /hg38/hapmap_3.3.hg38.vcf.gz \
-sMode REMOVE_REF_BIAS

java -Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T BaseRecalibrator -I <file>.ir.bam -o <file>.br2.grp \
-BQSR <file>.br.grp \
-R /hg38/Homo_sapiens_assembly38.fasta  --interval_padding 100 \
--knownSites /hg38/Mills_and_1000G_gold_standard.indels.hg38.vcf.gz \
--knownSites /hg38/dbsnp_146.hg38.vcf.gz \
--knownSites /hg38/hapmap_3.3.hg38.vcf.gz \
-sMode REMOVE_REF_BIAS

java -Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T PrintReads -I <file>.ir.bam -o <file>.pr.bam \
-BQSR <file>.br2.grp \
-R /hg38/Homo_sapiens_assembly38.fasta


// */

