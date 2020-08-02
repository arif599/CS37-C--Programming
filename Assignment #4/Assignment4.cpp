//----------------------------------------------------------------------------//
// Assignment: #                                                             //
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
    string discountType;
    int discountQty;
    int discountPercentage;
    float cost;

  public:
    Parts();
    Parts(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType, int discountQty, int discountPercentage);
    virtual float rebates_fees() const;
    string getDiscription() const;
    float getPrice() const;
    int getQtySold() const;
    string getManufacture() const;
    string getDiscountType() const;
    int getDiscountQty() const;
    float getDiscountPercentage() const;
    float getCost() const;
    string getPartsCategory() const;
};

class Brake : public Parts{
  private:
    Car brakeCarInfo;
    string material;
  public:
    Brake();
    Brake(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType = "", int discountQty = 0, int discountPercentage = 0, string brand = "", string model = "", int year = 0, string material = "");
    virtual float rebates_fees() const;
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
    Light(string discription, float price, string companyName, string category, int qtySold, float cost = 0.0, string discountType = "", int discountQty = 0, int discountPercentage = 0, string brand = "", string model = "", int year = 0, string material = "", float watts = 0.0);
    virtual float rebates_fees() const;
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
    Oil(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType = "", int discountQty = 0, int discountPercentage = 0, string weight = "", string type = "", int quarts = 0);
    virtual float rebates_fees() const;
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
    Tire(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType = "", int discountQty = 0, int discountPercentage = 0, int warranty = 0, string size = "");
    virtual float rebates_fees() const;
    float getWarranty() const;
    string getSize() const;
};

struct profitTracker{
  float netProfit = 0;
  float grossProfit = 0;
  float totalCost = 0;
  int indexLoc = 0;
};

//Prototypes
void findObjects(ifstream &file, int &breakCnt, int &lightCnt, int &oilCnt, int &tireCnt);
void parseLineToTokens(string lineText, string tokens[]);
int calcLines(ifstream &file);
int calcCategories(ifstream &file);
void populateBrakeObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i);
void populateOilObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i);
void populateTireObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i);
void populateLightObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i);
void populateObjects(ifstream &file, Parts **doubleObj, Parts *obj);
float applyBulkDiscount(Parts *obj, float discountQty, float discountPercentage);
float applyTieredDiscount(Parts *obj, string qtyWord, string percentWord);
void sortDisplay(Parts **multiObj, Parts *obj, int size);
double returnExtraFees(const Parts *const obj);
void bubbleSort(profitTracker *pT, int size);
void printProfitByCategory(Parts *obj, const profitTracker pT[], const int &size);
void printProfit(Parts *obj, const profitTracker pT[], const int &size);


//Takes input from file and sorts the items by descending net profit
int main(){
  int brakesCounter, lightsCounter, oilCounter, tireCounter, totalSize;
  brakesCounter = lightsCounter = oilCounter = tireCounter = 0;
  ifstream inputFile ("Parts_ListPoly.txt");
  
  findObjects(inputFile, brakesCounter, lightsCounter, oilCounter, tireCounter);
  totalSize = brakesCounter + lightsCounter + oilCounter + tireCounter;

  Parts *objs = new Parts[totalSize];
  Parts **multiObjs = new Parts*[totalSize];
  populateObjects(inputFile, multiObjs, objs);
  sortDisplay(multiObjs, objs, totalSize);
  
  for(int i=0;i<totalSize;i++){
    delete multiObjs[i];
  }
  delete[] multiObjs;
  delete []objs;
  inputFile.close();
}

