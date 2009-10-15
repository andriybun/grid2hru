//  Name: 
//  Author: Andriy Bun
//  Date: 08.10.09
//  Description: Conversion of data table for HRU from binary to text format

class g4m2hru {
  private:
    vector <double *> data;
    vector <int> numHRU, numTimePeriods;
    vector <string> headers;
    int maxNumHRU;
    void info();
  public:
    g4m2hru();
    ~g4m2hru();
    void addDataFromFile(string fileName, string header);
    void saveDataToFile(string fileName);
};
