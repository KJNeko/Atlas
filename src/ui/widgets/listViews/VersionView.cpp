//
// Created by kj16609 on 3/1/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VersionView.h" resolved

#include "VersionView.hpp"


VersionViewModel::VersionViewModel( QObject* parent ) : QAbstractItemModel( parent ) {}

QModelIndex VersionViewModel::index( int row, int column, [[maybe_unused]] const QModelIndex& parent ) const
{
	return QAbstractItemModel::createIndex( row, column );
}

QModelIndex VersionViewModel::parent( [[maybe_unused]] const QModelIndex& index ) const
{
	return {};
}

int VersionViewModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_versions.size() );
}

int VersionViewModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 5;
}

QVariant VersionViewModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		const auto& v_data { m_versions.at(
			static_cast< std::remove_reference_t< decltype( m_versions ) >::size_type >( index.row() ) ) };

		switch ( index.column() )
		{
			case 0:
				return v_data.m_version;
			case 1:
				return QString::fromStdString( v_data.m_game_path.string() );
			case 2:
				return QString::fromStdString( v_data.m_exec_path.string() );
			case 3:
				[[fallthrough]];
				;
			case 4:
				return QString( "Not implemented" );
			default:
				return QString( "I fucked up" );
		}
	}
	else
		return {};
}

QVariant VersionViewModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( orientation == Qt::Horizontal )
		{
			switch ( section )
			{
				case 0:
					return QString( "Version" );
				case 1:
					return QString( "Path" );
				case 2:
					return QString( "Executable" );
				case 3:
					return QString( "Last played" );
				case 4:
					return QString( "Total playtime" );
				default:
					return {};
			}
		}
		else { return QString::number( section + 1 ); }
	}

	return {};
}

void VersionViewModel::setVersions( std::vector< GameMetadata > metadata )
{
	beginResetModel();
	m_versions = std::move( metadata );
	endResetModel();
}

void VersionViewModel::addVersion( GameMetadata metadata )
{
	beginInsertRows( {}, rowCount(), rowCount() );
	m_versions.emplace_back( std::move( metadata ) );
	endInsertRows();
}

void VersionViewModel::removeVersion( const GameMetadata& metadata )
{
	//Find index
	const auto index { std::find( m_versions.begin(), m_versions.end(), metadata ) };

	if ( index == m_versions.end() )
		return;
	else
	{
		const auto pos { static_cast< int >( index - m_versions.begin() ) };
		beginRemoveRows( {}, pos, pos );

		m_versions.erase( index );

		endRemoveRows();
	}
}

std::vector< GameMetadata > VersionViewModel::versions() const
{
	return m_versions;
}


VersionView::VersionView( QWidget* parent ) : QListView( parent )
{
	QListView::setModel( new VersionViewModel() );
}

VersionView::~VersionView() {}


void VersionView::setVersions(std::vector<GameMetadata> metadata)
{
	dynamic_cast<VersionViewModel*>(model())->setVersions(std::move(metadata));
}

void VersionView::addVersion(GameMetadata metadata)
{
	dynamic_cast<VersionViewModel*>(model())->addVersion(std::move(metadata));
}

void VersionView::removeVersion(const GameMetadata& metadata)
{
	dynamic_cast<VersionViewModel*>(model())->removeVersion(metadata);
}

std::vector< GameMetadata > VersionView::versions() const
{
	return dynamic_cast<VersionViewModel*>(model())->versions();
}
