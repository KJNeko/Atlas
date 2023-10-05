#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDialog>

namespace Ui {
class Console;
}

class Console : public QDialog
{
    Q_DISABLE_COPY_MOVE( Console )
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();

private:
    Ui::Console *ui;
};

#endif // CONSOLE_H
