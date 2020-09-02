//usage :
//clas12root  4 myFirstSelector.C Ex3_ProofLite.C
//Where 4  is however many workers you would like
//myFirstSelector.C is the source dode of the selector
//   this can be created with any name using makeHipoSelector executable
//Note you must also change myFirstSelector in the macro below to your
//selector name
#include <stdlib.h>
#include "TProof.h"
#include "HipoChain.h"
#include "TSystem.h"
#include "TBenchmark.h"
#include "TString.h"
#include "pi0selector.h"

using std::cout;
using std::endl;

void runAnaProof(const Char_t input_list[], const Char_t outfilename[]){

  TStopwatch time;
  time.Start();

  std::ifstream list_of_files;
  char file_name[200];
  char last_file[200];

  int filecount = 0;
  TString outbuffname; //outfile buffer to be combined with hadd at the end of the script.
  
  //retrieve path of output file for buffer path
  TString dirname_cmd = Form("dirname %s", outfilename);
  TString outpath = gSystem->GetFromPipe(dirname_cmd.Data());
  TString buffpath = Form("%s/.outbuff/", outpath.Data());
  cout << endl << ">>> Buffer directory path is: " << buffpath << endl;

  //create buffer directory if it doesn't exist
  if((gSystem->AccessPathName(buffpath.Data()))){ //NB weird return convention: 0 => exists.
    cout << ">>> Buffer directory " << buffpath << " does not exist. Creating...";
    if (gSystem->MakeDirectory(buffpath.Data()) != 0){
      cout << endl << endl << " FAILED!!! Check disk space or permissions?" << endl << ">>> Process will now terminate." << endl;
      return;
    }
    else cout << " SUCCESS!" << endl << endl;
  }
  else{ //remove any previous contents if exists
    cout << ">>> Directory '" << buffpath << "' exists.  Removing any previous contents." << endl << endl;
    TString rmprev_cmd = Form("rm %s*", buffpath.Data());
    cout << rmprev_cmd << endl;
    gSystem->Exec(rmprev_cmd);
  }

  list_of_files.open(input_list);
  if (list_of_files.is_open()){
    cout << "Successfully opened list." << endl;

    while (!list_of_files.eof()){
      if (!list_of_files.good()){
        cout << endl << "ERROR: Issue with list." << endl << endl;
        break;
      }

      clas12root::HipoChain chain;

      list_of_files >> file_name;

      if (strcmp(last_file, file_name) != 0){ //make sure no double read on last
        TString file_str(file_name);

        chain.Add(file_str);

        clas12root::pi0selector selector(&chain);

        outbuffname.Form("%sbuff%d.root", buffpath.Data(), filecount);
        
        selector.SetOutFileName(outbuffname);

        gProof->Process(&selector,chain.GetNRecords());

        filecount++;
      }// duplicate check
      sprintf(last_file, "%s", file_name); // update "last_file"    
    }// filelist loop
  }// list-open check

  cout << endl << ">>> Merging buffer files into " << outfilename << endl;
  TString hadd_cmd = Form("hadd -f %s %s*", outfilename, buffpath.Data());
  cout << hadd_cmd << endl;
  gSystem->Exec(hadd_cmd.Data());

  cout << endl << ">>> Cleaning up buffer output files... " << endl;
  TString cleanup_cmd = Form("rm -rf %s", buffpath.Data());
  cout << cleanup_cmd << endl;
  gSystem->Exec(cleanup_cmd);
  //############# FINAL CLEAN UP 

  time.Stop();
  time.Print();

}