//Populates the parts pointer with appropriate objects from the input from file
void populateObjects(ifstream &file, Parts **doubleObj, Parts *obj){
  string inputLine, line;
  int categories = calcCategories(file);
  string brakeContents[categories];
  string lightContents[categories];
  string oilContents[categories];
  string tireContents[categories];

  int cnt = 0;
  if(file.good()){
    while(getline(file, line)){
      for(int i=0; i<line.length(); i++){
        inputLine = line.substr(0, line.find(","));
      }
      if(inputLine == "\"Brakes\""){
        parseLineToTokens(line, brakeContents);
        populateBrakeObj(brakeContents, doubleObj, (obj+cnt), cnt);
        cnt++;
      }
      if(inputLine == "\"Oil\""){
        parseLineToTokens(line, oilContents);
        populateOilObj(oilContents, doubleObj, (obj+cnt), cnt);
        cnt++;
      }
      if(inputLine == "\"Tires\""){
        parseLineToTokens(line, tireContents);
        populateTireObj(tireContents, doubleObj, (obj+cnt), cnt);
        cnt++;
      } 
      if(inputLine == "\"Lights\""){
        parseLineToTokens(line, lightContents);
        populateLightObj(lightContents, doubleObj, (obj+cnt), cnt);
        cnt++;
      }  
    }
  }
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

//Parse a line of text into tokens and stores them in an array of strings
void parseLineToTokens(string lineText, string tokens[]){
  int end, start;

  start = -2;
  for (int j = 0; j < 19; j++)
  {
    start = start + 3;
    end = lineText.find('"', start);
    tokens[j] = lineText.substr(start, end - start);
    start = end;
  }
}

//Applies a bulk discount to object's cost if discount type is Bulk
float applyBulkDiscount(Parts *obj, float discountQty, float discountPercentage){
  if(obj->getQtySold() >= discountQty){
    return obj->getCost() - (discountPercentage/100 *obj->getCost());
  }else{
    return obj->getCost();
  }
}

//Applies a tiered discount to object's cost if discount type is Tiered
float applyTieredDiscount(Parts *obj, string qtyWord, string percentWord){
  float qtyLow, qtyHigh, percentLow, percentHigh;

  qtyLow = stoi(qtyWord.substr(0, qtyWord.find("|")));
  qtyHigh = stoi(qtyWord.substr(qtyWord.find("|")+1, qtyWord.length()));

  percentLow = stoi(percentWord.substr(0, percentWord.find("|")));
  percentHigh = stoi(percentWord.substr(percentWord.find("|")+1, percentWord.length()-1));

  if(obj->getQtySold() >= qtyLow && obj->getQtySold() < qtyHigh){
    float updatedCost = obj->getCost() - (percentLow/100 * obj->getCost());
    return updatedCost;
  }else if(obj->getQtySold() >= qtyHigh){
    float updatedCost = obj->getCost() - (percentHigh/100 * obj->getCost());
    return updatedCost;
  }

  return obj->getCost();
}

//Function that stores the calculations into a struct and calls other functions to sort and display output
void sortDisplay(Parts **multiObj, Parts *obj, int size){
   profitTracker pT[size];
  
  for(int i=0; i<size; i++){
    
    if(obj->getPartsCategory().find("Brakes") != string::npos){
      pT[i].indexLoc = i;
      pT[i].grossProfit = obj->getQtySold() * obj->getPrice() * returnExtraFees(multiObj[i]);
      pT[i].totalCost = (obj->getQtySold() * obj->getCost());
      pT[i].netProfit = pT[i].grossProfit - pT[i].totalCost;

    }

    if(obj->getPartsCategory().find("Lights") != string::npos){
      pT[i].indexLoc = i;
      pT[i].grossProfit = obj->getQtySold() * obj->getPrice();
      pT[i].totalCost = (obj->getQtySold() * obj->getCost()) + returnExtraFees(multiObj[i]);
      pT[i].netProfit = pT[i].grossProfit - pT[i].totalCost; 

    }

    if(obj->getPartsCategory().find("Oil") != string::npos){
      pT[i].indexLoc = i;
      pT[i].grossProfit = (obj->getQtySold() * obj->getPrice()) + returnExtraFees(multiObj[i]);
      pT[i].totalCost = (obj->getQtySold() * obj->getCost());
      pT[i].netProfit = pT[i].grossProfit - pT[i].totalCost;

    }

    if(obj->getPartsCategory().find("Tires") != string::npos){
      pT[i].indexLoc = i;
      pT[i].grossProfit = obj->getQtySold() * obj->getPrice();
      pT[i].totalCost = (obj->getQtySold() * obj->getCost()) + returnExtraFees(multiObj[i]);
      pT[i].netProfit = pT[i].grossProfit - pT[i].totalCost;
 
    }

    obj++;
  }
  for(int i=0; i<size; i++){
    obj--;
  }

  bubbleSort(pT, size);
  printProfitByCategory(obj, pT, size);
  //printProfit(obj, pT, size);

}

//function that sorts an array in descending order
void bubbleSort(profitTracker *pT, int size){
   int x, j = 0;
    profitTracker pTtemp;

    for(x = 1; x < size; x++){
      for (j = 0; j < size-x; j++){
        if(pT[j].netProfit < pT[j + 1].netProfit){
          pTtemp = pT[j];
          pT[j] = pT[j+1];
          pT[j+1] = pTtemp;
        }
      }
    }
}

//prints the net profit in descending order within each category
void printProfitByCategory(Parts *obj, const profitTracker pT[], const int &size){
  ofstream outFile("NetProfit.txt");

  outFile << "**BRAKES**" << endl;
  for(int i=0; i<size; i++){
    if((obj+pT[i].indexLoc)->getPartsCategory() == "Brakes" && pT[i].netProfit != 0){
      outFile << setw(10) <<  left <<  (obj+pT[i].indexLoc)->getPartsCategory()
        << setw(35)  <<  (obj+pT[i].indexLoc)->getDiscription()
        << setw(15) << (obj+pT[i].indexLoc)->getManufacture()
        << setw(8) <<  (obj+pT[i].indexLoc)->getDiscountType()
        << setw(10) << right << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getPrice()
        << setw(10) << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getQtySold()
        << setw(10) << setprecision(2) << fixed << pT[i].grossProfit
        << setw(10) << setprecision(2) << fixed << pT[i].totalCost
        << setw(10) << setprecision(2) << fixed << pT[i].netProfit
        << endl;
    }
  }

    outFile << endl;
    outFile << "**TIRES**" << endl;
    for(int i=0; i<size; i++){
      if((obj+pT[i].indexLoc)->getPartsCategory() == "Tires" && pT[i].netProfit != 0){
        outFile << setw(10) <<  left <<  (obj+pT[i].indexLoc)->getPartsCategory()
        << setw(35)  <<  (obj+pT[i].indexLoc)->getDiscription()
        << setw(15) << (obj+pT[i].indexLoc)->getManufacture()
        << setw(8) <<  (obj+pT[i].indexLoc)->getDiscountType()
        << setw(10) << right << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getPrice()
        << setw(10) << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getQtySold()
        << setw(10) << setprecision(2) << fixed << pT[i].grossProfit
        << setw(10) << setprecision(2) << fixed << pT[i].totalCost
        << setw(10) << setprecision(2) << fixed << pT[i].netProfit
        << endl;
      }
    }
    
    outFile << endl;
    outFile << "**OIL**" << endl;
    for(int i=0; i<size; i++){
      if((obj+pT[i].indexLoc)->getPartsCategory() == "Oil" && pT[i].netProfit != 0){
        outFile << setw(10) <<  left <<  (obj+pT[i].indexLoc)->getPartsCategory()
        << setw(35)  <<  (obj+pT[i].indexLoc)->getDiscription()
        << setw(15) << (obj+pT[i].indexLoc)->getManufacture()
        << setw(8) <<  (obj+pT[i].indexLoc)->getDiscountType()
        << setw(10) << right << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getPrice()
        << setw(10) << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getQtySold()
        << setw(10) << setprecision(2) << fixed << pT[i].grossProfit
        << setw(10) << setprecision(2) << fixed << pT[i].totalCost
        << setw(10) << setprecision(2) << fixed << pT[i].netProfit
        << endl;
      }
    }

    outFile << endl;
    outFile << "**LIGHTS**" << endl;
    for(int i=0; i<size; i++){
      if((obj+pT[i].indexLoc)->getPartsCategory() == "Lights" && pT[i].netProfit != 0){
        outFile << setw(10) <<  left <<  (obj+pT[i].indexLoc)->getPartsCategory()
        << setw(35)  <<  (obj+pT[i].indexLoc)->getDiscription()
        << setw(15) << (obj+pT[i].indexLoc)->getManufacture()
        << setw(8) <<  (obj+pT[i].indexLoc)->getDiscountType()
        << setw(10) << right << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getPrice()
        << setw(10) << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getQtySold()
        << setw(10) << setprecision(2) << fixed << pT[i].grossProfit
        << setw(10) << setprecision(2) << fixed << pT[i].totalCost
        << setw(10) << setprecision(2) << fixed << pT[i].netProfit
        << endl;
      }
    }

  outFile.close();
}

//prints the net profit in descending order
void printProfit(Parts *obj, const profitTracker pT[], const int &size){
  ofstream outFile("NetProfit.txt");

  for(int i=0; i<size; i++){
    if(pT[i].netProfit != 0){
      outFile << setw(10) <<  left <<  (obj+pT[i].indexLoc)->getPartsCategory()
        << setw(35)  <<  (obj+pT[i].indexLoc)->getDiscription()
        << setw(15) << (obj+pT[i].indexLoc)->getManufacture()
        << setw(8) <<  (obj+pT[i].indexLoc)->getDiscountType()
        << setw(10) << right << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getPrice()
        << setw(10) << setprecision(2) << fixed << (obj+pT[i].indexLoc)->getQtySold()
        << setw(10) << setprecision(2) << fixed << pT[i].grossProfit
        << setw(10) << setprecision(2) << fixed << pT[i].totalCost
        << setw(10) << setprecision(2) << fixed << pT[i].netProfit
        << endl;
      }
  }

  outFile.close();
}
 
//returns the extra fees for each object using polymorphism
double returnExtraFees(const Parts *const obj){
  return obj->rebates_fees();
}


//Stores the specific information from the array of strings into Brake object
void populateBrakeObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i){

  try{
    string category = lineTokens[0];
    string discription = lineTokens[1];
    float price = stof(lineTokens[2]);
    string companyName = lineTokens[3];
    int qtySold = stoi(lineTokens[4]); 
    float cost =  stof(lineTokens[18]);
    string brand = lineTokens[5];
    string model = lineTokens[6];
    int year = stoi(lineTokens[7]);
    string material = lineTokens[8];
    string discountType = lineTokens[15];
    int discountQty;
    int discountPercentage;

    *obj = Brake(discription, price, companyName, category, qtySold, cost);
    doubleObj[i] = new Brake(discription, price, companyName, category, qtySold, cost);

    if(discountType.find("Tiered") != string::npos){
      cost = applyTieredDiscount(obj, lineTokens[16], lineTokens[17]);
    }else if(discountType.find("Bulk") != string::npos){
      discountQty = stoi(lineTokens[16]);
      discountPercentage = stoi(lineTokens[17]);
      cost = applyBulkDiscount(obj, discountQty, discountPercentage);
    }

    *obj = Brake(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, brand, model, year, material);
    doubleObj[i] = new Brake(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, brand, model, year, material);

  }catch(invalid_argument &err){
    *obj = Brake();
    doubleObj[i] = new Brake();
  }
}

