/*
    Author: Siddharth Hariprakash
    Date Created: 2-17-2020
*/

/*  
    Contains function definitions required to parse through the block structure of corsika output files. Check User's Guide at https://www.ikp.kit.edu/corsika/70.php for further info.
*/

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

#include "../include/corsika_output_file_to_ROOT.h"
#include "../include/corsika_constants.h"



using namespace std;


//
//  class corsika_output_files
//


int corsika_output_file_reader::process_run_header_sub_block(vector<float> run_header_sub_block,TTree* RUNH_tree,run_header_sub_block_branches RUNH_sub_block_branches)
{
    RUNH_sub_block_branches.run_number = (unsigned int)run_header_sub_block.at(1);
    RUNH_sub_block_branches.date_of_begin_run = (unsigned int)run_header_sub_block.at(2);
    RUNH_sub_block_branches.energy_spectrum_slope = run_header_sub_block.at(15);
    RUNH_sub_block_branches.energy_range_low = run_header_sub_block.at(16);
    RUNH_sub_block_branches.energy_range_high = run_header_sub_block.at(17);

    RUNH_tree->Fill();
    return 0;

}

int corsika_output_file_reader::process_event_header_sub_block(vector<float> event_header_sub_block, TTree* EVTH_tree,event_header_sub_block_branches EVTH_sub_block_branches)
{
    EVTH_sub_block_branches.event_number = (unsigned long long int)event_header_sub_block.at(1);
    EVTH_sub_block_branches.primary_id = event_header_sub_block.at(2);
    EVTH_sub_block_branches.primary_energy = event_header_sub_block.at(3);
    EVTH_sub_block_branches.primary_px = event_header_sub_block.at(7);
    EVTH_sub_block_branches.primary_py = event_header_sub_block.at(8);
    EVTH_sub_block_branches.primary_pz = event_header_sub_block.at(9);
    EVTH_sub_block_branches.theta = event_header_sub_block.at(10);
    EVTH_sub_block_branches.phi = event_header_sub_block.at(11);

    EVTH_tree->Fill();
    return 0;
}

bool corsika_output_file_reader::is_particle_data_line(float particle_id)
{
    if(particle_id > 0)
        return true;
    else
        return false;
}

bool corsika_output_file_reader::is_mother_particle_data_line(float particle_id,float z_position)
{
    if(particle_id < 0 && z_position > float(0))
        return true;
    else
        return false;
}

bool corsika_output_file_reader::is_grandmother_particle_data_line(float particle_id,float z_position)
{
    if(particle_id < 0 && z_position < float(0))
        return true;
    else
        return false;
}

int corsika_output_file_reader::process_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree)
{   
    //if(data_line.at(0) == 0) //file padded with zeros to fill out the block
    //return 0;

    data_sub_block_information->particle_description.push_back(data_line.at(0));    
    data_sub_block_information->px.push_back(data_line.at(1));
    data_sub_block_information->py.push_back(data_line.at(2));
    data_sub_block_information->pz.push_back(data_line.at(3));
    data_sub_block_information->x_coordinate.push_back(data_line.at(4));
    data_sub_block_information->y_coordinate.push_back(data_line.at(5));
    data_sub_block_information->time_since_first_interaction.push_back(data_line.at(6));
    
    return 0;
}

int corsika_output_file_reader::process_mother_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree)
{   
    //if(data_line.at(0) == 0) //file padded with zeros to fill out the block
    //return 0;

    data_sub_block_information->mother_particle_description.push_back(data_line.at(0));
    data_sub_block_information->mother_px.push_back(data_line.at(1));
    data_sub_block_information->mother_py.push_back(data_line.at(2));
    data_sub_block_information->mother_pz.push_back(data_line.at(3));
    data_sub_block_information->mother_x_coordinate.push_back(data_line.at(4));
    data_sub_block_information->mother_y_coordinate.push_back(data_line.at(5));
    data_sub_block_information->z_position_at_creation_point.push_back(data_line.at(6));    

    return 0;
}

int corsika_output_file_reader::process_grandmother_particle_data_line(data_sub_block_branches* data_sub_block_information,vector<float> data_line,TTree* PD_tree)
{    
    if(data_line.at(0) == 0) //file padded with zeros to fill out the block
    return 0;

    data_sub_block_information->grandmother_particle_description.push_back(data_line.at(0));
    data_sub_block_information->grandmother_px.push_back(data_line.at(1));
    data_sub_block_information->grandmother_py.push_back(data_line.at(2));
    data_sub_block_information->grandmother_pz.push_back(data_line.at(3));    
    data_sub_block_information->z_position_at_interaction_point.push_back(data_line.at(6));

    return 0;
}

