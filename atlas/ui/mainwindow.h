
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTreeWidget>

#include "core/database/Search.hpp"
QT_BEGIN_NAMESPACE

namespace Ui
{
	class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_DISABLE_COPY_MOVE( MainWindow )
	Q_OBJECT

	QThread search_thread {};
	Search record_search {};

  public:

	MainWindow( QWidget* parent = nullptr );
	~MainWindow();

  private:

	Ui::MainWindow* ui;

	void openBatchImportDialog();
	void resizeEvent( QResizeEvent* event ) override;
	void showEvent( QShowEvent* event ) override;
	void moveEvent( QMoveEvent* event ) override;

  signals:
	void triggerSearch( QString text, const SortOrder order, const bool asc );
	void triggerReSearch();

  private slots:
	void on_actionSimpleImporter_triggered();
	void on_actionBulkImporter_triggered();
	void on_actionGameListImporter_triggered();
	void on_actionSingleImporter_triggered();
	//void on_actionImport_triggered();
	void on_actionOptions_triggered();
	void on_actionExit_triggered();
	//void on_actionDownload_triggered();
	//void on_actionManage_triggered();
	//void on_actionArrangeBy_triggered();
	//void on_actionAboutUs_triggered();
	//void on_actionGithub_triggered();
	void on_actionAboutQt_triggered();
	void switchToDetailed( const Record record );
	void on_homeButton_pressed();
	void on_actionViewFileHistory_triggered();
	void searchTextChanged( const QString str );
	void on_sortOrderButton_clicked();
	void on_sortSelection_currentIndexChanged( int index );
	void showMessagePopup();
	void hideMessagePopup();
	void on_btnShowMessageLog_clicked();
	void movePopup();
	void taskPopupResized();
};

#endif // MAINWINDOW_H
