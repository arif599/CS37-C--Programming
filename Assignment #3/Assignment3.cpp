//----------------------------------------------------------------------------//
// Assignment: #3                                                             //
//----------------------------------------------------------------------------//
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Car{
  private:
    string brand;
    string model;
    int year;
  public:
    Car();
    Car(string brand, string model, int year);
    string getBrand() const;
    string getModel() const;
    int getYear() const;
};

class Manufacturer{
  private:
    string companyName;
    string category;
  public:
     Manufacturer();
     Manufacturer(string companyName, string category);
     string getCompanyName() const;
     string getCategory() const;    
};

class Parts{
  private:
    string discription;
    float price;
    Manufacturer company;
    int qtySold;
  public:
    Parts();
    Parts(string discription, float price, string companyName, string category, int qtySold);
    string getDiscription() const;
    float getPrice() const;
    int getQtySold() const;
    string getManufacture() const;
};

class Brake : public Parts{
  private:
    Car brakeCarInfo;
    string material;
  public:
    Brake();
    Brake(string discription, float price, string companyName, string category, int qtySold, string brand, string model, int year, string material);
    string getMaterial() const;
    string getBrakeCarBrand() const;
    string getBrakeCarModel() const;
    int getBrakeCarYear() const;
};

class Light : public Parts{
  private:
    Car lightCarInfo;
    float watts;
  public:
    Light();
    Light(string discription, float price, string companyName, string category, int qtySold, string brand, string model, int year, string material, float watts);
    string getLightCarBrand() const;
    string getLightCarModel() const;
    int getLightCarYear() const;
    float getWatts() const;
};


class Oil : public Parts{
  private:
    string weight;
    string type;
    int quarts;
  public:
    Oil();
    Oil(string discription, float price, string companyName, string category, int qtySold, string weight, string type, int quarts);
    string getWeight() const;
    string getType() const;
    int getQuarts() const;
};

class Tire : public Parts{
  private:  
    int warranty;
    string size;
  public:
    Tire();
    Tire(string discription, float price, string companyName, string category, int qtySold, int warranty, string size);
    float getWarranty() const;
    string getSize() const;
};

//Prototypes
void findObjects(ifstream &file, int &breakCnt, int &lightCnt, int &oilCnt, int &tireCnt);
void parseLineToTokens(string lineText, string tokens[]);
int calcLines(ifstream &file);
int calcCategories(ifstream &file);
void populateBrakeObj(string lineTokens[], Brake &obj);
void populateOilObj(string lineTokens[], Oil &obj);
void populateTireObj(string lineTokens[], Tire &obj);
void populateLightObj(string lineTokens[], Light &obj);
void populateObjects(ifstream &file, Brake brakeArr[], Light lightArr[], Oil oilArr[], Tire tireArr[]);
void findBestSelling(Brake brakeArr[], Light lightArr[], Oil oilArr[], Tire tireArr[]);

//GLobal variables
int brakesCounter, lightsCounter, oilCounter, tireCounter;
int categories;

//Takes input from file and determines the items that produce the most revenue 
int main(){
  ifstream inputFile ("Parts_List.txt");
  brakesCounter = lightsCounter = oilCounter = tireCounter = 0;
  
  findObjects(inputFile, brakesCounter, lightsCounter, oilCounter, tireCounter);
  Brake *brakeObjs = new Brake[brakesCounter];
  Light *lightObjs= new Light[lightsCounter];
  Oil *oilObjs = new Oil[oilCounter];
  Tire *tireObjs = new Tire[tireCounter];

  populateObjects(inputFile, brakeObjs, lightObjs, oilObjs, tireObjs);

  findBestSelling(brakeObjs, lightObjs, oilObjs, tireObjs);

  delete []brakeObjs;
  delete []lightObjs;
  delete []oilObjs;
  delete []tireObjs;
  inputFile.close();
}

//Takes all object arrays, seperates them by category and calls other functions to populate each specific object
void populateObjects(ifstream &file, Brake brakeArr[], Light lightArr[], Oil oilArr[], Tire tireArr[]){
  string inputLine, line;
  categories = calcCategories(file);
  string brakeContents[categories];
  string lightContents[categories];
  string oilContents[categories];
  string tireContents[categories];

  if(file.good()){
    while(getline(file, line)){
      for(int i=0; i<line.length(); i++){
        inputLine = line.substr(0, line.find(","));
      }
      if(inputLine == "\"Brakes\""){
        static int brakeIndex = 0;
        parseLineToTokens(line, brakeContents);
        populateBrakeObj(brakeContents, brakeArr[brakeIndex]);
        brakeIndex++;
      }
      if(inputLine == "\"Oil\""){
        static int oilIndex = 0;
        parseLineToTokens(line, oilContents);
        populateOilObj(oilContents, oilArr[oilIndex]);
        oilIndex++;
      }
      if(inputLine == "\"Tires\""){
        static int tireIndex = 0;
        parseLineToTokens(line, tireContents);
        populateTireObj(tireContents, tireArr[tireIndex]);
        tireIndex++; 
      } 
      if(inputLine == "\"Lights\""){
        static int lightIndex = 0;
        parseLineToTokens(line, lightContents);
        populateLightObj(lightContents, lightArr[lightIndex]);
        lightIndex++; 
      }  
    }
  }
}

