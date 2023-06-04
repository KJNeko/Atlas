//
// Created by kj16609 on 6/4/23.
//

#include "ImageWidget.hpp"

#include <QDragEnterEvent>
#include <QMimeData>

ImageWidget::ImageWidget( QWidget* parent, Qt::WindowFlags f ) : QLabel( parent, f )
{}

void ImageWidget::setPath( const QString path )
{
	m_path = path;
	m_pixmap = QPixmap( path );

	setPixmap( scaledPixmap() );
	emit pathChanged( path );
}

void ImageWidget::dragEnterEvent( QDragEnterEvent* event )
{
	if ( event->mimeData()->hasUrls() )
	{
		event->acceptProposedAction();
		return;
	}

	QWidget::dragEnterEvent( event );
}

void ImageWidget::dropEvent( QDropEvent* event )
{
	if ( event->isAccepted() && event->mimeData()->hasUrls() )
	{
		const auto urls { event->mimeData()->urls() };
		if ( urls[ 0 ].isLocalFile() )
		{
			const auto path { urls[ 0 ].toLocalFile() };
			setPath( path );
			return;
		}
	}

	QWidget::dropEvent( event );
}

void ImageWidget::resizeEvent( QResizeEvent* event )
{
	QLabel::resizeEvent( event );
	//TODO: Uncomment this once the tabs in the SingleImporter are fixed to not fight eachother on the minimum size constraints.
	//setPixmap( scaledPixmap() );
}

QPixmap ImageWidget::scaledPixmap()
{
	return m_pixmap.scaled( size() - QSize( 50, 35 ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
}
