//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_MESSAGENOTIFICATION_HPP
#define ATLASGAMEMANAGER_MESSAGENOTIFICATION_HPP

#include "Notification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class MessageNotification;
}

QT_END_NAMESPACE

class MessageNotification : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( MessageNotification )

  public:

	explicit MessageNotification( QString str, QWidget* parent = nullptr );
	~MessageNotification() override;

  private:

	Ui::MessageNotification* ui;
};

#endif //ATLASGAMEMANAGER_MESSAGENOTIFICATION_HPP
