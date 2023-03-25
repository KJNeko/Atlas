#ifndef BATCHIMPORTDIALOG_H
#define BATCHIMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class batchImportDialog;
}

class batchImportDialog : public QDialog
{
    Q_DISABLE_COPY_MOVE(batchImportDialog)
    Q_OBJECT

public:
    explicit batchImportDialog(QWidget *parent = nullptr);
    ~batchImportDialog();

private:
    Ui::batchImportDialog *ui;
};

#endif // BATCHIMPORTDIALOG_H
