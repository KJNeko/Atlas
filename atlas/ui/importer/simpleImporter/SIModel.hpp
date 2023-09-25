//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_SIMODEL_HPP
#define ATLASGAMEMANAGER_SIMODEL_HPP

#include <QAbstractItemModel>
#include <QDir>

#include "core/config/config.hpp"
#include "core/logging/logging.hpp"

enum class SupportingType
{
	NoSupportingType,
	TITLE,
	CREATOR,
	VERSION,
	ENGINE
};

struct DirInfo
{
	bool is_game_dir { false };
	QString title { "" };
	QString creator { "" };
	QString version { "" };
	QString engine { "" };

	bool is_supporting_name { false };
	SupportingType supporting_type { SupportingType::NoSupportingType };
};

struct FileInfo
{
	bool is_banner { false };
	BannerType banner_type { BannerType::Normal };

	bool is_preview { false };
};

struct Node
{
	Q_DISABLE_COPY_MOVE( Node )

	std::variant< DirInfo, FileInfo > m_info { DirInfo {} };
	QString m_path;

  private:

	bool m_scanned { false };

  public:

	Node( const QString str, Node* parent = nullptr, const bool scan_immediate = false );

	QString name() const
	{
		const auto split_pos { m_path.lastIndexOf( QDir::separator() ) };
		return m_path.mid( split_pos + 1 );
	}

	void scan()
	{
		QDir dir { m_path };
		QFileInfo info { m_path };
		if ( info.isFile() ) return;

		//Scan all files and directories
		for ( const auto& entry : dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) )
			m_children.push_back( new Node( entry.absoluteFilePath(), this ) );

		m_scanned = true;
	}

	bool scanned() const { return m_scanned; }

	int childCount() const { return static_cast< int >( m_children.size() ); }

	int row() const
	{
		if ( m_parent )
		{
			const auto& parent_children { m_parent->m_children };
			return static_cast< int >( std::distance(
				parent_children.begin(), std::find( parent_children.begin(), parent_children.end(), this ) ) );
		}
		return 0;
	}

	const Node* parent() const { return m_parent; }

	const Node* child( const int idx ) const
	{
		if ( m_children.size() < static_cast< std::size_t >( idx ) || idx < 0 )
			return nullptr;
		else
			return m_children[ static_cast< std::size_t >( idx ) ];
	}

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
