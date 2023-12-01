#include "AtlasUpdater.hpp"

#include <moc_AtlasUpdater.cpp>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>  
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QProcess>
#include <QCoreApplication>

#include "core/logging/logging.hpp"
#include "core/version.hpp"
#include "core/config/config.hpp"

#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#endif

#define REPO "https://api.github.com/repos/KJNeko/Atlas"

//#define REMOTE_BACKUP "https://futa.zip/atlas"

namespace atlas
{
	namespace internal
	{
		inline static AtlasUpdater* uManager { nullptr };
	}
    void initUpdateHandler(bool isManual)
	{
		if ( internal::uManager == nullptr ) internal::uManager = new AtlasUpdater();
		

		internal::uManager->check_for_updates(isManual);
	}

	void shutdownUpdateHandler()
	{
		delete internal::uManager;
	}

	AtlasUpdater::AtlasUpdater()
	{
		/*m_manager.moveToThread( &m_thread );
		moveToThread( &m_thread );
		m_thread.start();*/
	}

	AtlasUpdater::~AtlasUpdater()
	{
		ud->deleteLater();
	}

    void AtlasUpdater::check_for_updates(bool isManual)
    {
      	QUrl url("https://api.github.com/repos/KJNeko/Atlas/releases");
		atlas::logging::info( "Checking github for updates at {}", url.toString() );
		QNetworkRequest request { url};
		request.setTransferTimeout( 2000 );
		auto* reply { m_manager.get( request ) };

		connect(
			reply,
			&QNetworkReply::finished,
			this,
			[ =, this ]() { handleJsonResponse( reply, isManual ); },
			Qt::SingleShotConnection );

		connect(
			reply,
			&QNetworkReply::errorOccurred,
			this,
			[ =, this ]( const QNetworkReply::NetworkError& error ) { handleManifestError( error, reply ); },
			Qt::SingleShotConnection );
    }

	void AtlasUpdater::handleJsonResponse( QNetworkReply* reply, bool isManual )
	try
	{
		atlas::logging::debug( "Handling json response from {}", reply->url().path().toStdString() );
		//Read the json

		if ( reply->error() != QNetworkReply::NoError )
		{
			logging::warn(
				"Failed to handle json response from {}. Server returned code {}",
				reply->url().path().toStdString(),
				reply->errorString().toStdString() );
			return;
		}

		const QString response_data { reply->readAll() };
		//qInfo() << response_data;
		QJsonDocument jsonResponse  { QJsonDocument::fromJson( response_data.toUtf8() ) };

		

		if ( !jsonResponse.isArray() )
		{
			logging::warn(
				"Failed to handle json response from {}. The json response was not an array!",
				reply->url().path().toStdString() );
			logging::warn( "{}", response_data.toStdString() );
		}
		else{
			QJsonObject jsonObject = jsonResponse.object();
			const QJsonArray& array = jsonResponse.array();

			const QString branch = QString::
				fromLocal8Bit( utils::git_branch.data(), static_cast< qsizetype >( utils::git_branch.size() ) );

			const long int buildtime { converToShortEpoch( QString::fromLocal8Bit(
				utils::git_time.data(), static_cast< qsizetype >( utils::git_time.size() ) ) ) };
			struct release{
				QString tag_name;
				long int created_at;
				QString target_commitish;
				QString browser_download_url;
			};

			std::vector<release> releases;

			//Check that we are not on a dev branch
			if(branch == "master" || branch == "staging")
			{
				if(true)
				{
					int last_unix_ts = buildtime;
					for ( const auto& data : array )
					{
						const auto& obj { data.toObject() };
						const auto tag_name { obj[ "tag_name" ].toString() };
						const auto created_at { converToShortEpoch(obj[ "created_at" ].toString()) };
						const auto target_commitish { obj[ "target_commitish" ].toString() };
						QString browser_download_url { "" };
						for ( const auto& assets : obj["assets"].toArray())
						{
							const auto& asset { assets.toObject() };
							browser_download_url = asset[ "browser_download_url" ].toString();
						}
						if(created_at > last_unix_ts)
						{						
							if(config::application::update_channel::get() == "nightly")
							{						
								//qInfo() << target_commitish;
								//qInfo() << tag_name;
								//qInfo() <<  created_at << ">" << last_unix_ts;
								releases.push_back( release( tag_name, created_at, target_commitish, browser_download_url ) );
								last_unix_ts = created_at;
							}
							if(config::application::update_channel::get() == "stable" && target_commitish == "master")
							{
								//qInfo() << target_commitish;
								//qInfo() << tag_name;
								//qInfo() << created_at;
								releases.push_back( release( tag_name, created_at, target_commitish, browser_download_url ) );
								last_unix_ts = created_at;
							}

						}
					}

					if(releases.size() > 0)
					{
						const int msgBox {updateMessageBox("A new version of ATLAS is available!\n\nCurrent Version: " + utils::version_string_qt()
								+ "\nLatest Version: " + releases.back().tag_name + "\n\nDo you want to update?","Update Available", false )};

						if( msgBox == QMessageBox::Ok )	
						{
							downloadUpdate( releases.back().browser_download_url );
						}
					}

					if(releases.size() == 0 && isManual){
						updateMessageBox("There is no update Available\n\nCurrent Version: " + utils::version_string_qt(),"No Update Available", false );
					}
				}
			}
			else{			
				//Show box saying there is no update because your on a dev branch. This will only show if done from file menu
				if(isManual){
					
					QMessageBox msgBox;
					msgBox.setWindowTitle( "No Update Available" );
					msgBox.setText("You are on a dev branch. There are no updates for this build\n\nCurrent Branch: " + branch +"\nCurrent Version: " +utils::version_string_qt());
					msgBox.setIcon( QMessageBox::Warning );
					msgBox.exec();
				}
			}
		
		}
		reply->deleteLater();

	}
	catch ( const std::exception& e )
	{
		atlas::logging::error(
			"Failed to handle json response from {}. Exception: {}", reply->url().path().toStdString(), e.what() );
	}


