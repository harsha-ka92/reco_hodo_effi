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
#include "hits.h"

using namespace ROOT::VecOps;
using namespace std;

TFile *f_file;
TTree *tr;
TTree *tr_tls;
void getEffi( TTree* tlsTree, int ID, int nPaddles, int cut);
TH1D *diff = new TH1D("diff","difff", 11, -5.5, 5.5);

int pad_diff =0; int exps =0; int closest =0; bool bPassed;
int trigger_tls;

//ref to keep track of the entry from the tls tree
int tls_entry = 0; 

void paddleEffi()
{
    gSystem->mkdir("effi", 1);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

    f_file = TFile::Open("ana.root","READ");
    tr_tls = (TTree*) f_file->Get("tls");

   tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
   tr_tls->SetBranchAddress("detIDs", &detIDs);
   tr_tls->SetBranchAddress("trigger", &trigger_tls);
   tr_tls->SetBranchAddress("stID", &stID);
   tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
   tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);
   tr_tls->SetBranchAddress("chisq", &chisq);
   tr_tls->SetBranchAddress("nHits", &nHits);

    //H1
    getEffi(tr_tls, 31, 23, 1); getEffi(tr_tls, 32, 23, 1); getEffi(tr_tls, 33, 20, 1); getEffi(tr_tls, 34, 20, 1);
    //H2
    getEffi(tr_tls, 35, 19, 1); getEffi(tr_tls, 36, 19, 1); getEffi(tr_tls, 37, 16, 1); getEffi(tr_tls, 38, 16, 1);
    //H3
    getEffi(tr_tls, 39, 16, 1); getEffi(tr_tls, 40, 16, 1); 
    //H4Y1
    getEffi(tr_tls, 41, 16, 1); getEffi(tr_tls, 42, 16, 1); 
    //H4X and H4Y2
    getEffi(tr_tls, 43, 16, 1); getEffi(tr_tls, 44, 16, 1);  getEffi(tr_tls, 45, 16, 1); getEffi(tr_tls, 46, 16, 1);
}

void getEffi( TTree* tlsTree, int ID, int nPaddles, int cut){

    TEfficiency* effi = new TEfficiency("effi", "effi", nPaddles+1, 0.5, nPaddles+0.5);
    ostringstream oss;
    oss<< "efficiencies of the paddles of detID "<< ID ;
  
    int ID_index;
    bool h1x = false; bool h1y = false; 
    bool h2x = false; bool h2y = false;
    bool h3x = false;
    bool h4y1 = false; bool h4y2 = false; bool h4x= false;

    std::cout<<"Analyzing the stID :"<<ID<<std::endl;
    std::cout<< "paddel difference cut :" << cut<<endl;

    for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){

      if (++i_tls_entry % 70000 == 0) cout << " . " << flush;

      tr_tls->GetEntry(i_tls_entry);

      //use NIM 4 events to get st3 and st4 efficiencies. And NIM 2 events for st1 and st2
      if (ID <= 30) {std::cout<<"Invalid station ID"<<std::endl; break;}
      if ((ID>30 && ID<39) && ((trigger_tls & 0x2) == 0) ){continue;} //select NIM2
      if ((ID>38 && ID<47) && ((trigger_tls & 0x8) == 0) ){continue;} //select NIM4
      if (ID >= 47) {std::cout<<"Invalid station ID"<<std::endl; break;}
 
      //exclude the stIDs that are not considering for the analysis
      if (stID != 1 || stID != 3 || stID !=6) {continue;}

      ID_index = -99; closest = -99;
      h1y = false; h1x = false;
      h2y = false; h2x = false;
      h3x = false;
      h4y1 = false; h4y2 = false;  h4x = false; 

      //check for valid expected and coresponding closest hits in planes
      for ( int j =0; j< detIDs->size(); j++){
                                if ((detIDs -> at(j) == 33 || detIDs -> at(j) == 34) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut ) {h1y = true;}
                                if ((detIDs -> at(j) == 31 || detIDs -> at(j) == 32) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h1x = true;}
                                if ((detIDs -> at(j) == 35 || detIDs -> at(j) == 36) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h2y = true;}
                                if ((detIDs -> at(j) == 37 || detIDs -> at(j) == 38) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h2x = true;}
                                if ((detIDs -> at(j) == 39 || detIDs -> at(j) == 40) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h3x = true;}
                                if ((detIDs -> at(j) == 41 || detIDs -> at(j) == 42) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h4y1 = true;}
                                if ((detIDs -> at(j) == 43 || detIDs -> at(j) == 44) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h4y2 = true;}
                                if ((detIDs -> at(j) == 45 || detIDs -> at(j) == 46) &&  fabs(eleID_exps ->at(j) - eleID_closests->at(j)) <=cut) {h4x = true;}
                                if(detIDs->at(j) == ID){ ID_index = j; exps= eleID_exps->at(j); closest = eleID_closests->at(j);}
        }

        //st1 and st2 efficiencies
        if((stID == 1 || stID == 3) && chisq <8){
            if(ID>30 && ID < 37){
                         if (closest >0 && ID_index >=0 && h2x) {
                            pad_diff = exps-closest; 
                            bPassed = (fabs(pad_diff) <= 1);
                            effi->Fill(bPassed, exps);
                            if (bPassed) {diff->Fill(pad_diff);}
                        }
            }

            if(ID == 37 || ID == 38){
                         if (closest >0 && ID_index >=0 && h2y) {
                            pad_diff = exps-closest; 
                            bPassed = (fabs(pad_diff) <= 1);
                            effi->Fill(bPassed, exps);
                            if (bPassed) {diff->Fill(pad_diff);}
                        }
            }

        } 
    
        // for st3 and st4 pick only back partial tracklets and ask if they truely belongs to a H24 ray. Use H4X for the rest and H4Y2 for H4X.
        if(stID == 6 && chisq < 8){
            if(ID>38 && ID < 45){
            
                        if (closest >0 && ID_index >=0 && h4x) {
                            pad_diff = exps-closest; 
                            bPassed = (fabs(pad_diff) <= 1);
                            effi->Fill(bPassed, exps);
                            if (bPassed) {diff->Fill(pad_diff);}
                        }
            }

            if(ID == 45 || ID == 46){
                        if (closest >0 && ID_index >=0 && h4y2) {
                            pad_diff = exps-closest; 
                            bPassed = (fabs(pad_diff) <= 1);
                            effi->Fill(bPassed, exps);
                            if (bPassed) {diff->Fill(pad_diff);}
                        }
            }
       }
    }
    
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
    oss << "Distribution of exp - closest in detID :" << ID ;
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
