class HRUdataAvg {
  private:
// Type needed for calculation of average value
// pair.first - total sum of values
// pair.second - number of components
// => avg = pair.first / pair.second
    typedef pair<float, unsigned char> pairAvg;
    vector<pairAvg> data;
    double *grid;
// Functions for data processing
    double getAvg(int hruID);
    double * summary();
  public:
    HRUdataAvg();
    ~HRUdataAvg();
    void update(int hruID, float val);
    vector<double> summaryVector();
    void write_bin(string fileName);
    void write(string fileName);
};

class HRUdataAvgW {
  private:
// Type needed for calculation of weighted average values
// pair.first - total sum of values
// pair.second - sum of weighs
// => avg = pair.first / pair.second
    typedef pair<float, float> pairAvg;
    vector<pairAvg> data;
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
