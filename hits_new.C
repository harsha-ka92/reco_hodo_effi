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

  nPlanes = 16;
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
  }

  run_ID_temp = run_ID;	

  if (nTracklets == 0) {return Fun4AllReturnCodes::EVENT_OK;}

  trigger = event->get_trigger();

   for(SQHitVector::Iter it = hitVector->begin(); it != hitVector->end(); ++it)
 	{	
    		int det = (*it)->get_detector_id(); 
		if ( 30 < det && det < 47 ) {hitInfo[det-31].push_back((*it)->get_tdc_time());}
		
 	}
	
  ++eventID;
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaModule::End(PHCompositeNode* topNode)
{
  saveFile->cd();
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
  saveFile->WriteObject(&hitInfo,"hitInfo");
}

void AnaModule::registerDetector(TString name)
{
  detectorIDs.insert(p_geomSvc->getDetectorID(name.Data()));
}
