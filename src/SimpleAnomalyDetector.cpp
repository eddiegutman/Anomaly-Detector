
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

// create an array of points of given two arrays of data
Point** createPointsArray (float *x, float *y, int size) {
    auto **p = new Point*[size];
    for (int i = 0; i < size; i++) {
        auto* h = new Point(*(x + i), *(y + i));
        *(p + i) = h;
    }
    return p;
}

// free a given points array
void releasePointsArray (Point** p, int size) {
    for (int i = 0; i < size; i++) {
        delete *(p + i);
    }
    delete[] p;
}

// find the max threshold
float findThreshold (Line ln, Point** p, int size) {
    float max = 0;
    for (int i = 0; i < size; i++) {
        float distance = dev(**(p + i), ln);
        if (distance > max)
            max = distance;
    }
    return max * (float)1.1;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    // traverse on each feature
    for (int i = 0; i < ts.featureSize(); i++) {
        float max = 0;
        int c = -1;

        // traverse on the rest of the features
        for (int j = i + 1; j < ts.featureSize(); j++) {
            // calculate the pearson
            float p = pearson(ts.getArray(i), ts.getArray(j), ts.dataSize());

            // check if its bigger than the max pearson
            if (std::abs(p) > max) {
                max = p;
                c = j;
            }
        }

        // check if a correct pearson is found.
        if (c != -1 && abs(max) >= 0.9) {

            // create a correlated features pair
            int size = ts.dataSize();
            Point **points = createPointsArray(ts.getArray(i), ts.getArray(c), size);
            Line ln = linear_reg(points, size);
            float threshold = findThreshold(ln, points, size);
            correlatedFeatures newCorrelation = {ts.getAttributes()[i], ts.getAttributes()[c], max,
                                                 ln, threshold};

            // add the correlated features pair to cf
            this->cf.push_back(newCorrelation);

            // free the point array
            releasePointsArray(points, size);
        }
    }

}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    std::vector<AnomalyReport> reports;
    int size = ts.dataSize();

    // traverse on each timestep line
    for (int i = 0; i < size; i++) {

        // traverse on each correlated feature
        for (auto it = cf.begin(); it < cf.end(); it++) {
            Point p = {ts.getValue(it->feature1, i), ts.getValue(it->feature2, i)};

            // check if there is an anomaly and add it
            if (dev(p, it->lin_reg) > it->threshold) {
                AnomalyReport report = AnomalyReport(it->feature1 + "-" + it->feature2, i+1);
                reports.push_back(report);
            }
        }
    }

    return reports;
}