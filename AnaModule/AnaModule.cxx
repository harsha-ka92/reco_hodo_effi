#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <fstream>

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <interface_main/SQHitVector_v1.h>
#include <interface_main/SQTrackVector_v1.h>
#include <interface_main/SQDimuonVector_v1.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <UtilAna/UtilSQHit.h>
#include <vector>

#include "AnaModule.h"
//#include "HitData.h"

AnaModule::AnaModule(const std::string& name): SubsysReco(name), p_geomSvc(GeomSvc::instance())
{}

AnaModule::~AnaModule()
{}

int AnaModule::Init(PHCompositeNode* topNode)
{ 
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaModule::InitRun(PHCompositeNode* topNode)
{
  int ret = GetNodes(topNode);
  if(ret != Fun4AllReturnCodes::EVENT_OK) return ret;
  
  eventID = 0;
  run_ID_temp = 0;
  tlD0 = 0; tlD1 = 0; tlD2 = 0; tlD3p = 0; tlD3m = 0; tlBackPartial = 0; tlGlobal = 0;
  MakeTree();

  std::fstream infile("AnaModule/mysql_output.txt");

  while (getline(infile,id))
  {
    
    std::istringstream is( id );
    is >> r.runID >> r.beginT >> r.endT;

    run_time[r.runID]=std::pair<int,int>(r.beginT, r.endT);
    }
	
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaModule::process_event(PHCompositeNode* topNode)
{
  nTracklets = trackletVec->size();
  
  event_ID = event->get_event_id();
  run_ID = run -> get_run_id();
  if (run_ID_temp != run_ID){
 	 std::cout << "---> run_ID " << run_ID << std::endl;
	 std::cout<<  "start time " << run_time.at(run_ID).first <<std::endl;
	 std::cout<<  "end time " << run_time.at(run_ID).second <<std::endl;
  	 dor = run_time.at(run_ID).second - run_time.at(run_ID).first;
 	 std::cout<<  "run time " << dor <<std::endl;

	 tlD0 = 0; tlD1 = 0; tlD2 = 0; tlD3p = 0; tlD3m = 0; tlBackPartial = 0; tlGlobal = 0;
  }
  run_ID_temp = run_ID;	
	if (nTracklets == 0) {return Fun4AllReturnCodes::EVENT_OK;}
	
	std::cout << "Event ID : " << event_ID << std::endl;
	trigger = event->get_trigger();
	std::cout<< "triger bit of the event "<< trigger << std::endl;
	
	//Number of Hist in : St1
	std::shared_ptr<SQHitVector> hv_h1t(UtilSQHit::FindHits(hitVector, "H1T"));
	num_h1t = hv_h1t->size();
	//std::cout << "Num hits in h1t = " << num_h1t << std::endl;
 	for (auto it = hv_h1t->begin(); it != hv_h1t->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h1t.push_back(tdc_time);
	}
	
	std::shared_ptr<SQHitVector> hv_h1b(UtilSQHit::FindHits(hitVector, "H1B"));
	num_h1b = hv_h1b->size();
	//std::cout << "Num hits in h1b = " << num_h1b << std::endl;
 	for (auto it = hv_h1b->begin(); it != hv_h1b->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h1b.push_back(tdc_time);
	}

	std::shared_ptr<SQHitVector> hv_h1r(UtilSQHit::FindHits(hitVector, "H1R"));
	num_h1r = hv_h1r->size();
	//std::cout << "Num hits in h1r = " << num_h1r << std::endl;
 	for (auto it = hv_h1r->begin(); it != hv_h1r->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h1r.push_back(tdc_time);
	}

	std::shared_ptr<SQHitVector> hv_h1l(UtilSQHit::FindHits(hitVector, "H1L"));
	num_h1l = hv_h1l->size();
	//std::cout << "Num hits in h1l = " << num_h1l << std::endl;
 	for (auto it = hv_h1l->begin(); it != hv_h1l->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h1l.push_back(tdc_time);
	}


	//Number of Hist in : St2
	std::shared_ptr<SQHitVector> hv_h2t(UtilSQHit::FindHits(hitVector, "H2T"));
	num_h2t = hv_h2t->size();
	//std::cout << "Num hits in h2t = " << num_h2t << std::endl;
 	for (auto it = hv_h2t->begin(); it != hv_h2t->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h2t.push_back(tdc_time);
	}
	
	std::shared_ptr<SQHitVector> hv_h2b(UtilSQHit::FindHits(hitVector, "H2B"));
	num_h2b = hv_h2b->size();
	//std::cout << "Num hits in h2b = " << num_h2b << std::endl;
 	for (auto it = hv_h2b->begin(); it != hv_h2b->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h2b.push_back(tdc_time);
	}

	std::shared_ptr<SQHitVector> hv_h2r(UtilSQHit::FindHits(hitVector, "H2R"));
	num_h2r = hv_h2r->size();
	//std::cout << "Num hits in h2r = " << num_h2r << std::endl;
 	for (auto it = hv_h2r->begin(); it != hv_h2r->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h2r.push_back(tdc_time);
	}

	std::shared_ptr<SQHitVector> hv_h2l(UtilSQHit::FindHits(hitVector, "H2L"));
	num_h2l = hv_h2l->size();
	//std::cout << "Num hits in h2l = " << num_h2l << std::endl;
 	for (auto it = hv_h2l->begin(); it != hv_h2l->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h2l.push_back(tdc_time);
	}

	//Number of Hist in : St3
	std::shared_ptr<SQHitVector> hv_h3t(UtilSQHit::FindHits(hitVector, "H3T"));
	num_h3t = hv_h3t->size();
	//std::cout << "Num hits in h3t = " << num_h3t << std::endl;
 	for (auto it = hv_h3t->begin(); it != hv_h3t->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time  ();
		tdc_h3t.push_back(tdc_time);
	}
	
	std::shared_ptr<SQHitVector> hv_h3b(UtilSQHit::FindHits(hitVector, "H3B"));
	num_h3b = hv_h3b->size();
	//std::cout << "Num hits in h3b = " << num_h3b << std::endl;
 	for (auto it = hv_h3b->begin(); it != hv_h3b->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h3b.push_back(tdc_time);
	}

	//Number of Hits in : St4
	std::shared_ptr<SQHitVector> hv_h4t(UtilSQHit::FindHits(hitVector, "H4T"));
	num_h4t = hv_h4t->size();
	//std::cout << "Num hits in h4t = " << num_h4t << std::endl;
 	for (auto it = hv_h4t->begin(); it != hv_h4t->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h4t.push_back(tdc_time);
		eleId = (*it)->  get_element_id();
		eleIdsh4t.push_back(eleId);
	}

	std::shared_ptr<SQHitVector> hv_h4b(UtilSQHit::FindHits(hitVector, "H4B"));
	num_h4b = hv_h4b->size();
	//std::cout << "Num hits in h4b = " << num_h4b << std::endl;
 	for (auto it = hv_h4b->begin(); it != hv_h4b->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h4b.push_back(tdc_time);
		eleId = (*it)->  get_element_id();
		eleIdsh4b.push_back(eleId);
	}

	std::shared_ptr<SQHitVector> hv_h4y1l(UtilSQHit::FindHits(hitVector, "H4Y1L"));
	num_h4y1l = hv_h4y1l->size();
	//std::cout << "Num hits in h4y1l = " << num_h4y1l << std::endl;
 	for (auto it = hv_h4y1l->begin(); it != hv_h4y1l->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h4y1l.push_back(tdc_time);
		eleId = (*it)->  get_element_id();
		eleIdsh4y1l.push_back(eleId);
	}

	std::shared_ptr<SQHitVector> hv_h4y1r(UtilSQHit::FindHits(hitVector, "H4Y1R"));
	num_h4y1r = hv_h4y1r->size();
	//std::cout << "Num hits in h4y1r = " << num_h4y1r << std::endl;
 	for (auto it = hv_h4y1r->begin(); it != hv_h4y1r->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h4y1r.push_back(tdc_time);
		eleId = (*it)->  get_element_id();
		eleIdsh4y1r.push_back(eleId);
	}

	std::shared_ptr<SQHitVector> hv_h4y2l(UtilSQHit::FindHits(hitVector, "H4Y2L"));
	num_h4y2l = hv_h4y2l->size();
	//std::cout << "Num hits in h4y2l = " << num_h4y2l << std::endl;
 	for (auto it = hv_h4y2l->begin(); it != hv_h4y2l->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h4y2l.push_back(tdc_time);
		eleId = (*it)->  get_element_id();
		eleIdsh4y2l.push_back(eleId);
	}

	std::shared_ptr<SQHitVector> hv_h4y2r(UtilSQHit::FindHits(hitVector, "H4Y2R"));
	num_h4y2r = hv_h4y2r->size();
	//std::cout << "Num hits in h4y2r = " << num_h4y2r << std::endl;
 	for (auto it = hv_h4y2r->begin(); it != hv_h4y2r->end(); it++) {
		//int    ele_id = (*it)->get_element_id();
		double tdc_time   = (*it)->get_tdc_time();
		tdc_h4y2r.push_back(tdc_time);
		eleId = (*it)->  get_element_id();
		eleIdsh4y2r.push_back(eleId);
	}
	
	nQualTracklets = 0;
	
	for(int i = 0; i < nTracklets; ++i)
	{
		Tracklet* tracklet = trackletVec->at(i);
		nHits = tracklet->getNHits();
		chisq = tracklet->getChisq();

		//very loose cuts here
		if(nHits < 5 ) continue;
		if(chisq > 15.) continue;
		stID = tracklet->stationID;
		
		++nQualTracklets;
		
		 if(stID == 1){tlD0 += 1;}
		 if(stID == 2){tlD1 += 1;}
		 if(stID == 3){tlD2 += 1;}
		 if(stID == 4){tlD3p += 1;}
		 if(stID == 5){tlD3m += 1;}
		 if(stID == 6){tlBackPartial += 1;}
		 if(stID == 7){tlGlobal += 1;}
		
		//tlTree->Fill();

    		for(auto it = detectorIDs.begin(); it != detectorIDs.end(); ++it)
    		{
      			detectorID = *it;

      			double z_exp = p_geomSvc->getPlanePosition(detectorID);
      			x_exp = tracklet->getExpPositionX(z_exp);
      			y_exp = tracklet->getExpPositionY(z_exp);
     			if(!p_geomSvc->isInPlane(detectorID, x_exp, y_exp)) continue;

      			elementID_exp = p_geomSvc->getExpElementID(detectorID, tracklet->getExpPositionW(detectorID));
      			if(elementID_exp < 1 || elementID_exp > p_geomSvc->getPlaneNElements(detectorID)) continue;

      			SQHit* hit = findHit(detectorID, elementID_exp);
      			elementID_closest = hit == nullptr ? -1 : hit->get_element_id();

			detIDs.push_back(detectorID);
			eleID_exps.push_back(elementID_exp);
			eleID_closests.push_back(elementID_closest);
		}
     		tlTree->Fill();
      		detIDs.clear(); eleID_exps.clear(); eleID_closests.clear();
		/*int num_hits = detIDs.size();

		D0 = 0;
		D2 = 0;
		D3 = 0;
		P1 = 0;
		diff1 = 0;
		diff2 = 0;
		diff3 = 0;
		diff4 = 0;

		for(int ii = 0; ii < num_hits; ii++){
			int detID0 = detIDs.at(ii);
			int expID0 = eleID_exps.at(ii);
			int closestID0 = eleID_closests.at(ii);

			// D0
			if(0 < detID0 && detID0 < 7 && closestID0 > 0){
				D0 += 1;
				diff1 += abs(expID0 - closestID0);
			}

			// D2
			if(12 < detID0 && detID0 < 19 && closestID0 > 0){
				D2 += 1;
				diff2 += abs(expID0 - closestID0);
			}

			// D3
			if((18 < detID0 && detID0 < 25 && closestID0 > 0) || (24 < detID0 && detID0 < 31 && closestID0 > 0)){
				D3 += 1;
				diff3 += abs(expID0 - closestID0);
			}

			// PT
			if((46 < detID0 && detID0 < 51 && closestID0 > 0) || (50 < detID0 && detID0 < 55 && closestID0 > 0)){
				P1 += 1;
				diff4 += abs(expID0 - closestID0);
			}
		}

		for(int ii = 0; ii < num_hits; ii++){

			int detID0 = detIDs.at(ii);
			int expID0 = eleID_exps.at(ii);
			int closestID0 = eleID_closests.at(ii);

			if(30 < detID0 && detID0 < 47){
				detID = detID0;
				eleID_exp = expID0;
				eleID_closest = closestID0;
			}

		}*/

       }
  saveTree->Fill();
  tdc_h1t.clear(); tdc_h1b.clear(); tdc_h1r.clear(); tdc_h1l.clear(); 
  tdc_h2t.clear(); tdc_h2b.clear(); tdc_h2r.clear(); tdc_h2l.clear();
  tdc_h3t.clear(); tdc_h3b.clear();
  tdc_h4t.clear(); tdc_h4b.clear(); tdc_h4y1r.clear(); tdc_h4y1l.clear(); tdc_h4y2r.clear(); tdc_h4y2l.clear();
  eleIdsh4t.clear();eleIdsh4b.clear();eleIdsh4y1l.clear();eleIdsh4y1r.clear();eleIdsh4y2l.clear();eleIdsh4y2r.clear();
  ++eventID;
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaModule::End(PHCompositeNode* topNode)
{
  saveFile->cd();
  saveTree->Write();
  tlTree->Write();
  saveFile->Close();

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaModule::GetNodes(PHCompositeNode* topNode)
{
  hitVector   = findNode::getClass<SQHitVector>(topNode, "SQHitVector");
  trackletVec = findNode::getClass<TrackletVector>(topNode, "TrackletVector");
	event       = findNode::getClass<SQEvent    >(topNode, "SQEvent");
	run = findNode::getClass<SQRun    >(topNode, "SQRun");
	
  if(!hitVector || !trackletVec || !event)
  {
    return Fun4AllReturnCodes::ABORTRUN;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

void AnaModule::MakeTree()
{
  saveFile = new TFile(saveName, "RECREATE");

  saveTree = new TTree("save", "Efficiency tree Created by AnaModule");
	saveTree->Branch("eventID", &eventID, "eventID/I");
  	saveTree->Branch("event_ID", &event_ID, "event_ID/I");
	saveTree->Branch("run_ID", &run_ID,"run_ID/I");
	saveTree->Branch("dor", &dor,"dor/I");
	saveTree->Branch("trigger", &trigger, "trigger/I");
	saveTree->Branch("nTracklets", &nTracklets, "nTracklets/I");
	saveTree->Branch("nQualTracklets", &nQualTracklets, "nQualTracklets/I");
	saveTree->Branch("num_h1t", &num_h1t, "num_h1t/I");
	saveTree->Branch("tdc_h1t", &tdc_h1t);
	saveTree->Branch("num_h1b", &num_h1b, "num_h1b/I");
	saveTree->Branch("tdc_h1b", &tdc_h1b);
	saveTree->Branch("num_h1l", &num_h1l, "num_h1l/I");
	saveTree->Branch("tdc_h1l", &tdc_h1l);
	saveTree->Branch("num_h1r", &num_h1r, "num_h1r/I");
	saveTree->Branch("tdc_h1r", &tdc_h1r);

	saveTree->Branch("num_h2t", &num_h2t, "num_h2t/I");
	saveTree->Branch("tdc_h2t", &tdc_h2t);
	saveTree->Branch("num_h2b", &num_h2b, "num_h2b/I");
	saveTree->Branch("tdc_h2b", &tdc_h2b);
	saveTree->Branch("num_h2l", &num_h2l, "num_h2l/I");
	saveTree->Branch("tdc_h2l", &tdc_h2l);
	saveTree->Branch("num_h2r", &num_h2r, "num_h2r/I");
	saveTree->Branch("tdc_h2r", &tdc_h2r);
	
	saveTree->Branch("num_h3t", &num_h3t, "num_h3t/I");
	saveTree->Branch("tdc_h3t", &tdc_h3t);
	saveTree->Branch("num_h3b", &num_h3b, "num_h3b/I");
	saveTree->Branch("tdc_h3b", &tdc_h3b);

	saveTree->Branch("num_h4t", &num_h4t, "num_h4t/I");
	saveTree->Branch("tdc_h4t", &tdc_h4t);
	saveTree->Branch("num_h4b", &num_h4b, "num_h4b/I");
	saveTree->Branch("tdc_h4b", &tdc_h4b);
	saveTree->Branch("num_h4y1l", &num_h4y1l, "num_h4y1l/I");
	saveTree->Branch("tdc_h4y1l", &tdc_h4y1l);
	saveTree->Branch("num_h4y1r", &num_h4y1r, "num_h4y1r/I");
	saveTree->Branch("tdc_h4y1r", &tdc_h4y1r);
	saveTree->Branch("num_h4y2l", &num_h4y2l, "num_h4y2l/I");
	saveTree->Branch("tdc_h4y2l", &tdc_h4y2l);
	saveTree->Branch("num_h4y2r", &num_h4y2r, "num_h4y2r/I");
	saveTree->Branch("tdc_h4y2r", &tdc_h4y2r);

	saveTree->Branch("eleIdsh4t", &eleIdsh4t);
	saveTree->Branch("eleIdsh4b", &eleIdsh4b);
	saveTree->Branch("eleIdsh4y1r", &eleIdsh4y1r);
	saveTree->Branch("eleIdsh4y1l", &eleIdsh4y1l);
	saveTree->Branch("eleIdsh4y2r", &eleIdsh4y2r);
	saveTree->Branch("eleIdsh4y2l", &eleIdsh4y2l);
	
  	/*saveTree->Branch("detID", &detID, "detID/I");
 	saveTree->Branch("eleID_exp", &eleID_exp, "eleID_exp/I");
  	saveTree->Branch("eleID_closest", &eleID_closest, "eleID_closest/I");
	saveTree->Branch("D0", &D0, "D0/I");
	saveTree->Branch("D2", &D2, "D2/I");
	saveTree->Branch("D3", &D3, "D3/I");
	saveTree->Branch("P1", &P1, "P1/I");
	saveTree->Branch("diff1", &diff1, "diff1/I");
	saveTree->Branch("diff2", &diff2, "diff2/I");
	saveTree->Branch("diff3", &diff3, "diff3/I");
	saveTree->Branch("diff4", &diff4, "diff4/I");*/
  	saveTree->Branch("nHits", &nHits, "nHits/I");
 	saveTree->Branch("chisq", &chisq, "chisq/D");
	saveTree->Branch("tlD0", &tlD0, "tlD0/I");
	saveTree->Branch("tlD1", &tlD1, "tlD1/I");
	saveTree->Branch("tlD2", &tlD2, "tlD2/I");
	saveTree->Branch("tlD3p", &tlD3p, "tlD3p/I");
	saveTree->Branch("tlD3m", &tlD3m, "tlD3m/I");
	saveTree->Branch("tlBackPartial", &tlBackPartial, "tlBackPartial/I");
	saveTree->Branch("tlGlobal", &tlGlobal, "tlGlobal/I");

  tlTree = new TTree("tls", "tracklet information");
	tlTree->Branch("eventID", &eventID, "eventID/I");
	tlTree->Branch("event_ID", &event_ID, "event_ID/I");
	tlTree->Branch("run_ID", &run_ID,"run_ID/I");
	tlTree->Branch("stID", &stID, "stID/I");
	tlTree->Branch("detIDs", &detIDs);
	tlTree->Branch("eleID_exps", &eleID_exps);
	tlTree->Branch("eleID_closests", &eleID_closests);
}

void AnaModule::registerDetector(TString name)
{
  detectorIDs.insert(p_geomSvc->getDetectorID(name.Data()));
}

SQHit* AnaModule::findHit(int detID, int eleID)
{
  int delta = 999;
  SQHit* hit = nullptr;
  for(SQHitVector::Iter it = hitVector->begin(); it != hitVector->end(); ++it)
  {
    if((*it)->get_detector_id() != detID) continue;
    int delta_temp = abs(eleID - (*it)->get_element_id());
    if(delta > delta_temp)
    {
      delta = delta_temp;
      hit = (*it);
    }
  }

  return hit;
}
