//
// Created by kj16609 on 7/21/23.
//

#include "notifications.hpp"

#include <QPromise>

#include "core/logging/logging.hpp"
#include "core/utils/mainThread/mainThread.hpp"

namespace atlas::notifications
{
	namespace internal
	{
		inline static NotificationManagerUI* notification_manager { nullptr };
	}

	void initNotifications( QWidget* parent )
	{
		internal::notification_manager = new NotificationManagerUI( parent );
		internal::notification_manager->show();
	}

	NotificationManagerUI& handle()
	{
		if ( internal::notification_manager == nullptr )
			throw std::runtime_error( "Init notification handler before accessing handle" );
		return *internal::notification_manager;
	}

	void createMessage( QString message, const MessageLevel level )
	{
		if ( internal::notification_manager == nullptr )
			throw std::runtime_error( "Notification manage not initalized before notification!" );
		utils::executeOnMain(
			[ &message, level ]()
			{
				auto* ptr { new MessageNotification( level, std::move( message ), internal::notification_manager ) };
				ptr->show();
				internal::notification_manager->addNotification( ptr );
			} );
	}

	bool isNotificationsReady()
	{
		return internal::notification_manager != nullptr;
	}

} // namespace atlas::notifications