class HRUdataAvg {
  private:
// Type needed for calculation of average value
    // pair.first - total sum of values
    // pair.second - number of components
    // => avg = pair.first / pair.second
    typedef pair<float, unsigned char> pairAvg;
    typedef vector<pairAvg> pairAvgV;
    vector<pairAvgV> data;
// Data containers:
    // Array containing resulting average values
    double *grid;
    // 2D array - map of HRU:
    gridMap<int> hruMap;
    int NumTimePeriods;
// Functions for data processing:
    double getAvg(unsigned char timePer, int hruID);
    double * summary();
  public:
    HRUdataAvg();
    ~HRUdataAvg();
    void update(unsigned char timePer, int hruID, float val);
    void readXYdata(double xMin, double yMin, double cellSize, string fileName);
    void SaveToFile_bin(string fileName);
    void SaveToFile(string fileName);
};

class HRUdataAvgW {
  private:
// Type needed for calculation of weighted average values
// pair.first - total sum of values
// pair.second - sum of weighs
// => avg = pair.first / pair.second
    typedef pair<float, float> pairAvg;
    vector<pairAvg> data;
// Data containers:
    double *grid;
// Functions for data processing
    double getAvg(int hruID);
    double * summary();
  public:
    HRUdataAvgW();
    ~HRUdataAvgW();
    void update(int hruID, float val, float weight);
    vector<double> summaryVector();
    void write_bin(string fileName);
    void write(string fileName);
};
