#include "FilterWidget.hpp"
#include "ui_FilterWidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FilterWidget)
{
	ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
	delete ui;
}
