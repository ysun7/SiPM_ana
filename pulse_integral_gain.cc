#include <algorithm>
#include <stdio.h>
#include <math.h>
void pulse_integral_gain()
{
int i,j,k,n,m;
double gain[6],gain_e[6],sigma[6],sigma_e[6];

 
gROOT->SetStyle("C22");
gStyle->SetOptStat(0000); 
gStyle->SetTitleY(0.85);
gStyle->SetTitleX(0.65);
gStyle->SetTitleBorderSize(0);
TCanvas *myc = new TCanvas("myc","histograms",0,0,1700,1700/1.61803398875);
myc->UseCurrentStyle();
myc->Divide(2,2);

for (i=0;i<4;i++)
{
TFile *g1 = new TFile(Form("cryostat_dr_pgtrigger_sipm3_test7.root"));
TH1F *h11 = (TH1F*)g1->Get(Form("integral_%dV",i+23));

double spe_peak = h11->GetMaximumBin()/100.;
double spe_peak_width=0.1;

h11->UseCurrentStyle();
h11->SetTitle(Form("V_{bias}=%d;Pulse height (mV);Entries",i+23));
myc->cd(i+1);
h11->Draw();


TF1 *f1 = new TF1("f1","[3]*exp(-(x-[0]-[2])*(x-[0]-[2])/(2*[1]*[1]))+[4]*exp(-(x-[0]-[2]*2)*(x-[0]-[2]*2)/(2*[1]*[1]))+[5]*exp(-(x-[0]-[2]*3)*(x-[0]-[2]*3)/(2*[1]*[1]))",spe_peak-spe_peak_width,spe_peak*3+spe_peak_width);
double par[6]={0.0,spe_peak_width,spe_peak,h11->GetMaximum(),h11->GetMaximum()*0.2,h11->GetMaximum()*0.05};

f1->SetParLimits(0,-0.03,0.03);
f1->SetParLimits(1,0.0,0.1);
f1->SetParLimits(2,spe_peak-spe_peak_width,spe_peak+spe_peak_width);
f1->SetParLimits(3,0,h11->GetMaximum()*1.5);
f1->SetParLimits(4,0,h11->GetMaximum()*0.5);
f1->SetParLimits(5,0,h11->GetMaximum()*0.1);
f1->SetParameters(par);
f1->SetLineColor(2);

h11->Fit("f1","R");
gain[i]=f1->GetParameter(2);
gain_e[i]=f1->GetParError(2);
sigma[i]=f1->GetParameter(1);
sigma_e[i]=f1->GetParError(1);
h11->SetAxisRange(0,2);
}
for (i=0;i<6;i++){cout<<gain[i]<<", ";}cout<<endl;
for (i=0;i<6;i++){cout<<gain_e[i]<<", ";}cout<<endl;
/*
double spe[6],dpe[6];
int spe_start[3]={5,5,5},spe_end[3]={22,30,35},dpe_start[3]={23,30,35},dpe_end[3]={35,45,50};
for (i=0;i<2;i++)
{
  TFile *g1 = new TFile(Form("pulse_cryostat_dr_pgtrigger_sipm1_%dV_threshold0p35mv_dataset5.root",i+23));
TH1F *h11 = (TH1F*)g1->Get(Form("integral_h%d",i));
 spe[i]=0;dpe[i]=0 ;
  for (j=spe_start[i];j<spe_end[i];j++)
  {
    spe[i]+=h11->GetBinContent(j+1);
  }
  for (j=dpe_start[i];j<dpe_end[i];j++)
  {
    dpe[i]+=h11->GetBinContent(j+1);
  }  
 cout<<dpe[i]/spe[i]<<endl;
 cout<<gain[i]-5*sigma[i]<<" "<<gain[i]+5*sigma[i]<<"          "<<2*gain[i]-5*sigma[i]<<" "<<2*gain[i]+5*sigma[i]<<endl;
}

//for (i=0;i<3;i++){cout<<dpe[i]<<", ";}cout<<endl;
//for (i=0;i<3;i++){cout<<spe[i]<<", ";}cout<<endl;


for (i=2;i<6;i++)
{
  TFile *g1 = new TFile(Form("pulse_cryostat_dr_pgtrigger_sipm1_%dV_threshold0p35mv_dataset5.root",i+23));
TH1F *h11 = (TH1F*)g1->Get(Form("integral_h%d",i));
 spe[i]=0;dpe[i]=0 ;
  for (j=floor(100*(gain[i]-5*sigma[i]));j<floor(100*(gain[i]+5*sigma[i]));j++)
  {
    spe[i]+=h11->GetBinContent(j+1);
  }
  for (j=floor(100*(2*gain[i]-5*sigma[i]));j<floor(100*(2*gain[i]+5*sigma[i]));j++)
  {
    dpe[i]+=h11->GetBinContent(j+1);
  }  
 cout<<dpe[i]/spe[i]<<endl;
 cout<<gain[i]-5*sigma[i]<<" "<<gain[i]+5*sigma[i]<<"          "<<2*gain[i]-5*sigma[i]<<" "<<2*gain[i]+5*sigma[i]<<endl;
}
*/



}

