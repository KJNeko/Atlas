#include "Console.hpp"

#include "ConsoleWriter.hpp"
#include "moc_Console.cpp"
#include "ui_Console.h"

Console::Console( QWidget* parent ) : QDialog( parent ), ui( new Ui::Console )
{
	ui->setupUi( this );

	connect(
		atlas::logging::getConsoleWriter(), &atlas::logging::ConsoleWriter::dataWritten, this, &Console::writeData );

	atlas::logging::getConsoleWriter()->reprintMessages();
}

Console::~Console()
{
	delete ui;
}

void Console::writeData( const QString& message )
{
	ui->textEdit->append( message );
}
