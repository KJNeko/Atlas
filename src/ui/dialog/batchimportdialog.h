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


  private slots:
	void on_btnSetFolder_pressed();
	void on_btnNext_pressed();
	void on_btnBack_pressed();
	void processFiles();
};

#endif // BATCHIMPORTDIALOG_H
