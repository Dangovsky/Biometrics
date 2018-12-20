#include "bmodel.h"

#define STUDENT_T 1.38

BModel::BModel() : samples(), min(), max()
{
    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName("biometrics.db");
    if (!dbase.open()) {
       qDebug() << "Can not open connection to SQLite";
    }
}

BModel::~BModel()
{
    dbase.close();
}

void BModel::SaveModel(QString name){
    CalculateModel();

    if (name.size() == 0){
        qDebug() << "Can not save model, name is empty";
        return;
    }
    if (min.size() == 0 || max.size() == 0){
        qDebug() << "Can not save model, min/max is empty";
        return;
    }

    QSqlQuery a_query;
    // DDL query
    QString str_insert = "insert into Users (Name, Max, Min, Threshold) values(\"" + name + "\",\"";
    for (auto &n : max){
        str_insert += QString::number(n) + " ";
    }
    str_insert += "\",\"";
    for (auto &n : min){
        str_insert += QString::number(n) + " ";
    }
    str_insert += "\"," + QString::number(threshold) + ");";

    bool b = a_query.exec(str_insert);
    if (!b) {
        qDebug() << "Can not save model";
    }
}

void BModel::LoadModel(QString name){
    if (name.size() == 0){
        qDebug() << "Can not load model, name is empty";
        return;
    }

    QSqlQuery query;
    QString str_select= "SELECT * FROM Users where name = \"" + name + "\";";
    //QString str_select= "SELECT * FROM Users";
    bool b = query.exec(str_select);
    if (!b) {
        qDebug() << "Can not load model";
    }
    query.first();

    QString str = query.value("Max").toString();
    QStringList str_list = str.split(' ', QString::SkipEmptyParts);
    max.clear();
    for (auto &s : str_list)
        max.push_back(s.toInt());

    str = query.value("Min").toString();
    str_list = str.split(' ', QString::SkipEmptyParts);
    min.clear();
    for (auto &s : str_list)
        min.push_back(s.toInt());

    threshold = query.value("Threshold").toInt();
}

void BModel::AddSample(vector<unsigned long> timeKeyDown, vector<unsigned long> timeKeyUp){
    if (timeKeyDown.size() == 0 || timeKeyUp.size() == 0) return;
    if (timeKeyDown.size() != timeKeyUp.size()) return;

    samples.push_back(vector<long>(timeKeyDown.size() * 2, 0));
    samples.at(samples.size()-1).at(0) = timeKeyUp[0] - timeKeyDown[0];

    for(int i = 1; i < timeKeyDown.size(); i++){
        samples[samples.size()-1].at(i) = timeKeyUp[i] - timeKeyDown[i];
        samples[samples.size()-1].at(timeKeyDown.size() + i) = timeKeyDown[i] - timeKeyUp[i - 1];
    }
}

void BModel::AddSamples(vector<vector<unsigned long>> timeKeyDown, vector<vector<unsigned long>> timeKeyUp){
    if (timeKeyDown.size() == 0 || timeKeyUp.size() == 0) return;
    if (timeKeyDown.size() != timeKeyUp.size()) return;

    for(int i = 0; i < timeKeyDown.size(); i++) {
        this->AddSample(timeKeyDown[i], timeKeyUp[i]);
    }
}

bool BModel::Autorisation(){
    if (samples.at(samples.size() - 1).size() != min.size()) return false;

    int cnt = 0;
    for (int j = 0; j < samples.at(0).size(); j++){
        if (samples.at(samples.size() - 1).at(j) > max.at(j) || samples.at(samples.size() - 1).at(j) < min.at(j))
            cnt++;
    }
    samples.clear();
    return cnt <= threshold;
}

void BModel::CalculateModel(){
    int size;
    if (samples.size() == 0) return;
    if ((size = samples.at(0).size()) == 0) return;

    vector<float> m = vector<float>(samples.at(0).size(), 0);
    for (int i = 0; i < samples.at(0).size(); i++){
        for (int j = 0; j < samples.size(); j++){
            if (size != samples.at(j).size() || samples.at(j).size() == 0) return;
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

    min = vector<long>(m.size(), 0);
    max = vector<long>(m.size(), 0);
    for (int i = 0; i < min.size(); i++){
        min[i] = m[i] - STUDENT_T * sigma[i];
        max[i] = m[i] + STUDENT_T * sigma[i];
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

    threshold = hemming_m + STUDENT_T * hemming_sigma;

    samples.clear();
}