	void AtlasUpdater::downloadUpdate(QString url)
{
	#ifdef _WIN32

	QNetworkRequest request { url };
	request.setTransferTimeout( 2000 );
	auto* reply { m_manager.get( request ) };
	//reply->deleteLater();

	connect(
		reply, &QNetworkReply::finished, this, [ =, this ]() { saveFile( reply ); }, Qt::SingleShotConnection );

	connect(
		reply,
		&QNetworkReply::errorOccurred,
		this,
		[ =, this ]( const QNetworkReply::NetworkError& error ) { handleManifestError( error, reply ); },
		Qt::SingleShotConnection );
	connect( 
		reply, 
		&QNetworkReply::downloadProgress, 
		this, &downloadProgress );
	#endif
}

	void AtlasUpdater::saveFile(QNetworkReply* reply)
	{
		//Close Dialog
		ud->close();
		QFile file(QString::fromStdString(std::string( std::getenv( "APPDATA" )) + "\\ATLAS\\update.zip"));
		std::filesystem::create_directory(std::string( std::getenv( "APPDATA" )) + "\\ATLAS");
		file.open(QIODevice::WriteOnly);
    	file.write(reply->readAll());
		//qInfo() << QString::fromStdString( std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\update.zip" );
		//qInfo() << "FILE DOWNLOADED";
		//qInfo() << "App path : " << QString::fromStdString(std::filesystem::current_path().string());
		#ifdef _WIN32

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        //Wide string because windows is stupid
        const std::string path = "updater/AtlasUpdater.exe";
		const std::string args = "0"; //Default, DO NOT SHOW GUI
		//std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\update.zip" + std::to_string( ::getpid() );

		//qInfo() << QString::fromStdString(std::to_string( ::getpid() ));

		char* win_buffer { new char[args.size()] } ;
        std::memcpy(win_buffer, args.c_str(), args.size());
    
        if (CreateProcessA(path.c_str(), win_buffer, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        {
			QCoreApplication::quit(); //Kill QT application
			exit(0); //Kill Program
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

        }
		//QProcess *process = new QProcess(this);
		QString command { QString::fromStdString(std::string( std::getenv( "APPDATA" )) + "\\ATLAS\\update.zip") + " " + QString::number(getpid()) };
		//process->startDetached( "AtlasUpdater.exe", QStringList(command));


		/*QString command { "stop-process -name Atlas ; Start-Sleep -Seconds 3; Expand-Archive -Force " + file.fileName()
			              + " " + QString::fromStdString( std::filesystem::current_path().string() ) };
		//qInfo() << command;
		QProcess *process = new QProcess(this);
		process->startDetached("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe", QStringList(command));*/
		#endif

	}

	void AtlasUpdater::handleManifestError( QNetworkReply::NetworkError error, QNetworkReply* reply )
	{
		reply->deleteLater();
		atlas::logging::error( "Failed to download manifest from remote: {}", static_cast< int >( error ) );
		switch ( error )
		{
			default:
				[[fallthrough]];
			case QNetworkReply::NoError:
				break;
			case QNetworkReply::ConnectionRefusedError:
				break;
			case QNetworkReply::RemoteHostClosedError:
				break;
			case QNetworkReply::HostNotFoundError:
				break;
			case QNetworkReply::TimeoutError:
				atlas::logging::error( "Timed out" );
				break;
			case QNetworkReply::OperationCanceledError:
				break;
			case QNetworkReply::SslHandshakeFailedError:
				break;
			case QNetworkReply::TemporaryNetworkFailureError:
				break;
			case QNetworkReply::NetworkSessionFailedError:
				break;
			case QNetworkReply::BackgroundRequestNotAllowedError:
				break;
			case QNetworkReply::TooManyRedirectsError:
				break;
			case QNetworkReply::InsecureRedirectError:
				break;
			case QNetworkReply::UnknownNetworkError:
				break;
			case QNetworkReply::ProxyConnectionRefusedError:
				break;
			case QNetworkReply::ProxyConnectionClosedError:
				break;
			case QNetworkReply::ProxyNotFoundError:
				break;
			case QNetworkReply::ProxyTimeoutError:
				break;
			case QNetworkReply::ProxyAuthenticationRequiredError:
				break;
			case QNetworkReply::UnknownProxyError:
				break;
			case QNetworkReply::ContentAccessDenied:
				break;
			case QNetworkReply::ContentOperationNotPermittedError:
				break;
			case QNetworkReply::ContentNotFoundError:
				break;
			case QNetworkReply::AuthenticationRequiredError:
				break;
			case QNetworkReply::ContentReSendError:
				break;
			case QNetworkReply::ContentConflictError:
				break;
			case QNetworkReply::ContentGoneError:
				break;
			case QNetworkReply::UnknownContentError:
				break;
			case QNetworkReply::ProtocolUnknownError:
				break;
			case QNetworkReply::ProtocolInvalidOperationError:
				break;
			case QNetworkReply::ProtocolFailure:
				break;
			case QNetworkReply::InternalServerError:
				break;
			case QNetworkReply::OperationNotImplementedError:
				break;
			case QNetworkReply::ServiceUnavailableError:
				break;
			case QNetworkReply::UnknownServerError:
				break;
		}
	}

	long int AtlasUpdater::converToEpoch(QString time){
		std::tm t = {};
		std::istringstream ss(time.toStdString());

		if (ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
		{
			return static_cast<long int>(std::mktime( &t ));
		}
		else
		{
			return 0;
		}
	}

	long int AtlasUpdater::converToShortEpoch(QString time){
		std::tm t = {};
		std::istringstream ss(time.toStdString());

		if (ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
		{
			const long int unix_time {static_cast<long int>(std::mktime( &t ))};
			return QString::number( unix_time ).left( QString::number( unix_time ).length() - 3 ).toInt();
		}
		else
		{
			return 0;
		}
	}

	int AtlasUpdater::updateMessageBox(QString message, QString windowTitle, bool includeChangelog)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(windowTitle);
		msgBox.setText( message );
		msgBox.setIcon( QMessageBox::Question );
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
		if(includeChangelog)
		{
			msgBox.setDetailedText( "Changelog" );
		}
		QSpacerItem* horizontalSpacer = new QSpacerItem(450, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
		QGridLayout* layout = static_cast<QGridLayout*>(msgBox.layout());
		layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
	
		return msgBox.exec();
	}

	void AtlasUpdater::downloadProgress(int ist, int max)
	{
		ud->show();
		ud->UpdateProgressBar( static_cast<int>((100.0/max) *ist));
	}
}