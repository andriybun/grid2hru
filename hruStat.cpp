#ifndef HRUSTAT
#define HRUSTAT

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>

#include "gridMap.h"
#include "hruStat.h"

using namespace std;

// constructor
hruStat::hruStat(string hruMapFileName, string stat, string weightFileName="")
// Statistics type:
// SUM - sum; AVG - average; AVGW - weightet awg; VAL - value;
 {
  hruMap.LoadFromFile_bin(hruMapFileName);
  NumTimePeriods = 0;
  if (stat == "SUM") {
    statType = 0;
  } else if (stat == "AVG") {
    statType = 1;
  } else if (stat == "AVGW") {
    if (weightFileName.length()) 
      weight.LoadFromFile_bin(weightFileName);
    else {
      cout << "Error! Map of weights is missing." << endl;
      system("pause");
      exit(1);
    }
    statType = 2;
  } else if (stat == "VAL") {
    statType = 3;
  }
 }

// destructor
hruStat::~hruStat()
 {
  delete []grid;
 }

void hruStat::update(unsigned char timePer, int hruID, float val, float weight = 1)
 {
  if (timePer >= (int)data.size()) data.resize(timePer + 1);
  if (hruID >= (int)data[timePer].size()) {
    data[timePer].resize(hruID + 1);
    flags.resize(hruID + 1);
  }
  if (hruID >= 0) {
    flags[hruID] = 1;
    switch (statType) {
      case 0: data[timePer][hruID].first += val;
              data[timePer][hruID].second = 0;
              break;
      case 1: data[timePer][hruID].first += val;
              data[timePer][hruID].second++;
              break;
      case 2: data[timePer][hruID].first += val * weight;
              data[timePer][hruID].second += weight;
              break;
      case 3: data[timePer][hruID].first = val;
              data[timePer][hruID].second = 0;
              break;
    }
  }
 }

double hruStat::getStat(unsigned char timePer, int hruID)
 {
  double res;
  switch (statType) {
    case 0: res = data[timePer][hruID].first;
            break;
    case 1: res = data[timePer][hruID].first / data[timePer][hruID].second;
            break;
    case 2: res = data[timePer][hruID].first / data[timePer][hruID].second;
            break;
    case 3: res = data[timePer][hruID].first;
            break;
  }
  return res;
 }

double * hruStat::summary()
// Function returns pointer to vector grid[], containing average values for all
// HRUs and time periods (TP). Sequence is: HRU[0]: all TPs, HRU[1]: all TPs,...,
// HRU[numHRU]: all TPs.
 {
  int numHRU = data[0].size();
  grid = new double[numHRU * NumTimePeriods];
  for (int i = 0; i < numHRU; i++)
    for (int timePer = 0; timePer < NumTimePeriods; timePer++)
      grid[i * NumTimePeriods + timePer] = getStat(timePer, i);
  return grid;
 }

void hruStat::readXYdata(double xMin, double yMin, double cellSize, string fileName)
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
            if (statType == 2) {
              update(timePer, hruMap.getByCoords(x, y), val, weight.getByCoords(x, y));
            } else {
              update(timePer, hruMap.getByCoords(x, y), val);
            }
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

void hruStat::SaveToFile_bin(string fileName)
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

void hruStat::SaveToFile(string fileName)
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
        if (flags[i] == 1) {
          ss << getStat(timePer, i) << "\t";
          hasValue = true;
        }
      }
      if (hasValue) {
        f << ss.str() << endl;
      }
    }
    f.close();
    cout << "Successfully written to text file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

#endif
