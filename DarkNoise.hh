/**
 * \file DarkNoise.hh
 *
 * \brief Analysing Gain, Dark Rate and Cross Talk
 *
 * Original Version: 
 * @author Yujing Sun - UH 2015
 */

#ifndef DarkNoise_hh
#define DarkNoise_hh

#include "EventIO.hh"
#include "Plotter.hh"

#include <string>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFile.h>

class DarkNoise: public EventIO {
  
  public:
    
    DarkNoise();
    virtual ~DarkNoise();
    
    void Analysis();
    double PulseIntegral(TH1F *histogram, double baseline);
    void PulseSearch(TH1F *histogram, int biasv, int event, int run);
    void WriteOut(TFile *file, TH1F *histogram);
    void Initialise();
    void FitGain(TH1F *histogram, int biasv);
    void Skip(int biasv, int run, int event);

  private:
    
    Plotter*              fPlotter;
    TH1F*                 fSelectedPulse_h;
    TH1F*                 fPulseHeight_h[10];
    TH1F*                 fIntegral_h[10];
    TGraphErrors*         fGain_ge;
    TGraphErrors*         fDarkRate_ge;
    TGraphErrors*         fCrossTalk_ge;

    TFile*                fOutput_f;
    std::string           fOutputFileName;
    
    int                   fCounter;
    std::vector<int>      fSkipRun;
    std::vector<int>      fSkipEvent;
    std::vector<int>      fSkipBiasV;
    std::vector<int>      fNSkip;
    std::vector<double>   fBiasV;
    std::vector<double>   fBiasV_e;
    std::vector<double>   fGain;
    std::vector<double>   fGain_e;
    std::vector<double>   fCrossTalk;
    std::vector<double>   fCrossTalk_e;
    std::vector<double>   fDarkRate;
    std::vector<double>   fDarkRate_e;
};

#endif