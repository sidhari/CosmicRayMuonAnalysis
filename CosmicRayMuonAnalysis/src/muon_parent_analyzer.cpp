#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <map>

//ROOT headers

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TPie.h>

#include "../include/corsika_output_file_to_ROOT.h"
#include "../include/corsika_constants.h"
#include "../include/ROOT_tree_reader.h"



using namespace std;

bool sortByVal(const pair<unsigned int,unsigned int> &a,const pair<unsigned int,unsigned int> &b) 
{ 
    return (a.second > b.second); 
} 

int muon_parent_analyzer(string input_file_path, string output_file_path, int start_event = 0, int end_event = -1)
{   
    map<unsigned int,string> PID_to_name;
    map<unsigned int,string>::iterator PID_to_name_it;
    PID_to_name[1] = string("Gamma");
    PID_to_name[2] = string("E plus");
    PID_to_name[3] = string("E minus");
    PID_to_name[5] = string("Mu plus");
    PID_to_name[6] = string("Mu minus");
    PID_to_name[7] = string("Pi zero");
    PID_to_name[8] = string("Pi Plus");
    PID_to_name[9] = string("Pi Minus");
    PID_to_name[10] = string("K Long");
    PID_to_name[11] = string("K plus");
    PID_to_name[12] = string("K Minus");
    PID_to_name[13] = string("Neutron");
    PID_to_name[14] = string("Proton");
    PID_to_name[15] = string("Anti-Proton");
    PID_to_name[16] = string("K short");
    PID_to_name[17] = string("Eta");
    PID_to_name[18] = string("Lambda");
    PID_to_name[19] = string("Sigma plus");
    PID_to_name[20] = string("Sigma zero");
    PID_to_name[21] = string("Sigma minus");
    PID_to_name[22] = string("Xi zero");
    PID_to_name[23] = string("Xi minus");
    PID_to_name[24] = string("Omega minus");
    PID_to_name[25] = string("Anti-eta");
    PID_to_name[26] = string("Anti-lambda");
    PID_to_name[27] = string("Anti-sigma minus");
    PID_to_name[28] = string("Anti-sigma zero");
    PID_to_name[29] = string("Anti-sigma plus");
    PID_to_name[30] = string("Anti-xi zero");
    PID_to_name[31] = string("Anti-xi plus");
    PID_to_name[32] = string("Anti-omega plus");
    PID_to_name[48] = string("Eta prime");
    PID_to_name[49] = string("Phi");
    PID_to_name[50] = string("Omega (meson)");
    PID_to_name[51] = string("Rho zero");
    PID_to_name[52] = string("Rho plus");
    PID_to_name[53] = string("Rho minus");
    PID_to_name[54] = string("Delta plus plus");
    PID_to_name[55] = string("Delta plus");
    PID_to_name[56] = string("Delta zero");
    PID_to_name[57] = string("Delta minus");
    PID_to_name[58] = string("Anti-delta minus minus");
    PID_to_name[59] = string("Anti-delta minus");
    PID_to_name[60] = string("Anti-delta zero");
    
    map<unsigned int,unsigned int> mother_particle_counter;
    map<unsigned int,unsigned int>::iterator mother_particle_counter_it;

    map<unsigned int,unsigned int> grandmother_particle_counter;
    map<unsigned int,unsigned int>::iterator grandmother_particle_counter_it;

    unsigned int total_mothers = 0;
    unsigned int total_grandmothers = 0;

    cout << endl << endl;

    TFile* particle_info_file = TFile::Open(input_file_path.c_str());
    cout << "Using ROOT file " << string(input_file_path) << endl << endl;

    if(!particle_info_file)
    {
        cout << "Error opening file " << string(input_file_path) << endl << endl;
        return -1;
    }

    TTree* PD_tree = (TTree*)particle_info_file->Get("PD_tree");

    if(!PD_tree)
    {
        cout << "Error extracting tree" << endl << endl;
        return -1;
    }

    PD_tree_reader* simulated_particle_info = new PD_tree_reader(PD_tree);

    if((end_event > PD_tree->GetEntries()) || (end_event < 0))
    {
        end_event = PD_tree->GetEntries();
    }   

    for(int event_counter = start_event; event_counter < end_event; event_counter++)
    {
        if(event_counter%100 == 0)
        {
            cout << "EVENT " << event_counter << endl << endl;
        }

        PD_tree->GetEntry(event_counter);

        if(simulated_particle_info->particle_description->size() != simulated_particle_info->mother_particle_description->size() || simulated_particle_info->particle_description->size() != simulated_particle_info->grandmother_particle_description->size() || simulated_particle_info->mother_particle_description->size() != simulated_particle_info->grandmother_particle_description->size())
        {
            cout << "Sizes of particle, mother, and grandmother info vectors are different" << endl << endl;
            return -1;
        }

        for(unsigned int particle_counter = 0; particle_counter < simulated_particle_info->particle_description->size(); particle_counter++)
        {
            if((unsigned int)simulated_particle_info->particle_description->at(particle_counter)/1000 != mu_plus_id && (unsigned int)simulated_particle_info->particle_description->at(particle_counter)/1000 != mu_minus_id)
            {
                continue; //Not a muon
            }

            unsigned int mother_particle_id = (-1)*simulated_particle_info->mother_particle_description->at(particle_counter)/1000;
            unsigned int grandmother_particle_id = (-1)*simulated_particle_info->grandmother_particle_description->at(particle_counter)/1000;

            if(mother_particle_id != 0)
            {
                mother_particle_counter[mother_particle_id]++;
                total_mothers++;
            }

            if(grandmother_particle_id != 0)
            {
                grandmother_particle_counter[grandmother_particle_id]++;
                total_grandmothers++;
            }          
            
        }

    }

    vector<pair<unsigned int,unsigned int>> mother_particle_counter_vec;
    vector<pair<unsigned int,unsigned int>> grandmother_particle_counter_vec;
    
    for(mother_particle_counter_it = mother_particle_counter.begin(); mother_particle_counter_it != mother_particle_counter.end(); mother_particle_counter_it++)
    {
        mother_particle_counter_vec.push_back(make_pair(mother_particle_counter_it->first,mother_particle_counter_it->second));
    }

    for(grandmother_particle_counter_it = grandmother_particle_counter.begin(); grandmother_particle_counter_it != grandmother_particle_counter.end(); grandmother_particle_counter_it++)
    {
        grandmother_particle_counter_vec.push_back(make_pair(grandmother_particle_counter_it->first,grandmother_particle_counter_it->second));
    }

    sort(mother_particle_counter_vec.begin(),mother_particle_counter_vec.end(),sortByVal);
    sort(grandmother_particle_counter_vec.begin(),grandmother_particle_counter_vec.end(),sortByVal);

    //unsigned int mother_particle_counter_size = mother_particle_counter.size();
    //unsigned int grandmother_particle_counter_size = grandmother_particle_counter.size();

    map<unsigned int,string> mother_labels;
    mother_labels[5] = string("Other");

    map<unsigned int,string> grandmother_labels;
    grandmother_labels[5] = string("Other");

    float mother_pie_vals[6];
    mother_pie_vals[5] = 0;

    float grandmother_pie_vals[6];
    grandmother_pie_vals[5] = 0;

    int mother_entry_number = 0;
    int grandmother_entry_number = 0;

    int color[] = {2,3,4,5,6,7};

    for(unsigned int vec_index = 0; vec_index < mother_particle_counter_vec.size(); vec_index++)
    {
        int particle_id = mother_particle_counter_vec.at(vec_index).first;

        PID_to_name_it = PID_to_name.find(particle_id);
        if(PID_to_name_it == PID_to_name.end())
        {
            cout << "Label name not found, PID = " << particle_id << endl << endl;         
            continue;
        }
        else
        {
            if(mother_entry_number >= 5)
            {
                mother_pie_vals[5] += mother_particle_counter_vec.at(vec_index).second;
                continue;
            }
            else
            {
                mother_pie_vals[mother_entry_number] = mother_particle_counter_vec.at(vec_index).second;
                mother_labels[mother_entry_number] = string(PID_to_name_it->second);
                mother_entry_number++;
                continue;
            }
               
        }
        
    }

    for(unsigned int vec_index = 0; vec_index < grandmother_particle_counter_vec.size(); vec_index++)
    {
        int particle_id = grandmother_particle_counter_vec.at(vec_index).first;

        PID_to_name_it = PID_to_name.find(particle_id);
        if(PID_to_name_it == PID_to_name.end())
        {
            cout << "Label name not found, PID = " << particle_id << endl << endl;         
            continue;
        }
        else
        {
            if(grandmother_entry_number >= 5)
            {
                grandmother_pie_vals[5] += grandmother_particle_counter_vec.at(vec_index).second;
                continue;
            }
            else
            {
                grandmother_pie_vals[mother_entry_number] = grandmother_particle_counter_vec.at(vec_index).second;
                grandmother_labels[grandmother_entry_number] = string(PID_to_name_it->second);
                grandmother_entry_number++;
                continue;
            }
               
        }
        
    }

    /*mother_particle_counter_it = mother_particle_counter.begin();    

    grandmother_particle_counter_it = grandmother_particle_counter.begin();    

    while(mother_particle_counter_it != mother_particle_counter.end())
    {           
        unsigned int particle_id = mother_particle_counter_it->first;
        if(mother_particle_counter_it->second < total_mothers/100)
        {
            mother_pie_vals[0] += mother_particle_counter_it->second;
            mother_particle_counter_it++;
            continue;
        }

        PID_to_name_it = PID_to_name.find(particle_id);
        if(PID_to_name_it == PID_to_name.end())
        {
            mother_pie_vals[1] += mother_particle_counter_it->second;
            mother_particle_counter_it++;
            continue;
        }
        else
        {
            mother_labels[mother_entry_number] = string(PID_to_name_it->second);
            mother_pie_vals[mother_entry_number] = mother_particle_counter_it->second;
            mother_entry_number++;
            mother_particle_counter_it++;
            continue;
        }
                
    }
    
    while(grandmother_particle_counter_it != grandmother_particle_counter.end())
    {        
        unsigned int particle_id = grandmother_particle_counter_it->first;
        if(grandmother_particle_counter_it->second < total_grandmothers/100)
        {
            grandmother_pie_vals[0] += grandmother_particle_counter_it->second;
            grandmother_particle_counter_it++;
            continue;
        }

        PID_to_name_it = PID_to_name.find(particle_id);
        if(PID_to_name_it == PID_to_name.end())
        {
            grandmother_pie_vals[1] += grandmother_particle_counter_it->second;
            grandmother_particle_counter_it++;
            continue;
        }
        else
        {
            grandmother_labels[grandmother_entry_number] = string(PID_to_name_it->second);
            grandmother_pie_vals[grandmother_entry_number] = grandmother_particle_counter_it->second;
            grandmother_entry_number++;
            grandmother_particle_counter_it++;
            continue;
        }
    }*/

    int mother_nvals = sizeof(mother_pie_vals)/sizeof(mother_pie_vals[0]);
    int grandmother_nvals = sizeof(grandmother_pie_vals)/sizeof(grandmother_pie_vals[0]);


    TPie* mother_pie = new TPie("mother_pie","mother_pie",mother_nvals,mother_pie_vals,color);
    TPie* grandmother_pie = new TPie("grandmother_pie","grandmother_pie",grandmother_nvals,grandmother_pie_vals,color);

    for(int entry_number = 0; entry_number <= mother_entry_number; entry_number++)
    {
        mother_pie->SetEntryLabel(entry_number,mother_labels[entry_number].c_str());
    }

    for(int entry_number = 0; entry_number <= grandmother_entry_number; entry_number++)
    {
        grandmother_pie->SetEntryLabel(entry_number,grandmother_labels[entry_number].c_str());
    }

    TFile* output_file = new TFile(output_file_path.c_str(),"RECREATE");
    if(!output_file)
    {
        cout << "Failed to open output file: " << output_file_path << endl << endl;
        return -1;        
    }

    output_file->cd();  
    mother_pie->Write();
    grandmother_pie->Write();

    return 0;

}

int main(int argc, char* argv[])
{
    return muon_parent_analyzer(string(argv[1]),string(argv[2]));

}