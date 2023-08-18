#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

namespace Ui {
	class FilterWidget;
}

class FilterWidget : public QWidget
{
	Q_OBJECT

  public:
	explicit FilterWidget(QWidget *parent = nullptr);
	~FilterWidget();

  private:
	Ui::FilterWidget *ui;
};

#endif // FILTERWIDGET_H
