/*
    Author: Siddharth Hariprakash
    Date Created: 2-17-2020
*/

/*  
   Contains all required constants related to corsika output files. Check User's Guide at https://www.ikp.kit.edu/corsika/70.php for further info.
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <math.h>
#include <chrono>
#include <time.h>

//ROOT headers

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TGraph.h>

const unsigned int corsika_word_size = 4; //each word in the block structure is 4 bytes long
const unsigned int block_size = 273;
const unsigned int data_line_size = 7;

const unsigned int input_file_count = 1000;

bool multiple_files = false;

const unsigned int primary_energy_count = 6;

const unsigned int mu_plus_id = 5;
const unsigned int mu_minus_id = 6;

const unsigned int zeros_in_output_name = 6;

