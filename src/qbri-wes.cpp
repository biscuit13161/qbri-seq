#include "misc.h"
#include "config.h"
#include "rundata.h"
#include "qbri-bcl2fastq.h"
#include "qbri-bwa.h"
#include "qbri-markdup.h"
#include "qbri-indelRealign.h"
#include "qbri-baseRecal.h"
#include "qbri-hapCall.h"
#include "qbri-varRecal.h"
#include <iostream>
#include <cstring>
#include <set>
#include <map>
#include <cstring>
#include <vector>
#include <set>
#include <getopt.h>

using namespace std;

void printHelpDNA();


int main (int argc, char *argv[])
{
  int c;
  char select;
  RunData * run = new RunData;
  run->index = false;
  run->verbose = false;
  bool samplesheet = false;
  bool help = false;
  bool outputdirectory = false;
  bool inputdirectory = false;
  bool fixMisecodedQ = false;
  string reference;
  RefData * ref = new RefData;
  ref->call = 19;
  ref->type = 'D';

  while (1)
    {
      static struct option long_options[] =
        {
          {"samplesheet", required_argument, 0, 'S'},
          {"inputdirectory" , required_argument, 0,'d'}, 
          {"outputdirectory", required_argument, 0,'o'},
          {"reference", required_argument, 0,'R'},
          {"help", no_argument, 0, 'h'},
          {"version", no_argument, 0, 'V'},
          {"bcl2fastq", no_argument, 0, 'f'},
          {"bwa", no_argument, 0, 'b'},
          {"markduplicates", no_argument, 0, 'm'},
          {"indelrealign", no_argument, 0, 'i'},
          {"baserecalibration", no_argument, 0, 'r'},
          {"haplotypecaller", no_argument, 0, 't'},
          {"varientrecalibration", no_argument, 0, 'v'},
          {"fixmisencodedqualityscores", no_argument, 0, 'q'},
          {"disableindexcreation", no_argument, 0, 'I'}, 
          {"verbose", no_argument, 0, 'x'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "S:o:d:R:hrfbqmiItvVx",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
        {  /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;
        }
        case 'h':
        {
          help = true;
          printVersion ();
          printHelpDNA();
          break;
        }
        case 'f':
        case 'b':
        case 'm':
        case 'i':
        case 't':
        case 'r':
        case 'v':
        {
          select = c;
          break;
        }
        case 'R':
        {
          reference = optarg;
          break;
        }
        case 'V':
        {
          help = true;
          printVersion ();
          break;
        }
        case 'S':
        {
          run->sampleSheet = optarg;
          samplesheet = true;
          break;
        }
        case 'd':
        {
          run->inputDirectory = optarg;
          inputdirectory = true;
          break;
       }
        case 'o':
        {
          run->outputDirectory = optarg;
          outputdirectory = true;
          break;
       }
        case 'q':
        {
          fixMisecodedQ = true;
          break;
        }
        case 'I':
        {
          run->index = true;
          break;
        }
        case 'x':
        {
          run->verbose = true;
          break;
        }
        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
        {
          abort();
	}
          
          //select = c;
        }
    }

  if (!help)
  {

    run->pairedEnd = true;
    run->debug = false;

    if (select != 'f')
    {
      cout << "requested Reference is " << reference << endl;
      if (reference == "hg38")
        ref->call = 38;
      else if (reference == "hg19")
        ref->call = 19;
      else
        cout << "requested Reference not available, defaulting to hg19!" << endl;
    }

    if (!samplesheet && !outputdirectory)
      populateRunData(run);

    if (!inputdirectory)
      run->inputDirectory = run->outputDirectory;

    getSampleSheet(run);

    if (select != 'f')
      getReference(ref);


    switch(select)
    {
      case 'f':
        bcl2fastq(run);
        break;
      case 'b':
        runBWA(run,ref);
        break;
      case 'm':
        markDupes(run,ref);
        break;
      case 'i':
        runIndelRealign(run,ref,fixMisecodedQ);
        break;
      case 'r':
        runBaseRecalibration(run, ref);
        break;
      case 't':
        runHapCaller(run,ref);
        break;
      case 'v':
        runVarientRecalibration(run, "SNP",ref);
        runVarientRecalibration(run, "INDEL",ref);
        break;
      default:
        abort();
    }
  }
  return 0;
}

void printHelpDNA()
{
  //cout << "Create help document!" << endl;
          
  cout << "Usage:" << endl << endl;
  cout << "\tqbri-wes <function> [--samplesheet <path/to/samplesheet>] [--outputdirectory <path/to/output/directory>] [--reference <hg19|hg38>]" << endl << endl; 
  cout << "where function is one of: --bcl2fastq, --bwa, --markduplicates, --indelrealign --baserecalibration, --haplotypecaller, or --varientrecalibration" <<endl << endl;

  cout << "--samplesheet (-S)                Sample Sheet to be used for samples" << endl 
       << "--outputdirectory (-o)            Output directory for data" << endl
       << "--inputdirectory (-d)             Input directory for data [defaults to outputdirectory]" << endl
       << "--reference (-R)                  Select reference for alignment and processing" << endl
       << "--help (-h)                       Display This help page" << endl
       << "--version (-V)                    Display script version details" << endl
       << "--bcl2fastq (-f)                  Run Illumina's BCL2Fastq conversion and de-multiplexing" << endl
       << "--bwa (-b)                        Align Samples using BWA" << endl
       << "--markduplicates (-m)             Mark Duplicate reads for samples, using Picard" << endl
       << "--indelrealign (-i)               Realign Indels, using GATK" << endl
       << "--baserecalibration (-r)          Base Recalibration using GATK" << endl
       << "--haplotypecaller (-t)            Call snps for samples, using GATK" << endl
       << "--varientfiltration (-v)          Carry out Varient Filtration, using GATK" << endl
       << "--fixmisencodedqualityscores (-q) " << endl 
       << "--disableindexcreation (-I) " << endl 
       << "--verbose (-x) "<< endl << endl;

}


/*
static struct option long_options[] =
  {
    {"help", no_argument, 0, 'h'},
    {"verbose", required_argument, 0, 'v'},
    {"param", required_argument, 0, 0},
    {0,0,0,0}
  };

case 0:
  if (long_options[option_index].flag != 0)
    break;
  if (strcmp(long_options[option_index].name,"param") == 0)
    param = atoi(optarg);
  break;
//*/
