//
// Created by kj16609 on 6/23/23.
//

#ifndef ATLASGAMEMANAGER_CONSTRAINT_HPP
#define ATLASGAMEMANAGER_CONSTRAINT_HPP

namespace internal
{
	template < typename T >
	concept has_remote_link_to = requires( T t ) {
		{
			t.link_to_rid( RecordID( 0 ), RapidTransaction() )
		} -> std::same_as< void >;
	};

	template < typename T >
	concept has_remote_unlink_to = requires( T t ) {
		{
			t.unlink_to_rid( RecordID( 0 ), RapidTransaction() )
		} -> std::same_as< void >;
	};

	template < typename T >
	concept has_fetch_for = requires() {
		{
			T::fetch_for_rid( RecordID( 0 ), RapidTransaction() )
		} -> std::same_as< std::optional< T > >;
	};

	template < typename T >
	concept has_fetch_for_pkey = requires() {
		{
			T::fetch_for_pkey( T::PType )
		} -> std::same_as< T >;
	};

} // namespace internal

template < typename T >
concept is_remote = internal::has_remote_link_to< T > && internal::has_remote_unlink_to< T >
                 && internal::has_fetch_for< T > && internal::has_fetch_for_pkey< T >;

#endif //ATLASGAMEMANAGER_CONSTRAINT_HPP
