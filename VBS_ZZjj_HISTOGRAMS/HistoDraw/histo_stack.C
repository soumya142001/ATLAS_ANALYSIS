#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TF1.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <TH1D.h>

using std::cout;
using std::endl;

/* ===================================

This is the regular overlay script example for beginners.
Author: Soumya Sarkar
Usage : At the root prompt
    [].x overlay.C

This file is modified to work in Root6.

The primary function in this file is overlay().

   =================================== */



// Declare all the other functions needed. See the function implementations for more
// comments about what the functions do.
void decorate(TH1F *h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill);
void decorate(TLegend *g, float textSize, TString legendheader);
float get_nevents(TH1D *hst, float bin_lo, float bin_hi);
float get_nevents_err(TH1D *hst, float bin_lo, float bin_hi);


void histo_stack()
{

  //Defining the Parameters of the Plot
  double XMIN = -4.5;
  double XMAX = 4.5;
  double YMIN = 0;
  double YMAX = 40;
  TString REGION = "QCDCR";
  double binwidth = 0.3;
  TString variable = "Y_Z2";
  bool C_scale = false;
  //bool C_scale = true;
  
  TString xtitle = "Y_{Z2}";
  TString ytitle = "Events /  0.3 "; 

  //To include C-factor scaling for the SR
  TString scalename;
  if(C_scale) scalename="Scaled";
  else scalename="";
  
  //Declare the name of the plot that you want to overlay
  TString plotname;
  if (REGION.CompareTo("SR") == 0)    plotname = variable + "_cut3;1";
  if (REGION.CompareTo("QCDCR") == 0) plotname = variable + "_QCDCR;1";     
        
  //Define the THstack
  THStack *hstack = new THStack("hstack", ""); 
  
  //Declare number of files
  const int nfile = 3;
  //int colors[nfile] = {kYellow+1,kOrange+7,kGreen+2,kRed+1,kPink+7,kCyan+2,kBlue+1};
  int colors[nfile] = {kRed+1,kPink+7,kBlue+1};
  //int colors[nfile] = {kRed+1,kBlue+1,kGreen+2,kMagenta+1,kOrange+7,kCyan+2,kBlack,kViolet+1, kSpring+5, kPink+7};
  //int colors[nfile] = {kRed+1,kBlue+1,kGreen+2,kMagenta+1,kOrange+7,kCyan+1,kBlack};
  
  //First declare the file names (just strings)
  TString filename[nfile];
  TString samplename[nfile];
  
  //filename[0] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/VBS_ZZjjEW_TT_xsweighted.root";
  //filename[1] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/VBS_ZZjjEW_TL_xsweighted.root";
  //filename[2] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/VBS_ZZjjEW_LL_xsweighted.root";
  //filename[3] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/VBS_ZZjj_TT_xsweighted.root";
  //filename[4] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/VBS_ZZjj_TL_xsweighted.root";
  //filename[5] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/VBS_ZZjj_LL_xsweighted.root";
  filename[0] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/VBS_EW_ZZjj.root";
  filename[1] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/ggllll_130M4l.root";
  //filename[2] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/gg4l_m4l_70_3000.root";
  filename[2] = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/ZZ_QCD.root";
  
  //samplename[0] = "VBS_ZZjjEW_TT_xsweighted";
  //samplename[1] = "VBS_ZZjjEW_TL_xsweighted";
  //samplename[2] = "VBS_ZZjjEW_LL_xsweighted";
  //samplename[3] = "VBS_ZZjj_TT_xsweighted";
  //samplename[4] = "VBS_ZZjj_TL_xsweighted";
  //samplename[5] = "VBS_ZZjj_LL_xsweighted";
  samplename[0] = "VBS_EW_ZZjj";
  samplename[1] = "ggllll_M4l130";
  //samplename[2] = "gg4l_M4l_70_3000";
  samplename[2] = "ZZ_QCD";

  double Cfactor[nfile] = {0.663,0.702,0.741};

  //Now let us open the files
  TFile *file[nfile];
  for(int i=0;i<nfile;i++){
    file[i] = new TFile(filename[i]);
  }
  
  //Now open the respective histograms from the files
  TH1F *h[nfile];

  for(int i=0;i<nfile;i++){
    h[i] = (TH1F*)file[i]->Get(plotname);
    double binw = h[i]->GetBinWidth(1);
    int nrebins = static_cast<int>(binwidth/binw);
    h[i]->Rebin(nrebins);
    
    if (C_scale) h[i]->Scale(Cfactor[i]);
  }

  //Defining an Error histogram

  TH1F *herr = (TH1F*)h[0]->Clone("herr");
  herr->Reset();

  //Creating the total histogram with the total error

  int nBins = h[0]->GetNbinsX();

  for( int b = 0; b <= nBins; b++){
    double totalContent = 0.0;
    double totalErr2     = 0.0;

    for( int i = 0; i< nfile; i++){
      double content = h[i]->GetBinContent(b);
      double error   = h[i]->GetBinError(b);

      totalContent += content;
      totalErr2    += error*error;
    }

    double totalErr = sqrt(totalErr2);

    herr->SetBinContent(b, totalContent);
    herr->SetBinError(b, totalErr);
  }

  /********** Checking the Assymetry/Symmetry of histograms************/

  for(int i=0; i < nfile; i++){
    double xval = 0;
    int bin = h[i]->FindBin(xval);
    int nbins = h[i]->GetNbinsX();
    double n_less = h[i]->Integral(0,bin-1);
    double n_more = h[i]->Integral(bin,nbins);
    double err_less = -1;
    double err_more = -1;
    double sum_err2_less = 0;
    double sum_err2_more = 0;
    //Finding the Errors on each side too
    for( int b=0; b < bin; b++){
      double err = h[i]->GetBinError(b);
      sum_err2_less = sum_err2_less + err*err;
    }
    
    for( int b=bin; b < nbins; b++){
      double err = h[i]->GetBinError(b);
      sum_err2_more = sum_err2_more + err*err;
    }

    err_less = TMath::Sqrt(sum_err2_less);
    err_more = TMath::Sqrt(sum_err2_more);

    std::cout<<"Sample Name: "<<samplename[i]<<"  Xval = "<<xval<<"  nbins = "<<nbins<<"  Target Bin Number : "<<bin<<"  Variable :  "<<variable<<endl;
    std::cout<<" Number of Entries > "<<xval<<" for Sample: "<<samplename[i]<<" = "<<n_more<<endl;
    std::cout<<" Error for Entries > "<<xval<<" for Sample: "<<samplename[i]<<" = "<<err_more<<endl;
    std::cout<<" Number of Entries < "<<xval<<" for Sample: "<<samplename[i]<<" = "<<n_less<<endl; 
    std::cout<<" Error for Entries < "<<xval<<" for Sample: "<<samplename[i]<<" = "<<err_less<<endl;

  }
  
  
  //Decorate the histograms using function decorate 
  // See function definition below for syntax
  // See https://root.cern.ch/root/html/TColor.html for color names

  //Now let us declare a canvas 
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->SetTitle("");
  
  // Now we define a legend
  TLegend *lg = new TLegend(0.65,0.60,0.85,0.80,NULL,"NDC");
  decorate(lg,0.02,""); // Decorated the legend using function below.
  	
  //Defining the Xmax, Xmin and Ymax and Ymin for the histogram
  //double xmin = h[0]->GetXaxis()->GetXmin();
  //double xmax = h[0]->GetXaxis()->GetXmax();
  double ymin = YMIN;
  double ymax = YMAX;
  double xmin = XMIN;
  double xmax = XMAX;
  
  for(int i = 0; i < nfile; i++){
    h[i]->SetStats(0);
    h[i]->SetLineColor(colors[i]);
    h[i]->SetFillColor(colors[i]);
    h[i]->SetLineWidth(4);
    h[i]->SetTitle("");
    h[i]->GetXaxis()->SetRangeUser(xmin,xmax);
    h[i]->GetYaxis()->SetRangeUser(0,ymax);
    decorate(h[i],xtitle,ytitle,"",colors[i],4,kGreen+2,20,0);
    hstack->Add(h[i]);
    lg->AddEntry(h[i],samplename[i],"f");
    gPad->Update();
  }
  lg->AddEntry(herr,"Total Combined Error", "f");

  hstack->Draw("HIST");
  hstack->SetMinimum(ymin);
  hstack->SetMaximum(ymax);  
  hstack->GetXaxis()->SetRangeUser(xmin,xmax);
  hstack->GetXaxis()->SetTitle(xtitle);
  hstack->GetYaxis()->SetTitle(ytitle);

  //Drawing the Error Histogram on top
  gStyle->SetHatchesLineWidth(4);
  herr->SetFillStyle(3004);
  herr->SetFillColorAlpha(kBlue, 1.0);
  herr->SetLineColor(kBlue);
  herr->Draw("E2 SAME");

  //Drawing the Legend
  lg->Draw();
  //pt->Draw();

  //Adding Text to the histograms
  TLatex txt;
  txt.SetNDC(true);
  txt.SetTextFont(42);
  txt.SetTextSize(0.03);
  txt.DrawLatex(0.13,0.84,
		Form("#splitline{#it{#bf{ATLAS Simulation: Work in Progress}}}{#splitline{#sqrt{s} = 13.6 TeV, 140 fb^{-1}}{#bf{%s}}}", REGION.Data())
		);

  c1->SaveAs("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/Plots/STACKPLOTS/"+ variable + "_Nopol_" + REGION + "_" + scalename + ".png");

}

/*
This function decorates the histogram, by setting the titles of the X and Y axes,
by setting the line and marker colors, the marker style, and by deciding whether
to fill in the histogram with a color or not.
 */
void decorate(TH1F* h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill) {

  h->GetXaxis()->SetTitle(xtitle);
  h->GetYaxis()->SetTitle(ytitle);
  //h->SetTitle("Efficiency vs truth Eta");
  h->SetLineColor(linecolor); 
  h->SetLineWidth(linewidth);
  h->SetMarkerColor(markercolor);
  h->SetMarkerStyle(markerstyle);
  if(tofill==1) h->SetFillColor(markercolor);

  h->SetMarkerSize(0.25);
  //h->SetTitle(title);
}

/*
This function decorates the legend, by setting the textsize, and filling in the
legend with a pure white color.
 */
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


// Here are a couple of other utility functions

// For a given histogram hst, return the number of entries between bin_lo and bin_hi
float get_nevents(TH1D *hst, float bin_lo, float bin_hi)
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
float get_nevents_err(TH1D *hst, float bin_lo, float bin_hi)
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
