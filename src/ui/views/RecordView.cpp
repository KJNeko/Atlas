//
// Created by kj16609 on 3/29/23.
//

#include "RecordView.hpp"

#include "ui/delegates/RecordBannerDelegate.hpp"
#include "ui/models/RecordListModel.hpp"

RecordView::RecordView( QWidget *parent ) : QListView( parent )
{
	QListView::setModel( new RecordListModel() );
	setRenderMode( BANNER_VIEW );

	QListView::setFlow( QListView::LeftToRight );
	QListView::setWrapping( true );
	QListView::setSpacing( 5 );
	QListView::setResizeMode( QListView::Adjust );
	QListView::setMovement( QListView::Free );
}

void RecordView::setRenderMode( const DelegateType type )
{
	if ( type == current_render_mode ) return;

	switch ( type )
	{
		case BANNER_VIEW:
			{
				QListView::setItemDelegate( new RecordBannerDelegate() );
				break;
			}
		default:
			break;
	}
}

void RecordView::addRecords( const std::vector< RecordID > records )
{
	auto model { dynamic_cast< RecordListModel * >( QListView::model() ) };

	for ( const auto record : records ) model->addRecord( Record( record ) );
}

void RecordView::setRecords( const std::vector< Record > records )
{
	auto model { dynamic_cast< RecordListModel * >( QListView::model() ) };

	model->setRecords( records );
}
