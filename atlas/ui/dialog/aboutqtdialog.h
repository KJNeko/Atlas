
#pragma once
#ifndef ABOUTQTDIALOG_H
#define ABOUTQTDIALOG_H

#include <QDialog>

namespace Ui
{
	class AboutQtDialog;
}

class AboutQtDialog final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( AboutQtDialog )

  public:

	explicit AboutQtDialog( QWidget* parent = nullptr );
	~AboutQtDialog();

  private:

	Ui::AboutQtDialog* ui;
};

#endif // ABOUTQTDIALOG_H
