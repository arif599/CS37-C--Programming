//----------------------------------------------------------------------//
// Assignment #1                                                        //
//----------------------------------------------------------------------//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
const double eRadius = 6371;
const double pi = 4.0*atan(1.0);

struct Location{
    double latitude;
    double longitude;
};

struct Park{
    Location pkLoc;
    string pkName;
    string pkState;
    string pkCountry;
};

struct Distance{
    double pkDistance;
    int pkIndex;
};

bool openFile(ifstream &inFile, string fname);
int calcTotalParks(ifstream &inFile);
void populateArr(ifstream &inFile, int size, Park parkArr[]);
void calcDistance(Park &startingPark, Park &closestPark, Park &farthestPark, Park parkArr[], Distance distanceArr[], int totalParks);
void displayOutput(Park &startingPark, Park &closestPark, Park &farthestPark, Distance distanceArr[], int totalParks);

//finds the distance between two points on the surface of the Earth from a given file
int main()
{
    Park startLoc = {{33.675373, -117.777549},"Irvine Valley College", "CA", "US"};
    ifstream myFile;
    Park closeLoc;
    Park FarLoc;

    if(openFile(myFile, "npproject.txt")){
        int totalParks = calcTotalParks(myFile);
        Park *pParkLoc = new Park[totalParks];
        Distance *pDistanceArr = new Distance[totalParks];

        populateArr(myFile, totalParks, pParkLoc);
        calcDistance(startLoc, closeLoc, FarLoc, pParkLoc, pDistanceArr, totalParks);
        displayOutput(startLoc, closeLoc, FarLoc, pDistanceArr, totalParks);

        myFile.close();
        delete []pParkLoc;
        delete []pDistanceArr;
    }
}

//opens file and inserts the information of the park into an Park array
void populateArr(ifstream &inFile, int size, Park parkArr[]){
    string line;
    double inLoc;
    int i = 0;

    for (int j = 0; getline(inFile,line) && j < size+6; j++){
        if (j >= 6){
            parkArr[i].pkName = line;
            getline(inFile,line);
            parkArr[i].pkState = line;
            getline(inFile,line);
            parkArr[i].pkCountry = line;

            inFile >> inLoc;
            parkArr[i].pkLoc.latitude = inLoc;
            inFile >> inLoc;
            parkArr[i].pkLoc.longitude = inLoc;

            /*
            (check) print to check if Array has been properly populated
                cout << parkArr[i].pkName << endl;
                cout << parkArr[i].pkState << endl;
                cout << parkArr[i].pkCountry << endl;
                cout << parkArr[i].pkLoc.latitude << endl;
                cout << parkArr[i].pkLoc.longitude << endl;
                cout << endl;
            */

            i++;
            inFile.ignore();
        }
    }
}

//calculates the total number of parks in the file (excluding the starting park)
int calcTotalParks(ifstream &inFile){
    string line;
    int lineCounter = 0;

    if(inFile.is_open()){
        while(getline(inFile, line)){
            lineCounter++;
        }
    }

    inFile.clear();
    inFile.seekg(ios::beg);
    return (lineCounter-6)/5;
}

//checks to see if the file was succesfully opened 
bool openFile(ifstream &inFile, string fname){
    inFile.open(fname.c_str());
    if (inFile.good()) {
        cout << setw(25) << setfill('-')<< "-" << "Input file opened" << setw(25) << setfill('-') << "-" <<endl;
        return true;
    }else{
        cout << "Error. Input file not opened" << endl;
        return false;
    }
}

//calculates the distance between two parks and finds the closest and farthest park
void calcDistance(Park &startingPark, Park &closestPark, Park &farthestPark, Park parkArr[], Distance distanceArr[], int totalParks){
    double lat1 = startingPark.pkLoc.latitude * pi/180;
    double lat2;
    double long1 = startingPark.pkLoc.longitude * pi/180;
    double long2;
    double latDiff;
    double longDiff;
    double temp;
    double angRad;
    double distance;

    for(int i = 0; i < totalParks; i++){
        lat2= parkArr[i].pkLoc.latitude * pi/180;
        long2 = parkArr[i].pkLoc.longitude * pi/180;

        if(lat1 > lat2){
            latDiff = lat1 - lat2;
        }else{
            latDiff = lat2 - lat1;
        }

        if(long1 > long2){
            longDiff = long1 - long2;
        }else{
            longDiff = long2 - long1;
        }

        temp = pow(sin(latDiff/2),2) + (cos(lat1) * cos(lat2) * pow(sin(longDiff/2),2));
        angRad = 2 * atan2(sqrt(temp), sqrt(1-temp));
        distance = eRadius * angRad;

        distanceArr[i].pkDistance = distance;
        distanceArr[i].pkIndex = i;
    }

    Distance swap;
    for(int i = 0; i < totalParks; i++){
      for(int j = i+1; j < totalParks; j++){
        if(distanceArr[i].pkDistance > distanceArr[j].pkDistance){
            swap = distanceArr[i];
            distanceArr[i] = distanceArr[j];
            distanceArr[j] = swap;
        }
      }
    }

    closestPark = parkArr[distanceArr[0].pkIndex];
    farthestPark = parkArr[distanceArr[totalParks-1].pkIndex];

    /*
    (Check) print park name and distance in decending order to check disntanceArr
        cout << closestPark.pkName << endl;
        cout << farthestPark.pkName << endl;

        for(int i=0; i<totalParks; i++){
            cout << parkArr[distanceArr[i].pkIndex].pkName << endl;
            cout << distanceArr[i].pkDistance << endl;
            cout << endl;
        }
    */
}

//output of the program that displays information about starting point, closest park, and farthest park
void displayOutput(Park &startingPark, Park &closestPark, Park &farthestPark, Distance distanceArr[], int totalParks){
    cout << "Starting Point:" << endl;
    cout << "     Name: " << startingPark.pkName << " (" << startingPark.pkState << ", " << startingPark.pkCountry << ")" << endl;
    cout << "     Latitude: " << startingPark.pkLoc.latitude << endl;
    cout << "     Longitude: " << startingPark.pkLoc.longitude << endl;

    cout << "Closest Park:" << endl;
    cout << "     Name: " << closestPark.pkName << " (" << closestPark.pkState << ", " << closestPark.pkCountry << ")" << endl;
    cout << "     Latitude: " << closestPark.pkLoc.latitude << endl;
    cout << "     Longitude: " << closestPark.pkLoc.longitude << endl;
    cout << "     Distance: " << fixed << setprecision(2) << distanceArr[0].pkDistance << " kms" << endl;

    cout << "Farthest Park:" << endl;
    cout << "     Name: " << farthestPark.pkName << " (" << farthestPark.pkState << ", " << farthestPark.pkCountry << ")" << endl;
    cout << "     Latitude: " << farthestPark.pkLoc.latitude << endl;
    cout << "     Longitude: " << farthestPark.pkLoc.longitude << endl;
    cout << "     Distance: " << fixed << setprecision(2) <<  distanceArr[totalParks-1].pkDistance << " kms" << endl;
}

/*
    OUTPUT:
        Starting Point:
            Name: Irvine Valley College (CA, US)
            Latitude: 33.6754
            Longitude: -117.778
        Closest Park:
            Name: Joshua Tree National Park (CA, USA)
            Latitude: 33.8819
            Longitude: -115.901
            Distance: 174.98 kms
        Farthest Park:
            Name: Addo Elephant Nation Park (Eastern Cape, South Africa)
            Latitude: -33.48
            Longitude: 25.75
            Distance: 16654.18 kms
*/