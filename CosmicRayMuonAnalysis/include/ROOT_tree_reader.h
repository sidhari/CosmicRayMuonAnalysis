/*
    Author: Siddharth Hariprakash
    Date Created: 3-5-2020
*/

/*  
    Header file containing classes required to read information stored in ROOT trees.
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

class PD_tree_reader
{
    public:

        PD_tree_reader()
        {   
            event_number = 0;

            particle_description = 0;
            px = 0;
            py = 0;
            pz = 0;
            x_coordinate = 0;
            y_coordinate = 0;
            z_coordinate = 0;
            time_since_first_interaction = 0;
            
            z_position_at_creation_point = 0;
  
            z_position_at_interaction_point = 0;

        }
        
        PD_tree_reader(TTree* PD_tree) : PD_tree_reader()
        {   
            PD_tree->SetBranchAddress("event_number",&event_number);

            PD_tree->SetBranchAddress("particle_description",&particle_description);
            PD_tree->SetBranchAddress("px",&px);
            PD_tree->SetBranchAddress("py",&py);
            PD_tree->SetBranchAddress("pz",&pz);
            PD_tree->SetBranchAddress("x_coordinate",&x_coordinate);
            PD_tree->SetBranchAddress("y_coordinate",&y_coordinate);
            PD_tree->SetBranchAddress("z_coordinate",&z_coordinate);
            PD_tree->SetBranchAddress("time_since_first_interaction",&time_since_first_interaction);

            PD_tree->SetBranchAddress("z_position_at_creation_point",&z_position_at_creation_point);

            PD_tree->SetBranchAddress("z_position_at_interaction_point",&z_position_at_interaction_point);
            
        }

        unsigned int muon_count_in_event()
        {   
            unsigned int muon_counter = 0;

            for(unsigned int particle_index = 0; particle_index < particle_description->size(); particle_index++)
            {
                int particle_id = (int)(particle_description->at(particle_index)/1000);

                if((particle_id == mu_minus_id) || particle_id == mu_plus_id)
                {
                    muon_counter++;
                }
            }

            return muon_counter;
        }
        

        vector<unsigned long long int>* event_number;

        //PARTICLE DATA LINE INFO

        vector<float>* particle_description;
        vector<float>* px;
        vector<float>* py;
        vector<float>* pz;
        vector<float>* x_coordinate;
        vector<float>* y_coordinate;
        vector<float>* z_coordinate;
        vector<float>* time_since_first_interaction;

        //MOTHER PARTICLE DATA LINE INFO

        vector<float>* z_position_at_creation_point;

        //GRANDMOTHER PARTICLE DATA LINE INFO

        vector<float>* z_position_at_interaction_point; 


};

class EVTH_tree_reader
{
    public:

        EVTH_tree_reader()
        {
            event_number = 0;
            primary_id = 0;
            primary_energy = 0;
            primary_px = 0;
            primary_py = 0;
            primary_pz = 0;
            theta = 0;
            phi = 0;
        }

        EVTH_tree_reader(TTree* EVTH_tree) : EVTH_tree_reader()
        {
            EVTH_tree->SetBranchAddress("event_number",&event_number);
            EVTH_tree->SetBranchAddress("primary_id",&primary_id);
            EVTH_tree->SetBranchAddress("primary_energy",&primary_energy);
            EVTH_tree->SetBranchAddress("primary_px",&primary_px);
            EVTH_tree->SetBranchAddress("primary_py",&primary_py);
            EVTH_tree->SetBranchAddress("primary_pz",&primary_pz);
            EVTH_tree->SetBranchAddress("theta",&theta);
            EVTH_tree->SetBranchAddress("phi",&phi);
        }

        vector<unsigned long long int>* event_number;
        vector<float>* primary_id; 
        vector<float>* primary_energy; 
        vector<float>* primary_px;
        vector<float>* primary_py;
        vector<float>* primary_pz; 
        vector<float>* theta;
        vector<float>* phi; 
};