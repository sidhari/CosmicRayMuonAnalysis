#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

using namespace std;

unsigned int events_per_file = 1;

int steering_file_generator(string base_filepath, unsigned int total_events)
{
    ifstream base_file;
    base_file.open(base_filepath,ios::in);

    cout << "Base file is " << base_filepath << endl << endl;

    unsigned int run_number = 0;
    unsigned int event_number = 0;

    unsigned int seed_1_base = 998;
    unsigned int seed_2_base = 999;

    unsigned int seed_1 = 998;
    unsigned int seed_2 = 999;

    string run_string_old = string("RUNNR   ") + to_string(run_number) + string("                              number of run");
    string event_string_old = string("EVTNR   ") + to_string(event_number) + string("                              no of first shower event");
    string seed_1_string_old = string("SEED    ") + to_string(seed_1_base) + string(" 0 0                       seed for hadronic part");
    string seed_2_string_old = string("SEED    ") + to_string(seed_2_base) + string(" 0 0                       seed for EGS4 part");  

    string file_line;

    for(unsigned int event_counter = 1; event_counter <= total_events; event_counter += events_per_file)
    {   
        cout << "Event " << event_counter << endl << endl; 

        string steering_file_name = string("all-inputs_") + to_string(event_counter);
        string steering_filepath = string("../run/Inputs/1E6/") + steering_file_name;
        ofstream steering_file;
        steering_file.open(steering_filepath,ios::out | ios::trunc);      

        string run_string_new = string("RUNNR   ") + to_string(event_counter) + string("                              number of run");
        string event_string_new = string("EVTNR   ") + to_string(event_counter) + string("                              no of first shower event");
        seed_1 += (events_per_file + 5);
        seed_2 += (events_per_file + 5); 
        string seed_1_string_new = string("SEED    ") + to_string(seed_1) + string(" 0 0                       seed for hadronic part");
        string seed_2_string_new = string("SEED    ") + to_string(seed_2) + string(" 0 0                       seed for EGS4 part");   

        while(base_file.eof()==0)
        {
            getline(base_file,file_line);

            if(file_line == run_string_old)
            {
                steering_file << run_string_new << endl;
                continue;
            }
            else if(file_line == event_string_old)
            {
                steering_file << event_string_new << endl;
                continue;
            }
            else if(file_line == seed_1_string_old)
            {
                steering_file << seed_1_string_new << endl;
                continue;
            }
            else if (file_line == seed_2_string_old)
            {
                steering_file << seed_2_string_new << endl;
                continue;
            }
            else 
            {
                steering_file << file_line << endl;
                continue;
            }
            
        }    
          

        steering_file.close();  
        base_file.clear(); 
        base_file.seekg (0,ios::beg);         

    }

    ofstream bashfile;
    bashfile.open("../run/JobSplitter.sh",ios::out | ios::trunc);
    string line_1 = string("#!/bin/bash");
    bashfile << line_1 << endl;

    for(unsigned int exec_counter = 1; exec_counter <= total_events; exec_counter++)
    {
        string line = string("./corsika77100Linux_QGSII_urqmd < Inputs/1E6/all-inputs_") + to_string(exec_counter) + string(" &");
        bashfile << line << endl;
    }

    return 0;
}

int main(int argc,char* argv[])
{
    switch (argc)
    {
    case 3:
        return steering_file_generator(string(argv[1]),atoi(argv[2]));
        break;
    
    default:
        return -1;
        break;
    }

    return 0;
}