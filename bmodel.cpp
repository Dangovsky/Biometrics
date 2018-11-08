#include "bmodel.h"

BModel::BModel() : samples(), model()
{}

BModel::~BModel()
{}

vector<long> BModel::GetModel(){
    if (model.size() == 0){
        return {0};
    }
    return model;
}

void BModel::addSample(vector<unsigned long> timeKeyDown, vector<unsigned long> timeKeyUp){
    if (timeKeyDown.size() == 0 || timeKeyUp.size() == 0) return;
    if (timeKeyDown.size() != timeKeyUp.size()) return;

    samples.push_back(vector<long>(timeKeyDown.size() * 2, 0));
    samples.at(samples.size()-1).at(0) = timeKeyUp[0] - timeKeyDown[0];

    for(int i = 1; i < timeKeyDown.size(); i++){
        samples[samples.size()-1].at(i) = timeKeyUp[i] - timeKeyDown[i];
        samples[samples.size()-1].at(timeKeyDown.size() + i) = timeKeyDown[i] - timeKeyUp[i - 1];
    }
}

void BModel::addSamples(vector<vector<unsigned long>> timeKeyDown, vector<vector<unsigned long>> timeKeyUp){
    if (timeKeyDown.size() == 0 || timeKeyUp.size() == 0) return;
    if (timeKeyDown.size() != timeKeyUp.size()) return;

    for(int i = 0; i < timeKeyDown.size(); i++) {
        this->addSample(timeKeyDown[i], timeKeyUp[i]);
    }
}
