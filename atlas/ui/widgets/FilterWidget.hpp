
#pragma once
#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class FilterWidget;
}

QT_END_NAMESPACE

class FilterWidget final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( FilterWidget )

  public:

	explicit FilterWidget( QWidget* parent = nullptr );
	~FilterWidget();

  private:

	Ui::FilterWidget* ui;
};

#endif // FILTERWIDGET_H
