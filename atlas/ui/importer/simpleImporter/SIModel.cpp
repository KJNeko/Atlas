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
		if ( m_root == nullptr )
		{
			atlas::logging::error( "m_root is nullptr" );
			return QModelIndex();
		}

		const Node* child { m_root->child( row ) };

		if ( child )
			return createIndex( row, column, child );
		else
			return QModelIndex();
	}
	else
	{
		const Node* parent { static_cast< Node* >( parent_idx.internalPointer() ) };
		if ( parent == nullptr )
		{
			atlas::logging::error( "parent is nullptr" );
			return QModelIndex();
		}

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

	if ( child == nullptr )
	{
		atlas::logging::error( "child parent is nullptr" );
		return QModelIndex();
	}

	const Node* parent { child->parent() };

	if ( parent == nullptr )
	{
		atlas::logging::error( "parent is nullptr" );
		return QModelIndex();
	}

	if ( parent == m_root ) return QModelIndex();

	return createIndex( parent->row(), 0, parent );
}

int SIModel::rowCount( const QModelIndex& index ) const
{
	if ( index.column() > 0 ) return 0;

	if ( !index.isValid() )
	{
		if ( m_root == nullptr )
		{
			atlas::logging::error( "m_root is nullptr" );
			return 0;
		}
		else
			return m_root->childCount();
	}
	else
	{
		Node* ptr { static_cast< Node* >( index.internalPointer() ) };
		if ( ptr == nullptr )
		{
			atlas::logging::error( "ptr is nullptr" );
			return 0;
		}
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
	switch ( role )
	{
		case Qt::DisplayRole:
			{
				const Node* const node { static_cast< Node* >( index.internalPointer() ) };

				if ( node == nullptr )
				{
					atlas::logging::error( "node is nullptr" );
					return {};
				}

				return node->name();
			}
		case Qt::FontRole:
			{
				QFont font;
				const Node* const node { static_cast< Node* >( index.internalPointer() ) };

				if ( node == nullptr )
				{
					atlas::logging::error( "node is nullptr" );
					return {};
				}

				if ( node->isFolder() )
				{
					const auto& dir_info { std::get< DirInfo >( node->m_info ) };

					font.setItalic( dir_info.is_supporting_name );
					font.setBold( dir_info.is_game_dir );
					return font;
				}
				else if ( node->isFile() )
				{
					const auto& file_info { std::get< FileInfo >( node->m_info ) };

					font.setUnderline( file_info.is_banner || file_info.is_preview );
				}

				return font;
			}
		default:
			return {};
	}
}

SIModel::~SIModel()
{
	if ( m_root == nullptr )
		return;
	else
		delete m_root;
}

Node::Node( const QString str, Node* parent, const bool scan_immediate ) : m_name( str ), m_parent( parent )
{
	if ( scan_immediate ) scan();

	const QFileInfo info { pathStr() };
	if ( info.isDir() )
		m_info = DirInfo();
	else
		m_info = FileInfo();
}

Node* Node::find( const QString filename )
{
	if ( m_children.size() == 0 ) this->scan();

	auto itter { std::find_if(
		m_children.begin(), m_children.end(), [ &filename ]( Node* node ) { return node->name() == filename; } ) };

	if ( itter == m_children.end() )
		return nullptr;
	else
		return *itter;
}

Node* Node::findPath( const std::filesystem::path path )
{
	//Create relative from root node then find relative from that
	const Node* root { this->root() };
	if ( root == nullptr )
		return nullptr;
	else
		return findRelative( std::filesystem::relative( root->path(), path ) );
}

Node* Node::findRelative( std::filesystem::path relative_path )
{
	atlas::logging::debug( "Finding relative path at {} starting at {}", relative_path, this->path() );

	std::vector< QString > pieces;

	while ( !relative_path.empty() )
	{
		pieces.emplace_back( QString::fromStdString( relative_path.filename().string() ) );
		relative_path = relative_path.parent_path();
	}

	//Reverse order
	std::reverse( pieces.begin(), pieces.end() );

	Node* current { this };

	//Find each item
	while ( !pieces.empty() && current != nullptr )
	{
		current = current->find( pieces[ pieces.size() - 1 ] );
		pieces.pop_back();
	}

	return current;
}