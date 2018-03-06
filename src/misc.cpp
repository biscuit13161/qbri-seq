#include "misc.h"
#include "version.h"
#include "config.h"
#include "rundata.h"
#include <iostream>

using namespace std;

void printVersion()
{
  cout << endl << "QBRI Whole Exome/Genome and RNA Sequencing Pipeline" << endl;
  cout << "         Version " << PACKAGE_VERSION << endl << endl;
  cout << "Author: Dr Richard Thompson, QBRI-HBKU" << endl <<endl;
}

void getReference(RefData * ref)
{

/*
  ref->gatk = "/apps/GenomeAnalysisTK.jar";
  //ref->gatk = gatk2.c_str();
  ref->picard = "/apps/picard.jar";
  //ref->picard = picard2.c_str();
  ref->javamem = "-Xmx16g";
  //ref->javamem = javamem2.c_str();
*/
  cout << "using hg" << ref->call << endl;
  if (ref->call == 19)
    {
      string bundle = string(HG19_BUNDLE);
      ref->reference = bundle + "ucsc.hg19.fasta";
      ref->genomeDir = bundle + "STAR-index";
      ref->mills = bundle + "Mills_and_1000G_gold_standard.indels.hg19.sites.vcf";
      ref->dbsnp = bundle + "dbsnp_138.hg19.vcf";
      ref->hapmap = bundle + "hapmap_3.3.hg19.sites.vcf";
      ref->g1000 = bundle + "1000G_omni2.5.hg19.sites.vcf";
    }
  else if (ref->call == 38)
    {
      string bundle = string(HG38_BUNDLE);
      ref->reference = bundle + string("Homo_sapiens_assembly38.fasta");
      ref->genomeDir = bundle + string("STAR-index");
      ref->mills = bundle + string("Mills_and_1000G_gold_standard.indels.hg38.vcf,gz");
      ref->dbsnp = bundle + string("dbsnp_146.hg38.vcf.gz");
      ref->hapmap = bundle + string("hapmap_3.3.hg38.vcf.gz");
      ref->g1000 = bundle + string("1000G_omni2.5.hg38.vcf.gz");
    }
  cout << "* Reference Populated!" <<endl;
  
}

void sortReads(string sample, string& outputDirectory, char type)
{
  string input;
  if (type == 'D')
    input = (outputDirectory + "/" + sample + ".sam").c_str();
  else
    input = (outputDirectory + "/Aligned.out.sam").c_str();
  const char* inputfile = input.c_str();
  const char* outputfile = (outputDirectory + "/" + sample + ".sort.bam").c_str();
  cout << endl << "\tSorting " << sample << endl << endl;
  int pid = fork();
  if (pid > 0)
  {
    int status;
    waitpid(pid,&status,0);
  }
  else if (pid == 0)
    execlp("/usr/local/bin/samtools", "samtools","sort","-o",outputfile,inputfile, NULL);


}

string ReadGroups(string sample, string& outputDirectory)
{
  string input = "I=" + outputDirectory + "/" + sample + ".bam";
  string output_file = outputDirectory + "/" + sample + ".rg.bam";
  string output = "O=" + output_file;
  string readLib = "RGLB=" + sample;
  string readPlat = "RGPL=Illumina";
  string readPlatUnit = "RGPU=01";
  string readSample = "RGSM=" + sample;
  cout << endl << "\tAdding Read Groups for " << sample << endl << endl;

  int pid = fork();
  if (pid > 0)
  {
    int status;
    waitpid(pid,&status,0);
  }
  else if (pid == 0)
    execlp("/usr/bin/java", "java","-jar", PICARD,"AddOrReplaceReadGroups",input.c_str(),output.c_str(),"CREATE_MD5_FILE=true","CREATE_INDEX=true",readLib.c_str(),readPlat.c_str(),readPlatUnit.c_str(),readSample.c_str(), NULL);
 
  return output_file;
}

/*
template<typename T>
void verbCout (T s, bool verbose)
{
  if (verbose = true)
    cout << s << endl;
}
*/

