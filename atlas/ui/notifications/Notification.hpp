//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATION_HPP
#define ATLASGAMEMANAGER_NOTIFICATION_HPP

#include <QDialog>
#include <QWidget>

#pragma GCC diagnostic push
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

class Notification : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( Notification )

  public:

	Notification( QWidget* parent = nullptr );

	~Notification() {}

	void mousePressEvent( QMouseEvent* event ) override final;

  public:

  public slots:
	void selfCloseTrigger();

  signals:
	void selfClose();
	void selfClosePtr( Notification* ptr );
};

#pragma GCC diagnostic pop

#endif //ATLASGAMEMANAGER_NOTIFICATION_HPP
