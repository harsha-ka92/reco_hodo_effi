using namespace std;

void test(){

  int i=1;

  struct run{
      int runID;
      int beginT;
      int endT;
  };


  map<int, pair<int,int>> run_time;
  //map<int, int> eT;

  string id;
  ifstream infile("AnaModule/mysql_output.txt");

  while (getline(infile,id))
  {
    run r;
    istringstream is( id );
    is >> r.runID >> r.beginT >> r.endT;

    run_time[r.runID]=pair<int,int>(r.beginT, r.endT);

    }
  for (auto it = run_time.begin(); it != run_time.end(); it++){
  cout<< "runID " << it->first << endl;
  cout<< "begin time " << it->second.first << " s"<< endl;
  cout<< "end time " << it->second.second << " s"<< endl;
  cout<< "duration " << it->second.second - it->second.first << " s"<< endl;
  }
}
