//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEWWIDGET_HPP
#define HYDRUS95_RECORDVIEWWIDGET_HPP

#include <QWidget>
#include <QItemSelection>
#include "RecordViewModel.hpp"
#include "RecordViewDelegate.hpp"

struct Record;

QT_BEGIN_NAMESPACE
namespace Ui
{
	class RecordViewWidget;
}
QT_END_NAMESPACE

class RecordViewWidget final : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( RecordViewWidget )

	RecordViewDelegate delegate { this };
	RecordViewModel model { this };

	public:
	explicit RecordViewWidget( QWidget* parent = nullptr );
	~RecordViewWidget() override;

	public slots:
	void selectionChanged( const QItemSelection& selected, const QItemSelection& deseleted );
	void selectionHiding();
	void recordsUpdated( const std::vector< Record >& records );
	void requestUpdate() { emit requestRecordUpdate(); }

	signals:
	void changeSelection( const QPersistentModelIndex& index );
	void requestRecordUpdate();

	private:
	Ui::RecordViewWidget* ui;
};


#endif	//HYDRUS95_RECORDVIEWWIDGET_HPP
