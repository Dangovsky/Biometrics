#include "bmodel.h"

BModel::BModel() : samples(), model(), min(), max()
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

    vector<float> m = vector<float>(samples.at(0).size(), 0);
    for (int i = 0; i < samples.at(0).size(); i++){
        for (int j = 0; j < samples.size(); j++){
            if (size != samples.at(j).size() || samples.at(j).size() == 0) return -1;
            m.at(i) += samples.at(j).at(i);
        }
        m.at(i) /= samples.size();
    }

    vector<float> sigma = vector<float>(samples.at(0).size(), 0);
    for (int i = 0; i < samples.at(0).size(); i++){
        for (int j = 0; j < samples.size(); j++){
            sigma.at(i) += pow(samples.at(j).at(i) - m.at(i), 2);
        }
        sigma.at(i) /= samples.size() - 1;
        sigma.at(i) = sqrt(sigma.at(i));
    }

    min = vector<double>(m.size(), 0);
    max = vector<double>(m.size(), 0);
    for (int i = 0; i < min.size(); i++){
        min[i] = m[i] - 1.38 * sigma[i];
        max[i] = m[i] + 1.38 * sigma[i];
    }

    vector<int> hemming = vector<int>(samples.size(), 0);;
    int tmp;
    for (int i = 0; i < samples.size(); i++){
        tmp = 0;
        for (int j = 0; j < samples.at(0).size(); j++){
            if (samples.at(i).at(j) > max.at(j) || samples.at(i).at(j) < min.at(j))
                tmp++;
        }
        hemming.at(i) = tmp;
    }

    double hemming_m = 0;
    for (auto& n : hemming)
        hemming_m += n;
    hemming_m /= hemming.size();

    double hemming_sigma = 0;
    for (auto& n : hemming)
        hemming_sigma += pow(n - hemming_m, 2);
    hemming_sigma /= hemming.size() - 1;
    hemming_sigma  = sqrt(hemming_sigma);

    threshold = hemming_m + 1.38 * hemming_sigma;

    threshold = hemming_m + 1.38 * hemming_sigma;
}
