//
// Created by kj16609 on 1/31/23.
//

#ifndef HYDRUS95_TDELEGATE_HPP
#define HYDRUS95_TDELEGATE_HPP

#include <QAbstractItemDelegate>
#include <queue>
#include <QPainter>
#include <semaphore>
#include <thread>

template< typename T > using PaintFunc = void( QPainter*, const QStyleOptionViewItem&, const QModelIndex&, const T& );

template< typename T > using DefferLoader = void( std::shared_ptr< T > t_ptr );


template< typename T, const DefferLoader loader > class TListModel : public QAbstractListModel
{
	std::vector< std::shared_ptr< T > > vector {};


	using Idx = std::vector< T >::size_type;

	TListModel( QWidget* parent = nullptr ) : QAbstractListModel( parent ) {}

	static constexpr uint8_t LOAD_MAX { 64 };

	std::counting_semaphore< LOAD_MAX > load_sem;
	std::mutex queue_mutex;
	std::queue< std::shared_ptr< T > > load_queue;

	void requestLoad( std::shared_ptr< T > t_ptr )
	{
		std::lock_guard guard( queue_mutex );
		load_queue.push( std::move( t_ptr ) );
		load_sem.release();
		if ( load_queue.size() > load_sem.max() ) load_queue.pop();
	}

	void item_loader( std::stop_token& token )
	{
		while ( !token.stop_requested() )
		{
			using namespace std::chrono_literals;
			if(!load_sem.try_acquire_for(2s))
				continue;

			auto t = [&]()
			{
				std::lock_guard guard( queue_mutex );
				auto t { std::move( load_queue.back() ) };
				load_queue.pop();
				return t;
			};

			loader(t);
		}
	}

	std::jthread loader_thread { &TListModel::item_loader, this };

	~TListModel()
	{
		loader_thread.request_stop();
		loader_thread.join();
	}
};


template< typename T, DefferLoader< T > loader, PaintFunc< T > painter_func > class TDelegate :
  public QAbstractItemDelegate
{
	public:
	TListModel< T >& model_ref;

	TDelegate() = delete;

	TDelegate( TListModel< T >& model, QWidget* parent = nullptr ) : model_ref( model ), QAbstractItemDelegate( parent )
	{
	}

	void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override
	{
		painter->save();

		painter->drawRect( option.rect );

		if ( index.data().isNull() )
		{
			painter->restore();
			return;
		}

		auto data { index.data().value< std::shared_ptr< T > >() };

		std::optional< typename T::DataStore > data_store { data.data_store };

		if ( data_store.has_value() )
			painter_func( painter, option, index, data );
		else
			model_ref.requestLoader( data );
	}
};


#endif	//HYDRUS95_TDELEGATE_HPP
