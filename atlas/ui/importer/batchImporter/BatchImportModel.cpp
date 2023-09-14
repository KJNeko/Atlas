//
// Created by kj16609 on 3/26/23.
//

#include "BatchImportModel.hpp"

#include <QLineEdit>
#include <QLocale>

#include "core/database/record/Game.hpp"
#include "core/logging.hpp"

int BatchImportModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 7;
}

int BatchImportModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_data.size() );
}

QVariant BatchImportModel::data( const QModelIndex& index, int role ) const
{
	const auto& item { m_data.at( static_cast< std::size_t >( index.row() ) ) };

	switch ( role )
	{
		case Qt::DisplayRole:
			{
				switch ( index.column() )
				{
					case FOLDER_PATH:
						return QString::fromStdString( item.relative_path.string() );
					case TITLE:
						return item.title;
					case CREATOR:
						return item.creator;
					case ENGINE:
						return item.engine;
					case VERSION:
						return item.version;
					case SIZE:
						{
							const QLocale locale { QLocale::system() };
							return locale.formattedDataSize( static_cast< qint64 >( item.size ) );
						}
					case EXECUTABLE:
						return QString::fromStdString( item.executable.string() );
					default:
						return QString( "wtf?" );
				}
			}
		case ExecutablesEditRole:
			{
				return QVariant::fromStdVariant( std::variant<
												 std::vector< std::filesystem::path > >( item.executables ) );
			}
		case HasGLLinkRole:
			{
				return item.atlas_id != INVALID_ATLAS_ID;
			}
		case ExistingGameRole:
			{
				return item.game_id != INVALID_RECORD_ID;
			}
		case ConflictingVersionRole:
			{
				return item.conflicting_version;
			}
		default:
			return {};
	}
}

void BatchImportModel::addGame( GameImportData data )
{
	beginInsertRows( {}, static_cast< int >( m_data.size() ), static_cast< int >( m_data.size() ) );
	m_data.push_back( std::move( data ) );
	endInsertRows();
}

void BatchImportModel::addGames( std::vector< GameImportData > data )
{
	beginInsertRows( {}, static_cast< int >( m_data.size() ), static_cast< int >( m_data.size() + data.size() - 1 ) );
	for ( auto& item : data ) m_data.emplace_back( std::move( item ) );
	endInsertRows();
}

QVariant BatchImportModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		switch ( section )
		{
			case FOLDER_PATH:
				return QString( "Folder" );
			case TITLE:
				return QString( "Title" );
			case CREATOR:
				return QString( "Creator" );
			case ENGINE:
				return QString( "Engine" );
			case VERSION:
				return QString( "Version" );
			case SIZE:
				return QString( "Size" );
			case EXECUTABLE:
				return QString( "Executable" );
			default:
				return QString( "Oh fuck?" );
		}
	}
	else
		return QAbstractItemModel::headerData( section, orientation, role );
}

Qt::ItemFlags BatchImportModel::flags( const QModelIndex& index ) const
{
	switch ( index.column() )
	{
		case TITLE:
			[[fallthrough]];
		case CREATOR:
			[[fallthrough]];
		case VERSION:
			[[fallthrough]];
		case ENGINE:
			[[fallthrough]];
		case EXECUTABLE:
			return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		case SIZE:
			[[fallthrough]];
		case FOLDER_PATH:
			[[fallthrough]];
		default:
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
	return QAbstractItemModel::flags( index );
}

bool BatchImportModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	switch ( index.column() )
	{
		case EXECUTABLE:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).executable =
					value.value< QString >().toStdString();
				emit dataChanged( index, index );
				return true;
			}
		case TITLE:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).title = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}
		case CREATOR:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).creator = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}
		case ENGINE:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).engine = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}
		case VERSION:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).version = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}
		default:
			return QAbstractItemModel::setData( index, value, role );
	}
}

void BatchImportModel::clearData()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}

void BatchImportModel::sort( int idx, Qt::SortOrder order )
{
	beginResetModel();

	switch ( static_cast< ImportColumns >( idx ) )
	{
		default:
			[[fallthrough]];
		case TITLE:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right ) {
						return order == Qt::SortOrder::AscendingOrder ? left.title < right.title :
					                                                    left.title > right.title;
					} );
			}
			break;
		case CREATOR:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right ) {
						return order == Qt::SortOrder::AscendingOrder ? left.creator < right.creator :
					                                                    left.creator > right.creator;
					} );
			}
			break;
		case ENGINE:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right ) {
						return order == Qt::SortOrder::AscendingOrder ? left.engine < right.engine :
					                                                    left.engine > right.engine;
					} );
			}
			break;
		case VERSION:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right ) {
						return order == Qt::SortOrder::AscendingOrder ? left.version < right.version :
					                                                    left.version > right.version;
					} );
			}
			break;
		case EXECUTABLE:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right )
					{
						return order == Qt::SortOrder::AscendingOrder ? left.executable < right.executable :
					                                                    left.executable > right.executable;
					} );
			}
			break;
		case SIZE:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right ) {
						return order == Qt::SortOrder::AscendingOrder ? left.size < right.size : left.size > right.size;
					} );
			}
			break;
		case FOLDER_PATH:
			{
				std::sort(
					m_data.begin(),
					m_data.end(),
					[ order ]( const GameImportData& left, const GameImportData& right )
					{
						return order == Qt::SortOrder::AscendingOrder ? left.relative_path < right.relative_path :
					                                                    left.relative_path > right.relative_path;
					} );
			}
	}
	endResetModel();
}