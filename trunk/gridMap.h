#ifndef GRIDMAP
#define GRIDMAP

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <algorithm>

using namespace std;

template <class TP>
class gridMap {
  private:
    int HorResolution, VerResolution;
    TP *grid, noDataValue;
    double xMin, xMax, yMin, yMax, cellSize;
    void UpdateResolutions();      // recalculates resolutions
  public:
    gridMap(double, double, double, double, double, TP, string );
    gridMap(string fileName, string fileType);
    gridMap();
    gridMap(const gridMap<TP> &g);
    ~gridMap();
    void set(double x, double y, TP val);              // assigns value val to cell [x][y]
    TP get(int xInd, int yInd);                        // returns value for a specified cell
    TP getByCoords(double x, double y);                // returns value for the specified coordinates
    bool LoadFromFile(double , double , double , double , double , TP , string );
    bool LoadFromFile_bin(string fileName);
    void SaveToFile(string fileName, string format);   // print map to file
    void SaveToFile_bin(string fileName);              // print map to binary file
};

// Class constructor (from XY data)
template <class TP>
gridMap<TP>::gridMap(double hMin, double hMax, double vMin, double vMax, double Step, 
               TP noData, string fileName)
// hMin, hMax - longitude minimum and maximum values
// vMin, vMax - latitude minimum and maximum
// Step - cell size
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
  grid = new TP[sizeOfArray];
  for (int i = 0; i < sizeOfArray; i++) grid[i] = noDataValue;
//  memset(grid,noDataValue,sizeOfArray*sizeof(TP));
  ifstream f;
  cout << "> Opening file: " << fileName << endl;
  f.open(fileName.c_str(),ios::in);
  if (f.is_open()) {
    cout << " Started reading data."  << endl;
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
    cout << "  Successfully read " << numLines << " lines." << endl;
  } else {
    cout << "Unable to open input file!" << endl;
  }
 }

// Class constructor 2 (from grids)
// fileType: bin - binary grid file;
//           ASCII - ASCII grid file;
template <class TP>
gridMap<TP>::gridMap(string fileName, string fileType = "bin")
 {
  ifstream f;
// from binary file
  if (fileType == "bin") {
    LoadFromFile_bin(fileName);
  } else {
// from ASCII grid file  
    f.open(fileName.c_str(), ios::in);
    if (f.is_open()) {
      cout << "> Reading data from ASCII grid file: " << fileName << endl;
      string line;
      // reading grid parameters
      bool param = true;
      while (param) {
        getline(f,line);
        string header;
        transform(line.begin(), line.end(), line.begin(), (int(*)(int)) tolower);
        header = line.substr(0, int(line.find_first_of(" \t")));
        line = line.substr(int(line.find_first_of(" \t")));
        if(line.find(",") != string::npos)
          line.replace(line.find(","),1,".");
        stringstream ss(line);
        param = false;
        if (header == "ncols") {
          int val;
          ss >> HorResolution;
          param = true;
        } else if (header == "nrows") {
          int val;
          ss >> VerResolution;
          param = true;
        } else if (header == "xllcorner") {
          ss >> xMin;
          param = true;
        } else if (header == "yllcorner") {
          ss >> yMin;
          param = true;
        } else if (header == "cellsize") {
          ss >> cellSize;
          param = true;
        } else if (header == "nodata_value") {
          ss >> noDataValue;
          param = true;
        }
      }
      // reading grid data
      int cNum = 0;
      int rNum = 0;
      int l = 0;
      int sizeOfArray = HorResolution * VerResolution;
      grid = new TP[sizeOfArray];
      while (!f.eof()) {
        while (line.find(",") != string::npos)
          line.replace(line.find(","),1,".");
        stringstream ss(line);
        TP val;
        while (ss >> val) {
          int ind = (HorResolution)*(VerResolution-rNum-1) + cNum;
          if ((ind >= sizeOfArray) || (ind < 0)) {cout << "#\n"; break;}
          grid[ind] = val;
          cNum++;
          if (cNum >= HorResolution) {
            cNum = 0;
            rNum++;
          }
        }
        getline(f,line);
        l++;
      }
      f.close();
      cout << "  Successfully read from ASCII grid file." << endl;
    } else {
      cout << "Unable to open file!" << endl;
    }
  }
 }


// default constructor
template <class TP>
gridMap<TP>::gridMap() 
 {
  xMin = -180;
  xMax = 180;
  yMin = -90;
  yMax = 90;
  cellSize = 0.5/6;
  noDataValue = -1;
  UpdateResolutions();
  int sizeOfArray = HorResolution * VerResolution;
  grid = new TP[sizeOfArray];
  for (int i = 0; i < sizeOfArray; i++) grid[i] = noDataValue;
//  memset(grid,noDataValue,sizeOfArray*sizeof(TP));
 }

