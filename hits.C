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
   int xlow = 4685;
   int xhigh = 4702; 

   int run_num = 4690;

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("event_ID", &event_ID);
   tr->SetBranchAddress("dor", &dor);
   tr->SetBranchAddress("trigger", &trigger);

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

   tr->SetBranchAddress("tdc_h4t", &tdc_h4t);
   tr->SetBranchAddress("tdc_h4b", &tdc_h4b);
   tr->SetBranchAddress("tdc_h4y1r", &tdc_h4y1r);
   tr->SetBranchAddress("tdc_h4y1l", &tdc_h4y1l);
   tr->SetBranchAddress("tdc_h4y2r", &tdc_h4y2r);
   tr->SetBranchAddress("tdc_h4y2l", &tdc_h4y2l);
   
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

   nEvents = tr->GetEntries();
   bool Trigger_Filter = true; //set to "true" if need to filter hits based on trigger.
for (int i_ent = 0; i_ent < tr->GetEntries(); i_ent++) {
      tr->GetEntry(i_ent);
      
      if(Trigger_Filter == true){

          //remove the "continue" of the trigger you want in the analysis
          if(trigger == 1) {continue; trigger_temp = "NIM1"; ++total_N1;}  //NIM1
          if(trigger == 2) {continue; trigger_temp = "NIM2"; ++total_N2;}  //NIM2
          if(trigger == 3) {continue; trigger_temp = "NIM3"; ++total_N3;}  //NIM3
          if(trigger == 4) {trigger_temp = "NIM4"; total_N4++;}  //NIM4 
          if(trigger == 5) {continue; trigger_temp = "MATRIX5"; ++total_M5;}  //MATRIX5
          if(trigger !=1 && trigger !=2 && trigger !=3 && trigger !=4 && trigger !=5){std::cout<<"trigger value is different "<<trigger<<std::endl; new_trigger++; continue;}
          
      }

       if (tlBackPartial>0){ 
            total_bp++;
            if (num_h3b>0 && num_h3t ==0){++h3b_hits;}
            //if (num_h3b==0){++no_h3b_hits;}
            //if (num_h3t==0){++no_h3t_hits;}
            if (num_h3t>0 && num_h3b ==0){++h3t_hits;}
            if (num_h3t >0 && num_h3b >0 ) {++has_hits;} 
            if (num_h3t ==0 && num_h3b ==0){++no_hits;}
       }

      if(run_ID < xlow || run_ID > xhigh) {continue;}
      
      if(dor < 0) {run_num = run_ID; std::cout << "invalid dor"<<std::endl; continue;}

      //since run_num set to the run_ID of the first event in the tree following if condition will be satisfied.
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

        for ( int j =0; j< tdc_h4t->size(); j++){
            htdc_h4t->Fill(tdc_h4t->at(j));
        }
        for ( int j =0; j< tdc_h4b->size(); j++){
            htdc_h4b->Fill(tdc_h4b->at(j));
        }
        for ( int j =0; j< tdc_h4y1r->size(); j++){
            htdc_h4y1r->Fill(tdc_h4y1r->at(j));
        }
        for ( int j =0; j< tdc_h4y1l->size(); j++){
            htdc_h4y1l->Fill(tdc_h4y1l->at(j));
        }
        for ( int j =0; j< tdc_h4y2r->size(); j++){
            htdc_h4y2r->Fill(tdc_h4y2r->at(j));
        }
        for ( int j =0; j< tdc_h4y2l->size(); j++){
            htdc_h4y2l->Fill(tdc_h4y2l->at(j));
        }

        t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
        t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
        t_num_h3 += (num_h3t + num_h3b);
        t_num_h4 += (num_h4t + num_h4b + num_h4y1r + num_h4y1l + num_h4y2r + num_h4y2l);

        //categorizing events from either st1,st2 or st2,st4
        if ( (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) > 0){++st24;}
        if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0){++st12;}
        if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) >0 ){++st124;}

         run_time = dor;
         run_num = run_ID;
         std::cout<<run_ID<<std::endl;
         std::cout<<event_ID<<std::endl;
      }    

     //This is needed to complete the calculations and plot the results when looking at the last event in the tree
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
        
            for ( int j =0; j< tdc_h4t->size(); j++){
            htdc_h4t->Fill(tdc_h4t->at(j));
            }
            for ( int j =0; j< tdc_h4b->size(); j++){
            htdc_h4b->Fill(tdc_h4b->at(j));
            }
            for ( int j =0; j< tdc_h4y1r->size(); j++){
            htdc_h4y1r->Fill(tdc_h4y1r->at(j));
            }
            for ( int j =0; j< tdc_h4y1l->size(); j++){
            htdc_h4y1l->Fill(tdc_h4y1l->at(j));
            }
            for ( int j =0; j< tdc_h4y2r->size(); j++){
            htdc_h4y2r->Fill(tdc_h4y2r->at(j));
            }
            for ( int j =0; j< tdc_h4y2l->size(); j++){
            htdc_h4y2l->Fill(tdc_h4y2l->at(j));
            }

            t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
            t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
            t_num_h3 += (num_h3t + num_h3b);
            t_num_h4 += (num_h4t + num_h4b + num_h4y1r + num_h4y1l + num_h4y2r + num_h4y2l);

            //categorizing events from either st1,st2 or st2,st4
            if ( (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) > 0){++st24;}
            if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0){++st12;}
            if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) >0 ){++st124;}

           rh1 = t_num_h1/run_time/60;
           rh2 = t_num_h2/run_time/60;
           rh3 = t_num_h3/run_time/60;
           rh4 = t_num_h4/run_time/60;
    
           if(rh1_max < rh1 ){ rh1_max = rh1; }
           if(rh2_max < rh2 ){ rh2_max = rh2; }
           if(rh3_max < rh3 ){ rh3_max = rh3; }
           if(rh4_max < rh4 ){ rh4_max = rh4; }

           grh1->SetPoint(i, run_num, rh1);
           grh1->SetPointError(i, 0., 0., sqrt(t_num_h1)/run_time/60, sqrt(t_num_h1)/run_time/60);

           grh2->SetPoint(i, run_num, rh2);
           grh2->SetPointError(i, 0., 0., sqrt(t_num_h2)/run_time/60, sqrt(t_num_h2)/run_time/60);

           grh3->SetPoint(i, run_num, rh3);
           grh3->SetPointError(i, 0., 0., sqrt(t_num_h3)/run_time/60, sqrt(t_num_h3)/run_time/60);

           grh4->SetPoint(i, run_num, rh4);
           grh4->SetPointError(i, 0., 0., sqrt(t_num_h4)/run_time/60, sqrt(t_num_h4)/run_time/60);

           //rh1=0; rh2=0; rh3=0;
           run_num = run_ID;
           std::cout<<run_ID<<std::endl;
           std::cout<<event_ID<<std::endl;
     }

     //this will be executed when ever the run number changes in the tree. First it will calulates the rates and include points in the plots upto the previous event.
     //Then it will reset all the vars and start storing the info of the current event and next ones unitll the run number changes again or untill the last event is met.
     
     else {
           std::cout<< "run number changed"<<std::endl;
           rh1 = t_num_h1/run_time/60;
           rh2 = t_num_h2/run_time/60;
           rh3 = t_num_h3/run_time/60;
           rh4 = t_num_h4/run_time/60;
    
           if(rh1_max < rh1 ){ rh1_max = rh1; }
           if(rh2_max < rh2 ){ rh2_max = rh2; }
           if(rh3_max < rh3 ){ rh3_max = rh3; }
           if(rh4_max < rh4 ){ rh4_max = rh4; }

           grh1->SetPoint(i, run_num, rh1);
           grh1->SetPointError(i, 0., 0., sqrt(t_num_h1)/run_time/60, sqrt(t_num_h1)/run_time/60);

           grh2->SetPoint(i, run_num, rh2);
           grh2->SetPointError(i, 0., 0., sqrt(t_num_h2)/run_time/60, sqrt(t_num_h2)/run_time/60);

           grh3->SetPoint(i, run_num, rh3);
           grh3->SetPointError(i, 0., 0., sqrt(t_num_h3)/run_time/60, sqrt(t_num_h3)/run_time/60);

           grh4->SetPoint(i, run_num, rh4);
           grh4->SetPointError(i, 0., 0., sqrt(t_num_h4)/run_time/60, sqrt(t_num_h4)/run_time/60);

           //resetting the variable for the new run

           rh1=0; rh2=0; rh3=0; rh4=0;
           i++;
           std::cout << "i = " << i << std::endl;
           run_num = run_ID;
           run_time = dor;
           std::cout<<run_ID<<std::endl;
           std::cout<<event_ID<<std::endl;
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

            for ( int j =0; j< tdc_h4t->size(); j++){
            htdc_h4t->Fill(tdc_h4t->at(j));
            }
            for ( int j =0; j< tdc_h4b->size(); j++){
             htdc_h4b->Fill(tdc_h4b->at(j));
            }
            for ( int j =0; j< tdc_h4y1r->size(); j++){
            htdc_h4y1r->Fill(tdc_h4y1r->at(j));
            }
            for ( int j =0; j< tdc_h4y1l->size(); j++){
            htdc_h4y1l->Fill(tdc_h4y1l->at(j));
            }
            for ( int j =0; j< tdc_h4y2r->size(); j++){
            htdc_h4y2r->Fill(tdc_h4y2r->at(j));
            }
            for ( int j =0; j< tdc_h4y2l->size(); j++){
            htdc_h4y2l->Fill(tdc_h4y2l->at(j));
            }

            t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
            std::cout<<"total hits in st 1"<<t_num_h1<<std::endl;
            t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
            t_num_h3 += (num_h3t + num_h3b);
            t_num_h4 += (num_h4t + num_h4b + num_h4y1r + num_h4y1l + num_h4y2r + num_h4y2l);

            //categorizing events from either st1,st2 or st2,st4
            if ( (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) > 0){++st24;}
            if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0){++st12;}
            if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) >0 ){++st124;}

    }
}

    TCanvas* c1 = new TCanvas(Form("tdc_time of %s hits : St1", trigger_temp.c_str()), Form("tdc_time of %s hits : St1", trigger_temp.c_str()), 1000, 500);
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
    l2->AddEntry(htdc_h2t, Form("run ID %d",run_num) , "");
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
    l3->AddEntry(htdc_h3t, Form("run ID %d",run_num) , "");
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
    l4->AddEntry(htdc_h4t, Form("run ID %d",run_num) , "");
    l4->AddEntry(htdc_h4t, "h4t", "l");
    l4->AddEntry(htdc_h4b, "h4b", "l");
    l4->AddEntry(htdc_h4y1r, "h4y1r", "l");
    l4->AddEntry(htdc_h4y1l, "h4y1l", "l");
    l4->AddEntry(htdc_h4y2r, "h4y2r", "l");
    l4->AddEntry(htdc_h4y2l, "h4y2l", "l");
    l4->Draw();
    c4->Update();

    TCanvas* c5 = new TCanvas("c5", "", 1000, 500);

    grh1->SetTitle(Form("Hit rate of %s events : St1", trigger_temp.c_str()));
    grh1->SetMarkerColor(4);
    grh1->SetMarkerStyle(7);
    //grh1->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    //grh1->GetYaxis()->SetLimits(0,1.05*rh1_max);
    grh1->GetXaxis()->SetTitle("run_ID");
    grh1->GetYaxis()->SetTitle("number of hits/min");
    grh1->Draw("APE1");
    c5->Update();


    TCanvas* c6 = new TCanvas("c6", "", 1000, 500);

    grh2->SetTitle(Form("Hit rate of %s events : St2", trigger_temp.c_str()));
    grh2->SetMarkerColor(4);
    grh2->SetMarkerStyle(7);
    grh2->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    grh2->GetYaxis()->SetLimits(0,1.05*rh2_max);
    grh2->GetXaxis()->SetTitle("run_ID");
    grh2->GetYaxis()->SetTitle("number of hits/min");
    grh2->Draw("APE1");
    c6->Update();


    TCanvas* c7 = new TCanvas("c7", "", 1000, 500);

    grh3->SetTitle(Form("Hit rate of %s events : St3", trigger_temp.c_str()));
    grh3->SetMarkerColor(4);
    grh3->SetMarkerStyle(7);
    grh3->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    grh3->GetYaxis()->SetLimits(0,1.05*rh3_max);
    grh3->GetXaxis()->SetTitle("run_ID");
    grh3->GetYaxis()->SetTitle("number of hits/min");
    grh3->Draw("APE1");
    c7->Update();

    TCanvas* c8 = new TCanvas("c8", "", 1000, 500);

    grh4->SetTitle(Form("Hit rate of %s events : St4", trigger_temp.c_str()));
    grh4->SetMarkerColor(4);
    grh4->SetMarkerStyle(7);
    grh4->GetXaxis()->SetLimits(xlow-0.5,xhigh);
    grh4->GetYaxis()->SetLimits(0,1.05*rh4_max);
    grh4->GetXaxis()->SetTitle("run_ID");
    grh4->GetYaxis()->SetTitle("number of hits/min");
    grh4->Draw("APE1");
    c8->Update();


    gSystem->mkdir(Form("hitRates/%s",trigger_temp.c_str()), 1);

    c1->SaveAs(Form("hitRates/%s/tdc_h1.png",trigger_temp.c_str()));
    c2->SaveAs(Form("hitRates/%s/tdc_h2.png",trigger_temp.c_str()));
    c3->SaveAs(Form("hitRates/%s/tdc_h3.png",trigger_temp.c_str()));
    c4->SaveAs(Form("hitRates/%s/tdc_h4.png",trigger_temp.c_str()));
    c5->SaveAs(Form("hitRates/%s/rh1.png",trigger_temp.c_str()));
    c6->SaveAs(Form("hitRates/%s/rh2.png",trigger_temp.c_str()));
    c7->SaveAs(Form("hitRates/%s/rh3.png",trigger_temp.c_str()));
    c8->SaveAs(Form("hitRates/%s/rh4.png",trigger_temp.c_str()));

    std::cout<<"*************************"<<std::endl;
    std::cout<<"Total number of events : "<< nEvents <<std::endl;
    std::cout<<"Total "<<trigger_temp<<" events : "<< total_N4 <<std::endl; // total_N1, total_N2, total_N3, total_N4 or total_M5 depending on the trigger filter using
    std::cout<<"events triggered by a cosmic ray from st 24 : "<< st24 <<std::endl;
    std::cout<<"events triggered by a cosmic ray from st 12 : "<< st12 <<std::endl;
    std::cout<<"events with hits in all st1, st2 and st4 : "<< st124 <<std::endl;

    std::cout<<"*************************"<<std::endl;
    std::cout<<"In events triggered with "<<trigger_temp.c_str()<<std::endl;
    std::cout<<"number of events with at least one back partial track ;"<<total_bp<<std::endl;
    std::cout<<"number back partial tracks with hits in st3t && st3b;"<<has_hits<<std::endl;
    std::cout<<"number back partial tracks with hits only in st3b;"<<h3b_hits<<std::endl;
    std::cout<<"number back partial tracks with hits only in st3t;"<<h3t_hits<<std::endl;
    std::cout<<"number back partial tracks with no hits in st3;"<<no_hits<<std::endl;
    std::cout<<"*************************"<<std::endl;
    std::cout<<"New trigger events : "<< new_trigger <<std::endl;
}
