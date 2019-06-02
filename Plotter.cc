/**
 * \file Plotter.cc
 *
 * \brief Plot the results
 *
 * Original Version: 
 * \author Yujing Sun - UH 2015
 */

#define Plotter_cc

#include "Plotter.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TLine.h>
#include <TText.h>
#include <TFrame.h>
#include <TPaveText.h>

using namespace std;

Plotter::Plotter()
{
    //std::cout << "Plotter is being created" << std::endl;
    this->SetPlotStyle();
    //this->InitialisePars();
}

///////////////////////////////////////////////////////////

Plotter::~Plotter()
{
    //std::cout << "Plotter is being destroyed" << std::endl;
}

///////////////////////////////////////////////////////////

void Plotter::FirstPage(std::string output_bookname, std::string title) {
  
    std::string bookname=output_bookname+".pdf"+"(";
    TCanvas *mycanvas = new TCanvas(Form("Dark Rate Analysis Plots"),"Dark Rate Analysis Plots",1);
    //TText* line;
    mycanvas->UseCurrentStyle();

    TPaveText box(0.1,0.1,0.9,0.9);
    //line = box.AddText("");
    //line = box.AddText(title.c_str());
    box.AddText(title.c_str());
    //line = box.AddText("");
    mycanvas->cd();
    box.Draw();
    mycanvas->Update();
    mycanvas->Print(bookname.c_str());
    mycanvas->Clear();

    return;
}

///////////////////////////////////////////////////////////

void Plotter::LastPage(std::string output_bookname, std::vector<std::string> sVec) {

    std::string bookname=output_bookname + ".pdf" + ")";

    TCanvas *mycanvas = new TCanvas(Form("Dark Rate Analysis Plots"),"Dark Rate Analysis Plots",1);
    
    mycanvas->UseCurrentStyle();
    vector<string>::iterator sIter;
    sIter = sVec.begin();
    TPaveText box(0.1,0.1,0.9,0.9);
    for (uint i=0;i<sVec.size();i++) {
    
        if(sIter != sVec.end()){
	//TText* line;
        //line = box.AddText("");
        //line = box.AddText((*sIter).c_str());
	box.AddText((*sIter).c_str());
        //line = box.AddText("");
        mycanvas->cd();
        box.Draw();
        sIter++;
        }
        else {break;}
    
    }
    mycanvas->Update();
    mycanvas->Print(bookname.c_str());
    mycanvas->Clear();
    return;
}

///////////////////////////////////////////////////////////

void Plotter::TitlePage(std::string output_bookname, std::string title) {

    std::string bookname=output_bookname+".pdf";
  
    TCanvas *mycanvas = new TCanvas(Form("Dark Rate Analysis Plots"),"Dark Rate Analysis Plots",1);
    
    mycanvas->UseCurrentStyle();

    TPaveText box(0.1,0.1,0.9,0.9);
    //TText* line;
    //line = box.AddText("");
    //line = box.AddText(title.c_str());
    box.AddText(title.c_str());
    //line = box.AddText("");
    mycanvas->cd();
    box.Draw();
    mycanvas->Update();
    mycanvas->Print(bookname.c_str());
    mycanvas->Clear();

    return;
}

///////////////////////////////////////////////////////////

void Plotter::MainPage(std::string output_bookname, std::string title, int row, int column, std::vector<TH1*> hVec)
{
  
    std::string bookname=output_bookname+".pdf";
    if(title != "")this->TitlePage(output_bookname,title);
  
    vector<TH1*>::iterator hIter;
    hIter = hVec.begin();

    for (uint page=0;page<hVec.size()/float (column*row);page++) {
      
        TCanvas *mycanvas = new TCanvas(Form("Dark Rate Analysis Plots %d",page+1),"Dark Rate Analysis Plots",1);
        mycanvas->UseCurrentStyle();
        mycanvas->Divide(column,row);

        for (int i=0;i<column*row;i++) {
            if(hIter != hVec.end()){
                mycanvas->cd(i+1);
                (*hIter)->UseCurrentStyle();
                (*hIter)->Draw();
                hIter++;
        }
        else {break;}
        }

        mycanvas->Update();
        mycanvas->Print(bookname.c_str());
        mycanvas->Clear();
    }

    return;
}

///////////////////////////////////////////////////////////

void Plotter::MainPage(std::string output_bookname, TGraphErrors* graph)
{
    std::string bookname=output_bookname+".pdf";
    TCanvas *mycanvas = new TCanvas(Form("Dark Rate Analysis Plots"),"Dark Rate Analysis Plots",1);
    mycanvas->UseCurrentStyle();
    graph->Draw("AL*");
    mycanvas->Update();
    mycanvas->Print(bookname.c_str());
    mycanvas->Clear();

    return;
}

///////////////////////////////////////////////////////////

TStyle* Plotter::SetPlotStyle()
{
    TStyle *CStyle= new TStyle("CStyle","Approved plots style");

    CStyle->SetFrameBorderMode(0);
    CStyle->SetCanvasBorderMode(0);
    CStyle->SetPadBorderMode(0);
    CStyle->SetCanvasColor(0);
    CStyle->SetPadColor(0);
    CStyle->SetTitleFillColor(0);
    CStyle->SetStatColor(0);
    CStyle->SetFillColor(0);

    CStyle->SetPadTopMargin(0.05);
    CStyle->SetPadRightMargin(0.05);
    CStyle->SetPadBottomMargin(0.15);
    CStyle->SetPadLeftMargin(0.2);

    CStyle->SetTitleX(0.5); //title X location
    CStyle->SetTitleY(1); //title Y location
    CStyle->SetTitleW(0.4); //title width
    CStyle->SetTitleH(0.1); //title height
    CStyle->SetStatX(0.95); //title X location
    CStyle->SetStatY(0.98); //title Y location
    CStyle->SetStatW(0.3); //title width
    CStyle->SetStatH(0.2); //title height
    CStyle->SetLabelSize(0.06,"x");
    CStyle->SetTitleSize(0.08,"x");
    CStyle->SetTitleOffset(0.8,"x");
    CStyle->SetLabelSize(0.06,"y");
    CStyle->SetTitleSize(0.08,"y");
    CStyle->SetTitleOffset(0.85,"y");
    CStyle->SetLegendBorderSize(0);

    CStyle->SetCanvasBorderMode(0);
    CStyle->SetCanvasBorderSize(0);

    CStyle->SetFrameBorderMode(0);
    CStyle->SetFrameBorderSize(0);

    CStyle->SetMarkerStyle(20);
    CStyle->SetHistLineWidth(2);
    gROOT->SetStyle("CStyle");
    gStyle->SetOptStat(0000);

    return CStyle;
}