/*
    Author: Siddharth Hariprakash
    Date Created: 3-5-2020
*/

/*  
    Used to analyze muons at detector level
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
#include <TGraphErrors.h>

#include "../include/corsika_output_file_to_ROOT.h"
#include "../include/corsika_constants.h"
#include "../include/ROOT_tree_reader.h"



using namespace std;

int muon_analyzer(string input_file_path, string output_file_path, int start_event = 0, int end_event = -1)
{   
    cout << endl;

    vector<TFile*> particle_info_files;
    particle_info_files.reserve(primary_energy_count);

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {           
        string suffix = to_string(file_counter+1) + string(".root");
        string input_file_path_with_suffix = input_file_path + suffix;

        TFile* particle_info_file = TFile::Open(input_file_path_with_suffix.c_str());
        cout << "Using ROOT File: " << input_file_path_with_suffix << endl << endl;

        if(!particle_info_file)
        {
            cout << "Error opening file " << string(input_file_path_with_suffix) << endl << endl;
            return -1;
        }
        
        particle_info_files.push_back(particle_info_file);
    }   

    vector<TTree*> event_info_trees;
    event_info_trees.reserve(primary_energy_count);

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        TTree* EVTH_tree = (TTree*)particle_info_files.at(file_counter)->Get("EVTH_tree");

        if(!EVTH_tree)
        {
            cout << "Error opening tree" << endl << endl;
            return -1;
        }

        event_info_trees.push_back(EVTH_tree);

    }

    vector<EVTH_tree_reader*> simulated_event_info_collection;
    simulated_event_info_collection.reserve(primary_energy_count);

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        TTree* EVTH_tree = event_info_trees.at(file_counter);
        simulated_event_info_collection.push_back(new EVTH_tree_reader(EVTH_tree));
    } 

    
    vector<TTree*> particle_info_trees;
    particle_info_trees.reserve(primary_energy_count);

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        TTree* PD_tree = (TTree*)particle_info_files.at(file_counter)->Get("PD_tree");

        if(!PD_tree)
        {
            cout << "Error opening tree" << endl << endl;
            return -1;
        }

        particle_info_trees.push_back(PD_tree);

    }

    
    vector<PD_tree_reader*> simulated_particle_info_collection;
    simulated_particle_info_collection.reserve(primary_energy_count);

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        TTree* PD_tree = particle_info_trees.at(file_counter);
        simulated_particle_info_collection.push_back(new PD_tree_reader(PD_tree));
    }    

    //
    //HISTOGRAMS
    //

    map<string,unsigned int> muon_counts_bins;
    muon_counts_bins[string("muon_counts_1E1")] = 5;
    muon_counts_bins[string("muon_counts_1E2")] = 15;
    muon_counts_bins[string("muon_counts_1E3")] = 55;
    muon_counts_bins[string("muon_counts_1E4")] = 350;
    muon_counts_bins[string("muon_counts_1E5")] = 400;
    //muon_counts_bins[string("muon_counts_1E6")] = 450;
    //muon_counts_bins[string("muon_counts_1E7")] = 450;
    //muon_counts_bins[string("muon_counts_1E8")] = 450;

    map<string,unsigned int> muon_energies_bins;
    muon_energies_bins[string("muon_energies_1E1")] = 20;
    muon_energies_bins[string("muon_energies_1E2")] = 50;
    muon_energies_bins[string("muon_energies_1E3")] = 75;
    muon_energies_bins[string("muon_energies_1E4")] = 100;
    muon_energies_bins[string("muon_energies_1E5")] = 200;
    //muon_energies_bins[string("muon_energies_1E6")] = 300;
    //muon_energies_bins[string("muon_energies_1E7")] = 300;
    //muon_energies_bins[string("muon_energies_1E8")] = 300;

    map<string,unsigned int> muon_mothers_bins;
    muon_mothers_bins[string("muon_mothers_1E1")] = 15;
    muon_mothers_bins[string("muon_mothers_1E2")] = 20;
    muon_mothers_bins[string("muon_mothers_1E3")] = 60;
    muon_mothers_bins[string("muon_mothers_1E4")] = 60;
    muon_mothers_bins[string("muon_mothers_1E5")] = 60;
    //muon_mothers_bins[string("muon_mothers_1E6")] = 60;
    //muon_mothers_bins[string("muon_mothers_1E7")] = 60;
    //muon_mothers_bins[string("muon_mothers_1E8")] = 60;

    map<string,unsigned int> muon_grandmothers_bins;
    muon_grandmothers_bins[string("muon_grandmothers_1E1")] = 20;
    muon_grandmothers_bins[string("muon_grandmothers_1E2")] = 30;
    muon_grandmothers_bins[string("muon_grandmothers_1E3")] = 30;
    muon_grandmothers_bins[string("muon_grandmothers_1E4")] = 30;
    muon_grandmothers_bins[string("muon_grandmothers_1E5")] = 30;
    //muon_grandmothers_bins[string("muon_grandmothers_1E6")] = 30;
    //muon_grandmothers_bins[string("muon_grandmothers_1E7")] = 30;
    //muon_grandmothers_bins[string("muon_grandmothers_1E8")] = 30;

    map<string,unsigned int> muon_counts_range;
    muon_counts_range[string("muon_counts_1E1")] = 5;
    muon_counts_range[string("muon_counts_1E2")] = 15;
    muon_counts_range[string("muon_counts_1E3")] = 55;
    muon_counts_range[string("muon_counts_1E4")] = 350;
    muon_counts_range[string("muon_counts_1E5")] = 3000;
    //muon_counts_range[string("muon_counts_1E6")] = 20000;
    //muon_counts_range[string("muon_counts_1E7")] = 200000;
    //muon_counts_range[string("muon_counts_1E8")] = 2000000;

    map<string,unsigned int> muon_energies_range;
    muon_energies_range[string("muon_energies_1E1")] = 5;
    muon_energies_range[string("muon_energies_1E2")] = 60;
    muon_energies_range[string("muon_energies_1E3")] = 500;
    muon_energies_range[string("muon_energies_1E4")] = 2500;
    muon_energies_range[string("muon_energies_1E5")] = 12500;
    //muon_energies_range[string("muon_energies_1E6")] = 120000;
    //muon_energies_range[string("muon_energies_1E7")] = 1200000;
    //muon_energies_range[string("muon_energies_1E8")] = 12000000;

    map<string,unsigned int> muon_mothers_range;
    muon_mothers_range[string("muon_mothers_1E1")] = 15;
    muon_mothers_range[string("muon_mothers_1E2")] = 20;
    muon_mothers_range[string("muon_mothers_1E3")] = 60;
    muon_mothers_range[string("muon_mothers_1E4")] = 60;
    muon_mothers_range[string("muon_mothers_1E5")] = 60;
    //muon_mothers_range[string("muon_mothers_1E6")] = 60;
    //muon_mothers_range[string("muon_mothers_1E7")] = 60;
    //muon_mothers_range[string("muon_mothers_1E8")] = 60;

    map<string,unsigned int> muon_grandmothers_range;
    muon_grandmothers_range[string("muon_grandmothers_1E1")] = 20;
    muon_grandmothers_range[string("muon_grandmothers_1E2")] = 30;
    muon_grandmothers_range[string("muon_grandmothers_1E3")] = 30;
    muon_grandmothers_range[string("muon_grandmothers_1E4")] = 30;
    muon_grandmothers_range[string("muon_grandmothers_1E5")] = 30;
    //muon_grandmothers_range[string("muon_grandmothers_1E6")] = 30;
    //muon_grandmothers_range[string("muon_grandmothers_1E7")] = 30;
    //muon_grandmothers_range[string("muon_grandmothers_1E8")] = 30;

    //MUON COUNTS

    vector<TH1F*> muon_counts;
    muon_counts.reserve(primary_energy_count);

    //MUON COUNT VS PRIMARY COS(THETA)

    vector<TH1F*> muon_counts_vs_primary_cos_theta;
    muon_counts_vs_primary_cos_theta.reserve(primary_energy_count);

    //MUON ENERGIES

    vector<TH1F*> muon_energies;
    muon_energies.reserve(primary_energy_count);

    //MUON MOTHERS

    vector<TH1F*> muon_mothers;
    muon_mothers.reserve(primary_energy_count);

    //MUON GRANDMOTHERS

    vector<TH1F*> muon_grandmothers;
    muon_grandmothers.reserve(primary_energy_count);

    //MUONS COS(THETA)

    vector<TH1F*> muon_cos_theta;
    muon_cos_theta.reserve(primary_energy_count);

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {   
        //MUON COUNTS

        string muon_counts_hist_name = string("muon_counts_1E") + to_string(file_counter+1);
        muon_counts.push_back(new TH1F(muon_counts_hist_name.c_str(),"Muon Counts at Detector Level;Muon Count;Event Count",muon_counts_bins[muon_counts_hist_name],0,muon_counts_range[muon_counts_hist_name]));

        //MUON COUNT VS PRIMARY COS(THETA)

        string muon_counts_vs_primary_cos_theta_hist_name = string("muon_counts_vs_primary_cos_theta_1E") + to_string(file_counter+1);
        muon_counts_vs_primary_cos_theta.push_back(new TH1F(muon_counts_vs_primary_cos_theta_hist_name.c_str(),"Muon Counts at Detector Level vs cos(theta) of Primary;cos(theta) of primary [theta in radians];Muon Count",50,-2,2));
        
        //MUON ENERGIES

        string muon_energies_hist_name = string("muon_energies_1E") + to_string(file_counter+1);
        muon_energies.push_back(new TH1F(muon_energies_hist_name.c_str(),"Muon Energies;Energy[GeV];Muon Count",muon_energies_bins[muon_energies_hist_name],0,muon_energies_range[muon_energies_hist_name]));

        //MUON MOTHER PARTICLES

        string muon_mothers_hist_name = string("muon_mothers_1E") + to_string(file_counter+1);
        muon_mothers.push_back(new TH1F(muon_mothers_hist_name.c_str(),"Mother Particle Codes;Particle Code;Count",muon_mothers_bins[muon_mothers_hist_name],0,muon_mothers_range[muon_mothers_hist_name]));     

        //MUON GRANDMOTHER PARTICLES

        string muon_grandmothers_hist_name = string("muon_grandmothers_1E") + to_string(file_counter+1);
        muon_grandmothers.push_back(new TH1F(muon_grandmothers_hist_name.c_str(),"Grandmother Particle Codes;Particle Code;Count",muon_grandmothers_bins[muon_grandmothers_hist_name],0,muon_grandmothers_range[muon_grandmothers_hist_name]));

        //MUONS COS(THETA)

        string(muon_cos_theta_hist_name) = string("muon_cos_theta_1E") + to_string(file_counter+1);
        muon_cos_theta.push_back(new TH1F(muon_cos_theta_hist_name.c_str(),"cos(theta) of muons;cos(theta) [theta in radians];Muon Count",50,-2,2));
    }  
    
    vector<float> global_muon_counts;
    vector<float> average_muon_counts;
    vector<float> muon_energies_sum;
    vector<float> average_muon_energy;
    
    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        global_muon_counts.push_back(0);
        average_muon_counts.push_back(0);
        muon_energies_sum.push_back(0);
        average_muon_energy.push_back(0);
    }
            
    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {           
        cout << "Processing File " << file_counter + 1 << endl;                

        end_event = particle_info_trees.at(file_counter)->GetEntries();

        cout << "Number of events = " << end_event << endl << endl;

        for(unsigned int event_number = (unsigned int)start_event; event_number < (unsigned int)end_event; event_number++)
        {
            particle_info_trees.at(file_counter)->GetEntry(event_number);
            event_info_trees.at(file_counter)->GetEntry(event_number);

            if(simulated_event_info_collection.at(file_counter)->theta->size() == 0)
            {
                cout << "No event entry" << endl << endl;
                return -1;
            }

            float primary_cos_theta = cos(simulated_event_info_collection.at(file_counter)->theta->at(0));
            
            unsigned int muon_count_in_event = 0;

            for(unsigned int particle_counter = 0; particle_counter < simulated_particle_info_collection.at(file_counter)->particle_description->size(); )
            {
                //check if additional muon info data line                

                if((int)simulated_particle_info_collection.at(file_counter)->particle_description->at(particle_counter)/1000 != 75 && (int)simulated_particle_info_collection.at(file_counter)->particle_description->at(particle_counter)/1000 != 76)
                {
                    particle_counter++;
                    continue;
                }                

                //if code gets here, the next few data lines are associated to a muon

                //now particle_counter = index of muon additional info line

                float muon_z = simulated_particle_info_collection.at(file_counter)->z_coordinate->at(particle_counter);

                particle_counter++;

                //now particle_counter = index of first mother particle data line

                unsigned int mother_data_line_count = 0;

                for(unsigned int mother_counter = particle_counter; simulated_particle_info_collection.at(file_counter)->z_position_at_creation_point->at(mother_counter) > 0; mother_counter++)
                {
                    int mother_particle_id = (-1)*(simulated_particle_info_collection.at(file_counter)->particle_description->at(mother_counter))/1000;
                    muon_mothers.at(file_counter)->Fill(mother_particle_id);
                    mother_data_line_count++;
                }

                particle_counter += mother_data_line_count;

                //now particle_counter = index of first grandmother data line

                unsigned int grandmother_data_line_count = 0;      

                for(unsigned int grandmother_counter = particle_counter; (int)simulated_particle_info_collection.at(file_counter)->particle_description->at(grandmother_counter)/1000 < 0; grandmother_counter++)
                {   
                    int grandmother_particle_id = (-1)*(simulated_particle_info_collection.at(file_counter)->particle_description->at(grandmother_counter))/1000;
                    muon_grandmothers.at(file_counter)->Fill(grandmother_particle_id);
                    grandmother_data_line_count++;
                }
                

                particle_counter += grandmother_data_line_count;

                //now particle_counter = index of muon data line

                muon_count_in_event++;
                global_muon_counts.at(file_counter)++;

                float muon_px = simulated_particle_info_collection.at(file_counter)->px->at(particle_counter);
                float muon_py = simulated_particle_info_collection.at(file_counter)->py->at(particle_counter);
                float muon_pz = simulated_particle_info_collection.at(file_counter)->pz->at(particle_counter);
                float muon_energy = sqrt(pow(muon_px,2) + pow(muon_py,2) + pow(muon_pz,2));
                muon_energies.at(file_counter)->Fill(muon_energy);
                muon_energies_sum.at(file_counter) += muon_energy;

                float muon_x = simulated_particle_info_collection.at(file_counter)->x_coordinate->at(particle_counter);
                float muon_y = simulated_particle_info_collection.at(file_counter)->y_coordinate->at(particle_counter);
                float this_muon_cos_theta = (muon_z)/(sqrt(pow(muon_x,2) + pow(muon_y,2) + pow(muon_z,2)));
                muon_cos_theta.at(file_counter)->Fill(this_muon_cos_theta);    

                muon_counts_vs_primary_cos_theta.at(file_counter)->Fill(primary_cos_theta);

                particle_counter++;           


            }

            muon_counts.at(file_counter)->Fill(muon_count_in_event);
            
        }
        

        cout << "Finished File" << file_counter + 1 << endl << endl;     

    }          

    

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        average_muon_counts.at(file_counter) = global_muon_counts.at(file_counter)/(end_event - start_event);
        average_muon_energy.at(file_counter) = muon_energies_sum.at(file_counter)/global_muon_counts.at(file_counter);
    }

    float primary_energies[primary_energy_count];
    for(unsigned int primary_energy_index = 0; primary_energy_index < primary_energy_count; primary_energy_index++)
    {
        primary_energies[primary_energy_index] = pow(10,primary_energy_index+1);
    }

    float averages_temp[primary_energy_count];
    float average_energy_temp[primary_energy_count];

    float average_muon_counts_errors_x[primary_energy_count];
    for(unsigned int index = 0; index < primary_energy_count; index++)
    {
        average_muon_counts_errors_x[index] = float(0);
    }   
    float average_muon_counts_errors_y[primary_energy_count];
    float average_energy_errors_x[primary_energy_count];
    for(unsigned int index = 0; index < primary_energy_count; index++)
    {
        average_energy_errors_x[index] = float(0);
    }
    float average_energy_errors_y[primary_energy_count];

    for(unsigned file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        averages_temp[file_counter] = average_muon_counts.at(file_counter);
        average_energy_temp[file_counter] = average_muon_energy.at(file_counter);

        float muon_counts_standard_error = muon_counts.at(file_counter)->GetStdDev()/sqrt(end_event - start_event);        
        average_muon_counts_errors_y[file_counter] = (0.434)*(muon_counts_standard_error/average_muon_counts.at(file_counter)); //log-log error bars

        float muon_energies_standard_errors = muon_energies.at(file_counter)->GetStdDev()/sqrt(end_event - start_event);
        average_energy_errors_y[file_counter] = (0.434)*(muon_energies_standard_errors/average_muon_energy.at(file_counter)); //log-log error bars
    }

    TFile* output_file = new TFile(output_file_path.c_str(),"RECREATE");
    if(!output_file)
    {
        cout << "Failed to open output file: " << output_file_path << endl << endl;
        return -1;        
    }

    output_file->cd();   

    for(unsigned int file_counter = 0; file_counter < primary_energy_count; file_counter++)
    {
        muon_counts.at(file_counter)->Write();
        muon_counts_vs_primary_cos_theta.at(file_counter)->Write();
        muon_energies.at(file_counter)->Write();
        muon_cos_theta.at(file_counter)->Write();
        muon_mothers.at(file_counter)->Write();
        muon_grandmothers.at(file_counter)->Write();
    }

    auto average_muons_primary_energy = new TGraphErrors(primary_energy_count,primary_energies,averages_temp,average_muon_counts_errors_x,average_muon_counts_errors_y); 
    average_muons_primary_energy->Write("average_muon_count");

    auto average_muon_energy_primary_energy = new TGraphErrors(primary_energy_count,primary_energies,average_energy_temp,average_energy_errors_x,average_energy_errors_y);
    average_muon_energy_primary_energy->Write("average_muon_energy");

    output_file->Close();

    return 0;

}

int main(int argc, char* argv[])
{
    auto t1 = std::chrono::high_resolution_clock::now();

    switch(argc)
    {
        case 3:
            return muon_analyzer(string(argv[1]),string(argv[2]));
            break;
        case 4:
            return muon_analyzer(string(argv[1]),string(argv[2]),0,atoi(argv[3]));
            break;
        case 5:
            return muon_analyzer(string(argv[1]),string(argv[2]),atoi(argv[3]),atoi(argv[4]));
            break;
        default:
            return -1;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
	cout << "Time elapsed: " << chrono::duration_cast<chrono::seconds>(t2-t1).count() << " s" << endl << endl;

    return 0;
}