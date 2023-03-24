
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_DISABLE_COPY_MOVE(MainWindow)
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void addTreeRoot(QString name, QString record_id);
    void addTreeChild(QTreeWidgetItem *parent,
                      QString name, QString record_id);
    void openBatchImportDialog();

private slots:
    void on_actionImport_triggered();
};

#endif // MAINWINDOW_H
