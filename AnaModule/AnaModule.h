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
//#include <packages/UtilAna/UtilSQHit.h>

#include "HitData.h"
//#include "HodoConst.h"

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
  
  //create the structure to read txt file
  struct run_info{
      int runID;
      int beginT;
      int endT;
  };
  run_info r;

  //map
  std::map<int, std::pair<int,int>> run_time;
  std::string id;

  
  // Output
  TString saveName;
  TFile*  saveFile;
  TTree*  saveTree;
  TTree* tlTree;
  TTree* hitTree;
	
//	HitList hit_vec; 

  int eventID;
  int event_ID;
  int runID;
  int run_ID;
  int nTracklets;
  int run_ID_temp;
  int dor; // duration of the run
  int detectorID;
  int elementID_exp;
  int elementID_closest;
  double x_exp;
  double y_exp;
  double tdc_time;
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

// Tracklet info
  int tlD0, tlD1, tlD2, tlD3p, tlD3m, tlBackPartial, tlGlobal;
  int stID;

//Hit info
  #ifdef __MAKECINT__
  #pragma link C++ class vector<double>+;
  #endif

  std::vector<double> tdc_h1t;
  std::vector<double> tdc_h1b;
  std::vector<double> tdc_h1r;
  std::vector<double> tdc_h1l;

  std::vector<double> tdc_h2t;
  std::vector<double> tdc_h2b;
  std::vector<double> tdc_h2r;
  std::vector<double> tdc_h2l;

  std::vector<double> tdc_h3t;
  std::vector<double> tdc_h3b;

  std::vector<double> tdc_h4t;
  std::vector<double> tdc_h4b;
  std::vector<double> tdc_h4y1r;
  std::vector<double> tdc_h4y1l;
  std::vector<double> tdc_h4y2r;
  std::vector<double> tdc_h4y2l;

  std::string ID;
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
  int num_h4t; int num_h4b; int num_h4y1l; int num_h4y1r; int num_h4y2l; int num_h4y2r;
};

#endif
