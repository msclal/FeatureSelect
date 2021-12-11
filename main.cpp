#include <iostream> 
#include <sstream> 
#include <fstream> 
#include <vector>
#include <cmath>
#include <algorithm>

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
          cout << "Choose an algorithm." << endl; 
          cout << "   1) Forward Selection" << endl; 
          cout << "   2) Backward Elimination" << endl << endl;
          do{cin >> whichAlgorithm;}
          while(whichAlgorithm < 0 || whichAlgorithm > 2); 
      }
    void importFileData() {
          vector<double> temp_features;
          string row, feat;
          ifstream inFS;
          
          int userChoice = 0; string filename;
          cout << "Choose test_file." << endl;
          cout << "   (1) SMALL_42" << endl;
          cout << "   (2) LARGE_67" << endl;
          cout << "   (3) CUSTOM FILE" << endl;
          string f1 = "Ver_2_CS170_Fall_2021_Small_data__42.txt";
          string f2 = "Ver_2_CS170_Fall_2021_LARGE_data__22.txt";

          do{cin >> userChoice;}
          while(userChoice < 1 || userChoice > 3);
          if(userChoice == 1) filename = f1;
          else if (userChoice == 2) filename = f2;
          else {
            cout << "Enter test_file name: ";
            cin >> filename;
          }
          // cin >> filename;  
          cout << endl;

          inFS.open(filename);
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

      int nearestNeighbor(vector< vector<double>>instanceSet, vector<double> s) { 
          double shortestDist = 0.0;
          int classID = s.at(0); 
          for(int i = 0; i < instanceSet.at(0).size(); i++) { 
              double distance = 0.0; 
              for(int j = 1; j < instanceSet.size(); j++) {
                  distance += pow(instanceSet.at(j).at(i) - s.at(j), 2);  //calculate euclidian distance
              }
              distance = sqrt(distance);         
              if(i == 0) shortestDist = distance;   //class 1
              else if(shortestDist >= distance)     //class 2
                  shortestDist = distance, classID = static_cast<int>(instanceSet.at(0).at(i));
          } 
          return classID; 
      } 

      double leave_one_out_cross_validate(vector<vector<double>> featureSet) { 
          vector<vector <double>> instanceSet; vector<double> s; 
          double num_correctly_classified = 0; 

          for(int i = 0; i < featureSet.at(0).size(); i++) { 
              for(int j = 0; j < featureSet.size(); j++) {
                  vector<double> temp_dataSet = featureSet.at(j); 
                  s.push_back(temp_dataSet.at(i));
                  temp_dataSet.erase(temp_dataSet.begin() + i);
                  instanceSet.push_back(temp_dataSet); 
              }
              if(static_cast<int>(s.at(0)) == static_cast<int>(nearestNeighbor(instanceSet, s)))
                  num_correctly_classified++;
              s.erase(s.begin(), s.end()), instanceSet.erase(instanceSet.begin(), instanceSet.end());
          } 
          return (num_correctly_classified/featureSet.at(0).size())*100; 
      } 

      int indexInSet(vector<int> v, int x) {        //checks if an element has been previously visited before
          for(int i = 0; i < v.size(); ++i) {
              if(x == v.at(i)) {
                  return i;
              }
          }
          return -1;
      }

      void forwardSelection() {
          vector<int> local_dataSet, bestTemp_feature; 
          double accuracy, max_accuracy, bestSoFar = 0.0;  
          
          for(int i = 1; i < dataSet.size() - 1; i++) {
              for(int j = 1; j <= dataSet.size() - 1; j++) {
                  vector<vector<double>> temp_dataSet; 
                  vector<int> temp_features; 
                  temp_dataSet.push_back(dataSet.at(0)); 
                  temp_features = local_dataSet; 
                  
                      for(auto k : temp_features) {
                          temp_dataSet.push_back(dataSet.at(k));
                      }
                      
                      if(indexInSet(temp_features, j) != -1) continue;

                      temp_dataSet.push_back(dataSet.at(j)); 
                      temp_features.push_back(j); 
                      accuracy = leave_one_out_cross_validate(temp_dataSet); 
                      if(accuracy > bestSoFar)
                          bestSoFar = accuracy, bestTemp_feature = temp_features;

                      cout << "   Using feature(s) {"; 
                      displaySet(temp_features);
                      cout << "} -- accuracy is " << accuracy << "%" <<  endl; 
              } 

              local_dataSet = bestTemp_feature; 
              cout << "   Feature set {"; 
              displaySet(local_dataSet);
              cout << "} was best with accuracy of " << bestSoFar << "%" << endl << endl; 
              
              if(bestSoFar >= max_accuracy)
                  bestDataSet = local_dataSet, max_accuracy = bestSoFar;
              else if(bestSoFar < max_accuracy && i != sizeOfFeatures-1)
                  cout << "(Warning! Accuracy is decreasing. Continuing check in case of local maxima)" << endl;

              bestSoFar = 0.0;
          }
              cout << endl << "Finished searched! The best feature set is: {";
              displaySet(bestDataSet);
              cout << "}, with an accuracy of " << max_accuracy << "%" << endl; 
      }

      void simulateTest() {
          sizeOfFeatures = dataSet.size() - 1;
          sizeOfInstances = dataSet.at(0).size();

          cout << "This dataset has " << sizeOfFeatures << " (not including the class atribute), with " << sizeOfInstances << " instances." << endl;
          cout << "Running nearest neighbor with all " << sizeOfFeatures << " features, using \"leaving-one-out\" evaluation, I get an ";
          cout << "accuracy of " << leave_one_out_cross_validate(dataSet) << "%" << endl;
          
          cout << "Beginning search." << endl << endl;

          setTime(1);
          if(whichAlgorithm == 1) forwardSelection();
          else cout << "Backwards" << endl;
          setTime(0);
      }

      void displaySet(vector<int> &dataSet) {
          for(int i = 0; i < dataSet.size(); i++) {
              cout << dataSet.at(i);
              if(i != dataSet.size()-1) cout << ", ";
          } 
      }

      void setTime(int i){
          if(i == 1)
            start = clock();
          else if(i == 0) cout << "Elapsed time: " << (clock() - start)/static_cast<double>CLOCKS_PER_SEC << " seconds.\n";
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
