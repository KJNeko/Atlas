//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_MAINWINDOW_HPP
#define HYDRUS95_MAINWINDOW_HPP

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( MainWindow )

	public:
	explicit MainWindow( QWidget* parent = nullptr );
	~MainWindow() override;

	private:
	Ui::MainWindow* ui;

	private slots:
	void on_actionCreateRecord_triggered();
	void on_actionSettings_triggered();
};


#endif	//HYDRUS95_MAINWINDOW_HPP
