/**
 * \file Plotter.cc
 *
 * \brief Plot the results
 *
 * Original Version: 
 * \author Yujing Sun - UH 2015
 */

#ifndef PLOTTER_hh
#define PLOTTER_hh

#include <TH1.h>
#include <TFile.h>
#include <string>
#include <cstdlib>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TStyle.h>

class Plotter {

  public:
  
    Plotter();
    virtual ~Plotter();

    void FirstPage(std::string output_bookname, std::string title);
    void TitlePage(std::string output_bookname, std::string title);
    void MainPage(std::string output_bookname, std::string title, int row, int column, std::vector<TH1*> hVec);
    void MainPage(std::string output_bookname, TGraphErrors* graph);
    void LastPage(std::string output_bookname, std::vector<std::string> sVec);
    TStyle* SetPlotStyle();
    
  private:

};

#endif