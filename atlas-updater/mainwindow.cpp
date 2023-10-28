#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <elzip.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

