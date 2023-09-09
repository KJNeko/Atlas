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
	QStringList parseFileName( QString s );
	bool isDigit( QString &s );
	bool checkOsNames( QString s );
	QString addSpaces( QString s );
	QStringList findVersionType( QString s );
	QStringList parseStringByDelimiter( QString s, QString delimiter );
	bool checkLanguages( QString s );

  private slots:
	void on_btnSetRoot_pressed();
	void on_btnSetExtPath_pressed();
	void on_btnNext_pressed();

  private:
    Ui::ExtractionImportDialog *ui;
};

#endif // EXTRACTIONIMPORTDIALOG_HPP
