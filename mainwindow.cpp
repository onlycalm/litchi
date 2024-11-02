#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetLable(QString LabelText)
{
    ui->label->setText(LabelText);
    ui->label->setGeometry(200, 50, 200, 50);
    ui->label->setStyleSheet("background-color: yellow; color: green;");
}
