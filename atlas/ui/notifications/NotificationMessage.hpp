//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONMESSAGE_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONMESSAGE_HPP

#include <QWidget>

#include "NotificationPopup.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class NotificationMessage;
}

QT_END_NAMESPACE

class NotificationMessage final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( NotificationMessage )


  public:

	explicit NotificationMessage( const QString msg, QWidget* parent = nullptr );
	~NotificationMessage() override;

	using Signaler = void;

	Signaler getSignaler() { return; }

  private:

	Ui::NotificationMessage* ui;
	friend class NotificationPopup;

  public slots:
	void on_btnDismiss_clicked();
	void closeSelf();
};

#endif //ATLASGAMEMANAGER_NOTIFICATIONMESSAGE_HPP
