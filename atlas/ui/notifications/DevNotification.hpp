//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_DEVNOTIFICATION_HPP
#define ATLASGAMEMANAGER_DEVNOTIFICATION_HPP

#include "MessageNotification.hpp"
#include "Notification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class DevNotification;
}

QT_END_NAMESPACE

class DevNotification final : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( DevNotification )

	QString m_full_message;

  public:

	explicit DevNotification( const MessageLevel level, QString msg, QString full_message, QWidget* parent = nullptr );
	~DevNotification() override;

  private:

	Ui::DevNotification* ui;

  private slots:
	void on_copyError_pressed();
};

#endif //ATLASGAMEMANAGER_DEVNOTIFICATION_HPP
