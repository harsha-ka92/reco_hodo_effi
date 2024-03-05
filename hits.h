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
   int num_h4t; int num_h4b; int num_h4y1r; int num_h4y1l; int num_h4y2r; int num_h4y2l;
   int st24w3=0;
   int tlBackPartial;
   int total_M5 = 0;
   int total_N1 =0; int total_N2 =0; int total_N3 =0; int total_N4 =0; int new_trigger =0;
   int has_hits =0; int no_hits =0; int total_bp =0;
   int st12 =0; int st24 =0;  int st124 =0;
   int h3b_hits = 0; int no_h3b_hits = 0; int h3t_hits = 0; int no_h3t_hits = 0;
   //int h3b_hits = 0; int no_h3b_hits = 0; int h3t_hits = 0; int no_h3t_hits = 0;
   int valid_exps_3b =0; int valid_closests_3b =0;
   int valid_exps_3t =0; int valid_closests_3t =0;

   double t_num_h1 =0;    double t_num_h2 =0;    double t_num_h3 =0;   double t_num_h4=0;
   double rh1_max =0;    double rh2_max =0;     double rh3_max =0;  double rh4_max =0;
   double rh1 =0;    double rh2 =0;     double rh3 =0;  double rh4 =0;
   
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
   std::vector<double> *tdc_h4t =0; std::vector<double> *tdc_h4b =0; 
   std::vector<double> *tdc_h4y1r =0;  std::vector<double> *tdc_h4y1l =0; std::vector<double> *tdc_h4y2r =0; std::vector<double> *tdc_h4y2l =0;
   std::vector<double> *eleIdsh4t =0; std::vector<double> *eleIdsh4b =0; 
   std::vector<double> *eleIdsh4y1r =0;  std::vector<double> *eleIdsh4y1l =0; std::vector<double> *eleIdsh4y2r =0; std::vector<double> *eleIdsh4y2l =0;

   int tls_event_ID; int nQualTracklets;
   std::vector<int> *detIDs =0;
   std::vector<int> *eleID_exps =0;
   std::vector<int> *eleID_closests =0;


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

   TH1D *eleIds_h4t = new TH1D("eleIds_h4t", "eleIds_h4t", 16, 1,17);
   TH1D *eleIds_h4b = new TH1D("eleIds_h4b", "eleIds_h4b", 16, 1,17);
   TH1D *eleIds_h4y1l = new TH1D("eleIds_h4y1l", "eleIds_h4y1l", 16, 1,17);
   TH1D *eleIds_h4y1r = new TH1D("eleIds_h4y1r", "eleIds_h4y1r", 16, 1,17);
   TH1D *eleIds_h4y2l = new TH1D("eleIds_h4y2l", "eleIds_h4y2l", 16, 1,17);
   TH1D *eleIds_h4y2r = new TH1D("eleIds_h4y2r", "eleIds_h4y2r", 16, 1,17);

   TH1D *eleIds_exps_3t = new TH1D("eleIds_exps_3t", "eleIds_exps_3t", 16, 0.5,16.5);
   TH1D *eleIds_closests_3t = new TH1D("eleIds_closests_3t", "eleIds_closests_3t", 16, 0.5,16.5);
   TH1D *diff_eleIds_3t = new TH1D("diff_eleIds_3t", "diff_eleIds_3t", 16, -6,6);
   TH1D *eleIds_exps_3b = new TH1D("eleIds_exps_3b", "eleIds_exps_3b", 16, 0.5,16.5);
   TH1D *eleIds_closests_3b = new TH1D("eleIds_closests_3b", "eleIds_closests_3b", 16, 0.5,16.5);
   TH1D *diff_eleIds_3b = new TH1D("diff_eleIds_3b", "diff_eleIds_3b", 16, -6,6);


   TGraphAsymmErrors* grh1 = new TGraphAsymmErrors();
   TGraphAsymmErrors* grh2 = new TGraphAsymmErrors();
   TGraphAsymmErrors* grh3 = new TGraphAsymmErrors();
   TGraphAsymmErrors* grh4 = new TGraphAsymmErrors();

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