//Stores the specific information from the array of strings into Oil object
void populateOilObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i){

  try{
    string category = lineTokens[0];
    string discription = lineTokens[1];
    float price = stof(lineTokens[2]);
    string companyName = lineTokens[3];
    int qtySold = stoi(lineTokens[4]); 
    float cost =  stof(lineTokens[18]);
    string discountType = lineTokens[15];
    string weight = lineTokens[10];
    string type = lineTokens[11];
    int quarts = stoi(lineTokens[12]);
    int discountQty;
    int discountPercentage;

    *obj = Oil(discription, price, companyName, category, qtySold, cost);
    doubleObj[i] = new Oil(discription, price, companyName, category, qtySold, cost);

    if(discountType.find("Tiered") != string::npos){
      cost = applyTieredDiscount(obj, lineTokens[16], lineTokens[17]);
    }else if(discountType.find("Bulk") != string::npos){
      discountQty = stoi(lineTokens[16]);
      discountPercentage = stoi(lineTokens[17]);
      cost = applyBulkDiscount(obj, discountQty, discountPercentage);
    }

    *obj = Oil(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, weight, type, quarts);
    doubleObj[i] = new Oil(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, weight, type, quarts);
  }catch(invalid_argument &err){
    *obj = Oil();
    doubleObj[i] = new Oil();
  }
}

