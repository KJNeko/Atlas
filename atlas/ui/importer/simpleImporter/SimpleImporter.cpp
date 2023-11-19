//
// Created by kj16609 on 5/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SimpleImporter.h" resolved

#include "SimpleImporter.hpp"

#include <moc_SimpleImporter.cpp>

#include <QComboBox>
#include <QMenu>
#include <QProgressDialog>

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
	int depth { 1 };
	QModelIndex parent { index.parent() };
	while ( parent.isValid() )
	{
		++depth;
		parent = parent.parent();
	}
	return depth;
}

void SimpleImporter::setGameRoot( Node* node )
{
	if ( node->isFolder() )
	{
		auto& node_info { node->dirInfo() };
		node_info.is_game_dir = true;

		//Detect for any banners or preview folders

		QProgressDialog progress_dialog { "Scanning...", "", 0, 1, this };

		progress_dialog.show();
		node->scan();
		progress_dialog.setValue( 1 );

		auto children { node->children() };

		progress_dialog.setMaximum( children.size() );

		for ( auto child : children )
		{
			progress_dialog.setValue( progress_dialog.value() + 1 );
			QApplication::processEvents();
			if ( child->isFolder() && child->name() == "previews" )
			{
				child->scan();

				//Take all files within that folder and mark as previews
				for ( const auto& previews_children : child->children() )
				{
					if ( previews_children->isFile() ) previews_children->fileInfo().is_preview = true;
				}
			}
			else if ( child->isFile() && child->name().startsWith( "banner" ) )
			{
				//If the child is a file and starts with 'banner' then mark as banner
				child->fileInfo().is_banner = true;
			}
		}
	}
	else
		return;
}

