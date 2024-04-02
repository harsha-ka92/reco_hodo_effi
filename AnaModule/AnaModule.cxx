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
	std::cout<<"*********map completed**********"<<std::endl;
	for (auto it = run_time.begin(); it != run_time.end(); it++){
  	std::cout<< "runID " << it->first << std::endl;
 	std::cout<< "begin time " << it->second.first << " s"<< std::endl;
 	std::cout<< "end time " << it->second.second << " s"<< std::endl;
 	std::cout<< "duration " << it->second.second - it->second.first << " s"<< std::endl;
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
	//if (nTracklets == 0) {return Fun4AllReturnCodes::EVENT_OK;}
	
	//std::cout << "Event ID : " << event_ID << std::endl;
	trigger = event->get_trigger();
	//std::cout<< "triger bit of the event "<< trigger << std::endl;
	
	for(SQHitVector::Iter it = hitVector->begin(); it != hitVector->end(); ++it)
 	{	
    		int det = (*it)->get_detector_id(); 
		if ( 30 < det && det < 46 ) {hitInfo[det].push_back(pair<int, double> ((*it)->get_element_id(), (*it)->get_tdc_time());}
		
 	}
	
	nQualTracklets = 0;
	
	for(int i = 0; i < nTracklets; ++i)
	{
		Tracklet* tracklet = trackletVec->at(i);

		//very loose cuts here
		if(tracklet->getNHits() < 5 ) continue;
		if(tracklet->getChisq() > 15.) continue;
		
		stID = tracklet->stationID;
		nHits = tracklet->getNHits();
		chisq = tracklet->getChisq();
		
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
		
       }
  saveTree->Fill();
  hitInfo.clear();
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
	saveTree->Branch("hitInfo", &hitInfo);
	
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
	tlTree->Branch("nHits", &nHits, "nHits/I");
 	tlTree->Branch("chisq", &chisq, "chisq/D");
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