//Stores the specific information from the array of strings into Tire object
void populateTireObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i){

  try{
    string category = lineTokens[0];
    string discription = lineTokens[1];
    float price = stof(lineTokens[2]);
    string companyName = lineTokens[3];
    int qtySold = stoi(lineTokens[4]); 
    float cost =  stof(lineTokens[18]);
    string discountType = lineTokens[15];
    string weight = lineTokens[10];
    string type = lineTokens[11];
    string size = lineTokens[13];
    int warranty = stoi(lineTokens[14]);
    int discountQty;
    int discountPercentage;
    *obj = Tire(discription, price, companyName, category, qtySold, cost);
    doubleObj[i] = new Tire(discription, price, companyName, category, qtySold, cost);

    if(discountType.find("Tiered") != string::npos){
      cost = applyTieredDiscount(obj, lineTokens[16], lineTokens[17]);
    }else if(discountType.find("Bulk") != string::npos){
      discountQty = stoi(lineTokens[16]);
      discountPercentage = stoi(lineTokens[17]);
      cost = applyBulkDiscount(obj, discountQty, discountPercentage);
    }

    *obj = Tire(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, warranty, size);
    doubleObj[i] = new Tire(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, warranty, size);
  }catch(invalid_argument &err){
    *obj = Tire();
    doubleObj[i] = new Tire();
  }

}

