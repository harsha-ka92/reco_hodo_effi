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
int trigger_tls;

//ref to keep track of the entry from the tls tree
int tls_entry = 0; 

//select the padd:diff: cut
int diff_cut = 0;

TH1D *diff = new TH1D("diff","difff", 2*diff_cut + 1, -diff_cut - 0.5, diff_cut + 0.5);

//select X or Y planes
bool X_planes = false;
bool Y_planes = false;

//select the planes
bool H1 = true;
bool H2 = true;
bool H3 = true;
bool H4 = true;

void paddleEffi()
{
    gSystem->mkdir("effi", 1);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

    f_file = TFile::Open("ana.root","READ");
    tr_tls = (TTree*) f_file->Get("tls");

    string trigger = "NIM1";

    tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
    tr_tls->SetBranchAddress("detIDs", &detIDs);
    tr_tls->SetBranchAddress("trigger", &trigger_tls);
    tr_tls->SetBranchAddress("stID", &stID);
    tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
    tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);
    tr_tls->SetBranchAddress("chisq", &chisq);
    tr_tls->SetBranchAddress("nHits", &nHits);

    if(X_planes){ trigger = "NIM1"
        //H1
        if(H1){
            getEffi(tr_tls, 31, 23, diff_cut, trigger); 
            getEffi(tr_tls, 32, 23, diff_cut, trigger); 
        }

        //H2
        if(H2){
            getEffi(tr_tls, 37, 16, diff_cut, trigger); 
            getEffi(tr_tls, 38, 16, diff_cut, trigger);
        }

        //H3
        if(H3){
            getEffi(tr_tls, 39, 16, diff_cut, trigger); 
            getEffi(tr_tls, 40, 16, diff_cut, trigger);
        }

        //H4
        if(H4){
            getEffi(tr_tls, 45, 16, diff_cut, trigger); 
            getEffi(tr_tls, 46, 16, diff_cut, trigger);
        }
    }

    if(Y_planes){ trigger = "NIM2"
        //H1
        if(H1){
            getEffi(tr_tls, 33, 20, diff_cut, trigger); 
            getEffi(tr_tls, 34, 20, diff_cut, trigger);
        }

        //H2
        if(H2){
            getEffi(tr_tls, 35, 19, diff_cut, trigger); 
            getEffi(tr_tls, 36, 19, diff_cut, trigger); 
        }

        //H4
        if(H4){
            getEffi(tr_tls, 41, 16, diff_cut, trigger); 
            getEffi(tr_tls, 42, 16, diff_cut, trigger); 
            getEffi(tr_tls, 43, 16, diff_cut, trigger); 
            getEffi(tr_tls, 44, 16, diff_cut, trigger);  
        }
    }

}

void getEffi( TTree* tlsTree, int ID, int nPaddles, int cut, string trigger){

    TEfficiency* effi = new TEfficiency("effi", "effi", nPaddles+1, 0.5, nPaddles+0.5);
    ostringstream oss;
    oss<< "efficiencies of the paddles of detID "<< ID ;

    std::cout<<"Analyzing the stID :"<<ID<<std::endl;
    std::cout<< "paddel difference cut :" << cut<<endl;

    for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); ++i_tls_entry){

      if (i_tls_entry % 70000 == 0) cout << " . " << flush;

      tr_tls->GetEntry(i_tls_entry);

      //exclude the stIDs that are not considering for the analysis
      if (stID != 6 || chisq <8) {continue;}

      //use NIM 1 events to get X plabe efficiencies. And NIM 2 events to get Y plane efficiencies.
      if (ID <= 30) {std::cout<<"Invalid station ID"<<std::endl;}
      
      if (trigger == "NIM1" && (trigger_tls & 0x1) == 0 ) {continue;} //select NIM1
      if (trigger == "NIM2" && (trigger_tls & 0x2) == 0 ) {continue;} //select NIM2
      
      if (ID >= 47) {std::cout<<"Invalid station ID"<<std::endl;}

      //check for valid expected and coresponding closest hits in planes
            for ( int j =0; j< detIDs->size(); j++){
                    if(detIDs->at(j) == ID){ 
                       int exps= eleID_exps->at(j); 
                        int closest = eleID_closests->at(j);
                        int pad_diff = exps-closest; 
                        bool bPassed = (fabs(pad_diff) <= cut);
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
    diff->Reset("ICESM");
    std::cout<<std::endl;
}
