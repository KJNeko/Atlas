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

void RemoteDownloader::parse( const std::filesystem::path path )
try
{
	spdlog::info( "Parsing {:ce}", path );
	ZoneScoped;
	QFile file { QString::fromStdString( path.string() ) };
	file.open( QIODevice::ReadOnly );
	const auto file_data { file.readAll() };

	const QJsonObject json { QJsonDocument::fromJson( file_data ).object() };

	Transaction transaction { NoAutocommit };

	for ( const auto& table_key : json.keys() )
	{
		ZoneScopedN( "Process set" );
		const auto key_str { table_key.toStdString() };
		TracyMessage( key_str.c_str(), key_str.size() );
		const QJsonArray data = json[ table_key ].toArray();

		//Name of the table we intend to shove all this data into.
		const std::string table_name { table_key.endsWith( "_data" ) ? table_key.toStdString() :
			                                                           table_key.toStdString() + "_data" };

		//The query we will be building onto
		const std::string query_start { fmt::format( "INSERT INTO {} (", table_name ) };

		for ( const auto& obj_data : data )
		{
			ZoneScopedN( "Process object" );
			const auto obj { obj_data.toObject() };

			const auto keys { obj.keys() };

			std::string query { query_start };

			//Check if table exists
			bool table_exists { false };
			transaction << "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?;" << table_name
				>> table_exists;

			if ( !table_exists )
			{
				//If the table doesn't exist then we make a temporary table where everything is turned into strings instead.
				//Later on the client (when doing it's startup) can properly check the tables and set them as needed. This should be a rare case.
				//Since this will only really ever happen if the database gets updated before the client is ready to parse (or if a client is out of date)
				std::string table_creation_query { fmt::format( "CREATE TABLE IF NOT EXISTS {} (", table_name ) };

				for ( const auto& key : keys )
				{
					const auto item_type { obj[ key ].type() };

					switch ( item_type )
					{
						case QJsonValue::Null:
							[[fallthrough]];
						case QJsonValue::Bool:
							[[fallthrough]];
						case QJsonValue::Double:
							[[fallthrough]];
						case QJsonValue::String:
							//Later on we can try to 'hint' types. For now this works.
							table_creation_query += fmt::format( "{}", key.toStdString() );
							break;
						case QJsonValue::Array:
							//Ignored since we make the table later on if required.
							break;
						case QJsonValue::Object:
							[[fallthrough]];
						case QJsonValue::Undefined:
							[[fallthrough]];
						default:
							break;
					}
				}

				table_creation_query += ");";

				transaction << table_creation_query;
			}

			for ( int i = 0; i < keys.size(); ++i )
			{
				const auto key { keys[ i ] };
				query += fmt::format( "{}", key.toStdString() );
				if ( i != keys.size() - 1 ) query += fmt::format( "," );
			}
			query += ") VALUES (";
			for ( int i = 0; i < keys.size(); ++i )
			{
				const auto key { keys[ i ] };
				const auto value { obj[ key ] };

				if ( value.isArray() )
				{
					//In this case we use the supporting table
					const std::string supporting_table {
						fmt::format( "{}_data_{}", table_key.toStdString(), key.toStdString() )
					};
					//Does the supporting table exist?
					int count;
					transaction << "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?;"
								<< supporting_table
						>> count;

					if ( count == 0 )
					{
						//Create a table for now.
						transaction << fmt::format(
							"CREATE TABLE {} (id INTEGER PRIMARY KEY REFERENCES {} (id), value);",
							table_name,
							supporting_table );
					}

					//Insert the data into the supporting table
					transaction << fmt::format( "INSERT INTO {} (value) VALUES (?);", supporting_table )
								<< value.toString().toStdString();
				}
				else if ( value.isDouble() )
					query += fmt::format( "{}", value.toInteger() );
				else if ( value.isNull() )
					query += fmt::format( "NULL" );
				else
					query += fmt::format( "\'{}\'", value.toString().replace( '\'', "\'\'" ).toStdString() );
				if ( i != keys.size() - 1 ) query += fmt::format( "," );
			}

			query += ");";

			transaction << query;
		}
	}

	transaction.commit();
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::error( "Error parsing {:ce}: {}: query: {}", path, e.what(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "Error parsing {:ce}: {}", path, e.what() );
}
catch ( ... )
{
	spdlog::error( "Error parsing {:ce}: Unknown error", path );
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
