//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONMESSAGE_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONMESSAGE_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class NotificationMessage;
}

QT_END_NAMESPACE

class NotificationMessage : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( NotificationMessage )

  public:

	explicit NotificationMessage( const QString msg, QWidget* parent = nullptr );
	~NotificationMessage() override;

  private:

	Ui::NotificationMessage* ui;

  private slots:
	void on_btnDismiss_clicked();
};

namespace atlas::notifications
{
	void createNotification( const QString text, const bool reveal );
}

#endif //ATLASGAMEMANAGER_NOTIFICATIONMESSAGE_HPP