// copy constructor
template <class TP>
gridMap<TP>::gridMap(const gridMap<TP> &g) 
 {
  HorResolution = g.HorResolution;
  VerResolution = g.VerResolution;
  noDataValue = g.noDataValue;
  xMin = g.xMin;
  xMax = g.xMax;
  yMin = g.yMin;
  yMax = g.yMax;
  cellSize = g.cellSize;
  int sizeOfArray = HorResolution * VerResolution;
  grid = new TP[sizeOfArray];
  memcpy(grid,g.grid,sizeOfArray*sizeof(TP));
 }

// destructor
template <class TP>
gridMap<TP>::~gridMap()
 {
  delete []grid;
 }

template <class TP>
void gridMap<TP>::UpdateResolutions() 
 {
  HorResolution = int((xMax - xMin) / cellSize);
  VerResolution = int((yMax - yMin) / cellSize);
 }

template <class TP>
void gridMap<TP>::set(double x, double y, TP val)
 {
  int xInd = int((x - xMin) / (cellSize));
  int yInd = int((y - yMin) / (cellSize));
  grid[yInd * (HorResolution) + xInd] = val;
 }

template <class TP>
TP gridMap<TP>::get(int xInd, int yInd)
 {
  return grid[yInd * (HorResolution) + xInd];
 }

template <class TP>
TP gridMap<TP>::getByCoords(double x, double y)
 {
  int xInd = int((x - xMin) / (cellSize));
  int yInd = int((y - yMin) / (cellSize));
  if ((xInd < 0) || (xInd>=HorResolution) || (yInd < 0) || (yInd>=VerResolution))
    return noDataValue;
  return grid[yInd * (HorResolution) + xInd];
 }

template <class TP>
bool gridMap<TP>::LoadFromFile(double hMin, double hMax, double vMin, double vMax, double Step, 
               TP noData, string fileName)
// hMin, hMax - longitude minimum and maximum values
// vMin, vMax - latitude minimum and maximum
// Step - cell size
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
  delete []grid;
  grid = new TP[sizeOfArray];
  for (int i = 0; i < sizeOfArray; i++) grid[i] = noDataValue;
//  memset(grid,noDataValue,sizeOfArray*sizeof(TP));
  ifstream f;
  cout << "> Opening file: " << fileName << endl;
  f.open(fileName.c_str(),ios::in);
  if (f.is_open()) {
    cout << " Started reading data."  << endl;
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
    cout << "  Successfully read " << numLines << " lines." << endl;
    return true;
  } else {
    cout << "Unable to open input file!" << endl;
    return false;
  }
 }

template <class TP>
bool gridMap<TP>::LoadFromFile_bin(string fileName)
 {
  ifstream f;
  f.open(fileName.c_str(), ios::in | ios::binary);
  if (f.is_open()) {
    cout << "> Reading data from binary file" << endl;
    f.read(reinterpret_cast<char *>(&xMin), sizeof(double));
    f.read(reinterpret_cast<char *>(&xMax), sizeof(double));
    f.read(reinterpret_cast<char *>(&yMin), sizeof(double));
    f.read(reinterpret_cast<char *>(&yMax), sizeof(double));
    f.read(reinterpret_cast<char *>(&cellSize), sizeof(double));
    f.read(reinterpret_cast<char *>(&noDataValue), sizeof(TP));
    UpdateResolutions();
    int sizeOfArray = HorResolution * VerResolution;
    grid = new TP[sizeOfArray];
    f.read(reinterpret_cast<char *>(grid), sizeOfArray * sizeof(TP));
    cout << "  Successfully read from binary file: " << fileName << endl;
    return true;
  } else {
    cout << "Unable to open file!" << endl;
    return false;
  }
 }

template <class TP>
void gridMap<TP>::SaveToFile(string fileName, string format)
 {
  ofstream f;
  f.open(fileName.c_str(),ios::out);
  if (f.is_open()) {
// Select grid format:
//GRASS ascii Grid
    cout << "> Writing data to grid-file: " << fileName << endl;
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
        f << grid[(VerResolution  - j - 1) * HorResolution + i] << " ";
      }
      f << endl;  
    }
    f.close(); 
    cout << "  Successfully written grid to file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

template <class TP>
void gridMap<TP>::SaveToFile_bin(string fileName)
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
    f.write(reinterpret_cast<char *>(grid), sizeOfArray * sizeof(TP));
    cout << "> Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

#endif
