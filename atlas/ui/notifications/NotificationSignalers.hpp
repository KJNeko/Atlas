//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONSIGNALERS_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONSIGNALERS_HPP

#include <concepts>
#include <type_traits>

namespace atlas::notifications
{
	class NotificationSignaler : public QObject
	{
		Q_DISABLE_COPY_MOVE( NotificationSignaler )

	  public:

		NotificationSignaler() = default;
	};

	template < typename T >
	concept has_get_signaler = requires( T t ) {
		{
			t.getSignaler()
		} -> std::same_as< std::unique_ptr< typename T::Signaler > >;
	};

	template < typename T >
	concept has_valid_signaler = std::is_base_of_v< NotificationSignaler, typename T::Signaler >;

	template < typename T >
		requires has_get_signaler< T > && has_valid_signaler< T >
	using Signaler = std::unique_ptr< typename T::Signaler >;

} // namespace atlas::notifications

#endif //ATLASGAMEMANAGER_NOTIFICATIONSIGNALERS_HPP
