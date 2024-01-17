
#pragma once
#ifndef EXTRACTIONIMPORTDIALOG_HPP
#define EXTRACTIONIMPORTDIALOG_HPP

#include <QDialog>
#include <QThreadPool>

#include "core/Types.hpp"

namespace Ui
{
	class ExtractionImportDialog;
}

class ExtractionImportDialog final : public QDialog
{
	Q_DISABLE_COPY_MOVE( ExtractionImportDialog )
	Q_OBJECT

  public:

	explicit ExtractionImportDialog( QWidget* parent = nullptr );
	~ExtractionImportDialog();

	void parseFiles( const QString& path );
	QStringList parseFileName( const QString& s );
	void updateTable (std::vector<std::filesystem::directory_entry> archives);
	void contextMenuRequested(const QPoint& pos);
	void deleteTableItem(const int row);

  private slots:
	void on_btnSetRoot_pressed();
	void on_btnSetExtPath_pressed();
	void on_btnNext_pressed();


  private:

	Ui::ExtractionImportDialog* ui;
};

#endif // EXTRACTIONIMPORTDIALOG_HPP
