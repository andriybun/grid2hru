//  Name: 
//  Author: Andriy Bun
//  Date: 08.10.09
//  Description: Conversion of data table to HRU resolution

#ifndef G4M2HRU
#define G4M2HRU

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>

#include "gridMap.h"
#include "hruStat.cpp"
#include "g4m2hru.h"

// Default constructor
g4m2hru::g4m2hru()
 {
  maxNumHRU = 0;
//    gridMap<int> obj = gridMap<int>(-180, 180, -90, 90, 0.5/6, -1, "hru_0833_xy");
//    obj.SaveToFile_bin("HRU_GRID.bin");
  
 }

// Destructor
g4m2hru::~g4m2hru()
 {
  for (int i = 0; i < data.size(); i++) {
    delete []data[i];
  }
 }

void g4m2hru::addDataFromFile(string fileName, string header)
 {
  ifstream f;
  f.open(fileName.c_str(), ios::in | ios::binary);
  if (f.is_open()) {
    cout << "Reading data from binary file: " << fileName << endl;
    int HRUs, TimePeriods;
    f.read(reinterpret_cast<char *>(&HRUs), sizeof(int));
    numHRU.push_back(HRUs);
    if (HRUs > maxNumHRU) maxNumHRU = HRUs;
    f.read(reinterpret_cast<char *>(&TimePeriods), sizeof(int));
    numTimePeriods.push_back(TimePeriods);
    double * poi = new double[HRUs * TimePeriods];
    f.read(reinterpret_cast<char *>(poi), HRUs * TimePeriods * sizeof(double));
    data.push_back(poi);
    headers.push_back(header);
    f.close();
    cout << "Successfully read from binary file: " << fileName << endl;
  } else {
    cout << "Unable to open to file!" << endl;
  }
 }

void g4m2hru::saveDataToFile(string fileName)
 {
  ofstream f;
  f.open(fileName.c_str(), ios::out);
  if (f.is_open()) {
    cout << "Saving data to binary file: " << fileName << endl;
    // write header
    for (int rec = 0; rec < data.size(); rec++)
      for (int timePer = 0; timePer < numTimePeriods[rec]; timePer++)
        cout << headers[rec] << 2000 + 10 * timePer << "\t";
    cout << endl;
    system("pause");
    // write data
    for (int currHRU = 0; currHRU < maxNumHRU; currHRU++) {
      stringstream ss(stringstream::out);
      ss << currHRU << "\t";
      bool hasValue = false;
      for (int rec = 0; rec < data.size(); rec++) {
        for (int timePer = 0; timePer < numTimePeriods[rec]; timePer++) {
          double val = data[rec][currHRU * numTimePeriods[rec] + timePer];
          ss << val << "\t";
          if (!isnan(val)) hasValue = true;
        }
      }
      if (hasValue) {
        cout  << ss.str() << endl;
        system("pause");
      }
    }
    f.close();
    cout << "Successfully read from binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

#endif
