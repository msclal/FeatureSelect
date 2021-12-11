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
      vector<vector<double>> dataSet; //training set
      vector<int>bestDataSet; //features w highest accuracy
      int sizeOfInstances = 0;
      int sizeOfFeatures = 0;
      int whichAlgorithm = 0; //algorithm choice (forwards or backwards)
  public:
  //let user choose search algorithm
    void chooseAlgorithm() {
          int userChoice;
          cout << "Choose an algorithm." << endl; 
          cout << "   1) Forward Selection" << endl; 
          cout << "   2) Backward Elimination" << endl << endl;
          do{cin >> whichAlgorithm;}
          while(whichAlgorithm < 0 || whichAlgorithm > 2); 
      }
    //get test file data 
    void importFileData() {
          vector<double> temp_features;
          string row, feat;
          ifstream inFS;
          
          //let user choose input file or have them input custom file
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

          //open selected file input
          inFS.open(filename);
          if(inFS.is_open()) {
              getline(inFS, row); 
              stringstream sstream(row); 

              //while theres still features
              while(sstream >> feat) { 
                  double d = stod(feat);
                  temp_features.push_back(d); 
                  dataSet.push_back(temp_features); 
                  temp_features.clear(); 
                  sstream.clear();
              } 

              //while theres still instances, get features
              while(getline(inFS, row) ) { 
                  stringstream sstream(row); 
                  for(int i = 0; i < dataSet.size(); i++) { 
                      if(sstream >> feat)
                        dataSet.at(i).push_back(stold(feat));
                  } 
              } 
          }
          else {
              //exit program if theres issue loading file input
              cout << "Error loading test_file." << endl; 
              exit(EXIT_FAILURE); 
          }
          
          //close the file
          inFS.close(); 
      }

      //
      int nearestNeighbor(vector< vector<double>>instanceSet, vector<double> s) { 
          double shortestDist = 0.0;
          int classID = s.at(0); 

          //calculate the minimum distance for each instance in the subset
          for(int i = 0; i < instanceSet.at(0).size(); i++) { 
              double distance = 0.0; 
              for(int j = 1; j < instanceSet.size(); j++) {
                  distance += pow(instanceSet.at(j).at(i) - s.at(j), 2);  //calculate euclidian distance
              }
              distance = sqrt(distance);
              
              if(shortestDist >= distance) 
              //set the instance to its correspoinding class (either class 1 or 2)
                  shortestDist = distance, classID = static_cast<int>(instanceSet.at(0).at(i)); 
              else if(i == 0) shortestDist = distance;   //class 1
          } 
          return classID; 
      } 
      
      //project 2 briefing slides for cross validation
      double leave_one_out_cross_validate(vector<vector<double>> featureSet) { 
          vector<vector <double>> instanceSet; vector<double> s; 
          double num_correctly_classified = 0; 

          for(int i = 0; i < featureSet.at(0).size(); i++) { 
              for(int j = 0; j < featureSet.size(); j++) {
                //copy features to current data set
                  vector<double> temp_dataSet = featureSet.at(j); 
                  s.push_back(temp_dataSet.at(i));
                  temp_dataSet.erase(temp_dataSet.begin() + i);
                  instanceSet.push_back(temp_dataSet); 
              }
              
              if(static_cast<int>(s.at(0)) == static_cast<int>(nearestNeighbor(instanceSet, s)))
                  num_correctly_classified++; //increase counter if class is correctly correlated
              s.erase(s.begin(), s.end()), instanceSet.erase(instanceSet.begin(), instanceSet.end());
          } 
          //accuracy = number of correct classfications / number of instances
          return (num_correctly_classified/featureSet.at(0).size())*100; 
      } 

      //checks if an element has been previously visited before by returning its index
      int indexInSet(vector<int> v, int x) {
          for(int i = 0; i < v.size(); ++i) {
              if(x == v.at(i)) {
                  return i; //element found
              }
          }
          //not found
          return -1;
      }

      void forwardSelection() {
          vector<int> local_dataSet, bestTemp_feature; 
          double accuracy, max_accuracy, bestSoFar = 0.0;  
          
          //project 2 briefiing slides search
          for(int i = 1; i < dataSet.size() - 1; i++) {
              for(int j = 1; j <= dataSet.size() - 1; j++) {
                  vector<vector<double>> temp_dataSet; 
                  vector<int> temp_features; 
                  temp_dataSet.push_back(dataSet.at(0)); 
                  temp_features = local_dataSet; //copy previous iteraton data set to curr data set

                      //copy previous iteraton data set to curr temp subset
                      for(auto k : temp_features) {
                          temp_dataSet.push_back(dataSet.at(k));
                      }
                      
                      //if feature has been used before go to next iteration
                      if(indexInSet(temp_features, j) != -1) continue;

                      //feature hasnt been used before so include it in current selected data
                      temp_dataSet.push_back(dataSet.at(j)); 
                      temp_features.push_back(j); 
                      //calculate accuracy of current data set using NN and cross validation
                      accuracy = leave_one_out_cross_validate(temp_dataSet); 

                      //track the temporary subset w the highest accuracy
                      if(accuracy > bestSoFar)
                          bestSoFar = accuracy, bestTemp_feature = temp_features;

                      cout << "   Using feature(s) {"; 
                      displaySet(temp_features);
                      cout << "} -- accuracy is " << accuracy << "%" <<  endl; 
              } 

              //copy the best curr set to the selected features
              local_dataSet = bestTemp_feature; 
              cout << "   Feature set {"; 
              displaySet(local_dataSet);
              cout << "} was best with accuracy of " << bestSoFar << "%" << endl << endl; 
              
              //track the feature set w the highest accuracy
              if(bestSoFar >= max_accuracy)
                  bestDataSet = local_dataSet, max_accuracy = bestSoFar;
              else if(bestSoFar < max_accuracy && i != sizeOfFeatures-1)
                  cout << "(Warning! Accuracy is decreasing. Continuing check in case of local maxima)" << endl;

              bestSoFar = 0.0;
          }
          //output best data set features
              cout << endl << "Finished searched! The best feature set is: {";
              displaySet(bestDataSet);
              cout << "}, with an accuracy of " << max_accuracy << "%" << endl; 
      }

      void backwardsElimination() { 
          vector<int> local_dataSet = bestDataSet, bestTemp_features; 
          double accuracy, max_accuracy, bestSoFar = 0.0; 

           //project 2 briefiing slides search
          for(int i = 1; i < sizeOfFeatures; i++) {
              for(int j = 0; j < sizeOfFeatures; j++) {
                  vector<vector<double>> temp_dataSet; 
                  vector<int> temp_features; 
                  temp_dataSet.push_back(dataSet.at(0)); 
                  temp_features = local_dataSet; //copy previous iteraton data set to curr data set
                  
                  //copy previous iteraton data set to curr temp sub set
                  for(auto k : local_dataSet)
                      temp_dataSet.push_back(dataSet.at(k));
                      
                      //itereator to the curr feature index
                      vector<int>::iterator it = find(temp_features.begin(), temp_features.end(), j);     
                      //if feature has been used before go to next iteration
                      if(it == temp_features.end()) continue;

                      //delete feature from the curr data set
                      int fork = it - temp_features.begin() + 1; 
                      temp_features.erase(it);
                      temp_dataSet.erase(temp_dataSet.begin() + fork); 
                      //calculate accuracy of current data set using NN and cross validation
                      accuracy = leave_one_out_cross_validate(temp_dataSet); 

                      //track the temporary subset w the highest accuracy
                      if(accuracy >= bestSoFar)
                          bestSoFar = accuracy, bestTemp_features = temp_features;

                      cout << "   Using feature(s) {"; 
                      displaySet(temp_features);
                      cout << "} -- accuracy is " << accuracy << "%" <<  endl; 
              }    
              //copy the best curr set to the selected features
              local_dataSet = bestTemp_features; 
              cout << "   Feature set {"; 
              displaySet(local_dataSet);
              cout << "} was best with accuracy of " << bestSoFar << "%" << endl << endl; 
              
              //track the feature set w the highest accuracy
              if(bestSoFar >= max_accuracy) bestDataSet = local_dataSet, max_accuracy = bestSoFar;
              else if(bestSoFar < max_accuracy && i != sizeOfFeatures-1)
                  cout << "(Warning! Accuracy is decreasing. Continuing check in case of local maxima)" << endl;
              bestSoFar = 0.0;  
          } 

          //output best data set features
              cout << endl << "Finished searched! The best feature set is: {";
              displaySet(bestDataSet);
              cout << "}, with an accuracy of " << max_accuracy << "%" << endl; 
      }

      //gets number of features, instances, and calls the search algorithm we want
      void simulateTest() {
          sizeOfFeatures = dataSet.size() - 1;
          sizeOfInstances = dataSet.at(0).size();

          cout << "This dataset has " << sizeOfFeatures << " (not including the class atribute), with " << sizeOfInstances << " instances." << endl;
          cout << "Running nearest neighbor with all " << sizeOfFeatures << " features, using \"leaving-one-out\" evaluation, I get an ";
          cout << "accuracy of " << leave_one_out_cross_validate(dataSet) << "%" << endl;
          
          cout << "Beginning search." << endl << endl;

          setTime(1); //start time

          //call algorithm
          if(whichAlgorithm == 1) forwardSelection();
          else {
            //set set to increasing order of elements from 1 to number of features of the data set
              for(int i = 1; i < sizeOfFeatures; i++) {
                bestDataSet.push_back(i);
              } 
            backwardsElimination();
          }
          setTime(0); //end time
      }

      //helper function to print set
      void displaySet(vector<int> &dataSet) {
          for(int i = 0; i < dataSet.size(); i++) {
              cout << dataSet.at(i);
              if(i != dataSet.size()-1) cout << ", ";
          } 
      }

      //stopwatch for our search algorithm 
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
