//
// Created by kj16609 on 5/24/23.
//

#include "SIModel.hpp"

#include <QDir>

#include "core/logging.hpp"

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

	Node* parent { nullptr };

	if ( !parent_idx.isValid() )
		parent = m_root;
	else
		parent = static_cast< Node* >( parent_idx.internalPointer() );

	Node* child { parent->child( row ) };
	if ( child ) return createIndex( row, column, child );

	return QModelIndex();
}

QModelIndex SIModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() ) return QModelIndex();

	Node* child { static_cast< Node* >( index.internalPointer() ) };
	Node* parent { child->parent() };
	if ( parent && parent != m_root )
		return createIndex( parent->row(), 0, parent );
	else
		return QModelIndex();
}

int SIModel::rowCount( const QModelIndex& index ) const
{
	if ( !index.isValid() )
		return m_root->childCount();
	else
		return static_cast< Node* >( index.internalPointer() )->childCount();
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
}