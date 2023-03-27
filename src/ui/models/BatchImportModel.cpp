//
// Created by kj16609 on 3/26/23.
//

#include "BatchImportModel.hpp"

#include <QLineEdit>

#include <tracy/Tracy.hpp>

#include "h95/logging.hpp"

int BatchImportModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 7;
}

int BatchImportModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_data.size() );
}

QString temp( const QString temp )
{
	return "  " + temp;
}

QVariant BatchImportModel::data( const QModelIndex& index, int role ) const
{
	ZoneScoped;
	const auto& item { m_data.at( static_cast< std::size_t >( index.row() ) ) };
	if ( role == Qt::DisplayRole )
	{
		switch ( index.column() )
		{
			case FOLDER_PATH:
				return temp( QString::fromStdString( item.path.string() ) );
			case TITLE:
				return temp( item.title );
			case CREATOR:
				return temp( item.creator );
			case VERSION:
				return temp( item.version );
			case SIZE:
				{
					QLocale locale { QLocale::system() };
					return temp( locale.formattedDataSize( static_cast< qint64 >( item.size ) ) );
				}
			case EXECUTABLES:
				return temp( QString::fromStdString( item.executable.string() ) );
			case MOVE_FLAG:
				return temp( "To Move?" );
			default:
				return QString( "wtf?" );
		}
	}
	else if ( role == Qt::CheckStateRole && index.column() == MOVE_FLAG )
		return static_cast< int >( item.move_after_import ? Qt::Checked : Qt::Unchecked );
	else if ( role == Qt::EditRole && index.column() == EXECUTABLES )
		return QVariant::fromStdVariant( std::variant< std::vector< std::filesystem::path > >( item.executables ) );
	else
		return {};
}

void BatchImportModel::addGame( GameImportData data )
{
	ZoneScoped;
	beginInsertRows( {}, static_cast< int >( m_data.size() ), static_cast< int >( m_data.size() ) );
	m_data.push_back( std::move( data ) );
	endInsertRows();
}

QVariant BatchImportModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	ZoneScoped;
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
			case VERSION:
				return QString( "Version" );
			case SIZE:
				return QString( "Size" );
			case EXECUTABLES:
				return QString( "Executable" );
			case MOVE_FLAG:
				return QString( "Should Move" );
			default:
				return QString( "Oh fuck?" );
		}
	}
	else
		return QAbstractItemModel::headerData( section, orientation, role );
}

Qt::ItemFlags BatchImportModel::flags( const QModelIndex& index ) const
{
	ZoneScoped;
	switch ( index.column() )
	{
		case TITLE:
			[[fallthrough]];
		case CREATOR:
			[[fallthrough]];
		case VERSION:
			[[fallthrough]];
		case EXECUTABLES:
			[[fallthrough]];
		case MOVE_FLAG:
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
	ZoneScoped;
	spdlog::info( "Data set" );

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
