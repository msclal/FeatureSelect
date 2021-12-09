#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector>

using namespace std; 

clock_t start;

void setTime(int i){
    if(i == 1) start = clock();
    else if(i == 0) cout << "Elapsed time: " << (clock() - start)/(double) CLOCKS_PER_SEC << " seconds.\n";
}


vector< vector<long double>> importFileData() {
 	  string filename;
 	  ifstream inFS;
    cout << "Type in the name of the file to test: "; 
    cin >> filename;  
    cout << endl;

    inFS.open(filename);
    if(!inFS.is_open()) {
        cout << "Error opening file." << endl; 
        exit(1); 
    }
    
	vector< vector<long double> > dataSet;
    vector <long double> feat; 
    string row; 
    
    getline(inFS, row); 
    std::stringstream sstream(row); 
    string feature; 
    while(sstream >> feature) { 
        feat.push_back(stold(feature)); 
        dataSet.push_back(feat); 
        feat.clear(); 
    } 

    while(getline(inFS, row) ) { 
        std::stringstream sstream(row); 
        for(unsigned int i = 0; i < dataSet.size(); i++) { 
            if(sstream >> feature)
              dataSet.at(i).push_back(stold(feature));
        } 
    } 
    
    inFS.close(); 
    return dataSet;
}

void displaySet(vector<int> &dataSet) {
      for(int i = 0; i < dataSet.size(); i++) {
            cout << dataSet.at(i);
              if(i != dataSet.size()-1) cout << ", ";
        } 
    }

void simulateTest(vector< vector<long double>> dataSet) {
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

int main() {
    cout << "Welcome to Mika Shanela's Feature Selection Algorithm." << endl;
    vector< vector<long double> > dataSet = importFileData();
    // get user input for algo

    // simulate algo

    return 0; 
} 
