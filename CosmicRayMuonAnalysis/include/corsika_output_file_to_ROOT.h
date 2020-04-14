/*
    Author: Siddharth Hariprakash
    Date Created: 2-14-2020
*/

/*  
    Header file containing classes required to parse through the block structure of corsika output files, extract necessary information, and write to ROOT trees. Check User's Guide at https://www.ikp.kit.edu/corsika/70.php for further info.
*/

#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>

//ROOT headers

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;


class run_header_sub_block_branches //branches filled here contain run information, filled once per RUNH
{
    public:
        unsigned int run_number;
        unsigned int date_of_begin_run; //for the sake of documentation
        float energy_spectrum_slope;
        float energy_range_low;
        float energy_range_high;
        unsigned int nshow; //number of showers to be generated
        
        run_header_sub_block_branches(TTree *t)
        {
            t->Branch("run_number",&run_number);
            t->Branch("date_of_begin_run",&date_of_begin_run);
            t->Branch("energy_spectrum_slope",&energy_spectrum_slope);
            t->Branch("energy_range_low",&energy_range_low);
            t->Branch("energy_range_high",&energy_range_high);
            t->Branch("nshow",&nshow);
        }               
    
};

class event_header_sub_block_branches //branches filled here contaain event information, filled once per EVTH
{
    public:
        unsigned long long int event_number;
        float primary_id; //currently using protons (id = 14)
        float primary_energy; //in GEV
        float primary_px;
        float primary_py;
        float primary_pz; //downwards is positive
        float theta; //zenith angle in radians
        float phi; //azimuthal angle in radians

        event_header_sub_block_branches(TTree *t)
        {
            t->Branch("event_number",&event_number);
            t->Branch("primary_id",&primary_id);
            t->Branch("primary_energy",&primary_energy);
            t->Branch("primary_px",&primary_px);
            t->Branch("primary_py",&primary_py);
            t->Branch("primary_pz",&primary_pz);
            t->Branch("theta",&theta);
            t->Branch("phi",&phi);
        } 
        
};

class data_sub_block_branches 
{
    public:

        unsigned long long int event_number; 

        //PARTICLE DATA LINE INFO

        vector<float> particle_description;
        vector<float> px;
        vector<float> py;
        vector<float> pz;

        //MOTHER PARTICLE DATA LINE INFO

        vector<float> mother_particle_description;
        vector<float> mother_px;
        vector<float> mother_py;
        vector<float> mother_pz;
        vector<float> z_position_at_creation_point;

        //GRANDMOTHER PARTICLE DATA LINE INFO

        vector<float> grandmother_particle_description;
        vector<float> grandmother_px;
        vector<float> grandmother_py;
        vector<float> grandmother_pz;
        
        
        data_sub_block_branches(TTree *t)
        {   
            t->Branch("event_number",&event_number);            

            t->Branch("particle_description",&particle_description);
            t->Branch("px",&px);
            t->Branch("py",&py);
            t->Branch("pz",&pz);

            t->Branch("mother_particle_description",&mother_particle_description);
            t->Branch("mother_px",&mother_px);
            t->Branch("mother_py",&mother_py);
            t->Branch("mother_pz",&mother_pz);
            t->Branch("z_position_at_creation_point",&z_position_at_creation_point);

            t->Branch("grandmother_particle_description",&grandmother_particle_description);
            t->Branch("grandmother_px",&grandmother_px);
            t->Branch("grandmother_py",&grandmother_py);
            t->Branch("grandmother_pz",&grandmother_pz);


        }

        void clear_vectors(); 
                  
};

class corsika_output_file_reader
{
    public:

        int process_run_header_sub_block(vector<float> run_header_sub_block,TTree* RUNH_tree); //writes necesssary info from sub_block to ROOT tree
        int process_event_header_sub_block(vector<float> event_header_sub_block,TTree* EVTH_tree); //writes necesssary info from sub_block to ROOT tree
        
        bool is_particle_data_line(float particle_id);
        bool is_mother_particle_data_line(float particle_id,unsigned int ancestor_count);
        bool is_grandmother_particle_data_line(float particle_id,unsigned int ancestor_count);

        int process_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree);
        int process_mother_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree); 
        int process_grandmother_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree); 

        int process_input_file(string input_filepath,TTree* RUNH_tree,TTree* EVTH_tree,TTree* PD_tree); //parses through the input file, creates the sub-blocks, calls the process functions declared below to fill the trees

        
        
};

