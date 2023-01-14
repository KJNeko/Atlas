//
// Created by kj16609 on 1/10/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FileView.h" resolved

#include "FileView.hpp"
#include "ui_FileView.h"
#include "ui/dialog/RecordEditor.hpp"
#include <QPainter>
#include <QPicture>
#include <QPaintEvent>
#include <QPixmap>

#include <stack>

#include <backend/config.hpp>
#include <backend/records/RecordBase.hpp>
#include <QMenu>
#include <QDesktopServices>

FileView::FileView( QWidget* parent ) : QWidget( parent ), ui( new Ui::FileView )
{
	ui->setupUi( this );
}

FileView::~FileView()
{
	delete ui;
}

void FileView::recalcScrollArea()
{
	const auto thumb_height { getSettings< int >( "records_render/y_res", 200 ) };
	const auto thumb_width { getSettings< int >( "records_render/x_res", 300 ) };
	const auto thumbnail_padding { getSettings< int >( "records_render/padding", 8 ) };
	const auto item_padding { getSettings< int >( "records_render/item_padding", 8 ) };
	const auto item_height { thumb_height + item_padding + thumbnail_padding };
	const auto item_width { thumb_width + item_padding + thumbnail_padding };
	const auto rect { ui->frame->rect() };
	const auto frame_width { rect.width() };
	const auto items_per_row {
		std::floor( static_cast< double >( frame_width - item_padding ) / static_cast< double >( item_width ) ) };
	//Calculate number of rows
	const auto rows { std::max( std::ceil( static_cast< double >( records.size() ) / items_per_row ), 1.0 ) };
	const auto virtual_height { rows * item_height };

	ui->verticalScrollBar->setSingleStep( item_height );
	ui->verticalScrollBar->setPageStep( rect.height() );
	ui->verticalScrollBar->setMaximum(
		std::max( static_cast< int >( virtual_height ) - ui->verticalScrollBar->pageStep(), 0 ) );

	if ( ui->verticalScrollBar->maximum() == 0 )
		ui->verticalScrollBar->hide();
	else
		ui->verticalScrollBar->show();
}

void FileView::paintEvent( QPaintEvent* event )
{
	//Calculate where we are in the virtual scroll area
	const auto rect { ui->frame->rect() };

	if ( !rect.intersects( event->rect() ) ) return;

	const auto frame_width { rect.width() };
	const auto thumb_height { getSettings< int >( "records_render/y_res", 200 ) };
	const auto thumb_width { getSettings< int >( "records_render/x_res", 300 ) };
	const auto thumbnail_padding { getSettings< int >( "records_render/padding", 8 ) };
	const auto item_padding { getSettings< int >( "records_render/item_padding", 8 ) };
	const auto item_height { thumb_height + item_padding + thumbnail_padding };
	const auto item_width { thumb_width + item_padding + thumbnail_padding };
	const int items_per_row { static_cast< int >(
		std::floor( static_cast< double >( frame_width - item_padding ) / static_cast< double >( item_width ) ) ) };

	//Calculate our current row
	const auto top_pixel { ui->verticalScrollBar->value() };
	const auto bottom_pixel { top_pixel + ui->verticalScrollBar->pageStep() };

	const int top_row_index {
		static_cast< int >( std::floor( static_cast< double >( top_pixel ) / static_cast< double >( item_height ) ) ) };
	const int bottom_row_index { static_cast< int >(
		std::ceil( static_cast< double >( bottom_pixel ) / static_cast< double >( item_height ) ) ) };

	const auto pixel_offset { ui->verticalScrollBar->value() };

	QPainter painter { this };
	painter.save();

	for ( auto y = top_row_index; y < bottom_row_index; ++y )
	{
		for ( auto x = 0; x < items_per_row; ++x )
		{
			const auto index {
				static_cast< std::remove_pointer_t< decltype( records ) >::size_type >( y * items_per_row + x ) };
			if ( index >= records.size() ) break;

			const QRect item_rect { item_width * x, ( item_height * y ) - pixel_offset, item_width, item_height };
			const QPoint center { item_rect.center() };
			const QRect box_rect {
				center.x() - ( ( item_width - item_padding ) / 2 ) + item_padding,
				center.y() - ( ( item_height - item_padding ) / 2 ),
				item_width - item_padding,
				item_height - item_padding };

			auto& record { records.at( index ) };
			const QImage banner { record->getBanner() };
			const QPixmap thumbnail { QPixmap::fromImage( banner ) };

			const QRect thumb_rect {
				center.x() - ( thumbnail.width() / 2 ),
				center.y() - ( thumbnail.height() / 2 ),
				thumbnail.width(),
				thumbnail.height() };

			painter.drawRect( box_rect );
			painter.drawPixmap( thumb_rect, thumbnail );

			if ( std::find( selection.begin(), selection.end(), record ) != selection.end() )
			{
				painter.save();

				painter.setBrush( QColor( 0, 0, 255, 50 ) );
				painter.drawRect( box_rect );
				painter.restore();
			}

			painter.drawText( box_rect, record->getName(), Qt::AlignHCenter | Qt::AlignTop );

			//Styling + extra information
			const auto record_type {record->getType()};
			switch(record_type)
			{
				case RecordType::NORMAL_RECORD:
					painter.drawText( box_rect, record->getDesc(), Qt::AlignHCenter | Qt::AlignBottom );
					break;
				case RecordType::GAME_RECORD:
				{
					const auto& g_record {record->getSubRecord<GameRecord>()};
					painter.drawText( box_rect, g_record.getVersion() );
					break;
				}
				default:
					break;
			}
		}
	}

	painter.restore();
}

void FileView::recordsUpdated()
{
	recalcScrollArea();
	QWidget::repaint();
}

