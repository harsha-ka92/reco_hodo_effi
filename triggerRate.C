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

    gStyle->SetOptStat(1);
    //gStyle->SetOptFit(1);

    TFile *f_file = TFile::Open("ana.root","READ");
    TTree *tr = (TTree*) f_file->Get("save");

   // choose the range of run numbers need to be analyzed and show up in the plots
   int xlow = 4685;
   int xhigh = 4702; 

   int run_num = 0;
   int run_time = 0;
   int nEvents;
   int event_ID;
   int event_ID_temp=0;
   int num_tls = 0;
   double nim1 = 0;
   double nim2 = 0;
   double nim3 = 0;
   double nim4 = 0;
   double matrix5 = 0;
   int i =0;
   int qual_tl = 0;

   int trigger, dor, run_ID, tlD0, tlD1, tlD2, tlD3p, tlD3m, nTracklets, tlBackPartial, tlGlobal;
   int tlD0_temp, tlD1_temp, tlD2_temp, tlD3m_temp, tlD3p_temp, tlBackPartial_temp, tlGlobal_temp;
   double nim1_max = 0.0;
   double nim2_max = 0.0;
   double nim3_max = 0.0;
   double nim4_max = 0.0;
   double matrix5_max = 0.0;

   double rnim1, rnim2, rnim3, rnim4, rmatrix5;

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("nTracklets", &nTracklets);
   tr->SetBranchAddress("dor", &dor);
   tr->SetBranchAddress("tlD0", &tlD0);
   tr->SetBranchAddress("tlD1", &tlD1);
   tr->SetBranchAddress("tlD2", &tlD2);
   tr->SetBranchAddress("tlD3p", &tlD3p);
   tr->SetBranchAddress("tlD3m", &tlD3m);
   tr->SetBranchAddress("tlBackPartial", &tlBackPartial);
   tr->SetBranchAddress("tlGlobal", &tlGlobal);

   TGraphAsymmErrors* gNIM1 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gNIM2 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gNIM3 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gNIM4 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gMatrix5 = new TGraphAsymmErrors();
   TGraphAsymmErrors* gruntime = new TGraphAsymmErrors();
   TGraphAsymmErrors* gtls = new TGraphAsymmErrors();
   TGraphAsymmErrors* gqtls = new TGraphAsymmErrors();
   TMultiGraph* mg = new TMultiGraph();

   nEvents = tr->GetEntries();
   std::cout << "Nevents = " << nEvents << std::endl;

