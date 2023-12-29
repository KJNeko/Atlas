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
		case Roles::Raw:
			return QVariant::fromStdVariant( std::variant< atlas::records::Game >( game ) );
		case Qt::ToolTipRole:
			[[fallthrough]];
		case Qt::StatusTipRole:
			{
				return QString( "Game: %1" ).arg( m_records.at( static_cast< std::size_t >( index.row() ) )->m_title );
			}
		case Roles::Banner:
			{
				//See if we have a loader cached for this
				if ( auto itter = loaders.find( index.row() ); itter != loaders.end() )
				{
					if ( itter->second->isFinished() )
					{
						return itter->second->future().result();
					}
					else
						return {};
				}
				else
					return {};
			}
		default:
			return { "UNHANDLED ROLE IN RecordListModel::data" };
	}
}

void RecordListModel::reloadRecord( QPersistentModelIndex index )
{
	emit dataChanged( index, index );
}

void RecordListModel::
	refreshOnLoader( QPersistentModelIndex index, std::unique_ptr< atlas::images::ImageLoader > loader )
{
	if ( loader->isFinished() ) emit dataChanged( index, index );

	auto lamda = [ this, index ]() mutable { this->reloadRecord( index ); };

	QObject::connect( loader.get(), &atlas::images::ImageLoader::finished, lamda );

	if ( auto itter = loaders.find( index.row() ); itter != loaders.end() )
	{
		if ( !itter->second->isFinished() )
		{
			//Cancel the new loader
			loader->cancel();
		}
		else
		{
			//Replace the old loader
			itter->second = std::move( loader );
		}
	}
	else
	{
		//Add the new loader
		loaders.emplace( index.row(), std::move( loader ) );
	}
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

void RecordListModel::killOutOfView( const int first, const int last )
{
	//Kill loaders that are out of view
	for ( auto itter = loaders.begin(); itter != loaders.end(); )
	{
		if ( itter->first < first || itter->first > last )
		{
			itter->second->cancel();
			itter = loaders.erase( itter );
		}
		else
			++itter;
	}
}
