//--------------------------------------------------------//
// Assignment: #2                                         //
//--------------------------------------------------------//

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

float boxTotalVolume = 0;
float containerVolume = 0;

class Box{
	public:
		Box();
		float getHeight() const;
		float getLength() const;
		float getWidth() const;	
		float getMaxX() const;
		float getMaxY() const;
		float getMaxZ() const;
		float getMinX() const;
		float getMinY() const;
		float getMinZ() const;
		float getVolume() const;	
    string getDiscription() const;
    void setDiscription(string pDiscription);
    friend ostream &operator << (ostream &out, const Box &obj); 
    friend istream &operator >> (istream &in,  Box &obj);

	private:
    float x1, x2, x3, x4, y1, y2, y3, y4, z;
    float xArray[4];
    float yArray[4];
    string discription;
};
//Default constructor for Box class
Box::Box(){
  x1 = x2 = x3 = x4 = y1 = y2 = y3 = y4 = z = 0;
  discription = "";
}
//Return the height by getting the difference between two y values
float Box::getHeight() const{
  return getMaxY() - getMinY();
}
//Return the length by getting the difference between two x values
float Box::getLength() const{
  return getMaxX() - getMinX();
}
//Return the width by getting the difference between two z values
float Box::getWidth() const{
  return getMaxZ() - getMinZ();
}
//Finds and return the maximun x value
float Box::getMaxX() const{
 float max = x1;
  for(int i=0; i<sizeof(xArray)/sizeof(xArray[0]); i++){
    if(max < xArray[i]){
      max = xArray[i];
    }
  }
  return max;
}
//Finds and return the maximun y value
float Box::getMaxY() const{
  float max = y1;
  for(int i=0; i<sizeof(yArray)/sizeof(yArray[0]); i++){
    if(max < yArray[i]){
      max = yArray[i];
    }
  }
  return max;
}
//Returns the z value
float Box::getMaxZ() const{
  return z;
}
//Finds and return the minimum x value
float Box::getMinX() const{
 float min = x1;
  for(int i=0; i<sizeof(xArray)/sizeof(xArray[0]); i++){
    if(min > xArray[i]){
      min = xArray[i];
    }
  }
  return min;
}
//Finds and return the minimum y value
float Box::getMinY() const{
 float min = y1;
  for(int i=0; i<sizeof(yArray)/sizeof(yArray[0]); i++){
    if(min > yArray[i]){
      min = yArray[i];
    }
  }
  return min;
}
//Returns the minimum z value
float Box::getMinZ() const{
  return 0;
}
//Returns the discription of the items inside the box
string Box::getDiscription() const{
  return discription;
}
//Calculates and returns the volume of the box
float Box::getVolume() const{
  return getHeight() * getLength() * getWidth();
}
//Takes a parameter value and updates the box discription 
void Box::setDiscription(string pDiscription){
  discription = pDiscription;
}
//Overloading >> operator that takes input from file
istream &operator >> (istream &in,  Box &obj){
  getline(in, obj.discription); 

  in >> obj.x1 >> obj.y1;
  in >> obj.x2 >> obj.y2;
  in >> obj.x3 >> obj.y3;
  in >> obj.x4 >> obj.y4;
  in >> obj.z;
  in.ignore();

  obj.xArray[0] = obj.x1;
  obj.xArray[1] = obj.x2;
  obj.xArray[2] = obj.x3;
  obj.xArray[3] = obj.x4;

  obj.yArray[0] = obj.y1;
  obj.yArray[1] = obj.y2;
  obj.yArray[2] = obj.y3;
  obj.yArray[3] = obj.y4;
  return in;
}
//Overloading << operator that outputs the specifications for a container
ostream &operator << (ostream &out, const Box (&obj)[10]){
  float length = obj[0].getMaxX();
  float width = obj[0].getMaxZ();
  float height = obj[0].getMaxY();

  out << "Minimum container specifications:" << endl;
  for(int i=0; i<10; i++){
    if(length < obj[i].getMaxX()){
      length = obj[i].getMaxX();
    }
    if(width < obj[i].getMaxZ()){
      width = obj[i].getMaxZ();
    }
    if(height < obj[i].getMaxY()){
      height = obj[i].getMaxY();
    }
  }

  float volume = length*width*height;
  out << fixed << setprecision(1)
      << "Length: " << length << "  "
      << "Width: " << width << "  "
      << "Height: " << height << "  " 
      << "Volume: " << volume << " cu/units" << endl;
  out << endl;

  containerVolume = volume;
  return out;
}

//Prototypes
void loadInput(Box (&load)[10]);
void printBoxLog(const Box (&log)[10]);
void printTotalVolume(const Box (&boxVolume)[10]);

//Checks if a certian layout for loading boxes into the smallest possible shipping container is efficent or not efficent
int main()
{
  Box boxArr[10];
  loadInput(boxArr);
  printBoxLog(boxArr);
  printTotalVolume(boxArr);
  cout << boxArr;
  float layoutEfficient = (boxTotalVolume/containerVolume)*100;

  if(layoutEfficient >= 90){
      cout << fixed << setprecision(1) << "The layout is efficient, it uses " << layoutEfficient << "% of the space." << endl;
  }else{
    cout << fixed << setprecision(1) << "The layout is not efficient, it wastes " << 100-layoutEfficient << "% of the space." << endl;
  }
}

//Takes box data from a file and loads the data into each object
void loadInput(Box (&load)[10]){
  string line;
  float numLine;
  ifstream inFile;
  inFile.open ("Assignment_2_Data_Box.txt");

  for(int i=0; i<10 && inFile.good(); i++){      
    inFile >> load[i];    
  }
}

//Outputs the discription and the volume of each box in a neat format
void printBoxLog(const Box (&log)[10]){
    cout << left << setw(25) << "Box#";
    cout << setw(25) << "Discription" ;
    cout << setw(25) << "Volume(cu/units)";
    cout << endl;

    for(int i=0; i<10; i++){
      cout << left << setw(25) << i + 1;
      cout << left << setw(25) << log[i].getDiscription();
      cout << right << fixed << setprecision(1) << setw(15) << log[i].getVolume();
      cout << endl;
    }

    cout << endl;
}

//Calculates and outputs the total volume of all the boxes combined
void printTotalVolume(const Box (&boxVolume)[10]){
   float totalVolume = 0;

    for(int i=0; i<10; i++){
      totalVolume += boxVolume[i].getVolume(); 
    }

    cout << fixed << setprecision(1) << "The total volume of the boxes is: " 
         << totalVolume << " cu/units" << endl;

    boxTotalVolume = totalVolume;
    cout << endl;
 
}



/*

Box#                     Discription              Volume(cu/units)         
1                        Standard tools                     350.0
2                        Metric tools                       125.0
3                        Plastic parts                      125.0
4                        Steel parts                         11.4
5                        Aluminum parts                      45.0
6                        Rubber gaskets                      64.0
7                        Large bags                          64.0
8                        Small bags                          72.0
9                        Sealant                             20.0
10                       Liquid nitrogen                      5.0

The total volume of the boxes is: 881.4 cu/units

Minimum container specifications:
Length: 15.0  Width: 5.0  Height: 14.0  Volume: 1050.0 cu/units

The layout is not efficient, it wastes 16.1% of the space.

*/