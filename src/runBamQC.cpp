#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>  // provides execlp()
#include "runBamQC.h"
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

void runBamQC(string sample, string file)
{
  bamqc(sample, file);
  flagstat(sample, file);
}

void bamqc(string& sample, string& file)
{
  //string exec2 = "/apps/BamQC/bamqc -o " + sample + " " + file;
  string exec2 = "/apps/BamQC/bamqc " + file;
  cout << exec2 << endl;
  int pid = fork();
  if (pid > 0)
  {
    int status;
    waitpid(pid,&status,0);
  }
  else if (pid == 0)
    execlp("/usr/bin/bash","/usr/bin/bash","-c",exec2.c_str(),NULL); 
}

void flagstat(string& sample, string& file)
{
  boost::filesystem::path p(file);
  p.remove_filename();
  string output_file = p.c_str() + sample + ".flagstat.txt";
  boost::filesystem::ofstream out(output_file, boost::filesystem::ofstream::app);
  //ofstream out(output_file, ofstream::app);
  out << endl << endl << file << endl;
  out.close();
  string exec2 = "samtools flagstat -@ 4 " + file + " >> " + output_file;
  execCmd(exec2);
}

void execCmd (string& exec)
{
  cout << exec << endl;
  int pid = fork();
  if (pid > 0)
  {
    int status;
    waitpid(pid,&status,0);
  }
  else if (pid == 0)
    execlp("/usr/bin/bash","/usr/bin/bash","-c",exec.c_str(),NULL); 
}
