//
// Created by kj16609 on 7/21/23.
//

#include "Notification.hpp"

#include <moc_Notification.cpp>

#include <QMouseEvent>

void Notification::mousePressEvent( QMouseEvent* event )
{
	if ( event->button() == Qt::RightButton )
	{
		emit selfClosePtr( this );
		return;
	}
	else
		QWidget::mousePressEvent( event );
}

Notification::Notification( QWidget* parent ) : QWidget( parent )
{
	connect( this, &Notification::selfClose, this, &Notification::selfCloseTrigger );
}

void Notification::selfCloseTrigger()
{
	emit selfClosePtr( this );
}
