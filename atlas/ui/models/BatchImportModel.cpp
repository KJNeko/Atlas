//
// Created by kj16609 on 3/26/23.
//

#include "BatchImportModel.hpp"

#include <QLineEdit>
#include <QLocale>

#include "core/database/RecordData.hpp"
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
						return QString::fromStdString( item.path.string() );
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
							QLocale locale { QLocale::system() };
							return locale.formattedDataSize( static_cast< qint64 >( item.size ) );
						}
					case EXECUTABLES:
						return QString::fromStdString( item.executable.string() );
					default:
						return QString( "wtf?" );
				}
			}
		case Qt::EditRole:
			{
				if ( index.column() == EXECUTABLES )
					return QVariant::fromStdVariant( std::variant<
													 std::vector< std::filesystem::path > >( item.executables ) );
				else
					return {};
			}
		case Qt::BackgroundRole:
			{
				if ( recordExists( item.title, item.creator, item.engine ) )
					return QColor( 255, 0, 0 );
				else
					return {};
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
			case EXECUTABLES:
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
		case EXECUTABLES:
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
		case EXECUTABLES:
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
