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

void hits()
{
    gSystem->mkdir("hitRates", 1);

    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    TFile *f_file = TFile::Open("ana.root","READ");
    TTree *tr = (TTree*) f_file->Get("save");

   // choose the range of run numbers need to be analyzed and show up in the plots
   int xlow = 4685;
   int xhigh = 4702; 

   int run_num = 4690;
   int run_time = 0;
   double dor;
   int nEvents;
   int event_ID;
   int event_ID_temp=0;
   int i =0;
   int qual_tl = 0;
   int run_ID;

   int num_h1t;
   int num_h1b;
   int num_h1r;
   int num_h1l;
   int num_h2t;
   int num_h2b;
   int num_h2r;
   int num_h2l;
   int num_h3t;
   int num_h3b;

   double t_num_h1 =0;    double t_num_h2 =0;    double t_num_h3 =0;
   double rh1_max =0;    double rh2_max =0;     double rh3_max =0; 
   double rh1 =0;    double rh2 =0;     double rh3 =0; 
   
   std::vector<double> *tdc_h1t =0;
   std::vector<double> *tdc_h1b =0;
   std::vector<double> *tdc_h1r =0;
   std::vector<double> *tdc_h1l =0;
   std::vector<double> *tdc_h2t =0;
   std::vector<double> *tdc_h2b =0;
   std::vector<double> *tdc_h2r =0;
   std::vector<double> *tdc_h2l =0;
   std::vector<double> *tdc_h3t =0;
   std::vector<double> *tdc_h3b =0;

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("dor", &dor);

   tr->SetBranchAddress("tdc_h1t", &tdc_h1t);
   tr->SetBranchAddress("tdc_h1b", &tdc_h1b);
   tr->SetBranchAddress("tdc_h1r", &tdc_h1r);
   tr->SetBranchAddress("tdc_h1l", &tdc_h1l);

   tr->SetBranchAddress("tdc_h2t", &tdc_h2t);
   tr->SetBranchAddress("tdc_h2b", &tdc_h2b);
   tr->SetBranchAddress("tdc_h2r", &tdc_h2r);
   tr->SetBranchAddress("tdc_h2l", &tdc_h2l);

   tr->SetBranchAddress("tdc_h3t", &tdc_h3t);
   tr->SetBranchAddress("tdc_h3b", &tdc_h3b);

   TH1F *htdc_h1t = new TH1F("htdc_h1t","htdc_h1t", 400, 800, 1200);
   TH1F *htdc_h1b = new TH1F("htdc_h1b","htdc_h1b", 400, 800, 1200);
   TH1F *htdc_h1r = new TH1F("htdc_h1r","htdc_h1r", 400, 800, 1200);
   TH1F *htdc_h1l = new TH1F("htdc_h1l","htdc_h1l", 400, 800, 1200);

   TH1F *htdc_h2t = new TH1F("htdc_h2t","htdc_h2t", 400, 800, 1200);
   TH1F *htdc_h2b = new TH1F("htdc_h2b","htdc_h2b", 400, 800, 1200);
   TH1F *htdc_h2r = new TH1F("htdc_h2r","htdc_h2r", 400, 800, 1200);
   TH1F *htdc_h2l = new TH1F("htdc_h2l","htdc_h2l", 400, 800, 1200);

   TH1F *htdc_h3t = new TH1F("htdc_h3t","htdc_h3t", 400, 800, 1200);
   TH1F *htdc_h3b = new TH1F("htdc_h3b","htdc_h3b", 400, 800, 1200);

   TGraphAsymmErrors* grh1 = new TGraphAsymmErrors();
   TGraphAsymmErrors* grh2 = new TGraphAsymmErrors();
   TGraphAsymmErrors* grh3 = new TGraphAsymmErrors();

   TGraphAsymmErrors* gtdc_h1t = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h1b = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h1r = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h1l = new TGraphAsymmErrors();

   TGraphAsymmErrors* gtdc_h2t = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h2b = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h2r = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h2l = new TGraphAsymmErrors();

   TGraphAsymmErrors* gtdc_h3t = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtdc_h3b = new TGraphAsymmErrors();

   //TMultiGraph* mg = new TMultiGraph();

   nEvents = tr->GetEntries();

for (int i_ent = 0; i_ent < tr->GetEntries(); i_ent++) {
      tr->GetEntry(i_ent);
    
      if(run_ID < xlow || run_ID > xhigh) {continue;}
      
      //if(dor < 0) {run_num = run_ID; std::cout << "invalid dor"<<std::endl; continue;}
      
      if(run_num == run_ID && i_ent != nEvents-1){
        
        for ( int j =0; j< tdc_h1t->size(); j++){
            htdc_h1t->Fill(tdc_h1t->at(j));
        }
        for ( int j =0; j< tdc_h1b->size(); j++){
            htdc_h1b->Fill(tdc_h1b->at(j));
        }
        for ( int j =0; j< tdc_h1r->size(); j++){
            htdc_h1r->Fill(tdc_h1r->at(j));
        }
        for ( int j =0; j< tdc_h1l->size(); j++){
            htdc_h1l->Fill(tdc_h1l->at(j));
        }

        for ( int j =0; j< tdc_h2t->size(); j++){
            htdc_h2t->Fill(tdc_h2t->at(j));
        }
        for ( int j =0; j< tdc_h2b->size(); j++){
            htdc_h2b->Fill(tdc_h2b->at(j));
        }
        for ( int j =0; j< tdc_h2r->size(); j++){
            htdc_h2r->Fill(tdc_h2r->at(j));
        }
        for ( int j =0; j< tdc_h2l->size(); j++){
            htdc_h2l->Fill(tdc_h2l->at(j));
        }

        for ( int j =0; j< tdc_h3t->size(); j++){
            htdc_h3t->Fill(tdc_h3t->at(j));
        }
        for ( int j =0; j< tdc_h3b->size(); j++){
            htdc_h3b->Fill(tdc_h3b->at(j));
        }

        t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
        t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
        t_num_h3 += (num_h3t + num_h3b);

         run_time = dor;
         run_num = run_ID;
         std::cout<<run_ID<<std::endl;
      }    
     else if (run_num == run_ID && i_ent == nEvents-1){
         std::cout<< "Filling the last event"<<std::endl;

            for ( int j =0; j< tdc_h1t->size(); j++){
                htdc_h1t->Fill(tdc_h1t->at(j));
            }
            for ( int j =0; j< tdc_h1b->size(); j++){
                htdc_h1b->Fill(tdc_h1b->at(j));
            }
            for ( int j =0; j< tdc_h1r->size(); j++){
                htdc_h1r->Fill(tdc_h1r->at(j));
            }
            for ( int j =0; j< tdc_h1l->size(); j++){
                htdc_h1l->Fill(tdc_h1l->at(j));
            }

            for ( int j =0; j< tdc_h2t->size(); j++){
            htdc_h2t->Fill(tdc_h2t->at(j));
            }
            for ( int j =0; j< tdc_h2b->size(); j++){
                htdc_h2b->Fill(tdc_h2b->at(j));
            }
            for ( int j =0; j< tdc_h2r->size(); j++){
                htdc_h2r->Fill(tdc_h2r->at(j));
            }
            for ( int j =0; j< tdc_h2l->size(); j++){
                htdc_h2l->Fill(tdc_h2l->at(j));
            }

            for ( int j =0; j< tdc_h3t->size(); j++){
                htdc_h3t->Fill(tdc_h3t->at(j));
            }
            for ( int j =0; j< tdc_h3b->size(); j++){
                htdc_h3b->Fill(tdc_h3b->at(j));
            }

            t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
            t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
            t_num_h3 += (num_h3t + num_h3b);

           rh1 = t_num_h1/run_time/60;
           rh2 = t_num_h2/run_time/60;
           rh3 = t_num_h3/run_time/60;
    
           if(rh1_max < rh1 ){ rh1_max = rh1; }
           if(rh2_max < rh2 ){ rh2_max = rh2; }
           if(rh3_max < rh3 ){ rh3_max = rh3; }

           grh1->SetPoint(i, run_num, rh1);
           grh1->SetPointError(i, 0., 0., 0., 0.);

           grh2->SetPoint(i, run_num, rh2);
           grh2->SetPointError(i, 0., 0., 0., 0.);

           grh3->SetPoint(i, run_num, rh3);
           grh3->SetPointError(i, 0., 0., 0., 0.);

           //rh1=0; rh2=0; rh3=0;
           run_num = run_ID;
     }

     else {
           std::cout<< "run number changed"<<std::endl;
           rh1 = t_num_h1/run_time/60;
           rh2 = t_num_h2/run_time/60;
           rh3 = t_num_h3/run_time/60;
    
           if(rh1_max < rh1 ){ rh1_max = rh1; }
           if(rh2_max < rh2 ){ rh2_max = rh2; }
           if(rh3_max < rh3 ){ rh3_max = rh3; }

           /*int bmxh1t = htdc_h1t->GetMaximumBin(); double tdc_max_h1t = htdc_h1t->GetXaxis()->GetBinCenter(bmxh1t);
           int bmnh1t = htdc_h1t->GetMinimumBin(); double tdc_min_h1t = htdc_h1t->GetXaxis()->GetBinCenter(bmnh1t);
           int bmxh1b = htdc_h1b->GetMaximumBin(); double tdc_max_h1b = htdc_h1b->GetXaxis()->GetBinCenter(bmxh1b);
           int bmnh1b = htdc_h1b->GetMinimumBin(); double tdc_min_h1b = htdc_h1b->GetXaxis()->GetBinCenter(bmnh1b);
           int bmxh1l = htdc_h1l->GetMaximumBin(); double tdc_max_h1l = htdc_h1l->GetXaxis()->GetBinCenter(bmxh1l);
           int bmnh1l = htdc_h1l->GetMinimumBin(); double tdc_min_h1l = htdc_h1l->GetXaxis()->GetBinCenter(bmnh1l);
           int bmxh1r = htdc_h1r->GetMaximumBin(); double tdc_max_h1r = htdc_h1r->GetXaxis()->GetBinCenter(bmxh1r);
           int bmnh1r = htdc_h1r->GetMinimumBin(); double tdc_min_h1r = htdc_h1r->GetXaxis()->GetBinCenter(bmnh1r);

           int bmxh2t = htdc_h2t->GetMaximumBin(); double tdc_max_h2t = htdc_h2t->GetXaxis()->GetBinCenter(bmxh2t);
           int bmnh2t = htdc_h2t->GetMinimumBin(); double tdc_min_h2t = htdc_h2t->GetXaxis()->GetBinCenter(bmnh2t);
           int bmxh2b = htdc_h2b->GetMaximumBin(); double tdc_max_h2b = htdc_h2b->GetXaxis()->GetBinCenter(bmxh2b);
           int bmnh2b = htdc_h2b->GetMinimumBin(); double tdc_min_h2b = htdc_h2b->GetXaxis()->GetBinCenter(bmnh2b);
           int bmxh2l = htdc_h2l->GetMaximumBin(); double tdc_max_h2l = htdc_h2l->GetXaxis()->GetBinCenter(bmxh2l);
           int bmnh2l = htdc_h2l->GetMinimumBin(); double tdc_min_h2l = htdc_h2l->GetXaxis()->GetBinCenter(bmnh2l);
           int bmxh2r = htdc_h2r->GetMaximumBin(); double tdc_max_h2r = htdc_h2r->GetXaxis()->GetBinCenter(bmxh2r);
           int bmnh2r = htdc_h2r->GetMinimumBin(); double tdc_min_h2r = htdc_h2r->GetXaxis()->GetBinCenter(bmnh2r);

           int bmxh3t = htdc_h3t->GetMaximumBin(); double tdc_max_h3t = htdc_h3t->GetXaxis()->GetBinCenter(bmxh3t);
           int bmnh3t = htdc_h3t->GetMinimumBin(); double tdc_min_h3t = htdc_h3t->GetXaxis()->GetBinCenter(bmnh3t);
           int bmxh3b = htdc_h3b->GetMaximumBin(); double tdc_max_h3b = htdc_h3b->GetXaxis()->GetBinCenter(bmxh3b);
           int bmnh3b = htdc_h3b->GetMinimumBin(); double tdc_min_h3b = htdc_h3b->GetXaxis()->GetBinCenter(bmnh3b);*/

           grh1->SetPoint(i, run_num, rh1);
           grh1->SetPointError(i, 0., 0., 0., 0.);

           grh2->SetPoint(i, run_num, rh2);
           grh2->SetPointError(i, 0., 0., 0., 0.);

           grh3->SetPoint(i, run_num, rh3);
           grh3->SetPointError(i, 0., 0., 0., 0.);

           rh1=0; rh2=0; rh3=0;
           i++;
           std::cout << "i = " << i << std::endl;
           run_num = run_ID;
           run_time = dor;
           t_num_h1 = 0; t_num_h2 = 0; t_num_h3 = 0;
           htdc_h1t->Reset("ICESM"); htdc_h1b->Reset("ICESM"); htdc_h1l->Reset("ICESM"); htdc_h1r->Reset("ICESM"); 
           htdc_h2t->Reset("ICESM"); htdc_h2b->Reset("ICESM"); htdc_h2l->Reset("ICESM"); htdc_h2r->Reset("ICESM"); 
           htdc_h3t->Reset("ICESM"); htdc_h3b->Reset("ICESM"); 

           for ( int j =0; j< tdc_h1t->size(); j++){
                htdc_h1t->Fill(tdc_h1t->at(j));
            }
           for ( int j =0; j< tdc_h1b->size(); j++){
                htdc_h1b->Fill(tdc_h1b->at(j));
            }
            for ( int j =0; j< tdc_h1r->size(); j++){
                htdc_h1r->Fill(tdc_h1r->at(j));
            }
            for ( int j =0; j< tdc_h1l->size(); j++){
                htdc_h1l->Fill(tdc_h1l->at(j));
            }

            for ( int j =0; j< tdc_h2t->size(); j++){
            htdc_h2t->Fill(tdc_h2t->at(j));
            }
            for ( int j =0; j< tdc_h2b->size(); j++){
                htdc_h2b->Fill(tdc_h2b->at(j));
            }
            for ( int j =0; j< tdc_h2r->size(); j++){
                htdc_h2r->Fill(tdc_h2r->at(j));
            }
            for ( int j =0; j< tdc_h2l->size(); j++){
                htdc_h2l->Fill(tdc_h2l->at(j));
            }

            for ( int j =0; j< tdc_h3t->size(); j++){
                htdc_h3t->Fill(tdc_h3t->at(j));
            }
            for ( int j =0; j< tdc_h3b->size(); j++){
                htdc_h3b->Fill(tdc_h3b->at(j));
            }
            t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
            t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
            t_num_h3 += (num_h3t + num_h3b);
    }
}

    TCanvas* c1 = new TCanvas("tdc_time: St1", "tdc_time: St1", 1000, 500);
    c1->SetFillColor(18);
    htdc_h1t->SetLineColor(7);
    htdc_h1t->Draw();
    htdc_h1b->SetLineColor(2);
    htdc_h1b->Draw("SAME");
    htdc_h1r->SetLineColor(3);
    htdc_h1r->Draw("SAME");
    htdc_h1l->SetLineColor(4);
    htdc_h1l->Draw("SAME");
    /*htdc_h1l->SetTitle("");
    htdc_h1l->GetXaxis()->SetTitle("tdc_time");
    htdc_h1l->GetYaxis()->SetTitle("number of events");*/
    c1->Update();
    
    TCanvas* c2 = new TCanvas("tdc_time: St2", "tdc_time: St2", 1000, 500);
    htdc_h2t->SetLineColor(7);
    htdc_h2t->Draw();
    htdc_h2b->SetLineColor(2);
    htdc_h2b->Draw("SAME");
    htdc_h2r->SetLineColor(3);
    htdc_h2r->Draw("SAME");
    htdc_h2l->SetLineColor(4);
    htdc_h2l->Draw("SAME");
    /*htdc_h2l->SetTitle("tdc_time");
    htdc_h2l->GetXaxis()->SetTitle("tdc_time");
    htdc_h2l->GetYaxis()->SetTitle("number of events");*/
    c2->Update();

    TCanvas* c3 = new TCanvas("tdc_time: St3", "tdc_time: St3", 1000, 500);
    htdc_h3t->SetLineColor(7);
    htdc_h3t->Draw();
    htdc_h3b->SetLineColor(2);
    htdc_h3b->Draw("SAME");
    /*htdc_h3b->SetTitle("tdc_time");
    htdc_h3b->GetXaxis()->SetTitle("tdc_time");
    htdc_h3b->GetYaxis()->SetTitle("number of events");*/
    c3->Update();

    TCanvas* c4 = new TCanvas("c4", "", 1000, 500);

    grh1->SetTitle("Hit Rate : Station 1");
    grh1->SetMarkerColor(4);
    grh1->SetMarkerStyle(43);
    grh1->SetMarkerSize(3);
    grh1->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    grh1->GetYaxis()->SetLimits(0,1.05*rh1_max);
    grh1->GetXaxis()->SetTitle("run_ID");
    grh1->GetYaxis()->SetTitle("number of hits/min");
    grh1->Draw("APE1");
    c4->Update();

    TCanvas* c5 = new TCanvas("c5", "", 1000, 500);

    grh2->SetTitle("Hit Rate : Station 2");
    grh2->SetMarkerColor(4);
    grh2->SetMarkerStyle(43);
    grh2->SetMarkerSize(3);
    grh2->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    grh2->GetYaxis()->SetLimits(0,1.05*rh2_max);
    grh2->GetXaxis()->SetTitle("run_ID");
    grh2->GetYaxis()->SetTitle("number of hits/min");
    grh2->Draw("APE1");
    c5->Update();

    TCanvas* c6 = new TCanvas("c6", "", 1000, 500);

    grh3->SetTitle("Hit Rate : Station 3");
    grh3->SetMarkerColor(4);
    grh3->SetMarkerStyle(43);
    grh3->SetMarkerSize(3);
    grh3->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    grh3->GetYaxis()->SetLimits(0,1.05*rh3_max);
    grh3->GetXaxis()->SetTitle("run_ID");
    grh3->GetYaxis()->SetTitle("number of hits/min");
    grh3->Draw("APE1");
    c6->Update();
    
/*
    TCanvas* c7 = new TCanvas("c7", "", 1000, 500);

    gqtls->SetTitle("Number of qualified tracklets  in the run");
    gqtls->SetMarkerColor(4);
    gqtls->SetMarkerStyle(21);
    //gqtls->SetMarkerSize(3);
    //gqtls->GetXaxis()->SSetLimits(xlow-0.5,xhigh);
    //gqtls->GetYaxis()->SetLimits(0,360);
    gqtls->GetXaxis()->SetTitle("run_ID");
    gqtls->GetYaxis()->SetTitle("Number of qualified tracklets");
    gtls->SetTitle("Total number of tracklets in the run");
    //gtls->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    //gtls->GetYaxis()->SetLimits(0,360);
    gtls->SetMarkerColor(2);
    gtls->SetMarkerStyle(22);
    //gtls->SetMarkerSize(3);
    gtls->Draw("APE1");
    mg -> Add(gtls);
    mg-> Add(gqtls);
    mg->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    mg->SetMinimum(0);
    mg->SetMaximum(360);
    mg->Draw("APE1");
    c7->BuildLegend(0.45, 0.6, 0.8, 0.8, "", "");

*/

    c1->SaveAs("hitRates/tdc_h1.png");
    c2->SaveAs("hitRates/tdc_h2.png");
    c3->SaveAs("hitRates/tdc_h3.png");
    c4->SaveAs("hitRates/rh1.png");
    c5->SaveAs("hitRates/rh2.png");
    c6->SaveAs("hitRates/rh3.png");
    //c7->SaveAs("triggerRates/tracklet_info.png");
}
