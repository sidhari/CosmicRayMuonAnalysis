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

            mother_particle_description = 0;
            mother_px = 0;
            mother_py = 0;
            mother_pz = 0;
            z_position_at_creation_point = 0;

            grandmother_particle_description = 0;
            grandmother_px = 0;
            grandmother_py = 0;
            grandmother_pz = 0;    

        }
        
        PD_tree_reader(TTree* PD_tree) : PD_tree_reader()
        {   
            PD_tree->SetBranchAddress("event_number",&event_number);

            PD_tree->SetBranchAddress("particle_description",&particle_description);
            PD_tree->SetBranchAddress("px",&px);
            PD_tree->SetBranchAddress("py",&py);
            PD_tree->SetBranchAddress("pz",&pz);

            PD_tree->SetBranchAddress("mother_particle_description",&mother_particle_description);
            PD_tree->SetBranchAddress("mother_px",&mother_px);
            PD_tree->SetBranchAddress("mother_py",&mother_py);
            PD_tree->SetBranchAddress("mother_pz",&mother_pz);
            PD_tree->SetBranchAddress("z_position_at_creation_point",&z_position_at_creation_point);

            PD_tree->SetBranchAddress("grandmother_particle_description",&grandmother_particle_description);
            PD_tree->SetBranchAddress("grandmother_px",&grandmother_px);
            PD_tree->SetBranchAddress("grandmother_py",&grandmother_py);
            PD_tree->SetBranchAddress("grandmother_pz",&grandmother_pz);
            
        }

        unsigned int particle_count_in_event()
        {
            return particle_description->size();
        }

        unsigned int muon_count_in_event()
        {   
            unsigned int muon_counter = 0;

            for(unsigned int particle_index = 0; particle_index < particle_description->size(); particle_index++)
            {
                int particle_id = (int)(particle_description->at(particle_index)/1000);

                if((particle_id == 5) || particle_id == 6)
                {
                    muon_counter++;
                }
            }

            return muon_counter;
        }
        

        unsigned long long int* event_number;

        //PARTICLE DATA LINE INFO

        vector<float>* particle_description;
        vector<float>* px;
        vector<float>* py;
        vector<float>* pz;

        //MOTHER PARTICLE DATA LINE INFO

        vector<float>* mother_particle_description;
        vector<float>* mother_px;
        vector<float>* mother_py;
        vector<float>* mother_pz;
        vector<float>* z_position_at_creation_point;

        //GRANDMOTHER PARTICLE DATA LINE INFO

        vector<float>* grandmother_particle_description;
        vector<float>* grandmother_px;
        vector<float>* grandmother_py;
        vector<float>* grandmother_pz;


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

        unsigned long long int event_number;
        float primary_id; 
        float primary_energy; 
        float primary_px;
        float primary_py;
        float primary_pz; 
        float theta;
        float phi; 
};