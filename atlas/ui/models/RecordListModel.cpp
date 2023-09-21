//
// Created by kj16609 on 3/15/23.
//

#include "RecordListModel.hpp"

#include <moc_RecordListModel.cpp>

#include <QFutureWatcher>
#include <QTimer>
#include <QtConcurrent>

#include "core/database/record/GameData.hpp"

void RecordListModel::setRecords( std::vector< atlas::records::Game > records )
{
	beginResetModel();
	m_records = std::move( records );
	endResetModel();
	emit recordsChanged( m_records );
}

void RecordListModel::addRecord( atlas::records::Game record, const std::size_t place_at )
{
	const int pos { static_cast< int >( std::min( place_at, m_records.size() ) ) };
	beginInsertRows( {}, pos, pos );
	//connect( &record, &Game::dataChanged, this, &RecordListModel::reloadRecord );
	m_records.insert( m_records.begin() + static_cast< int >( pos ), record );
	endInsertRows();
	emit recordsChanged( m_records );
}

void RecordListModel::removeRecord( QPersistentModelIndex index )
{
	if ( !index.isValid() ) throw AtlasException( "Index is not valid" );

	beginRemoveRows( {}, index.row(), index.row() );
	auto itter { m_records.begin() + index.row() };
	m_records.erase( itter );
	endRemoveRows();
	emit recordsChanged( m_records );
}

int RecordListModel::rowCount( [[maybe_unused]] const QModelIndex& index ) const
{
	return static_cast< int >( m_records.size() );
}

QVariant RecordListModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() ) return {};

	switch ( role )
	{
		case Qt::DisplayRole:
			[[fallthrough]];
		case RecordListModelRoles::Raw:
			return QVariant::fromStdVariant( std::variant<
											 atlas::records::Game >( m_records.at( static_cast<
																				   std::size_t >( index.row() ) ) ) );
		case Qt::ToolTipRole:
			[[fallthrough]];
		case Qt::StatusTipRole:
			{
				return QString( "Game: %1" ).arg( m_records.at( static_cast< std::size_t >( index.row() ) )->m_title );
			}
		default:
			return { "UNHANDLED ROLE IN RecordListModel::data" };
	}
}

void RecordListModel::reloadRecord( QPersistentModelIndex index )
{
	if ( index.isValid() )
	{
		emit dataChanged( index, index );
	}
}

void RecordListModel::refreshOnFuture( QPersistentModelIndex index, QFuture< QPixmap > future )
{
	//Upon future finishing we signal `dataChanged` in order to get Qt to repaint the specific item. Repaint is luckly not called if it's off screen. Meaning we don't 'waste' paints by responding to off-screen futures finally finishing
	if ( future.isFinished() )
	{
		// Optimistic checking
		emit dataChanged( index, index );
	}
	else
	{
		if ( future.isValid() )
		{
			if ( !loaders.contains( index.row() ) )
			{
				auto* loader { new ImageLoader( index, std::move( future ) ) };
				connect( loader, &ImageLoader::imageReady, this, &RecordListModel::reloadRecord );
				loader->moveToThread( &loading_thread );
				loaders.insert( std::make_pair( index.row(), loader ) );
			}
			else
				future.cancel();
			//Image is already loading. So cancel this one
		}
		else
		{
			atlas::logging::warn( "Future is invalid" );
		}
	}

	/*
		QFutureWatcher< QPixmap >* watcher { new QFutureWatcher< QPixmap >() };
		QFuture< QPixmap >* future_ptr { new QFuture< QPixmap >( future ) };
		watcher->setFuture( *future_ptr );

		connect( watcher, &QFutureWatcher< QPixmap >::finished, watcher, &QFutureWatcher< QPixmap >::deleteLater );
		connect( watcher, &QFutureWatcher< QPixmap >::finished, [ future_ptr ]() { delete future_ptr; } );
		connect(
			watcher,
			&QFutureWatcher< QPixmap >::finished,
			this,
			[ this, index ]() { emit dataChanged( index, index ); } );
		 */
}

void RecordListModel::killLoaders()
{
	for ( const auto& loader : loaders ) delete loader.second;

	loaders.clear();
}

ImageLoader::ImageLoader( QPersistentModelIndex index, QFuture< QPixmap > future ) :
  m_index( index ),
  m_future( std::move( future ) )
{
	connect( &watcher, &decltype( watcher )::finished, this, &ImageLoader::triggerReady );
	watcher.setFuture( m_future );
}

void ImageLoader::triggerReady()
{
	emit imageReady( m_index );
}

QVariant RecordListModel::headerData( int i, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole && i == 0 )
	{
		switch ( orientation )
		{
			case Qt::Horizontal:
				return QVariant( "Horizontal" ); // no column header, it's already in the tree
			default:
				return QVariant( "Verticle" );
		}
	}
	else
	{
		return QVariant( "Error" );
	}
}
