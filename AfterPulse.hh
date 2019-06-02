/**
 * \file AfterPulse.hh
 *
 * \brief Analysing After Pulsing, Gain and Cross Talk
 *
 * Original Version: 
 * @author Yujing Sun - UH 2015
 */

#ifndef AfterPulse_hh
#define AfterPulse_hh

#include "EventIO.hh"
#include "Plotter.hh"

#include <string>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFile.h>

class AfterPulse: public EventIO {
  
  public:
    
    AfterPulse();
    virtual ~AfterPulse();
    
    void Analysis();
    double PulseIntegral(TH1F *histogram, double baseline, int peakbin);
    void AfterPulseSearch(TH1F *histogram, int biasv, int event, int run);
    double Baseline(TH1F *histogram, int baseline_start, int baseline_end);
    int PeakBin(TH1F *histogram, int peaksearch_start, int peaksearch_end);
    void WriteOut(TFile *file, TH1F *histogram);
    void Initialise();
    void FitGain(TH1F *histogram, int biasv);
    void Skip(int biasv, int run, int event);
    
    void   SetTriggerTime(int in=-9880)              { fTriggerTime = in; };		///default -9880 ns for after pulse dedicated data
    int    GetTriggerTime()                          { return fTriggerTime; };
    
    void   SetSearchStartTime(int in=-9780)          { fSearchStartTime = in; };	///default -9780 ns for Hamamatsu MPPCs
    int    GetSearchStartTime()                      { return fSearchStartTime; };
    
    void   SetSampleRate(int in=20)                  { fSampleRate = in; };		///default 20 Samples/ns for after pulse dedicated data
    int    GetSampleRate()                           { return fSampleRate; };
  private:
    
    Plotter*              fPlotter;

    TH1F*                 fMainPulseHeight_h[10];
    TH1F*                 fMainPulseIntegral_h[10];
    TH1F*                 fAfterPulseHeight_h[10];
    TH1F*                 fAfterPulseIntegral_h[10];
    TH1F*                 fAfterPulseTiming_h[10];
    TGraphErrors*         fGain_ge;
    TGraphErrors*         fAfterPulse_ge;
    TGraphErrors*         fCrossTalk_ge;

    TFile*                fOutput_f;
    std::string           fOutputFileName;
    
    int                   fCounter;
    int                   fTriggerTime;
    int                   fSearchStartTime;
    int                   fSampleRate;
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
    std::vector<double>   fAfterPulse;
    std::vector<double>   fAfterPulse_e;
};

#endif