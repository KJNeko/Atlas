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

	const auto game { m_records.at( static_cast< std::size_t >( index.row() ) ) };

	switch ( role )
	{
		case Qt::DisplayRole:
			[[fallthrough]];
		case RecordListModelRoles::Raw:
			return QVariant::fromStdVariant( std::variant< atlas::records::Game >( game ) );
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
	loaders.erase( index.row() );
	emit dataChanged( index, index );
}

void RecordListModel::refreshOnFuture( QPersistentModelIndex index, QFuture< QPixmap > future )
{
	//Upon future finishing we signal `dataChanged` in order to get Qt to repaint the specific item. Repaint is luckly not called if it's off screen. Meaning we don't 'waste' paints by responding to off-screen futures finally finishing
	if ( future.isFinished() )
	{
		// Optimistic checking
		emit dataChanged( index, index );
		return;
	}

	//In some case the future might not be valid anymore. Unsure why this happens. But we shouldn't access it if it is invalid
	if ( loaders.contains( index.row() ) )
	{
		future.cancel();
		return;
	}

	auto* loader { new atlas::images::ImageLoader( index, future ) };
	connect(
		loader,
		&atlas::images::ImageLoader::imageReady,
		this,
		&RecordListModel::reloadRecord,
		Qt::SingleShotConnection );
	loader->moveToThread( &loading_thread );
	loaders.insert( std::make_pair( index.row(), loader ) );
}

void RecordListModel::killLoaders()
{
	loaders.clear();
}

QVariant RecordListModel::headerData( int i, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole && i == 0 )
	{
		switch ( orientation )
		{
			case Qt::Horizontal:
				return QVariant( "Horizontal" ); // no column header, it's already in the tree
			case Qt::Vertical:
				[[fallthrough]];
			default:
				return QVariant( "Vertical" );
		}
	}
	else
	{
		return QVariant( "Error" );
	}
}