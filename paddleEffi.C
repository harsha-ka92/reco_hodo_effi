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

TFile *f_file;
TTree *tr;
TTree *tr_tls;
void getEffi(TTree* evtTree, TTree* tlsTree, int ID, int nPaddles, int cut);
TH1D *diff = new TH1D("diff","difff", 11, -5.5, 5.5);

int pad_diff =0; int exps; int closest; bool bPassed;

// choose the range of run numbers need to be analyzed and show up in the plots
int xlow = 5024;
int xhigh = 5042; 

//set the first run_ID in the save tree as the run_num
int run_num = 5024;

//ref to keep track of the entry from the tls tree
int tls_entry = 0; 

void paddleEffi()
{
    gSystem->mkdir("effi", true);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

    f_file = TFile::Open("ana.root","READ");
    tr = (TTree*) f_file->Get("save");
    tr_tls = (TTree*) f_file->Get("tls");

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

    //H1
    getEffi(tr, tr_tls, 31, 23, 10); getEffi(tr, tr_tls, 32, 23, 10); getEffi(tr, tr_tls, 33, 20, 10); getEffi(tr, tr_tls, 34, 20, 10);
    //H2
    /*getEffi(tr, tr_tls, 35, 19, 10); getEffi(tr, tr_tls, 36, 19, 10); getEffi(tr, tr_tls, 37, 16, 10); getEffi(tr, tr_tls, 38, 16, 10); */
    //H3
    getEffi(tr, tr_tls, 39, 16, 10); getEffi(tr, tr_tls, 40, 16, 10); 
    //H4Y1
    getEffi(tr, tr_tls, 41, 16, 10); getEffi(tr, tr_tls, 42, 16, 10); 
    //H4X and H4Y2
    /*getEffi(tr, tr_tls, 43, 16, 10); getEffi(tr, tr_tls, 44, 16, 10); getEffi(tr, tr_tls, 45, 16, 10); getEffi(tr, tr_tls, 46, 16, 10);*/
}

void getEffi(TTree* evtTree, TTree* tlsTree, int ID, int nPaddles, int cut){
    //auto exps = new TH1D("exps","exps",nPaddles+1, 0.5, nPaddles+0.5);
    //auto closest = new TH1D("closest","closest", nPaddles+1, 0.5, nPaddles+0.5);
    
    TEfficiency* effi = new TEfficiency("effi", "effi", nPaddles+1, 0.5, nPaddles+0.5);
    ostringstream oss;
    oss<< "efficiencies of the paddles of detID "<< ID ;

    bool Trigger_Filter = true; //set to "true" if need to filter hits based on trigger.
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
                                    exps= eleID_exps->at(j);
                                    pad_diff =eleID_exps->at(j)-eleID_closests->at(j);
                                    bPassed = (eleID_closests->at(j)>0 && pad_diff <10);
                                    effi->Fill(bPassed, exps);
                                }
                                else {;}
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
                                    exps= eleID_exps->at(j);
                                    pad_diff =eleID_exps->at(j)-eleID_closests->at(j);
                                    bPassed = (eleID_closests->at(j)>0 && pad_diff <10);
                                    effi->Fill(bPassed, exps);
                                }
                                else {;}
                        }
                }
                }    
            }
    }
    }
    }
    TCanvas* c1 = new TCanvas("c1", "");
    c1->SetGrid();

    effi->SetName(oss.str().c_str());
    effi->SetTitle(";paddle number; efficiency");
    
    effi->SetMarkerColor(kMagenta);
    effi->SetMarkerStyle(20);

    oss.str("");
    oss << "effi/effi_of_" << ID << ".png";
    effi->Draw("APE1");
    c1->Update();
    c1->SaveAs(oss.str().c_str());

    diff->SetFillColorAlpha(kAzure+6, 0.35);
    oss.str("");
    oss << "effi/diff_of_" << ID << ".png";
    diff->Draw("HIST");
    c1->Update();
    c1->SaveAs(oss.str().c_str());
    
    effi =0;
    diff->Reset("ICESM");;
}
