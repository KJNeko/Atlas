#include "batchimportdialog.h"
#include "ui_batchimportdialog.h"

batchImportDialog::batchImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::batchImportDialog)
{
    ui->setupUi(this);
}

batchImportDialog::~batchImportDialog()
{
    delete ui;
}
