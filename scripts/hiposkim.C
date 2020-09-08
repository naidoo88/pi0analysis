#include <cstdlib>
#include <iostream>
#include <chrono>
#include "TBenchmark.h"
#include "clas12reader.h"
#include "clas12writer.h"

using namespace clas12;
using std::cout;

void eraseSubStr(std::string & mainStr, const std::string & toErase);

void hiposkim(const Char_t input_list[]){

	TStopwatch time;
	time.Start();
  auto start = std::chrono::high_resolution_clock::now();

  std::ifstream list_of_files;
  char file_name[200];
  char last_file[200];

	list_of_files.open(input_list);
	if (list_of_files.is_open()){
		cout << "Successfully opened list:  " << input_list << endl;

		while (!list_of_files.eof()){
			if (!list_of_files.good()){
				cout << "Error: Issue with list." << endl;
				break;
			}

			list_of_files >> file_name;

			if (strcmp(last_file, file_name) != 0){ // makes sure the last file doesn't get counted twice.

        if(strstr (file_name,"*.hipo") != nullptr){
          cout << " *** the input file must be a hipo file..." << endl;
          exit(1);
        }

        string inputFile = file_name;
				cout << "INPUT FILE: " << inputFile << endl;
				string inbuff = inputFile;
				eraseSubStr(inbuff, ".hipo");
        string outputFile = inbuff + "_preskim.hipo";
				cout << "OUTPUT FILE: " << outputFile << endl << endl;

        clas12writer c12writer(outputFile.c_str()); //initialising clas12writer with output path
				clas12reader c12(inputFile.c_str());        //create the event reader
				c12writer.assignReader(c12);                //assign a reader to the writer
				
				while(c12.next()==true){

					// get particles by type
					auto electron = c12.getByID(11);
					//auto recoil = c12.getByID(2212); //proton
					auto recoil   = c12.getByID(2112); //neutron
					auto photons  = c12.getByID(22);

					// write events with 1 electron, 1 recoil, and at least 2 photons.
					if(electron.size()>0 &&recoil.size()>0 &&photons.size()>1){
						c12writer.writeEvent(); 
					}
				}

				//close writer
				c12writer.closeWriter();

			}//duplicate check
			sprintf(last_file, "%s", file_name); // save name of the current file into "last_file"
		}//file list loop
	}//list open-check

	list_of_files.close();

	time.Stop();
	time.Print();
} //fxn

//--from thispointer.com
void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

