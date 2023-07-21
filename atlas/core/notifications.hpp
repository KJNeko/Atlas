//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONS_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONS_HPP

#include <QString>

#include "ui/notifications/DevNotification.hpp"
#include "ui/notifications/GroupedProgressNotification.hpp"
#include "ui/notifications/MessageNotification.hpp"
#include "ui/notifications/NotificationManagerUI.hpp"
#include "ui/notifications/NotificationSignalers.hpp"
#include "ui/notifications/ProgressNotification.hpp"

namespace atlas::notifications
{
	void initNotifications( QWidget* parent );
	NotificationManagerUI& handle();

	void createMessage( QString message );

	Signaler< ProgressNotification > createProgressMessage( QString str );
	//Signaler< GroupedProgressNotification > createGroupedProcessMessage();

	template < typename... Ts >
	void createDevNotification( const QString message, const Ts&... ts );

} // namespace atlas::notifications

#endif //ATLASGAMEMANAGER_NOTIFICATIONS_HPP
