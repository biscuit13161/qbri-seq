#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include "config.h"
#include "getFileList.h"
#include "qbri-varRecal.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;

void VariantRecalibrator(string sample, string& outputDirectory, string mode, string& reference, string& mills, string& dbsnp, string& hapmap, string& g1000)
{
  string inputFile = outputDirectory;
  if (mode == "SNP")
    inputFile += "/" + sample + ".hc.vcf";
  else
    inputFile += "/" + sample + ".SNP.vcf";
  string recalFile = outputDirectory + "/" + sample + "." + mode + ".recal";
  string tranchesFile = outputDirectory + "/" + sample + "." + mode + ".tranches";
  string rscriptFile = outputDirectory + "/" + sample + "." + mode + ".rscript";
  cout << endl << "\tRecalibrating (" << mode << ") "<< sample << endl << endl;

  string cmd;  
  if (mode == "SNP")
    {
     cmd = string(JAVA) + " "+  string(GATKMEM) + " -jar "+  string(GATK) +" -T VariantRecalibrator " //-I "+  inputFile
        + " --input "+  inputFile 
        + " -recalFile "+  recalFile 
        + " -tranchesFile "+  tranchesFile 
        + " -rscriptFile "+ rscriptFile 
        + " -R "+  reference 
        + " -resource:hapmap,known=false,training=true,truth=true,prior=15.0 "+  hapmap 
        + " -resource:omni,known=false,training=true,truth=true,prior=12.0 "+  g1000 
        + " -resource:dbsnp,known=true,training=false,truth=false,prior=2.0 "+  dbsnp +" -mode "+  mode 
        + " --maxGaussians 4 -minNumBad 1000 -nt " + string(GATK_CORES)+ " -an QD -an MQRankSum -an ReadPosRankSum -an FS -an DP";
    }
    else
    {
      cmd = string(JAVA) + " "+ string(GATKMEM) + " -jar "+ string(GATK) +" -T VariantRecalibrator --input "+  inputFile +" -recalFile "+  recalFile +" -tranchesFile "+  tranchesFile
        + " -rscriptFile "+ rscriptFile 
        + " -R " +  reference 
        + " -resource:mills,known=false,training=true,truth=true,prior=12.0 "+  mills 
        + " -resource:dbsnp,known=true,training=false,truth=false,prior=2.0 "+  dbsnp +" -mode "+  mode 
        + " --maxGaussians 4 -minNumBad 1000 -nt " + string(GATK_CORES)+ " -an FS -an DP";
    }
  execCmd(cmd);
}

void runVarientRecalibration(RunData * run, string mode, RefData * ref)
{
  set<string> samples;
  // it;
  for (vector<RunSample*>::iterator it = run->Samples.begin(); it!= run->Samples.end(); ++it)
  {
    RunSample * sample = *it;
    samples.insert(sample->sampleID);
    VariantRecalibrator(sample->sampleID, run->outputDirectory, mode, ref->reference, ref->mills, ref->dbsnp, ref->hapmap, ref->g1000);
    ApplyRecalibration(sample->sampleID, run->outputDirectory, mode, ref->reference);
  }
}

void ApplyRecalibration(string sample, string& outputDirectory, string mode, string& reference)
{
  string inputFile = outputDirectory;
  if (mode == "SNP")
    inputFile += "/" + sample + ".hc.vcf";
  else
    inputFile += "/" + sample + ".SNP.vcf";
  string outputFile = outputDirectory + "/" + sample + "." + mode + ".vcf";
  string recalFile = outputDirectory + "/" + sample + "." + mode + ".recal";
  string tranchesFile = outputDirectory + "/" + sample + "." + mode + ".tranches";

  cout << endl << "\tApplying " << mode << " Recalibration for " << sample << endl << endl;

  string cmd = string(JAVA) + " "+ string(GATKMEM) + " -jar "+ string(GATK) + " -T ApplyRecalibration --input "+ inputFile +" -o "+ outputFile +" -R "+ reference +" -recalFile "+ recalFile +" -tranchesFile "+ tranchesFile +" -nt " + string(GATK_CORES) + " --ts_filter_level 99.9 --generate_md5 -mode "+ mode;
  execCmd(cmd);
}

/*
java -Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T VariantRecalibrator -R /hg38/Homo_sapiens_assembly38.fasta \
-input <file>.hc.vcf -recalFile <file>.snp.recal -tranchesFile <file>.snp.tranches \
-rscriptFile <file>.snp.rscript -nt 4 \
-resource:hapmap,known=false,training=true,truth=true,prior=15.0 \
/hg38/hapmap_3.3.hg38.vcf.gz \
-resource:omni,known=false,training=true,truth=true,prior=12.0 \
/hg38/1000G_omni2.5.hg38.vcf.gz  \
-resource:dbsnp,known=true,training=false,truth=false,prior=2.0 \
/hg38/dbsnp_146.hg38.vcf.gz -mode SNP --maxGaussians 4 -minNumBad 1000 \
-an QD -an MQRankSum -an ReadPosRankSum -an FS -an DP

java -Xmx16g -jar /apps/string(GATK)/GenomeAnalysisTK.jar \
-T ApplyRecalibration -R /hg38/Homo_sapiens_assembly38.fasta -input <file>.hc.vcf \
-recalFile <file>.snp.recal -tranchesFile <file>.snp.tranches \
 -o <file>.snp.vcf -nt 6 --ts_filter_level 99.9 -mode SNP


// */

