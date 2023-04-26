//
// Created by kj16609 on 4/26/23.
//

#include "ConfigNotification.hpp"

#include "config.hpp"

namespace config
{
	namespace internal
	{
		inline static ConfigNotification notif;

		ConfigNotification& getNotifier()
		{
			return notif;
		}
	} // namespace internal

	void notify()
	{
		internal::notif.notify();
	}

	ConfigNotification::ConfigNotification( QObject* parent ) : QObject( parent )
	{
		spdlog::info( "ConfigNotification constructor" );
	}

	void ConfigNotification::notify()
	{
		spdlog::info( "Notifying of config change" );
		emit notification();
	}

} // namespace config
