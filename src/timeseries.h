

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

class TimeSeries{
    std::vector<std::string> features;
    std::vector<std::vector<float>> data;


public:

	TimeSeries(const char* CSVfileName);
};



#endif /* TIMESERIES_H_ */
