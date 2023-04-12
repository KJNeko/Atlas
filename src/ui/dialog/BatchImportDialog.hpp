#ifndef BATCHIMPORTDIALOG_H
#define BATCHIMPORTDIALOG_H

#include <QDialog>

#include "atlas/threading/ImportPreProcessor.hpp"
#include "atlas/threading/ImportProcessor.hpp"

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

  public slots:
	void reject() override;

  private slots:
	void on_btnSetFolder_pressed();
	void on_btnNext_pressed();
	void on_btnBack_pressed();
	void on_btnCancel_pressed();
	void modelChanged(
		const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList< int >& roles = QList< int >() );
	void processFinishedDirectory( const std::vector< GameImportData > data );
	void finishedPreProcessing();
	void finishedImporting();
	void importFailure( const QString top, const QString bottom );

  signals:
	void startProcessingDirectory( const QString regex, const std::filesystem::path path, const bool skip_filesize );
	void addToModel( const std::vector< GameImportData > data );
	void startImportingGames(
		const std::vector< GameImportData > data, const std::filesystem::path source, const bool move_after_import );
	void importComplete( std::vector< RecordID > records );
	void unpauseImport();
};

#endif // BATCHIMPORTDIALOG_H
