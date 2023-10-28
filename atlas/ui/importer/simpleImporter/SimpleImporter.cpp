//
// Created by kj16609 on 5/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SimpleImporter.h" resolved

#include "SimpleImporter.hpp"

#include <moc_SimpleImporter.cpp>

#include <QComboBox>
#include <QMenu>

#include "SIModel.hpp"
#include "ui_SimpleImporter.h"

SimpleImporter::SimpleImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::SimpleImporter )
{
	ui->setupUi( this );

	ui->dirView->setModel( new SIModel() );
	ui->dirView->setContextMenuPolicy( Qt::CustomContextMenu );

	connect( ui->dirView, &QTreeView::customContextMenuRequested, this, &SimpleImporter::onCustomContextMenuRequested );

	connect(
		ui->dirView->selectionModel(),
		&QItemSelectionModel::selectionChanged,
		this,
		&SimpleImporter::dirView_itemSelectionChanged );
}

SimpleImporter::~SimpleImporter()
{
	delete ui;
}

void SimpleImporter::setRoot( const QString path )
{
	static_cast< SIModel* >( ui->dirView->model() )->setRoot( path );
}

enum SimpleImporterPages
{
	DirPage = 0,
	BlankPage,
	StartPage,
	FilePage
};

void SimpleImporter::dirView_itemSelectionChanged(
	[[maybe_unused]] const QItemSelection& selected, [[maybe_unused]] const QItemSelection& deselected )
{
	updateSidebar();
}

int depthOfIndex( const QModelIndex& index )
{
	int depth { 0 };
	QModelIndex parent { index.parent() };
	while ( parent.isValid() )
	{
		++depth;
		parent = parent.parent();
	}
	return depth;
}

void SimpleImporter::onCustomContextMenuRequested( [[maybe_unused]] const QPoint& point )
{
	QMenu menu;

	const QModelIndex item { ui->dirView->indexAt( point ) };
	int idx_depth { depthOfIndex( item ) };

	Node* node { static_cast< Node* >( item.internalPointer() ) };
	const bool is_folder { std::holds_alternative< DirInfo >( node->m_info ) };

	menu.addSection( QString( "Depth: %1" ).arg( idx_depth ) );

	if ( is_folder )
	{
		auto* dir_info { &std::get< DirInfo >( node->m_info ) };

		auto this_item_menu { menu.addMenu( "This Item" ) };

		this_item_menu->addAction(
			"Set nothing",
			[ dir_info, &item, this ]()
			{
				dir_info->is_supporting_name = false;
				dir_info->is_game_dir = false;
				dir_info->supporting_type = SupportingType::TITLE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );

		this_item_menu->addAction(
			"Set game root",
			[ dir_info, &item, this ]()
			{
				dir_info->is_game_dir = true;
				this->ui->dirView->model()->dataChanged( item, item );
			} );

		auto this_item_supporting_menu { this_item_menu->addMenu( "Set supporting" ) };
		this_item_supporting_menu->addAction(
			"None",
			[ dir_info, &item, this ]()
			{
				dir_info->is_supporting_name = false;
				dir_info->supporting_type = SupportingType::TITLE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Title",
			[ dir_info, &item, this ]()
			{
				dir_info->is_supporting_name = true;
				dir_info->supporting_type = SupportingType::TITLE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Creator",
			[ dir_info, &item, this ]()
			{
				dir_info->is_supporting_name = true;
				dir_info->supporting_type = SupportingType::CREATOR;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Version",
			[ dir_info, &item, this ]()
			{
				dir_info->is_supporting_name = true;
				dir_info->supporting_type = SupportingType::VERSION;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Engine",
			[ dir_info, &item, this ]()
			{
				dir_info->is_supporting_name = true;
				dir_info->supporting_type = SupportingType::ENGINE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );

		auto this_level { menu.addMenu( "This Depth" ) };
		this_level->addAction( "Set nothing", []() {} );
		this_level->addAction( "Set game root", []() {} );
		auto this_level_supporting_menu { this_level->addMenu( "Set supporting" ) };
		this_level_supporting_menu->addAction( "None", []() {} );
		this_level_supporting_menu->addAction( "Title", []() {} );
		this_level_supporting_menu->addAction( "Creator", []() {} );
		this_level_supporting_menu->addAction( "Version", []() {} );
		this_level_supporting_menu->addAction( "Engine", []() {} );

		menu.addAction( "Set preview folder", []() {} );
	}
	else
	{
		menu.addAction( "Set preview", []() {} );

		auto banner_actions { menu.addMenu( "Set banner" ) };
		banner_actions->addAction( "Normal", []() {} );
		banner_actions->addAction( "Wide", []() {} );
		banner_actions->addAction( "Logo", []() {} );
		banner_actions->addAction( "Cover", []() {} );
	}

	menu.exec( QCursor::pos() );
}

std::vector< QPersistentModelIndex > SimpleImporter::selected() const
{
	std::vector< QPersistentModelIndex > nodes;

	for ( const auto& index : ui->dirView->selectionModel()->selectedIndexes() ) nodes.push_back( index );

	return nodes;
}

void SimpleImporter::on_cIsGameRoot_toggled( bool checked )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).is_game_dir = checked;
		}

		ui->dirView->model()->dataChanged( node_idx, node_idx );
	}
	updateSidebar();
}

void SimpleImporter::on_cIsSupporting_toggled( bool checked )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).is_supporting_name = checked;
		}

		ui->dirView->model()->dataChanged( node_idx, node_idx );
	}
	updateSidebar();
}

