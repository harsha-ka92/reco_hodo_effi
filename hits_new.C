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
   int xlow = 5170;
   int xhigh = 5210; 

   //set the first run_ID in the save tree as the run_num
   int run_num = 5170;

   //ref to keep track of the entry from the tls tree
   int tls_entry = 0; 

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("event_ID", &event_ID);
   tr->SetBranchAddress("dor", &dor);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("nQualTracklets", &nQualTracklets);
   tr->SetBranchAddress("hitInfo", &hitInfo);
   
   tr->SetBranchAddress("tlBackPartial", &tlBackPartial);

   tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
   tr_tls->SetBranchAddress("detIDs", &detIDs);
   tr_tls->SetBranchAddress("stID", &stID);
   tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
   tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);

   //////////
   bool Trigger_Filter = false; //set to "true" if need to filter hits based on trigger.
   int det_ID = 0; // pick the detector plane to be analyzed
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
     
        for (auto it = hitInfo->begin(); it != hitInfo->end(); it++){
            if (it->first == det_ID){
               for ( int j =0; j< tdc_h1t->size(); j++){
                    htdc_h1t->Fill(tdc_h1t->at(j));
                }
            }
        }
    } 
    TCanvas* c1 = new TCanvas(Form("tdc_time of hits in %d from all events", det_ID), Form("tdc_time of hits in %d from all events", det_ID), 1000, 500);
    
    htdc_h1t->SetLineColor(7);
    htdc_h1t->SetTitle(Form("tdc time - %s",));

    auto l1 = new TLegend(0.7,0.65,0.85,0.85);
    l1->AddEntry(htdc_h1t, Form("run ID %d",run_num) , "");
    l1->AddEntry(htdc_h1t, "h1t", "l");
    l1->AddEntry(htdc_h1b, "h1b", "l");
    l1->AddEntry(htdc_h1r, "h1r", "l");
    l1->AddEntry(htdc_h1l, "h1l", "l");
    l1->Draw();
    c1->Update();
    
    gSystem->mkdir(Form("hitTDC/%s",trigger_temp.c_str()), 1);

    c1->SaveAs(Form("hitRates/%s/tdc_h1.png",trigger_temp.c_str()));
    c2->SaveAs(Form("hitRates/%s/tdc_h2.png",trigger_temp.c_str()));
    c3->SaveAs(Form("hitRates/%s/tdc_h3.png",trigger_temp.c_str()));
    c4->SaveAs(Form("hitRates/%s/tdc_h4.png",trigger_temp.c_str()));

    std::cout<<"*************************"<<std::endl;
}

