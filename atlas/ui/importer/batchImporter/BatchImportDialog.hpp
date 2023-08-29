#ifndef BATCHIMPORTDIALOG_H
#define BATCHIMPORTDIALOG_H

#include <QDialog>
#include <QThreadPool>

#include "core/Types.hpp"
#include "core/import/GameScanner.hpp"

namespace Ui
{
	class BatchImportDialog;
}

class BatchImportDialog final : public QDialog
{
	Q_DISABLE_COPY_MOVE( BatchImportDialog )
	Q_OBJECT

  public:

	explicit BatchImportDialog( QWidget* parent = nullptr );
	~BatchImportDialog();

  private:

	GameScanner scanner {};
	Ui::BatchImportDialog* ui;
	bool search_started { false };
	bool import_triggered { false };
	//! If true then the main runner has finished and we are waiting on the preprocessing of the other threads.
	bool import_waiting { false };

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
	void processFinishedDirectory( const GameImportData data );
	void finishedPreProcessing();
	void importFailure( const QString top, const QString bottom );
	void waitingOnThreads();

  signals:
	void addToModel( const GameImportData data );
	void startImportingGames(
		const std::vector< GameImportData > data, const std::filesystem::path source, const bool move_after_import );
	void importComplete( std::vector< RecordID > records );
	void importComplete(); //Easier to use since we dont need to pass anything to main ui
	void unpauseImport();
};

#endif // BATCHIMPORTDIALOG_H
