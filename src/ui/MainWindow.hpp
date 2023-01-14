//
// Created by kj16609 on 12/28/22.
//

#pragma once
#ifndef IDHAN_MAINWINDOW_HPP
#define IDHAN_MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( MainWindow )

	public:
	explicit MainWindow( QWidget* parent = nullptr );
	~MainWindow() override;

	private:
	Ui::MainWindow* ui;

	private slots:
	void on_actionCreateEmptyRecord_triggered();
};

#endif	// IDHAN_MAINWINDOW_HPP
