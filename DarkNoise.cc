/**
 * \file DarkNoise.cc
 *
 * \brief Analysing Gain, Dark Rate and Cross Talk
 *
 * Original Version: 
 * \author Yujing Sun - UH 2015
 */

#define DarkNoise_cc

#include "DarkNoise.hh"

#include <string>
#include <iostream>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>

using namespace std;

//#########################################################################################################################################  

DarkNoise::DarkNoise()
{
    //std::cout << "Dark rate analysis is started!" << std::endl;
}

//#########################################################################################################################################  

DarkNoise::~DarkNoise()
{
    if (fPlotter){ delete fPlotter;}
    //std::cout << "Dark rate analysis is finished!" << std::endl;
}

//#########################################################################################################################################  

void DarkNoise::Initialise()
{
//std::cout << "Initialsing in DarkRate" << std::endl;

    for (int biasv=this->GetBiasVStart();biasv<=this->GetBiasVEnd();biasv++) {
        fPulseHeight_h[biasv-this->GetBiasVStart()] = new TH1F(Form("pulse_height_%dV",biasv),Form("V_{bias} = %d V;Pulse height (mV);Entries / [0.01 mV]",biasv),1500,0,15);
        fIntegral_h[biasv-this->GetBiasVStart()] = new TH1F(Form("integral_%dV",biasv),Form("V_{bias} = %d V;Pulse integral;Entries",biasv),700,0,7);
    }
    
    fOutputFileName = this->GetDirDataOUTPUT() + this->GetLabelOUTPUT();  
    fOutput_f = new TFile((fOutputFileName + ".root").c_str(),"new");
    
    fPlotter = new Plotter();
    
    int nskip[this->GetBiasVEnd()-this->GetBiasVStart()+1]={0};
    
    for (uint skip=0;skip<fSkipRun.size();skip++) {
        nskip[fSkipBiasV.at(skip)-this->GetBiasVStart()]++;
    }
    
    for (int i=0;i<this->GetBiasVEnd()-this->GetBiasVStart()+1;i++) {
        fNSkip.push_back(nskip[i]);
    }
    
}

//#########################################################################################################################################  

