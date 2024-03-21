R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libanamodule)

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

void paddleEffi()
{
    gSystem->mkdir("effi", true);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    
    TFile *f_file = TFile::Open("ana.root","READ");
    TTree *tr = (TTree*) f_file->Get("save");
    TTree *tr_tls = (TTree*) f_file->Get("tls");

   // choose the range of run numbers need to be analyzed and show up in the plots
   int xlow = 5024;
   int xhigh = 5042; 

   //set the first run_ID in the save tree as the run_num
   int run_num = 5024;

   //ref to keep track of the entry from the tls tree
   int tls_entry = 0; 

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("event_ID", &event_ID);
   tr->SetBranchAddress("dor", &dor);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("nQualTracklets", &nQualTracklets);

   tr->SetBranchAddress("num_h1t", &num_h1t);
   tr->SetBranchAddress("num_h1b", &num_h1b);
   tr->SetBranchAddress("num_h1r", &num_h1r);
   tr->SetBranchAddress("num_h1l", &num_h1l);

   tr->SetBranchAddress("num_h2t", &num_h2t);
   tr->SetBranchAddress("num_h2b", &num_h2b);
   tr->SetBranchAddress("num_h2r", &num_h2r);
   tr->SetBranchAddress("num_h2l", &num_h2l);

   tr->SetBranchAddress("num_h3t", &num_h3t);
   tr->SetBranchAddress("num_h3b", &num_h3b);

   tr->SetBranchAddress("num_h4t", &num_h4t);
   tr->SetBranchAddress("num_h4b", &num_h4b);
   tr->SetBranchAddress("num_h4y1r", &num_h4y1r);
   tr->SetBranchAddress("num_h4y1l", &num_h4y1l);
   tr->SetBranchAddress("num_h4y2r", &num_h4y2r);
   tr->SetBranchAddress("num_h4y2l", &num_h4y2l);
   
   tr->SetBranchAddress("tlBackPartial", &tlBackPartial);

   tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
   tr_tls->SetBranchAddress("detIDs", &detIDs);
   tr_tls->SetBranchAddress("stID", &stID);
   tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
   tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);

   Void getEffi(int ID, int nPaddles, int cut){
}

Void getEffi(int ID, int nPaddles, int cut){
    TH1D *exps = new TH1F("exps","exps",npaddles+1, 0.5, nPaddles+0.5);
    TH1D *closest = new TH1F("closest","closest", npaddles+1, 0.5, nPaddles+0.5);
    TH1D *diff = new TH1F("diff","difff", 11, -5.5, 5.5);
    TCanvas* c1 = new TCanvas("c1", "");
    c1->SetGrid();
    int pad_diff =0;

    bool Trigger_Filter = false; //set to "true" if need to filter hits based on trigger.
    int nEntries = tr->GetEntries();
    for (int i_ent = 0; i_ent < nEntries; i_ent++) {

      tr->GetEntry(i_ent);
      if(run_ID < xlow || run_ID > xhigh) {continue;}
      ++nEvents;

      if(Trigger_Filter == true){
          //use NIM 4 events to get st1 and st3 efficiencies and H4Y1. NIM 2 events for now to get st2 and st4 efficiencies.
          //if((trigger & 0x1) != 0) { trigger_temp = "NIM1"; ++total_N1;}  //NIM1
          //if((trigger & 0x2) != 0) { trigger_temp = "NIM2"; ++total_N2;}  //NIM2
          //if((trigger & 0x4) != 0) { trigger_temp = "NIM3"; ++total_N3;}  //NIM3
          if((trigger & 0x8) != 0) { trigger_temp = "NIM4"; total_N4++;}  //NIM4 
          //if((trigger & 0x200) != 0) { trigger_temp = "MATRIX5"; ++total_M5;}  //MATRIX5
          else{continue;}
       
      }

    if(dor < 0) {run_num = run_ID; std::cout << "invalid dor"<<std::endl; continue;}

    //st1 and st3 efficiencies
    if(ID>30 && ID < 43){
        if ((num_h2t + num_h2b) >0 && (num_h2r + num_h2l) > 0 && (num_h4t + num_h4b)>0 && (num_h4y2r + num_h4y2l) > 0 ){
            for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){
                tr_tls->GetEntry(i_tls_entry);
                if(stID == 6){
                if (event_ID == tls_event_ID){ 
                        for ( int j =0; j< detIDs->size(); j++){
                                if(detIDs->at(j) == ID){
                                    exps-> Fill(eleID_exps->at(j));
                                    if(eleID_closests->at(j)>0){ pad_diff =eleID_exps->at(j)-eleID_closests->at(j);
                                            if (pad_diff <= cut) {
                                            closest -> Fill(eleID_closests->at(j));
                                            diff -> Fill(pad_diff);
                                            }
                                    }
                                    }
                                else { std::cout<<"no valid expected hit for the back partial track considered"<<std::endl;}
                        }
                }
                }    
            }
          }
        }

    //st4 efficiencies
    else if (ID >42 && ID < 47){
    if ((num_h1t + num_h1b) >0 && (num_h1r+ num_h1l) > 0 && (num_h2t + num_h2b) >0 && (num_h2r + num_h2l) > 0) {
        for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){
                tr_tls->GetEntry(i_tls_entry);
                if(stID == 6){
                if (event_ID == tls_event_ID){ 
                        for ( int j =0; j< detIDs->size(); j++){
                                if(detIDs->at(j) == ID){
                                    exps-> Fill(eleID_exps->at(j));
                                    if(eleID_closests->at(j)>0){ pad_diff =eleID_exps->at(j)-eleID_closests->at(j);
                                            if (pad_diff <= cut) {
                                            closest -> Fill(eleID_closests->at(j));
                                            diff -> Fill(pad_diff);
                                            }
                                    }
                                    }
                                else { std::cout<<"no valid expected hit for the back partial track considered"<<std::endl;}
                        }
                }
                }    
            }
    }
    }
    }
    ostringstream oss;
    oss<< "efficiencies of the paddles of detID :"<< ID ;
    auto effi = new TEfficiency(closest, exps);
    effi->SetName(oss);
    effi->SetTitle(";paddle number; efficiency");
    
    effi->SetMarkerColor(kMagenta);
    effi->SetMarkerStyle(20);

    oss.str("");
    oss << "effi/effi_of:" << ID << ".png";
    effi->Draw("APE1");
    c1->SaveAs(oss.str().c_str());

    diff->SetFillColorAlpha(kAzure+6, 0.35);
    oss.str("");
    oss << "effi/diff_of:" << ID << ".png";
    diff->Draw("HIST");
    c1->SaveAs(oss.str().c_str());
    
    
    delete exps;
    delete closest;
    delete effi;
    delete c1;
    delete diff;
}