//Finds the best selling item in each category and outputs the results into "MostRevenueFile.txt"
void findBestSelling(Brake brakeArr[], Light lightArr[], Oil oilArr[], Tire tireArr[]){
  float maxBrakePrice = brakeArr[0].getPrice();
  float maxLightPrice = lightArr[0].getPrice();
  float maxOilPrice = oilArr[0].getPrice();
  float maxTirePrice = tireArr[0].getPrice();

  int i;
  int brakeIdx, lightIdx, oilIdx, tireIdx;
  brakeIdx = lightIdx = oilIdx = tireIdx = 0;

  for(i=0; i<brakesCounter; i++){
    if(brakeArr[i].getPrice() > maxBrakePrice){
        maxBrakePrice = brakeArr[i].getPrice();
        brakeIdx = i;
    }
  }

  for(i=0; i<lightsCounter; i++){
    if(lightArr[i].getPrice() > maxLightPrice){
        maxLightPrice = lightArr[i].getPrice();
        lightIdx = i;
    }
  }

  for(i=0; i<oilCounter; i++){
    if(oilArr[i].getPrice() > maxOilPrice){
        maxOilPrice = oilArr[i].getPrice();
        oilIdx = i;
    }
  }

  for(i=0; i<tireCounter; i++){
    if(tireArr[i].getPrice() > maxTirePrice){
        maxTirePrice = tireArr[i].getPrice();
        tireIdx = i;
    }
  }

  ofstream outFile("MostRevenueFile.txt");
  outFile << "** Brakes **" << endl;
  outFile << "\t Discription: " << brakeArr[brakeIdx].getDiscription() << endl;
  outFile << "\t Price: " << brakeArr[brakeIdx].getPrice() << endl;
  outFile << "\t Manufacturer: " << brakeArr[brakeIdx].getManufacture() << endl;
  outFile << "\t QtySold: " << brakeArr[brakeIdx].getQtySold() << endl;
  outFile << "\t Car: " << endl;
  outFile << "\t \t Brand: " << brakeArr[brakeIdx].getBrakeCarBrand() << endl;
  outFile << "\t \t Model: " << brakeArr[brakeIdx].getBrakeCarModel() << endl;
  outFile << "\t \t Year: " << brakeArr[brakeIdx].getBrakeCarYear() << endl;
  outFile << "\t Material: " << brakeArr[brakeIdx].getMaterial() << endl;
  outFile << endl;

  outFile << "** Lights **" << endl;
  outFile << "\t Discription: " << lightArr[lightIdx].getDiscription() << endl;
  outFile << "\t Price: " << lightArr[lightIdx].getPrice() << endl;
  outFile << "\t Manufacturer: " << lightArr[lightIdx].getManufacture() << endl;
  outFile << "\t QtySold: " << lightArr[lightIdx].getQtySold() << endl;
  outFile << "\t Car: " << endl;
  outFile << "\t \t Brand: " << lightArr[lightIdx].getLightCarBrand() << endl;
  outFile << "\t \t Model: " << lightArr[lightIdx].getLightCarModel() << endl;
  outFile << "\t \t Year: " << lightArr[lightIdx].getLightCarYear() << endl;
  outFile << "\t Watts: " << lightArr[lightIdx].getWatts() << endl;
  outFile << endl;
  
  outFile << "** Oil **" << endl;
  outFile << "\t Discription: " << oilArr[oilIdx].getDiscription() << endl;
  outFile << "\t Price: " << oilArr[oilIdx].getPrice() << endl;
  outFile << "\t Manufacturer: " << oilArr[oilIdx].getManufacture() << endl;
  outFile << "\t QtySold: " << oilArr[oilIdx].getQtySold() << endl;
  outFile << "\t Weight: " << oilArr[oilIdx].getWeight() << endl;
  outFile << "\t Type: " << oilArr[oilIdx].getType() << endl;
  outFile << "\t Quarts: " << oilArr[oilIdx].getQuarts() << endl;
  outFile << endl;

  outFile << "** Tires **" << endl;
  outFile << "\t Discription: " << tireArr[tireIdx].getDiscription() << endl;
  outFile << "\t Price: " << tireArr[tireIdx].getPrice() << endl;
  outFile << "\t Manufacturer: " << tireArr[tireIdx].getManufacture() << endl;
  outFile << "\t QtySold: " << tireArr[tireIdx].getQtySold() << endl;
  outFile << "\t Size: " << tireArr[tireIdx].getSize() << endl;
  outFile << "\t Warranty: " << tireArr[tireIdx].getWarranty() << endl;
  outFile.close();
}

