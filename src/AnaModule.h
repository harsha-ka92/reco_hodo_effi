#ifndef _ANA_Module__H_
#define _ANA_Module__H_

#include <map>
#include <set>
#include <fun4all/SubsysReco.h>
#include <TString.h>
#include <TVector3.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <ktracker/SRecEvent.h>
#include <ktracker/FastTracklet.h>
#include <geom_svc/GeomSvc.h>
#include <interface_main/SQHit_v1.h>


class TFile;
class TTree;
class SQHitVector;
class SQTrackVector;
class SQDimuonVector;


struct detData{
    int eventID;
    int trigger_bits[5];
    int detectorID;
    int nHits;
    double chisq;
    int elementID_exp;
    int elementID_closest;
};

class AnaModule: public SubsysReco 
{
public:
  AnaModule(const std::string& name = "AnaModule");
  virtual ~AnaModule();

  int Init(PHCompositeNode* topNode);
  int InitRun(PHCompositeNode* topNode);
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

  // Output
  TString saveName;
  TFile*  saveFile;
  TTree*  saveTree;

  detData* dd;

  std::vector<detData*> dv;

  int eventID;
  int detectorID;
  int elementID_exp;
  int elementID_closest;
  double x_exp;
  double y_exp;
  int nHits;
  double chisq;
};

#endif
