#include "misc.h"
#include "config.h"
#include "rundata.h"
#include "qbri-bcl2fastq.h"
#include "qbri-star.h"
#include "qbri-markdup.h"
#include "qbri-split.h"
#include "qbri-indelRealign.h"
#include "qbri-baseRecal.h"
#include "qbri-hapCall.h"
#include "qbri-varFilt.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <set>
#include <getopt.h>

using namespace std;

void printHelpRNA();


int main (int argc, char *argv[])
{
  int c;
  char select;
  RunData * run = new RunData;
  bool samplesheet = false;
  bool outputdirectory = false;
  bool inputdirectory = false;
  bool help = false;
  bool fixMisecodedQ = false;
  string reference;
  RefData * ref = new RefData;
  ref->call = 19;
  ref->type = 'R';

  while (1)
    {
      static struct option long_options[] =
        {
          {"samplesheet", required_argument, 0, 'S'},
          {"outputdirectory", required_argument, 0,'o'},
          {"inputdirectory" , required_argument, 0,'d'}, 
          {"reference", required_argument, 0,'R'},
          {"help", no_argument, 0, 'h'},
          {"version", no_argument, 0, 'V'},
          {"bcl2fastq", no_argument, 0, 'f'},
          {"star", no_argument, 0, 's'},
          {"markduplicates", no_argument, 0, 'm'},
          {"splitcigars", no_argument, 0, 'c'},
          {"indelrealign", no_argument, 0, 'i'},
          {"baserecalibration", no_argument, 0, 'r'},
          {"haplotypecaller", no_argument, 0, 't'},
          {"varientrecalibration", no_argument, 0, 'v'},
          {"fixmisencodedqualityscores", no_argument, 0, 'q'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "sS:o:d:R:hrfcmqitvV",
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
          printHelpRNA();
          break;
        }
        case 'f':
        case 's':
        case 'm':
        case 'i':
        case 'c':
        case 't':
        case 'r':
        case 'v':
        {
          select = c;
          break;
        }
        case 'q':
        {
          fixMisecodedQ = true;
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
      cout << "Input reference is: " << reference << endl;
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

    getReference(ref);


    switch(select)
    {
      case 'f':
        bcl2fastq(run);
        break;
      case 's':
        runSTAR(run,ref);
        break;
      case 'm':
        markDupes(run,ref);
        break;
      case 'c':
        splitCigars(run,ref);
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
        runVarientFiltration(run,ref);
        break;
      default:
        abort();
    }
  }
  return 0;
}

void printHelpRNA()
{
  //cout << "Create help document!" << endl;
          
  cout << "Usage:" << endl << endl;
  cout << "\tqbri-wes <function> [--samplesheet <path/to/samplesheet>] [--outputdirectory <path/to/output/directory>] [--reference <hg19|hg38>]" << endl << endl; 
  cout << "where function is one of: --bcl2fastq, --bwa, --markduplicates, --splitcigars, --indelrealign, --baserecalibration, --haplotypecaller, or --varientrecalibration" <<endl << endl;

  cout << "--samplesheet (-S)           Sample Sheet to be used for samples" << endl 
       << "--outputdirectory (-o)       Output directory for data" << endl
       << "--reference (-R)             Select reference for alignment and processing" << endl
       << "--help (-h)                  Display This help page" << endl
       << "--version (-V)               Display script version details" << endl
       << "--bcl2fastq (-f)             Run Illumina's BCL2Fastq conversion and de-multiplexing" << endl
       << "--star (-s)                  Align Samples using STAR" << endl
       << "--markduplicates (-m)        Mark Duplicate reads for samples, using Picard" << endl
       << "--splitcigars (-c)           Split reads into exon segments, using GATK" << endl
       << "--indelrealign (-i)          Realign Indels, using GATK" << endl
       << "--baserecalibration (-r)     Base Recalibration using GATK" << endl
       << "--haplotypecaller (-t)       Call snps for samples, using GATK" << endl
       << "--varientfiltration (-v)     Carry out Varient Filtration, using GATK" << endl
       <<"fixmisencodedqualityscores (-q)" << endl << endl;

}




