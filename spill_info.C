#include<iostream> 
#include<map>
#include<string>
#include<vector>

using namespace std;

void spill_info(){
 int i=0;
 int dos = 0;
 int sID = 0;
  struct run{
      int runID;
      int spillID;
      int bos_vme;
      int eos_vme;
      int bos_coda_id;
      int eos_coda_id;
  };


  std::map<pair<int,int>, pair<int,int>> spill_time;

  string id;
  ifstream infile("mysql_output_spill_info.txt");
  TGraph *gSpillTime = new TGraph();
  while (getline(infile,id))
  {
 
    istringstream is( id );
    run r;
    is >> r.runID >> r.spillID >> r.bos_coda_id >> r.bos_vme >> r.eos_coda_id >> r.eos_vme;
   
    spill_time[std::pair<int,int>(r.runID, r.spillID)] = std::pair<int,int>(r.bos_vme, r.eos_vme); 
    
    }

  for (auto it = spill_time.begin(); it != spill_time.end(); it++) {
    //cout << it->first.first << "\t" << it->first.second << "\t" << it->second.first << "\t" << it->second.second << "\n";
    dos = it->second.second - it->second.first;
    sID = it->first.second; 
    //std::cout << "Spill ID "<< sID << std::endl;
    //std::cout << "dos"<< dos << std::endl;
    gSpillTime->SetPoint(i,i, dos);
    i++;
  }

  TCanvas *c1 = new TCanvas("c1","Spill Durations",200,10,600,400);
  gSpillTime->Draw("AF");
  c1->SaveAs("Spill_Durations.png");
}
