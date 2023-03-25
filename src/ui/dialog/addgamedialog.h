#ifndef ADDGAMEDIALOG_H
#define ADDGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class addGameDialog;
}

class addGameDialog : public QDialog
{
    Q_DISABLE_COPY_MOVE(addGameDialog)
    Q_OBJECT

public:
    explicit addGameDialog(QWidget *parent = nullptr);
    ~addGameDialog();

private:
    Ui::addGameDialog *ui;
};

#endif // ADDGAMEDIALOG_H
