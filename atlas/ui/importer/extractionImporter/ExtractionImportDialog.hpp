#ifndef EXTRACTIONIMPORTDIALOG_HPP
#define EXTRACTIONIMPORTDIALOG_HPP

#include <QDialog>

namespace Ui
{
	class ExtractionImportDialog;
}

class ExtractionImportDialog : public QDialog
{
	Q_DISABLE_COPY_MOVE( ExtractionImportDialog )
	Q_OBJECT

  public:

	explicit ExtractionImportDialog( QWidget* parent = nullptr );
	~ExtractionImportDialog();

  private:

	Ui::ExtractionImportDialog* ui;
};

#endif // EXTRACTIONIMPORTDIALOG_HPP
