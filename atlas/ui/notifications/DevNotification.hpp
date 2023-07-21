//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_DEVNOTIFICATION_HPP
#define ATLASGAMEMANAGER_DEVNOTIFICATION_HPP

#include "Notification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class DevNotification;
}

QT_END_NAMESPACE

class DevNotification : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( DevNotification )

  public:

	explicit DevNotification( QWidget* parent = nullptr );
	~DevNotification() override;

  private:

	Ui::DevNotification* ui;
};

#endif //ATLASGAMEMANAGER_DEVNOTIFICATION_HPP
