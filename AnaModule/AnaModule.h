#ifndef _ANA_Module__H_
#define _ANA_Module__H_

#include <map>
#include <set>
#include <fun4all/SubsysReco.h>
#include <TString.h>
#include <TVector3.h>
#include <ktracker/SRecEvent.h>
#include <ktracker/FastTracklet.h>
#include <geom_svc/GeomSvc.h>
#include <interface_main/SQHit_v1.h>
#include <vector>
#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQHardSpill.h>

#include "HitData.h"

class TFile;
class TTree;
class SQHitVector;
class SQTrackVector;
class SQDimuonVector;

class AnaModule: public SubsysReco 
{
public:
  AnaModule(const std::string& name = "AnaModule");
  virtual ~AnaModule();

  int Init(PHCompositeNode* topNode);
  int InitRun(PHCompositeNode* topNode);
  int process_run(PHCompositeNode* topNode);
  int process_event(PHCompositeNode* topNode);
  int End(PHCompositeNode* topNode);

  void set_output_filename(const TString& n) { saveName = n; }
  void registerDetector(TString name);

private:
  int GetNodes(PHCompositeNode* topNode);
  void MakeTree();

  SQHit* findHit(int detectorID, int elementID);
  std::set<int> detectorIDs;

  GeomSvc* p_geomSvc;

  // Input
  SQHitVector*    hitVector;
  TrackletVector* trackletVec;
  SQEvent* event;
  SQRun* run;
  SQHardSpill spill;


  // Output
  TString saveName;
  TFile*  saveFile;
  TTree*  saveTree;
  TTree* runTree;
	
//	HitList hit_vec; 

  int eventID;
  int runID;
  int run_ID;
  int dor; // duration of the run
  int tot;
  int detectorID;
  int elementID_exp;
  int elementID_closest;
  double x_exp;
  double y_exp;
  int nHits;
  double chisq;
  int trigger;
  int detID;
  int eleID_exp;
  int eleID_closest;
  int D0;
  int D2;
  int D3;
  int P1;
  int diff1;
  int diff2;
  int diff3;
  int diff4;
};

#endif
