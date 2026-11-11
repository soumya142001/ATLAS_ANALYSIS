#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
/*
This is a driver script.
It decides which code to run over which sample, the names
of output files and so on.
*/

/**********************************************************
     Cross-Sections of individual Samples (in fb)
************************************************************/

double xs_inclZZjj_TT = 3.4597;
double xs_inclZZjj_TL = 1.6042;
double xs_inclZZjj_LL = 0.31321;
double xs_ZZjjEW_TT = 0.3566;
double xs_ZZjjEW_TL = 0.1727;
double xs_ZZjjEW_LL = 0.0486;
double xs_ZZ_QCD  = 1331.2;
double xs_EWK_ZZjj  = 14.802;
double xs_ggllll_M4l130  = 12.4*1.7;
double xs_gg4l_M4l_70_3000  = 35.2;


void ana(TString sample = "incl_ZZjj_TT"){
  TString sumfilename, rootfilename;
  int data;
  double xs;
  //Declare a chain for input files.
  TChain *chain = new TChain("Nominal;1");
  
  //Declare an instance of our code class
  MiniTree_Analyzer m_selec;
  
  if(sample=="ZZjjEW_TT"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/EWK_polar/merged/mc23_13p6TeV.546887.merged.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_ZZjjEW_TT;
  }

  if(sample=="ZZjjEW_TL"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/EWK_polar/merged/mc23_13p6TeV.546888.merged.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_ZZjjEW_TL;
  }
  
  if(sample=="ZZjjEW_LL"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/EWK_polar/merged/mc23_13p6TeV.546889.merged.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_ZZjjEW_LL;
  }

    if(sample=="incl_ZZjj_TT"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/QCD_polar/merged/mc23_13p6TeV.546884.*.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_inclZZjj_TT;
  }

  if(sample=="incl_ZZjj_TL"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/QCD_polar/merged/mc23_13p6TeV.546885.merged.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_inclZZjj_TL;
  }
  
  if(sample=="incl_ZZjj_LL"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/QCD_polar/merged/mc23_13p6TeV.546886.merged.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_inclZZjj_LL;
  }
  
  if(sample=="incl_ZZjj_LL"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/QCD_polar/merged/mc23_13p6TeV.546886.merged.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_inclZZjj_LL;
  }

  if(sample=="ZZ_QCD"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/mc/mc23_13p6TeV.701040.*.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_ZZ_QCD;
  }
  
  if(sample=="EWK_ZZjj"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/mc/mc23_13p6TeV.701000.*.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_EWK_ZZjj;
  }
  
  if(sample=="gg4l_M4l_70_3000"){
    //Add one file to chain. This is the input file.
    chain->Add("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/reco_wp85/mc/mc23_13p6TeV.605999.*.root");
    rootfilename = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_ROOTFILES/"+ sample + ".root";
    sumfilename  = "/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/NTupleAnalyzer/OUTPUT_SUMMARYFILES/"+ sample +".txt";
    data = 0; //data = 1, MC = 0
    xs = xs_gg4l_M4l_70_3000;
  }
    
  //Finally running the process
  std::cout<<"Output Files are: "<<rootfilename<<" and "<<sumfilename<<std::endl;
  m_selec.SetRootFileName(rootfilename);
  m_selec.SetSumFileName(sumfilename);
  m_selec.SetDataMCFlag(data);
  m_selec.SetXS(xs);
  chain->Process(&m_selec);
}
