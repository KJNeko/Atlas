#include "Console.h"

#include <moc_Console.cpp>

#include "ui_Console.h"

Console::Console(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
}

Console::~Console()
{
    delete ui;
}
