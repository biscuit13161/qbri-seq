#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <regex>
#include <sys/wait.h>
#include <unistd.h>  // provides execlp()
#include <regex>
#include "getFileList.h"
#include "qbri-bwa.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"
#include "misc.h"

using namespace std;

void alignReads (string sample, string& outputDirectory,vector<string> files, string& ref)
{
      cout << ref << endl;
  
  string regex_string = ".*" + sample + ".*\\.fastq\\.gz";
  regex sam(regex_string);
  regex r1(".*R1.*");
  regex r2(".*R2.*");
  string fastq1;
  string fastq2;

  for (vector<string>::iterator vt = files.begin(); vt != files.end(); ++vt)
  {
    if (regex_match(*vt,sam) )
    {
      if (regex_match(*vt,r1) )
      {
        fastq1 = *vt;
      }
      else if (regex_match(*vt,r2) )
      {
        fastq2 = *vt;
      }
    }
  } 
  string output_file = outputDirectory + "/" + sample + ".bam";
  cout << endl << "\tAligning " << sample << endl << endl;

  //string readGroup = "'@RG\tID:"+sample+"\tLB:" + sample + "\tPL:Illumina\tPU:2500\tSM:" + sample +"\n'";

  //*
    cout << "Output_file: " << output_file << endl;
    string exec2 = "bwa mem -t 8 " + ref + " " + fastq1 + " " + fastq2 + " | samtools sort -@8 -O BAM -o " + output_file ;
    // cout << exec2 << endl;
    
  execCmd(exec2);
  //*/
}

void runBWA(RunData * run,RefData * ref)
{
  cout << endl << "runBWA: " << ref->reference << endl << endl;
  set<string> samples;
  cout << run->outputDirectory << endl;
  vector<string> files = getFileList(run->outputDirectory);  
  int count = 1;

  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    cout << sample->sampleID << endl;
    samples.insert(sample->sampleID);
    alignReads(sample->sampleID,run->outputDirectory,files,ref->reference);
    //sortReads(sample->sampleID,run->outputDirectory);
    string output = ReadGroups(sample->sampleID,run->outputDirectory);
    runBamQC(sample->sampleID, output);
    cout << count << " " << sample->sampleID <<endl;
    count++;
  }
}

void sortReads(string sample, string& outputDirectory)
{
  const char* inputfile = (outputDirectory + "/" + sample + ".sam").c_str();
  const char* outputfile = (outputDirectory + "/" + sample + ".sort.bam").c_str();
  cout << "\tSorting " << sample << endl;
  int pid = fork();
  if (pid > 0)
  {
    int status;
    waitpid(pid,&status,0);
  }
  else if (pid == 0)
    execlp("/usr/local/bin/samtools", "samtools","sort","-o",outputfile,inputfile, NULL);


}

/*string ReadGroups(string sample, string& outputDirectory, string& picard)
{
  string input = "I=" + outputDirectory + "/" + sample + ".bam";
  string output_file = outputDirectory + "/" + sample + ".rg.bam";
  string output = "O=" + output_file;
  string readLib = "RGLB=" + sample;
  string readPlat = "RGPL=Illumina";
  string readPlatUnit = "RGPU=01";
  string readSample = "RGSM=" + sample;
  cout << endl << "\tAdding Read Groups for " << sample << endl << endl;

//*
  int pid = fork();
  if (pid > 0)
  {
    int status;
    waitpid(pid,&status,0);
  }
  else if (pid == 0)
    execlp("/usr/bin/java", "java","-jar",picard.c_str(),"AddOrReplaceReadGroups",input.c_str(),output.c_str(),"CREATE_MD5_FILE=true","CREATE_INDEX=true",readLib.c_str(),readPlat.c_str(),readPlatUnit.c_str(),readSample.c_str(), NULL);
/*
java -jar /apps/picard-tools/picard-2.8.2.jar \
AddOrReplaceReadGroups I=<file>.sort.bam \
O=<file>.rg.bam CREATE_MD5_FILE=true CREATE_INDEX=true \
RGLB=Lib1 RGPL=Illumina RGPU=01 RGSM=Test


 return output_file;
}
*/