for (int i_ent = 0; i_ent < tr->GetEntries(); i_ent++) {
      tr->GetEntry(i_ent);

      if(run_ID < xlow || run_ID > xhigh) {continue;}
      
      if(dor < 0) {run_num = run_ID; std::cout << "invalid dor"<<std::endl; continue;}
      
      if(run_num == run_ID && i_ent != nEvents-1){
         std::cout<< "analysis started"<<std::endl;
         if(trigger == 1) {nim1 +=1.0;}
         if(trigger == 2) {nim2 +=1.0;}
         if(trigger == 3) {nim3 +=1.0;}
         if(trigger == 4) {nim4 +=1.0;}
         if(trigger == 5) {matrix5 +=1.0;}
         tlD0_temp = tlD0; tlD1_temp = tlD1; tlD2_temp = tlD2; tlD3p_temp = tlD3p; tlD3m_temp = tlD3m; tlBackPartial_temp = tlBackPartial; tlGlobal_temp = tlGlobal;
         run_time = dor;
         run_num = run_ID;
         num_tls += nTracklets;
      }    
     else if (run_num == run_ID && i_ent == nEvents-1){
         std::cout<< "last event"<<std::endl;
         if(trigger == 1) {nim1 +=1.0;}
         if(trigger == 2) {nim2 +=1.0;}
         if(trigger == 3) {nim3 +=1.0;}
         if(trigger == 4) {nim4 +=1.0;}
         if(trigger == 5) {matrix5 +=1.0;}
         qual_tl += (tlD0_temp + tlD1_temp + tlD2_temp + tlD3p_temp + tlD3m_temp + tlBackPartial_temp + tlGlobal_temp);
         run_time = dor;
         run_num = run_ID;
         num_tls += nTracklets;

         rnim1 = nim1/run_time/60;
         rnim2 = nim2/run_time/60;
         rnim3 = nim3/run_time/60;
         rnim4 = nim4/run_time/60;
         rmatrix5 = matrix5/run_time/60;

         if(nim1_max < rnim1 ){ nim1_max = rnim1; }
         if(nim2_max < rnim2 ){ nim2_max = rnim2; }
         if(nim3_max < rnim3 ){ nim3_max = rnim3; }
         if(nim4_max < rnim4 ){ nim4_max = rnim4; }
         if(matrix5_max < rmatrix5 ){ matrix5_max = rmatrix5; }

         gNIM1->SetPoint(i, run_num, rnim1);
         gNIM1->SetPointError(i, 0., 0., 0., 0.);

         gNIM2->SetPoint(i, run_num, rnim2);
         gNIM2->SetPointError(i, 0., 0., 0., 0.);

         gNIM3->SetPoint(i, run_num, rnim3);
         gNIM3->SetPointError(i, 0., 0., 0., 0.);

         gNIM4->SetPoint(i, run_num, rnim4);
         gNIM4->SetPointError(i, 0., 0., 0., 0.);

         gMatrix5->SetPoint(i, run_num, rmatrix5);
         gMatrix5->SetPointError(i, 0., 0., 0., 0.);

         gruntime->SetPoint(i, run_num, run_time/60);
         gruntime->SetPointError(i, 0., 0., 0., 0.);

         std::cout<< "filling the value"<< num_tls <<std::endl;
         gtls->SetPoint(i, run_num, num_tls);
         gtls->SetPointError(i, 0., 0., 0., 0.);

         gqtls->SetPoint(i, run_num, qual_tl);
         gqtls->SetPointError(i, 0., 0., 0., 0.);

         nim1=0; nim2=0; nim3=0; nim4=0; matrix5=0, qual_tl =0, num_tls = 0;
         i++;
         std::cout << "i = " << i << std::endl;
         std::cout << "run # " << run_num << " nim1_max " << nim1_max<< std::endl;
         std::cout << "run # " << run_num << " nim3_max " << nim3_max<< std::endl;
         run_num = run_ID;
     }

     else {

           qual_tl += (tlD0_temp + tlD1_temp + tlD2_temp + tlD3p_temp + tlD3m_temp + tlBackPartial_temp + tlGlobal_temp);
           tlD0_temp = tlD0; tlD1_temp = tlD1; tlD2_temp = tlD2; tlD3p_temp = tlD3p; tlD3m_temp = tlD3m; tlBackPartial_temp = tlBackPartial; tlGlobal_temp = tlGlobal;
           std::cout<< "run number changed"<<std::endl;
           rnim1 = nim1/run_time/60;
           rnim2 = nim2/run_time/60;
           rnim3 = nim3/run_time/60;
           rnim4 = nim4/run_time/60;
           rmatrix5 = matrix5/run_time/60;

           if(nim1_max < rnim1 ){ nim1_max = rnim1; }
           if(nim2_max < rnim2 ){ nim2_max = rnim2; }
           if(nim3_max < rnim3 ){ nim3_max = rnim3; }
           if(nim4_max < rnim4 ){ nim4_max = rnim4; }
           if(matrix5_max < rmatrix5 ){ matrix5_max = rmatrix5; }

           gNIM1->SetPoint(i, run_num, rnim1);
           gNIM1->SetPointError(i, 0., 0., 0., 0.);

           gNIM2->SetPoint(i, run_num, rnim2);
           gNIM2->SetPointError(i, 0., 0., 0., 0.);

           gNIM3->SetPoint(i, run_num, rnim3);
           gNIM3->SetPointError(i, 0., 0., 0., 0.);

           gNIM4->SetPoint(i, run_num, rnim4);
           gNIM4->SetPointError(i, 0., 0., 0., 0.);

           gMatrix5->SetPoint(i, run_num, rmatrix5);
           gMatrix5->SetPointError(i, 0., 0., 0., 0.);

           gruntime->SetPoint(i, run_num, run_time/60);
           gruntime->SetPointError(i, 0., 0., 0., 0.);

           gtls->SetPoint(i, run_num, num_tls);
           gtls->SetPointError(i, 0., 0., 0., 0.);

           gqtls->SetPoint(i, run_num, qual_tl);
           gqtls->SetPointError(i, 0., 0., 0., 0.);

           nim1=0; nim2=0; nim3=0; nim4=0; matrix5=0, qual_tl =0, num_tls = 0;
           i++;
           std::cout << "i = " << i << std::endl;
           std::cout << "run # " << run_num << " nim1_max " << nim1_max<< std::endl;
           std::cout << "run # " << run_num << " nim3_max " << nim3_max<< std::endl;
           run_num = run_ID;
    }
}

    TCanvas* c1 = new TCanvas("c1", "", 1000, 500);

    gNIM1->SetTitle("Trigger Rate : NIM1");
    gNIM1->SetMarkerColor(4);
    gNIM1->SetMarkerStyle(43);
    gNIM1->SetMarkerSize(3);
    gNIM1->GetXaxis()->SetRangeUser(xlow,xhigh);
    gNIM1->GetYaxis()->SetRangeUser(0,1.05*nim1_max);
    gNIM1->GetXaxis()->SetTitle("run_ID");
    gNIM1->GetYaxis()->SetTitle("number of events/min");
    gNIM1->Draw("APE1");
    
    TCanvas* c2 = new TCanvas("c2", "", 1000, 500);

    gNIM2->SetTitle("Trigger Rate : NIM2");
    gNIM2->SetMarkerColor(4);
    gNIM2->SetMarkerStyle(43);
    gNIM2->SetMarkerSize(3);
    gNIM2->GetXaxis()->SetRangeUser(xlow,xhigh);
    gNIM2->GetYaxis()->SetRangeUser(0,1.05*nim2_max);
    gNIM2->GetXaxis()->SetTitle("run_ID");
    gNIM2->GetYaxis()->SetTitle("number of events/min");
    gNIM2->Draw("APE1");

    TCanvas* c3 = new TCanvas("c3", "", 1000, 500);

    gNIM3->SetTitle("Trigger Rate : NIM3");
    gNIM3->SetMarkerColor(4);
    gNIM3->SetMarkerStyle(43);
    gNIM3->SetMarkerSize(3);
    gNIM3->GetXaxis()->SetRangeUser(xlow,xhigh);
    gNIM3->GetYaxis()->SetRangeUser(0,1.05*nim3_max);
    gNIM3->GetXaxis()->SetTitle("run_ID");
    gNIM3->GetYaxis()->SetTitle("number of events/min");
    gNIM3->Draw("APE1");

    TCanvas* c4 = new TCanvas("c4", "", 1000, 500);

    gNIM4->SetTitle("Trigger Rate : NIM4");
    gNIM4->SetMarkerColor(4);
    gNIM4->SetMarkerStyle(43);
    gNIM4->SetMarkerSize(3);
    gNIM4->GetXaxis()->SetRangeUser(xlow,xhigh);
    gNIM4->GetYaxis()->SetRangeUser(0,1.05*nim4_max);
    gNIM4->GetXaxis()->SetTitle("run_ID");
    gNIM4->GetYaxis()->SetTitle("number of events/min");
    gNIM4->Draw("APE1");

    TCanvas* c5 = new TCanvas("c5", "", 1000, 500);

    gMatrix5->SetTitle("Trigger Rate : Matrix5");
    gMatrix5->SetMarkerColor(4);
    gMatrix5->SetMarkerStyle(43);
    gMatrix5->SetMarkerSize(3);
    gMatrix5->GetXaxis()->SetRangeUser(xlow,xhigh);
    gMatrix5->GetYaxis()->SetRangeUser(0,1.05*matrix5_max);
    gMatrix5->GetXaxis()->SetTitle("run_ID");
    gMatrix5->GetYaxis()->SetTitle("number of events/min");
    gMatrix5->Draw("APE1");

    TCanvas* c6 = new TCanvas("c6", "", 1000, 500);

    gruntime->SetTitle("run lengths");
    gruntime->SetMarkerColor(4);
    gruntime->SetMarkerStyle(43);
    gruntime->SetMarkerSize(3);
    gruntime->GetXaxis()->SetRangeUser(xlow,xhigh);
    gruntime->GetXaxis()->SetTitle("run_ID");
    gruntime->GetYaxis()->SetTitle("length of the run (mins)");
    gruntime->Draw("APE1");

    TCanvas* c7 = new TCanvas("c7", "", 1000, 500);

    gqtls->SetTitle("Number of qualified tracklets  in the run");
    gqtls->SetMarkerColor(4);
    gqtls->SetMarkerStyle(21);
    //gqtls->SetMarkerSize(3);
    gqtls->GetXaxis()->SetRangeUser(xlow,xhigh);
    //gqtls->GetYaxis()->SetRangeUser(0,5);
    gqtls->GetXaxis()->SetTitle("run_ID");
    gqtls->GetYaxis()->SetTitle("Number of qualified tracklets");
    gtls->SetTitle("Total number of tracklets in the run");
    gtls->GetXaxis()->SetRangeUser(xlow,xhigh);
    gtls->SetMarkerColor(2);
    gtls->SetMarkerStyle(22);
    //gtls->SetMarkerSize(3);
    gtls->Draw("APE1");
    mg -> Add(gtls);
    mg-> Add(gqtls);
    mg->GetXaxis()->SetRangeUser(xlow,xhigh);
    mg->Draw("APE1");
    c7->BuildLegend(0.2, 0.6, 0.5, 0.8, "", "");

    c1->SaveAs("triggerRates/rNIM1.png");
    c2->SaveAs("triggerRates/rNIM2.png");
    c3->SaveAs("triggerRates/rNIM3.png");
    c4->SaveAs("triggerRates/rNIM4.png");
    c5->SaveAs("triggerRates/rMatrix5.png");
    c6->SaveAs("triggerRates/run_times.png");
    c7->SaveAs("triggerRates/tracklet_info.png");
}
