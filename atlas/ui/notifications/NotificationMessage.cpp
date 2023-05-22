//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NotificationMessage.h" resolved

#include "NotificationMessage.hpp"

#include "NotificationPopup.hpp"
#include "core/logging.hpp"
#include "ui_NotificationMessage.h"

NotificationMessage::NotificationMessage( const QString msg, QWidget* parent ) :
  QWidget( parent ),
  ui( new Ui::NotificationMessage )
{
	ui->setupUi( this );
	ui->label->setText( msg );
}

NotificationMessage::~NotificationMessage()
{
	delete ui;
}

void NotificationMessage::on_btnDismiss_clicked()
{
	getNotificationPopup()->removeMessage( this );
}

namespace atlas::notifications
{
	void createNotification( const QString text, const bool reveal )
	{
		auto& task_popup = *getNotificationPopup();
		auto* notification = new NotificationMessage( text );
		task_popup.addMessage( notification );

		if ( reveal )
		{
			task_popup.show();
			task_popup.expand();
		}
	}
} // namespace atlas::notifications
