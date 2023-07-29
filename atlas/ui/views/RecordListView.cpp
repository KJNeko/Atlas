//
// Created by kj16609 on 3/29/23.
//

#include "RecordListView.hpp"

#include <moc_RecordListView.cpp>

#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>

#include "core/database/record/Version.hpp"
#include "ui/delegates/RecordBannerDelegate.hpp"
#include "ui/dialog/RecordEditor.hpp"
#include "ui/models/RecordListModel.hpp"

RecordListView::RecordListView( QWidget* parent ) : QListView( parent )
{
	ZoneScoped;
	QListView::setModel( new RecordListModel() );
	setRenderMode( BANNER_VIEW );

	setContextMenuPolicy( Qt::CustomContextMenu );

	connect( this, &RecordListView::customContextMenuRequested, this, &RecordListView::on_customContextMenuRequested );

	CONFIG_ATTACH_THIS;
}

void RecordListView::setRenderMode( const DelegateType type )
{
	spdlog::debug( "Setting Render Mode" );
	ZoneScoped;
	if ( type == current_render_mode ) return;

	switch ( type )
	{
		case BANNER_VIEW:
			{
				QListView::setItemDelegate( new RecordBannerDelegate( static_cast< RecordListModel* >( model() ) ) );
				current_render_mode = BANNER_VIEW;
				QWidget::repaint();
				break;
			}
		case NO_MODE:
			[[fallthrough]];
		default:
			return;
	}

	reloadConfig();
}

void RecordListView::addRecords( const std::vector< RecordID > records )
{
	ZoneScoped;
	auto model { dynamic_cast< RecordListModel* >( QListView::model() ) };

	for ( const auto& record : records ) model->addRecord( atlas::records::Game( record ) );
}

void RecordListView::setRecords( const std::vector< atlas::records::Game > records )
{
	ZoneScoped;
	auto model { dynamic_cast< RecordListModel* >( QListView::model() ) };

	model->setRecords( records );
}

void RecordListView::on_customContextMenuRequested( const QPoint& pos )
{
	ZoneScoped;
	QMenu menu { this };
	menu.move( mapToGlobal( pos ) );

	atlas::records::Game record { selectionModel()->currentIndex().data().value< atlas::records::Game >() };

	//menu.addAction( QString( "Title: %1" ).arg( record->getTitle() ) );
	//menu.addAction( QString( "Creator: %1" ).arg( record->getCreator() ) );

	std::vector< atlas::records::Version > versions { record->m_versions };

	auto version_menu { menu.addMenu( QString( "%1 versions" ).arg( versions.size() ) ) };

	for ( auto& version : versions )
	{
		auto version_submenu { version_menu->addMenu( version->m_version ) };
		version_submenu->addAction( "Launch", [ &version ]() { version.playGame(); } );
		version_submenu->addSeparator();
		version_submenu->addAction( "Delete version" );
	}

	version_menu->addSeparator();
	version_menu->addAction( "Add version" );
	version_menu->addAction(
		"Manage versions",
		[ record, this ]()
		{
			RecordEditor dialog { record->m_game_id, this };
			dialog.show();
			dialog.switchTabs( 2 );
			dialog.exec();
		} );

	//Image stuff
	auto image_menu { menu.addMenu( "Banner/Previews" ) };

	const auto banner { record.requestBanner( Normal ).result() };
	if ( banner.isNull() )
		image_menu->addAction( "Banner not set" );
	else
		image_menu->addAction( QString( "Banner: (%1x%2)" ).arg( banner.width() ).arg( banner.height() ) );

	const auto id { record->m_game_id };

	image_menu->addAction( QString( "%1 previews" ).arg( record->m_preview_paths.size() ) );
	image_menu->addSeparator();
	image_menu->addAction(
		"Set banner",
		[ id, this ]()
		{
			atlas::records::Game game { id };
			const auto path {
				QFileDialog::
					getOpenFileName( this, "Select banner", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.webp)" )
			};
			if ( !path.isEmpty() ) game.setBanner( path.toStdString(), Normal );
		} );
	image_menu->addAction(
		"Add preview",
		[ id, this ]()
		{
			atlas::records::Game game { id };
			const auto path {
				QFileDialog::
					getOpenFileName( this, "Select preview", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.webp)" )
			};
			if ( !path.isEmpty() ) game.addPreview( path.toStdString() );
		} );
	image_menu->addAction(
		"Manage images",
		[ record, this ]()
		{
			RecordEditor dialog { record->m_game_id, this };
			dialog.show();
			dialog.switchTabs( 1 );
			dialog.exec();
		} );

	menu.addAction(
		"Manage record",
		[ record, this ]()
		{
			RecordEditor dialog { record->m_game_id, this };
			dialog.show();
			dialog.exec();
		} );

	menu.exec();
}

void RecordListView::mouseDoubleClickEvent( [[maybe_unused]] QMouseEvent* event )
{
	if ( selectionModel()->hasSelection() )
	{
		emit openDetailedView( selectionModel()->currentIndex().data().value< atlas::records::Game >() );
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void RecordListView::reloadConfig()
{
	spdlog::debug( "Config Reloaded" );
	ZoneScoped;
	switch ( current_render_mode )
	{
		case NO_MODE:
			return;
		case BANNER_VIEW:
			{
				auto delegate { dynamic_cast< RecordBannerDelegate* >( QListView::itemDelegate() ) };
				assert( delegate != nullptr );
				delegate->reloadConfig();
				//Set spacing between each item
				//QListView::setSpacing( delegate->m_grid_spacing );
				delegate->sizeHintChanged( selectionModel()->currentIndex() );

				QWidget::repaint();
				return;
			}
		default:
			return;
	}
}

void RecordListView::paintEvent( QPaintEvent* event )
{
	ZoneScoped;
	static_cast< RecordListModel* >( this->model() )->killLoaders();
	QListView::paintEvent( event );
}
