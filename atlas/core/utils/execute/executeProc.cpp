
#include "executeProc.hpp"

#include <QDebug>
#include <QDesktopServices>
#include <QFuture>
#include <QProcess>
#include <QString>
#include <QUrl>
#include <QtCore>

#include "core/database/record/Game.hpp"
#include "core/database/record/Version.hpp"
#include "core/logging/logging.hpp"

inline static QProcess* running_process { nullptr };
inline static bool terminating { false };

void executeProc( const RecordID game_id, const QString version, const QString& path )
{
	atlas::logging::debug( fmt::format( "Running {}", path.toStdString() ) );
	//temp fix to test html games
	if ( path.contains( "html" ) )
	{
		atlas::logging::debug( "Trying to open html game" );
		//const QUrl uri { path };
		QDesktopServices::openUrl( QUrl::fromLocalFile( path ) );
	}

	const auto start_time { std::chrono::steady_clock::now() };
	;

	running_process = new QProcess();
	running_process->start( path );

	if ( !running_process->waitForStarted() )
		atlas::logging::error( fmt::format( "Failed to start executable at {}", path ) );

	QProcess::connect(
		running_process,
		&QProcess::finished,
		[ = ]()
		{
			atlas::records::Game game { game_id };
			atlas::records::Version& g_version { game[ version ] };

			const auto stop_time { std::chrono::steady_clock::now() };
			const auto time_diff { stop_time - start_time };

			g_version.addPlaytime( time_diff );

			running_process->deleteLater();
			running_process = nullptr;
			terminating = false;
		} );

	return;
}

bool processIsRunning()
{
	return running_process != nullptr;
}

void softTerminateProcess()
{
	running_process->terminate();
	running_process->waitForFinished( 15000 );
}

void hardTerminateProcess()
{
	running_process->kill();
	running_process->waitForFinished( 15000 );
}
