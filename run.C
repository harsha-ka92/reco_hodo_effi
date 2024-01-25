#include <TSystem.h>

R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libanamodule)

int run(const char* fn_list_run="list_reco.txt")
{
  const double FMAGSTR = -1.054;
  const double KMAGSTR = -0.951;
  const bool cosmic = true;
  const int nEvents = -1;

  recoConsts* rc = recoConsts::instance();
  rc->set_DoubleFlag("FMAGSTR", FMAGSTR);
  rc->set_DoubleFlag("KMAGSTR", KMAGSTR);
  if(cosmic)
  {
    rc->init("cosmic");
    rc->set_BoolFlag("COARSE_MODE", true);
    rc->set_DoubleFlag("KMAGSTR", 0.);
    rc->set_DoubleFlag("FMAGSTR", 0.);
  }
  rc->Print();

  GeomSvc::UseDbSvc(true);  
  GeomSvc* geom_svc = GeomSvc::instance();

  Fun4AllServer* se = Fun4AllServer::instance();
  se->Verbosity(0);

  AnaModule* ana = new AnaModule();
  ana->set_output_filename("ana.root");
  ana->registerDetector("H1B");     //register detector by its name, all detectors that do not directly partipate the tracking can be used
  ana->registerDetector("H1T");
  se->registerSubsystem(ana);

  Fun4AllInputManager* in = new Fun4AllDstInputManager("DSTIN");
  in->Verbosity(0);
  //in->fileopen("data.root");
  se->registerInputManager(in);

  //we do not really need an output manager
  //Fun4AllDstOutputManager* out = new Fun4AllDstOutputManager("DSTOUT", "result.root");
  //se->registerOutputManager(out);

  ifstream ifs(fn_list_run);
  string fn_in;
  while (ifs >> fn_in) {
    //string fn_in = UtilOnline::GetDstFilePath(run);
    std::cout << "---> using reco file " << fn_in << std::endl;
    in->fileopen(fn_in);
    se->run(nEvents);
  }
  ifs.close();

  //se->run(nEvents);

  // finish job - close and save output files
  se->End();
  se->PrintTimer();
  std::cout << "All done" << std::endl;

  delete se;
  gSystem->Exit(0);
  return 0;
}
