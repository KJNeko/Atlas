//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_SIMODEL_HPP
#define ATLASGAMEMANAGER_SIMODEL_HPP

#include <QAbstractItemModel>
#include <QDir>

#include <queue>

#include "core/config/config.hpp"
#include "core/logging/logging.hpp"

enum class SupportingType
{
	TITLE = 0,
	CREATOR = 1,
	VERSION = 2,
	ENGINE = 3
};

enum SupportingMask : int
{
	NO_SUPPORTING_MASK = 0,
	TITLE = 1 << 0,
	CREATOR = 1 << 1,
	VERSION = 1 << 2,
	ENGINE = 1 << 3
};

struct DirInfo
{
	bool is_game_dir { false };
	QString title { "" };
	QString creator { "" };
	QString version { "" };
	QString engine { "" };

	bool is_supporting_name { false };
	SupportingType supporting_type { SupportingType::TITLE };
	int supporting_mask { SupportingMask::NO_SUPPORTING_MASK };
};

struct FileInfo
{
	bool is_banner { false };
	BannerType banner_type { BannerType::Normal };

	bool is_preview { false };
	bool is_executable { false };
};

struct Node
{
	Q_DISABLE_COPY_MOVE( Node )

	std::variant< DirInfo, FileInfo > m_info { DirInfo {} };
	QString m_name;

	std::vector< Node* > m_children {};
	Node* m_parent { nullptr };

	bool m_scanned { false };

  public:

	DirInfo filledInfo() const
	{
		if ( !std::holds_alternative< DirInfo >( m_info ) )
			return {};
		else
		{
			auto info { std::get< DirInfo >( m_info ) };

			//Check for any fields populated by the parents
			const Node* ptr { this };

			while ( ptr != nullptr )
			{
				if ( !std::holds_alternative< DirInfo >( ptr->m_info ) ) //The fuck?
					throw std::runtime_error( "Expected dir info but got file info instead!" );

				const auto& parent_info { std::get< DirInfo >( ptr->m_info ) };

				if ( parent_info.is_supporting_name )
				{
					switch ( parent_info.supporting_type )
					{
						case SupportingType::TITLE:
							info.title = ptr->name();
							info.supporting_mask |= SupportingMask::TITLE;
							break;
						case SupportingType::CREATOR:
							info.creator = ptr->name();
							info.supporting_mask |= SupportingMask::CREATOR;
							break;
						case SupportingType::VERSION:
							info.version = ptr->name();
							info.supporting_mask |= SupportingMask::VERSION;
							break;
						case SupportingType::ENGINE:
							info.engine = ptr->name();
							info.supporting_mask |= SupportingMask::ENGINE;
							break;
					}
				}

				ptr = ptr->parent();
			}

			return info;
		}
	}

	Node( const QString str, Node* parent = nullptr, const bool scan_immediate = false );

	QString name() const
	{
		return m_name;
		//const auto split_pos { m_path.lastIndexOf( QDir::separator() ) };
		//return m_path.mid( split_pos + 1 );
	}

	void scan()
	{
		const QString path_str { this->pathStr() };
		QFileInfo info { path_str };

		if ( info.isFile() ) return;

		QDir dir { path_str };
		//Scan all files and directories
		for ( const auto& entry : dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) )
			m_children.push_back( new Node( entry.fileName(), this ) );

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

	Node* root()
	{
		Node* ptr { this };

		if ( this->parent() == nullptr ) return ptr;

		while ( ptr->parent() != nullptr )
		{
			ptr = ptr->parent();
		}

		return ptr;
	}

	const Node* root() const
	{
		const Node* ptr { this };
		if ( this->parent() == nullptr ) return ptr;

		while ( ptr->parent() != nullptr )
		{
			ptr = ptr->parent();
		}

		return ptr;
	}

	int depth() const
	{
		int counter { 0 };
		const Node* ptr { this };

		if ( this->parent() == nullptr ) return 0;

		while ( ptr != nullptr )
		{
			++counter;
			ptr = ptr->parent();
		}

		return counter;
	}

	std::vector< Node* > childrenAtDepth( const int target_depth )
	{
		if ( target_depth == 0 )
			return { this };
		else if ( target_depth > 0 )
		{
			if ( !m_scanned ) scan();

			std::vector< Node* > nodes;

			for ( auto child : m_children )
			{
				auto child_data { child->childrenAtDepth( target_depth - 1 ) };
				std::copy( child_data.begin(), child_data.end(), std::back_inserter( nodes ) );
			}

			return nodes;
		}
		else
			return {};
	}

	const Node* parent() const { return m_parent; }

	Node* parent() { return m_parent; }

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

	bool isFile() const { return std::holds_alternative< FileInfo >( m_info ); }

	bool isFolder() const { return std::holds_alternative< DirInfo >( m_info ); }

	DirInfo& dirInfo() { return std::get< DirInfo >( m_info ); }

	FileInfo& fileInfo() { return std::get< FileInfo >( m_info ); }

	std::vector< Node* > children() const { return m_children; }

	std::filesystem::path path() const
	{
		if ( m_parent == nullptr )
			return std::filesystem::path( name().toStdString() );
		else
			return m_parent->path() / name().toStdString();
	}

	QString pathStr() const
	{
		if ( m_parent == nullptr )
			return name();
		else
			return m_parent->pathStr() + QDir::separator() + name();
	}

	~Node()
	{
		for ( auto child : m_children ) delete child;
	}
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
