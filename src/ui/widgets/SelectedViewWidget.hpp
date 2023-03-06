//
// Created by kj16609 on 1/18/23.
//

#ifndef HYDRUS95_SELECTEDVIEWWIDGET_HPP
#define HYDRUS95_SELECTEDVIEWWIDGET_HPP

#include <QWidget>
#include <QPersistentModelIndex>

#include "h95/database/Record.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class SelectedViewWidget;
}
QT_END_NAMESPACE

class SelectedViewWidget final : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( SelectedViewWidget )

	std::optional< QPersistentModelIndex > selected { std::nullopt };

	public:
	explicit SelectedViewWidget( QWidget* parent = nullptr );
	~SelectedViewWidget() override;

	public slots:
	void recordSelected( const QPersistentModelIndex& );
	void on_closeButton_pressed();
	void on_playButton_pressed();

	void keyPressEvent( QKeyEvent* event ) override;
	void resizeEvent( QResizeEvent* event ) override;

	signals:
	void hiding();

	private:
	Ui::SelectedViewWidget* ui;
};


#endif	//HYDRUS95_SELECTEDVIEWWIDGET_HPP
