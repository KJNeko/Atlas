//
// Created by kj16609 on 1/31/23.
//

#ifndef HYDRUS95_TDELEGATE_HPP
#define HYDRUS95_TDELEGATE_HPP

#include <QAbstractItemDelegate>
#include <queue>

template< typename T > class TListModel : public QAbstractListModel
{
	std::vector< T > vector;

	using Idx = std::vector< T >::size_type;

	std::queue<Idx> loader_queue;

	TListModel( QWidget* parent = nullptr ) : QAbstractListModel( parent ) {}

	void insert( const Idx index, T value );
	void remove( const Idx index );
};

template<typename T>
using PaintFunc = void(QPainter*, const QStyleOptionViewItem&, const QModelIndex&, const T&);

template<typename T>
using DefferLoader = void(T& t, TListModel<T>& model_ref);


template< typename T, DefferLoader<T> loader, PaintFunc<T> painter_func>
class TDelegate : public QAbstractItemDelegate
{
	TListModel< T >& model_ref;

	TDelegate() = delete;

	public:
	TDelegate( TListModel< T >& model, QWidget* parent = nullptr ) : model_ref( model ), QAbstractItemDelegate( parent )
	{
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		T data {index.data().value<T>()};

		std::optional<typename T::DataStore> data_store {data.data_store};

		if(data_store.has_value())
			painter_func(painter, option, index, data);
		else
			loader(data, model_ref);
	}


};


#endif	//HYDRUS95_TDELEGATE_HPP
