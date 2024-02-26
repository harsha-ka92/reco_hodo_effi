#include<iostream> 
#include<map>
#include<string>
#include<vector>

using namespace std;

void spill_info(){

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

  while (getline(infile,id))
  {
 
    istringstream is( id );
    run r;
    is >> r.runID >> r.spillID >> r.bos_coda_id >> r.bos_vme >> r.eos_coda_id >> r.eos_vme;
   
    spill_time[std::pair<int,int>(r.runID, r.spillID)] = std::pair<int,int>(r.bos_vme, r.eos_vme); 
    
    }

  for (auto it = spill_time.begin(); it != spill_time.end(); it++) {
    cout << it->first.first << "\t" << it->first.second << "\t" << it->second.first << "\t" << it->second.second << "\n";
  }
}
