#include <iostream>
#include <fstream>
#include <sstream>

#include "gridMap.h"
#include "hruStat.cpp"
#include "g4m2hru.cpp"

using namespace std;

int main() 
 {
  g4m2hru data;
  data.addDataFromFile("-delete-me.bin", "POPDENS");
  data.saveDataToFile("-del.txt");
  system("pause");
 }
