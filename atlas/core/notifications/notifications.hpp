//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONS_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONS_HPP

#include <QJsonArray>
#include <QString>

#include "core/logging/formatters.hpp"
#include "ui/notifications/DevNotification.hpp"
#include "ui/notifications/MessageNotification.hpp"
#include "ui/notifications/NotificationManagerUI.hpp"
#include "ui/notifications/NotificationSignalers.hpp"
#include "ui/notifications/ProgressNotification.hpp"

namespace atlas::notifications
{
	void initNotifications( QWidget* parent );
	NotificationManagerUI& handle();

	/**
	 * @param user_message The abbreviated message sent to the user
	 * @param full_message The full message sent to the log
	 * @param level
	 */
	void createMessage(
		QString user_message, QString full_message, const MessageLevel level = MessageLevel::ATLAS_INFO );

	inline void createMessage(
		const format_ns::string_view user_message,
		const std::string_view full_message,
		const MessageLevel level = MessageLevel::ATLAS_INFO )
	{
		createMessage(
			QString::fromLocal8Bit( user_message.data(), static_cast< qsizetype >( user_message.size() ) ),
			QString::fromLocal8Bit( full_message.data(), static_cast< qsizetype >( full_message.size() ) ),
			level );
	}

	inline void createUserMessage( const QString str )
	{
		createMessage( str, {} );
	}

	bool isNotificationsReady();

} // namespace atlas::notifications

#endif //ATLASGAMEMANAGER_NOTIFICATIONS_HPP
