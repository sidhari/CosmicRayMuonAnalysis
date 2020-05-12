#Submits condor jobs assuming the necessary directories and executables exist

import glob
import os

SRC_DIR = "../src/" #This is where the executable is

RUN_ON_CONDOR_SCRIPT = "run_on_condor.sh" #bash file that actually executes the program
EXECUTABLE = "corsika_output_file_processor"

SHELL_FOLDER = "condor/" #Condor scripts go here 
TEMP_RUN_DIR = "condor/run/" #executables go here
OTHER_DIR = "condor/other/"

TAR_PATH = TEMP_RUN_DIR + "CosmicRayMuonAnalysis.tar.gz"

def write_submit_script(script_file, run_folder, input_file_path, output_file,start,end):

    
    script_file.write("universe               = vanilla\n")
    script_file.write("executable             = " + (run_folder + RUN_ON_CONDOR_SCRIPT) + "\n")
    script_file.write("arguments              = \"" + EXECUTABLE + " " + input_file_path + " " + output_file + " " + str(start) + " " + str(end) + "\"\n")
    script_file.write("output                 = " + (OTHER_DIR + output_file) + ".out\n")
    script_file.write("error                  = " + (OTHER_DIR + output_file) + ".err\n")
    script_file.write("log                    = " + (OTHER_DIR + output_file) + ".log\n")
    script_file.write("should_transfer_files  = YES\n")
    script_file.write("transfer_input_files   = " + (run_folder + RUN_ON_CONDOR_SCRIPT) + "," + TAR_PATH + "\n")
    script_file.write("transfer_output_files  = " + output_file + "\n")
    script_file.write("transfer_output_remaps = \"" + output_file + " = /eos/user/s/sharipra/CORSIKA/OUTPUTS/PARTICLEINFO/1E6/" + output_file + "\"\n")
    script_file.write("whentotransferoutput   = ON_EXIT\n")
    script_file.write("getenv                 = True\n")
    script_file.write("on_exit_remove         = (ExitCode == 0)\n")
    script_file.write("MaxTransferOutputMB    = 12000\n")
    script_file.write("+JobFlavour            = \"tomorrow\"\n")
    script_file.write("request_memory         = 12 GB\n")
    script_file.write("queue                  1\n")



#copy executable to the run directory
print "\nCopying executable to the temporary run folder\n"
if os.path.isfile(SRC_DIR + EXECUTABLE):
    os.system('cp ' + SRC_DIR + EXECUTABLE + ' ' + TEMP_RUN_DIR)
else:
    print "Could not find executable\n"
    exit

#Copy the bash file needed for execution 
print "Copying Condor bash file\n"
if os.path.isfile(RUN_ON_CONDOR_SCRIPT):
    os.system('cp ' + RUN_ON_CONDOR_SCRIPT + ' ' + TEMP_RUN_DIR)
else:
    print "Could not find the bash script\n"
    exit

INPUT_FILE_PATH = "/eos/user/s/sharipra/CORSIKA/OUTPUTS/PARTICLEINFO/1E6/DAT"

files = 1000

start = 0
end = 1

for num in range(1,files+1):

    digits = 0
    temp_num = end
    while(temp_num > 0):
        digits = digits + 1
        temp_num = temp_num//10
     
    zero_count = 6 - digits

    INPUT_TEMP = INPUT_FILE_PATH

    while(zero_count > 0):
        INPUT_TEMP = INPUT_TEMP + str(0)
        zero_count = zero_count - 1

    INPUT_TEMP = INPUT_TEMP + str(end)

    if os.path.isfile(INPUT_TEMP):        
    
        print "Making Condor script for %s"%INPUT_TEMP

        OUTPUT_FILE = "nevents_1E3_PE_1E8_" + str(num) + ".root"

        print "Output file name is %s"%OUTPUT_FILE

        SCRIPT_FILE_NAME = SHELL_FOLDER + OUTPUT_FILE + '.sub'
        SCRIPT_FILE = open(SCRIPT_FILE_NAME,"w")

        write_submit_script(SCRIPT_FILE, TEMP_RUN_DIR, INPUT_FILE_PATH, OUTPUT_FILE, start, end)
        SCRIPT_FILE.close()

        os.system("condor_submit " + SCRIPT_FILE_NAME)

        start = start + 1
        end = end + 1

    else:
        print "\nNot simulated yet\n"
        start = start + 1
        end = end + 1


    












