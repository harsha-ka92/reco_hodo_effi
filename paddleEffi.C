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
#include <algorithm>
#include <AnaModule/LinkDef.h>
#include "hits.h"

using namespace ROOT::VecOps;
using namespace std;

TFile *f_file;
TTree *tr;
TTree *tr_tls;
void getEffi(TTree* evtTree, TTree* tlsTree, int ID, int nPaddles, int cut);
TH1D *diff = new TH1D("diff","difff", 11, -5.5, 5.5);

int pad_diff =0; int exps =0; int closest =0; bool bPassed;
int trigger_tls;

// choose the range of run numbers need to be analyzed and show up in the plots
int xlow = 5170;
int xhigh = 5221; 

//set the first run_ID in the save tree as the run_num
int run_num = 5170;

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
   tr->SetBranchAddress("hitInfo", &hitInfo);
   
   tr->SetBranchAddress("tlBackPartial", &tlBackPartial);

   tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
   tr_tls->SetBranchAddress("detIDs", &detIDs);
   tr_tls->SetBranchAddress("trigger", &trigger_tls);
   tr_tls->SetBranchAddress("stID", &stID);
   tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
   tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);
   tr_tls->SetBranchAddress("chisq", &chisq);
   tr_tls->SetBranchAddress("nHits", &nHits);

    //H1
    /*getEffi(tr, tr_tls, 31, 23, 2); getEffi(tr, tr_tls, 32, 23, 2); getEffi(tr, tr_tls, 33, 20, 2); getEffi(tr, tr_tls, 34, 20, 2);*/
    //H2
   /* getEffi(tr, tr_tls, 35, 19, 2); getEffi(tr, tr_tls, 36, 19, 2); getEffi(tr, tr_tls, 37, 16, 2); getEffi(tr, tr_tls, 38, 16, 2); */
    //H3
    getEffi(tr, tr_tls, 39, 16, 1); getEffi(tr, tr_tls, 40, 16, 1); 
    //H4Y1
    getEffi(tr, tr_tls, 41, 16, 1); getEffi(tr, tr_tls, 42, 16, 1); 
    //H4X and H4Y2
    getEffi(tr, tr_tls, 43, 16, 1); getEffi(tr, tr_tls, 44, 16, 1);  getEffi(tr, tr_tls, 45, 16, 1); getEffi(tr, tr_tls, 46, 16, 1);
}

void getEffi(TTree* evtTree, TTree* tlsTree, int ID, int nPaddles, int cut){

    TEfficiency* effi = new TEfficiency("effi", "effi", nPaddles+1, 0.5, nPaddles+0.5);
    ostringstream oss;
    oss<< "efficiencies of the paddles of detID "<< ID ;

    //bool Trigger_Filter = true; //set to "true" if need to filter hits based on trigger.
    
    int nEntries = tr->GetEntries();
    
    int ID_index;
    bool h4y1 = false; bool h4y2 = false; bool h4x = false; 

    std::cout<<"Analyzing the stID :"<<ID<<std::endl;
    std::cout<< "paddel difference cut :" << cut<<endl;

    for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){

      if(run_ID < xlow || run_ID > xhigh) {continue;}
      if(dor < 0) {run_num = run_ID; continue;}

      tr_tls->GetEntry(i_tls_entry);

      //use NIM 4 events to get st3 and st4 efficiencies.

      if ((trigger & 0x8) == 0 || event_ID != tls_event_ID){continue;}

      ID_index = -99; closest = -99;
      h4y1 = false; h4y2 = false;  h4x = false; 

        //st1 efficiencies
        if(ID>30 && ID < 35){
                if(stID == 1 || stID == 3 && chisq <8){
                    for ( int j =0; j< detIDs->size(); j++){
                                if ((detIDs -> at(j) == 41 || detIDs -> at(j) == 42) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=1 ) {h2y = true;}
                                if ((detIDs -> at(j) == 43 || detIDs -> at(j) == 44) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=1 ) {h2x = true;}
                                if ((detIDs -> at(j) == 45 || detIDs -> at(j) == 46) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=1 ) {h3x = true;}
                                if(detIDs->at(j) == ID){ ID_index = j; exps= eleID_exps->at(j); closest = eleID_closests->at(j);}
                                }
                         if (closest >0 && ID_index >=0 && h2x) {
                            pad_diff = exps-closest; 
                            bPassed = (fabs(pad_diff) <= 1);
                            effi->Fill(bPassed, exps);
                            if (bPassed) {diff->Fill(pad_diff);}
                        }
                }

        } 

        //st2 efficiencies
        if(ID>34 && ID < 39){
                if(stID == 1 || stID == 2 || stID == 5){
                        for ( int j =0; j< detIDs->size(); j++){
                                if(detIDs->at(j) == ID){
                                    exps= eleID_exps->at(j); 
                                    closest = eleID_closests->at(j);
                                    if (closest >0) {
                                        pad_diff =exps-closest; 
                                        bPassed = (fabs(pad_diff) <= cut);
                                        effi->Fill(bPassed, exps);
                                        if (bPassed) {diff->Fill(pad_diff);}
                                    }
                                }
                                else {;}
                        }
                }
        }    

        // for st3 and st4 pick only back partial tracklets from NIM 4 events and ask if they truely belongs to a H24 ray.
        if(ID>38 && ID < 47){
            if(stID == 6 && chisq < 8){
                        for ( int j =0; j< detIDs->size(); j++){
                                if ((detIDs -> at(j) == 41 || detIDs -> at(j) == 42) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=1 ) {h4y1 = true;}
                                if ((detIDs -> at(j) == 43 || detIDs -> at(j) == 44) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=1 ) {h4y2 = true;}
                                if ((detIDs -> at(j) == 45 || detIDs -> at(j) == 46) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=1 ) {h4x = true;}
                                if(detIDs->at(j) == ID){ ID_index = j; exps= eleID_exps->at(j); closest = eleID_closests->at(j);}
                                }
            }
        
            if (closest >0 && ID_index >=0 && h4x) {
                pad_diff = exps-closest; 
                bPassed = (fabs(pad_diff) <= 1);
                effi->Fill(bPassed, exps);
                if (bPassed) {diff->Fill(pad_diff);}
            }
                
        }
     }
    }
    

    std::cout<<std::endl;

    TCanvas* c1 = new TCanvas("c1", "");
    c1->SetGrid();

    effi->SetName(oss.str().c_str());
    effi->SetTitle(";Paddle Number; Efficiency");
    
    effi->SetMarkerColor(kMagenta);
    effi->SetMarkerStyle(20);

    oss.str("");
    oss << "effi/effi_of_" << ID << ".png";
    effi->Draw("APE1");
    c1->Update();
    c1->SaveAs(oss.str().c_str());

    diff->SetFillColorAlpha(kAzure+6, 0.35);
    oss.str("");
    oss << "Distribution of exp - closest in detID :" << ID << ".png";
    diff->SetTitle(oss.str().c_str());
    diff->GetXaxis()->SetTitle("Paddle Number");
    diff->GetYaxis()->SetTitle("nTracklets");
    diff->Draw("HIST");
    c1->Update();
    oss.str("");
    oss << "effi/diff_of_" << ID << ".png";
    c1->SaveAs(oss.str().c_str());
    
    effi =0;
    diff->Reset("ICESM");;
}