//Reads the input file and finds the number of Break, Oil, Tire, and Light objects
void findObjects(ifstream &file, int &breakCnt, int &lightCnt, int &oilCnt, int &tireCnt){
  int objects = calcLines(file);
  string discriptionArr[objects];
  string line;
  getline(file, line);

  int j = 0;
  if(file.good()){
    while(getline(file, line)){
        for(int i=0; i<line.length(); i++){
          line = line.substr(0, line.find(","));
        }
      discriptionArr[j] = line;
      j++;
    }
    for(int i=0; i<objects; i++){
      if(discriptionArr[i] == "\"Brakes\""){
          breakCnt++;
      }
      if(discriptionArr[i] == "\"Oil\""){
          oilCnt++;
      }
      if(discriptionArr[i] == "\"Tires\""){
          tireCnt++;
      } 
      if(discriptionArr[i] == "\"Lights\""){
        lightCnt++;
      }  
    }
  }
  file.clear();
  file.seekg(ios::beg);
}

//Parse a line of text into tokens and store them in an array of strings
void parseLineToTokens(string lineText, string tokens[]){
  int quotePosition;
  string token;

  for(char &replace: lineText){
    if(replace == '"'){
      replace = ' ';
    }
  }

  for(int i=0; i<categories && (quotePosition=lineText.find(",")); i++){
    if(quotePosition == string::npos){
      token = lineText.substr(quotePosition+1);
      tokens[i] = token;
      break;
    }
      token = lineText.substr(1, quotePosition-2);
      tokens[i] = token;
      lineText = lineText.substr(quotePosition+1);
  }  
}

//Stores the specific information from the array of strings into Brake object
void populateBrakeObj(string lineTokens[], Brake &obj){
  string category = lineTokens[0];
  string discription = lineTokens[1];
  float price = stof(lineTokens[2]);
  string companyName = lineTokens[3];
  int qtySold = stoi(lineTokens[4]); 
  string brand = lineTokens[5];
  string model = lineTokens[6];
  int year = stoi(lineTokens[7]);
  string material = lineTokens[8];

  obj = Brake(discription, price, companyName, category, qtySold, brand, model, year, material);
}

//Stores the specific information from the array of strings into Oil object
void populateOilObj(string lineTokens[], Oil &obj){
  string category = lineTokens[0];
  string discription = lineTokens[1];
  float price = stof(lineTokens[2]);
  string companyName = lineTokens[3];
  int qtySold = stoi(lineTokens[4]); 
  string weight = lineTokens[10];
  string type = lineTokens[11];
  int quarts = stoi(lineTokens[12]);

  obj = Oil(discription, price, companyName, category, qtySold, weight, type, quarts);
}

//Stores the specific information from the array of strings into Tire object
void populateTireObj(string lineTokens[], Tire &obj){
  string category = lineTokens[0];
  string discription = lineTokens[1];
  float price = stof(lineTokens[2]);
  string companyName = lineTokens[3];
  int qtySold = stoi(lineTokens[4]); 
  string size = lineTokens[13];
  int warranty = stoi(lineTokens[14]);

  obj = Tire(discription, price, companyName, category, qtySold, warranty, size);
}

//Stores the specific information from the array of strings into Light object
void populateLightObj(string lineTokens[], Light &obj){
  string category = lineTokens[0];
  string discription = lineTokens[1];
  float price = stof(lineTokens[2]);
  string companyName = lineTokens[3];
  int qtySold = stoi(lineTokens[4]); 
  string brand = lineTokens[5];
  string model = lineTokens[6];
  int year = stoi(lineTokens[7]);
  string material = lineTokens[8];
  float watts = stof(lineTokens[9]);

  obj = Light(discription, price, companyName, category, qtySold, brand, model, year, material, watts);
}

//Calculates the total numbers of line
int calcLines(ifstream &file){
  int lineCnt;
  string line;

  if(file.is_open()){
    while(getline(file, line)){
      lineCnt++;
    }
  }

  file.clear();
  file.seekg(ios::beg);
  return lineCnt;
}

