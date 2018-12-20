#ifndef BMODEL_H
#define BMODEL_H

#include <vector>
#include <cmath>
#include <QtSql>
#include <QString>
#include <QStringList>

using namespace std;

class BModel
{
    QSqlDatabase dbase;
    vector<vector<long>> samples;
    vector<long> min, max;
    long threshold;
public:
    BModel();
    ~BModel();

    void SaveModel(QString name);
    void LoadModel(QString name);

    bool Autorisation();

    void AddSample(vector<unsigned long> timeKeyDown, vector<unsigned long> timeKeyUp);
    void AddSamples(vector<vector<unsigned long>> timeKeyDown, vector<vector<unsigned long>> timeKeyUp);

private:
    void CalculateModel();
};

#endif // BMODEL_H
