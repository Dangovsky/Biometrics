#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <vector>
#include "bmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    BModel model;
    std::vector<unsigned long> timeKeyDown;
    std::vector<unsigned long> timeKeyUp;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void keyReleaseEvent(QKeyEvent* e);

private slots:
    void on_registrButton_clicked();

    void on_getModelButton_clicked();

    void on_autorisationButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
