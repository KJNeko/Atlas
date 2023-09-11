#include "ExtractionImportDialog.hpp"

#include "ui_ExtractionImportDialog.h"

ExtractionImportDialog::ExtractionImportDialog( QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::ExtractionImportDialog )
{
	ui->setupUi( this );
}

ExtractionImportDialog::~ExtractionImportDialog()
{
	delete ui;
}
