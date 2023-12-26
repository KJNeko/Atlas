//
// Created by kj16609 on 12/6/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ImportSelectionPopup.h" resolved

#include "ImportSelectionPopup.hpp"

#include <QDialog>
#include <QFileDialog>

#include "core/logging/logging.hpp"
#include "ui/importer/batchImporter/BatchImportDialog.hpp"
#include "ui/importer/simpleImporter/SimpleImporter.hpp"
#include "ui/importer/singleImporter/SingleImporter.hpp"
#include "ui_ImportSelectionPopup.h"

namespace atlas::ui::imports
{
	enum ImportSelectionStackPages
	{
		GameCountSelection = 0,
		FormatSelection,
		SelectPage,
	};

	ImportSelectionPopup::ImportSelectionPopup( QWidget* parent ) :
	  QDialog( parent ),
	  ui( new Ui::ImportSelectionPopup )
	{
		ui->setupUi( this );
	}

	ImportSelectionPopup::~ImportSelectionPopup()
	{
		delete ui;
	}

	void ImportSelectionPopup::on_btnOneGame_pressed()
	{
		use_id = SINGLE;

		ui->stackedWidget->setCurrentIndex( SelectPage );
	}

	void ImportSelectionPopup::on_btnMultipleGames_pressed()
	{
		ui->stackedWidget->setCurrentIndex( FormatSelection );
		ui->btnSelectFolder->setText( " Click to select top level folder " );
	}

	void ImportSelectionPopup::on_btnLaidRandom_pressed()
	{
		use_id = TREE;
		ui->stackedWidget->setCurrentIndex( SelectPage );
	}

	void ImportSelectionPopup::on_btnLaidSorted_pressed()
	{
		use_id = BULK;
		ui->stackedWidget->setCurrentIndex( SelectPage );
	}

	void ImportSelectionPopup::on_btnSelectFolder_pressed()
	{
		//Open folder prompt
		const auto filepath { QFileDialog::getExistingDirectory( this, "Select folder to import" ) };

		//Check if filepath is empty or if it doesn't exist then return

		if ( filepath.isEmpty() ) return;

		if ( !std::filesystem::exists( filepath.toStdString() ) )
		{
			atlas::logging::warn( "Invalid filepath given: {}. Aborting import process", filepath );
			return;
		}

		this->hide();

		switch ( use_id )
		{
			default:
				[[fallthrough]];
			case NONE:
				{
					atlas::logging::error(
						"Somehow managed to get to folder selection without a use_id set.",
						static_cast< int >( use_id ) );
					return;
				}
				break;
			case SINGLE:
				{
					SingleImporter importer { this->parentWidget() };
					importer.setPath( filepath );
					importer.exec();
				}
				break;
			case BULK:
				{
					BatchImportDialog importer { this->parentWidget() };
					importer.setPath( filepath );
					importer.exec();
				}
				break;
			case TREE:
				{
					SimpleImporter importer { this->parentWidget() };
					importer.setRoot( filepath );
					importer.exec();
				}
				break;
		}
	}

	void ImportSelectionPopup::on_lblDragHere_dragEnterEvent( QDragEnterEvent* event )
	{
		//TODO: Handle
	}

} // namespace atlas::ui::imports
