/*
    Author: Siddharth Hariprakash
    Date Created: 2-22-2020
*/

#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>

//ROOT headers

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

#include "../include/corsika_constants.h"
#include "../include/corsika_output_file_to_ROOT.h"

using namespace std;

int corsika_output_file_processor(string input_file_path, string output_file_path, int start, int end)
{
    corsika_output_file_reader input_file_reader;

    /*if(input_file_reader.is_valid(string(input_file_path.c_str())) == false)
    {
        cout << "Error opening input file" << endl << endl;
        return -1;
    }*/

    TFile* outF = new TFile(output_file_path.c_str(),"RECREATE");
	if(!outF)
	{
		printf("Failed to open output file: %s\n", output_file_path.c_str());
		return -1;
	}

    TTree* RUNH_tree = new TTree("RUNH_tree","RUNH_tree");
    TTree* EVTH_tree = new TTree("EVTH_tree","EVTH_tree");
    TTree* PD_tree = new TTree("PD_tree","PD_tree");
      
    if(input_file_reader.process_input_file(input_file_path,RUNH_tree,EVTH_tree,PD_tree,start,end))
    {
        cout << "Error processing file" << endl << endl;
        return -1;
    }

    outF->cd();
    RUNH_tree->Write();
    EVTH_tree->Write();
    PD_tree->Write();

    cout << "Succesfully wrote to file " << output_file_path << endl << endl;

    return 0;

}

int main(int argc, char* argv[]) //input is two strings: input filepath and output filepath
{
    return corsika_output_file_processor(string(argv[1]), string(argv[2]), atoi(argv[3]), atoi(argv[4]));
}