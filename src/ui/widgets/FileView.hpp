//
// Created by kj16609 on 1/10/23.
//

#ifndef IDHAN_FILEVIEW_HPP
#define IDHAN_FILEVIEW_HPP

#include <QWidget>
#include <QTimer>
#include <QPixmap>

#include <queue>
#include <mutex>

#include "backend/records/Record.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class FileView;
}
QT_END_NAMESPACE

class FileView : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( FileView )

	std::vector< Record > records {};
	std::mutex records_mtx {};
	std::vector< Record > selection {};
	std::remove_pointer_t< decltype( records ) >::size_type previous_selection { 0 };

	void recalcScrollArea();
	std::remove_pointer_t< decltype( records ) >::size_type indexFromPos( const QPointF pos );
	void paintEvent( QPaintEvent* event ) override;
	void resizeEvent( QResizeEvent* event ) override;

	public:
	explicit FileView( QWidget* parent = nullptr );
	~FileView() override;

	void setRecords( std::vector< Record > records_in )
	{
		records = std::move( records_in );
		recalcScrollArea();
		QWidget::repaint();
	}

	void addRecord( Record record )
	{
		records.push_back( record );
		recalcScrollArea();
		QWidget::repaint();
	}

	public slots:
	void recordsUpdated();
	void on_verticalScrollBar_valueChanged( int value );
	void wheelEvent( QWheelEvent* event ) override;
	void mousePressEvent( QMouseEvent* event ) override;
	void mouseDoubleClickEvent( QMouseEvent* event ) override;
	void contextMenuEvent( QContextMenuEvent* event ) override;

	signals:
	void selectionChanged( const std::vector< Record >& records_in );

	private:
	Ui::FileView* ui;
};

#endif	//IDHAN_FILEVIEW_HPP
