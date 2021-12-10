#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector>

using namespace std; 

clock_t start;

class FeatureSelection {
  private:
      vector<vector<double>> dataSet;
      vector<int>bestDataSet;
      int sizeOfInstances = 0;
      int sizeOfFeatures = 0;
      int whichAlgorithm = 0;
  public:
    void chooseAlgorithm() {
          int userChoice;
          cout << "Type the number of the algorithm you want to run." << endl; 
          cout << "   1) Forward Selection" << endl; 
          cout << "   2) Backward Elimination" << endl << endl;

          whichAlgorithm = 1;
          // do{cin >> whichAlgorithm;}
          // while(whichAlgorithm < 0 || whichAlgorithm > 2); 
      }
    void importFileData() {
          vector<double> temp_features;
          string row, feat;
          ifstream inFS;
          
          cout << "Enter test_file name: " << endl;
          cout << "   (1) SMALL_42" << endl;
          cout << "   (2) LARGE_67" << endl;
          // string filename = "Ver_2_CS170_Fall_2021_Small_data__42.txt";
          string filename = "Ver_2_CS170_Fall_2021_LARGE_data__22.txt";
          // cin >> filename;  
          cout << endl;

          inFS.open(filename.c_str());
          if(inFS.is_open()) {
              getline(inFS, row); 
              stringstream sstream(row); 
              while(sstream >> feat) { 
                  double d = stod(feat);
                  temp_features.push_back(d); 
                  dataSet.push_back(temp_features); 
                  temp_features.clear(); 
                  sstream.clear();
              } 

              while(getline(inFS, row) ) { 
                  stringstream sstream(row); 
                  for(int i = 0; i < dataSet.size(); i++) { 
                      if(sstream >> feat)
                        dataSet.at(i).push_back(stold(feat));
                  } 
              } 
          }
          else {
              cout << "Error loading test_file." << endl; 
              exit(EXIT_FAILURE); 
          }
          
          inFS.close(); 
      }

      void simulateTest() {
        int sizeOfFeatures = dataSet.size() - 1;
        int sizeOfInstances = dataSet.at(0).size();
        
        cout << "This dataset has " << sizeOfFeatures << " (not including the class atribute), with " << sizeOfInstances << " instances." << endl;
        cout << "Running nearest neighbor with all " << sizeOfFeatures << " features, using \"leaving-one-out\" evaluation, I get an ";
        cout << "accuracy of --- %" << endl;
              
        cout << "Beginning search." << endl << endl;

        setTime(1);
        int whichAlgorithm;
        cin >> whichAlgorithm;
        if(whichAlgorithm == 1) cout << "forward" << endl;
        else cout << "backward" << endl;
        setTime(0);
      }

      void displaySet(vector<int> &dataSet) {
          for(int i = 0; i < dataSet.size(); i++) {
              cout << dataSet.at(i);
              if(i != dataSet.size()-1) cout << ", ";
          } 
      }

      void setTime(int i){
          if(i == 1) start = clock();
          else if(i == 0) cout << "Elapsed time: " << (clock() - start)/(double) CLOCKS_PER_SEC << " seconds.\n";
      }

};

int main() {
    FeatureSelection set;
    cout << "Welcome to Mika Shanela's Feature Selection Algorithm." << endl;
    set.importFileData();
    set.chooseAlgorithm();
    set.simulateTest();

    return 0; 
} 
