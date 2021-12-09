#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector>

using namespace std; 

vector< vector<long double> > importFileData() {
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


int main() {
    cout << "Welcome to Mika Shanela's Feature Selection Algorithm." << endl;
    vector< vector<long double> > dataSet = importFileData();
    // get user input for algo

    // simulate algo

    return 0; 
} 
