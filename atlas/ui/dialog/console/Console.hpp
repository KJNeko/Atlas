
#pragma once
#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <QDialog>

namespace Ui
{
	class Console;
}

class Console : public QDialog
{
	Q_DISABLE_COPY_MOVE( Console )
	Q_OBJECT

  public:

	explicit Console( QWidget* parent = nullptr );
	~Console();

  public slots:
	void writeData( const QString& message );

  private:

	Ui::Console* ui;
};

#endif // CONSOLE_HPP