//Calculates the total number of columns (categories)
int calcCategories(ifstream &file){
  string line;
  getline(file, line);
  int rowCounter = 0;
  
  for(char &temp: line){
    if(temp == ','){
      rowCounter++;
    }
  }

  return rowCounter+1;
}

//Car Definitions
Car::Car(){
    brand = "";
    model = "";
    year = 0;
}
Car::Car(string brand, string model, int year){
  this->brand = brand;
  this->model = model;
  this->year = year;
}
string Car::getBrand() const{
  return brand;
}
string Car::getModel() const{
  return model;
}
int Car::getYear() const{
  return year;
}

//Manufacturer definitions
Manufacturer::Manufacturer(){
  companyName = "";
  category = "";
}
Manufacturer::Manufacturer(string companyName, string category){
  this->companyName = companyName;
  this->category = category;
}
string Manufacturer::getCompanyName() const{
  return companyName;
}
string Manufacturer::getCategory() const{
  return category;
}  

//Parts defintions
Parts::Parts(){
  discription = "";
  price = 0;
  company = Manufacturer();
  qtySold = 0;
}
Parts::Parts(string discription, float price, string companyName, string category, int qtySold):company(companyName, category){
    this->discription = discription;
    this->price = price;
    this->qtySold = qtySold;
}
string Parts::getDiscription() const{
  return discription;
}
float Parts::getPrice() const{
  return price;
}
int Parts::getQtySold() const{
  return qtySold;
}
string Parts::getManufacture() const{
  return company.getCompanyName();
}

//Brake defintions
Brake::Brake():Parts(){
  material = "";
}
Brake::Brake(string discription, float price, string companyName, string category, int qtySold, string brand, string model, int year, string material):Parts(discription, price, companyName, category, qtySold), brakeCarInfo(brand, model, year){
  this->material = material;
}
string Brake::getMaterial() const{
  return material;
}
string Brake::getBrakeCarBrand() const{
  return brakeCarInfo.getBrand();
}
string Brake::getBrakeCarModel() const{
  return brakeCarInfo.getModel();
}
int Brake::getBrakeCarYear() const{
  return brakeCarInfo.getYear();
}

//Light defintions
string Light::getLightCarBrand() const{
  return lightCarInfo.getBrand();
}
string Light::getLightCarModel() const{
  return lightCarInfo.getModel();
}
int Light::getLightCarYear() const{
  return lightCarInfo.getYear();
}
Light::Light():Parts(){
  watts = 0;
}
Light::Light(string discription, float price, string companyName, string category, int qtySold, string brand, string model, int year, string material, float watts):Parts(discription, price, companyName, category, qtySold), lightCarInfo(brand, model, year){
  this->watts = watts;
}
float Light::getWatts() const{
  return watts;
}

//Oil definitions
Oil::Oil():Parts(){
  weight = "";
  type = "";
  quarts = 0;
} 
Oil::Oil(string discription, float price, string companyName, string category, int qtySold, string weight, string type, int quarts):Parts(discription, price, companyName, category, qtySold){
  this->weight = weight;
  this->type = type;
  this->quarts = quarts;
}
string Oil::getWeight() const{
  return weight;
}
string Oil::getType() const{
  return type;
}
int Oil::getQuarts() const{
  return quarts;
}

//Tire definitions
Tire::Tire():Parts(){
  warranty = 0;
}
Tire::Tire(string discription, float price, string companyName, string category, int qtySold, int warranty, string size):Parts(discription, price, companyName, category, qtySold){
  this->warranty = warranty;
  this->size = size;
}
float Tire::getWarranty() const{
  return warranty;
}
string Tire::getSize() const{
  return size;
}

/* 
In MostRevenueFile.txt (Output):

** Brakes **
	 Discription: ProACT Premium Disc Brake Pads
	 Price: 149.85
	 Manufacturer: Akebono
	 QtySold: 99
	 Car: 
	 	 Brand: Lexus
	 	 Model: LS460
	 	 Year: 2012
	 Material: Ceramic

** Lights **
	 Discription: SilverStar ULTRA Headlight Bulb
	 Price: 44.99
	 Manufacturer: Sylvania
	 QtySold: 128
	 Car: 
	 	 Brand: Toyota
	 	 Model: Corolla
	 	 Year: 2015
	 Watts: 60

** Oil **
	 Discription: Mobil 1 Motor Oil
	 Price: 36.99
	 Manufacturer: Mobil
	 QtySold: 39
	 Weight: 0W-20
	 Type: Synthetic
	 Quarts: 5

** Tires **
	 Discription: LTX
	 Price: 224.99
	 Manufacturer: Michelin
	 QtySold: 11
	 Size: P245/70R16
	 Warranty: 60000

*/