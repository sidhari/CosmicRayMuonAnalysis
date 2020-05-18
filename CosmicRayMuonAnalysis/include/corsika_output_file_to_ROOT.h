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
        vector<unsigned int> run_number;
        vector<unsigned int> date_of_begin_run; //for the sake of documentation
        vector<float> energy_spectrum_slope;
        vector<float> energy_range_low;
        vector<float> energy_range_high;
        vector<unsigned int> nshow; //number of showers to be generated
        
        run_header_sub_block_branches(TTree *t)
        {   
            t->Branch("run_number",&run_number);
            t->Branch("date_of_begin_run",&date_of_begin_run);
            t->Branch("energy_spectrum_slope",&energy_spectrum_slope);
            t->Branch("energy_range_low",&energy_range_low);
            t->Branch("energy_range_high",&energy_range_high);
            t->Branch("nshow",&nshow);
        }  

        void clear_vectors();

};

class event_header_sub_block_branches //branches filled here contaain event information, filled once per EVTH
{
    public:
        vector<unsigned long long int> event_number;
        vector<float> primary_id; //currently using protons (id = 14)
        vector<float> primary_energy; //in GEV
        vector<float> primary_px;
        vector<float> primary_py;
        vector<float> primary_pz; //downwards is positive
        vector<float> theta; //zenith angle in radians
        vector<float> phi; //azimuthal angle in radians

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

        void clear_vectors();
        
};

class data_sub_block_branches 
{
    public:

        vector<unsigned long long int> event_number; 
        
        //PARTICLE DATA LINE INFO

        vector<float> particle_description;
        vector<float> px;
        vector<float> py;
        vector<float> pz;
        vector<float> x_coordinate;
        vector<float> y_coordinate;
        vector<float> z_coordinate;
        vector<float> time_since_first_interaction;

        //MOTHER PARTICLE DATA LINE INFO

        vector<float> z_position_at_creation_point;

        //GRANDMOTHER PARTICLE DATA LINE INFO
         
        vector<float> z_position_at_interaction_point;        

       data_sub_block_branches(TTree *t)
        {   
            t->Branch("event_number",&event_number);            

            t->Branch("particle_description",&particle_description);
            t->Branch("px",&px);
            t->Branch("py",&py);
            t->Branch("pz",&pz);
            t->Branch("x_coordinate",&x_coordinate);
            t->Branch("y_coordinate",&y_coordinate);
            t->Branch("z_coordinate",&z_coordinate);
            t->Branch("time_since_first_interaction",&time_since_first_interaction);
            
            t->Branch("z_position_at_creation_point",&z_position_at_creation_point);
            
            t->Branch("z_position_at_interaction_point",&z_position_at_interaction_point);

        }    
        

        void clear_vectors(); 
                  
};

class corsika_output_file_reader
{
    public:

        int process_run_header_sub_block(vector<float> run_header_sub_block,TTree* RUNH_tree,run_header_sub_block_branches* RUNH_sub_block_branches); //writes necesssary info from sub_block to ROOT tree
        int process_event_header_sub_block(vector<float> event_header_sub_block,TTree* EVTH_tree,event_header_sub_block_branches* EVTH_sub_block_branches); //writes necesssary info from sub_block to ROOT tree
        
        bool is_particle_data_line(float particle_id);
        bool is_mother_particle_data_line(float particle_id,float z_position);
        bool is_grandmother_particle_data_line(float particle_id,float z_position);

        int process_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree);
        int process_mother_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree); 
        int process_grandmother_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree); 

        int process_input_file(string input_filepath,TTree* RUNH_tree,TTree* EVTH_tree,TTree* PD_tree,int start,int end); //parses through the input file, creates the sub-blocks, calls the process functions declared below to fill the trees

        
        
};

