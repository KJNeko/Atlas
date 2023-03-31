//
// Created by kj16609 on 3/29/23.
//

#include "RecordView.hpp"

#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>

#include <tracy/Tracy.hpp>

#include "ui/delegates/RecordBannerDelegate.hpp"
#include "ui/models/RecordListModel.hpp"

RecordView::RecordView( QWidget *parent ) : QListView( parent )
{
	ZoneScoped;
	QListView::setModel( new RecordListModel() );
	setRenderMode( BANNER_VIEW );

	/*
	QListView::setFlow( QListView::LeftToRight );
	QListView::setWrapping( true );
	QListView::setSpacing( 5 );
	QListView::setResizeMode( QListView::Adjust );
	QListView::setMovement( QListView::Free );*/

	setContextMenuPolicy( Qt::CustomContextMenu );

	connect( this, &RecordView::customContextMenuRequested, this, &RecordView::on_customContextMenuRequested );
}

void RecordView::setRenderMode( const DelegateType type )
{
	ZoneScoped;
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
	ZoneScoped;
	auto model { dynamic_cast< RecordListModel * >( QListView::model() ) };

	for ( const auto &record : records ) model->addRecord( Record( record ) );
}

void RecordView::setRecords( const std::vector< Record > records )
{
	ZoneScoped;
	auto model { dynamic_cast< RecordListModel * >( QListView::model() ) };

	model->setRecords( records );
}

void RecordView::on_customContextMenuRequested( const QPoint &pos )
{
	ZoneScoped;
	QMenu menu { this };
	menu.move( mapToGlobal( pos ) );

	const auto record { selectionModel()->currentIndex().data().value< Record >() };

	//menu.addAction( QString( "Title: %1" ).arg( record->getTitle() ) );
	//menu.addAction( QString( "Creator: %1" ).arg( record->getCreator() ) );

	const auto connection { connect(
		record.get(),
		&RecordData::dataChanged,
		this,
		[ this, pos ]() { this->dataChanged( this->indexAt( pos ), this->indexAt( pos ) ); },
		Qt::SingleShotConnection ) };

	auto version_menu { menu.addMenu( QString( "%1 versions" ).arg( record->getVersions().size() ) ) };
	for ( const auto &version : record->getVersions() )
	{
		auto version_submenu { version_menu->addMenu( version.getVersionName() ) };
		version_submenu->addAction( "Launch" );
		version_submenu->addSeparator();
		version_submenu->addAction( "Delete version" );
	}
	version_menu->addSeparator();
	version_menu->addAction( "Add version" );
	version_menu->addAction( "Manage versions" );

	//Image stuff
	auto image_menu { menu.addMenu( "Banner/Previews" ) };

	const auto banner { record->getBanner() };
	if ( banner.isNull() )
		image_menu->addAction( "Banner not set" );
	else
		image_menu->addAction( QString( "Banner: (%1x%2)" ).arg( banner.width() ).arg( banner.height() ) );

	image_menu->addAction( QString( "%1 previews" ).arg( record->getPreviewPaths().size() ) );
	image_menu->addSeparator();
	image_menu->addAction(
		"Set banner",
		[ record, this ]()
		{
			const auto path {
				QFileDialog::
					getOpenFileName( this, "Select banner", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.webp)" )
			};
			if ( !path.isEmpty() ) record->setBanner( path.toStdString() );
		} );
	image_menu->addAction(
		"Add preview",
		[ record, this ]()
		{
			const auto path {
				QFileDialog::
					getOpenFileName( this, "Select preview", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.webp)" )
			};
			if ( !path.isEmpty() ) record->addPreview( path.toStdString() );
		} );
	image_menu->addAction( "Manage images" );

	menu.addAction( "Manage record" );

	menu.exec();

	disconnect( connection );
}

void RecordView::mouseDoubleClickEvent( [[maybe_unused]] QMouseEvent *event )
{
	ZoneScoped;
	if ( selectionModel()->hasSelection() )
	{
		emit openDetailedView( selectionModel()->currentIndex().data().value< Record >() );
		event->accept();
	}
	else
	{
		event->ignore();
	}
}
