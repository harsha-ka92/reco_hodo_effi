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

   bool Trigger_Filter = false; //set to "true" if need to filter hits based on trigger.
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

           for ( int j =0; j< tdc_h1t->size(); j++){
                htdc_h1t->Fill(tdc_h1t->at(j));
            }
           for ( int j =0; j< tdc_h1b->size(); j++){
                htdc_h1b->Fill(tdc_h1b->at(j));
            }
            for ( int j =0; j< tdc_h1r->size(); j++){
                htdc_h1r->Fill(tdc_h1r->at(j));
            }
        }
    }

    TCanvas* c1 = new TCanvas(Form("tdc_time of hits from %s events : St1", trigger_temp.c_str()), Form("tdc_time of %s hits : St1", trigger_temp.c_str()), 1000, 500);
    //THStack* hs;
    //hs  = new THStack("hs", Form("tdc_time of hits from %s events : St1", trigger_temp.c_str()));
    htdc_h1t->SetLineColor(7);
    htdc_h1t->SetTitle("tdc time - st1");
    htdc_h1t->Draw();
    htdc_h1b->SetLineColor(2);
    htdc_h1b->Draw("SAME");
    htdc_h1r->SetLineColor(3);
    htdc_h1r->Draw("SAME");
    htdc_h1l->SetLineColor(4);
    htdc_h1l->Draw("SAME");
    auto l1 = new TLegend(0.7,0.65,0.85,0.85);
    l1->AddEntry(htdc_h1t, Form("run ID %d",run_num) , "");
    l1->AddEntry(htdc_h1t, "h1t", "l");
    l1->AddEntry(htdc_h1b, "h1b", "l");
    l1->AddEntry(htdc_h1r, "h1r", "l");
    l1->AddEntry(htdc_h1l, "h1l", "l");
    l1->Draw();
    /*hs->Add(htdc_h1t); hs->Add(htdc_h1b); hs->Add(htdc_h1r); hs->Add(htdc_h1l);*/
    //hs->Draw("nonstack");
    c1->Update();
    
    TCanvas* c2 = new TCanvas(Form("tdc_time of %s hits : St2", trigger_temp.c_str()), Form("tdc_time of %s hits : St2", trigger_temp.c_str()), 1000, 500);
    htdc_h2t->SetLineColor(7);
    htdc_h2t->SetTitle("tdc time - st2");
    htdc_h2t->Draw();
    htdc_h2b->SetLineColor(2);
    htdc_h2b->Draw("SAME");
    htdc_h2r->SetLineColor(3);
    htdc_h2r->Draw("SAME");
    htdc_h2l->SetLineColor(4);
    htdc_h2l->Draw("SAME");
    auto l2 = new TLegend(0.7,0.65,0.85,0.85);
    //l2->AddEntry(htdc_h2t, Form("run ID %d",run_num) , "");
    l2->AddEntry(htdc_h2t, "h2t", "l");
    l2->AddEntry(htdc_h2b, "h2b", "l");
    l2->AddEntry(htdc_h2r, "h2r", "l");
    l2->AddEntry(htdc_h2l, "h2l", "l");
    l2->Draw();
    c2->Update();

    TCanvas* c3 = new TCanvas(Form("tdc_time of %s hits : St3", trigger_temp.c_str()), Form("tdc_time of %s hits : St3", trigger_temp.c_str()), 1000, 500);
    htdc_h3t->SetLineColor(7);
    htdc_h3t->SetTitle("tdc time - st3");
    htdc_h3t->Draw();
    htdc_h3b->SetLineColor(2);
    htdc_h3b->Draw("SAME");
    auto l3 = new TLegend(0.7,0.65,0.85,0.85);
    //l3->AddEntry(htdc_h3t, Form("run ID %d",run_num) , "");
    l3->AddEntry(htdc_h3t, "h3t", "l");
    l3->AddEntry(htdc_h3b, "h3b", "l");
    l3->Draw();
    c3->Update();

    TCanvas* c4 = new TCanvas(Form("tdc_time of %s hits : St4", trigger_temp.c_str()), Form("tdc_time of %s hits : St4", trigger_temp.c_str()), 1000, 500);
    htdc_h4t->SetLineColor(7);
    htdc_h4t->SetTitle("tdc time - st4");
    htdc_h4t->Draw();
    htdc_h4b->SetLineColor(2);
    htdc_h4b->Draw("SAME");
    htdc_h4y1r->SetLineColor(3);
    htdc_h4y1r->Draw("SAME");
    htdc_h4y1l->SetLineColor(4);
    htdc_h4y1l->Draw("SAME");
    htdc_h4y2r->SetLineColor(5);
    htdc_h4y2r->Draw("SAME");
    htdc_h4y2l->SetLineColor(6);
    htdc_h4y2l->Draw("SAME");
    auto l4 = new TLegend(0.7,0.65,0.85,0.85);
    //l4->AddEntry(htdc_h4t, Form("run ID %d",run_num) , "");
    l4->AddEntry(htdc_h4t, "h4t", "l");
    l4->AddEntry(htdc_h4b, "h4b", "l");
    l4->AddEntry(htdc_h4y1r, "h4y1r", "l");
    l4->AddEntry(htdc_h4y1l, "h4y1l", "l");
    l4->AddEntry(htdc_h4y2r, "h4y2r", "l");
    l4->AddEntry(htdc_h4y2l, "h4y2l", "l");
    l4->Draw();
    c4->Update();

    gSystem->mkdir(Form("hitTDC/%s",trigger_temp.c_str()), 1);

    c1->SaveAs(Form("hitRates/%s/tdc_h1.png",trigger_temp.c_str()));
    c2->SaveAs(Form("hitRates/%s/tdc_h2.png",trigger_temp.c_str()));
    c3->SaveAs(Form("hitRates/%s/tdc_h3.png",trigger_temp.c_str()));
    c4->SaveAs(Form("hitRates/%s/tdc_h4.png",trigger_temp.c_str()));

    std::cout<<"*************************"<<std::endl;
}

