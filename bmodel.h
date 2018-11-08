#ifndef BMODEL_H
#define BMODEL_H

#include <vector>
#include <cmath>

using namespace std;

class BModel
{
    vector<vector<long>> samples;
    vector<long> model;
    vector<float> m;
    vector<float> sigma;
public:
    BModel();
    ~BModel();

    vector<long> GetModel();

    void addSample(vector<unsigned long> timeKeyDown, vector<unsigned long> timeKeyUp);
    void addSamples(vector<vector<unsigned long>> timeKeyDown, vector<vector<unsigned long>> timeKeyUp);

private:
    int CalculateModel();
};

#endif // BMODEL_H
