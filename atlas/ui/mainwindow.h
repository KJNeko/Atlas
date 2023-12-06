
#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QTreeWidget>

#include "core/database/Search.hpp"
#include "core/utils/fileDownloader.hpp"

#ifdef _WIN32
#include "core/updater/AtlasUpdater.hpp"
#include "ui/dialog/console/Console.hpp"
#endif
QT_BEGIN_NAMESPACE

namespace Ui
{
	class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
	Q_DISABLE_COPY_MOVE( MainWindow )
	Q_OBJECT

	QThread search_thread {};
	Search record_search {};

  public:

	MainWindow( QWidget* parent = nullptr );
	Console* console = new Console();
	~MainWindow();

  private:

	Ui::MainWindow* ui;

	void resizeEvent( QResizeEvent* event ) override;
	void showEvent( QShowEvent* event ) override;
	void moveEvent( QMoveEvent* event ) override;
	void keyPressEvent( QKeyEvent* event ) override;

	void closeEvent( QCloseEvent* event ) override;
	void readSettings();

  signals:
	//void triggerSearch( QString text, const SortOrder order, const bool asc );
	void triggerSearch( QString title );

  private slots:
	void on_actionSimpleImporter_triggered();
	void on_actionBulkImporter_triggered();
	void on_actionSingleImporter_triggered();
	void on_actionExtractionImporter_triggered();
	void on_actionUpdates_triggered();
	void on_actionOptions_triggered();
	void on_actionExit_triggered();
	void on_actionConsoleWindow_triggered();
	void on_actionUpdateMeta_triggered();
	//void on_actionDownload_triggered();
	//void on_actionManage_triggered();
	//void on_actionArrangeBy_triggered();
	//void on_actionAboutUs_triggered();
	//void on_actionGithub_triggered();
	void on_actionAboutQt_triggered();
	void on_actionAboutAtlas_triggered();
	void switchToDetailed( const atlas::records::Game record );
	void on_homeButton_pressed();
	void on_actionViewFileHistory_triggered();
	void on_btnAddGame_pressed();
	void on_btnFilter_pressed();
	void on_btnLog_pressed();
	void searchTextChanged( const QString str );
	//void on_sortOrderButton_clicked();
	//void on_sortSelection_currentIndexChanged( int index );
	void movePopup();
	void taskPopupResized();
	void setBottomGameCounter();
	void refreshSearch();
	void on_stackedWidget_currentChanged( const int idx );
	void loadImage( const FileDownloader* fdownloader, RecordID id );
};

#endif // MAINWINDOW_H
