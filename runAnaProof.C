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
#include "pi0selector.h"

using std::cout;
using std::endl;

void runAnaProof(const Char_t input_list[], const Char_t outfilename[]){

  clas12root::HipoChain chain;

  std::ifstream list_of_files;
  char file_name[200];
  char last_file[200];

  list_of_files.open(input_list);
  if (list_of_files.is_open()){
    cout << "Successfully opened list." << endl;

    while (!list_of_files.eof()){
      if (!list_of_files.good()){
        cout << endl << "ERROR: Issue with list." << endl << endl;
        break;
      }

      list_of_files >> file_name;

      if (strcmp(last_file, file_name) != 0){ //make sure no double read on last
        TString file_str(file_name);
        chain.Add(file_str);
      }
    }
  }

  clas12root::pi0selector selector(&chain);

  selector.SetOutFileName(outfilename);

  gProof->Process(&selector,chain.GetNRecords());
}
