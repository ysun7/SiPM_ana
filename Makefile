ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)
CXX = g++
CXXFLAGS = -g -Wall $(ROOTCFLAGS)
INCFLAGS = -I.
LD = g++
LDFLAGS = $(ROOTLIBS)

.PHONY: all

all: darknoise.exe afterpulse.exe

##############
objects := darknoise.exe afterpulse.exe

darknoise.exe: %.exe: %.o DarkNoise.o EventIO.o Plotter.o 
	$(LD) DarkNoise.o EventIO.o Plotter.o -o $@ $< $(LDFLAGS)

afterpulse.exe: %.exe: %.o AfterPulse.o EventIO.o Plotter.o 
	$(LD) AfterPulse.o EventIO.o Plotter.o -o $@ $< $(LDFLAGS)

#$(objects): %.exe: %.o DarkNoise.o EventIO.o Plotter.o 
#	$(LD) DarkNoise.o EventIO.o Plotter.o -o $@ $< $(LDFLAGS)

##############



# default rule for making .o files from .cc or .C files
%.o: %.cc %.hh
	@echo '<< compiling' $< '>>'
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.C %.h
	@echo '<< compiling' $< '>>'
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.C
	@echo '<< compiling' $< '>>'
	$(CXX) $(CXXFLAGS) -c -o $@ $<


.PHONY: clean
clean: 
	rm -f ./*.o
	rm -f ./*.so
	rm -f ./*.exe a.out

