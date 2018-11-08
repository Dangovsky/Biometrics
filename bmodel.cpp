#include "bmodel.h"

BModel::BModel() : samples(), model()
{}

BModel::~BModel()
{}

vector<long> BModel::GetModel(){
    this->CalculateModel();
    if (model.size() == 0){
        return {};
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

int BModel::CalculateModel(){
    int size;
    if (samples.size() == 0) return -1;
    if ((size = samples.at(0).size()) == 0) return -1;

    m = vector<float>(samples.at(0).size(), 0);
    sigma = vector<float>(samples.at(0).size(), 0);

    for (int i = 0; i < samples.at(0).size(); i++){
        for (int j = 0; j < samples.size(); j++){
            if (size != samples.at(j).size() || samples.at(j).size() == 0) return -1;
            m.at(i) += samples.at(j).at(i);
        }
        m.at(i) /= samples.size();
    }

    for (int i = 0; i < samples.at(0).size(); i++){
        for (int j = 0; j < samples.size(); j++){
            sigma.at(i) += pow(samples.at(j).at(i) - m.at(i), 2);
        }
        sigma.at(i) /= samples.size() - 1;
    }

    for (int i = 0; i < sigma.size(); i++){
        sigma.at(i) = sqrt(sigma.at(i));
    }
}
