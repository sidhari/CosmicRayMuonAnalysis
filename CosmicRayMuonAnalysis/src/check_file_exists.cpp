#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#include "../include/corsika_constants.h"
#include "../include/corsika_output_file_to_ROOT.h"

using namespace std;

int check_file_exists(string base_file_path, unsigned int events)
{   

    int counter = 0;

    ofstream missing_indices;
    missing_indices.open("missing_indices_new.txt",ios::out|ios::trunc);

    int missing_indices_int[] = {57,65,67,72,73,78,79,83,87,88,93,95,98,102,104,105,126,127,128,129,139,140,148,149,155,158,159,161,162,163,164,170,180,181,186,191,192,199,205,206,210,211,219,221,227,239,242,243,245,258,260,271,272,277,285,287,306,313,321,326,330,332,337,338,339,350,351,353,358,366,371,373,382,383,386,387,396,401,408,410,411,415,416,420,425,434,447,453,458,462,469,471,476,477,481,483,484,486,501,505,513,520,521,525,528,537,539,545,548,549,555,577,583,589,590,591,603,607,608,618,620,625,627,632,636,637,651,661,673,675,689,690,696,697,704,706,708,722,726,727,732,733,736,744,747,749,753,756,758,759,760,763,768,772,775,781,788,792,795,796,798,807,811,812,813,819,822,840,842,846,853,860,887,898,909,912,914,920,921,926,927,929,936,942,943,944,946,947,952,953,954,956,957,962,968,972,976,977,978,983,992,997,999,1000};

    for(unsigned int file_counter = 0; file_counter < events; file_counter++)
    {   
        string real_input_filepath = base_file_path;

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

        ifstream ifile;
        ifile.open(real_input_filepath.c_str());
        if(ifile)
            continue;
        else
        {   
            if(find(begin(missing_indices_int),end(missing_indices_int),file_counter+1) == end(missing_indices_int))
                continue;

            missing_indices << file_counter + 1 << ",";
            counter++;
        }
        
    }

    cout << endl << counter << endl << endl;

    return 0;
}

int main(int argc,char* argv[])
{
    switch (argc)
    {
    case 3:
        return check_file_exists(string(argv[1]),atoi(argv[2]));
        break;
    
    default:
        return -1;
        break;
    }

    return 0;
}