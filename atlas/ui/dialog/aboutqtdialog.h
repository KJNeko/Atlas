#ifndef ABOUTQTDIALOG_H
#define ABOUTQTDIALOG_H

#include <QDialog>

namespace Ui {
	class AboutQtDialog;
}

class AboutQtDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit AboutQtDialog(QWidget *parent = nullptr);
	~AboutQtDialog();

  private:
	Ui::AboutQtDialog *ui;
};

#endif // ABOUTQTDIALOG_H
