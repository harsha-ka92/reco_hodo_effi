using namespace std;

void test(){

  int i=1;

  struct run{
      int runID;
      int beginT;
      int endT;
  };


  map<int, int> bT;
  map<int, int> eT;

  string id;
  ifstream infile("mysql_output.txt");

  while (getline(infile,id))
  {

    istringstream is( id );
    run r;
    string s;

    getline( is, s, '\t' );
    r.runID = stoi( s );
    getline( is, s, '\t' );
    r.beginT = stoi( s );
    getline( is, s, '\t' );
    r.endT = stoi( s );

    bT.insert(pair<int, int>(r.runID, r.beginT));
    eT.insert(pair<int, int>(r.runID, r.endT));

    cout<<"begin times "<<bT[i]<<endl;
    cout<<"end times "<<eT[i]<<endl;
    cout<<i<<endl;
    i++;
    }
}

