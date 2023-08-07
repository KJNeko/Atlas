//
// Created by kj16609 on 5/24/23.
//

#include "SIModel.hpp"

#include <moc_SIModel.cpp>

#include <QColor>
#include <QDir>

#include <stack>

std::uint64_t hash( const int first, const int second )
{
	return ( static_cast< std::uint64_t >( first ) << 32 ) | static_cast< std::uint64_t >( second );
}

SIModel::SIModel( QObject* parent ) : QAbstractItemModel( parent )
{}

void SIModel::setRoot( const QString path )
{
	beginResetModel();

	m_root = new Node( path, nullptr, true );

	endResetModel();
}

QModelIndex SIModel::index( int row, int column, const QModelIndex& parent_idx ) const
{
	if ( !hasIndex( row, column, parent_idx ) ) return QModelIndex();

	if ( !parent_idx.isValid() )
	{
		const Node* child { m_root->child( row ) };

		if ( child )
			return createIndex( row, column, child );
		else
			return QModelIndex();
	}
	else
	{
		const Node* parent { static_cast< Node* >( parent_idx.internalPointer() ) };
		const Node* child { parent->child( row ) };
		if ( child )
			return createIndex( row, column, child );
		else
			return QModelIndex();
	}
}

QModelIndex SIModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() ) return QModelIndex();

	const Node* child { static_cast< Node* >( index.internalPointer() ) };
	const Node* parent { child->parent() };

	if ( parent == m_root ) return QModelIndex();

	return createIndex( parent->row(), 0, parent );
}

int SIModel::rowCount( const QModelIndex& index ) const
{
	if ( index.column() > 0 ) return 0;

	if ( !index.isValid() )
		return m_root->childCount();
	else
	{
		Node* ptr { static_cast< Node* >( index.internalPointer() ) };
		if ( !ptr->scanned() ) ptr->scan();
		return ptr->childCount();
	}
}

int SIModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 1;
}

QVariant SIModel::data( const QModelIndex& index, int role ) const
{
	Node* node { static_cast< Node* >( index.internalPointer() ) };

	switch ( role )
	{
		case Qt::DisplayRole:
			{
				const auto str { node->m_path };
				const auto itter { str.lastIndexOf( QDir::separator() ) };
				return str.mid( itter + 1 );
			}
		default:
			return QVariant();
	}
}

SIModel::~SIModel()
{
	delete m_root;
}

Node::Node( const QString str, Node* parent, const bool scan_immediate ) : m_path( str ), m_parent( parent )
{
	if ( scan_immediate ) scan();

	QFileInfo info { str };
	if ( info.isDir() )
		m_info = DirInfo();
	else
		m_info = FileInfo();
}
