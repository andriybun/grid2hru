#ifndef HRUDATA
#define HRUDATA

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>

#include "gridMap.h"
#include "hruData.h"

using namespace std;

// default constructor
HRUdataAvg::HRUdataAvg()
 {
  hruMap.LoadFromFile_bin("HRU_GRID.bin");
  NumTimePeriods = 0;
 }

// destructor
HRUdataAvg::~HRUdataAvg()
 {
  delete []grid;
 }

void HRUdataAvg::update(unsigned char timePer, int hruID, float val)
 {
  if (timePer >= (int)data.size()) data.resize(timePer + 1);
  if (hruID >= (int)data[timePer].size()) data[timePer].resize(hruID + 1);
  if (hruID >= 0) {
    data[timePer][hruID].first += val;
    data[timePer][hruID].second++;
  }
 }

double HRUdataAvg::getAvg(unsigned char timePer, int hruID)
 {
  return data[timePer][hruID].first / data[timePer][hruID].second;
 }

double * HRUdataAvg::summary()
// Function returns pointer to vector grid[], containing average values for all
// HRUs and time periods (TP). Sequence is: HRU[0]: all TPs, HRU[1]: all TPs,...,
// HRU[numHRU]: all TPs.
 {
  int numHRU = data[0].size();
  grid = new double[numHRU * NumTimePeriods];
  for (int i = 0; i < numHRU; i++)
    for (int timePer = 0; timePer < NumTimePeriods; timePer++)
      grid[i * NumTimePeriods + timePer] = getAvg(timePer, i);
  return grid;
 }

void HRUdataAvg::readXYdata(double xMin, double yMin, double cellSize, string fileName)
 {
  ifstream f;
  cout << "Opening file: " << fileName << endl;
  f.open(fileName.c_str(),ios::in);
  if (f.is_open()) {
    cout << "Started reading: x y data..."  << endl;
    int numLines = 0;
    while (!f.eof()) {
      string line;
      getline(f,line);
      if(line[0] != '#' && line.size()>0) {
        stringstream ss(line);
        double x,y;
        float val;
        if ((ss >> x) && (ss >> y)) {
          int xInd = int((x - xMin) / (cellSize));
          int yInd = int((y - yMin) / (cellSize));
          int timePer = 0;
          while (ss >> val) {
            update(timePer, hruMap.getByCoords(x, y), val);
            timePer++;
            if (timePer > NumTimePeriods) NumTimePeriods = timePer;
          }
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

void HRUdataAvg::SaveToFile_bin(string fileName)
 {
  ofstream f;
  f.open(fileName.c_str(), ios::out | ios::binary);
  if (f.is_open()) {
    int numHRU = data[0].size();
    f.write(reinterpret_cast<char *>(&numHRU), sizeof(int));
    f.write(reinterpret_cast<char *>(&NumTimePeriods), sizeof(int));
    double * poi = summary();
    f.write(reinterpret_cast<char *>(poi), numHRU * NumTimePeriods * sizeof(double));
    f.close();
    cout << "Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

void HRUdataAvg::SaveToFile(string fileName)
 {
  ofstream f;
  f.open(fileName.c_str(), ios::out);
  if (f.is_open()) {
    cout << "Started writing data to file: " << fileName << endl;
    f << "HRU id\tvalue(s)" << endl;
    int numHRU = data[0].size();
    for (int i = 0; i < numHRU; i++) {
      stringstream ss(stringstream::out);
      ss << i << "\t";
      bool hasValue = false;
      for (int timePer = 0; timePer < data.size(); timePer++) {
        if (data[timePer][i].second != 0) {
          ss << getAvg(timePer, i) << "\t";
          hasValue = true;
        }
      }
      if (hasValue) {
        cout << "" << ss.str() << endl;
        system("pause");
      }
    }

//    for (int i=0; i<result.size(); i++) {
//      if (!isnan(result[i * numHRU + timePer]))
//        f << i << "\t" << result[i * numHRU + timePer] << endl;
//    }
    f.close();
    cout << "Successfully written to text file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

//******************************************************************************
//************* Weighted average ***********************************************
//******************************************************************************

// default constructor
HRUdataAvgW::HRUdataAvgW()
 {

 }

// destructor
HRUdataAvgW::~HRUdataAvgW()
 {
  delete []grid;
 }

void HRUdataAvgW::update(int hruID, float val, float weight)
 {
  if (hruID >= (int)data.size()) data.resize(hruID + 1);
  data[hruID].first += val * weight;
  data[hruID].second += weight;
 }

double HRUdataAvgW::getAvg(int hruID)
 {
  return data[hruID].first / data[hruID].second;
 }

double * HRUdataAvgW::summary()
 {
  int numHRU = data.size();
  grid = new double[numHRU];
  for (int i = 0; i < numHRU; i++) {
    grid[i] = getAvg(i);
    cout << i << "  " << grid[i] << endl;
  }
  return grid;
 }

vector<double> HRUdataAvgW::summaryVector()
 {
  vector<double> res;
  int numHRU = data.size();
  for (int i = 0; i < numHRU; i++) res.push_back(getAvg(i));
  return res;
 }

void HRUdataAvgW::write_bin(string fileName)
 {
  ofstream f;
  f.open(fileName.c_str(), ios::out | ios::binary);
  if (f.is_open()) {
    int numHRU = data.size();
    f.write(reinterpret_cast<char *>(&numHRU), sizeof(int));
    f.write(reinterpret_cast<char *>(summary()), numHRU * sizeof(double));
    f.close();
    cout << "Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

void HRUdataAvgW::write(string fileName)
 {
  ofstream f;
  f.open(fileName.c_str(), ios::out);
  if (f.is_open()) {
    cout << "Started writing data to file: " << fileName << endl;
    vector<double> result =  summaryVector();
    f << "HRU id\tvalue" << endl;
    for (int i=0; i<result.size(); i++) {
      if (!isnan(result[i]))
        f << i << "\t" << result[i] << endl;
    }
    f.close();
    cout << "Successfully written to text file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

#endif
