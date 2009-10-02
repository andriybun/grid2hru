#ifndef HRUMAP
#define HRUMAP

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

template <class TP>
class HRUmap {
  private:
    int HorResolution, VerResolution;
    TP *grid_HRU, noDataValue;
    double xMin, xMax, yMin, yMax, cellSize;
    void UpdateResolutions();      // recalculates resolutions
  public:
    HRUmap(double, double, double, double, double, TP, string );
    HRUmap(string );
    HRUmap();
    ~HRUmap();
    void set(double x, double y, TP val);              // assigns value val to cell [x][y]
    TP get(int xInd, int yInd);                      // returns number of HRU for cell 
    void SaveToFile(string fileName, string format);   // print HRU map to file
    void SaveToFile_bin(string fileName);              // print HRU map to binary file
};

#endif

// Class constructor
template <class TP>
HRUmap<TP>::HRUmap(double hMin, double hMax, double vMin, double vMax, double Step, 
               TP noData, string fileName)
// hMin, hMax - longitude minimum and maximum values
// vMin, vMax - latitude minimum and maximum
// Step - cell size
// rate - g4m resolution / HRU resolution
// noData - value that will be stored for all cells for which data is missing
// fileName - path to input text file in format "x y value"
 {
  xMin = hMin;
  xMax = hMax;
  yMin = vMin;
  yMax = vMax;
  cellSize = Step;
  noDataValue = noData;
  UpdateResolutions();
  int sizeOfArray = HorResolution * VerResolution;
  grid_HRU = new TP[sizeOfArray];
  memset(grid_HRU,noDataValue,sizeOfArray*sizeof(TP));
  ifstream f;
  cout << "Opening file: " << fileName << endl;
  f.open(fileName.c_str(),ios::in);
  if (f.is_open()) {
    cout << "Started reading data."  << endl;
    int numLines = 0;
    while (!f.eof()) {
      string line;
      getline(f,line);
      if(line[0] != '#' && line.size()>0) {
        stringstream ss(line);
        double x,y;
        TP val;
        if ((ss >> val) && (ss >> x) && (ss >> y)) {
          set(x, y, val);
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

// Class constructor 2 (from binary file)
template <class TP>
HRUmap<TP>::HRUmap(string fileName)
 {
  ifstream f;
  f.open(fileName.c_str(), ios::in | ios::binary);
  if (f.is_open()) {
    f.read(reinterpret_cast<char *>(&xMin), sizeof(double));
    f.read(reinterpret_cast<char *>(&xMax), sizeof(double));
    f.read(reinterpret_cast<char *>(&yMin), sizeof(double));
    f.read(reinterpret_cast<char *>(&yMax), sizeof(double));
    f.read(reinterpret_cast<char *>(&cellSize), sizeof(double));
    f.read(reinterpret_cast<char *>(&noDataValue), sizeof(TP));
    UpdateResolutions();
    int sizeOfArray = HorResolution * VerResolution;
    grid_HRU = new TP[sizeOfArray];
    f.read(reinterpret_cast<char *>(grid_HRU), sizeOfArray * sizeof(TP));
    cout << "Successfully read from binary file: " << fileName << endl;
  } else {
    cout << "Unable to open file!" << endl;
  }
 }


// default constructor
template <class TP>
HRUmap<TP>::HRUmap() 
 {
  xMin = -180;
  xMax = 180;
  yMin = -90;
  yMax = 90;
  cellSize = 0.5/6;
  noDataValue = -1;
  UpdateResolutions();
  int sizeOfArray = HorResolution * VerResolution;
  grid_HRU = new TP[sizeOfArray];
  memset(grid_HRU,noDataValue,sizeOfArray*sizeof(TP));
 }

// destructor
template <class TP>
HRUmap<TP>::~HRUmap()
 {
  delete []grid_HRU;
 }

template <class TP>
void HRUmap<TP>::UpdateResolutions() 
 {
  HorResolution = int((xMax - xMin) / cellSize);
  VerResolution = int((yMax - yMin) / cellSize);
 }

template <class TP>
void HRUmap<TP>::set(double x, double y, TP val)
 {
  int xInd = int((x - xMin) / (cellSize));
  int yInd = int((y - yMin) / (cellSize));
  grid_HRU[yInd * (HorResolution) + xInd] = val;
 }

template <class TP>
TP HRUmap<TP>::get(int xInd, int yInd)
 {
  return grid_HRU[yInd * (HorResolution) + xInd];
 }

template <class TP>
void HRUmap<TP>::SaveToFile(string fileName, string format)
 {
  ofstream f;
  f.open(fileName.c_str(),ios::out);
  if (f.is_open()) {
// Select grid format:
//GRASS ascii Grid
if (format == "GRASS") {
    f << "cols: " << HorResolution << endl;
    f << "rows: " << VerResolution << endl;
    f << "west: " << xMin << endl;
    f << "south: " << yMin << endl;
    f << "north: " << xMin + cellSize * HorResolution << endl;
    f << "east: " << yMin + cellSize * VerResolution << endl;
} else if (format == "ESRI") {
//----------------------------    
// ESRI ascii Grid
    cout << "Writing data to grid-file: " << fileName << endl;
    f << "NCOLS " << HorResolution << endl;
    f << "NROWS " << VerResolution << endl;
    f << "XLLCORNER " << xMin << endl;
    f << "YLLCORNER " << yMin << endl;
    f << setprecision (10) << "CELLSIZE " <<  cellSize << endl;
    f << "NODATA_VALUE " << noDataValue << endl;
}
//-----------------------------        
    for (int j = 0; j < VerResolution; j++) {
      for (int i = 0; i < HorResolution; i++) {
        f << grid_HRU[(VerResolution  - j - 1) * HorResolution + i] << " ";
      }
      f << endl;  
    }
    f.close(); 
    cout << "Successfully written grid to file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

template <class TP>
void HRUmap<TP>::SaveToFile_bin(string fileName)
 {
  ofstream f;
  f.open(fileName.c_str(), ios::out | ios::binary);
  if (f.is_open()) {
    f.write(reinterpret_cast<char *>(&xMin), sizeof(double));
    f.write(reinterpret_cast<char *>(&xMax), sizeof(double));
    f.write(reinterpret_cast<char *>(&yMin), sizeof(double));
    f.write(reinterpret_cast<char *>(&yMax), sizeof(double));
    f.write(reinterpret_cast<char *>(&cellSize), sizeof(double));
    f.write(reinterpret_cast<char *>(&noDataValue), sizeof(TP));
    int sizeOfArray = HorResolution * VerResolution;
    f.write(reinterpret_cast<char *>(grid_HRU), sizeOfArray * sizeof(TP));
    cout << "Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }
