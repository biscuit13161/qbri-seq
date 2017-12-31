#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <regex>
#include <sys/wait.h>
#include <unistd.h>  // provides execlp()
#include <regex>
#include "config.h"
#include "misc.h"
#include "getFileList.h"
#include "qbri-star.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"
#include "getMemorySize.c"

using namespace std;

void alignReads1pass (string sample, string& outputDirectory,vector<string> files, string& ref)
{
      cout << ref << endl;
  
  string regex_string = ".*" + sample + ".*\\.fastq";
  //string regex_string = ".*" + sample + ".*\\.fastq\\.gz";
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
 size_t memorySize = getMemorySize( );
  //string readGroup = "'@RG\tID:"+sample+"\tLB:" + sample + "\tPL:Illumina\tPU:2500\tSM:" + sample +"\n'";

  //  cout << "Output_file: " << output_file << endl;
  //  string exec2 = "STAR --genomeDir " + ref + " --limitGenomeGenerateRAM " + to_string(memorySize) + " --runThreadN 8 --readFilesIn " + fastq1 + " " + fastq2 + " --outFileNamePrefix ./" + sample + "_ --twopassMode Basic";
    string exec2 = "STAR --genomeDir " + ref + " --limitGenomeGenerateRAM " + to_string(memorySize) + " --runThreadN " + string(GATK_CORES) + " --readFilesIn " + fastq1 + " " + fastq2 + " --twopassMode Basic --outSAMstrandField intronMotif";
    cout << exec2 << endl;
    
  execCmd(exec2);
  //*/
}


void runSTAR(RunData * run,RefData * ref)
{
  cout << endl << "runSTAR: " << ref->reference << endl << endl;
  set<string> samples;
  cout << run->outputDirectory << endl;
  vector<string> files = getFileList(run->outputDirectory);  
  int count = 1;

  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    cout << sample->sampleID << endl;
    samples.insert(sample->sampleID);
    alignReads1pass(sample->sampleID,run->outputDirectory,files,ref->genomeDir);
    cout << "STAR completed" << endl;
    //sortReads(sample->sampleID,run->outputDirectory, 'R');
    string output = ReadGroups(sample->sampleID,run->outputDirectory);
    runBamQC(sample->sampleID, output);
    cout << count << " " << sample->sampleID <<endl;
    count++;
  }
}



