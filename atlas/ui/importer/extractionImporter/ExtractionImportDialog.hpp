#ifndef EXTRACTIONIMPORTDIALOG_HPP
#define EXTRACTIONIMPORTDIALOG_HPP

#include <QDialog>
#include <QThreadPool>

#include "core/Types.hpp"

namespace Ui {
class ExtractionImportDialog;
}

class ExtractionImportDialog : public QDialog
{
    Q_DISABLE_COPY_MOVE( ExtractionImportDialog )
    Q_OBJECT

public:
    explicit ExtractionImportDialog(QWidget *parent = nullptr);
    ~ExtractionImportDialog();
	void parseFiles(std::string path);
	QString parseTitle( QString title );
	QString parseVersion( QString version );
	bool isDigit( QString &s );
	bool checkOsNames( QString s );

  private slots:
	void on_btnSetRoot_pressed();
	void on_btnSetExtPath_pressed();
	void on_btnNext_pressed();

  private:
    Ui::ExtractionImportDialog *ui;
};

#endif // EXTRACTIONIMPORTDIALOG_HPP
