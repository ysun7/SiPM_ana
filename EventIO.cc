/**
 * \file EventIO.cc
 *
 * \brief Data process and access interface
 *
 * Original Version: 
 * \author Yujing Sun - UH 2015
 */

#define EventIO_cc

#include "EventIO.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>

using namespace std;

///////////////////////////////////////////////////////////

EventIO::EventIO()
{
    //std::cout << "Object is being created" << std::endl;
    this->InitialisePars();
}

///////////////////////////////////////////////////////////

EventIO::~EventIO()
{
    //std::cout << "Object is being destroyed" << std::endl;
}

///////////////////////////////////////////////////////////

bool EventIO::FilesExist(const std::string name) 
{
    std::ifstream infile(name.c_str());
 
    if( infile.good()) {
       //std::cout<<"Input file "<< fInputFile << std::endl;
        return true;
    }
 
    else {
        std::cout<<"Input file: "<< name << " does not exist !!!" << std::endl;
        return false;
    }
}

///////////////////////////////////////////////////////////

void EventIO::InitialisePars() 
{
    //std::cout << "Initialsing in EventIO" << std::endl;
    this->SetNumberofRuns(100);
    this->SetNumberofEvents(100);
    this->SetTimeWindow(2000000);
    this->SetThreshold(0.35);
    this->SetBiasVStart(23);
    this->SetBiasVEnd(26);
    this->SetDirDataINPUT("/home/sunyujing/new_desktop/c++study/SiPM_analysis_software/SiPM123_20150410/c3/");
    this->SetDirDataOUTPUT("/home/sunyujing/new_desktop/c++study/SiPM_analysis_software/SiPM123_20150410/");
    this->SetLabelINPUT("cryostat_dr_pgtrigger_sipm123");
    this->SetLabelOUTPUT("cryostat_dr_pgtrigger_sipm3.root");
}

///////////////////////////////////////////////////////////


TFile* EventIO::NextRun(int biasv, int n) 
{
    std::string inputfile = fDirINPUTName + fFileILabel + std::string(Form("_bias%dV_%d.hdf5.root",biasv,n));
    if(!this->FilesExist(inputfile)){exit(0);}
    fInputFile = new TFile(inputfile.c_str());
    return fInputFile;
}


///////////////////////////////////////////////////////////

TH1F* EventIO::NextEvent(TFile *file, int n) 
{
    fInputHistogram = (TH1F*)file->Get(Form("h%d",n));
    return fInputHistogram;
}

///////////////////////////////////////////////////////////

void EventIO::CloseEvent(TH1F* histogram) 
{
    delete histogram;
}

///////////////////////////////////////////////////////////

void EventIO::CloseRun(TFile* file) 
{
    delete file;
}

///////////////////////////////////////////////////////////
