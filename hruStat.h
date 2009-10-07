class hruStat {
  private:
// Type needed for calculation of average value
    // pair.first - total sum of values
    // pair.second - number of components
    // => avg = pair.first / pair.second
    typedef pair<float, float> pairAvg;
    typedef vector<pairAvg> pairAvgV;
    vector<pairAvgV> data;
    vector<char> flags;
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
    // Class constructor: input parameters:
    // hruMapFileName - name of file with HRU map, stored in binary grid format of
    //     gridMap class;
    // stat - statistics type: SUM - sum; AVG - average; AVGW - weightet awg; 
    //     VAL - value;
    // weightFileName (optional) - name of file with map of weights, stored in 
    //     binary grid format of gridMap class;
    hruStat(string hruMapFileName, string stat, string weightFileName);
    ~hruStat();
    // Updating data stored in memory for HRU with hruID, for time period 
    // timePer. Value val is summed, or multiplied with weight and summed
    // depends on statistics type. 4th param. weight is optional (default = 1)
    void update(unsigned char timePer, int hruID, float val, float weight);
    // Reading data from "X Y data" file, without header. There may be any
    // number of columns with data corresponding to time points 2000, 2010...
    // with a period of 10 years
    void readXYdata(double xMin, double yMin, double cellSize, string fileName);
    // Storing result of conversion in own binary format
    void SaveToFile_bin(string fileName);
    // Storing result of conversion in "[HRU ID] data" text file
    void SaveToFile(string fileName);
};
