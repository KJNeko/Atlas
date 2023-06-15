//
// Created by kj16609 on 6/6/23.
//

/*
RemoteDownloader::RemoteDownloader() :
  m_last_check( std::chrono::system_clock::from_time_t( static_cast< time_t >( config::remote::last_check::get() ) ) ),
  m_check_rate( static_cast< size_t >( config::remote::check_rate::get() ) )
{
	check_timer.setInterval( std::chrono::minutes( 1 ) );
	check_timer.setSingleShot( false );
	check_timer.start();
	connect( &check_timer, &QTimer::timeout, this, &RemoteDownloader::checkRemote );
	connect( &manager, &QNetworkAccessManager::finished, this, &RemoteDownloader::processResponse );
}

void RemoteDownloader::checkRemote()
{
	if ( std::chrono::system_clock::now() < nextCheck() ) return;

	using namespace std::chrono_literals;
	if ( m_last_check.time_since_epoch().count() == 0 )
	{
		QUrl url { REMOTE "packages" };
		QNetworkRequest request { url };
		manager.get( request );
	}
}


void RemoteDownloader::processResponse( QNetworkReply* reply )
{
	ZoneScoped;

	if(reply->error() != QNetworkReply::NoError)
	{
		spdlog::error( "Error processing response {}: {}", reply->url().toString().toStdString(), reply->errorString().toStdString() );
		return;
	}

	//Check if the response is for the versions json or a download
}*/
