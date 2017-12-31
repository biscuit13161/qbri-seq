#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include <thread>
#include "config.h"
#include "getFileList.h"
#include "qbri-indelRealign.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

string IndelRealigner (string sample, string& outputDirectory, string& reference, string& mills, string& dbsnp, char type, bool fixMisecodedQ)
{
  string inputFile;
  if (type == 'D')
    inputFile = outputDirectory  + "/" + sample + ".md.bam";
  else
    inputFile = outputDirectory  + "/" + sample + ".sc.bam";
  string targetIntervals = outputDirectory + "/" + sample + ".rtc.intervals";
  string outputFile = outputDirectory + "/" + sample + ".ir.bam";
  cout << endl << "\tRealigning " << sample << endl <<endl;

  string cmd = string(JAVA) + " "+ string(GATKMEM) +" -Djava.io.tmpdir=" + string(JAVATMP) + " -jar "+ string(GATK) +" -T IndelRealigner -I "+ inputFile +" -o "+ outputFile +" -targetIntervals "+ targetIntervals +" -R "+ reference +" -known "+ mills +" -known "+ dbsnp +" --generate_md5";
  if (fixMisecodedQ == true)
    cmd = cmd + " --fix_misencoded_quality_scores"; 
  execCmd(cmd);
  return outputFile;
}

void runIndelRealign(RunData * run, RefData * ref,bool fixMisecodedQ)
{
  //set<string> samples;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    /* samples.insert(sample->sampleID); */
    if (ref->type == 'D')
        RealignerTargetCreator(sample->sampleID,run->outputDirectory,ref->reference, ref->mills, ref->dbsnp, ref->type, fixMisecodedQ);
    string output = IndelRealigner(sample->sampleID,run->outputDirectory,ref->reference, ref->mills, ref->dbsnp, ref->type,fixMisecodedQ);
    runBamQC(sample->sampleID, output);
  }
}

void RealignerTargetCreator(string sample, string& outputDirectory, string& reference, string& mills, string& dbsnp , char type, bool fixMisecodedQ)
{
  string inputFile;
  if (type == 'D')
    inputFile = outputDirectory  + "/" + sample + ".md.bam";
  else
    inputFile = outputDirectory  + "/" + sample + ".sc.bam";
  string outputFile = outputDirectory + "/" + sample + ".rtc.intervals";
  /*  cout << "rtc " << sample << endl;
    cout << "rtc out " << outputDirectory << endl;
    cout << "rtc ref " << reference << endl;
    cout << "rtc string(GATK) " << string(GATK) << endl;
    cout << "rtc string(PICARDMEM) " << string(PICARDMEM) << endl;
    cout << "rtc mills " << mills << endl;
    cout << "rtc dbsnp " << dbsnp << endl;

    cout << "rtc in " << inputFile << endl;
    cout << "rtc out " << outputFile << endl;
  //*/
  cout << endl << "\tCreating Realigner Target for " << sample << endl << endl;

  //int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
  int numCPU = thread::hardware_concurrency();

  string cmd = string(JAVA) + " " + string(GATKMEM) + " -Djava.io.tmpdir=" + string(JAVATMP) + "  -jar " + string(GATK) +" -T RealignerTargetCreator -I " + inputFile +" -o "+ outputFile +" -nt " + to_string(numCPU) + "  --interval_padding 100 -R "+ reference + " -known " + mills + " -known "+ dbsnp;
  if (fixMisecodedQ == true)
    cmd = cmd + " --fix_misencoded_quality_scores"; 

  execCmd(cmd);
}

/*
java –Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T RealignerTargetCreator -I <file>.md.bam --interval_padding 100 \
-o <file>.rtc.intervals -R /hg38/Homo_sapiens_assembly38.fasta \
-known /hg38/Mills_and_1000G_gold_standard.indels.hg38.vcf.gz\
-known /hg38/dbsnp_146.hg38.vcf.gz

java –Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T IndelRealigner -I <file>.md.bam -o <file>.ir.bam \
-targetIntervals <file>.rtc.intervals \
-R /hg38/Homo_sapiens_assembly38.fasta \
-known /hg38/Mills_and_1000G_gold_standard.indels.hg38.vcf.gz \
-known /hg38/dbsnp_146.hg38.vcf.gz

// */

