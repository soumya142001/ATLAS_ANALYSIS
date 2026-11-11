#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TF1.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <math.h>

using std::cout;
using std::endl;

void decorate(TH1*h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill);
void decorate(TLegend *g, float textSize, TString legendheader);
float get_nevents(TH1F *hst, float bin_lo, float bin_hi);
float get_nevents_err(TH1F *hst, float bin_lo, float bin_hi);

void histo_draw()
{
  TString filename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/ZZ_QCD.root";
  TString plotname = "M_jj_cut3;1";
  TString name = "M_{jj}";

  TString xtitle = "#bf{Number of Leptons}";
  TString ytitle = "#bf{Probability}";

  TFile *file = new TFile(filename);
  

  TH1F *h = (TH1F*)file->Get(plotname);
  TH1F *hBand = (TH1F*)h->Clone("hBand");
  //Rebinning the histogram if required
  h->Rebin(20);
  hBand->Rebin(20);
  //Normalisation
  //h->Scale(1.0/h->Integral());
  
  //Decorate
  decorate(h,xtitle,ytitle,"",kRed+2,1,kRed+2,20,1);
  
  TCanvas *c1 = new TCanvas("canvas","canvas",800,600);

  //Histogram properties
  h->GetXaxis()->SetRangeUser(0,3000);
  h->GetYaxis()->SetRangeUser(0,40);
  h->SetStats(0);
  h->SetFillStyle(1001);

  //For Error Band on top of original histogram
  hBand->SetFillColorAlpha(kGray, 0.35);
  hBand->SetFillStyle(3004);
  hBand->SetLineWidth(0);
  hBand->SetMarkerSize(0);
  
  h->Draw("HIST");
  hBand->Draw("E2 SAME");
  //h->Draw("HIST SAME");
  
  //Adding Text to the histograms
  TLatex txt;
  txt.SetNDC(true);
  txt.SetTextFont(42);
  txt.SetTextSize(0.03);
  txt.DrawLatex(0.1,0.92,"#it{#bf{ATLAS Simulation: Work in Progress}}");

  //Adding the legends
  TLegend *lg1 = new TLegend(0.55,0.50,0.85,0.75,NULL,"NDC");
  decorate(lg1,0.05,"");
  lg1->Draw();
  

  TString variable = plotname;
  //c1->SaveAs("Plots/"+name+".png");
  // c1->SaveAs("/home/soumya/CMSSW_files/NTuples/Analyzer/Plots/var_plots/"+variable+".png");
}


void decorate(TH1*h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill) {

  h->GetXaxis()->SetTitle(xtitle);
  h->GetYaxis()->SetTitle(ytitle);
  h->GetXaxis()->SetTitleSize(0.04);
  h->GetYaxis()->SetTitleSize(0.04);
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->CenterTitle(true);
  h->GetXaxis()->CenterTitle(true);
  h->GetYaxis()->CenterTitle(true);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1);

  h->SetLineColor(linecolor); 
  h->SetLineWidth(linewidth);
  h->SetMarkerColor(markercolor);
  h->SetMarkerStyle(markerstyle);
  if(tofill==1) h->SetFillColor(markercolor);

  h->SetMarkerSize(0.25);
  h->SetTitle(title);
}


void decorate(TLegend *g, float textSize, TString legendheader)
{
  g->SetTextSize(textSize);
  g->SetFillStyle(1);
  g->SetFillColor(10);
  g->SetBorderSize(1);
  g->SetLineColor(10);
  //Usually legends should not have headers
  //but if you want one, uncomment the next line.
  //g->SetHeader(legendheader);
}


// For a given histogram hst, return the number of entries between bin_lo and bin_hi
float get_nevents(TH1F *hst, float bin_lo, float bin_hi)
{
  int bin_width = hst->GetBinWidth(1);
  int ibin_begin = 1;
  float nevents = 0;
  while ( hst->GetBinCenter(ibin_begin) < bin_lo )
    ibin_begin++;
  int ibin_end = ibin_begin;
  while ( hst->GetBinCenter(ibin_end) < bin_hi )
    ibin_end++;
  for(int i=ibin_begin; i<ibin_end; i++)
    nevents += hst->GetBinContent(i);

  return nevents;
}

// Partner function for above, returning the error for the above nevents
float get_nevents_err(TH1F *hst, float bin_lo, float bin_hi)
{
  int bin_width = hst->GetBinWidth(1);
  int ibin_begin = 1;
  float nevents_err = 0;
  while ( hst->GetBinCenter(ibin_begin) < bin_lo )
    ibin_begin++;
  int ibin_end = ibin_begin;
  while ( hst->GetBinCenter(ibin_end) < bin_hi )
    ibin_end++;
  for(int i=ibin_begin; i<ibin_end; i++)
    nevents_err += pow(hst->GetBinError(i),2);
  nevents_err = sqrt(nevents_err);

  return nevents_err;
}
