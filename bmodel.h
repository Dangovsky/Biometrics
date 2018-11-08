#ifndef BMODEL_H
#define BMODEL_H

#include <vector>

using namespace std;

class BModel
{
    vector<vector<long>> samples;
    vector<long> model;
public:
    BModel();
    ~BModel();

    vector<long> GetModel();

    void addSample(vector<unsigned long> timeKeyDown, vector<unsigned long> timeKeyUp);
    void addSamples(vector<vector<unsigned long>> timeKeyDown, vector<vector<unsigned long>> timeKeyUp);
};

#endif // BMODEL_H
