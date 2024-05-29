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

#include "hits.h"

using namespace ROOT::VecOps;
using namespace std;
using ROOT::RDataFrame;

void hits()
{

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

    TFile *f_file = TFile::Open("ana.root","READ");
    TTree *tr = (TTree*) f_file->Get("save");
    TTree *tr_tls = (TTree*) f_file->Get("tls");

   // choose the range of run numbers need to be analyzed and show up in the plots
   int xlow = 5469;
   int xhigh = 5471; 

   //set the first run_ID in the save tree as the run_num
   int run_num = 5470;

   //ref to keep track of the entry from the tls tree
   int tls_entry = 0; 

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("event_ID", &event_ID);
   tr->SetBranchAddress("dor", &dor);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("nQualTracklets", &nQualTracklets);
   tr->SetBranchAddress("tlBackPartial", &tlBackPartial);

   tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
   tr_tls->SetBranchAddress("detIDs", &detIDs);
   tr_tls->SetBranchAddress("stID", &stID);
   tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
   tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);

   std::map<int, std::vector<std::pair<int, double>>>* hitInfoP;
   tr->SetBranchAddress("hitInfo", hitInfoP);
   std::map<int, std::vector<std::pair<int, double>>> hitInfo = *hitInfoP;

   //////////
   bool Trigger_Filter = false; //set to "true" if need to filter hits based on trigger.
   int det_ID = 32; // pick the detector plane to be analyzed
   /////////

   int nEntries = tr->GetEntries();
   for (int i_ent = 0; i_ent < nEntries; i_ent++) {

      tr->GetEntry(i_ent);
      if(run_ID < xlow || run_ID > xhigh) {continue;}
      ++nEvents;

      if(Trigger_Filter){

          //if((trigger & 0x1) != 0) { trigger_temp = "NIM1"; ++total_N1;}  //NIM1
          //if((trigger & 0x2) != 0) { trigger_temp = "NIM2"; ++total_N2;}  //NIM2
          //if((trigger & 0x4) != 0) { trigger_temp = "NIM3"; ++total_N3;}  //NIM3
          //if((trigger & 0x8) != 0) { trigger_temp = "NIM4"; total_N4++;}  //NIM4 
          if((trigger & 0x200) != 0) { trigger_temp = "MATRIX5"; ++total_M5;}  //MATRIX5
          else{continue;}
       
      }

       if (tlBackPartial>0){ 
            total_bp++;
            if (num_h3b>0 && num_h3t ==0){++h3b_hits;}
            if (num_h3t>0 && num_h3b ==0){++h3t_hits;}
            if (num_h3t >0 && num_h3b >0 ) {++has_hits;} 
            if (num_h3t ==0 && num_h3b ==0){++no_hits;}
       }
      
      if(dor < 0) {run_num = run_ID; std::cout << "invalid dor"<<std::endl; continue;}

      //since run_num set to the run_ID of the first event in the tree following if condition will be satisfied in the first iteration.
     
        for (auto it = hitInfo.begin(); it != hitInfo.end(); it++){
            if (it->first == det_ID){
               for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++ ){
                    htdc->Fill(it2->second);
                    std::cout<<"Filling :"<<it2->second<<std::endl;
                }
            }
        }
    } 
    TCanvas* c1 = new TCanvas(Form("tdc_time of hits in %d from all events", det_ID), Form("tdc_time of hits in %d from all events", det_ID), 1000, 500);
    
    htdc->SetLineColor(7);
    htdc->SetTitle(Form("tdc time - %d", det_ID));
    htdc->Draw();
    c1->Update();
    
    gSystem->mkdir(Form("hitTDC/%s",trigger_temp.c_str()), 1);

    c1->SaveAs(Form("hitTDC/tdc_h1_%d.png",det_ID));
    std::cout<<"*************************"<<std::endl;
}

