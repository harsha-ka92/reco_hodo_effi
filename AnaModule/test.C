#include<iostream> 
#include<iterator>
#include<map>

using namespace std;

void test(){
  map<int, int>> runlist;
  
  int runID;
  runlist id;

  ifstream infile("mysql_output.txt");

  while (infile >> id)
  {
      // See if the key/value pair is already
      // in the map
    runlist::iterator it = id.find(runID);

      // If it is present, increment the count (value)
    if (it != id.end())
      it->second++;  // Same as: (*it).second++
    else
    {
        // Create a new pair with value set to 1
      pair<int, int> pr(runID, 1);
      id.insert(pr);
    }
  }
}