//Stores the specific information from the array of strings into Light object
void populateLightObj(const string lineTokens[], Parts **doubleObj, Parts *obj, int i){

  try{
    string category = lineTokens[0];
    string discription = lineTokens[1];
    float price = stof(lineTokens[2]);
    string companyName = lineTokens[3];
    int qtySold = stoi(lineTokens[4]); 
    float cost =  stof(lineTokens[18]);
    string brand = lineTokens[5];
    string model = lineTokens[6];
    int year = stoi(lineTokens[7]);
    string material = lineTokens[8];
    string discountType = lineTokens[15];
    float watts = stof(lineTokens[9]);
    int discountQty;
    int discountPercentage;
    *obj = Light(discription, price, companyName, category, qtySold, cost);
    doubleObj[i] = new Light(discription, price, companyName, category, qtySold, cost);;

  if(discountType.find("Tiered") != string::npos){
    cost = applyTieredDiscount(obj, lineTokens[16], lineTokens[17]);
  }else if(discountType.find("Bulk") != string::npos){
    discountQty = stoi(lineTokens[16]);
    discountPercentage = stoi(lineTokens[17]);
    cost = applyBulkDiscount(obj, discountQty, discountPercentage);
  }

  *obj = Light(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, brand, model, year, material, watts);
  doubleObj[i] = new Light(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage, brand, model, year, material, watts);
  }catch(invalid_argument &err){
    *obj = Light();
    doubleObj[i] = new Light();
  }

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
  discountType = "";
  cost = 0;
  discountQty = 0;
  discountPercentage = 0;
}
Parts::Parts(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType, int discountQty, int discountPercentage):company(companyName, category){
    this->discription = discription;
    this->cost = cost;
    this->discription = discription;
    this->price = price;
    this->qtySold = qtySold;
    this->discountQty = discountQty;
    this->discountType = discountType;
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
float Parts::rebates_fees() const{
  return 0;
}
string Parts::getDiscountType() const{
  return discountType;
}
int Parts::getDiscountQty() const{
  return discountQty;
}
float Parts::getDiscountPercentage() const{
  return discountPercentage;
}
float Parts::getCost() const{
  return cost;
}
string Parts::getPartsCategory() const{
  return company.getCategory();
}

//Brake defintions
Brake::Brake():Parts(){
  material = "";
}
Brake::Brake(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType, int discountQty, int discountPercentage, string brand, string model, int year, string material):Parts(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage), brakeCarInfo(brand, model, year){
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
float Brake::rebates_fees() const{
  float revenue = 1;
  if(getMaterial() == "Organic"){
    revenue = 1.05;
  }
  return revenue;
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
Light::Light(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType, int discountQty, int discountPercentage, string brand, string model, int year, string material, float watts):Parts(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage), lightCarInfo(brand, model, year){
  this->watts = watts;
}
float Light::getWatts() const{
  return watts;
}
float Light::rebates_fees() const{
  float extraCost = 0;

  if(lightCarInfo.getYear() < 2002){
    extraCost += 0.25*getQtySold();
  }

  return 0.25;
}

//Oil definitions
Oil::Oil():Parts(){
  weight = "";
  type = "";
  quarts = 0;
} 
Oil::Oil(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType, int discountQty, int discountPercentage, string weight, string type, int quarts):Parts(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage){
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
float Oil::rebates_fees() const{
  float revenue = 0;

  if(getType() == "Synthetic"){
    revenue += 0.10*getQuarts();
  }

  return revenue;
}

//Tire definitions
Tire::Tire():Parts(){
  warranty = 0;
}
Tire::Tire(string discription, float price, string companyName, string category, int qtySold, float cost, string discountType, int discountQty, int discountPercentage, int warranty, string size):Parts(discription, price, companyName, category, qtySold, cost, discountType, discountQty, discountPercentage){
  this->warranty = warranty;
  this->size = size;
}
float Tire::getWarranty() const{
  return warranty;
}
string Tire::getSize() const{
  return size;
}
float Tire::rebates_fees() const{
  float extraCost = 0;
  extraCost += 1*getQtySold();
  return extraCost;
}
