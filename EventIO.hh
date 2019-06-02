/**
 * \file EventIO.hh
 *
 * \brief Data process and access interface
 *
 * Original Version: 
 * \author Yujing Sun - UH 2015
 */
#ifndef EVENTIO_hh
#define EVENTIO_hh

#include <TH1.h>
#include <TFile.h>
#include <string>
#include <cstdlib>

class EventIO {
  
  public:

    EventIO();
    virtual ~EventIO();

    bool                 FilesExist(const std::string name);
    void                 InitialisePars();
    TH1F*                NextEvent(TFile *file, int n);
    TFile*               NextRun(int biasv, int n);

    void                 SetDirDataINPUT(const std::string name)    { fDirINPUTName = name; };                 
    std::string          GetDirDataINPUT()                          { return fDirINPUTName; };                 
    void                 SetDirDataOUTPUT(const std::string name)   { fDirOUTPUTName = name; };                 
    std::string          GetDirDataOUTPUT()                         { return fDirOUTPUTName; };                 

    void                 SetLabelINPUT(const std::string name)      { fFileILabel = name; };                 
    void                 SetLabelOUTPUT(const std::string name)     { fFileOLabel = name; };                 
    std::string          GetLabelINPUT()                            { return fFileILabel; };                 
    std::string          GetLabelOUTPUT()                           { return fFileOLabel; };                 

    void                 SetNumberofEvents(int in=100)              { fNumberofEvents = in; };	        ///default 100
    int                  GetNumberofEvents()                        { return fNumberofEvents; };

    void                 SetSiPMNumber(int in)                      { fSiPMNumber = in; };
    int                  GetSiPMNumber()                            { return fSiPMNumber; };

    void                 SetNumberofRuns(int in=100)                { fNumberofRuns = in; };		///default 100
    int                  GetNumberofRuns()                          { return fNumberofRuns; };

    void                 SetTimeWindow(int in=2000000)              { fTimeWindow = in; };		///default 2000000 ns
    int                  GetTimeWindow()                            { return fTimeWindow; };

    void                 SetThreshold (float in=0.35)               { fThreshold = in; };		///default 0.35 mv
    float                GetThreshold ()                            { return fThreshold; };

    void                 SetBiasVStart(int in=23)                   { fBiasVStart = in; };		///default 23 V
    int                  GetBiasVStart()                            { return fBiasVStart; };

    void                 SetBiasVEnd(int in=26)                     { fBiasVEnd = in; };		///default 26 V
    int                  GetBiasVEnd()                              { return fBiasVEnd; };

    TFile*               GetINPUTFile()                             { return fInputFile; };
    TH1F*                GetINPUTHistogram()                        { return fInputHistogram; };
    void                 CloseEvent(TH1F* histogram);
    void                 CloseRun(TFile* file);

  private:
    
    int                  fSiPMNumber;
    int                  fNumberofEvents;
    int                  fNumberofRuns;
    int                  fTimeWindow;
    float                fThreshold;
    int                  fBiasVStart;
    int                  fBiasVEnd;

    std::string          fFileOLabel;                    ///< Output file name label
    std::string          fFileILabel;                    ///< Input file name label;
    std::string          fDirOUTPUTName;                 ///< Output dir name
    std::string          fDirINPUTName;                  ///< Output dir name

    TH1F*                fInputHistogram;
    TFile*               fInputFile;
  //ClassDef(EnDep,1)
};

#endif