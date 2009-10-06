class hruStat {
  private:
// Type needed for calculation of average value
    // pair.first - total sum of values
    // pair.second - number of components
    // => avg = pair.first / pair.second
    typedef pair<float, float> pairAvg;
    typedef vector<pairAvg> pairAvgV;
    vector<pairAvgV> data;
// Data containers:
    // Array containing resulting average values
    double *grid;
    // 2D array - map of HRU:
    gridMap<int> hruMap;
    // 2D array - weight:
    gridMap<double> weight;
    int NumTimePeriods;  // numbers of time periods in file
    int statType;        // statistics type
// Functions for data processing:
    double getStat(unsigned char timePer, int hruID); // returns statistics value
    double * summary();                               // returns vector of results
  public:
    hruStat(string hruMapFileName, string stat, string weightFileName);
    ~hruStat();
    void update(unsigned char timePer, int hruID, float val, float weight);
    void readXYdata(double xMin, double yMin, double cellSize, string fileName);
    void SaveToFile_bin(string fileName);
    void SaveToFile(string fileName);
};