void SimpleImporter::on_cbSupportingSelection_currentIndexChanged( int index )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).supporting_type = static_cast< SupportingType >( index );
		}

		ui->dirView->model()->dataChanged( node_idx, node_idx );
	}
	updateSidebar();
}

void SimpleImporter::on_leCreator_textChanged( const QString& text )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).creator = text;
		}
	}
	updateSidebar();
}

void SimpleImporter::on_leVersion_textChanged( const QString& text )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).version = text;
		}
	}
	updateSidebar();
}

void SimpleImporter::on_leTitle_textChanged( const QString& text )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).title = text;
		}
	}
	updateSidebar();
}

void SimpleImporter::on_leEngine_textChanged( const QString& text )
{
	const auto current { selected() };

	if ( current.empty() || no_modification ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< DirInfo >( node->m_info ) )
		{
			std::get< DirInfo >( node->m_info ).engine = text;
		}
	}
	updateSidebar();
}

void SimpleImporter::updateSidebar()
{
	const auto current { selected() };
	no_modification = true; // Prevent the GUI slots from making changes to the record.

	//Check if current selection is just made of files or directories.

	if ( current.empty() )
	{
		ui->stackedWidget->setCurrentIndex( StartPage );
		return;
	}

	std::size_t dir_count { 0 };
	std::size_t file_count { 0 };

	for ( const auto& index : current )
	{
		const Node* node { static_cast< Node* >( index.internalPointer() ) };
		const auto variant_idx { node->m_info.index() };

		if ( variant_idx == 0 )
			++dir_count;
		else
			++file_count;

		if ( dir_count > 0 && file_count > 0 )
		{
			//In this case we have a mix so we can't do anything.
			ui->stackedWidget->setCurrentIndex( BlankPage );
			return;
		}
	}

	if ( dir_count > 0 && file_count == 0 ) //Only dirs
	{
		ui->stackedWidget->setCurrentIndex( DirPage );

		std::size_t checked_root { 0 };
		std::size_t checked_support { 0 };
		std::size_t count { 0 };

		// Count up how many are checked and how many are not.
		for ( const auto& idx : current )
		{
			const Node* node { static_cast< Node* >( idx.internalPointer() ) };
			if ( std::holds_alternative< DirInfo >( node->m_info ) )
			{
				const auto& dir_info { std::get< DirInfo >( node->m_info ) };
				if ( dir_info.is_game_dir ) ++checked_root;
				if ( dir_info.is_supporting_name ) ++checked_support;
				++count;
			}
		}

		// Tristate handling for if we have a mix of checked and unchecked dirs
		if ( checked_root < count && checked_root > 0 )
			ui->cIsGameRoot->setCheckState( Qt::PartiallyChecked );
		else if ( checked_root == count )
			ui->cIsGameRoot->setCheckState( Qt::Checked );
		else
			ui->cIsGameRoot->setCheckState( Qt::Unchecked );

		if ( checked_support == count )
			ui->cIsSupporting->setCheckState( Qt::Checked );
		else if ( checked_support > 0 )
			ui->cIsSupporting->setCheckState( Qt::PartiallyChecked );
		else
			ui->cIsSupporting->setCheckState( Qt::Unchecked );

		if ( count > 1 )
		{
			//Multi selection. Don't allow editing of the text fields
			ui->gameBasicInfo->setEnabled( false );
		}
		else
		{
			//Only one selection if we are here.
			Node* node { static_cast< Node* >( current.front().internalPointer() ) };
			const auto& dir_info { std::get< DirInfo >( node->m_info ) };
			const auto& [ is_game, title, creator, version, engine, supporting, type ] { dir_info };

			if ( is_game )
			{
				ui->gameBasicInfo->setEnabled( true );

				ui->leTitle->setText( title );
				ui->leCreator->setText( creator );
				ui->leEngine->setText( engine );
				ui->leVersion->setText( version );
			}

			if ( supporting ) ui->cbSupportingSelection->setCurrentIndex( static_cast< int >( type ) );
		}
	}
	else if ( file_count > 0 && dir_count == 0 ) // Only files
	{
		ui->stackedWidget->setCurrentIndex( FilePage );

		std::size_t checked_banner { 0 };
		std::size_t checked_preview { 0 };
		std::size_t total { 0 };

		for ( const auto& index : current )
		{
			Node* node { static_cast< Node* >( index.internalPointer() ) };
			if ( std::holds_alternative< FileInfo >( node->m_info ) )
			{
				const auto& file_info { std::get< FileInfo >( node->m_info ) };
				if ( file_info.is_banner ) ++checked_banner;
				if ( file_info.is_preview ) ++checked_preview;
				++total;
			}
		}

		if ( checked_banner < total && checked_banner > 0 )
			ui->cIsBanner->setCheckState( Qt::PartiallyChecked );
		else if ( checked_banner == total )
			ui->cIsBanner->setCheckState( Qt::Checked );
		else
			ui->cIsBanner->setCheckState( Qt::Unchecked );

		if ( checked_preview < total && checked_preview > 0 )
			ui->cIsPreview->setCheckState( Qt::PartiallyChecked );
		else if ( checked_preview == total )
			ui->cIsPreview->setCheckState( Qt::Checked );
		else
			ui->cIsPreview->setCheckState( Qt::Unchecked );

		if ( total > 1 )
			ui->cbBannerType->setCurrentIndex( 0 );
		else
		{
			Node* first { static_cast< Node* >( current.front().internalPointer() ) };
			const auto& file_info { std::get< FileInfo >( first->m_info ) };
			const auto& [ is_banner, type, is_preview ] { file_info };
			if ( is_banner ) ui->cbBannerType->setCurrentIndex( static_cast< int >( type ) );
		}
	}
	else // Mix
	{
		ui->stackedWidget->setCurrentIndex( BlankPage );
	}

	updateSidebar();
	no_modification = false;
}

