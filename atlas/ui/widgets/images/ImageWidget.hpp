//
// Created by kj16609 on 6/4/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_IMAGEWIDGET_HPP
#define ATLASGAMEMANAGER_IMAGEWIDGET_HPP

#include <QLabel>

class ImageWidget final : public QLabel
{
	Q_OBJECT

	QString m_path {};
	QPixmap m_pixmap {};

	QPixmap scaledPixmap();

  public:

	ImageWidget( QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags() );

	void setPath( const QString path );

	QString path() const { return m_path; }

	//Drag n' drop stuff
	void dragEnterEvent( QDragEnterEvent* event ) override;
	void dropEvent( QDropEvent* event ) override;
	void resizeEvent( QResizeEvent* event ) override;

  signals:
	void pathChanged( const QString& path );
};

#endif //ATLASGAMEMANAGER_IMAGEWIDGET_HPP
