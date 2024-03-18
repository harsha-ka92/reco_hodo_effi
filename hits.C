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
   int xlow = 5024;
   int xhigh = 5042; 

   //set the first run_ID in the save tree as the run_num
   int run_num = 5024;

   //ref to keep track of the entry from the tls tree
   int tls_entry = 0; 

   tr->SetBranchAddress("run_ID", &run_ID);
   tr->SetBranchAddress("event_ID", &event_ID);
   tr->SetBranchAddress("dor", &dor);
   tr->SetBranchAddress("trigger", &trigger);
   tr->SetBranchAddress("nQualTracklets", &nQualTracklets);

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

   tr->SetBranchAddress("eleIdsh4t", &eleIdsh4t);
   tr->SetBranchAddress("eleIdsh4b", &eleIdsh4b);
   tr->SetBranchAddress("eleIdsh4y1r", &eleIdsh4y1r);
   tr->SetBranchAddress("eleIdsh4y1l", &eleIdsh4y1l);
   tr->SetBranchAddress("eleIdsh4y2r", &eleIdsh4y2r);
   tr->SetBranchAddress("eleIdsh4y2l", &eleIdsh4y2l);
   
   tr->SetBranchAddress("tlBackPartial", &tlBackPartial);

   tr_tls->SetBranchAddress("event_ID", &tls_event_ID);
   tr_tls->SetBranchAddress("detIDs", &detIDs);
   tr_tls->SetBranchAddress("stID", &stID);
   tr_tls->SetBranchAddress("eleID_exps", &eleID_exps);
   tr_tls->SetBranchAddress("eleID_closests", &eleID_closests);

   bool Trigger_Filter = true; //set to "true" if need to filter hits based on trigger.
   int nEntries = tr->GetEntries();
   for (int i_ent = 0; i_ent < nEntries; i_ent++) {

      tr->GetEntry(i_ent);
      if(run_ID < xlow || run_ID > xhigh) {continue;}
      ++nEvents;

      if(Trigger_Filter == true){

          //if((trigger & 0x1) != 0) { trigger_temp = "NIM1"; ++total_N1;}  //NIM1
          //if((trigger & 0x2) != 0) { trigger_temp = "NIM2"; ++total_N2;}  //NIM2
          //if((trigger & 0x4) != 0) { trigger_temp = "NIM3"; ++total_N3;}  //NIM3
          if((trigger & 0x8) != 0) { trigger_temp = "NIM4"; total_N4++;}  //NIM4 
          //if((trigger & 0x200) != 0) { trigger_temp = "MATRIX5"; ++total_M5;}  //MATRIX5
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
      if(run_num == run_ID && i_ent != nEntries-1){
        
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
            if(run_ID == 4692){eleIds_h4t->Fill(eleIdsh4t->at(j));}
        }
        for ( int j =0; j< tdc_h4b->size(); j++){
            htdc_h4b->Fill(tdc_h4b->at(j));
            if(run_ID == 4692){eleIds_h4b->Fill(eleIdsh4b->at(j));}
        }
        for ( int j =0; j< tdc_h4y1r->size(); j++){
            htdc_h4y1r->Fill(tdc_h4y1r->at(j));
            if(run_ID == 4692){eleIds_h4y1r->Fill(eleIdsh4y1r->at(j));}
        }
        for ( int j =0; j< tdc_h4y1l->size(); j++){
            htdc_h4y1l->Fill(tdc_h4y1l->at(j));
            if(run_ID == 4692){eleIds_h4y1l->Fill(eleIdsh4y1l->at(j));}
        }
        for ( int j =0; j< tdc_h4y2r->size(); j++){
            htdc_h4y2r->Fill(tdc_h4y2r->at(j));
            if(run_ID == 4692){eleIds_h4y2r->Fill(eleIdsh4y2r->at(j));}
        }
        for ( int j =0; j< tdc_h4y2l->size(); j++){
            htdc_h4y2l->Fill(tdc_h4y2l->at(j));
            if(run_ID == 4692){eleIds_h4y2l->Fill(eleIdsh4y2l->at(j));}
        }

        t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
        t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
        t_num_h3 += (num_h3t + num_h3b);
        t_num_h4 += (num_h4t + num_h4b + num_h4y1r + num_h4y1l + num_h4y2r + num_h4y2l);

        //categorizing events from either st1,st2 or st2,st4
        if ( (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) > 0 ){
            ++st24; 
            if((num_h3t + num_h3b)>0) {++st24w3;}

                for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){
                    tr_tls->GetEntry(i_tls_entry);
                    if(stID == 6){
                    if (event_ID == tls_event_ID){ 
                                for ( int j =0; j< detIDs->size(); j++){
                                    if(detIDs->at(j) == 39 && eleID_exps->at(j)>0){
                                        st3_OK = true;
                                        ++valid_exps_3b; 
                                        if(eleID_closests->at(j)>0){++valid_closests_3b; 
                                                                    eleIds_exps_3b -> Fill(eleID_exps->at(j)); 
                                                                    eleIds_closests_3b -> Fill(eleID_closests->at(j));
                                                                    diff_eleIds_3b -> Fill(eleID_exps->at(j)-eleID_closests->at(j));
                                        }
                                    }
                                    else if(detIDs->at(j) == 40 && eleID_exps->at(j)>0){
                                        st3_OK = true;
                                        ++valid_exps_3t; 
                                        if(eleID_closests->at(j)>0){++valid_closests_3t; 
                                                                    eleIds_exps_3t -> Fill(eleID_exps->at(j)); 
                                                                    eleIds_closests_3t -> Fill(eleID_closests->at(j));
                                                                    diff_eleIds_3t -> Fill(eleID_exps->at(j)-eleID_closests->at(j));
                                        }
                                    }
                                    if(st3_OK) {if(detIDs->at(j) == 47 && eleID_closests->at(j)>0) {++has_a_hit_p1y1;}
                                              else if(detIDs->at(j) == 48 && eleID_closests->at(j)>0) {++has_a_hit_p1y2;}
                                              else if(detIDs->at(j) == 49 && eleID_closests->at(j)>0) {++has_a_hit_p1x1;}
                                              else if(detIDs->at(j) == 50 && eleID_closests->at(j)>0) {++has_a_hit_p1x2;}
                                              else if(detIDs->at(j) == 51 && eleID_closests->at(j)>0) {++has_a_hit_p2x1;}
                                              else if(detIDs->at(j) == 52 && eleID_closests->at(j)>0) {++has_a_hit_p2x2;}
                                              else if(detIDs->at(j) == 53 && eleID_closests->at(j)>0) {++has_a_hit_p2y1;}
                                              else if(detIDs->at(j) == 54 && eleID_closests->at(j)>0) {++has_a_hit_p2y2;}
                                 }
                            }
                    }    
                }
            }
        }
        if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0){++st12;}
        if ( (num_h1t + num_h1b + num_h1r + num_h1l) > 0 && (num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) >0 ){++st124;}

         run_time = dor;
         run_num = run_ID;
         //std::cout<<run_ID<<std::endl;
         //std::cout<<event_ID<<std::endl;
      }    

     //This is needed to complete the calculations and plot the results when looking at the last event in the tree
     else if (run_num == run_ID && i_ent == nEntries-1){
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
            if(run_ID == 4692){eleIds_h4t->Fill(eleIdsh4t->at(j));}
            }
            for ( int j =0; j< tdc_h4b->size(); j++){
            htdc_h4b->Fill(tdc_h4b->at(j));
            if(run_ID == 4692){eleIds_h4b->Fill(eleIdsh4b->at(j));}
            }
            for ( int j =0; j< tdc_h4y1r->size(); j++){
            htdc_h4y1r->Fill(tdc_h4y1r->at(j));
            if(run_ID == 4692){eleIds_h4y1r->Fill(eleIdsh4y1r->at(j));}
            }
            for ( int j =0; j< tdc_h4y1l->size(); j++){
            htdc_h4y1l->Fill(tdc_h4y1l->at(j));
            if(run_ID == 4692){eleIds_h4y1l->Fill(eleIdsh4y1l->at(j));}
            }
            for ( int j =0; j< tdc_h4y2r->size(); j++){
            htdc_h4y2r->Fill(tdc_h4y2r->at(j));
            if(run_ID == 4692){eleIds_h4y2r->Fill(eleIdsh4y2r->at(j));}
            }
            for ( int j =0; j< tdc_h4y2l->size(); j++){
            htdc_h4y2l->Fill(tdc_h4y2l->at(j));
            if(run_ID == 4692){eleIds_h4y2l->Fill(eleIdsh4y2l->at(j));}
            }

            t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
            t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
            t_num_h3 += (num_h3t + num_h3b);
            t_num_h4 += (num_h4t + num_h4b + num_h4y1r + num_h4y1l + num_h4y2r + num_h4y2l);

            //categorizing events from either st1,st2 or st2,st4
            if ((num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) > 0 ){
               ++st24; 
               if((num_h3t + num_h3b)>0) {++st24w3;}
                
                for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){
                    tr_tls->GetEntry(i_tls_entry);
                    if(stID == 6){
                    if (event_ID == tls_event_ID){ 
                                for ( int j =0; j< detIDs->size(); j++){
                                    if(detIDs->at(j) == 39 && eleID_exps->at(j)>0){
                                        st3_OK = true;
                                        ++valid_exps_3b; 
                                        if(eleID_closests->at(j)>0){++valid_closests_3b; 
                                                                    eleIds_exps_3b -> Fill(eleID_exps->at(j)); 
                                                                    eleIds_closests_3b -> Fill(eleID_closests->at(j));
                                                                    diff_eleIds_3b -> Fill(eleID_exps->at(j)-eleID_closests->at(j));
                                        }
                                    }
                                    else if(detIDs->at(j) == 40 && eleID_exps->at(j)>0){
                                        st3_OK = true;
                                        ++valid_exps_3t; 
                                        if(eleID_closests->at(j)>0){++valid_closests_3t; 
                                                                    eleIds_exps_3t -> Fill(eleID_exps->at(j)); 
                                                                    eleIds_closests_3t -> Fill(eleID_closests->at(j));
                                                                    diff_eleIds_3t -> Fill(eleID_exps->at(j)-eleID_closests->at(j));
                                        }
                                    }
                                  
                                    if(st3_OK) {if(detIDs->at(j) == 47 && eleID_closests->at(j)>0) {++has_a_hit_p1y1;}
                                              else if(detIDs->at(j) == 48 && eleID_closests->at(j)>0) {++has_a_hit_p1y2;}
                                              else if(detIDs->at(j) == 49 && eleID_closests->at(j)>0) {++has_a_hit_p1x1;}
                                              else if(detIDs->at(j) == 50 && eleID_closests->at(j)>0) {++has_a_hit_p1x2;}
                                              else if(detIDs->at(j) == 51 && eleID_closests->at(j)>0) {++has_a_hit_p2x1;}
                                              else if(detIDs->at(j) == 52 && eleID_closests->at(j)>0) {++has_a_hit_p2x2;}
                                              else if(detIDs->at(j) == 53 && eleID_closests->at(j)>0) {++has_a_hit_p2y1;}
                                              else if(detIDs->at(j) == 54 && eleID_closests->at(j)>0) {++has_a_hit_p2y2;}
                                 }
                            }
                    }    
                }
              }
            }

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
           //std::cout<<run_ID<<std::endl;
           //std::cout<<event_ID<<std::endl;
     }

     //this will be executed when ever the run number changes in the tree. First it will calulates the rates and include points in the plots upto the previous event.
     //Then it will reset all the vars and start storing the info of the current event and next ones unitll the run number changes again or untill the last event is met.
     
     else {
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
           t_num_h1 = 0; t_num_h2 = 0; t_num_h3 = 0;

           i++;
           std::cout << "i = " << i << std::endl;

           run_num = run_ID;
           run_time = dor;
           std::cout<< "run number changed"<<std::endl;
           std::cout<<run_ID<<std::endl;
           //std::cout<<event_ID<<std::endl;

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
            if(run_ID == 4692){eleIds_h4t->Fill(eleIdsh4t->at(j));}
            }
            for ( int j =0; j< tdc_h4b->size(); j++){
            htdc_h4b->Fill(tdc_h4b->at(j));
            if(run_ID == 4692){eleIds_h4b->Fill(eleIdsh4b->at(j));}
            }
            for ( int j =0; j< tdc_h4y1r->size(); j++){
            htdc_h4y1r->Fill(tdc_h4y1r->at(j));
            if(run_ID == 4692){eleIds_h4y1r->Fill(eleIdsh4y1r->at(j));}
            }
            for ( int j =0; j< tdc_h4y1l->size(); j++){
            htdc_h4y1l->Fill(tdc_h4y1l->at(j));
            if(run_ID == 4692){eleIds_h4y1l->Fill(eleIdsh4y1l->at(j));}
            }
            for ( int j =0; j< tdc_h4y2r->size(); j++){
            htdc_h4y2r->Fill(tdc_h4y2r->at(j));
            if(run_ID == 4692){eleIds_h4y2r->Fill(eleIdsh4y2r->at(j));}
            }
            for ( int j =0; j< tdc_h4y2l->size(); j++){
            htdc_h4y2l->Fill(tdc_h4y2l->at(j));
            if(run_ID == 4692){eleIds_h4y2l->Fill(eleIdsh4y2l->at(j));}
            }

            t_num_h1 += (num_h1t + num_h1b + num_h1r + num_h1l);
            t_num_h2 += (num_h2t + num_h2b + num_h2r + num_h2l);
            t_num_h3 += (num_h3t + num_h3b);
            t_num_h4 += (num_h4t + num_h4b + num_h4y1r + num_h4y1l + num_h4y2r + num_h4y2l);

            //categorizing events from either st1,st2 or st2,st4
            if ((num_h2t + num_h2b + num_h2r + num_h2l) > 0 && (num_h4t + num_h4b + num_h4y2r + num_h4y2l) > 0 ){
                ++st24; 
                if((num_h3t + num_h3b)>0) {++st24w3;}
                
                  for(int i_tls_entry =0;  i_tls_entry < tr_tls->GetEntries(); i_tls_entry++){
                    tr_tls->GetEntry(i_tls_entry);
                    if(stID == 6){
                    if (event_ID == tls_event_ID){ 
                                for ( int j =0; j< detIDs->size(); j++){
                                    if(detIDs->at(j) == 39 && eleID_exps->at(j)>0){
                                        st3_OK = true;
                                        ++valid_exps_3b; 
                                        if(eleID_closests->at(j)>0){++valid_closests_3b; 
                                                                    eleIds_exps_3b -> Fill(eleID_exps->at(j)); 
                                                                    eleIds_closests_3b -> Fill(eleID_closests->at(j));
                                                                    diff_eleIds_3b -> Fill(eleID_exps->at(j)-eleID_closests->at(j));
                                        }
                                    }
                                    else if(detIDs->at(j) == 40 && eleID_exps->at(j)>0){
                                        st3_OK = true;
                                        ++valid_exps_3t; 
                                        if(eleID_closests->at(j)>0){++valid_closests_3t; 
                                                                    eleIds_exps_3t -> Fill(eleID_exps->at(j)); 
                                                                    eleIds_closests_3t -> Fill(eleID_closests->at(j));
                                                                    diff_eleIds_3t -> Fill(eleID_exps->at(j)-eleID_closests->at(j));
                                        }
                                    }
                                    if(st3_OK) {if(detIDs->at(j) == 47 && eleID_closests->at(j)>0) {++has_a_hit_p1y1;}
                                              else if(detIDs->at(j) == 48 && eleID_closests->at(j)>0) {++has_a_hit_p1y2;}
                                              else if(detIDs->at(j) == 49 && eleID_closests->at(j)>0) {++has_a_hit_p1x1;}
                                              else if(detIDs->at(j) == 50 && eleID_closests->at(j)>0) {++has_a_hit_p1x2;}
                                              else if(detIDs->at(j) == 51 && eleID_closests->at(j)>0) {++has_a_hit_p2x1;}
                                              else if(detIDs->at(j) == 52 && eleID_closests->at(j)>0) {++has_a_hit_p2x2;}
                                              else if(detIDs->at(j) == 53 && eleID_closests->at(j)>0) {++has_a_hit_p2y1;}
                                              else if(detIDs->at(j) == 54 && eleID_closests->at(j)>0) {++has_a_hit_p2y2;}
                                 }
                            }
                    }    
                }
              }
            }

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

    TCanvas* c9 = new TCanvas("c9", "", 1000, 500);
    c9->cd();
    c9->Divide(2,3,0,0);
    c9->cd(1); eleIds_h4t->Draw();
    c9->cd(2); eleIds_h4b->Draw();    
    c9->cd(3); eleIds_h4y1r->Draw();
    c9->cd(4); eleIds_h4y1l->Draw();
    c9->cd(5); eleIds_h4y2r->Draw();
    c9->cd(6); eleIds_h4y2l->Draw();
    c9->cd();
    c9->Update();

    TCanvas* c10 = new TCanvas("c10", "H3B", 500, 1000);
    c10->cd();
    c10->Divide(1,3,0,0);
    c10->cd(1); eleIds_exps_3b->Draw();
    c10->cd(2); eleIds_closests_3b->Draw();    
    c10->cd(3); diff_eleIds_3b->Draw();
    c10->cd();
    c10->Update();

    TCanvas* c11 = new TCanvas("c11", "H3T", 500, 1000);
    c11->cd();
    c11->Divide(1,3,0,0);
    c11->cd(1); eleIds_exps_3t->Draw();
    c11->cd(2); eleIds_closests_3t->Draw();    
    c11->cd(3); diff_eleIds_3t->Draw();
    c11->cd();
    c11->Update();

    gSystem->mkdir(Form("hitRates/%s",trigger_temp.c_str()), 1);

    c1->SaveAs(Form("hitRates/%s/tdc_h1.png",trigger_temp.c_str()));
    c2->SaveAs(Form("hitRates/%s/tdc_h2.png",trigger_temp.c_str()));
    c3->SaveAs(Form("hitRates/%s/tdc_h3.png",trigger_temp.c_str()));
    c4->SaveAs(Form("hitRates/%s/tdc_h4.png",trigger_temp.c_str()));
    c5->SaveAs(Form("hitRates/%s/rh1.png",trigger_temp.c_str()));
    c6->SaveAs(Form("hitRates/%s/rh2.png",trigger_temp.c_str()));
    c7->SaveAs(Form("hitRates/%s/rh3.png",trigger_temp.c_str()));
    c8->SaveAs(Form("hitRates/%s/rh4.png",trigger_temp.c_str()));
    c10->SaveAs(Form("hitRates/%s/eleIDs_h3b.png",trigger_temp.c_str()));
    c11->SaveAs(Form("hitRates/%s/eleIDs_h3t.png",trigger_temp.c_str()));
    c9->SaveAs("hitRates/st4Hits.png");

    std::cout<<"*************************"<<std::endl;
    std::cout<<"Total number of events with at least one tracklet: "<< nEvents <<std::endl;
    std::cout<<"Total "<<trigger_temp<<" events : "<< total_N4 <<std::endl; // total_N1, total_N2, total_N3, total_N4 or total_M5 depending on the trigger filter using
    if (trigger_temp == "MATRIX5" || trigger_temp == "NIM4"){
        std::cout<<"number of events that satisfy H2 && H4 is : "<< st24 <<std::endl;
        std::cout<<"number of events that satisfy H1 && H2 is : "<< st12 <<std::endl;
        std::cout<<"number of events with hits in all st1, st2 and st4 : "<< st124 <<std::endl;
        std::cout<<"events triggered by a cosmic ray from st 24 and with hits in st3 : "<< st24w3 <<std::endl;
    }
    if (trigger_temp == "NIM2"){
        std::cout<<"number of events that satisfy H1 && H2 is : "<< st12 <<std::endl;
        std::cout<<"number of events with hits in all st1, st2 and st4 : "<< st124 <<std::endl;
    }
    
    std::cout<<"*************************"<<std::endl;
    std::cout<<"In events triggered with "<<trigger_temp.c_str()<<std::endl;
    std::cout<<"number of events with at least one back partial track ;"<<total_bp<<std::endl;
    std::cout<<"number back partial tracks with hits in st3t && st3b;"<<has_hits<<std::endl;
    std::cout<<"number back partial tracks with hits only in st3b;"<<h3b_hits<<std::endl;
    std::cout<<"number back partial tracks with hits only in st3t;"<<h3t_hits<<std::endl;
    std::cout<<"number back partial tracks with no hits in st3;"<<no_hits<<std::endl;
    std::cout<<"*************************"<<std::endl;

    std::cout<<"In the events with hits in both H2 and H4 ("<<st24<<")"<<std::endl;
    std::cout<<"Number of back partial tacklets that produce a valid expected element in H3T: "<< valid_exps_3t <<std::endl;
    std::cout<<"Number of back partial tacklets that has a valid closest fired element in H3T: "<< valid_closests_3t <<std::endl;
    std::cout<<"-------------------------------------------------------------------------------------------------"<<std::endl;
    std::cout<<"Number of back partial tacklets that produce a valid expected element in H3B: "<< valid_exps_3b <<std::endl;
    std::cout<<"Number of back partial tacklets that has a valid closest fired element in H3B: "<< valid_closests_3b <<std::endl;
    std::cout<<"-------------------------------------------------------------------------------------------------"<<std::endl;
    std::cout<<"Total number of back partial tacklets that produce a valid expected element in H3: "<< (valid_exps_3b + valid_exps_3t) <<std::endl;
    std::cout<<"Total number of back partial tacklets that produce a valid fired element in H3: "<< (valid_closests_3b + valid_closests_3t) <<std::endl;
    std::cout<<"*************************"<<std::endl;


    std::cout<<"In the events with a valid expected element in st3 ("<<valid_exps_3b + valid_exps_3t<<")"<<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p1y1: "<< has_a_hit_p1y1 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p1y2: "<< has_a_hit_p1y2 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p1x1: "<< has_a_hit_p1x1 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p1x2: "<< has_a_hit_p1x2 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p2x1: "<< has_a_hit_p2x1 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p2x2: "<< has_a_hit_p2x2 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p2y1: "<< has_a_hit_p2y1 <<std::endl;
    std::cout<<"Number of back partial tacklets that has a supporting hit in p2y2: "<< has_a_hit_p2y2 <<std::endl;
    std::cout<<"*************************"<<std::endl;
    std::cout<<"New trigger events : "<< new_trigger <<std::endl;
}
