#include "filterwidget.h"
#include "ui_filterwidget.h"

filterwidget::filterwidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::filterwidget)
{
	ui->setupUi(this);
}

filterwidget::~filterwidget()
{
	delete ui;
}
