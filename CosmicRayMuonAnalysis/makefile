FLAGS=-std=c++11 -g -Wall -fPIC 
#flags necessary to compile an executable which contains access to root libraries
ROOTFLAGS=`root-config --cflags --libs` -L$(ROOTSYS)/lib

LIBDIR=lib
SRCDIR=src
INCDIR=include
PROJLIBS=$(LIBDIR)/corsika_output_file_to_ROOT_cpp.so 


# Assume it contains a main() function from https://gist.github.com/ghl3/3975167
all: $(PROJLIBS) $(SRCDIR)/corsika_output_file_processor $(SRCDIR)/muon_analyzer $(SRCDIR)/steering_file_generator $(SRCDIR)/check_file_exists $(SRCDIR)/muon_parent_analyzer



# Make shared libraries to minimize code compilation, but primarily to
# allow access of C++ classes / functions through python using
#
#  >> ROOT.gSystem.Load(<lib>)  //without .so
#
# then access with
#
#  >> ROOT.<function>(<args>)
#
$(LIBDIR)/%_cpp.so : $(SRCDIR)/%.cpp $(INCDIR)/%.h $(INCDIR)/corsika_constants.h
	##### Making shared library #####
	echo ".L $<++" | root -b -l #make the shared library using ACLiC
	mv $(SRCDIR)/*_cpp* $(LIBDIR) #move it to the libs dir
	$(shell export PROJLIBS="$(PROJLIBS)")
	$(shell export CSCPROJLIBS="$(PROJLIBS)") #for other projects
	
	
clean:
	rm $(LIBDIR)/*.so $(LIBDIR)/*.pcm $(LIBDIR)/*.d

#Makes whatever script you name, add $(ROOTLIBS) after -L$(ROOTSYS)/lib if you need it
$(SRCDIR)/%: $(SRCDIR)/%.cpp $(PROJLIBS) $(INCDIR)/corsika_constants.h
	##### Making executable #####
	$(shell export LD_LIBRARY_PATH=$(LIBDIR):${LD_LIBRARY_PATH})
	g++ $(FLAGS) $(ROOTFLAGS) -L$(LIBDIR) $(PROJLIBS) $< -o $@ 


	