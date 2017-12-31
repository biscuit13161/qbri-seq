#include <iostream>
#include <cstring>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <set>
#include <sstream>
#include <fstream>
#include <sys/wait.h>
#include <regex>
#include "isBlankLine.h"
#include "getFileList.h"
#include "qbri-bcl2fastq.h"
#include "runBamQC.h"

using namespace std;
namespace fs = boost::filesystem;

/* int main (int argc, char *argv[])
{
  bcl2fastq(argc, argv);
}//*/

void bcl2fastq(RunData * run)
{
  set<string>::iterator it;
  for (it=run->projectNames.begin(); it!= run->projectNames.end(); ++it)
    cout << *it <<endl;
  cout << " * Sample Sheet read" << endl;
//  populateSamples()

  runBcl2fastq(run);
  cout << " * BCL2FASTQ run!" << endl;
//
//  if (checkForOutput())
  runFastqc(run);
  cout << " * FASTQC run!" << endl;
//  else
//   error - no output files
//  
  delete run;
}

void getSampleSheet(RunData * run)
{

  bool data = false;
  bool data2 = false;  // second variable to test for first unaffectted until following line!
  ifstream  file(run->sampleSheet);
  string line;
  int sampleid = 0;
  int sampleproject = 9;
  while(getline(file, line))
  {
    if (!isBlankLine(line) )
    {
      vector<string> array;
      replace(line.begin(), line.end(), ' ', '_');
      if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);
      stringstream s(line);
      string temp;
      while (getline(s,temp,',') )
      {
        if (isBlankLine(temp) )
        {
          array.push_back("\0");
        }
        else
	{
          array.push_back(temp);
          if (!strcmp(temp.c_str(),"[Data]\0"))
          {
            data2 = true;
          }
        } 
      }


      if (data == true)
      {

        if (array[0] == "Lane")
        {
          //Lane,Sample_ID,Sample_Name,Sample_Plate,Sample_Well,I7_Index_ID,index,Sample_Project,Description
          int pos = 0;
          for (vector<string>::iterator pt = array.begin(); pt != array.end(); ++pt)
          {
            if (*pt == "Sample_ID")
            {
              sampleid = pos;
            }
            /*
            else if (*pt == "Sample_Name")
              samplename = pos;
            else if (*pt == "I7_Index_ID")
              i7indexid = pos;
            else if (*pt == "index")
              i7index = pos;
            //*/
            else if (*pt == "Sample_Project")
            {
              sampleproject = pos;
            }
            pos++;
          }
        }
        else
        {
          RunSample * sample = new RunSample;
          sample->Lane = array[0];
          sample->sampleID = array[sampleid];
          //  sample->sampleName = array[samplename];
          //sample->i7IndexID = array[5];
          //sample->i7index = array[6];
          //  sample->i5IndexID = array[7];
          //  sample->i5index = array[8];
          sample->projectName = array[sampleproject];
          run->projectNames.insert(array[sampleproject]);
          run->Samples.push_back(sample);
          for (int i = 0; i <= sampleproject; i++)
            cout << array[i] << "\t";
            cout << endl;
          }
        //*/
      }
      data = data2;
    //*/
    }
  }
  cout << "* Data Populated!" <<endl;
}

// */

void runBcl2fastq(RunData * run)
{
  //execlp(
  //*
  string cmd = "bcl2fastq --sample-sheet ";
  cmd += run->sampleSheet ;
  cmd += " -o ";
  cmd += run->outputDirectory;
  cmd += " --ignore-missing-bcls --no-lane-splitting";

  if (run->debug == true)
    cmd += " -l DEBUG";

  execCmd(cmd);
//*/

// ../SampleSheets/FCA_SampleSheet_Exome2.csv -o ../exome2.AHJ33YBBXX
}


void runFastqc(RunData * run)
{
// parallel -j 4 fastqc {} ::: /results/rnaseq.BHJ35GBBXX/Rnaseq_Essam/B*.fastq.gz
  /*string cmd = "parallel -j 4 fastqc {} ::: ";
  cmd += run->outputDirectory;
  cmd += "/";
  cmd += run->projectName; 
  cmd += "/*.fastqc.gz";

  system(cmd.c_str());
 //*/
  set<string>::iterator it;
  for (it=run->projectNames.begin(); it!= run->projectNames.end(); ++it)
  {  
    string dirs= run->outputDirectory;
    dirs += "/";
    dirs += run->projectName;
    dirs += *it;
    cout << dirs << endl;
    regex exp(".*\\.fastq\\.gz");
    smatch m;
    string filelist;
    vector<string> files = getFileList(dirs); 
    for (vector<string>::iterator ls = files.begin(); ls != files.end(); ++ls)
    {
      string out = *ls; 
      if (regex_match(out,exp) )
      {
        string cmd = "fastqc -o "+ dirs + " " + out;
        execCmd(cmd);
      }
    }
  }
}


bool checkForOutput(const RunData * run)
{
/*
	if (fs::exists(run->samples[0]))
		return true;

	return false;
//*/
}

void populateRunData(RunData * run)
{
  cout << "Please input Output Directory: ";
  string direct;
  getline(cin, run->outputDirectory);
  //strcpy(run->outputDirectory,direct);

  cout << "Please input Sample Sheet: ";
  string sheet;
  getline(cin,run->sampleSheet);
  //strcpy(run->sampleSheet, sheet);

}

/*
  std::set<int> myset;
  std::set<int>::iterator it;
  myset.insert (it,25);                 // max efficiency inserting

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
*/