int corsika_output_file_reader::process_input_file(string input_filepath,TTree* RUNH_tree,TTree* EVTH_tree,TTree* PD_tree,int start,int end)
{    
    unsigned int actual_counter = 0;

    data_sub_block_branches* data_sub_block_information = new data_sub_block_branches(PD_tree);
    run_header_sub_block_branches RUNH_sub_block_branches(RUNH_tree);
    event_header_sub_block_branches EVTH_sub_block_branches(EVTH_tree);

    for(unsigned int file_counter = (unsigned int)start; file_counter < (unsigned int)end; file_counter++)
    {   
        cout << endl << endl << "FILE " << file_counter + 1 << endl << endl;

        string real_input_filepath = input_filepath;

        unsigned int digits = 0;
        unsigned int temp_file_counter = file_counter+1;
        while(temp_file_counter > 0)
        {
            digits++;
            temp_file_counter = temp_file_counter/10;
        }

        unsigned int number_of_zeros = zeros_in_output_name - digits;
         while(number_of_zeros > 0)
         {
             real_input_filepath = real_input_filepath + to_string(0);
             number_of_zeros--;
         }

         real_input_filepath = real_input_filepath + to_string(file_counter+1);

        ifstream input_file;
        input_file.open(real_input_filepath,ios::in); //open input file for processing

        if(!input_file)
        {
            cout << "Can't open input file " << string(real_input_filepath) <<  endl << endl;
            continue;
        }
            

        cout << real_input_filepath << endl << endl;

        actual_counter++;

        vector<float> file_contents; //store contents of the input file here 4 bytes at a time till eof is reached

        for(unsigned int file_contents_index = 0; input_file.eof() == false; file_contents_index++) 
        {          
            float temp;
            input_file.read((char*)&temp,4);
            if(file_contents_index == 0)
            continue;
            file_contents.push_back(temp);
            
            //input_file.read((char*)&file_contents.at(file_contents_index),4);

        }      

        unsigned long long int event_counter = 0;

        unsigned int ancestor_count = 0; //0 => mother particle, 1 => grandmother particle
        unsigned int sub_block_count = 0; //have no idea why, but skip 8 bytes every 21 sub_blocks
        unsigned int data_line_count = 0; //keep track of number of particles seen, every 39 particles -> 1 sub-block

        //now file_contents has all the info we need, don't need to deal with the actual file any more
        //loop through file_contents, search for headers/data-blocks and process them   

        for(unsigned int file_contents_index = 0; file_contents_index < file_contents.size(); ) //updating file_content_index will depend on if/which header we run into
        {   
            if((sub_block_count > 0) && ((sub_block_count%21)==0))
            {   
                file_contents_index+=2;
                sub_block_count = 0;
                continue;
            }

            if(!strcmp("RUNH",(char*)&file_contents.at(file_contents_index))) //if a run Header is encountered, then make the run header sub-block and process it
            {   
                sub_block_count++;

                vector<float> run_header_sub_block;
                
                for(unsigned int file_contents_index_1 = file_contents_index; file_contents_index_1 < file_contents_index + block_size; file_contents_index_1++)
                {
                    run_header_sub_block.push_back(file_contents.at(file_contents_index_1));
                }             

                if(process_run_header_sub_block(run_header_sub_block,RUNH_tree,RUNH_sub_block_branches))
                {
                    cout << "Error processing run header sub-block at index " << file_contents_index << endl << endl;
                    return -1;
                }

                file_contents_index+=block_size;
                continue;

            }
            else if(!strcmp("EVTH",(char*)&file_contents.at(file_contents_index))) // if an event header is encountered, make the sub-block and process it
            { 
                sub_block_count++;

                vector<float> event_header_sub_block;                       

                for(unsigned int file_contents_index_1 = file_contents_index; file_contents_index_1 < file_contents_index + block_size; file_contents_index_1++)
                {
                    event_header_sub_block.push_back(file_contents.at(file_contents_index_1));
                }

                if(process_event_header_sub_block(event_header_sub_block,EVTH_tree,EVTH_sub_block_branches))
                {
                    cout << "Error processing event header sub-block at index " << file_contents_index << endl << endl;
                    return -1;
                }

                //data_sub_block_information->event_number = event_header_sub_block.at(1);
                event_counter = (unsigned long long int)event_header_sub_block.at(1);

                cout << "EVENT NUMBER: " << event_header_sub_block.at(1) << " " << file_contents_index << endl << endl;

                file_contents_index+=block_size; 
                continue;            

            }
            else if(!strcmp("EVTE",(char*)&file_contents.at(file_contents_index)))
            {   
                sub_block_count++;

                /*unsigned int particle_size = data_sub_block_information->particle_description.size();
                unsigned int mother_particle_size = data_sub_block_information->mother_particle_description.size();
                unsigned int grandmother_particle_size = data_sub_block_information->grandmother_particle_description.size();

                if(mother_particle_size < particle_size)
                {
                    for(unsigned int padder = mother_particle_size; padder < particle_size; padder++)
                    {
                        data_sub_block_information->mother_particle_description.push_back(float(0));
                        data_sub_block_information->mother_px.push_back(float(0));
                        data_sub_block_information->mother_py.push_back(float(0));
                        data_sub_block_information->mother_pz.push_back(float(0));
                        data_sub_block_information->z_position_at_creation_point.push_back(float(0));
                    }
                }

                if(grandmother_particle_size < particle_size)
                {
                    for(unsigned int padder = grandmother_particle_size; padder < particle_size; padder++)
                    {
                        data_sub_block_information->grandmother_particle_description.push_back(float(0));
                        data_sub_block_information->grandmother_px.push_back(float(0));
                        data_sub_block_information->grandmother_py.push_back(float(0));
                        data_sub_block_information->grandmother_pz.push_back(float(0));
                    }
                }*/
                
                file_contents_index+=block_size;
                continue;
            }  
            else if(!strcmp("RUNE",(char*)&file_contents.at(file_contents_index)) || !strcmp("LONG",(char*)&file_contents.at(file_contents_index)))
            {   
                sub_block_count++;

                file_contents_index+=block_size;
                continue;
            }
            else //then it is a particle data line
            {   
                data_line_count++;

                if(data_line_count > 0 && (data_line_count%39) == 0)
                {
                    sub_block_count++;
                }

                vector<float> data_line;            

                for(unsigned int word_counter = 0; word_counter < data_line_size; word_counter++)
                {
                    data_line.push_back(file_contents.at(file_contents_index + word_counter));
                }

                //need to figure out which kind of data line it is (additional muon info, mother, grandmother, or actual particle): 
                
                int particle_id = data_line.at(0);                

                if((particle_id/1000 == 75) || particle_id/1000 == 76) //skip additional muon info data line, don't care about this for now
                {   
                    data_sub_block_information->z_coordinate.push_back(data_line.at(6));
                    file_contents_index+=data_line_size;
                    continue;
                }

                if(is_particle_data_line((float)particle_id))
                {
                    if(process_particle_data_line(data_sub_block_information,data_line,PD_tree))
                    {   
                        cout << "Error processing particle data line at index " << file_contents_index << endl << endl;
                        return -1;
                    }

                    if(event_counter == 0)
                    {
                        cout << "event = 0, something went wrong" << endl << endl;
                        return -1;
                    }

                    data_sub_block_information->event_number = event_counter;
                    PD_tree->Fill();//fill tree once per particle           
                    data_sub_block_information->clear_vectors(); //prepare for next particle by clearing all the vectors                    

                }
                else if(is_mother_particle_data_line((float)particle_id,data_line.at(6)))
                {
                    if(process_mother_particle_data_line(data_sub_block_information,data_line,PD_tree))
                    {
                        cout << "Error processing mother particle data line at index " << file_contents_index << endl << endl;
                        return -1;
                    }

                    ancestor_count = 1; //grandmother data line up next
                }
                else if(is_grandmother_particle_data_line((float)particle_id,data_line.at(6)))
                {
                    if(process_grandmother_particle_data_line(data_sub_block_information,data_line,PD_tree))
                    {
                        cout << "Error processing grandmother particle data line at index " << file_contents_index << endl << endl;
                        return -1;
                    }

                    ancestor_count = 0; //particle data line up next
                }

                file_contents_index+=data_line_size;
                continue;               

            } 

        }

        file_contents.clear();

    }        

    cout << actual_counter <<  " "  << start << " " << end << endl << endl;

    return 0;

}

//
// class data_sub_block_branches
//

void data_sub_block_branches::clear_vectors()
{
    particle_description.clear();
    px.clear();
    py.clear();
    pz.clear();
    x_coordinate.clear();
    y_coordinate.clear();
    z_coordinate.clear();
    time_since_first_interaction.clear();

    mother_particle_description.clear();
    mother_px.clear();
    mother_py.clear();
    mother_pz.clear();
    mother_x_coordinate.clear();
    mother_y_coordinate.clear();
    z_position_at_creation_point.clear();

    grandmother_particle_description.clear();
    grandmother_px.clear();
    grandmother_py.clear();
    grandmother_pz.clear();
    z_position_at_interaction_point.clear();

}