void DarkNoise::Analysis() {
    //std::cout << "Dark rate analysis starts!" << std::endl;
    
    this->Initialise();
    std::string title = "Dark Noise Analysis";
    fPlotter->FirstPage(fOutputFileName, title);
    
    for (int biasv=this->GetBiasVStart();biasv<=this->GetBiasVEnd();biasv++){
      
        fCounter=0;
        fBiasV.push_back(biasv);
        fBiasV_e.push_back(0);
        std::cout<<biasv<<" V (bias)"<<std::endl;
 
        for (int run=0;run<this->GetNumberofRuns();run++) {
            
	    this->NextRun(biasv,run);

            for (int event=1;event<this->GetNumberofEvents()+1;event++) {
                
	        this->NextEvent(this->GetINPUTFile(),event);
                this->PulseSearch(this->GetINPUTHistogram(),biasv, event, run);
                this->CloseEvent(this->GetINPUTHistogram());
    //delete h;
            }
            this->CloseRun(this->GetINPUTFile());
        }
        
        this->FitGain(fIntegral_h[biasv-this->GetBiasVStart()],biasv);
        this->WriteOut(fOutput_f,fPulseHeight_h[biasv-this->GetBiasVStart()]);
        this->WriteOut(fOutput_f,fIntegral_h[biasv-this->GetBiasVStart()]);
        std::cout<<std::endl;
	std::cout<<fCounter << " dark noise found!"<<std::endl;
	std::cout<<std::endl;
        vector<TH1*> hVec;
	
        for (int i=0;i<fCounter;i++) {
	  
            TH1F *plot_h = (TH1F*)fOutput_f->Get(Form("%dVh%d",biasv,i));
            hVec.push_back(plot_h);
        }
        
        title=Form("Selected Pulses Under Biasv = %d V",biasv);
        fPlotter->MainPage(fOutputFileName, title, 3, 5, hVec);
    }

    title="";
    std::vector<TH1*> hVec1(fPulseHeight_h, fPulseHeight_h+this->GetBiasVEnd()-this->GetBiasVStart()+1);
    std::vector<TH1*> hVec2(fIntegral_h, fIntegral_h+this->GetBiasVEnd()-this->GetBiasVStart()+1);


    switch((this->GetBiasVEnd()-this->GetBiasVStart()+1)) {
        case 1 : fPlotter->MainPage(fOutputFileName, title, 1, 1, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 1, 1, hVec2);
                 break;
        case 2 : fPlotter->MainPage(fOutputFileName, title, 1, 2, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 1, 2, hVec2);
                 break;
        case 3 : fPlotter->MainPage(fOutputFileName, title, 3, 1, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 3, 1, hVec2);
                 break;
        case 4 : fPlotter->MainPage(fOutputFileName, title, 2, 2, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 2, 2, hVec2);
                 break;
        case 5 : fPlotter->MainPage(fOutputFileName, title, 2, 3, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 2, 3, hVec2);
                 break;
        case 6 : fPlotter->MainPage(fOutputFileName, title, 2, 3, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 2, 3, hVec2);
                 break;
        case 7 : fPlotter->MainPage(fOutputFileName, title, 2, 4, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 2, 4, hVec2);
                 break;
        case 8 : fPlotter->MainPage(fOutputFileName, title, 2, 4, hVec1);
                 fPlotter->MainPage(fOutputFileName, title, 2, 4, hVec2);
                 break;
    }


    fGain_ge = new TGraphErrors(this->GetBiasVEnd()-this->GetBiasVStart()+1, &fBiasV[0], &fGain[0], &fBiasV_e[0], &fGain_e[0]);
    fGain_ge->SetTitle(";BiasV (V);Gain");
    fPlotter->MainPage(fOutputFileName, fGain_ge);

    fDarkRate_ge = new TGraphErrors(this->GetBiasVEnd()-this->GetBiasVStart()+1, &fBiasV[0], &fDarkRate[0], &fBiasV_e[0], &fDarkRate_e[0]);
    fDarkRate_ge->SetTitle(";BiasV (V);Dark Rate (Hz)");
    fPlotter->MainPage(fOutputFileName, fDarkRate_ge);

    fCrossTalk_ge = new TGraphErrors(this->GetBiasVEnd()-this->GetBiasVStart()+1, &fBiasV[0], &fCrossTalk[0], &fBiasV_e[0], &fCrossTalk_e[0]);
    fCrossTalk_ge->SetTitle(";BiasV (V);Cross Talk (100%)");
    fPlotter->MainPage(fOutputFileName, fCrossTalk_ge);

    std::vector<std::string> sVec;
    std::string par0_str = Form("SiPM %d Input Parameters:", this->GetSiPMNumber());
    sVec.push_back(par0_str);
    std::string par1_str = Form("%d runs", this->GetNumberofRuns());
    sVec.push_back(par1_str);
    std::string par2_str = Form("%d events in each run", this->GetNumberofEvents());
    sVec.push_back(par2_str);
    std::string par3_str = Form("time window = %d ns", this->GetTimeWindow());
    sVec.push_back(par3_str);
    std::string par4_str = Form("threshold = %0.2f mV", this->GetThreshold());
    sVec.push_back(par4_str);
    std::string par5_str = Form("BiasV from %d to %d V", this->GetBiasVStart(), this->GetBiasVEnd());
    sVec.push_back(par5_str);
    std::string par6_str = "input data dir: " + this->GetDirDataINPUT();
    sVec.push_back(par6_str);
    std::string par7_str = "input data label: " + this->GetLabelOUTPUT();
    sVec.push_back(par7_str);
    std::string par8_str = "output data dir: " + this->GetDirDataOUTPUT();
    sVec.push_back(par8_str);
    std::string par9_str = "output data label: " + this->GetLabelOUTPUT();
    sVec.push_back(par9_str);
    std::string par10_str = Form("skip %d events",int (fSkipBiasV.size()));
    sVec.push_back(par10_str);
    
    for(uint i=0;i<fSkipBiasV.size();i++) {
	std::string par10_str = Form("skip -> (%dV, run %d, event %d) ",fSkipBiasV.at(i),fSkipRun.at(i),fSkipEvent.at(i));
        sVec.push_back(par10_str);
    }
    
    fPlotter->LastPage(fOutputFileName, sVec);  
    fOutput_f->Close();
    delete fOutput_f;
}

