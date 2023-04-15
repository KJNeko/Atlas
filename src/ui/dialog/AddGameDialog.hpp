#ifndef ADDGAMEDIALOG_H
#define ADDGAMEDIALOG_H

#include <QDialog>

namespace Ui
{
	class AddGameDialog;
}

class AddGameDialog : public QDialog
{
	Q_DISABLE_COPY_MOVE( AddGameDialog )
	Q_OBJECT

  public:

	explicit AddGameDialog( QWidget* parent = nullptr );
	~AddGameDialog();

  private:

	Ui::AddGameDialog* ui;
};

#endif // ADDGAMEDIALOG_H
