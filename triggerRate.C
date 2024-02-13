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

    TFile *f_file = TFile::Open("ana.root","READ");
    TTree *tr = (TTree*) f_file->Get("save");

   int run_num = 4796;
   int run_time = 0;
   double nim1 = 0;
   double nim2 = 0;
   double nim3 = 0;
   double nim4 = 0;
   double matrix5 = 0;
   int i =0;

   int trigger, dor, run_ID;

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("dor", &dor);

   TGraphAsymmErrors* gNIM1 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gNIM2 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gNIM3 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gNIM4 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gMatrix5 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gruntime = new TGraphAsymmErrors();
   
   for (int i_ent = 0; i_ent <tr->GetEntries(); i_ent++) {
      tr->GetEntry(i_ent);

      if(dor<0) {run_num = run_ID; std::cout << "invalid dor"<<std::endl; continue;}
      
      if(run_num == run_ID){
         if(trigger == 1) {nim1 +=1.0;}
         if(trigger == 2) {nim2 +=1.0;}
         if(trigger == 3) {nim3 +=1.0;}
         if(trigger == 4) {nim4 +=1.0;}
         if(trigger == 5) {matrix5 +=1.0;}
         run_time = dor;
         run_num = run_ID;
      }
      else{
         gNIM1->SetPoint(i, run_num, nim1/run_time/60);
         gNIM1->SetPointError(i, 0., 0., 0., 0.);

         gNIM2->SetPoint(i, run_num, nim2/run_time/60);
         gNIM2->SetPointError(i, 0., 0., 0., 0.);

         gNIM3->SetPoint(i, run_num, nim3/run_time/60);
         gNIM3->SetPointError(i, 0., 0., 0., 0.);

         gNIM4->SetPoint(i, run_num, nim4/run_time/60);
         gNIM4->SetPointError(i, 0., 0., 0., 0.);

         gMatrix5->SetPoint(i, run_num, matrix5/run_time/60);
         gMatrix5->SetPointError(i, 0., 0., 0., 0.);

         gruntime->(i, run_num, run_time/60);
         gruntime->SetPointError(i, 0., 0., 0., 0.);

         nim1=0; nim2=0; nim3=0; nim4=0; matrix5=0; 
         run_num = run_ID;
         i++;
        //std::cout << "i = " << i << std::endl;
        //std::cout << "run # " << run_num << " nim1/dor " << nim1/dor << std::endl;
      }
   }
    TCanvas* c1 = new TCanvas("c1", "", 1000, 500);

    gNIM1->SetTitle("Trigger Rate : NIM1");
    gNIM1->SetMarkerColor(4);
    gNIM1->SetMarkerStyle(43);
    gNIM1->SetMarkerSize(3);
    gNIM1->GetXaxis()->SetTitle("run_ID");
    gNIM1->GetYaxis()->SetTitle("number of events/min");
    gNIM1->Draw("APE1");
    
    TCanvas* c2 = new TCanvas("c2", "", 1000, 500);

    gNIM2->SetTitle("Trigger Rate : NIM2");
    gNIM2->SetMarkerColor(4);
    gNIM2->SetMarkerStyle(43);
    gNIM2->SetMarkerSize(3);
    gNIM2->GetXaxis()->SetTitle("run_ID");
    gNIM2->GetYaxis()->SetTitle("number of events/min");
    gNIM2->Draw("APE1");

    TCanvas* c3 = new TCanvas("c3", "", 1000, 500);

    gNIM3->SetTitle("Trigger Rate : NIM3");
    gNIM3->SetMarkerColor(4);
    gNIM3->SetMarkerStyle(43);
    gNIM3->SetMarkerSize(3);
    gNIM3->GetXaxis()->SetTitle("run_ID");
    gNIM3->GetYaxis()->SetTitle("number of events/min");
    gNIM3->Draw("APE1");

    TCanvas* c4 = new TCanvas("c4", "", 1000, 500);

    gNIM4->SetTitle("Trigger Rate : NIM4");
    gNIM4->SetMarkerColor(4);
    gNIM4->SetMarkerStyle(43);
    gNIM4->SetMarkerSize(3);
    gNIM4->GetXaxis()->SetTitle("run_ID");
    gNIM4->GetYaxis()->SetTitle("number of events/min");
    gNIM4->Draw("APE1");

    TCanvas* c5 = new TCanvas("c5", "", 1000, 500);

    gMatrix5->SetTitle("Trigger Rate : Matrix5");
    gMatrix5->SetMarkerColor(4);
    gMatrix5->SetMarkerStyle(43);
    gMatrix5->SetMarkerSize(3);
    gMatrix5->GetXaxis()->SetTitle("run_ID");
    gMatrix5->GetYaxis()->SetTitle("number of events/min");
    gMatrix5->Draw("APE1");

    TCanvas* c6 = new TCanvas("c6", "", 1000, 500);

    gruntime->SetTitle("Duration of runs");
    gruntime->SetMarkerColor(4);
    gruntime5->SetMarkerStyle(43);
    gruntime->SetMarkerSize(3);
    gruntime->GetXaxis()->SetTitle("run_ID");
    gruntime->GetYaxis()->SetTitle("Duration of the run (mins)");
    gruntime->Draw("APE1");

    c1->SaveAs("triggerRates/rNIM1.png");
    c2->SaveAs("triggerRates/rNIM2.png");
    c3->SaveAs("triggerRates/rNIM3.png");
    c4->SaveAs("triggerRates/rNIM4.png");
    c5->SaveAs("triggerRates/rMatrix5.png");
    c6->SaveAs("triggerRates/run_times.png");
}
