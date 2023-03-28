#ifndef BATCHIMPORTDIALOG_H
#define BATCHIMPORTDIALOG_H

#include <QDialog>

#include "h95/threading/ImportProcessor.hpp"

namespace Ui
{
	class batchImportDialog;
}

class batchImportDialog : public QDialog
{
	Q_DISABLE_COPY_MOVE( batchImportDialog )
	Q_OBJECT

	ImportProcessor processor {};

  public:

	explicit batchImportDialog( QWidget* parent = nullptr );
	~batchImportDialog();

  private:
	Ui::batchImportDialog* ui;

	void loadConfig();
	void saveConfig();

  private slots:
	void on_btnSetFolder_pressed();
	void on_btnNext_pressed();
	void on_btnBack_pressed();
	void processFiles();
	void modelChanged(
		const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList< int >& roles = QList< int >() );
	void processFinishedDirectory(const GameImportData data);
	void finishedProcessing();

  signals:
	void startProcessingDirectory( const QString regex, const std::filesystem::path path, const bool move_imported, const bool skip_filesize );
	void addToModel(const GameImportData data);
};

#endif // BATCHIMPORTDIALOG_H
