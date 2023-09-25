//
// Created by kj16609 on 4/17/23.
//

#include "FilepathModel.hpp"

#include <moc_FilepathModel.cpp>

#include <QIODevice>
#include <QMimeData>

#include "core/images/loader.hpp"
#include "core/images/thumbnails.hpp"
#include "core/logging/logging.hpp"

void FilepathModel::setFilepaths( const std::vector< std::filesystem::path >& filepaths )
{
	beginResetModel();
	killLoaders();
	this->m_paths = filepaths;
	endResetModel();
}

FilepathModel::FilepathModel( QObject* parent ) : QAbstractListModel( parent )
{
	loading_thread.start();
}

QModelIndex FilepathModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !hasIndex( row, column, parent ) )
		return QModelIndex();
	else
		return createIndex( row, column );
}

QModelIndex FilepathModel::parent( [[maybe_unused]] const QModelIndex& child ) const
{
	return QModelIndex();
}

int FilepathModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_paths.size() );
}

QVariant FilepathModel::data( const QModelIndex& index, int role ) const
{
	const auto& path { m_paths[ static_cast< unsigned long >( index.row() ) ] };

	switch ( role )
	{
		case FilepathModel::PixmapRole:
			{
				const QSize size { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
				return QVariant::fromStdVariant( std::variant< QFuture< QPixmap > >(
					atlas::images::async::loadScaledPixmap( size, SCALE_TYPE::KEEP_ASPECT_RATIO, path ) ) );
				break;
			}
		case FilepathRole:
			[[fallthrough]];
		case Qt::StatusTipRole:
			[[fallthrough]];
		case Qt::DisplayRole:
			return QVariant::fromValue( QString::fromStdString( path.string() ) );
		default:
			return {};
	}
}

Qt::DropActions FilepathModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

bool FilepathModel::removeRows( int row, int count, const QModelIndex& parent )
{
	if ( row < 0 || static_cast< std::size_t >( row + count ) > m_paths.size() ) return false;

	beginRemoveRows( parent, row, row + count - 1 );
	killLoaders();

	for ( int i = 0; i < count; ++i ) m_paths.erase( m_paths.begin() + row );

	endRemoveRows();
	return true;
}

Qt::ItemFlags FilepathModel::flags( const QModelIndex& index ) const
{
	const Qt::ItemFlags defaults = QAbstractListModel::flags( index );
	if ( index.isValid() )
		return Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled | defaults;
	else
		return Qt::ItemIsDropEnabled | defaults;
}

bool FilepathModel::insertRows( int row, int col, const QModelIndex& parent )
{
	if ( row < 0 ) return false;
	if ( row > rowCount( {} ) ) return false;

	beginInsertRows( parent, row, row + col - 1 );
	killLoaders();

	if ( row == rowCount() )
	{
		//Insert to end
		for ( int i = 0; i < col; ++i )
		{
			m_paths.push_back( {} );
		}
	}
	else if ( row == 0 )
	{
		//beg
		for ( int i = 0; i < col; ++i )
		{
			m_paths.insert( m_paths.begin(), {} );
		}
	}
	else
	{
		for ( int i = 0; i < col; ++i )
		{
			m_paths.insert( m_paths.begin() + row, {} );
		}
	}

	endInsertRows();
	return true;
}

bool FilepathModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	if ( role == Qt::DisplayRole || role == Qt::EditRole )
	{
		m_paths.at( static_cast< unsigned long >( index.row() ) ) = value.value< QString >().toStdString();
		return true;
	}

	return false;
}

bool FilepathModel::moveRows(
	const QModelIndex& sourceParent,
	int sourceRow,
	int count,
	const QModelIndex& destinationParent,
	int destinationChild )
{
	const auto s_beg = m_paths.begin() + sourceRow;
	const auto s_end = s_beg + count;

	if ( s_end > m_paths.end() ) return false;

	const std::vector< std::filesystem::path > data { s_beg, s_end };

	beginMoveRows( sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild );
	killLoaders();

	m_paths.erase( s_beg, s_end );

	if ( destinationChild > static_cast< int >( m_paths.size() ) )
		destinationChild = static_cast< int >( m_paths.size() );

	m_paths.insert( m_paths.begin() + destinationChild, data.begin(), data.end() );

	endMoveRows();

	emit reordered();

	return true;
}

std::vector< std::filesystem::path > FilepathModel::getFilepaths() const
{
	return m_paths;
}

void FilepathModel::refreshOnFuture( QPersistentModelIndex index, QFuture< QPixmap > future )
{
	//Upon future finishing we signal `dataChanged` in order to get Qt to repaint the specific item. Repaint is luckly not called if it's off screen. Meaning we don't 'waste' paints by responding to off-screen futures finally finishing
	if ( future.isFinished() )
	{
		// Optimistic checking
		emit dataChanged( index, index );
	}
	else
	{
		//In some case the future might not be valid anymore. Unsure why this happens. But we shouldn't access it if it is invalid
		if ( future.isValid() )
		{
			if ( !loaders.contains( index.row() ) )
			{
				auto* loader { new atlas::images::ImageLoader( index, std::move( future ) ) };
				connect(
					loader,
					&atlas::images::ImageLoader::imageReady,
					this,
					&FilepathModel::reloadRecord,
					Qt::SingleShotConnection );
				loader->moveToThread( &loading_thread );
				loaders.insert( std::make_pair( index.row(), loader ) );
			}
			else
				future.cancel();
			//Image is already loading. So cancel this one
		}
		else
		{
			//Even if the future is valid. It might mean that it completed before we were ready?
			emit dataChanged( index, index );
		}
	}
}

void FilepathModel::reloadRecord( QPersistentModelIndex index )
{
	if ( index.isValid() )
	{
		emit dataChanged( index, index );
	}
	loaders.erase( index.row() );
}

void FilepathModel::killLoaders()
{
	loaders.clear();
}
