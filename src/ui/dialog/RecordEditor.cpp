//
// Created by kj16609 on 1/13/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordEditor.h" resolved

#include <QFileDialog>
#include <QStandardPaths>
#include "RecordEditor.hpp"
#include "ui_RecordEditor.h"
#include "backend/records/RecordBase.hpp"


RecordEditor::RecordEditor( Record record, QWidget* parent ) :
  QDialog( parent ),
  m_record( record ),
  ui( new Ui::RecordEditor )
{
	ui->setupUi( this );

	switch ( record->getType() )
	{
		case RecordType::GAME_RECORD:
			ui->tabWidget->setTabEnabled( 3, true );
			ui->tabWidget->setTabEnabled( 4, false );
			ui->tabWidget->setTabEnabled( 5, false );
			ui->recordType->setCurrentIndex( 1 );
			break;
		case RecordType::NORMAL_RECORD:
			ui->tabWidget->setTabEnabled( 3, false );
			ui->tabWidget->setTabEnabled( 4, false );
			ui->tabWidget->setTabEnabled( 5, false );
			ui->recordType->setCurrentIndex( 0 );
			break;
		default:
			ui->tabWidget->setTabEnabled( 3, false );
			ui->tabWidget->setTabEnabled( 4, false );
			ui->tabWidget->setTabEnabled( 5, false );
			ui->recordType->setCurrentIndex( 0 );
			break;
	}

	ui->recordName->setText( record->getName() );
	ui->bannerPath->setText( QString::fromStdString( record->getBannerPath().string() ) );
}

RecordEditor::~RecordEditor()
{
	delete ui;
}


void RecordEditor::on_recordType_currentIndexChanged()
{
	switch ( ui->recordType->currentIndex() )
	{
		default:
			[[fallthrough]];
		case 0:
			ui->tabWidget->setTabEnabled( 3, false );
			ui->tabWidget->setTabEnabled( 4, false );
			ui->tabWidget->setTabEnabled( 5, false );
			break;
		case 1:
			ui->tabWidget->setTabEnabled( 3, true );
			ui->tabWidget->setTabEnabled( 4, false );
			ui->tabWidget->setTabEnabled( 5, false );
			break;
		case 2:
			ui->tabWidget->setTabEnabled( 3, false );
			ui->tabWidget->setTabEnabled( 4, true );
			ui->tabWidget->setTabEnabled( 5, false );
			break;
		case 3:
			ui->tabWidget->setTabEnabled( 3, false );
			ui->tabWidget->setTabEnabled( 4, false );
			ui->tabWidget->setTabEnabled( 5, true );
			break;
	}
}

void RecordEditor::on_selectBanner_clicked()
{
	QFileDialog dialog { this };
	dialog.setFileMode( QFileDialog::ExistingFile );
	dialog.setNameFilter( tr( "Images (*.png *.png *.bmp)" ) );
	dialog.setViewMode( QFileDialog::Detail );

	QStringList files;
	if ( dialog.exec() )
	{
		files = dialog.selectedFiles();
		if ( files.size() > 0 ) { ui->bannerPath->setText( files.first() ); }
	}
}

void RecordEditor::on_selectExec_clicked()
{
	QFileDialog dialog { this };
	dialog.setFileMode( QFileDialog::ExistingFile );
	dialog.setNameFilter( tr( "Executables (*.exe *.msi *.py" ) );
	dialog.setViewMode( QFileDialog::Detail );

	QStringList files;
	if ( dialog.exec() )
	{
		files = dialog.selectedFiles();
		if ( files.size() > 0 ) { ui->execPath->setText( files.first() ); }
	}
}

void RecordEditor::on_apply_clicked()
{
	ui->status_label->setText( "Verifying settings" );

	//Checking if banner image is good
	if ( !std::filesystem::exists( ui->bannerPath->text().toStdString() ) )
	{
		ui->status_label->setText( "Banner image path was not valid. Aborting" );
		ui->tabWidget->setCurrentIndex( 0 );
		return;
	}

	auto new_type { [&]()
					{
						switch ( ui->recordType->currentIndex() )
						{
							case 0:
								return RecordType::NORMAL_RECORD;
							case 1:
								return RecordType::GAME_RECORD;
							case 2:
								[[fallthrough]];
							case 3:
								[[fallthrough]];
							default:
								return RecordType::NORMAL_RECORD;
						}
					}() };


	switch ( new_type )
	{
		default:
			[[fallthrough]];
		case RecordType::NORMAL_RECORD:
		{
			break;
		}
		case RecordType::GAME_RECORD:
		{
			//Check executable path
			QFile file { ui->execPath->text() };
			QFileInfo info { file };

			if ( !file.exists() )
			{
				ui->status_label->setText( "Executable path invalid" );
				ui->tabWidget->setCurrentIndex( 3 );
				return;
			}

			if ( !info.isExecutable() )
			{
				ui->status_label->setText( "File provided as exe is not an executable" );
				ui->tabWidget->setCurrentIndex( 3 );
				return;
			}

			if ( !file.permissions().testFlag( QFileDevice::ExeUser ) )
			{
				ui->status_label->setText( "Unable to execute provided executable. Check permissions." );
				ui->tabWidget->setCurrentIndex( 3 );
				return;
			}
			break;
		}
	}


	if ( ui->recordName->text() != m_record->getName() ) m_record->setName( ui->recordName->text() );
	if ( ui->descText->toPlainText() != m_record->getDesc() )
		m_record->setDescription( ui->descText->toPlainText() );
	if ( ui->noteText->toPlainText() != m_record->getNotes() ) m_record->setNotes( ui->noteText->toPlainText() );

	switch ( new_type )
	{
		case RecordType::GAME_RECORD:
		{


			break;
		}
		case RecordType::NORMAL_RECORD: [[fallthrough]];
		default:
			break;
	}

	this->accept();
}

void RecordEditor::on_cancel_clicked()
{
	this->close();
}