void SimpleImporter::onCustomContextMenuRequested( [[maybe_unused]] const QPoint& point )
{
	QMenu menu;

	const QModelIndex item { ui->dirView->indexAt( point ) };
	int idx_depth { depthOfIndex( item ) };

	Node* node { static_cast< Node* >( item.internalPointer() ) };

	menu.addSection( QString( "Depth: %1" ).arg( idx_depth ) );

	if ( node->isFolder() )
	{
		DirInfo& dir_info { node->dirInfo() };

		auto this_item_menu { menu.addMenu( "This Item" ) };

		this_item_menu->addAction(
			"Set nothing",
			[ &dir_info, &item, this ]()
			{
				dir_info.is_supporting_name = false;
				dir_info.is_game_dir = false;
				dir_info.supporting_type = SupportingType::TITLE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );

		this_item_menu->addAction(
			"Set game root",
			[ node, &item, this ]()
			{
				setGameRoot( node );
				this->ui->dirView->model()->dataChanged( item, item );
			} );

		auto this_item_supporting_menu { this_item_menu->addMenu( "Set supporting" ) };
		this_item_supporting_menu->addAction(
			"None",
			[ &dir_info, &item, this ]()
			{
				dir_info.is_supporting_name = false;
				dir_info.supporting_type = SupportingType::TITLE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Title",
			[ &dir_info, &item, this ]()
			{
				dir_info.is_supporting_name = true;
				dir_info.supporting_type = SupportingType::TITLE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Creator",
			[ &dir_info, &item, this ]()
			{
				dir_info.is_supporting_name = true;
				dir_info.supporting_type = SupportingType::CREATOR;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Version",
			[ &dir_info, &item, this ]()
			{
				dir_info.is_supporting_name = true;
				dir_info.supporting_type = SupportingType::VERSION;
				this->ui->dirView->model()->dataChanged( item, item );
			} );
		this_item_supporting_menu->addAction(
			"Engine",
			[ &dir_info, &item, this ]()
			{
				dir_info.is_supporting_name = true;
				dir_info.supporting_type = SupportingType::ENGINE;
				this->ui->dirView->model()->dataChanged( item, item );
			} );

		Node* root { node->root() };

		auto this_level { menu.addMenu( "This Depth" ) };
		this_level->addAction(
			"Set nothing",
			[ idx_depth, root ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };

				for ( auto child : children )
				{
					QApplication::processEvents();

					if ( std::holds_alternative< DirInfo >( child->m_info ) )
					{
						DirInfo& info { std::get< DirInfo >( child->m_info ) };
						info.is_supporting_name = false;
						info.is_game_dir = false;
					}
				}
			} );

		this_level->addAction(
			"Set game root",
			[ idx_depth, root, this ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };
				for ( auto child : children )
				{
					QApplication::processEvents();
					setGameRoot( child );
				}
			} );

		auto this_level_supporting_menu { this_level->addMenu( "Set supporting" ) };

		this_level_supporting_menu->addAction(
			"None",
			[ idx_depth, root ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };
				for ( auto child : children )
				{
					QApplication::processEvents();
					if ( std::holds_alternative< DirInfo >( child->m_info ) )
					{
						DirInfo& info { std::get< DirInfo >( child->m_info ) };
						info.is_supporting_name = false;
					}
				}
			} );
		this_level_supporting_menu->addAction(
			"Title",
			[ idx_depth, root ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };
				for ( auto child : children )
				{
					QApplication::processEvents();
					if ( std::holds_alternative< DirInfo >( child->m_info ) )
					{
						DirInfo& info { std::get< DirInfo >( child->m_info ) };
						info.is_supporting_name = true;
						info.supporting_type = SupportingType::TITLE;
					}
				}
			} );
		this_level_supporting_menu->addAction(
			"Creator",
			[ idx_depth, root ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };
				for ( auto child : children )
				{
					QApplication::processEvents();
					if ( std::holds_alternative< DirInfo >( child->m_info ) )
					{
						DirInfo& info { std::get< DirInfo >( child->m_info ) };
						info.is_supporting_name = true;
						info.supporting_type = SupportingType::CREATOR;
					}
				}
			} );
		this_level_supporting_menu->addAction(
			"Version",
			[ idx_depth, root ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };
				for ( auto child : children )
				{
					QApplication::processEvents();
					if ( std::holds_alternative< DirInfo >( child->m_info ) )
					{
						DirInfo& info { std::get< DirInfo >( child->m_info ) };
						info.is_supporting_name = true;
						info.supporting_type = SupportingType::VERSION;
					}
				}
			} );
		this_level_supporting_menu->addAction(
			"Engine",
			[ idx_depth, root ]()
			{
				auto children { root->childrenAtDepth( idx_depth ) };
				for ( auto child : children )
				{
					QApplication::processEvents();
					if ( std::holds_alternative< DirInfo >( child->m_info ) )
					{
						DirInfo& info { std::get< DirInfo >( child->m_info ) };
						info.is_supporting_name = true;
						info.supporting_type = SupportingType::ENGINE;
					}
				}
			} );

		menu.addAction(
			"Set preview folder",
			[ node ]()
			{
				auto children { node->children() };

				for ( auto child : children )
				{
					QApplication::processEvents();
					if ( std::holds_alternative< FileInfo >( child->m_info ) )
					{
						auto& info { std::get< FileInfo >( child->m_info ) };
						info.is_preview = true;
					}
				}
			} );
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
	updateSidebar();
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
		ui->dirView->model()->dataChanged( node_idx, node_idx );
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
		ui->dirView->model()->dataChanged( node_idx, node_idx );
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
		ui->dirView->model()->dataChanged( node_idx, node_idx );
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
		ui->dirView->model()->dataChanged( node_idx, node_idx );
	}
	updateSidebar();
}

