#ifndef _hits__H_
#define _hits__H_

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

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

   int run_time = 0;
   int dor;
   int nEvents;
   int event_ID;
   int event_ID_temp=0;
   int i =0;
   int qual_tl = 0;
   int run_ID;
   int trigger;
   int stID;
   std::string trigger_temp = "All";

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
   int tlBackPartial;
   int total_M5 = 0;
   int total_N1 =0; int total_N2 =0; int total_N3 =0; int total_N4 =0; 
   int has_hits =0; int no_hits =0; int total_bp =0;
   int st12 =0; int st24 =0;
   int h3b_hits = 0; int no_h3b_hits = 0; int h3t_hits = 0; int no_h3t_hits = 0;
   //int h3b_hits = 0; int no_h3b_hits = 0; int h3t_hits = 0; int no_h3t_hits = 0;
   
   

   double t_num_h1 =0;    double t_num_h2 =0;    double t_num_h3 =0;   double t_num_h4=0;
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

   TH1F *htdc_h1t = new TH1F("htdc_h1t","htdc_h1t", 350, 750, 1100);
   TH1F *htdc_h1b = new TH1F("htdc_h1b","htdc_h1b", 350, 750, 1100);
   TH1F *htdc_h1r = new TH1F("htdc_h1r","htdc_h1r", 350, 750, 1100);
   TH1F *htdc_h1l = new TH1F("htdc_h1l","htdc_h1l", 350, 750, 1100);

   TH1F *htdc_h2t = new TH1F("htdc_h2t","htdc_h2t", 350, 750, 1100);
   TH1F *htdc_h2b = new TH1F("htdc_h2b","htdc_h2b", 350, 750, 1100);
   TH1F *htdc_h2r = new TH1F("htdc_h2r","htdc_h2r", 350, 750, 1100);
   TH1F *htdc_h2l = new TH1F("htdc_h2l","htdc_h2l", 350, 750, 1100);

   TH1F *htdc_h3t = new TH1F("htdc_h3t","htdc_h3t", 300, 900, 1200);
   TH1F *htdc_h3b = new TH1F("htdc_h3b","htdc_h3b", 300, 900, 1200);

   TH1F *htdc_h4t = new TH1F("htdc_h4t","htdc_h4t", 350, 750, 1100);
   TH1F *htdc_h4b = new TH1F("htdc_h4b","htdc_h4b", 350, 750, 1100);
   TH1F *htdc_h4y1r = new TH1F("htdc_h4y1r","htdc_h4y1r", 350, 750, 1100);
   TH1F *htdc_h4y1l = new TH1F("htdc_h4y1l","htdc_h4y1l", 350, 750, 1100);
   TH1F *htdc_h4y2r = new TH1F("htdc_h4y2r","htdc_h4y2r", 350, 750, 1100);
   TH1F *htdc_h4y2l = new TH1F("htdc_h4y2l","htdc_h4y2l", 350, 750, 1100);

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

   auto hs1 = new THStack("hs1","");
   auto hs2 = new THStack("hs2","");
   auto hs3 = new THStack("hs2","");

#endif
