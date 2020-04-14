#Submits condor jobs assuming the necessary directories and executables exist

import glob
import os

EOS = "/eos/user/s/sharipra/" #personal EOS directory path, this is where the output directory will be placed
PROJECT_DIR =  EOS + "CORSIKA/" #All files related to CORSIKA will go here
SRC_DIR = "../" #This is where the executable is

RUN_ON_CONDOR_SCRIPT = "run_on_condor.sh" #bash file that actually executes the program
EXECUTABLE = "corsika77100Linux_QGSII_urqmd" #simulation executable

OUTPUT_IDENTIFIER = "DAT"

OUTPUT_FOLDER = PROJECT_DIR + "OUTPUTS/" #Outputs go here
PARTICLE_INFO =  OUTPUT_FOLDER + "PARTICLEINFO/1E6/"
LONG_INFO = OUTPUT_FOLDER + "LONGINFO/1E6/"
SHELL_FOLDER = "condor/" #Condor scripts go here 
TEMP_RUN_DIR = "condor/run/" #executables go here
OTHER_DIR = "condor/other/"

TAR_PATH = TEMP_RUN_DIR + "corsika.tar.gz"

def write_submit_script(script_file, run_folder, input_file_path, output_file):

    input_file = input_file_path.split('/')[-1]
    long_output_file = output_file + ".long"

    script_file.write("universe               =vanilla\n")
    script_file.write("executable             =" + (run_folder + RUN_ON_CONDOR_SCRIPT) + "\n")
    script_file.write("arguments              =" + EXECUTABLE + " " + input_file + " " + output_file + " " + long_output_file + "\n")
    script_file.write("output                 =" + (OTHER_DIR + output_file) + ".out\n")
    script_file.write("error                  =" + (OTHER_DIR + output_file) + ".err\n")
    script_file.write("log                    =" + (OTHER_DIR + output_file) + ".log\n")
    script_file.write("should_transfer_files  = YES\n")
    script_file.write("transfer_input_files   =" + (run_folder + RUN_ON_CONDOR_SCRIPT) + "," + TAR_PATH + "," + input_file_path + "\n")
    script_file.write("transfer_output_files  =" + output_file + "," + long_output_file + "\n")
    script_file.write("transfer_output_remaps = \"" + output_file + "=" + PARTICLE_INFO + output_file + ";" + long_output_file + "=" + LONG_INFO + long_output_file + "\"\n")
    script_file.write("whentotransferoutput   =ON_EXIT\n")
    script_file.write("getenv                 =True\n")
    script_file.write("queue                  1\n")



#copy executable to the run directory
#print "Copying executable to the temporary run folder\n"
#if os.path.isfile(SRC_DIR + EXECUTABLE):
#    os.system('cp ' + SRC_DIR + EXECUTABLE + ' ' + TEMP_RUN_DIR)
#else:
#    print "Could not find executable\n"
#    exit

#Copy the bash file needed for execution 
#print "Copying Condor bash file\n"
#if os.path.isfile(RUN_ON_CONDOR_SCRIPT):
#    os.system('cp ' + RUN_ON_CONDOR_SCRIPT + ' ' + TEMP_RUN_DIR)
#else:
#    print "Could not find the bash script\n"
#    exit

BASE_INPUT_FILE = "../Inputs/1E6/all-inputs_"
events = 1

for num in range(1,events+1):

    INPUT_FILE_PATH = BASE_INPUT_FILE + str(num)
    print "Making Condor script for %s"%INPUT_FILE_PATH
    print "\n"

    OUTPUT_FILE = OUTPUT_IDENTIFIER

    digits = 0
    temp_num = num
    while(temp_num > 0):
        digits = digits + 1
        temp_num = temp_num//10
    
    zero_count = 6 - digits

    while(zero_count > 0):
        OUTPUT_FILE = OUTPUT_FILE + str(0)
        zero_count = zero_count - 1

    OUTPUT_FILE = OUTPUT_FILE + str(num)

    print "Output file name is %s"%OUTPUT_FILE
    print "\n"

    SCRIPT_FILE_NAME = SHELL_FOLDER + OUTPUT_FILE + '.sub'
    SCRIPT_FILE = open(SCRIPT_FILE_NAME,"w")

    write_submit_script(SCRIPT_FILE, TEMP_RUN_DIR, INPUT_FILE_PATH, OUTPUT_FILE)
    SCRIPT_FILE.close()

    os.system("condor_submit " + SCRIPT_FILE_NAME + " -allow-crlf-script")












