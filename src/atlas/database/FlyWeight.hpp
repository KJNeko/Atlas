//
// Created by kj16609 on 3/12/23.
//

#ifndef ATLAS_FLYWEIGHT_HPP
#define ATLAS_FLYWEIGHT_HPP

#include <concepts>
#include <memory>
#include <mutex>
#include <unordered_map>

//! Requires `FlyWeightID<T_Key> key() const` to be defined
template < typename T, typename T_Key >
concept HasObjectKeyFunc = requires( const T t ) {
	{
		t.key()
	} -> std::same_as< T_Key >;
};

//! Requires `static FlyWeightID<T_Key> key(T_Args...)` to be defined
template < typename T, typename T_Key, typename... T_Args >
concept HasStaticKeyFunc = requires( T_Args... args ) {
	{
		T::key( args... )
	} -> std::same_as< T_Key >;
};

template < typename T, typename T_Key, typename... T_Args >
concept HasKeyFunc = HasObjectKeyFunc< T, T_Key > || HasStaticKeyFunc< T, T_Key, T_Args... >;

/**
 * @brief Allows for objects of the same 'key' to be in a shared memory space.
 * @tparam T Object to store in the flyweight
 * @tparam T_Key Key to use as the index.
 * @note If you have to debug this. I'm sorry.
 */
template < typename T, typename T_Key >
	requires HasObjectKeyFunc< T, T_Key >
class FlyWeight : public std::shared_ptr< T >
{
	inline static std::unordered_map< T_Key, std::shared_ptr< T > > map;
	inline static std::mutex map_mtx;

	//! Returns the ptr for the given key. Returns nullptr if not found
	inline static std::shared_ptr< T > getPtr( const T_Key key )
	{
		std::lock_guard guard { map_mtx };
		if ( auto itter = map.find( key ); itter != map.end() )
			return itter->second;
		else
			return nullptr;
	}

	//! Inserts the pointer into the map and returns the ptr
	inline static std::shared_ptr< T > insertPtr( const T_Key key, std::shared_ptr< T > ptr )
	{
		std::lock_guard guard { map_mtx };

		//Check
		if ( map.contains( key ) )
			return map.find( key )->second;
		else
			map.insert( { key, ptr } );

		return ptr;
	}

	//! Returns a shared_ptr to a new instance of the object or an existing one in map
	/**
	 * @note Class is required to have the ctor be PUBLIC as well as either having `static FlyWeight<T_Key> key(T_Args)` defined OR `T_Key key() const` defined
	 * @tparam T_Args Arguments to pass to ctor
	 * @param args Arguments for ctor
	 * @return
	 */
	template < typename... T_Args >
		requires HasKeyFunc< T, T_Key, T_Args... > && std::constructible_from< T, T_Args... >
	inline static std::shared_ptr< T > determinePtr( T_Args&&... args )
	{
		if constexpr ( HasStaticKeyFunc< T, T_Key, T_Args... > )
		{
			const T_Key key { T::key( std::forward< T_Args >( args )... ) };

			auto ptr { getPtr( key ) };
			if ( ptr == nullptr )
				return insertPtr( key, std::shared_ptr< T >( new T( std::forward< T_Args >( args )... ) ) );
			else
				return ptr;
		}
		else
		{
			std::shared_ptr< T > obj { new T( std::forward< T_Args >( args )... ) };
			return insertPtr( obj->key(), obj );
		}
	}

	//! Returns the key from the inherited pointer
	inline T_Key key() const { return std::shared_ptr< T >::get()->key(); }

  public:

	FlyWeight() : std::shared_ptr< T >( nullptr ) {}

	//! Passthrough for ctor to T
	/**
	 * @tparam T_Args Argument list. Must follow constraint std::constructible_from. Where T is constructible from T_Args
	 * @param args Args to construct with
	 */
	template < typename... T_Args >
		requires HasKeyFunc< T, T_Key, T_Args... > && std::constructible_from< T, T_Args... >
	FlyWeight( T_Args&&... args ) : std::shared_ptr< T >( determinePtr( std::forward< T_Args >( args )... ) )
	{}

	FlyWeight( const FlyWeight& other ) = default;
	FlyWeight( FlyWeight&& other ) = default;
	FlyWeight& operator=( const FlyWeight& other ) = default;

	~FlyWeight()
	{
		std::lock_guard guard( map_mtx );
		if ( this->use_count() == 2 )
			if ( auto itter = map.find( key() ); itter != map.end() ) map.erase( itter );
	}

	//! Returns the size of this flyweight's map
	inline static std::size_t flyweightSize() { return map.size(); }
};

#endif //ATLAS_FLYWEIGHT_HPP
