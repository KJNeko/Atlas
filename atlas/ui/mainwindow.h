
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTreeWidget>

#include "core/database/Search.hpp"
#include "ui/tasks/TaskPopup.hpp"

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

	void addTreeRoot( QString name, QString record_id );
	void addTreeChild( QTreeWidgetItem* parent, QString name, QString record_id );
	void openBatchImportDialog();
	void resizeEvent( QResizeEvent* event ) override;
	void showEvent( QShowEvent* event ) override;
	void moveEvent( QMoveEvent* event ) override;

  signals:
	void triggerSearch( QString text, const SortOrder order, const bool asc );

  private slots:
	void on_actionImport_triggered();
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
	void on_btnShowMessageLog_clicked();
	void movePopup();
	void taskPopupResized();
};

#endif // MAINWINDOW_H
