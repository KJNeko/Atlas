//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_SIMODEL_HPP
#define ATLASGAMEMANAGER_SIMODEL_HPP

#include <QAbstractItemModel>
#include <QDir>

#include "core/logging.hpp"

enum Type
{
	DIR_NONE,
	GAME_ROOT,
	GAME_EXEC,
	PREVIEW,
	BANNER_N,
	BANNER_W,
	BANNER_L,
	BANNER_C
};

struct Node
{
	Q_DISABLE_COPY_MOVE( Node )

	Type m_type { DIR_NONE };
	QString m_path;

	Node( const QString str, Node* parent = nullptr, const bool scan_immediate = false );

	void scan()
	{
		QDir dir { m_path };
		QFileInfo info { m_path };
		if ( info.isFile() ) return;

		//Scan all files and directories
		for ( const auto& entry : dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) )
			m_children.push_back( new Node( entry.absoluteFilePath(), this ) );

		qDebug() << "Finished scan of " << dir << " found " << m_children.size() << " children";

		for ( auto& child : m_children ) child->scan();
	}

	int childCount() { return static_cast< int >( m_children.size() ); }

	int row()
	{
		if ( m_parent )
		{
			const auto& parent_children { m_parent->m_children };
			return static_cast< int >( std::distance(
				std::find( parent_children.begin(), parent_children.end(), this ), parent_children.begin() ) );
		}
		return 0;
	}

	Node* parent() { return m_parent; }

	Node* child( const int idx )
	{
		if ( m_children.size() < static_cast< std::size_t >( idx ) || idx < 0 )
			return nullptr;
		else
			return m_children[ static_cast< std::size_t >( idx ) ];
	}

	~Node()
	{
		for ( auto& child : m_children ) delete child;
	}

  private:

	std::vector< Node* > m_children {};
	Node* m_parent { nullptr };
};

class SIModel final : public QAbstractItemModel
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( SIModel )

	Node* m_root { nullptr };

  public:

	explicit SIModel( QObject* parent = nullptr );
	~SIModel() override;

	void setRoot( const QString path );

	QModelIndex index( int row, int column, const QModelIndex& parent ) const override;
	QModelIndex parent( const QModelIndex& child ) const override;
	int rowCount( const QModelIndex& parent ) const override;
	int columnCount( const QModelIndex& parent ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
};

#endif //ATLASGAMEMANAGER_SIMODEL_HPP
