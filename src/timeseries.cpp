#include "timeseries.h"


TimeSeries::TimeSeries(const char* CSVfileName) {
    std::ifstream stream(CSVfileName);
    std::string line, cell;
    std::getline(stream,line);
    std::stringstream headlineStream(line);

    while(std::getline(headlineStream,cell, ',')) {
        this->features.push_back(cell);
    }


    for (int j = 0; j < features.size() ; j++) {
        std::vector<float> p;
        data.push_back(p);
    }

    int i = 0;
    while (std::getline(stream, line)) {
        std::stringstream lineStream(line);
        while(std::getline(lineStream,cell, ',')) {
            data[i].push_back(std::stof(cell));
        }
        i++;
    }
}


