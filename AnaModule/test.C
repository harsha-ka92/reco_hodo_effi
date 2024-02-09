#include<iostream> 
#include<iterator>

using namespace std;

void test(){
  typedef std::map<std::string, std::vector<float>> runlist;
  
  std::int runID;
  runlist ID;

  std::ifstream infile("mysql_output.txt");

  while (infile >> ID)
  {
      // See if the key/value pair is already
      // in the map
    runlist::iterator it = ID.find(runID);

      // If it is present, increment the count (value)
    if (it != ID.end())
      it->second++;  // Same as: (*it).second++
    else
    {
        // Create a new pair with value set to 1
      std::pair<std::string, int> pr(runID, 1);
      ID.insert(pr);
    }
  }
}
