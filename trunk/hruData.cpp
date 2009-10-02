#ifndef HRUDATA
#define HRUDATA

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>

#include "hruData.h"

using namespace std;

// default constructor
HRUdataAvg::HRUdataAvg()
 {

 }

// destructor
HRUdataAvg::~HRUdataAvg()
 {
  delete []grid;
 }

void HRUdataAvg::set(int hruID, float val)
 {
  if (hruID >= (int)data.size()) data.resize(hruID + 1);
  if (hruID >= 0) {
    data[hruID].first += val;
    data[hruID].second++;
  }
 }

double HRUdataAvg::getAvg(int hruID)
 {
  return data[hruID].first / data[hruID].second;
 }

double * HRUdataAvg::summary()
 {
  int numHRU = data.size();
  grid = new double[numHRU];
  for (int i = 0; i < numHRU; i++) {
    grid[i] = getAvg(i);
    cout << i << "  " << grid[i] << endl;
  }
  return grid;
 }

vector<double> HRUdataAvg::summaryVector()
 {
  vector<double> res;
  int numHRU = data.size();
  for (int i = 0; i < numHRU; i++) res.push_back(getAvg(i));
  return res;
 }

void HRUdataAvg::write_bin(string fileName)
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

void HRUdataAvg::write(string fileName)
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

void HRUdataAvgW::set(int hruID, float val, float weight)
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