//#########################################################################################################################################

void DarkNoise::WriteOut(TFile *file, TH1F *histogram)
{
    file->cd();
    histogram->Write();
}

//#########################################################################################################################################

void DarkNoise::Skip(int biasv, int run, int event)
{
    fSkipBiasV.push_back(biasv);
    fSkipRun.push_back(run);
    fSkipEvent.push_back(event);
    return;
}

//#########################################################################################################################################
  
void DarkNoise::PulseSearch(TH1F *h,int biasv, int event, int run)
{
  /*
    vector<int>::iterator run_iter;
    run_iter = fSkipRun.begin();
    vector<int>::iterator event_iter;
    event_iter = fSkipEvent.begin();
    */
    for (uint skip=0;skip<fSkipRun.size();skip++)
    {
        if(biasv==fSkipBiasV.at(skip) && run==fSkipRun.at(skip) && event==fSkipEvent.at(skip)) {return;}
    }
    //std::cout<<run<<" "<<event<<std::endl;
    int baseline_start,baseline_end,min;
    double baseline,counter1;
    
    for (int bin=0;bin<this->GetTimeWindow();bin++)
    {
        //std::cout << "Dark rate analysis begins 3!" << std::endl;
        if(((h->GetBinContent(bin+1))*1000)>this->GetThreshold())
        {
	    if(bin<1000){fSelectedPulse_h = new TH1F(Form("%dVh%d",biasv,fCounter),Form("%dVrun%devent%d;Time (ns);Voltage (mV)",biasv,run,event),2000,-1000000,-998000);min=-1000000;baseline_start=-1000000;baseline_end=bin-200-1000000;}
	    else if(bin>1999000){fSelectedPulse_h = new TH1F(Form("%dVh%d",biasv,fCounter),Form("%dVrun%devent%d;Time (ns);Voltage (mV)",biasv,run,event),2000,998000,1000000);min=998000;baseline_start=bin-1000-1000000;baseline_end=bin-200-1000000;}
	    else {fSelectedPulse_h = new TH1F(Form("%dVh%d",biasv,fCounter),Form("%dVrun%devent%d;Time (ns);Voltage (mV)",biasv,run,event),2000,bin-1000-1000000,bin+1000-1000000);min=bin-1000-1000000;baseline_start=bin-1000-1000000;baseline_end=bin-200-1000000;}
	
	    for (int k=min;k<min+2000;k++) {
	         fSelectedPulse_h->Fill(k,h->GetBinContent(k+1+1000000));
	    }
	
	    baseline=0;
	    counter1=0;
	
	    for (int k=baseline_start;k<baseline_end;k++) {
	        if((1000*h->GetBinContent(k+1+1000000))<this->GetThreshold() && (1000*h->GetBinContent(k+1+1000000))>-this->GetThreshold()) {
	        baseline+=h->GetBinContent(k+1+1000000);counter1++;
	        }
	    }
	  
	    if(counter1>0){baseline/=counter1;}
	    else{cout<<"Background noise is high! Jump 1 #mus!"<<endl;bin+=1000;continue;}
	
	    if(((h->GetBinContent(bin+1)-baseline)*1000)>this->GetThreshold()) {
	  
	        this->WriteOut(fOutput_f,fSelectedPulse_h);
	        fIntegral_h[biasv-this->GetBiasVStart()]->Fill(this->PulseIntegral(fSelectedPulse_h,baseline));
	        fPulseHeight_h[biasv-this->GetBiasVStart()]->Fill((fSelectedPulse_h->GetBinContent(fSelectedPulse_h->GetMaximumBin())-baseline)*1000);
	        fCounter++;
	        bin+=1000;
	    }
	delete fSelectedPulse_h;
	
        }
    }
    return;
}

//#########################################################################################################################################  

