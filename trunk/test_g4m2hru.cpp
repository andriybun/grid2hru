#include <iostream>
#include <fstream>
#include <sstream>

#include "hruMap.h"
#include "hruData.cpp"

using namespace std;

void readFile_xydata(double xMin, double yMin, double cellSize, HRUdataAvg &d, 
                     HRUmap<int> &map_HRU, string fileName);

int main() 
 {
//  HRUmap<int> obj = HRUmap<int>(-180, 180, -90, 90, 0.5/6, -1, "hru_0833_xy");  // calling constructor from text file
//  obj.SaveToFile("HRU_ASCII.asc", "ESRI");
//  obj.SaveToFile_bin("HRU_GRID.bin");
//*******************************************************
//  must create the same map HRUmap<float> for landarea
//*******************************************************
  HRUmap<int> object = HRUmap<int>("HRU_GRID.bin");  // calling constructor from binary file
  HRUdataAvg popdens = HRUdataAvg();
  readFile_xydata(-180, -90, 0.5/6, popdens, object, "iiasapopdensb2_hru.txt");
//  popdens.write_bin("-delete-me.bin");
  popdens.write("iiasapopdens_hru.txt");
  system("pause");
 }

void readFile_xydata(double xMin, double yMin, double cellSize, HRUdataAvg &d, 
                     HRUmap<int> &map_HRU, string fileName)
 {
  ifstream f;
  cout << "Opening file: " << fileName << endl;
  f.open(fileName.c_str(),ios::in);
  if (f.is_open()) {
    cout << "Started reading: x y data"  << endl;
    int numLines = 0;
    while (!f.eof()) {
      string line;
      getline(f,line);
      if(line[0] != '#' && line.size()>0) {
        stringstream ss(line);
        double x,y;
        float val;
        if ((ss >> x) && (ss >> y) && (ss >> val)) {
          int xInd = int((x - xMin) / (cellSize));
          int yInd = int((y - yMin) / (cellSize));
          d.update(map_HRU.get(xInd, yInd), val);
          numLines++;
        }
      }
      if ((numLines % 250000) == 0) cout << "Line #" << numLines << endl;
     }
    f.close(); 
    cout << "Successfully read " << numLines << " lines." << endl;
  } else {
    cout << "Unable to open input file!" << endl;
  }
 }
