#ifndef BATCHIMPORTDIALOG_H
#define BATCHIMPORTDIALOG_H

#include <QDialog>

#include "h95/threading/ImportPreProcessor.hpp"
#include "h95/threading/ImportProcessor.hpp"

namespace Ui
{
	class BatchImportDialog;
}

class BatchImportDialog : public QDialog
{
	Q_DISABLE_COPY_MOVE( BatchImportDialog )
	Q_OBJECT

	QThread processing_thread {};
	ImportPreProcessor preprocessor {};
	ImportProcessor processor {};
	ProgressBarDialog progress { this };

  public:

	explicit BatchImportDialog( QWidget* parent = nullptr );
	~BatchImportDialog();

  private:

	Ui::BatchImportDialog* ui;

	void loadConfig();
	void saveConfig();
	void importFiles();
	void processFiles();

  private slots:
	void on_btnSetFolder_pressed();
	void on_btnNext_pressed();
	void on_btnBack_pressed();
	void on_btnCancel_pressed();
	void modelChanged(
		const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList< int >& roles = QList< int >() );
	void processFinishedDirectory( const GameImportData data );
	void finishedProcessing();
	void finishedImporting();

  signals:
	void startProcessingDirectory(
		const QString regex, const std::filesystem::path path, const bool move_imported, const bool skip_filesize );
	void addToModel( const GameImportData data );
	void startImportingGames( const std::vector< GameImportData > data, const std::filesystem::path source );
	void importComplete( std::vector< RecordID > records );
};

#endif // BATCHIMPORTDIALOG_H
