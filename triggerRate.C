R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libanamodule)

#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TFile.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>
#include <TCanvas.h>
#include <iostream>

using namespace ROOT::VecOps;
using namespace std;
using ROOT::RDataFrame;

void triggerRate()
{
    gSystem->mkdir("triggerRates", 1);

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

    TFile *f_file = TFile::Open("ana.root","read");
    TTree *tr = (TTree*) f_file->Get("save");

   int run_num = 4796;
   int run_time = 0;
   int nim1 = 0;
   int nim2 = 0;
   int nim3 = 0;
   int nim4 = 0;
   int matrix5 = 0;
  
   int trigger, dor, run_ID;

   tr->SetBranchAddress("run_ID", &run_ID);
        //tr->SetBranchAddress("weight", &weight);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("dor", &dor);

   
   for (int i_ent = 0; i_ent <tr->GetEntries(); i_ent++) {
        tr->GetEntry(i_ent);
        
     
      if(run_num == run_ID){
         if(trigger == 1) {nim1 +=1;}
         if(trigger == 2) {nim2 +=1;}
         if(trigger == 3) {nim3 +=1;}
         if(trigger == 4) {nim4 +=1;}
         if(trigger == 5) {matrix5 +=1;}
         run_time=dor;
         run_num == run_ID.at(i_ent);
       }
   }
   
}
