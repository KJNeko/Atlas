//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_STATSDIALOG_HPP
#define ATLASGAMEMANAGER_STATSDIALOG_HPP

#include <QDialog>
#include <QLineSeries>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class StatsDialog;
}

QT_END_NAMESPACE

class StatsDialog : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( StatsDialog )

  public:

	explicit StatsDialog( QWidget* parent = nullptr );
	~StatsDialog() override;

  private:

	Ui::StatsDialog* ui;
};

#endif //ATLASGAMEMANAGER_STATSDIALOG_HPP