void SimpleImporter::on_cIsBanner_toggled( bool checked )
{
	if ( no_modification ) return;

	const auto current { selected() };

	if ( current.size() == 0 ) return;

	for ( const auto& index : current )
	{
		Node* node { static_cast< Node* >( index.internalPointer() ) };
		if ( std::holds_alternative< FileInfo >( node->m_info ) )
		{
			auto& file_info { std::get< FileInfo >( node->m_info ) };
			file_info.is_banner = checked;
		}
	}
}

void SimpleImporter::on_cIsPreview_toggled( bool checked )
{
	if ( no_modification ) return;

	const auto current { selected() };

	if ( current.size() == 0 ) return;

	for ( const auto& index : current )
	{
		Node* node { static_cast< Node* >( index.internalPointer() ) };
		if ( std::holds_alternative< FileInfo >( node->m_info ) )
		{
			auto& file_info { std::get< FileInfo >( node->m_info ) };
			file_info.is_preview = checked;
		}
	}
}

void SimpleImporter::on_cbBannerType_currentIndexChanged( int index )
{
	if ( no_modification ) return;

	const auto current { selected() };

	if ( current.size() == 0 ) return;

	for ( const auto& node_idx : current )
	{
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< FileInfo >( node->m_info ) )
		{
			auto& file_info { std::get< FileInfo >( node->m_info ) };
			file_info.banner_type = static_cast< BannerType >( index );
		}
	}
}