void FileView::resizeEvent( QResizeEvent* event )
{
	recalcScrollArea();
	QWidget::resizeEvent( event );
}

void FileView::on_verticalScrollBar_valueChanged( [[maybe_unused]] int value )
{
	QWidget::repaint();
}

void FileView::wheelEvent( QWheelEvent* event )
{
	const QPoint num_pixels { event->pixelDelta() };

	if ( !num_pixels.isNull() )
	{
		const int y { num_pixels.y() };
		const int scroll_value { ( y / std::max( std::abs( y ), 1 ) ) * ui->verticalScrollBar->singleStep() };
		ui->verticalScrollBar->setValue(
			std::min( ui->verticalScrollBar->value() - scroll_value, ui->verticalScrollBar->maximum() ) );
		event->accept();
	}
	else
		event->ignore();
}

std::vector< Record >::size_type FileView::indexFromPos( const QPointF pos )
{
	//Calculate the row we are on
	const auto frame_width { this->rect().width() };
	const auto thumb_width { getSettings< int >( "records_render/x_res", 300 ) };
	const auto thumbnail_padding { getSettings< int >( "records_render/padding", 8 ) };
	const auto item_padding { getSettings< int >( "records_render/item_padding", 8 ) };
	const auto item_width { thumb_width + item_padding + thumbnail_padding };
	const int items_per_row { static_cast< int >(
		std::floor( static_cast< double >( frame_width - item_padding ) / static_cast< double >( item_width ) ) ) };
	const int row_index { static_cast< int >( std::floor(
		( pos.y() + ui->verticalScrollBar->value() )
		/ static_cast< double >( ui->verticalScrollBar->singleStep() ) ) ) };
	const int col_index { static_cast< int >(
		std::floor( ( pos.x() - item_padding ) / static_cast< double >( item_width + item_padding ) ) ) };

	if ( pos.x() > item_width * items_per_row ) return std::numeric_limits< std::vector< Record >::size_type >::max();

	return static_cast< std::vector< Record >::size_type >( ( row_index * items_per_row ) + col_index );
}

void FileView::mousePressEvent( [[maybe_unused]] QMouseEvent* event )
{
	std::lock_guard guard( records_mtx );
	if ( records.size() == 0 )
	{
		event->ignore();
		return;
	}

	const auto pos { event->position().toPoint() };
	if ( !this->rect().contains( pos ) )
	{
		event->ignore();
		return;
	}
	const auto index { indexFromPos( pos ) };
	if ( index >= records.size() )
	{
		selection.clear();
		QWidget::repaint();
		return;
	}

	switch ( event->button() )
	{
		case Qt::LeftButton:
		{
			event->accept();
			if ( !( event->modifiers() & Qt::ControlModifier ) ) selection.clear();

			if ( event->modifiers() & Qt::ShiftModifier )
			{
				if ( static_cast< int >( previous_selection ) - static_cast< int >( index ) < 0 )
					for ( auto i = previous_selection; i < index + 1; ) selection.push_back( records.at( i++ ) );
				else
					for ( auto i = previous_selection; i + 1 < index; ) selection.push_back( records.at( i-- ) );
			}
			else
			{
				const auto itter { std::find( selection.begin(), selection.end(), records.at( index ) ) };
				if ( itter != selection.end() ) { selection.erase( itter ); }
				else
				{
					selection.push_back( records.at( index ) );
					previous_selection = index;
				}
			}
			QWidget::repaint();
			break;
		}
		case Qt::RightButton:
		{
			event->accept();
			const auto itter { std::find( selection.begin(), selection.end(), records.at( index ) ) };
			if ( itter == selection.end() )
			{
				selection.clear();
				selection.push_back( records.at( index ) );

				std::sort(
					selection.begin(),
					selection.end(),
					[]( const Record one, const Record two ) { return one->record_id < two->record_id; } );
				const auto last { std::unique(
					selection.begin(),
					selection.end(),
					[]( const Record one, const Record two ) { return one->record_id == two->record_id; } ) };
				selection.erase( last, selection.end() );

				QWidget::repaint();
			}
			break;
		}
		default:
			break;
	}
}

void FileView::mouseDoubleClickEvent( QMouseEvent* event )
{
	event->ignore();
}

void FileView::contextMenuEvent( [[maybe_unused]] QContextMenuEvent* event )
{
	if(selection.size() == 0)
		return;

	auto record{selection.at(previous_selection)};
	const auto type {record->getType()};

	QMenu menu;

	menu.addAction("Modify record", [&](){
		auto ptr {new RecordEditor(record)};
		ptr->exec();
		ptr->deleteLater();
	});

	menu.addAction("Open file location", [&](){
	  QDesktopServices::openUrl(QUrl::fromLocalFile(record->getPath().parent_path().c_str()));
	})
		->setEnabled(record->getPath().empty());

	menu.addSeparator();

	switch(type)
	{
		case RecordType::GAME_RECORD:
		{
			menu.addAction(QIcon(":/icons/playicon"), "Play", [&](){
			  record->getSubRecord<GameRecord>().execute_game();
			});

			QMenu* link_menu { new QMenu( "Open thread" ) };
			link_menu->addAction( QIcon( ":/icons/links/f95icon" ), "F95" );
			link_menu->addAction( QIcon( ":/icons/links/steamicon" ), "Steam" );
			link_menu->addAction( QIcon( ":/icons/links/dlsiteicon" ), "DLSite" );
			menu.addMenu( link_menu );
			link_menu->deleteLater();
			break;
		}
		case RecordType::NORMAL_RECORD:

			break;
		default:
			break;
	}


	menu.exec(event->globalPos());
	return;
}
