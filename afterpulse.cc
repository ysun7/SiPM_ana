#include <iostream>
#include "EventIO.hh"
#include "AfterPulse.hh"
#include "Plotter.hh"
#include <string>
#include <fstream>

int main(){

    std::string name;
    int sipm;
    int nevents;
    int nruns;
    int timewindow;
    int triggertime;
    float threshold;
    int biasv_start;
    int biasv_end;
    int nskip;
    int searchstarttime;
    int samplerate;
    std::vector<int> biasvVec;
    std::vector<int> runVec;
    std::vector<int> eventVec;
    std::string input_dir;
    std::string output_dir;
    std::string input_label;
    std::string output_label;

    // read in parameters from the input file "input_parameters" 
    std::ifstream ifile;
    const char *filename = "input_parameters_afterpulse";
    ifile.open(filename);
                          std::cout<<std::endl;std::cout<<"################### analysis summary starts ###################"<<std::endl;std::cout<<std::endl;
    ifile >> name >> sipm;std::cout<<"SiPM #"<<sipm<<std::endl;
    ifile >> name >> nruns;std::cout<<nruns<<" runs"<<std::endl;
    ifile >> name >> nevents;std::cout<<nevents<<" events in each run"<<std::endl;
    ifile >> name >> timewindow;std::cout<<"time window = "<<timewindow<<" ns"<<std::endl;
    ifile >> name >> triggertime;std::cout<<"trigger time = "<<triggertime<<" ns"<<std::endl;
    ifile >> name >> searchstarttime;std::cout<<"search start time = "<<searchstarttime<<" ns"<<std::endl;
    ifile >> name >> samplerate;std::cout<<"sample rate = "<<samplerate<<" samples/ns"<<std::endl;
    ifile >> name >> threshold;std::cout<<"threshold = "<<threshold<<" mV"<<std::endl;
    ifile >> name >> biasv_start;
    ifile >> name >> biasv_end;std::cout<<"biasv from "<<biasv_start<<"V to "<<biasv_end<<"V"<<std::endl;
    ifile >> name >> input_dir;std::cout<<"input data dir: "<<input_dir<<std::endl;
    ifile >> name >> output_dir;std::cout<<"output data dir: "<<output_dir<<std::endl;
    ifile >> name >> input_label;std::cout<<"input data label: "<<input_label<<std::endl;
    ifile >> name >> output_label;std::cout<<"output data label: "<<output_label<<std::endl;
                          std::cout<<std::endl;std::cout<<"--------------------"<<std::endl;std::cout<<std::endl;
    ifile >> name >> nskip;std::cout<<"skip "<<nskip<<" events"<<std::endl;std::cout<<std::endl;
    //when external source noise were found only in one or several particular events, please use "skip function" to skip these events. In "input_parameters" file, please specify the total number of events one wants to skip and append the list of them in the format of "biasv	run	event"
    
    
    if(nskip>0){
      
        int skipbiasv[nskip],skiprun[nskip],skipevent[nskip];
    
        for(int i=0;i<nskip;i++) {
            ifile >> skipbiasv[i] >> skiprun[i] >> skipevent[i];std::cout<<"skip -> ("<<skipbiasv[i]<<"V, run "<<skiprun[i]<<", event"<<skipevent[i]<<" )"<<std::endl; 
	    biasvVec.push_back(skipbiasv[i]);
	    runVec.push_back(skiprun[i]);
	    eventVec.push_back(skipevent[i]);
        }
    }
    
    std::cout<<std::endl;   
    std::cout<<"################### analysis summary ends #####################"<<std::endl;
    ifile.close();
    
    // finish reading parameters from the input file "input_parameters" 
    
    // initialising parameters using the readin parameters 
    AfterPulse afterpulse;
    afterpulse.SetSiPMNumber(sipm);
    afterpulse.SetNumberofEvents(nevents);
    afterpulse.SetNumberofRuns(nruns);
    afterpulse.SetTimeWindow(timewindow);
    afterpulse.SetTriggerTime(triggertime);
    afterpulse.SetSearchStartTime(searchstarttime);
    afterpulse.SetSampleRate(samplerate);
    afterpulse.SetThreshold(threshold);
    afterpulse.SetBiasVStart(biasv_start);
    afterpulse.SetBiasVEnd(biasv_end);
    afterpulse.SetDirDataINPUT(input_dir);
    afterpulse.SetDirDataOUTPUT(output_dir);
    afterpulse.SetLabelINPUT(input_label);
    afterpulse.SetLabelOUTPUT(output_label);
    
    
    if(nskip>0){
        for(int i=0;i<nskip;i++) {
            afterpulse.Skip(biasvVec.at(i),runVec.at(i),eventVec.at(i));
        }
    }
    
    // analysis starts
    afterpulse.Analysis();
    return 0;
}
