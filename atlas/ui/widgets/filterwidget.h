#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

namespace Ui {
	class filterwidget;
}

class filterwidget : public QWidget
{
	Q_OBJECT

  public:
	explicit filterwidget(QWidget *parent = nullptr);
	~filterwidget();

  private:
	Ui::filterwidget *ui;
};

#endif // FILTERWIDGET_H