void SimpleImporter::updateSidebar()
{
	no_modification = true; // Prevent the GUI slots from making changes to the record.

	//Wipe UI
	ui->stackedWidget->setCurrentIndex( BlankPage );
	ui->leTitle->clear();
	ui->leCreator->clear();
	ui->leEngine->clear();
	ui->leVersion->clear();
	ui->cIsGameRoot->setCheckState( Qt::Unchecked );
	ui->cIsSupporting->setCheckState( Qt::Unchecked );
	ui->cbSupportingSelection->setCurrentIndex( static_cast< int >( SupportingType::TITLE ) );

	const std::vector< QPersistentModelIndex > current { selected() };

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
			QApplication::processEvents();
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
			const DirInfo dir_info { node->filledInfo() };
			const auto& [ is_game_dir, title, creator, version, engine, supporting, supporting_type, supporting_mask ] {
				dir_info
			};

			if ( is_game_dir )
			{
				ui->gameBasicInfo->setEnabled( true );

				ui->leTitle->setText( title );
				ui->leCreator->setText( creator );
				ui->leEngine->setText( engine );
				ui->leVersion->setText( version );

				//Lock depending on mask
				ui->leTitle->setEnabled( !( supporting_mask & SupportingMask::TITLE ) );
				ui->leCreator->setEnabled( !( supporting_mask & SupportingMask::CREATOR ) );
				ui->leEngine->setEnabled( !( supporting_mask & SupportingMask::ENGINE ) );
				ui->leVersion->setEnabled( !( supporting_mask & SupportingMask::VERSION ) );
			}
			else
			{
				ui->leTitle->setEnabled( false );
				ui->leCreator->setEnabled( false );
				ui->leEngine->setEnabled( false );
				ui->leVersion->setEnabled( false );
			}

			if ( supporting ) ui->cbSupportingSelection->setCurrentIndex( static_cast< int >( supporting_type ) );
		}
	}
	else if ( file_count > 0 && dir_count == 0 ) // Only files
	{
		ui->stackedWidget->setCurrentIndex( FilePage );

		std::size_t checked_banner { 0 };
		std::size_t checked_preview { 0 };
		std::size_t checked_executables { 0 };
		std::size_t total { 0 };

		for ( const auto& index : current )
		{
			QApplication::processEvents();
			Node* node { static_cast< Node* >( index.internalPointer() ) };
			if ( std::holds_alternative< FileInfo >( node->m_info ) )
			{
				const auto& file_info { std::get< FileInfo >( node->m_info ) };
				if ( file_info.is_banner ) ++checked_banner;
				if ( file_info.is_preview ) ++checked_preview;
				if ( file_info.is_executable ) ++checked_executables;
				++total;
			}
		}

		ui->cbBannerType->setDisabled( checked_banner < total && checked_banner > 0 );

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

		if ( checked_executables < total && checked_executables > 0 )
			ui->cbIsExecutable->setCheckState( Qt::PartiallyChecked );
		else if ( checked_executables == total )
			ui->cbIsExecutable->setCheckState( Qt::Checked );
		else
			ui->cbIsExecutable->setCheckState( Qt::Unchecked );
	}
	else // Mix
	{
		ui->stackedWidget->setCurrentIndex( BlankPage );
	}

	no_modification = false;
}

void SimpleImporter::on_cIsBanner_toggled( bool checked )
{
	if ( no_modification ) return;

	const auto current { selected() };

	if ( current.size() == 0 ) return;

	for ( const auto& index : current )
	{
		QApplication::processEvents();
		Node* node { static_cast< Node* >( index.internalPointer() ) };
		if ( std::holds_alternative< FileInfo >( node->m_info ) )
		{
			auto& file_info { std::get< FileInfo >( node->m_info ) };
			file_info.is_banner = checked;
		}
		ui->dirView->model()->dataChanged( index, index );
	}
}

void SimpleImporter::on_cIsPreview_toggled( bool checked )
{
	if ( no_modification ) return;

	const auto current { selected() };

	if ( current.size() == 0 ) return;

	for ( const auto& index : current )
	{
		QApplication::processEvents();
		Node* node { static_cast< Node* >( index.internalPointer() ) };
		if ( std::holds_alternative< FileInfo >( node->m_info ) )
		{
			auto& file_info { std::get< FileInfo >( node->m_info ) };
			file_info.is_preview = checked;
		}
		ui->dirView->model()->dataChanged( index, index );
	}
}

void SimpleImporter::on_cbBannerType_currentIndexChanged( int index )
{
	if ( no_modification ) return;

	const auto current { selected() };

	if ( current.size() == 0 ) return;

	for ( const auto& node_idx : current )
	{
		QApplication::processEvents();
		Node* node { static_cast< Node* >( node_idx.internalPointer() ) };
		if ( std::holds_alternative< FileInfo >( node->m_info ) )
		{
			auto& file_info { std::get< FileInfo >( node->m_info ) };
			file_info.banner_type = static_cast< BannerType >( index );
		}
		ui->dirView->model()->dataChanged( node_idx, node_idx );
	}
}
