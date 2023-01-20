//
// Created by kj16609 on 1/18/23.
//

#ifndef HYDRUS95_SELECTEDVIEWWIDGET_HPP
#define HYDRUS95_SELECTEDVIEWWIDGET_HPP

#include <QWidget>

#include "h95/database/Record.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class SelectedViewWidget;
}
QT_END_NAMESPACE

class SelectedViewWidget : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( SelectedViewWidget )

	std::optional< Record > selected { std::nullopt };

	public:
	explicit SelectedViewWidget( QWidget* parent = nullptr );
	~SelectedViewWidget() override;

	public slots:
	void recordSelected( const Record& record );

	private:
	Ui::SelectedViewWidget* ui;
};


#endif	//HYDRUS95_SELECTEDVIEWWIDGET_HPP
