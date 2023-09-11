//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONS_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONS_HPP

#include <QJsonArray>
#include <QString>

#include "core/logging.hpp"
#include "core/logging/dev.hpp"
#include "ui/notifications/DevNotification.hpp"
#include "ui/notifications/MessageNotification.hpp"
#include "ui/notifications/NotificationManagerUI.hpp"
#include "ui/notifications/NotificationSignalers.hpp"
#include "ui/notifications/ProgressNotification.hpp"

namespace atlas::notifications
{
	void initNotifications( QWidget* parent );
	NotificationManagerUI& handle();

	void createMessage( QString message );

	template < std::uint64_t idx, typename T, typename... Ts >
	void serializeInto( QJsonArray& array, const T& t, const Ts&... ts )
	{
		array.append( atlas::logging::dev::internalSerializer< T >( t ) );
		if constexpr ( sizeof...( Ts ) > 0 ) serializeInto< idx + 1, Ts... >( array, ts... );
	}

	namespace internal
	{
		void createDevMessage( std::string body, QJsonDocument doc );
	}

	template < typename... Ts >
	void createDevMessage( std::string message_body, const Ts&... objs )
	{
		//Serialize objs into a single json object
		using namespace atlas::logging::dev;

		QJsonDocument doc;
		QJsonArray array;
		serializeInto< 0, Ts... >( array, objs... );
		doc.setArray( std::move( array ) );
		internal::createDevMessage( std::move( message_body ), std::move( doc ) );
	}

} // namespace atlas::notifications

#endif //ATLASGAMEMANAGER_NOTIFICATIONS_HPP
