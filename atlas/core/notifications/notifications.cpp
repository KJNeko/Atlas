//
// Created by kj16609 on 7/21/23.
//

#include "notifications.hpp"

#include <QPromise>

#include "core/exceptions.hpp"
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
		if ( internal::notification_manager )
			internal::notification_manager->show();
		else
			throw AtlasException( "Failed to init notification manager" );
	}

	NotificationManagerUI& handle()
	{
		if ( internal::notification_manager == nullptr )
			throw AtlasException( "Init notification handler before accessing handle" );
		return *internal::notification_manager;
	}

	void createMessage( QString user_message, QString full_message, const MessageLevel level )
	{
		if ( internal::notification_manager == nullptr )
			throw AtlasException( "Notification manage not initalized before notification!" );
		utils::executeOnMain(
			[ user_message = std::move( user_message ), level, full_message = std::move( full_message ) ]() -> void
			{
				switch ( level )
				{
					default:
						[[fallthrough]];
					case MessageLevel::ATLAS_DEBUG:
						[[fallthrough]];
					case MessageLevel::ATLAS_INFO_SELFCLOSE:
						[[fallthrough]];
					case MessageLevel::ATLAS_INFO:
						{
							auto* ptr { new MessageNotification(
								level,
								std::move( user_message ),
								std::move( full_message ),
								internal::notification_manager ) };
							ptr->show();
							notifications::handle().addNotification( ptr );
							return;
						}
					case MessageLevel::ATLAS_WARNING:
						[[fallthrough]];
					case MessageLevel::ATLAS_ERROR:
						[[fallthrough]];
					case MessageLevel::ATLAS_CRITICAL:
						{
							auto* ptr { new DevNotification(
								level,
								std::move( user_message ),
								std::move( full_message ),
								internal::notification_manager ) };
							ptr->show();
							notifications::handle().addNotification( ptr );
							return;
						}
				}
			} );
	}

	bool isNotificationsReady()
	{
		return internal::notification_manager != nullptr;
	}

} // namespace atlas::notifications