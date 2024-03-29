#include <iostream>
#include <fstream>
#include <sstream>

#include "gridMap.h"
#include "hruStat.cpp"

using namespace std;

int main() 
 {
//  calling constructor from XY data text file:
//    gridMap<int> obj = gridMap<int>(-180, 180, -90, 90, 0.5/6, -1, "hru_0833_xy");
//    obj.SaveToFile_bin("HRU_GRID.bin");
//  saving to ASCII grid file (2nd parameter "ESRI" or "GRASS" - depends on output format):
//    obj.SaveToFile("HRU_ASCII.asc", "ESRI");
//  calling constructor from ASCII grid:
//    gridMap<float> landArea = gridMap<float>("landarea_0833.txt","ASCII");
//  saving to binary grid file used by hruData classes:
//    landArea.SaveToFile_bin("landArea_grid.bin");

//******************************************************************************
// calling constructor from binary file:
//  gridMap<float> landArea = gridMap<float>("landarea_grid.bin","bin");
//  gridMap<int> object = gridMap<int>("HRU_GRID.bin","bin");  
  hruStat popdens = hruStat("HRU_GRID.bin", "AVG");
// 3rd parameter is optional:
//   hruStat popdens = hruStat("HRU_GRID.bin", "AVG", "HRU_GRID.bin");
  popdens.readXYdata(-180, -90, 0.5/6, "iiasapopdensb2_hru.txt");
  popdens.SaveToFile_bin("-delete-me.bin");
  popdens.SaveToFile("-delete-me.txt");
  system("pause");
 }
