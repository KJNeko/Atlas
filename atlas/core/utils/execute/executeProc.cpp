
#include "executeProc.hpp"

#include <QDebug>
#include <QDesktopServices>
#include <QFuture>
#include <QProcess>
#include <QString>
#include <QUrl>
#include <QtCore>

#include "core/logging.hpp"

QFuture< int > executeProc( const QString& path )
{
	spdlog::debug( "Running {}", path.toStdString() );
	//temp fix to test html games
	if ( path.contains( "html" ) )
	{
		spdlog::info( "Trying to open html game" );
		//const QUrl uri { path };
		QDesktopServices::openUrl( QUrl::fromLocalFile( path ) );
	}
	QProcess* process { new QProcess() };

	process->start( path );

	if ( !process->waitForStarted() ) spdlog::error( "Failed to start executable at {}", path );

	//Wait for finished signal emitted from QProcess
	QPromise< int >* promise { new QPromise< int > {} };
	QFuture< int > future { promise->future() };

	QProcess::connect(
		process,
		&QProcess::finished,
		[ promise, process ]()
		{
			promise->addResult( process->exitCode() );
			promise->finish();
			process->deleteLater();
			spdlog::debug( "Process finished" );
		} );

	return future;
}
