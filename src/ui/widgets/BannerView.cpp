//
// Created by kj16609 on 3/1/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BannerView.h" resolved

#include "BannerView.hpp"

#include <QDragEnterEvent>
#include <QMimeData>

#include <h95/logging.hpp>
#include <QPixmapCache>

BannerView::BannerView( QWidget* parent ) : QLabel( parent )
{
	setAcceptDrops( true );
	setAlignment( Qt::AlignCenter );
}

BannerView::~BannerView() {}

void BannerView::dragEnterEvent( QDragEnterEvent* event )
{
	if ( edit_allowed )
	{
		if ( event->mimeData()->hasUrls() && event->mimeData()->hasFormat( "text/uri-list" ) )
		{
			event->acceptProposedAction();
		}
	}
}

void BannerView::dropEvent( QDropEvent* event )
{
	if ( event->mimeData()->hasUrls() )
	{
		for ( const auto& url : event->mimeData()->urls() )
		{
			if ( url.isLocalFile() )
			{
				m_banner = url.toLocalFile().toStdString();
				loadImage();
				event->accept();
				return;
			}
		}
	}
}

void BannerView::setEditable( const bool flag )
{
	edit_allowed = flag;
}

void BannerView::setBanner( const std::filesystem::path& banner )
{
	m_banner = banner;
	loadImage();
}

void BannerView::loadImage()
{
	QPixmap pixmap;

	const auto& str { QString::fromStdString( m_banner.string() ) };

	if ( !QPixmapCache::find( str, &pixmap ) )
	{
		pixmap.load( str );
		QPixmapCache::insert( str, pixmap );
	}

	if ( pixmap.isNull() ) return;

	pixmap = pixmap.scaledToHeight( 150 );
	if ( pixmap.width() > width() ) pixmap = pixmap.scaledToWidth( width() );

	setPixmap( std::move( pixmap ) );
	setMinimumSize( 150, 1 );
}

std::filesystem::path BannerView::path() const
{
	return m_banner;
}

void BannerView::resizeEvent( QResizeEvent* event )
{
	QWidget::resizeEvent( event );
	loadImage();
}
