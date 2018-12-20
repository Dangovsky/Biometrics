#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->nameText->installEventFilter(this);
    ui->passwText->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->nameText || obj == ui->passwText) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            timeKeyDown.push_back(keyEvent->timestamp());
        }
    }
    // pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);

}

void MainWindow::keyReleaseEvent(QKeyEvent* e){
    timeKeyUp.push_back(e->timestamp());
}

void MainWindow::on_registrButton_clicked()
{
    model.AddSample(timeKeyDown, timeKeyUp);
    timeKeyDown = {};
    timeKeyUp = {};
}

void MainWindow::on_getModelButton_clicked()
{
    model.SaveModel(ui->nameText->toPlainText());
}

void MainWindow::on_autorisationButton_clicked()
{
    model.AddSample(timeKeyDown, timeKeyUp);
    timeKeyDown = {};
    timeKeyUp = {};
    model.LoadModel(ui->nameText->toPlainText());
    if (model.Autorisation()){
        ui->passwText->setText("Access granted");
    }
    else {
        ui->passwText->setText("Access denied");
    }
}
