//
// Created by towerwatchman on 01/24/24.
//

#include "extractor.hpp"

#include <bit7z/bitarchivereader.hpp>
#include <bit7z/bitfileextractor.hpp>

namespace atlas::utils
{

	bool Extractor::extractArchive( GameImportData game )
	{
		m_signaler = std::make_unique< ProgressSignaler >();
		m_signaler->setMessage( "Extracting: " + game.title );
		m_signaler->setSubMessage( "test" );
		m_signaler->setProgress( 0 );
		m_signaler->setMax( 0 );

		try
		{ // bit7z classes can throw BitException objects
			using namespace bit7z;

			Bit7zLibrary lib { "7z.dll" };
			BitFileExtractor extractor { lib, BitFormat::Auto };
			uint64_t data_size {};

			//Get the total size
			extractor.setTotalCallback( [ &data_size ]( const uint64_t total_size ) { data_size = total_size; } );
			qInfo() << data_size;

			extractor.setProgressCallback(
				[ &data_size, this ]( uint64_t processed_size )
				{
					auto progress = static_cast< int >( ( 100.0 * processed_size ) / data_size );
					extractionProgress( progress, 100 );
					std::cout << progress << std::endl;
					return true;
				} );
			extractor.setFileCallback( [ this ]( std::string file_path ) { fileProgress( file_path ); } );

			// Extracting a simple archive
			qInfo() << "Archive Path: " << game.archive_path
					<< "\nRelative Path: " << QString::fromStdString( game.relative_path.string() );
			//CREATE DIR FIRST
			if ( std::filesystem::create_directories( game.relative_path ) )
			{
				//Extract files
				extractor.extract( game.archive_path.toStdString(), game.relative_path.string() );
				//Check if there is a single folder in dir. If there is, move all files one folder up
				std::string folder_name = game.archive_path.split( "/" )[ game.archive_path.split( "/" ).count() ]
				                              .split( "." )[ 0 ]
				                              .toStdString();

				if ( std::filesystem::exists( game.relative_path / folder_name ) )
				{
					qInfo() << QString::fromStdString( folder_name );
				}
			}
			else
			{
				qInfo() << "Unable to create path ";
			}
			this->deleteLater();
			return true;
		}
		catch ( const bit7z::BitException& ex )
		{
			std::cout << ex.what();
			this->deleteLater();
			return false;
		}
	}

	void Extractor::extractionProgress( qint64 bytesReceived, qint64 bytesTotal )
	{
		m_signaler->setProgress( bytesReceived );
		m_signaler->setMax( bytesTotal );
	}

	void Extractor::fileProgress( std::string file )
	{
		m_signaler->setSubMessage( "Current file: " + QString::fromStdString( file ) );
	}
} // namespace atlas::utils