double DarkNoise::PulseIntegral(TH1F *histogram, double baseline)
{
    double integral=0;
  
    for (int k=histogram->GetMaximumBin();k<2000;k++) {
      
        if(histogram->GetBinContent(k+1)>baseline) {
            integral+=(histogram->GetBinContent(k+1)-baseline);
        }
        
        else {
            break; 
        }
    }
    
    for (int k=histogram->GetMaximumBin();k>0;k--) {
   
        if(histogram->GetBinContent(k+1)>baseline) {
            integral+=(histogram->GetBinContent(k+1)-baseline);
        }
        else {
            break; 
        }
    }
  
  return integral; 
}


//#########################################################################################################################################  

void DarkNoise::FitGain(TH1F *histogram, int biasv)
{
    double spe_peak = histogram->GetMaximumBin()/100.;
    double spe_peak_width=0.1;
    
    double par[6]={0.0,spe_peak_width,spe_peak,histogram->GetMaximum(),histogram->GetMaximum()*0.2,histogram->GetMaximum()*0.05};
    TF1 *f1;
    if(biasv-this->GetBiasVStart()==0) f1 = new TF1("f1","[3]*exp(-(x-[0]-[2])*(x-[0]-[2])/(2*[1]*[1]))+[4]*exp(-(x-[0]-[2]*2)*(x-[0]-[2]*2)/(2*[1]*[1]))",spe_peak-spe_peak_width,spe_peak*2+2*spe_peak_width);
    else f1 = new TF1("f1","[3]*exp(-(x-[0]-[2])*(x-[0]-[2])/(2*[1]*[1]))+[4]*exp(-(x-[0]-[2]*2)*(x-[0]-[2]*2)/(2*[1]*[1]))+[5]*exp(-(x-[0]-[2]*3)*(x-[0]-[2]*3)/(2*[1]*[1]))",spe_peak-spe_peak_width,spe_peak*3+3*spe_peak_width);
    f1->SetParLimits(0,-0.05,0.05);
    f1->SetParLimits(1,0.0,0.05);
    f1->SetParLimits(2,spe_peak-0.3*spe_peak_width,spe_peak+0.3*spe_peak_width);
    f1->SetParLimits(3,histogram->GetMaximum()*0.8,histogram->GetMaximum()*1.1);
    f1->SetParLimits(4,0,histogram->GetMaximum()*0.3);
    if(biasv-this->GetBiasVStart()!=0)f1->SetParLimits(5,0,histogram->GetMaximum()*0.1);
    f1->SetParameters(par);
    
    f1->SetLineColor(2);
    histogram->Fit("f1","R");
    double factor=47.62*15*1.6e-10;

    fGain.push_back(f1->GetParameter(2)/factor);
    fGain_e.push_back(f1->GetParError(2)/factor);
 
    double npulse=0,nmpulse=0;

    for (int i=0;i<f1->GetParameter(2)*0.5*100;i++) {
        npulse+=histogram->GetBinContent(i+1);
    }
  
    for (int i=0;i<f1->GetParameter(2)*1.5*100;i++) {
        nmpulse+=histogram->GetBinContent(i+1);
    }
  
    double ct=(histogram->GetEntries()-nmpulse)/(histogram->GetEntries()-npulse);
    double ct_e=ct*sqrt(1/(histogram->GetEntries()-nmpulse)+1/(histogram->GetEntries()-npulse));
  
    double dr=(histogram->GetEntries()-npulse)/((double (this->GetNumberofRuns()) * double(this->GetNumberofEvents()) - fNSkip.at(biasv-this->GetBiasVStart())) * double(this->GetTimeWindow())  * 1e-9);
    //std::cout<<double (this->GetNumberofRuns()) * double(this->GetNumberofEvents()) - fNSkip.at(biasv-this->GetBiasVStart())<<std::endl;
    double dr_e=sqrt(dr)/(double (this->GetNumberofRuns()) * double(this->GetNumberofEvents()) * double(this->GetTimeWindow()) * 1e-9);
    fDarkRate.push_back(dr);
    fDarkRate_e.push_back(dr_e);
  
  
    fCrossTalk.push_back(ct);
    fCrossTalk_e.push_back(ct_e);  
    return; 
}

//#########################################################################################################################################  
